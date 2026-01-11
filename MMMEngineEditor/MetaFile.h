#pragma once
#include "MUID.h"
#include <string>

namespace MMMEngine::Editor
{
    struct MetaFile
    {
        Utility::MUID muid{};
        std::string sourcePath;          // key
        std::wstring artifactPath;    // ¿¹: "Cache/Resources/<guid>.mtexture"
        uint32_t importerVersion = 1;     // ¿É¼Ç
    };

    bool WriteMeta(const std::wstring& metaAbsPath, const MetaFile& m);
    bool ReadMeta(const std::wstring& metaAbsPath, MetaFile& out);
}
