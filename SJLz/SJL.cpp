#include "pch.h"
#include "SJL.h"

#include <filesystem>
#include <fstream>
#include <cstdint>
#include <vector>
#include <windows.h>

namespace fs = std::filesystem;

// --- UTF16 <-> UTF8 ---
static std::string WToUtf8(const std::wstring& w)
{
    if (w.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), n, nullptr, nullptr);
    return s;
}
static std::wstring Utf8ToW(const std::string& s)
{
    if (s.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), n);
    return w;
}

// --- Little-endian IO ---
template<typename T>
static void WriteLE(std::ofstream& out, T v)
{
    for (size_t i = 0; i < sizeof(T); ++i)
        out.put((char)((v >> (8 * i)) & 0xFF));
}
template<typename T>
static bool ReadLE(std::ifstream& in, T& v)
{
    v = 0;
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        int c = in.get();
        if (c == EOF) return false;
        v |= (T)((uint64_t)(unsigned char)c << (8 * i));
    }
    return true;
}

static void CollectFiles(const fs::path& root, std::vector<fs::path>& outFiles)
{
    std::error_code ec;
    if (fs::is_regular_file(root, ec))
    {
        outFiles.push_back(root);
        return;
    }
    if (fs::is_directory(root, ec))
    {
        for (auto it = fs::recursive_directory_iterator(root, ec);
            !ec && it != fs::recursive_directory_iterator();
            it.increment(ec))
        {
            if (ec) break;
            if (it->is_regular_file(ec))
                outFiles.push_back(it->path());
        }
    }
}

/*
格式（SJL1）：
4字节 "SJL1"
重复：
  uint32 pathLen
  pathLen 字节 UTF-8 相对路径（用 /）
  uint64 dataLen
  dataLen 字节数据
*/
bool SJL::Pack(const std::vector<std::wstring>& roots,
    const std::wstring& outFile,
    const std::wstring& baseDir)
{
    try
    {
        if (roots.empty()) return false;

        fs::path base(baseDir);
        if (!fs::exists(base)) return false;

        std::vector<fs::path> files;
        files.reserve(256);

        for (const auto& r : roots)
            CollectFiles(fs::path(r), files);

        if (files.empty()) return false;

        std::ofstream out(outFile, std::ios::binary);
        if (!out) return false;

        out.write("SJL1", 4);

        std::vector<char> buffer(1024 * 1024); // 1MB

        for (const auto& f : files)
        {
            // 相对路径（保留目录结构）
            fs::path rel;
            std::error_code ec;
            rel = fs::relative(f, base, ec);
            if (ec || rel.empty() || rel.native().rfind(L"..", 0) == 0)
                rel = f.filename(); // fallback

            std::string relUtf8 = WToUtf8(rel.generic_wstring()); // 用 / 分隔
            uint32_t pathLen = (uint32_t)relUtf8.size();

            uint64_t dataLen = (uint64_t)fs::file_size(f, ec);
            if (ec) dataLen = 0;

            WriteLE<uint32_t>(out, pathLen);
            out.write(relUtf8.data(), (std::streamsize)relUtf8.size());
            WriteLE<uint64_t>(out, dataLen);

            std::ifstream in(f, std::ios::binary);
            if (!in)
            {
                // 文件打不开就写空（也可以选择 return false）
                continue;
            }

            uint64_t remaining = dataLen;
            while (remaining > 0)
            {
                uint64_t chunk = remaining > buffer.size() ? (uint64_t)buffer.size() : remaining;
                in.read(buffer.data(), (std::streamsize)chunk);
                std::streamsize got = in.gcount();
                if (got <= 0) break;
                out.write(buffer.data(), got);
                remaining -= (uint64_t)got;
            }
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SJL::Unpack(const std::wstring& archiveFile,
    const std::wstring& outDir,
    bool overwrite)
{
    try
    {
        std::ifstream in(archiveFile, std::ios::binary);
        if (!in) return false;

        char magic[4] = { 0 };
        in.read(magic, 4);
        if (in.gcount() != 4 || std::string(magic, 4) != "SJL1")
            return false;

        fs::path outBase(outDir);
        fs::create_directories(outBase);

        std::vector<char> buffer(1024 * 1024);

        while (true)
        {
            uint32_t pathLen = 0;
            if (!ReadLE<uint32_t>(in, pathLen)) break; // EOF 正常结束
            if (pathLen == 0) return false;

            std::string relUtf8(pathLen, '\0');
            in.read(relUtf8.data(), (std::streamsize)pathLen);
            if (in.gcount() != (std::streamsize)pathLen) return false;

            uint64_t dataLen = 0;
            if (!ReadLE<uint64_t>(in, dataLen)) return false;

            fs::path rel(Utf8ToW(relUtf8));
            fs::path outPath = outBase / rel;

            fs::create_directories(outPath.parent_path());

            if (!overwrite && fs::exists(outPath))
            {
                in.seekg((std::streamoff)dataLen, std::ios::cur);
                continue;
            }

            std::ofstream out(outPath, std::ios::binary);
            if (!out) return false;

            uint64_t remaining = dataLen;
            while (remaining > 0)
            {
                uint64_t chunk = remaining > buffer.size() ? (uint64_t)buffer.size() : remaining;
                in.read(buffer.data(), (std::streamsize)chunk);
                std::streamsize got = in.gcount();
                if (got <= 0) return false;
                out.write(buffer.data(), got);
                remaining -= (uint64_t)got;
            }
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}
