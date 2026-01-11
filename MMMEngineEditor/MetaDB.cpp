#include "MetaDB.h"
#include <filesystem>
#include <system_error>

bool MMMEngine::Editor::MetaDB::BuildFromAssetsFolder(const std::wstring& assetsRoot)
{
    namespace fs = std::filesystem;

    m_bySourcePath.clear();
    m_byMUID.clear();

    std::error_code ec;
    fs::path root(assetsRoot);

    if (!fs::exists(root, ec) || !fs::is_directory(root, ec))
        return false;

    // 재귀 순회
    // 권장: 권한 문제/깨진 symlink 등에 대해 예외가 터질 수 있으니 error_code 기반 또는 try/catch 사용
    try
    {
        for (fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec);
            it != fs::recursive_directory_iterator();
            it.increment(ec))
        {
            if (ec)
            {
                // 디렉토리 순회 중 에러가 생기면 MVP에선 실패 처리하는게 안전
                return false;
            }

            const fs::directory_entry& entry = *it;
            if (!entry.is_regular_file(ec))
                continue;

            const fs::path& p = entry.path();

            // "*.meta" 만 처리
            if (p.extension() != L".meta")
                continue;

            MetaFile meta{};
            if (!ReadMeta(p.wstring(), meta))
            {
                // 정책 1) meta 하나라도 실패하면 전체 실패
                return false;

                // 정책 2) 실패한 meta만 스킵하고 계속 (원하면 위 return false 대신 continue)
                // continue;
            }

            // 기본 검증(필요 시 강화)
            if (meta.sourcePath.empty() || !meta.muid.IsValid())
            {
                // 잘못된 meta면 실패(또는 스킵)
                return false;
            }

            // 등록 (중복이면 덮어씀)
            m_bySourcePath[meta.sourcePath] = meta;
            m_byMUID[meta.muid] = meta;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}


bool MMMEngine::Editor::MetaDB::TryResolve(std::string_view sourcePath, MetaFile& out) const
{
    auto it = m_bySourcePath.find(std::string(sourcePath));
    if (it == m_bySourcePath.end()) return false;
    out = it->second;
    return true;
}

bool MMMEngine::Editor::MetaDB::TryResolve(Utility::MUID muid, MetaFile& out) const
{
    auto it = m_byMUID.find(muid);
    if (it == m_byMUID.end()) return false;
    out = it->second;
    return true;
}
