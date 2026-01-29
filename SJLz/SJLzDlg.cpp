
// SJLzDlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "SJLz.h"
#include "SJLzDlg.h"
#include "afxdialogex.h"
#include "Dialog1.h"
#include "CDialog2.h"
#include <vector>
#include <shellapi.h> 
#include <afxlayout.h>
#include <Shlwapi.h>
#include <shlobj.h>  
#include <string>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Shlwapi.lib")






#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_CTX_SJL         51001
#define ID_CTX_OPEN        51002
#define ID_CTX_PROPERTIES  51003
#define ID_CTX_RENAME      51004
#define ID_CTX_COPY        51005
#define ID_CTX_MOVETO      51006

#pragma comment(lib, "Shlwapi.lib")


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
	ON_WM_SIZE()


	ON_BN_CLICKED(IDC_BUTTON1, &CSJLzDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSJLzDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSJLzDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSJLzDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSJLzDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSJLzDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSJLzDlg::OnBnClickedButton7)

	ON_EN_CHANGE(IDC_EDIT1, &CSJLzDlg::OnEnChangeEdit1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSJLzDlg::OnNMDblclkList1)
	//ON_LBN_SELCHANGE(IDC_LIST1, &CSJLzDlg::OnLbnSelchangeList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSJLzDlg::OnNMRClickList1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, &CSJLzDlg::OnLvnEndlabeleditList1)



	ON_COMMAND(ID_CTX_SJL, &CSJLzDlg::OnCtxSJL)
	ON_COMMAND(ID_CTX_OPEN, &CSJLzDlg::OnCtxOpen)
	ON_COMMAND(ID_CTX_PROPERTIES, &CSJLzDlg::OnCtxProperties)
	ON_COMMAND(ID_CTX_RENAME, &CSJLzDlg::OnCtxRename)
	ON_COMMAND(ID_CTX_COPY, &CSJLzDlg::OnCtxCopy)
	ON_COMMAND(ID_CTX_MOVETO, &CSJLzDlg::OnCtxMoveTo)

	//菜单命令
	ON_COMMAND(32771, &CSJLzDlg::OnMenuOpen)
	ON_COMMAND(ID_32773, &CSJLzDlg::OnMenuDelete)
	ON_COMMAND(ID_32774, &CSJLzDlg::OnMenuProperties)
	ON_COMMAND(ID_32775, &CSJLzDlg::OnMenuNewFolder)
	ON_COMMAND(ID_32777, &CSJLzDlg::OnMenuNewFile)
	ON_COMMAND(ID_32778, &CSJLzDlg::OnMenuExit)



	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CSJLzDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()



void CSJLzDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// 窗口还没创建完控件时会进 OnSize，先防一下
	if (!::IsWindow(m_list1.GetSafeHwnd()))
		return;

	// ====== 你可以按实际UI微调这几个边距/高度 ======
	const int margin = 12;          // 外边距
	const int gap = 10;             // 控件间距
	const int navH = 28;            // “上一级+地址栏”这一行高度（含控件高度）
	const int topAreaH = 90;        // 顶部按钮区域高度（根据你那排按钮实际高度来）

	// 取“上一级”按钮当前宽度（保持它不变）
	CRect rcUp;
	GetDlgItem(IDC_BUTTON7)->GetWindowRect(&rcUp);
	ScreenToClient(&rcUp);
	int upW = rcUp.Width();

	// ====== 布局第二行：上一级 + 地址栏 ======
	int yNav = margin + topAreaH; // 第二行的 y（顶上按钮区域下面）
	int x = margin;

	// 上一级按钮：位置固定（贴左）
	GetDlgItem(IDC_BUTTON7)->MoveWindow(x, yNav, upW, navH);

	// 地址栏：占满剩余宽度
	int editX = x + upW + gap;
	int editW = cx - margin - editX;
	if (editW < 50) editW = 50; // 防止太小
	GetDlgItem(IDC_EDIT1)->MoveWindow(editX, yNav, editW, navH);

	// ====== 布局列表：占据剩余空间 ======
	int listY = yNav + navH + gap;
	int listH = cy - margin - listY;
	if (listH < 50) listH = 50;

	GetDlgItem(IDC_LIST1)->MoveWindow(margin, listY, cx - 2 * margin, listH);
}



static void GetSelectedPaths(CMFCShellListCtrl& list, std::vector<CString>& out)
{
	out.clear();
	int i = -1;
	while ((i = list.GetNextItem(i, LVNI_SELECTED)) != -1)
	{
		CString path;
		if (list.GetItemPath(path, i) && !path.IsEmpty())
			out.push_back(path);
	}
}




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

	m_list1.ModifyStyle(0, LVS_EDITLABELS);


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





void CSJLzDlg::OnBnClickedButton1()
{
	std::vector<CString> selected;
	GetSelectedPaths(m_list1, selected);

	if (selected.empty())
	{
		AfxMessageBox(_T("请先选择文件或文件夹。"));
		return;
	}

	// 1) 默认源文件夹 = 当前列表正在显示的目录（最稳）
	CString baseFolder;
	if (!m_list1.GetCurrentFolder(baseFolder) || baseFolder.IsEmpty())
	{
		// 兜底：从第一个选中项推父目录
		baseFolder = selected[0];
		int pos = baseFolder.ReverseFind(_T('\\'));
		if (pos > 0) baseFolder = baseFolder.Left(pos);
	}

	// 去掉末尾 '\'
	while (!baseFolder.IsEmpty() &&
		(baseFolder.Right(1) == _T("\\") || baseFolder.Right(1) == _T("/")))
	{
		baseFolder.Delete(baseFolder.GetLength() - 1);
	}

	// 2) 如果只选中了“一个文件夹”，则源文件夹=该文件夹本身（你要的规则）
	if (selected.size() == 1)
	{
		DWORD attr = GetFileAttributes(selected[0]);
		if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
		{
			baseFolder = selected[0];
			// 同样去掉末尾 '\'
			while (!baseFolder.IsEmpty() &&
				(baseFolder.Right(1) == _T("\\") || baseFolder.Right(1) == _T("/")))
			{
				baseFolder.Delete(baseFolder.GetLength() - 1);
			}
		}
	}

	Dialog1 dlg(selected, baseFolder, this);
	dlg.DoModal();
}



void CSJLzDlg::OnBnClickedButton2()
{
	std::vector<CString> selected;
	GetSelectedPaths(m_list1, selected);

	if (selected.empty())
	{
		AfxMessageBox(_T("请先选择文件。"));
		return;
	}

	// 只保留 .SJL 文件
	std::vector<CString> sjlFiles;
	for (auto& s : selected)
	{
		if (s.Right(4).CompareNoCase(_T(".SJL")) == 0)
			sjlFiles.push_back(s);
	}

	if (sjlFiles.empty())
	{
		AfxMessageBox(_T("请选择 .SJL 文件。"));
		return;
	}

	CDialog2 dlg(sjlFiles, this);
	dlg.DoModal();
}
void CSJLzDlg::OnBnClickedButton3() {}
void CSJLzDlg::OnBnClickedButton4() {}
void CSJLzDlg::OnBnClickedButton5()
{
	// 1) 收集选中项路径
	std::vector<CString> paths;
	int idx = -1;
	while ((idx = m_list1.GetNextItem(idx, LVNI_SELECTED)) != -1)
	{
		CString p;
		if (m_list1.GetItemPath(p, idx) && !p.IsEmpty())
			paths.push_back(p);
	}

	// 2) 没选中：提示
	if (paths.empty())
	{
		AfxMessageBox(_T("未选定内容"));
		return;
	}

	// 3) 组织提示文本：把所有选中名称全部列出来
	CString selNames;
	selNames.Preallocate(4096);
	for (size_t i = 0; i < paths.size(); ++i)
	{
		CString name = PathFindFileName(paths[i]); // 只显示文件/文件夹名
		if (i > 0) selNames += _T("\r\n");
		selNames += name;
	}

	CString msg;
	msg.Format(_T("确定删除选定内容？\r\n\r\n%s"), selNames.GetString());

	// 4) 确认框：默认选“取消”更安全
	if (AfxMessageBox(msg, MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2) != IDOK)
		return;

	// 5) 执行删除：放入回收站（资源管理器 Delete 行为）
	std::wstring from;
	from.reserve(32768);
	for (const auto& p : paths)
	{
		from += (LPCWSTR)p;
		from.push_back(L'\0');
	}
	from.push_back(L'\0'); // 双0结尾

	SHFILEOPSTRUCTW op = { 0 };
	op.hwnd = this->GetSafeHwnd();
	op.wFunc = FO_DELETE;
	op.pFrom = from.c_str();
	op.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION; // 我们自己确认过了

	int ret = SHFileOperationW(&op);
	if (ret != 0 || op.fAnyOperationsAborted)
	{
		// 失败 / 用户在系统对话框里取消
		return;
	}

	// 6) 刷新当前目录列表
	CString cur;
	if (m_list1.GetCurrentFolder(cur) && !cur.IsEmpty())
		m_list1.DisplayFolder(cur);
}

void CSJLzDlg::OnBnClickedButton6() {
	OnCtxProperties();
}

void CSJLzDlg::OnBnClickedButton7()
{
	// DisplayParentFolder 返回 HRESULT
	HRESULT hr = m_list1.DisplayParentFolder();
	if (FAILED(hr))
	{
		AfxMessageBox(_T("无法进入父目录"));
	}
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


void CSJLzDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// 没选中项就不弹（你也可以改成照样弹）
	if (m_list1.GetSelectedCount() <= 0)
		return;

	CPoint pt;
	GetCursorPos(&pt); // 屏幕坐标

	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, ID_CTX_SJL, _T("SJL"));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_CTX_OPEN, _T("打开"));
	menu.AppendMenu(MF_STRING, ID_CTX_PROPERTIES, _T("属性"));
	menu.AppendMenu(MF_STRING, ID_CTX_RENAME, _T("重命名"));
	menu.AppendMenu(MF_STRING, ID_CTX_COPY, _T("复制"));
	menu.AppendMenu(MF_STRING, ID_CTX_MOVETO, _T("移动到"));

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}


void CSJLzDlg::OnCtxSJL() {
	OnBnClickedButton1();
}




void CSJLzDlg::OnCtxOpen()
{
	// 取第一个选中项
	int i = m_list1.GetNextItem(-1, LVNI_SELECTED);
	if (i < 0)
		return;

	CString path;
	if (!m_list1.GetItemPath(path, i) || path.IsEmpty())
		return;

	DWORD attr = GetFileAttributes(path);
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		// 是文件夹：进入
		m_list1.DisplayFolder(path);
	}
	else
	{
		// 是文件：打开
		::ShellExecute(nullptr, _T("open"), path, nullptr, nullptr, SW_SHOWNORMAL);
	}
}



void CSJLzDlg::OnCtxProperties()
{
	// 取第一个选中项
	int i = m_list1.GetNextItem(-1, LVNI_SELECTED);
	if (i < 0)
		return;

	CString path;
	if (!m_list1.GetItemPath(path, i) || path.IsEmpty())
		return;

	SHELLEXECUTEINFO sei = { 0 };
	sei.cbSize = sizeof(sei);
	sei.hwnd = this->GetSafeHwnd();
	sei.lpVerb = _T("properties");     // ✅ 系统属性页
	sei.lpFile = path;
	sei.nShow = SW_SHOW;
	sei.fMask = SEE_MASK_INVOKEIDLIST;

	::ShellExecuteEx(&sei);
}

void CSJLzDlg::OnCtxRename()
{
	int i = m_list1.GetNextItem(-1, LVNI_SELECTED);
	if (i < 0)
		return;

	m_list1.SetFocus();     // 让编辑框能正确显示/输入
	m_list1.EditLabel(i);   // ✅ 进入原地重命名编辑
}


void CSJLzDlg::OnCtxCopy()
{
	// 收集选中项路径
	std::vector<CString> paths;
	int idx = -1;
	while ((idx = m_list1.GetNextItem(idx, LVNI_SELECTED)) != -1)
	{
		CString p;
		if (m_list1.GetItemPath(p, idx) && !p.IsEmpty())
			paths.push_back(p);
	}
	if (paths.empty())
		return;

	// 计算 CF_HDROP 需要的总字符数（每条路径以 '\0' 结尾，最后再加一个 '\0'）
	size_t totalChars = 0;
	for (auto& p : paths)
		totalChars += (size_t)p.GetLength() + 1;
	totalChars += 1;

	// 分配全局内存：DROPFILES + 路径多字符串
	const size_t bytes = sizeof(DROPFILES) + totalChars * sizeof(wchar_t);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, bytes);
	if (!hMem) return;

	auto* df = (DROPFILES*)GlobalLock(hMem);
	if (!df) { GlobalFree(hMem); return; }

	df->pFiles = sizeof(DROPFILES);
	df->fWide = TRUE; // Unicode

	// 写入多字符串
	wchar_t* dst = (wchar_t*)((BYTE*)df + sizeof(DROPFILES));
	for (auto& p : paths)
	{
		wcscpy_s(dst, totalChars, (LPCWSTR)p);
		dst += p.GetLength() + 1;
	}
	*dst = L'\0'; // 结尾双0

	GlobalUnlock(hMem);

	// 打开剪贴板并写入
	if (!OpenClipboard())
	{
		GlobalFree(hMem);
		return;
	}

	EmptyClipboard();

	// 1) 文件列表（可被资源管理器粘贴）
	SetClipboardData(CF_HDROP, hMem); // 成功后内存归剪贴板管理，不能再 GlobalFree

	// 2) 告诉系统这是“复制”而不是“剪切”
	UINT fmt = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	if (fmt)
	{
		HGLOBAL hEff = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(DWORD));
		if (hEff)
		{
			DWORD* eff = (DWORD*)GlobalLock(hEff);
			if (eff)
			{
				*eff = DROPEFFECT_COPY;
				GlobalUnlock(hEff);
				SetClipboardData(fmt, hEff);
			}
			else
			{
				GlobalFree(hEff);
			}
		}
	}

	CloseClipboard();
}

void CSJLzDlg::OnCtxMoveTo() {}
//先空着，后续可以移动到同一局域网下的用户电脑等功能


void CSJLzDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


//菜单烂
void CSJLzDlg::OnMenuOpen()
{
	OnCtxOpen();  // 复用右键“打开”的逻辑
}

void CSJLzDlg::OnMenuDelete()
{
	OnBnClickedButton5();   // 复用 button5 的删除逻辑
}

void CSJLzDlg::OnMenuProperties()
{
	OnCtxProperties();   // 复用右键“属性”逻辑
}



void CSJLzDlg::OnLvnEndlabeleditList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = FALSE; // 默认不接受（会回滚到原名）

	auto* pDisp = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	if (!pDisp) return;

	// 用户按 ESC 或没改名时，pszText 可能为空
	if (!pDisp->item.pszText) return;

	CString newName = pDisp->item.pszText;
	newName.Trim();
	if (newName.IsEmpty()) return;

	// 禁止输入路径分隔符
	if (newName.Find(_T('\\')) >= 0 || newName.Find(_T('/')) >= 0)
	{
		AfxMessageBox(_T("名称不能包含 \\ 或 /"));
		return;
	}

	// 原始完整路径
	CString oldPath;
	if (!m_list1.GetItemPath(oldPath, pDisp->item.iItem) || oldPath.IsEmpty())
		return;

	// 父目录
	wchar_t parentBuf[MAX_PATH] = { 0 };
	wcsncpy_s(parentBuf, (LPCWSTR)oldPath, _TRUNCATE);
	if (!PathRemoveFileSpecW(parentBuf))
		return;

	CString newPath;
	newPath.Format(_T("%s\\%s"), parentBuf, newName.GetString());

	// 没变化直接接受
	if (newPath.CompareNoCase(oldPath) == 0)
	{
		*pResult = TRUE;
		return;
	}

	// 同名冲突
	if (PathFileExists(newPath))
	{
		AfxMessageBox(_T("已存在同名文件/文件夹，请换一个名字。"));
		return;
	}

	// ✅ 真正重命名（文件/文件夹都适用）
	if (!MoveFileExW(oldPath, newPath, MOVEFILE_COPY_ALLOWED))
	{
		DWORD err = GetLastError();
		CString msg;
		msg.Format(_T("重命名失败（错误码：%u）"), err);
		AfxMessageBox(msg);
		return;
	}

	*pResult = TRUE; // ✅ 接受编辑结果（不回滚）
}

void CSJLzDlg::OnMenuNewFolder()
{
	// 1) 获取当前显示目录
	CString curFolder;
	if (!m_list1.GetCurrentFolder(curFolder) || curFolder.IsEmpty())
	{
		AfxMessageBox(_T("当前不是有效的文件夹位置。"));
		return;
	}

	// 2) 确认是文件系统目录（“此电脑”等虚拟位置不允许新建）
	DWORD attr = GetFileAttributes(curFolder);
	if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		AfxMessageBox(_T("当前不是磁盘文件夹，无法新建文件夹。"));
		return;
	}

	// 3) 生成不冲突的文件夹名：新建文件夹 / 新建文件夹 (2) / ...
	CString baseName = _T("新建文件夹");
	CString newName = baseName;

	CString fullPath;
	fullPath.Format(_T("%s\\%s"), curFolder.GetString(), newName.GetString());

	int n = 2;
	while (PathFileExists(fullPath))
	{
		newName.Format(_T("%s (%d)"), baseName.GetString(), n++);
		fullPath.Format(_T("%s\\%s"), curFolder.GetString(), newName.GetString());
	}

	// 4) 创建目录（更像系统行为）
	int rc = SHCreateDirectoryEx(this->GetSafeHwnd(), fullPath, nullptr);
	if (rc != ERROR_SUCCESS && rc != ERROR_FILE_EXISTS && rc != ERROR_ALREADY_EXISTS)
	{
		CString msg;
		msg.Format(_T("新建文件夹失败（错误码：%d）"), rc);
		AfxMessageBox(msg);
		return;
	}

	// 5) 刷新列表
	HRESULT hr = m_list1.DisplayFolder(curFolder);
	if (FAILED(hr))
		return;

	// 6) 找到新建的那一项并进入原地重命名
	int found = -1;
	int itemCount = m_list1.GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		CString p;
		if (m_list1.GetItemPath(p, i) && !p.IsEmpty())
		{
			if (p.CompareNoCase(fullPath) == 0)
			{
				found = i;
				break;
			}
		}
	}

	if (found >= 0)
	{
		m_list1.SetFocus();
		m_list1.SetItemState(found, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_list1.EnsureVisible(found, FALSE);
		m_list1.EditLabel(found); // ✅ 像资源管理器一样立刻重命名
	}
}

void CSJLzDlg::OnMenuNewFile()
{
	// 1) 当前目录
	CString curFolder;
	if (!m_list1.GetCurrentFolder(curFolder) || curFolder.IsEmpty())
	{
		AfxMessageBox(_T("当前不是有效的文件夹位置。"));
		return;
	}

	DWORD attr = GetFileAttributes(curFolder);
	if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		AfxMessageBox(_T("当前不是磁盘文件夹，无法新建文件。"));
		return;
	}

	// 2) 生成不冲突的文件名：新建文件.txt / 新建文件 (2).txt / ...
	CString baseName = _T("新建文件");
	CString ext = _T(".txt");
	CString fileName = baseName + ext;

	CString fullPath;
	fullPath.Format(_T("%s\\%s"), curFolder.GetString(), fileName.GetString());

	int n = 2;
	while (PathFileExists(fullPath))
	{
		fileName.Format(_T("%s (%d)%s"), baseName.GetString(), n++, ext.GetString());
		fullPath.Format(_T("%s\\%s"), curFolder.GetString(), fileName.GetString());
	}

	// 3) 创建空文件
	HANDLE h = CreateFile(fullPath, GENERIC_WRITE, FILE_SHARE_READ, nullptr,
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (h == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		CString msg;
		msg.Format(_T("新建文件失败（错误码：%u）"), err);
		AfxMessageBox(msg);
		return;
	}
	CloseHandle(h);

	// 4) 刷新列表
	HRESULT hr = m_list1.DisplayFolder(curFolder);
	if (FAILED(hr))
		return;

	// 5) 找到新建项并原地重命名（像资源管理器）
	int found = -1;
	int itemCount = m_list1.GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		CString p;
		if (m_list1.GetItemPath(p, i) && !p.IsEmpty())
		{
			if (p.CompareNoCase(fullPath) == 0)
			{
				found = i;
				break;
			}
		}
	}

	if (found >= 0)
	{
		m_list1.SetFocus();
		m_list1.SetItemState(found, LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED);
		m_list1.EnsureVisible(found, FALSE);
		m_list1.EditLabel(found); // ✅ 进入编辑
	}
}

void CSJLzDlg::OnMenuExit()
{
	SendMessage(WM_CLOSE);   // 等同右上角“×”
}

