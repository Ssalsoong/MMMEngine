#pragma once
#include "json/json.hpp"
#include "AssetMeta.h"

namespace MMMEngine
{
    inline void to_json(nlohmann::json& j, const AssetMeta& m)
    {
        j = {
            { "version", m.version },
            { "muid", m.muid.ToString() },
            { "importer", m.importer },
            { "source", m.sourcePath },
            { "artifact", m.artifactPath },
            { "type", static_cast<uint32_t>(m.type) }
        };
    }

    inline void from_json(const nlohmann::json& j, AssetMeta& m)
    {
        m.version = j.at("version").get<int>();
        m.muid = Utility::MUID::Parse(j.at("muid").get<std::string>()).value();
        m.importer = j.at("importer").get<std::string>();
        m.sourcePath = j.at("source").get<std::string>();
        m.artifactPath = j.at("artifact").get<std::string>();
        m.type = static_cast<AssetType>(j.at("type").get<uint32_t>());
    }
}
