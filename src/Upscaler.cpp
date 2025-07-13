#include "Upscaler.h"
#include "Config.h"
#include <unordered_set>
#include <filesystem>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace HDUtils {

static std::unordered_set<uint64_t> dumpedTextureHashes;
static std::unordered_set<uint64_t> dumpedSpriteHashes;

bool BilinearUpscale(const std::vector<uint32_t>& input, int width, int height, int scale,
                     std::vector<uint32_t>& output, int& outWidth, int& outHeight)
{
    if (input.empty() || width <= 0 || height <= 0 || scale <= 1)
        return false;
    outWidth = width * scale;
    outHeight = height * scale;
    output.resize(outWidth * outHeight);

    for (int y = 0; y < outHeight; ++y) {
        float fy = static_cast<float>(y) / scale;
        int sy = static_cast<int>(fy);
        float dy = fy - sy;
        if (sy >= height - 1) sy = height - 2;
        for (int x = 0; x < outWidth; ++x) {
            float fx = static_cast<float>(x) / scale;
            int sx = static_cast<int>(fx);
            float dx = fx - sx;
            if (sx >= width - 1) sx = width - 2;

            uint32_t p00 = input[sy * width + sx];
            uint32_t p01 = input[sy * width + sx + 1];
            uint32_t p10 = input[(sy + 1) * width + sx];
            uint32_t p11 = input[(sy + 1) * width + sx + 1];

            uint8_t* c00 = reinterpret_cast<uint8_t*>(&p00);
            uint8_t* c01 = reinterpret_cast<uint8_t*>(&p01);
            uint8_t* c10 = reinterpret_cast<uint8_t*>(&p10);
            uint8_t* c11 = reinterpret_cast<uint8_t*>(&p11);

            uint32_t result = 0;
            uint8_t* res = reinterpret_cast<uint8_t*>(&result);
            for (int ch = 0; ch < 4; ++ch) {
                float top = c00[ch] * (1 - dx) + c01[ch] * dx;
                float bot = c10[ch] * (1 - dx) + c11[ch] * dx;
                res[ch] = static_cast<uint8_t>(top * (1 - dy) + bot * dy);
            }
            output[y * outWidth + x] = result;
        }
    }
    return true;
}

void SavePNG(const std::string& path, const std::vector<uint32_t>& data, int width, int height)
{
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    stbi_write_png(path.c_str(), width, height, 4, data.data(), width * 4);
}

bool LoadPNG(const std::string& path, std::vector<uint32_t>& data, int& width, int& height)
{
    int ch;
    unsigned char* img = stbi_load(path.c_str(), &width, &height, &ch, 4);
    if (!img) return false;
    data.resize(width * height);
    std::memcpy(data.data(), img, width * height * 4);
    stbi_image_free(img);
    return true;
}

static void DumpImage(uint64_t hash, const uint32_t* data, int width, int height,
                      const char* dumpDir, const char* hdDir,
                      bool dumpFlag, bool hdFlag, std::unordered_set<uint64_t>& set)
{
    if (!dumpFlag) return;
    if (set.count(hash)) return;
    set.insert(hash);
    std::vector<uint32_t> buf(data, data + width * height);
    SavePNG(std::string(dumpDir) + "/" + std::to_string(hash) + ".png", buf, width, height);
    if (hdFlag) {
        std::vector<uint32_t> up;
        int w2, h2;
        if (BilinearUpscale(buf, width, height, 4, up, w2, h2))
            SavePNG(std::string(hdDir) + "/" + std::to_string(hash) + ".png", up, w2, h2);
    }
}

void DumpTexture(uint64_t hash, const uint32_t* data, int width, int height)
{
    DumpImage(hash, data, width, height, "dumps/textures", "hd_textures",
              Config::GetGlobalTable().GetBool("HD.DumpTextures"),
              Config::GetGlobalTable().GetBool("HD.CreateHDTextures"),
              dumpedTextureHashes);
}

bool LoadHDTexture(uint64_t hash, std::vector<uint32_t>& data, int& width, int& height)
{
    if (!Config::GetGlobalTable().GetBool("HD.UseHDTextures"))
        return false;
    std::string path = std::string("hd_textures/") + std::to_string(hash) + ".png";
    return LoadPNG(path, data, width, height);
}

void DumpSprite(uint64_t hash, const uint32_t* data, int width, int height)
{
    DumpImage(hash, data, width, height, "dumps/sprites", "hd_sprites",
              Config::GetGlobalTable().GetBool("HD.DumpSprites"),
              Config::GetGlobalTable().GetBool("HD.CreateHDSprites"),
              dumpedSpriteHashes);
}

bool LoadHDSprite(uint64_t hash, std::vector<uint32_t>& data, int& width, int& height)
{
    if (!Config::GetGlobalTable().GetBool("HD.UseHDSprites"))
        return false;
    std::string path = std::string("hd_sprites/") + std::to_string(hash) + ".png";
    return LoadPNG(path, data, width, height);
}

} // namespace HDUtils

