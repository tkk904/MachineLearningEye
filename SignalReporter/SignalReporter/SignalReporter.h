#pragma once

#include <tchar.h>
#include <string>


#ifdef SIGNALREPORTER_EXPORTS
#define SIGNALREPORTER_API __declspec(dllexport)
#else
#define SIGNALREPORTER_API __declspec(dllimport) 
#endif

//using tstring = std::basic_string<TCHAR>;

extern "C" {
	/**
	* @fn bool Start(char* PortName)
	* @brief 外部接続機器（出力側）の指定されたポート名の接続を確立する。
	* @param[in] PortName ポート名
	* @return bool オープン結果を返す　オープン成功時にはtrue。それ以外はfalseを返す
	*/
	SIGNALREPORTER_API bool Start(char* PortName);

	/**
	* @fn void StopNotify()
	* @brief  外部接続機器（出力側）クローズ関数
	* @return なし 
	*/
	SIGNALREPORTER_API void StopNotify();

	/**
	* @fn void Notify(int time)
	* @brief  外部接続機器（出力側）。通知実行を指示する処理。具体的には、timeで指定された時間、LEDを点灯する処理
	* @param[in] time 通知実行の指示時間。指定された時間、通知を行う。
	* @return なし
	*/
	SIGNALREPORTER_API void Notify(int time);
}

