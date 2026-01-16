#include "pch.h"
#include "resource.h"
#include "CDialog2.h"
#include "afxdialogex.h"

#include <afxdlgs.h>   // CFolderPickerDialog
#include "SJL.h"       // 你的解包函数：SJL::Unpack(...)

IMPLEMENT_DYNAMIC(CDialog2, CDialogEx)

CDialog2::CDialog2(const std::vector<CString>& sjlFiles, CWnd* pParent)
    : CDialogEx(IDD_DIALOG2, pParent),
    m_sjlFiles(sjlFiles)
{
}

CDialog2::~CDialog2() {}

void CDialog2::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialog2, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDialog2::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CDialog2::OnBnClickedButton2)
END_MESSAGE_MAP()

static CString GetParentFolder(const CString& filePath)
{
    CString dir = filePath;
    int pos = dir.ReverseFind(_T('\\'));
    if (pos > 0) dir = dir.Left(pos);
    return dir;
}

// Button1：选择解包目录（不解包）
void CDialog2::OnBnClickedButton1()
{
    CFolderPickerDialog dlg(nullptr, OFN_PATHMUSTEXIST, this, 0);
    if (dlg.DoModal() == IDOK)
    {
        m_outFolder = dlg.GetFolderPath();
        AfxMessageBox(_T("解包目录：") + m_outFolder);
    }
}

// Button2：直接解包到源文件夹（立刻执行）
void CDialog2::OnBnClickedButton2()
{
    for (auto& sjl : m_sjlFiles)
    {
        CString outDir = GetParentFolder(sjl);
        SJL::Unpack((LPCWSTR)sjl, (LPCWSTR)outDir);
    }

    AfxMessageBox(_T("解包完成"));
    EndDialog(IDOK);
}

// OK：如果用户选过目录，就解包到该目录；否则解包到源文件夹
void CDialog2::OnOK()
{
    for (auto& sjl : m_sjlFiles)
    {
        CString outDir = m_outFolder.IsEmpty() ? GetParentFolder(sjl) : m_outFolder;
        SJL::Unpack((LPCWSTR)sjl, (LPCWSTR)outDir);
    }

    AfxMessageBox(_T("解包完成"));
    CDialogEx::OnOK();
}
