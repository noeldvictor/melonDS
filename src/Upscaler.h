#ifndef UPSCALER_H
#define UPSCALER_H

#include <vector>
#include <cstdint>
#include <string>

namespace Upscaler
{

bool BilinearUpscale(const std::vector<uint32_t>& input, int width, int height, int scale,
    std::vector<uint32_t>& output, int& outWidth, int& outHeight);

bool LoadPNG(const std::string& path, std::vector<uint32_t>& data, int& width, int& height);
void SavePNG(const std::string& path, const std::vector<uint32_t>& data, int width, int height);

}

#endif // UPSCALER_H
