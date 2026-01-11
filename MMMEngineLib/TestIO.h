#pragma once
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <commdlg.h>

#include "MUID.h"
#include "StringHelper.h"
#include "json/json.hpp"

#pragma comment(lib, "Comdlg32.lib")


#pragma pack(push,1)
struct MTextHeader
{
    uint32_t magic = 0x5458544D; // 'MTXT'
    uint32_t version = 1;
    uint32_t size = 0;
};
#pragma pack(pop)


static std::optional<std::wstring> OpenTextFileDialog()
{
    wchar_t fileName[MAX_PATH] = { 0 };

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
        return std::wstring(fileName);

    return std::nullopt;
}


static bool ReadFileBytes(const std::wstring& path, std::vector<uint8_t>& out)
{
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return false;
    ifs.seekg(0, std::ios::end);
    size_t sz = (size_t)ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    out.resize(sz);
    if (sz) ifs.read((char*)out.data(), (std::streamsize)sz);
    return true;
}

inline bool WriteFileBytes(const std::wstring& path, const void* data, size_t size)
{
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) return false;
    if (size > 0)
        ofs.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
    return true;
}

static bool WriteMTextFile(const std::wstring& outMtextPath, const std::vector<uint8_t>& txtBytes)
{
    MTextHeader hdr;
    hdr.size = static_cast<uint32_t>(txtBytes.size());

    std::ofstream ofs(outMtextPath, std::ios::binary);
    if (!ofs) return false;

    ofs.write(reinterpret_cast<const char*>(&hdr), sizeof(hdr));
    if (!txtBytes.empty())
        ofs.write(reinterpret_cast<const char*>(txtBytes.data()),
                  static_cast<std::streamsize>(txtBytes.size()));

    return true;
}

static bool WriteMetaJson(const std::wstring& metaPathW,
    const MMMEngine::Utility::MUID& muid,
    const std::string& canonicalArtifactPathUtf8,
    uint32_t typeId)
{
    nlohmann::json j;

    try {
        j["muid"] = muid.ToString();
        j["artifact"] = canonicalArtifactPathUtf8;  // 여기서 에러 발생 가능
        j["type"] = typeId;

        std::ofstream ofs(metaPathW);
        if (!ofs) return false;
        ofs << j.dump(2);
        return true;
    }
    catch (const nlohmann::json::exception& e) {
        // 디버깅: 어떤 값이 문제인지 확인
        OutputDebugStringA(("JSON error: " + std::string(e.what())).c_str());

        // canonicalArtifactPathUtf8의 바이트 출력
        for (unsigned char c : canonicalArtifactPathUtf8) {
            char buf[32];
            sprintf_s(buf, "0x%02X ", c);
            OutputDebugStringA(buf);
        }

        std::cout << std::string(e.what()) << std::endl;

        return false;
    }
}

nlohmann::json ReadMetaJson(const std::wstring& metaPathW)
{
    // 1. 파일 열기
    std::ifstream ifs(metaPathW);
    if (!ifs.is_open()) {
        // C++17 이상에서는 std::string으로 변환하여 오류 메시지에 포함할 수 있습니다.
        // 여기서는 간단하게 처리합니다.
        throw std::runtime_error("메타 파일 열기 실패.");
    }

    // 2. 파일 전체 내용을 문자열로 읽기
    // 더 큰 파일에 대해 성능을 고려한다면, std::istreambuf_iterator를 사용합니다.
    try {
        std::string content((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());

        // 3. 문자열을 nlohmann::json 객체로 파싱
        return nlohmann::json::parse(content);

    }
    catch (const nlohmann::json::parse_error& e) {
        // JSON 파싱 에러 처리
        std::string errMsg = "JSON 파싱 에러: " + std::string(e.what());
        throw std::runtime_error(errMsg);
    }
    catch (const std::exception& e) {
        // 기타 예외 처리 (예: 메모리 부족 등)
        std::string errMsg = "파일 읽기/파싱 중 알 수 없는 에러 발생: " + std::string(e.what());
        throw std::runtime_error(errMsg);
    }
}