#include "MetaFile.h"

#include <fstream>
#include <filesystem>
#include <string>

#include "json/json.hpp"
#include "StringHelper.h"

namespace MMMEngine::Editor
{
    using json = nlohmann::json;

    // -----------------------------
    // JSON 스키마 (권장)
    // -----------------------------
    // {
    //   "muid": "....",
    //   "sourcePath": "Assets/Textures/A.png",
    //   "artifactPath": "Cache/Resources/<guid>.mtexture",
    //   "importerVersion": 1
    // }
    //
    // 파일 확장자는 보통 "<원본파일>.meta" 를 권장:
    //   Assets/Textures/A.png.meta
    //

    bool WriteMeta(const std::wstring& metaAbsPath, const MetaFile& m)
    {
        try
        {
            // 폴더가 없으면 생성
            std::error_code ec;
            std::filesystem::path p(metaAbsPath);
            if (p.has_parent_path())
                std::filesystem::create_directories(p.parent_path(), ec);

            json j;
            j["muid"] = m.muid.ToString();
            j["sourcePath"] = m.sourcePath;
            j["artifactPath"] = Utility::StringHelper::WStringToString(m.artifactPath);
            j["importerVersion"] = m.importerVersion;

            std::ofstream f(metaAbsPath, std::ios::binary);
            if (!f) return false;

            // pretty print (디버깅/충돌 해결 편함)
            const std::string dumped = j.dump(2);
            f.write(dumped.data(), (std::streamsize)dumped.size());
            return (bool)f;
        }
        catch (...)
        {
            return false;
        }
    }

    bool ReadMeta(const std::wstring& metaAbsPath, MetaFile& out)
    {
        try
        {
            std::ifstream f(metaAbsPath, std::ios::binary);
            if (!f) return false;

            std::string text;
            f.seekg(0, std::ios::end);
            auto sz = f.tellg();
            if (sz < 0) return false;
            f.seekg(0, std::ios::beg);

            text.resize((size_t)sz);
            if (sz > 0) f.read(text.data(), sz);
            if (!f) return false;

            json j = json::parse(text, nullptr, /*allow_exceptions=*/true);

            // 필수 필드 체크
            if (!j.contains("muid") || !j.contains("sourcePath") || !j.contains("artifactPath"))
                return false;

            // muid
            {
                const std::string s = j.at("muid").get<std::string>();
                auto id = Utility::MUID::Parse(s);
                if (!id)
                    return false;
                out.muid = id.value();
            }

            // sourcePath
            out.sourcePath = j.at("sourcePath").get<std::string>();

            // artifactPath (json은 UTF-8로 저장)
            out.artifactPath = Utility::StringHelper::StringToWString(j.at("artifactPath").get<std::string>());

            // importerVersion (옵션)
            out.importerVersion = j.value("importerVersion", 1u);

            return true;
        }
        catch (...)
        {
            return false;
        }
    }
}