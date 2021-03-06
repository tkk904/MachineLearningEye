
// TestSignalReporterDlg.h : ヘッダー ファイル
//

#pragma once

class ISignalReporter;

// CTestSignalReporterDlg ダイアログ
class CTestSignalReporterDlg : public CDialog
{
	HMODULE m_hSignalReporterDLL;
	ISignalReporter* m_pSignalReporter;
	bool m_bRunning;
	// コンストラクション
public:
	CTestSignalReporterDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSIGNALREPORTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedOutput();
	afx_msg void OnCbnDropdownPortList();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};
