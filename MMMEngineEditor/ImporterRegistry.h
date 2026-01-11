#pragma once
#include "Importer.h"
#include <vector>
#include <string>
#include <string_view>

namespace MMMEngine::Editor
{
    class ImporterRegistry
    {
    public:
        void Register(Importer* importer);

        // sourcePath ("Assets/Textures/A.png") 기준으로 확장자 보고 선택
        Importer* FindForSourcePath(std::wstring_view sourcePath) const;

        // 확장자만으로 선택 (".png" / "png" 모두 허용)
        Importer* FindForExtension(std::wstring_view ext) const;

    private:
        static std::wstring GetExtensionLower(std::wstring_view path);
        static std::wstring NormalizeExt(std::wstring_view ext);

    private:
        std::vector<Importer*> m_importers;
    };
}