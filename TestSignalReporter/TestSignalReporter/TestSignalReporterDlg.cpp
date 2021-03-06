
// TestSignalReporterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TestSignalReporter.h"
#include "TestSignalReporterDlg.h"
#include "afxdialogex.h"
#include "SignalReporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CTestSignalReporterDlg ダイアログ

class CNullSignalReporter : public ISignalReporter
{
public:
	virtual void Notify(int msec);
	virtual bool Start(const char* portname);
	virtual void Stop();
	//
	CNullSignalReporter();
};
CNullSignalReporter::CNullSignalReporter()
{
}
void CNullSignalReporter::Notify(int msec)
{
}
bool CNullSignalReporter::Start(const char* portname)
{
	return true;
}
void CNullSignalReporter::Stop()
{
}

CTestSignalReporterDlg::CTestSignalReporterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TESTSIGNALREPORTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//
	m_hSignalReporterDLL = LoadLibrary(_T("TWELiteSignalReporter.dll"));
	FARPROC GetSignalReporter = GetProcAddress(m_hSignalReporterDLL, "SignalReporter");
	m_pSignalReporter = NULL;
	if (GetSignalReporter != NULL) {
		m_pSignalReporter = (ISignalReporter*)GetSignalReporter();
	}
	if (m_pSignalReporter == NULL) {
		static CNullSignalReporter c_reporter;
		m_pSignalReporter = &c_reporter;
	}
	m_bRunning = false;
}

void CTestSignalReporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestSignalReporterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CTestSignalReporterDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_OUTPUT, &CTestSignalReporterDlg::OnBnClickedOutput)
	ON_CBN_DROPDOWN(IDC_PORT_LIST, &CTestSignalReporterDlg::OnCbnDropdownPortList)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestSignalReporterDlg メッセージ ハンドラー

BOOL CTestSignalReporterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CTestSignalReporterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CTestSignalReporterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CTestSignalReporterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CTestSignalReporterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_RETURN:
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTestSignalReporterDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	//	m_pDrillMonitor->Finalize();
	if (m_hSignalReporterDLL != NULL) {
		FreeLibrary(m_hSignalReporterDLL);
		m_hSignalReporterDLL = NULL;
	}
}

extern void EnumSerialPort(CComboBox& combobox);
static CString PortName(CWnd* pWnd)
{
	CComboBox* pCombo = reinterpret_cast<CComboBox*>(pWnd);
	CString s;
	int index = pCombo->GetCurSel();
	if (0 <= index) {
		pCombo->GetLBText(index, s);
	}
	return s;
}
void CTestSignalReporterDlg::OnCbnDropdownPortList()
{
	EnumSerialPort(*reinterpret_cast<CComboBox*>(GetDlgItem(IDC_PORT_LIST)));
}
void CTestSignalReporterDlg::OnBnClickedStart()
{
	if (m_bRunning) {
		m_pSignalReporter->Stop();
		m_bRunning = false;
		SetDlgItemText(IDC_START, _T("Start"));
	}
	else {
		CString portame = PortName(GetDlgItem(IDC_PORT_LIST));
		if (m_pSignalReporter->Start(portame)) {
			m_bRunning = true;
			SetDlgItemText(IDC_START, _T("Stop"));
		}
	}
}
void CTestSignalReporterDlg::OnBnClickedOutput()
{
	m_pSignalReporter->Notify(1000);
}

