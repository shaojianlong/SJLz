// SJLzDlg.h
#pragma once
#include <afxshelllistctrl.h> 
#include <afxcmn.h>   // 常用通知结构：NMHDR、NMLISTVIEW、NMITEMACTIVATE 等




class CSJLzDlg : public CDialogEx
{
public:
    CSJLzDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SJLZ_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    // 实现
protected:
    HICON m_hIcon;

    // ✅ 这些在 cpp 里有实现，头文件也必须声明
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // ✅ 控件成员（DDX）
    CButton  m_btn1, m_btn2, m_btn3, m_btn4, m_btn5, m_btn6, m_btn7;
    CEdit    m_edit1;

    CMFCShellListCtrl m_list1; 
    // ✅ 控件消息
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedButton6();
    afx_msg void OnBnClickedButton7();
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnLbnSelchangeList1();

    afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);


    DECLARE_MESSAGE_MAP()
};
