#pragma once
#include <mutex>
#include <string>
#include <vector>
#include "types.h"

class TextureDumpManager {
public:
    static TextureDumpManager& Instance();

    void Dump3DTexture(u32 texCRC, const void* rgba8888,
                       int w, int h, int fmt, int paletteCRC);
    void DumpSprite(u32 sprCRC, const void* rgba8888,
                    int w, int h, int fmt, int paletteCRC);

    bool LoadHD(u32 srcCRC, std::vector<u32>& dstRGBA,
                int& outW, int& outH);

    bool Enabled() const { return enable; }
    void SetEnabled(bool v) { enable = v; }

private:
    TextureDumpManager();
    std::string  dumpDir, replaceDir;
    std::mutex   ioMutex;
    bool         enable = false;

    std::string  MakeName(const char* prefix, u32 crc,
                          int w, int h, bool dump) const;
    bool         LoadPNG(const std::string& path,
                         std::vector<u32>& out,
                         int& w, int& h) const;
    void         SavePNG(const std::string& path,
                         const void* rgba8888,
                         int w, int h) const;
};
