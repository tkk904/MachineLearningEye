//
//
//

#include "stdafx.h"
#include "TWELiteSignalReporter.h"

CTWELiteSignalReorter::CTWELiteSignalReorter()
{
}
CTWELiteSignalReorter& CTWELiteSignalReorter::Instance()
{
	static CTWELiteSignalReorter c_instance;
	return c_instance;
}

void CTWELiteSignalReorter::Notify(int msec)
{
	if (m_port.IsConnected()) {
#define DO_ON	":7880010101000000000000000005\r\n"
		m_port.Write(DO_ON, sizeof(DO_ON));
		Sleep(msec);
		if (m_port.IsConnected()) {
#define DO_OFF	":7880010001000000000000000006\r\n"
			m_port.Write(DO_OFF, sizeof(DO_OFF));
		}
	}
}
bool CTWELiteSignalReorter::Start(const char* portname)
{
	m_port.Close();
	if (!m_port.Open(portname, 115200, CSerialPort::N81)) {
		return false;
	}
	return true;
}
void CTWELiteSignalReorter::Stop()
{
	m_port.Close();
}
