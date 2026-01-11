#pragma once
#include "IAssetResolver.h"
#include "PathMapRuntime.h"
#include "AssetIndexRuntime.h"

namespace MMMEngine::Player
{
    class PlayerResolver final : public MMMEngine::IAssetResolver
    {
    public:
        PlayerResolver(const PathMapRuntime* pm, const AssetIndexRuntime* ix)
            : m_pathMap(pm), m_index(ix) {
        }

        bool Resolve(std::string_view sourcePath, MMMEngine::AssetEntry& out) const override
        {
            if (!m_pathMap || !m_index) return false;

            Utility::MUID muid{};
            if (!m_pathMap->TryGetMUID(sourcePath, muid)) return false;

            AssetSpan sp{};
            if (!m_index->TryGetSpan(muid, sp)) return false;

            out.muid = muid;
            out.source = MMMEngine::AssetEntry::Source::Pak;
            out.filePath.clear();
            out.offset = sp.offset;
            out.size = sp.size;
            return true;
        }

        bool Resolve(const Utility::MUID& muid, MMMEngine::AssetEntry& out) const override
        {
            if (!m_index) return false;

            AssetSpan sp{};
            if (!m_index->TryGetSpan(muid, sp)) return false;

            out.muid = muid;
            out.source = MMMEngine::AssetEntry::Source::Pak;
            out.filePath.clear();
            out.offset = sp.offset;
            out.size = sp.size;
            return true;
        }

    private:
        const PathMapRuntime* m_pathMap = nullptr;
        const AssetIndexRuntime* m_index = nullptr;
    };
}
