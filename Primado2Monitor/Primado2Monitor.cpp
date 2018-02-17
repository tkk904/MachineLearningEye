//
//
//

#include "stdafx.h"
#include "Primado2Monitor.h"

#define	T_SAMPLING	20	// 20msec

CPrimado2Monitor::CPrimado2Monitor()
{
}
CPrimado2Monitor::~CPrimado2Monitor()
{
}
CPrimado2Monitor& CPrimado2Monitor::Instance()
{
	static CPrimado2Monitor c_instance;
	return c_instance;
}
int  CPrimado2Monitor::GetError()
{
	return 0;
}
bool CPrimado2Monitor::Initialize()
{
	m_logger.Initialize();
	return true;
}
void CPrimado2Monitor::Finalize()
{
	Stop();
}

bool CPrimado2Monitor::Start(const char* primado2, const char* tsnd121, const char* filename)
{
	Stop();
	//
	m_logname = filename;
	m_logger.Empty();
	m_enablecounter = 0;
	m_bEnableLogging = false;
	//
	if (!m_primado2.Start(primado2)) {
		return false;
	}
	if (!m_tsnd121.Start(tsnd121)) {
		m_primado2.Stop();
		return false;
	}
	m_timer.Start(T_SAMPLING, SamplingHandler, this);
	return true;
}
void CPrimado2Monitor::Stop()
{
	m_timer.Stop();
	m_primado2.Stop();
	m_tsnd121.Stop();
	//
	if (!m_logger.IsEmty() && !m_logname.IsEmpty()) {
		m_logger.Save(m_logname);
		m_logger.Empty();
	}
}

int CPrimado2Monitor::GetData(double* ptr, int count)
{
	if (sizeof(m_data) / sizeof(m_data[0]) < count) {
		count = sizeof(m_data) / sizeof(m_data[0]);
	}
	memcpy(ptr, m_data, sizeof(m_data[0])*count);
	return count;
}
void CPrimado2Monitor::Sampling()
{
	long rpm = m_primado2.RPM();
	m_data[0] = static_cast<double>(rpm);
	m_data[1] = m_primado2.Ampere();
	for (int i = 0; i < 3; i++) {
		m_data[2 + i] = m_tsnd121.Accelerate(i);
		m_data[2 + 3 + i] = m_tsnd121.AngularVelocity(i);
	}
	if (0 < rpm) {
		m_enablecounter = 10;
		m_bEnableLogging = true;
	} else if(0 < m_enablecounter){
		m_bEnableLogging = 0 < --m_enablecounter;
	}
	if (m_bEnableLogging) {
		const DrillLogData* ptr = reinterpret_cast<const DrillLogData*>(m_data);
		m_logger.Add(*ptr);
	}
}
void CPrimado2Monitor::SamplingHandler(void* parameter)
{
	static_cast<CPrimado2Monitor*>(parameter)->Sampling();
}

void CPrimado2Monitor::PlayData()
{
	DrillLogData* ptr = reinterpret_cast<DrillLogData*>(m_data);
	m_logger.Read(*ptr);
}
void CPrimado2Monitor::PlayHandler(void* parameter)
{
	static_cast<CPrimado2Monitor*>(parameter)->PlayData();
}
void CPrimado2Monitor::Play(const char* filename)
{
	m_timer.Stop();
	//
	m_logname.Empty();
	m_logger.Empty();
	m_logger.Load(filename);
	m_timer.Start(T_SAMPLING, PlayHandler, this);
}

