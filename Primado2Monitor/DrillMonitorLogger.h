//
//
//

#ifndef DRILL_MONITOR_LOGGER_H_
#define DRILL_MONITOR_LOGGER_H_

struct DrillLogData {
	double data[8];
};

class CDrillMonitorLogger
{
	DrillLogData* m_buffer, *m_end;
	DrillLogData* m_ptr;
	unsigned int m_rc;
	unsigned int m_points;
	const unsigned int m_allocpoints;
public:
	bool IsEmty() const;
	unsigned int Size() const;
	unsigned int Length() const;
	//
	void Add(const DrillLogData& data);
	bool Read(DrillLogData& data);
	//
	bool Save(LPCTSTR pathname);
	bool Load(LPCTSTR pathname);
	//
	void Empty();
	void Initialize();
	//
	CDrillMonitorLogger();
	virtual ~CDrillMonitorLogger();
	//
	void Dump() const;
};

#endif // !DRILL_MONITOR_LOGGER_H_
