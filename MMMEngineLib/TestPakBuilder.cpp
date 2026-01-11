#include "TestPakBuilder.h"
#include <fstream>

namespace MMMEngine
{
    static void WriteU32(std::ofstream& ofs, uint32_t v) { ofs.write(reinterpret_cast<const char*>(&v), 4); }
    static void WriteU64(std::ofstream& ofs, uint64_t v) { ofs.write(reinterpret_cast<const char*>(&v), 8); }

    bool PakBuilder::Build(const std::wstring& pakPath, const std::vector<PakItem>& items)
    {
        std::ofstream ofs(pakPath, std::ios::binary);
        if (!ofs) return false;

        // 1) 빈 헤더 먼저 기록
        PakHeader header{};
        ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));

        struct IndexRec
        {
            std::string pathUtf8;
            uint64_t offset = 0;
            uint64_t size = 0;
            std::string muidStr;
        };

        std::vector<IndexRec> index;
        index.reserve(items.size());

        // 2) blob 쓰기
        for (auto& it : items)
        {
            const uint64_t offset = static_cast<uint64_t>(ofs.tellp());
            const uint64_t size = static_cast<uint64_t>(it.bytes.size());

            if (size > 0)
                ofs.write(reinterpret_cast<const char*>(it.bytes.data()), static_cast<std::streamsize>(size));

            IndexRec rec;
            rec.pathUtf8 = it.pathUtf8;
            rec.offset = offset;
            rec.size = size;
            rec.muidStr = it.muid.ToString(); // 문자열로 저장 (테스트용)
            index.push_back(std::move(rec));
        }

        // 3) index 쓰기
        const uint64_t indexOffset = static_cast<uint64_t>(ofs.tellp());

        WriteU32(ofs, static_cast<uint32_t>(index.size()));

        for (auto& rec : index)
        {
            WriteU32(ofs, static_cast<uint32_t>(rec.pathUtf8.size()));
            if (!rec.pathUtf8.empty())
                ofs.write(rec.pathUtf8.data(), static_cast<std::streamsize>(rec.pathUtf8.size()));

            WriteU64(ofs, rec.offset);
            WriteU64(ofs, rec.size);

            WriteU32(ofs, static_cast<uint32_t>(rec.muidStr.size()));
            if (!rec.muidStr.empty())
                ofs.write(rec.muidStr.data(), static_cast<std::streamsize>(rec.muidStr.size()));
        }

        const uint64_t indexEnd = static_cast<uint64_t>(ofs.tellp());
        const uint64_t indexSize = indexEnd - indexOffset;

        // 4) 헤더 patch
        header.indexOffset = indexOffset;
        header.indexSize = indexSize;

        ofs.seekp(0, std::ios::beg);
        ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));

        return true;
    }
}
