// ImporterRegistry.cpp
#include "ImporterRegistry.h"
#include <algorithm>
#include <cwctype>

namespace MMMEngine::Editor
{
    void ImporterRegistry::Register(Importer* importer)
    {
        if (!importer) return;
        m_importers.push_back(importer);
    }

    Importer* ImporterRegistry::FindForSourcePath(std::wstring_view sourcePath) const
    {
        const std::wstring ext = GetExtensionLower(sourcePath);
        return FindForExtension(ext);
    }

    Importer* ImporterRegistry::FindForExtension(std::wstring_view ext) const
    {
        const std::wstring norm = NormalizeExt(ext);

        for (Importer* imp : m_importers)
        {
            if (imp && imp->CanImportExtension(norm))
                return imp;
        }
        return nullptr;
    }

    std::wstring ImporterRegistry::GetExtensionLower(std::wstring_view path)
    {
        // 마지막 '.' 찾기
        const size_t dot = path.find_last_of(L'.');
        if (dot == std::wstring_view::npos)
            return L"";

        std::wstring ext(path.substr(dot)); // ".png"
        std::transform(ext.begin(), ext.end(), ext.begin(),
            [](wchar_t c) { return (wchar_t)std::towlower(c); });
        return ext;
    }

    std::wstring ImporterRegistry::NormalizeExt(std::wstring_view ext)
    {
        if (ext.empty()) return L"";

        std::wstring e(ext);
        // "png"면 ".png"로
        if (!e.empty() && e[0] != L'.')
            e.insert(e.begin(), L'.');

        std::transform(e.begin(), e.end(), e.begin(),
            [](wchar_t c) { return (wchar_t)std::towlower(c); });
        return e;
    }
}
