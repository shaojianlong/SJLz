#pragma once
#include <string>
#include <vector>

class SJL
{
public:
    // roots: 选中的文件/文件夹（完整路径）
    // outFile: 输出 .SJL 完整路径
    // baseDir: 生成相对路径的基准目录（通常是当前显示目录）
    static bool Pack(const std::vector<std::wstring>& roots,
        const std::wstring& outFile,
        const std::wstring& baseDir);

    static bool Unpack(const std::wstring& archiveFile,
        const std::wstring& outDir,
        bool overwrite = true);
};
