#pragma once

#pragma once

#include <tchar.h>
#include <string>

#ifdef SVMCORE_EXPORTS
#define SVMCORE_API __declspec(dllexport)
#else
#define SVMCORE_API __declspec(dllimport) 
#endif

using tstring = std::basic_string<TCHAR>;

extern "C" {
	SVMCORE_API void Training();
	SVMCORE_API int Predict(float a,float b, int datasize);
	SVMCORE_API void ClearDefectFlg();
}

