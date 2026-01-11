#pragma once
#include "AssetEntry.h"
#include <string_view>

namespace MMMEngine
{
    struct IAssetResolver
    {
        virtual ~IAssetResolver() = default;
        virtual bool Resolve(std::string_view sourcePath, AssetEntry& out) const = 0;

        virtual bool Resolve(const Utility::MUID& guid, AssetEntry& out) const = 0;
    };
}
