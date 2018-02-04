#pragma once

#include <tchar.h>
#include <string>


#ifdef SIGNALREPORTER_EXPORTS
#define SIGNALREPORTER_API __declspec(dllexport)
#else
#define SIGNALREPORTER_API __declspec(dllimport) 
#endif

using tstring = std::basic_string<TCHAR>;

extern "C" {
	SIGNALREPORTER_API bool Start(char* PortName);
	SIGNALREPORTER_API void StopNotify();
	SIGNALREPORTER_API void Notify(int time);
}

