// TWELiteSignalReporter.h : TWELiteSignalReporter DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CTWELiteSignalReporterApp
// このクラスの実装に関しては TWELiteSignalReporter.cpp をご覧ください
//

class CTWELiteSignalReporterApp : public CWinApp
{
public:
	CTWELiteSignalReporterApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
