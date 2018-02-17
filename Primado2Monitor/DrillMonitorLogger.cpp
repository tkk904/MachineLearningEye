//
//
//

#include "stdafx.h"
#include "DrillMonitorLogger.h"

CDrillMonitorLogger::CDrillMonitorLogger()
	: m_allocpoints(1000)
{
	m_buffer = NULL;
	m_points = 0;
}
CDrillMonitorLogger::~CDrillMonitorLogger()
{
	delete m_buffer;
}
bool CDrillMonitorLogger::IsEmty() const
{
	return m_points <= 0;
}
unsigned int CDrillMonitorLogger::Size() const
{
	return m_allocpoints;
}
unsigned int CDrillMonitorLogger::Length() const
{
	return m_points;
}
void CDrillMonitorLogger::Initialize()
{
	m_points = 0;
	try {
		if (m_buffer == NULL) {
			m_buffer = new DrillLogData[m_allocpoints];
		}
		m_end = m_buffer + m_allocpoints;
	}
	catch (CException* e) {
		e->ReportError();
		m_end = m_buffer = NULL;
		return;
	}
	m_ptr = m_buffer;
}
void CDrillMonitorLogger::Empty()
{
	m_points = 0;
	m_ptr = m_buffer;
}
void CDrillMonitorLogger::Add(const DrillLogData& data)
{
	if (m_points < m_allocpoints) {
		m_points++;
	}
	*m_ptr = data;
	if (m_end <= ++m_ptr) {
		m_ptr = m_buffer;
	}
}
bool CDrillMonitorLogger::Read(DrillLogData& data)
{
	if (m_rc == 0) {
		static const DrillLogData nulldata = { 0,0,0,0,0,0,0,0 };
		return false;
	}
	m_rc--;
	data = *m_ptr;
	if (m_end <= ++m_ptr) {
		m_ptr = m_buffer;
	}
	return true;
}

bool CDrillMonitorLogger::Save(LPCTSTR pathname)
{
	CFile file;
	CFileException fe;
	if (!file.Open(pathname, CFile::modeWrite | CFile::modeCreate, &fe)) {
		fe.ReportError();
		return false;
	}
	unsigned int cc = m_points;
	const DrillLogData* ptr = m_ptr - m_points;
	if (ptr < m_buffer) {
		int n = m_buffer - ptr;
		ptr = m_end - n;
		if (0 < n) {
			file.Write(ptr, sizeof(*ptr)*n);
			ptr = m_buffer;
			cc -= n;
		}
	}
	if (0 < cc) {
		file.Write(ptr, sizeof(*ptr)*cc);
	}
	file.Close();
	return true;
}
bool CDrillMonitorLogger::Load(LPCTSTR pathname)
{
	m_points = 0;
	m_ptr = m_buffer;
	//
	CFile file;
	CFileException fe;
	if (!file.Open(pathname, CFile::modeRead, &fe)) {
		fe.ReportError();
		return false;
	}
	m_rc = m_points = file.Read(m_buffer, sizeof(m_buffer[0])*m_allocpoints) / sizeof(m_buffer[0]);
	file.Close();
	return true;
}

#ifdef TEST_LOGGER

static void dump(const DrillLogData& data)
{
	TRACE("%04d : %d %.3f", data.time, data.rpm, data.ampere);
	TRACE("  %d %d %d", data.accelerate[0], data.accelerate[1], data.accelerate[2]);
	TRACE("  %d %d %d\n", data.angularvel[0], data.angularvel[1], data.angularvel[2]);
}
void CDrillMonitorLogger::Dump() const
{
	const DrillLogData* ptr = m_buffer;
	for (unsigned int i = 0; i < m_allocpoints; i++) {
		dump(*ptr);
		ptr++;
	}
}
void Test()
{
	CDrillMonitorLogger log;
	DrillLogData data;
	memset(&data, 0, sizeof(data));

	log.Initialize();
	int i, n;
	n = log.Size();
	n += 3;
	for (i = 0; i < n; i++) {
		data.time = i;
		log.Add(data);
	}
	log.Save("TEST.DAT");
	log.Load("TEST.DAT");
	log.Dump();
	for (i = 0; i < n; i++) {
		const DrillLogData& rd = log.Read();
		TRACE("%d %d\n", i, rd.time);
	}
}

#endif // TEST_LOGGER
