//
//
//

#ifndef PRIMADO2_MONITOR_H_
#define PRIMADO2_MONITOR_H_

#include "DrillMonitor.h"

#include "TSND121.h"
#include "Primado2.h"
#include "MMTimer.h"
#include "DrillMonitorLogger.h"

class CPrimado2Monitor : public IDrillMonitor
{
	CPrimado2 m_primado2;
	CTSND121 m_tsnd121;
	double m_data[8];
	CMultiMediaTimer m_timer;
	bool m_bEnableLogging;
	unsigned int m_enablecounter;
	CString m_logname;
	CDrillMonitorLogger m_logger;
public:
	virtual int  GetData(double* ptr, int count);
	virtual int  GetError();
	virtual bool Start(const char* spindle, const char* sensor, const char* filename);
	virtual void Stop();
	virtual void Play(const char* filename);
	virtual bool Initialize();
	virtual void Finalize();
	//
	static CPrimado2Monitor& Instance();
private:
	void Sampling();
	static void SamplingHandler(void* parameter);
	void PlayData();
	static void PlayHandler(void* parameter);
	//
	CPrimado2Monitor();
	virtual ~CPrimado2Monitor();
};

#endif // !PRIMADO2_MONITOR_H_
