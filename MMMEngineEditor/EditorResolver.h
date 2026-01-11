#pragma once
#include "IAssetResolver.h"
#include "MetaDB.h"

namespace MMMEngine::Editor
{
    class EditorResolver final : public MMMEngine::IAssetResolver
    {
    public:
        explicit EditorResolver(const MetaDB* db) : m_db(db) {}

        virtual bool Resolve(std::string_view sourcePath, MMMEngine::AssetEntry& out) const override;
        virtual bool Resolve(const Utility::MUID& muid, AssetEntry& out) const override;

    private:
        const MetaDB* m_db = nullptr;
    };
}
