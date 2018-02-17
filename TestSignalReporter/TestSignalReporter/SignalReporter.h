//
//
//

class ISignalReporter
{
public:
	virtual void Notify(int msec) = 0;
	virtual bool Start(const char* portname) = 0;
	virtual void Stop() = 0;
};