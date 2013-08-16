#include "ASCTiming.h"
#include "ASCUtils.h"

#ifdef MSWINDOWS
	#define OSTIMINGSUPPORT
#endif

#ifdef POSIX
	#define OSTIMINGSUPPORT
#endif

CASCTiming* ASCTiming()
{
	static CASCTiming gASCTiming;
	return &gASCTiming;
}

CASCTiming::CASCTiming()
{
	m_Mode = atmNotStarted;
#ifdef MSWINDOWS
	m_nHighFrequency.QuadPart = 0;
#endif
}

CASCTiming::~CASCTiming()
{

}

CASCTimingMode CASCTiming::GetTimingMode()
{
	return m_Mode;
}

ASCDouble CASCTiming::GetTimeValue()
{
#ifdef MSWINDOWS
	if (m_Mode == atmNotStarted)
	{
		m_Mode = atmLowPrecision;
		if (QueryPerformanceFrequency(&m_nHighFrequency) && (m_nHighFrequency.QuadPart > 0))
		{
			m_Mode = atmHighPrecision;
		}
	}

	if (m_Mode == atmHighPrecision)
	{
		LARGE_INTEGER nTimeCounter;
		QueryPerformanceCounter(&nTimeCounter);
		return ((nTimeCounter.QuadPart * 1000.0) / m_nHighFrequency.QuadPart);
	}
	else
	{
		return (timeGetTime());
	}
#endif

#ifdef POSIX

#endif

#ifndef OSTIMINGSUPPORT
	ASCDouble fCurrTime = Now();
	return (fCurrTime * 86400000.0);
#endif
}

void CASCTiming::Sleep(ASCInt nMilliSeconds)
{
#ifdef MSWINDOWS
	SleepEx(nMilliSeconds, true);
#endif

#ifdef POSIX

#endif

#ifndef OSTIMINGSUPPORT
	Sleep(nMilliSeconds);
#endif
}

ASCUInt32 CASCTiming::GetTickCount()
{
	return (ASCUInt32)ASCRound(GetTimeValue());
}