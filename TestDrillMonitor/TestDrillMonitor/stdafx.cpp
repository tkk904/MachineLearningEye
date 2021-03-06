
// stdafx.cpp : 標準インクルード [!output PROJECT_NAME].pch のみを
// TestDrillMonitor.pch はプリコンパイル済みヘッダーになります
// stdafx.obj にはプリコンパイル済み型情報が含まれます。

#include "stdafx.h"


#include <setupapi.h>
#include <Ntddser.h>
extern "C" {
#include "hidsdi.h"
}
#pragma comment(lib,"hid.lib")
#pragma comment(lib,"setupapi.lib")
static void EnumSerialPort(CStringArray& list)
{
	HDEVINFO hDevInfo;
	DWORD MemberIndex = 0;
	SP_DEVINFO_DATA Data = { sizeof(SP_DEVINFO_DATA) };

	hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);	// デバイス情報セットを取得
	if (hDevInfo == 0) {	//	デバイス情報セットが取得できなかった場合
		return;
	}
	Data.cbSize = sizeof(Data);
	DWORD index = 0;
	while (SetupDiEnumDeviceInfo(hDevInfo, index, &Data)) {	// デバイスインターフェイスの取得
		DWORD dataT;
		DWORD size;
		LPTSTR buf;
		//	COM ポート名の取得
		HKEY key = SetupDiOpenDevRegKey(hDevInfo, &Data, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
		if (key) {
			TCHAR name[256];
			DWORD type = 0;
			size = sizeof(name);
			RegQueryValueEx(key, _T("PortName"), NULL, &type, (LPBYTE)name, &size);
			list.Add(name);
			TRACE(L"%s", name);
		}
		//	デバイスの説明を取得
		size = 0;
		buf = NULL;
		while (!SetupDiGetDeviceRegistryProperty(hDevInfo, &Data, SPDRP_DEVICEDESC, &dataT, (PBYTE)buf, size, &size)) {
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
				break;
			}
			if (buf) {
				LocalFree(buf);
			}
			buf = (LPTSTR)LocalAlloc(LPTR, size * 2);
		}
		TRACE(L"(%s)\n", buf);
		if (buf) {
			LocalFree(buf);
		}
		index++;
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);	//	デバイス情報セットを解放
}
void EnumSerialPort(CComboBox& combobox)
{
	CStringArray list;
	EnumSerialPort(list);
	combobox.ResetContent();
	for (unsigned int i = 0; i < list.GetSize(); i++) {
		combobox.AddString(list[i]);
	}
}
