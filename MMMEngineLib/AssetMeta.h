#pragma once
#include <string>
#include "MUID.h"
#include "AssetTypes.h"

namespace MMMEngine
{
    struct AssetMeta
    {
        int version = 1;
        Utility::MUID muid;
        std::string importer;
        std::string sourcePath;   // UTF-8 canonical
        std::string artifactPath; // UTF-8 canonical (.mtext)
        AssetType type = AssetType::Unknown;
    };
}
