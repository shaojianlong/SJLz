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
END_MESSAGE_MAP()

BOOL Dialog1::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_edit1.SetWindowText(_T("mypack"));
    return TRUE;
}

void Dialog1::OnOK()
{
    CString name;
    m_edit1.GetWindowText(name);
    name.Trim();

    if (name.IsEmpty())
    {
        AfxMessageBox(_T("请输入文件名，例如 mypack"));
        return;
    }

    // 输出：源文件夹\name.SJL
    std::wstring outFile = std::wstring(m_baseFolder) + L"\\" + std::wstring(name) + L".SJL";

    // roots：选中的文件/文件夹完整路径
    std::vector<std::wstring> roots;
    roots.reserve(m_selected.size());
    for (auto& s : m_selected)
        roots.push_back((LPCWSTR)s);

    // baseDir：源文件夹（当前显示目录）
    bool ok = SJL::Pack(roots, outFile, (LPCWSTR)m_baseFolder);
    if (!ok)
    {
        AfxMessageBox(_T("打包失败（可能是权限/路径/文件占用）"));
        return;
    }

    AfxMessageBox(_T("打包完成：") + CString(outFile.c_str()));
    CDialogEx::OnOK(); // 关闭对话框
}


void Dialog1::OnBnClickedButton1()
{
    // 初始目录：优先上次选的，否则用源文件夹
    CString init = m_outFolder.IsEmpty() ? m_baseFolder : m_outFolder;

    CFolderPickerDialog dlg(init, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, this, 0);
    if (dlg.DoModal() == IDOK)
    {
        m_outFolder = dlg.GetFolderPath();

        // 可选：给用户一个提示（你也可以不提示）
        // AfxMessageBox(_T("输出目录：") + m_outFolder);
    }
}
