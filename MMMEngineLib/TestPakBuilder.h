#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "MUID.h"
#include "StringHelper.h"

namespace MMMEngine
{
#pragma pack(push, 1)
    struct PakHeader
    {
        uint32_t magic = 0x4B41504D; // 'MPAK'
        uint16_t version = 1;
        uint16_t reserved = 0;
        uint64_t indexOffset = 0;
        uint64_t indexSize = 0;
    };
#pragma pack(pop)

    struct PakItem
    {
        std::string pathUtf8;           // key (¿¹: "Assets/Text/Hello.mtext")
        Utility::MUID muid;
        std::vector<uint8_t> bytes;     // blob
    };

    class PakBuilder
    {
    public:
        static bool Build(const std::wstring& pakPath, const std::vector<PakItem>& items);
    };
}
