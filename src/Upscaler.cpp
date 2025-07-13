#include "Upscaler.h"
#include <algorithm>
#include <filesystem>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Upscaler
{

bool BilinearUpscale(const std::vector<uint32_t>& input, int width, int height, int scale,
    std::vector<uint32_t>& output, int& outWidth, int& outHeight)
{
    if (input.empty() || width <= 0 || height <= 0 || scale <= 1)
        return false;

    outWidth = width * scale;
    outHeight = height * scale;
    output.resize(outWidth * outHeight);

    for (int y = 0; y < outHeight; y++)
    {
        float fy = static_cast<float>(y) / scale;
        int sy = static_cast<int>(fy);
        float dy = fy - sy;
        sy = std::clamp(sy, 0, height - 1);
        int sy1 = std::clamp(sy + 1, 0, height - 1);

        for (int x = 0; x < outWidth; x++)
        {
            float fx = static_cast<float>(x) / scale;
            int sx = static_cast<int>(fx);
            float dx = fx - sx;
            sx = std::clamp(sx, 0, width - 1);
            int sx1 = std::clamp(sx + 1, 0, width - 1);

            uint32_t p00 = input[sy * width + sx];
            uint32_t p01 = input[sy * width + sx1];
            uint32_t p10 = input[sy1 * width + sx];
            uint32_t p11 = input[sy1 * width + sx1];

            uint8_t* c00 = reinterpret_cast<uint8_t*>(&p00);
            uint8_t* c01 = reinterpret_cast<uint8_t*>(&p01);
            uint8_t* c10 = reinterpret_cast<uint8_t*>(&p10);
            uint8_t* c11 = reinterpret_cast<uint8_t*>(&p11);

            uint32_t result = 0;
            uint8_t* res = reinterpret_cast<uint8_t*>(&result);

            for (int ch = 0; ch < 4; ch++)
            {
                float top = c00[ch] * (1.0f - dx) + c01[ch] * dx;
                float bot = c10[ch] * (1.0f - dx) + c11[ch] * dx;
                res[ch] = static_cast<uint8_t>(top * (1.0f - dy) + bot * dy);
            }

            output[y * outWidth + x] = result;
        }
    }

    return true;
}

bool LoadPNG(const std::string& path, std::vector<uint32_t>& data, int& width, int& height)
{
    int channels = 0;
    unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (!img)
        return false;

    data.resize(width * height);
    std::memcpy(data.data(), img, width * height * 4);
    stbi_image_free(img);
    return true;
}

void SavePNG(const std::string& path, const std::vector<uint32_t>& data, int width, int height)
{
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    stbi_write_png(path.c_str(), width, height, 4, data.data(), width * 4);
}

}
