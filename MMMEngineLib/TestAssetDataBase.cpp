#include "TestAssetDataBase.h"

namespace MMMEngine
{
    static bool ReadU32(std::ifstream& ifs, uint32_t& v)
    {
        ifs.read(reinterpret_cast<char*>(&v), 4);
        return static_cast<bool>(ifs);
    }
    static bool ReadU64(std::ifstream& ifs, uint64_t& v)
    {
        ifs.read(reinterpret_cast<char*>(&v), 8);
        return static_cast<bool>(ifs);
    }

    bool PakAssetDatabase::Mount(const std::wstring& pakPath)
    {
        m_index.clear();
        m_stream = std::ifstream(pakPath, std::ios::binary);
        if (!m_stream) return false;

        PakHeader header{};
        m_stream.read(reinterpret_cast<char*>(&header), sizeof(header));
        if (!m_stream) return false;

        if (header.magic != 0x4B41504D) return false; // 'MPAK'
        if (header.version != 1) return false;

        m_stream.seekg(static_cast<std::streamoff>(header.indexOffset), std::ios::beg);

        uint32_t count = 0;
        if (!ReadU32(m_stream, count)) return false;

        for (uint32_t i = 0; i < count; ++i)
        {
            uint32_t pathLen = 0;
            if (!ReadU32(m_stream, pathLen)) return false;

            std::string pathUtf8;
            pathUtf8.resize(pathLen);
            if (pathLen > 0)
                m_stream.read(pathUtf8.data(), static_cast<std::streamsize>(pathLen));

            uint64_t offset = 0, size = 0;
            if (!ReadU64(m_stream, offset)) return false;
            if (!ReadU64(m_stream, size)) return false;

            uint32_t muidLen = 0;
            if (!ReadU32(m_stream, muidLen)) return false;

            std::string muidStr;
            muidStr.resize(muidLen);
            if (muidLen > 0)
                m_stream.read(muidStr.data(), static_cast<std::streamsize>(muidLen));

            auto parsed = Utility::MUID::Parse(muidStr);
            if (!parsed.has_value()) return false;

            PakEntry e;
            e.muid = parsed.value();
            e.offset = offset;
            e.size = size;

            m_index.emplace(std::move(pathUtf8), e);
        }

        return true;
    }

    bool PakAssetDatabase::TryGetEntry(const std::string& pathUtf8, PakEntry& out) const
    {
        auto it = m_index.find(pathUtf8);
        if (it == m_index.end()) return false;
        out = it->second;
        return true;
    }

    bool PakAssetDatabase::ReadBytes(const PakEntry& entry, std::vector<uint8_t>& outBytes) const
    {
        if (!m_stream) return false;

        outBytes.resize(static_cast<size_t>(entry.size));
        m_stream.seekg(static_cast<std::streamoff>(entry.offset), std::ios::beg);

        if (entry.size > 0)
            m_stream.read(reinterpret_cast<char*>(outBytes.data()), static_cast<std::streamsize>(entry.size));

        return static_cast<bool>(m_stream);
    }
}
