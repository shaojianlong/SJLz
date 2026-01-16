#pragma once
#include "afxdialogex.h"
#include <vector>

class Dialog1 : public CDialogEx
{
    DECLARE_DYNAMIC(Dialog1)

public:
    Dialog1(const std::vector<CString>& selected, const CString& baseFolder, CWnd* pParent = nullptr);
    virtual ~Dialog1();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG1 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK(); 


    CEdit m_edit1;

    std::vector<CString> m_selected;
    CString m_baseFolder;

    CString m_outFolder;

    // ✅ Dialog1里按钮（IDC_BUTTON1）点击：选择文件夹
    afx_msg void OnBnClickedButton1();



    DECLARE_MESSAGE_MAP()
};
