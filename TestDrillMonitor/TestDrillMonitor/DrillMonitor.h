//
//
//

#ifndef DRILL_MONITOR_H_
#define	DRILL_MONITOR_H_

class IDrillMonitor
{
public:
	virtual int  GetData(double* ptr, int count) = 0;
	virtual int  GetError() = 0;
	virtual bool Start(const char* spindle, const char* sensor, const char* filename) = 0;
	virtual void Stop() = 0;
	virtual void Play(const char* filename) = 0;
	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;
};

#endif // !DRILL_MONITOR_H_


