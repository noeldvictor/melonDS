#include "TextureDumper.h"

#include <filesystem>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

namespace melonDS
{
bool TextureDumper::DumpEnabled = false;
bool TextureDumper::ReplaceEnabled = false;
std::string TextureDumper::DumpDirectory;
std::string TextureDumper::ReplaceDirectory;
std::unordered_set<u64> TextureDumper::DumpedTextures;

void TextureDumper::SetConfig(bool dump, bool replace,
    const std::string& dumpPath, const std::string& replacePath)
{
    DumpEnabled = dump;
    ReplaceEnabled = replace;
    DumpDirectory = dumpPath;
    ReplaceDirectory = replacePath;
}

void TextureDumper::DumpTexture(const u32* pixels, u32 width, u32 height, u64 hash)
{
    if (!DumpEnabled)
        return;
    if (DumpedTextures.find(hash) != DumpedTextures.end())
        return;
    DumpedTextures.insert(hash);

    if (!DumpDirectory.empty())
        std::filesystem::create_directories(DumpDirectory);
    std::string name = DumpDirectory + "/" + std::to_string(hash) + "_" +
        std::to_string(width) + "x" + std::to_string(height) + ".png";
    stbi_write_png(name.c_str(), width, height, 4, pixels, width * 4);
}

bool TextureDumper::LoadReplacement(u32* pixels, u32 width, u32 height, u64 hash)
{
    if (!ReplaceEnabled)
        return false;
    std::string name = ReplaceDirectory + "/" + std::to_string(hash) + "_" +
        std::to_string(width) + "x" + std::to_string(height) + ".png";
    int w, h, n;
    unsigned char* data = stbi_load(name.c_str(), &w, &h, &n, 4);
    if (!data)
        return false;
    if ((u32)w != width || (u32)h != height)
    {
        stbi_image_free(data);
        return false;
    }
    std::memcpy(pixels, data, width * height * 4);
    stbi_image_free(data);
    return true;
}
}

