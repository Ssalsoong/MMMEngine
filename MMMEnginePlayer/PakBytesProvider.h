#pragma once
#include "IBytesProvider.h"
#include "PakFileReader.h"

namespace MMMEngine::Player
{
    class PakBytesProvider final : public MMMEngine::IBytesProvider
    {
    public:
        explicit PakBytesProvider(PakFileReader* reader) : m_reader(reader) {}

        bool ReadAll(const MMMEngine::AssetEntry& entry, std::vector<uint8_t>& outBytes) override
        {
            if (!m_reader) return false;
            if (entry.source != MMMEngine::AssetEntry::Source::Pak) return false;
            return m_reader->Read(entry.offset, entry.size, outBytes);
        }

    private:
        PakFileReader* m_reader = nullptr;
    };
}
