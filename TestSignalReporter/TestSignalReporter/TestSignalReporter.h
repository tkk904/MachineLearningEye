
// TestSignalReporter.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CTestSignalReporterApp:
// このクラスの実装については、TestSignalReporter.cpp を参照してください
//

class CTestSignalReporterApp : public CWinApp
{
public:
	CTestSignalReporterApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CTestSignalReporterApp theApp;
