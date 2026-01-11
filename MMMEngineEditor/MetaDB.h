#pragma once
#include "MetaFile.h"
#include <unordered_map>

namespace MMMEngine::Editor
{
    class MetaDB
    {
    public:
        // 프로젝트 Assets 스캔해서 meta들을 로드
        bool BuildFromAssetsFolder(const std::wstring& assetsRoot);

        bool TryResolve(std::string_view sourcePath, MetaFile& out) const;

        bool TryResolve(Utility::MUID muid, MetaFile& out) const;

        template<typename Fn>
        void ForEachMeta(Fn&& fn) const
        {
            for (const auto& kv : m_bySourcePath)
                fn(kv.second);
        }

    private:
        std::unordered_map<std::string, MetaFile> m_bySourcePath;
        std::unordered_map<Utility::MUID, MetaFile, Utility::MUID::Hash> m_byMUID;
    };
}
