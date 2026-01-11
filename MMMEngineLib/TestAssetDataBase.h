#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

#include "MUID.h"
#include "StringHelper.h"
#include "TestPakBuilder.h" // PakHeader Àç»ç¿ë

namespace MMMEngine
{
    struct PakEntry
    {
        Utility::MUID muid;
        uint64_t offset = 0;
        uint64_t size = 0;
    };

    class PakAssetDatabase
    {
    public:
        bool Mount(const std::wstring& pakPath);

        bool TryGetEntry(const std::string& pathUtf8, PakEntry& out) const;
        bool ReadBytes(const PakEntry& entry, std::vector<uint8_t>& outBytes) const;

    private:
        std::unordered_map<std::string, PakEntry> m_index; // UTF-8 path -> entry
        mutable std::ifstream m_stream;
    };
}
