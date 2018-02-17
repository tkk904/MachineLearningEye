//
//
//

#ifndef TWELITE_SIGNAL_REPOTER_H_
#define	TWELITE_SIGNAL_REPOTER_H_

#include "SignalReporter.h"
#include "SerialPort.h"

class CTWELiteSignalReorter : public ISignalReporter
{
	CSerialPort m_port;
public:
	virtual void Notify(int msec);
	virtual bool Start(const char* portname);
	virtual void Stop();
	//
	static CTWELiteSignalReorter& Instance();
private:
	CTWELiteSignalReorter();
	CTWELiteSignalReorter(const CTWELiteSignalReorter&);
};

#endif // !TWELITE_SIGNAL_REPOTER_H_
