#pragma once

#pragma once

#include <tchar.h>
#include <string>

#ifdef SVMCORE_EXPORTS
#define SVMCORE_API __declspec(dllexport)
#else
#define SVMCORE_API __declspec(dllimport) 
#endif

/*! \mainpage MachineLearningEye
*
* \section intro_sec 概要
*
*MachineLearningEyeの主な機能は、1つのEXEファイル、3つのDLLファイルから構成されている。
*Doxygen上で3つのDLLのインターフェースについてドキュメント化する。
*3つのDLLはC++で記述されている。
*また、MachineLearningEye、1つのEXEファイルは、C\#で記述されている。
*言語的にはC\#からC++のDLLを呼び出して使っていることになる。
*
*/

extern "C" {
	/**
	* @fn void Training()
	* @brief サポートベクターマシンを使って貫通状況を学習する関数
	*/
	SVMCORE_API void Training();

	/**
	* @fn int Predict(float a,float b, int datasize)
	* @brief サポートベクターマシンを使って貫通状況を予測する関数
	* @param[in] a 電流値
	* @param[in] b 回転数
	* @param[in] datasize 引数の数
	* @return int 予測結果を返す　貫通と判断した場合には1を返す。それ以外は0を返す
	*/
	SVMCORE_API int Predict(float a,float b, int datasize);

	/**
	* @fn void ClearDefectFlg()
	* @brief サポートベクターマシン内部で使用されている判断フラグをクリアする
	*/
	SVMCORE_API void ClearDefectFlg();
}

