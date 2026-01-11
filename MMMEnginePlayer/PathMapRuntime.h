#pragma once
#include "MUID.h"
#include <unordered_map>
#include <string>

namespace MMMEngine::Player
{
    class PathMapRuntime
    {
    public:
        bool LoadFromFile(const std::wstring& path);

        bool TryGetMUID(std::string_view sourcePath, Utility::MUID& out) const
        {
            auto it = m_map.find(std::string(sourcePath));
            if (it == m_map.end()) return false;
            out = it->second;
            return true;
        }

    private:
        std::unordered_map<std::string, Utility::MUID> m_map;
    };
}
