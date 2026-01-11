#pragma once
#include "IBytesProvider.h"
#include <fstream>

namespace MMMEngine::Editor
{
    class FileBytesProvider final : public MMMEngine::IBytesProvider
    {
    public:
        bool ReadAll(const MMMEngine::AssetEntry& entry, std::vector<uint8_t>& outBytes) override;
    };
}
