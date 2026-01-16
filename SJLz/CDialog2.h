#pragma once
#include "afxdialogex.h"
#include <vector>

class CDialog2 : public CDialogEx
{
    DECLARE_DYNAMIC(CDialog2)

public:
    CDialog2(const std::vector<CString>& sjlFiles, CWnd* pParent = nullptr);
    virtual ~CDialog2();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG2 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK();

    afx_msg void OnBnClickedButton1(); // 选目录
    afx_msg void OnBnClickedButton2(); // 直接解包

    std::vector<CString> m_sjlFiles;
    CString m_outFolder;

    DECLARE_MESSAGE_MAP()
};
