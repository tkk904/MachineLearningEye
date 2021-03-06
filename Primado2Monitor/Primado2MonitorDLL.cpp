// DrillMonitor.cpp : DLL の初期化ルーチンを定義します。
//

#include "stdafx.h"
#include "resource.h"		// メイン シンボル

#include "DrillMonitor.h"

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

// CDrillMonitorApp
class CDrillMonitorApp : public CWinApp
{
public:
	//
	CDrillMonitorApp();

	// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CDrillMonitorApp, CWinApp)
END_MESSAGE_MAP()


// CDrillMonitorApp の構築

CDrillMonitorApp::CDrillMonitorApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CDrillMonitorApp オブジェクト

CDrillMonitorApp theApp;


// CDrillMonitorApp の初期化

BOOL CDrillMonitorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#include "Primado2Monitor.h"
static CPrimado2Monitor& Sensor()
{
	return CPrimado2Monitor::Instance();
}

#define DllExport   __declspec( dllexport )
/**
* @fn IDrillMonitor* DrillMonitor()
* @brief IDrillMonitorを継承しているクラスのインスタンスを返す
* @return IDrillMonitor* インスタンスを返す 
*/
extern "C" DllExport IDrillMonitor* DrillMonitor()
{
	return &Sensor();
}

/**
* @fn bool Initialize()
* @brief  外部接続機器（入力側）。初期化関数。内部フラグをクリアしたり、通信をするための準備を行う。
* @return bool 初期化が成功したたtrueを返す
*/
extern "C" DllExport bool Initialize()
{
	return Sensor().Initialize();
}

/**
* @fn void Finilize()
* @brief 外部接続機器（入力側）。終了処理。通信をするためのハンドルの削除を行う。
* @return なし
*/
extern "C" DllExport void Finilize()
{
	Sensor().Finalize();
}

/**
* @fn bool Start(const char* SpindleComName, const char* MotionComName,const char* filename)
* @brief 外部接続機器（入力側）の指定されたポート名の接続を確立する。filenameにファイル名が設定されている場合にはデータ保存を行う。
* @param[in] SpindleComName 接続先ポート名
* @param[in] MotionComName 加速度センサーの接続先ポート名
* @param[in] filename データ保存ファイル名
* @return なし
*/
extern "C" DllExport bool Start(const char* SpindleComName, const char* MotionComName,const char* filename)
{
	return Sensor().Start(SpindleComName, MotionComName, filename);
}

/**
* @fn void Stop()
* @brief　 外部接続機器（入力側）。Start関数で指定されたポート名との接続を閉じる
* @return なし
*/
extern "C" DllExport void Stop()
{
	Sensor().Stop();
}

/**
* @fn int  GetData(double* Data, int DataSize)
* @brief 外部接続機器（入力側）。接続した機器からデータを取得し、Dataに格納し呼び出し元に渡す
* @param[inout] Data 接続先ポート名
* @param[in] DataSize データサイズ
* @return int 取得できたデータサイズを返す
*/
extern "C" DllExport int  GetData(double* Data, int DataSize)
{
	return Sensor().GetData(Data, DataSize);
}

/**
* @fn int  GetError()
* @brief　（未実装）エラー取得関数。
* @return なし
*/
extern "C" DllExport int  GetError()
{
	return 0;
}

/**
* @fn void  Play(const char* filename)
* @brief 外部接続機器（入力側）。ログファイルからデータを取得するモードで動作するよう指示する関数
* @param[in] filename ログファイル名
* @return なし
*/
extern "C" DllExport void  Play(const char* filename)
{
	Sensor().Play(filename);
}
