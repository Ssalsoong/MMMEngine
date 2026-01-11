#pragma once
#include "MetaDB.h"
#include "BuildIndexWriter.h"
#include <string>
#include <vector>

namespace MMMEngine::Editor
{
    class PakBuilder
    {
    public:
        struct OutputPaths
        {
            std::wstring pakPath;        // Data/assets.pak
            std::wstring assetIndexPath; // Data/AssetIndex.bin
            std::wstring pathMapPath;    // Data/PathMap.bin
        };

        // MetaDB를 순회해서 pak을 만들고, records를 구축한 뒤 인덱스 파일들을 생성
        // assetsRoot는 "Assets" 경로가 아니라, metaDB가 이미 완성됐다는 전제로 DB만 사용합니다.
        static bool BuildAll(const MetaDB& metaDB, const OutputPaths& out);
    };
}
