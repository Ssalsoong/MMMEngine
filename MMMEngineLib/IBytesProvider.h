#pragma once
#include "AssetEntry.h"
#include <vector>

namespace MMMEngine
{
    struct IBytesProvider
    {
        virtual ~IBytesProvider() = default;
        virtual bool ReadAll(const AssetEntry& entry, std::vector<uint8_t>& outBytes) = 0;
    };
}
