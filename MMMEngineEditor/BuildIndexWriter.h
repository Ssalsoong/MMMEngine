#pragma once
#include "MUID.h"
#include <string>
#include <vector>
#include <cstdint>

namespace MMMEngine::Editor
{
    struct BuiltAssetRecord
    {
        Utility::MUID muid{};
        std::string   sourcePath; // key (UTF-8)
        uint64_t      offset = 0;  // in pak
        uint64_t      size = 0;    // bytes
    };

    class BuildIndexWriter
    {
    public:
        static bool WriteAssetIndexBin(const std::wstring& outPath, const std::vector<BuiltAssetRecord>& records);
        static bool WritePathMapBin(const std::wstring& outPath, const std::vector<BuiltAssetRecord>& records);

        // 키 안정성을 위해 sourcePath 슬래시 통일(권장)
        static std::string NormalizeSourcePath(std::string_view p);
    };
}