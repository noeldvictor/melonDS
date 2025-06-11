#include "TextureDumpManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "extern/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "extern/stb_image_write.h"

#include <filesystem>
#include <vector>
#include "frontend/qt_sdl/Config.h"
#include "Platform.h"

using namespace std;
namespace fs = std::filesystem;

TextureDumpManager& TextureDumpManager::Instance() {
    static TextureDumpManager s;
    return s;
}

TextureDumpManager::TextureDumpManager() {
    const char* base = Platform::GetUserDataDir();
    dumpDir    = string(base) + "/HD_Textures/dump/";
    replaceDir = string(base) + "/HD_Textures/replace/";
    fs::create_directories(dumpDir);
    fs::create_directories(replaceDir);
    enable = Config::GetGlobalTable().GetBool("enable_hd_textures");
}

void TextureDumpManager::Dump3DTexture(u32 crc, const void* buf,
                                       int w, int h, int, int) {
    if (!enable) return;
    lock_guard<mutex> lk(ioMutex);
    string path = MakeName("tex", crc, w, h, true);
    if (!fs::exists(path))
        SavePNG(path, buf, w, h);
}

void TextureDumpManager::DumpSprite(u32 crc, const void* buf,
                                    int w, int h, int, int) {
    if (!enable) return;
    lock_guard<mutex> lk(ioMutex);
    string path = MakeName("spr", crc, w, h, true);
    if (!fs::exists(path))
        SavePNG(path, buf, w, h);
}

bool TextureDumpManager::LoadHD(u32 crc, vector<u32>& out, int& w, int& h) {
    if (!enable) return false;
    lock_guard<mutex> lk(ioMutex);
    for (auto prefix : {"tex", "spr"}) {
        string glob = MakeName(prefix, crc, 0, 0, false);
        for (auto& p : fs::directory_iterator(replaceDir))
            if (p.path().string().find(glob) != string::npos)
                return LoadPNG(p.path().string(), out, w, h);
    }
    return false;
}

string TextureDumpManager::MakeName(const char* prefix, u32 crc,
                                    int w, int h, bool dump) const {
    char tmp[64];
    if (dump)
        snprintf(tmp, sizeof(tmp), "%s_%08X_%dx%d.png", prefix, crc, w, h);
    else
        snprintf(tmp, sizeof(tmp), "%s_%08X", prefix, crc);
    return string(dump ? dumpDir : replaceDir) + tmp;
}

void TextureDumpManager::SavePNG(const string& p,
                                 const void* rgba, int w, int h) const {
    stbi_write_png(p.c_str(), w, h, 4, rgba, w * 4);
}

bool TextureDumpManager::LoadPNG(const string& p,
                                 vector<u32>& out, int& w, int& h) const {
    int comp;
    unsigned char* data = stbi_load(p.c_str(), &w, &h, &comp, 4);
    if (!data) return false;
    out.assign(reinterpret_cast<u32*>(data),
               reinterpret_cast<u32*>(data) + w * h);
    stbi_image_free(data);
    return true;
}
