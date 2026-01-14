
// SJLzDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SJLz.h"
#include "SJLzDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSJLzDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &CSJLzDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSJLzDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSJLzDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSJLzDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSJLzDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSJLzDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSJLzDlg::OnBnClickedButton7)

	ON_EN_CHANGE(IDC_EDIT1, &CSJLzDlg::OnEnChangeEdit1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSJLzDlg::OnNMDblclkList1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSJLzDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()




// CSJLzDlg 对话框



CSJLzDlg::CSJLzDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SJLZ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSJLzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_BUTTON3, m_btn3);
	DDX_Control(pDX, IDC_BUTTON4, m_btn4);
	DDX_Control(pDX, IDC_BUTTON5, m_btn5);
	DDX_Control(pDX, IDC_BUTTON6, m_btn6);
	DDX_Control(pDX, IDC_BUTTON7, m_btn7);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_LIST1, m_list1);
}


// CSJLzDlg 消息处理程序

BOOL CSJLzDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSJLzDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSJLzDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSJLzDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CSJLzDlg::OnBnClickedButton1() {}
void CSJLzDlg::OnBnClickedButton2() {}
void CSJLzDlg::OnBnClickedButton3() {}
void CSJLzDlg::OnBnClickedButton4() {}
void CSJLzDlg::OnBnClickedButton5() {}
void CSJLzDlg::OnBnClickedButton6() {}
void CSJLzDlg::OnBnClickedButton7()
{
	CString cur;
	// 关键：获取当前显示的文件夹
	if (!m_list1.GetCurrentFolder(cur) || cur.IsEmpty())
		return;

	// 去掉末尾可能的 '\'
	while (!cur.IsEmpty() && (cur.Right(1) == _T("\\") || cur.Right(1) == _T("/")))
		cur.Delete(cur.GetLength() - 1);

	// 找到最后一个 '\'
	int pos = cur.ReverseFind(_T('\\'));
	if (pos <= 2)
	{
		// pos <= 2 基本就是 "C:\" 这种根目录了（"C:"长度2，根目录特殊）
		// 根目录就不再往上退
		return;
	}

	CString parent = cur.Left(pos);

	// 进入父目录
	m_list1.DisplayFolder(parent);
}


void CSJLzDlg::OnEnChangeEdit1() {}

void CSJLzDlg::OnLbnSelchangeList1() {}


void CSJLzDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	auto* pAct = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (!pAct || pAct->iItem < 0)
		return;

	CString path;
	if (!m_list1.GetItemPath(path, pAct->iItem))
		return;

	DWORD attr = GetFileAttributes(path);
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		m_list1.DisplayFolder(path);
	}
}
