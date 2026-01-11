// BuildIndexWriter.cpp
#include "BuildIndexWriter.h"
#include <fstream>
#include <unordered_map>

namespace MMMEngine::Editor
{
#pragma pack(push, 1)
    struct AssetIndexHeader
    {
        uint32_t magic;   // 'AIX0'
        uint32_t version; // 1
        uint32_t entryCount;
        uint32_t reserved;
    };

    struct AssetIndexEntry
    {
        Utility::MUID muid;
        uint64_t offset;
        uint64_t size;
    };

    struct PathMapHeader
    {
        uint32_t magic;   // 'PMAP'
        uint32_t version; // 1
        uint32_t entryCount;
        uint32_t stringBytes;
    };

    struct PathMapEntry
    {
        uint32_t pathOffset;
        Utility::MUID muid;
    };
#pragma pack(pop)

    static constexpr uint32_t AIX0_MAGIC =
        (uint32_t('A')) | (uint32_t('I') << 8) | (uint32_t('X') << 16) | (uint32_t('0') << 24);

    static constexpr uint32_t PMAP_MAGIC =
        (uint32_t('P')) | (uint32_t('M') << 8) | (uint32_t('A') << 16) | (uint32_t('P') << 24);

    std::string BuildIndexWriter::NormalizeSourcePath(std::string_view p)
    {
        // MVP: 백슬래시 -> 슬래시, 중복 슬래시 제거 정도만
        std::string s(p);

        for (char& c : s)
        {
            if (c == '\\') c = '/';
        }

        // "//" -> "/" 반복 제거(간단)
        std::string out;
        out.reserve(s.size());
        char prev = '\0';
        for (char c : s)
        {
            if (c == '/' && prev == '/') continue;
            out.push_back(c);
            prev = c;
        }
        return out;
    }

    bool BuildIndexWriter::WriteAssetIndexBin(const std::wstring& outPath, const std::vector<BuiltAssetRecord>& records)
    {
        std::ofstream f(outPath, std::ios::binary);
        if (!f) return false;

        AssetIndexHeader h{};
        h.magic = AIX0_MAGIC;
        h.version = 1;
        h.entryCount = static_cast<uint32_t>(records.size());
        h.reserved = 0;

        f.write(reinterpret_cast<const char*>(&h), sizeof(h));

        for (const auto& r : records)
        {
            AssetIndexEntry e{};
            e.muid = r.muid;
            e.offset = r.offset;
            e.size = r.size;

            f.write(reinterpret_cast<const char*>(&e), sizeof(e));
        }

        return (bool)f;
    }

    bool BuildIndexWriter::WritePathMapBin(const std::wstring& outPath, const std::vector<BuiltAssetRecord>& records)
    {
        // string blob 구축 (NUL-terminated)
        std::vector<char> blob;
        blob.reserve(records.size() * 64);

        std::vector<PathMapEntry> entries;
        entries.reserve(records.size());

        // 중복 sourcePath 방지(선택). 중복이면 마지막을 우선
        std::unordered_map<std::string, size_t> seen;

        for (const auto& r : records)
        {
            std::string key = NormalizeSourcePath(r.sourcePath);

            // 중복이면 기존 엔트리 무시하고 새로 덮어쓰려면,
            // 간단하게 blob/entries를 다시 짜야해서 MVP에서는 "중복이면 스킵" 추천.
            if (seen.find(key) != seen.end())
                continue;

            const uint32_t offset = static_cast<uint32_t>(blob.size());
            blob.insert(blob.end(), key.begin(), key.end());
            blob.push_back('\0');

            PathMapEntry e{};
            e.pathOffset = offset;
            e.muid = r.muid;

            entries.push_back(e);
            seen.emplace(std::move(key), entries.size() - 1);
        }

        std::ofstream f(outPath, std::ios::binary);
        if (!f) return false;

        PathMapHeader h{};
        h.magic = PMAP_MAGIC;
        h.version = 1;
        h.entryCount = static_cast<uint32_t>(entries.size());
        h.stringBytes = static_cast<uint32_t>(blob.size());

        f.write(reinterpret_cast<const char*>(&h), sizeof(h));
        if (!blob.empty())
            f.write(blob.data(), static_cast<std::streamsize>(blob.size()));
        if (!entries.empty())
            f.write(reinterpret_cast<const char*>(entries.data()),
                static_cast<std::streamsize>(entries.size() * sizeof(PathMapEntry)));

        return (bool)f;
    }
}
