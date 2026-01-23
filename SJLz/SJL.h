#pragma once
#include <string>
#include <vector>

class SJL
{
public:
    static bool Pack(const std::vector<std::wstring>& roots,
        const std::wstring& outFile,
        const std::wstring& baseDir);

    static bool Unpack(const std::wstring& archiveFile,
        const std::wstring& outDir,
        bool overwrite = true);
};
