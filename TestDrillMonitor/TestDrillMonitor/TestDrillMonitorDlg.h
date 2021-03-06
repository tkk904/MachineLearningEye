
// TestDrillMonitorDlg.h : ヘッダー ファイル
//

#pragma once

#include "DrillMonitor.h"

// CTestDrillMonitorDlg ダイアログ
class CTestDrillMonitorDlg : public CDialog
{
	HMODULE m_hDrillMonitorDLL;
	IDrillMonitor* m_pDrillMonitor;
	UINT_PTR m_timerID;
	bool m_bRunning;
// コンストラクション
public:
	CTestDrillMonitorDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTDRILLMONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnDropdownPrimado2PortList();
	afx_msg void OnCbnDropdownTsnd121PortList();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPlay();
	DECLARE_MESSAGE_MAP()
};
