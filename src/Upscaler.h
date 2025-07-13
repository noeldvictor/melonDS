#ifndef UPSCALER_H
#define UPSCALER_H

#include <vector>
#include <string>
#include <cstdint>

namespace HDUtils {

bool BilinearUpscale(const std::vector<uint32_t>& input, int width, int height, int scale,
                     std::vector<uint32_t>& output, int& outWidth, int& outHeight);
void SavePNG(const std::string& path, const std::vector<uint32_t>& data, int width, int height);
bool LoadPNG(const std::string& path, std::vector<uint32_t>& data, int& width, int& height);

void DumpTexture(uint64_t hash, const uint32_t* data, int width, int height);
bool LoadHDTexture(uint64_t hash, std::vector<uint32_t>& data, int& width, int& height);
void DumpSprite(uint64_t hash, const uint32_t* data, int width, int height);
bool LoadHDSprite(uint64_t hash, std::vector<uint32_t>& data, int& width, int& height);

}

#endif // UPSCALER_H
