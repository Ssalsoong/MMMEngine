#include "PakBuilder.h"
#include <fstream>
#include <filesystem>

namespace MMMEngine::Editor
{
    static bool ReadAllBytes(const std::wstring& path, std::vector<uint8_t>& out)
    {
        std::ifstream f(path, std::ios::binary);
        if (!f) return false;

        f.seekg(0, std::ios::end);
        const auto sz = f.tellg();
        if (sz < 0) return false;
        f.seekg(0, std::ios::beg);

        out.resize(static_cast<size_t>(sz));
        if (sz > 0)
            f.read(reinterpret_cast<char*>(out.data()), sz);
        return (bool)f;
    }

    bool PakBuilder::BuildAll(const MetaDB& metaDB, const OutputPaths& out)
    {
        namespace fs = std::filesystem;

        // 출력 폴더 생성
        {
            std::error_code ec;
            fs::path pakP(out.pakPath);
            if (pakP.has_parent_path())
                fs::create_directories(pakP.parent_path(), ec);

            fs::path ixP(out.assetIndexPath);
            if (ixP.has_parent_path())
                fs::create_directories(ixP.parent_path(), ec);

            fs::path pmP(out.pathMapPath);
            if (pmP.has_parent_path())
                fs::create_directories(pmP.parent_path(), ec);
        }

        std::ofstream pak(out.pakPath, std::ios::binary);
        if (!pak) return false;

        std::vector<BuiltAssetRecord> records;
        records.reserve(4096);

        uint64_t cursor = 0;

        // --- MetaDB 순회 (MetaDB에 ForEachMeta를 추가해서 쓰세요) ---
        metaDB.ForEachMeta([&](const MetaFile& meta)
            {
                // 여기서 meta.sourcePath는 key, meta.artifactPath는 읽을 파일
                std::vector<uint8_t> bytes;
                if (!ReadAllBytes(meta.artifactPath, bytes))
                {
                    // 람다에서 실패를 밖으로 전달하기 어려우니,
                    // MVP에서는 예외를 던지거나 플래그로 처리합니다.
                    throw std::runtime_error("Failed to read artifact file.");
                }

                // pak에 append
                if (!bytes.empty())
                    pak.write(reinterpret_cast<const char*>(bytes.data()),
                        static_cast<std::streamsize>(bytes.size()));

                BuiltAssetRecord r{};
                r.muid = meta.muid;
                r.sourcePath = BuildIndexWriter::NormalizeSourcePath(meta.sourcePath);
                r.offset = cursor;
                r.size = static_cast<uint64_t>(bytes.size());

                records.push_back(std::move(r));
                cursor += static_cast<uint64_t>(bytes.size());
            });

        if (!pak) return false;

        // 인덱스 파일 생성
        if (!BuildIndexWriter::WriteAssetIndexBin(out.assetIndexPath, records))
            return false;

        if (!BuildIndexWriter::WritePathMapBin(out.pathMapPath, records))
            return false;

        return true;
    }
}
