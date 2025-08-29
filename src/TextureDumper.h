#ifndef TEXTUREDUMPER_H
#define TEXTUREDUMPER_H

#include <string>
#include <unordered_set>

#include "types.h"

namespace melonDS
{
class TextureDumper
{
public:
    static void SetConfig(bool dump, bool replace,
        const std::string& dumpPath, const std::string& replacePath);

    static void DumpTexture(const u32* pixels, u32 width, u32 height, u64 hash);
    static bool LoadReplacement(u32* pixels, u32 width, u32 height, u64 hash);

private:
    static bool DumpEnabled;
    static bool ReplaceEnabled;
    static std::string DumpDirectory;
    static std::string ReplaceDirectory;
    static std::unordered_set<u64> DumpedTextures;
};
}

#endif // TEXTUREDUMPER_H
