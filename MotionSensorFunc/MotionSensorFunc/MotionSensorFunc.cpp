// MotionSensorFunc.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "MotionSensorFunc.h"
#include <iostream>

#ifdef UNICODE
static auto& tcout = ::std::wcout;
static auto& tcin = ::std::wcin;
#else
static auto& tcout = ::std::cout;
static auto& tcin = ::std::cin;
#endif

bool Initialize()
{
	return true;
}

void Finilize()
{
}

bool Start(char* SpindleComName, char* MotionComName)
{
	std::cout << SpindleComName << std::endl;
	return true;
}

void Stop()
{
}

int  GetData(double* Data, int DataSize) 
{
	Data[0] = rand()%5;
	Data[1] = rand() % 3;
	Data[2] = rand() % 4;
	return 0;
}

int  GetError()
{
	return 0;
}
