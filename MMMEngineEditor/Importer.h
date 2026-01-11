#pragma once
#include "MUID.h"
#include <string>
#include <vector>

namespace MMMEngine::Editor
{
    struct ImportInput
    {
        Utility::MUID muid;             // meta에 들어갈 guid
        std::wstring sourcePath;        // "Assets/Textures/A.png"
        std::wstring artifactPath;      // "Cache/Resources/" <- artifact
    };

    struct ImportOutput
    {
        // 생성된 artifact 경로들 (예: .mtexture, .mmat)
        std::vector<std::wstring> artifactAbsPaths;
        // 대표 artifact (Load(path)가 이걸 가리키게 할지 결정)
        std::wstring mainArtifactAbsPath;
        // resource type tag/extension 같은 거 추가 가능
        std::string artifactExt; // ".mtexture" etc (옵션)
    };

    class Importer
    {
    public:
        virtual ~Importer() = default;

        virtual bool CanImportExtension(std::wstring_view ext) const = 0;
        virtual bool Import(const ImportInput& in, ImportOutput& out) = 0;
    };
}
