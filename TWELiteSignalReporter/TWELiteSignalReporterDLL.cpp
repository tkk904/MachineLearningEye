// TWELiteSignalReporter.cpp : DLL の初期化ルーチンを定義します。
//

#include "stdafx.h"
#include "TWELiteSignalReporterDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: この DLL が MFC DLL に対して動的にリンクされる場合、
//		MFC 内で呼び出されるこの DLL からエクスポートされたどの関数も
//		関数の最初に追加される AFX_MANAGE_STATE マクロを
//		持たなければなりません。
//
//		例:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 通常関数の本体はこの位置にあります
//		}
//
//		このマクロが各関数に含まれていること、MFC 内の
//		どの呼び出しより優先することは非常に重要です。
//		it は、次の範囲内で最初のステートメントとして表示されるべきです
//		らないことを意味します、コンストラクターが MFC
//		DLL 内への呼び出しを行う可能性があるので、オブ
//		ジェクト変数の宣言よりも前でなければなりません。
//
//		詳細については MFC テクニカル ノート 33 および
//		58 を参照してください。
//

// CTWELiteSignalReporterApp

BEGIN_MESSAGE_MAP(CTWELiteSignalReporterApp, CWinApp)
END_MESSAGE_MAP()


// CTWELiteSignalReporterApp の構築

CTWELiteSignalReporterApp::CTWELiteSignalReporterApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CTWELiteSignalReporterApp オブジェクト

CTWELiteSignalReporterApp theApp;


// CTWELiteSignalReporterApp の初期化

BOOL CTWELiteSignalReporterApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#define DllExport   __declspec( dllexport )

#include "TWELiteSignalReporter.h"
static ISignalReporter& Reporter()
{
	return CTWELiteSignalReorter::Instance();
}

extern "C" DllExport ISignalReporter* SignalReporter()
{
	return &Reporter();
}

extern "C" DllExport bool Start(const char* portname)
{
	return Reporter().Start(portname);
}
extern "C" DllExport void Stop()
{
	Reporter().Stop();
}

extern "C" DllExport void Notify(int msec)
{
	Reporter().Notify(msec);
}
