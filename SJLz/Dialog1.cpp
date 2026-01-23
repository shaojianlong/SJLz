#include "pch.h"
#include "Dialog1.h"
#include "afxdialogex.h"
#include "SJL.h"
#include "resource.h"


IMPLEMENT_DYNAMIC(Dialog1, CDialogEx)

Dialog1::Dialog1(const std::vector<CString>& selected, const CString& baseFolder, CWnd* pParent)
    : CDialogEx(IDD_DIALOG1, pParent), m_selected(selected), m_baseFolder(baseFolder)
{
}

Dialog1::~Dialog1() {}

void Dialog1::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_edit1);
}

BEGIN_MESSAGE_MAP(Dialog1, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &Dialog1::OnBnClickedButton1)
    ON_BN_CLICKED(IDOK, &Dialog1::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL Dialog1::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_edit1.SetWindowText(_T("mypack"));
    return TRUE;
}




static bool PickFolderForceFileSystem(HWND owner, const CString& initFolder, CString& outFolder)
{
    CComPtr<IFileDialog> pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (FAILED(hr)) return false;

    DWORD opts = 0;
    pfd->GetOptions(&opts);
    pfd->SetOptions(opts | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);

    // 设置初始目录（可选）
    if (!initFolder.IsEmpty())
    {
        CComPtr<IShellItem> psi;
        hr = SHCreateItemFromParsingName(initFolder, nullptr, IID_PPV_ARGS(&psi));
        if (SUCCEEDED(hr)) pfd->SetFolder(psi);
    }

    hr = pfd->Show(owner);
    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) return false;
    if (FAILED(hr)) return false;

    CComPtr<IShellItem> item;
    if (FAILED(pfd->GetResult(&item))) return false;

    PWSTR psz = nullptr;
    if (FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &psz)) || !psz) return false;

    outFolder = psz;
    CoTaskMemFree(psz);
    outFolder.Trim();
    return !outFolder.IsEmpty();
}



void Dialog1::OnBnClickedButton1()
{
    CString init = m_outFolder.IsEmpty() ? m_baseFolder : m_outFolder;

    CString chosen;
    if (PickFolderForceFileSystem(this->GetSafeHwnd(), init, chosen))
    {
        m_outFolder = chosen;
        AfxMessageBox(_T("已选择输出文件夹：") + m_outFolder);
    }
}



void Dialog1::OnBnClickedOk()
{
    CString name;
    m_edit1.GetWindowText(name);
    name.Trim();

    if (name.IsEmpty())
    {
        AfxMessageBox(_T("请输入文件名，例如 mypack"));
        return;
    }

    // ✅ 如果用户输入了 .SJL，自动去掉
    if (name.Right(4).CompareNoCase(_T(".SJL")) == 0)
        name = name.Left(name.GetLength() - 4);

    // ✅ 优先输出到 m_outFolder，否则输出到 m_baseFolder
    CString outDir = m_outFolder;
    if (outDir.IsEmpty())
        outDir = m_baseFolder;

    outDir.Trim();
    if (outDir.IsEmpty())
    {
        AfxMessageBox(_T("输出目录为空，请重试。"));
        return;
    }

    std::wstring outFile = std::wstring(outDir) + L"\\" + std::wstring(name) + L".SJL";

    // ✅ roots：选中的文件/文件夹完整路径
    std::vector<std::wstring> roots;
    roots.reserve(m_selected.size());
    for (const auto& s : m_selected)
        roots.push_back((LPCWSTR)s);

    // ✅ baseDir 仍然用源文件夹（用于生成相对路径）
    bool ok = SJL::Pack(roots, outFile, (LPCWSTR)m_baseFolder);
    if (!ok)
    {
        AfxMessageBox(_T("打包失败（可能是权限/路径/文件占用）"));
        return;
    }

    AfxMessageBox(_T("打包完成：") + CString(outFile.c_str()));
    CDialogEx::OnOK();
}
