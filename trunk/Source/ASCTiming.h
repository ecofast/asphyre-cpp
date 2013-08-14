/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCTiming.h
 *  Content: ASC(Asphyre Sphinx for C++) cross-platform timing and utilities
 *  Date   : 2013/08/12
 *  Desc   : High accuracy timing and sleep routines that can be used on different platforms
 *           including Win32, Win64 and Mac OS
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include <windows.h>
#include "ASCTypes.h"

/*
 * The timing mode that is currently being used by CASTiming.
 * It is typically chosen the first time one of the routines is called
 * and remains unchanged during the lifetime of application
*/
enum CASCTimingMode
{
	/*
	 * The timing operations have not yet started. One of the timing functions
	 * need to be called for this state to change
	*/
	atmNotStarted,
	/*
	 * The timing operations are running in low-precision mode that may degrade
	 * the quality of operations. This means that the system has no high precision
	 * timer available
	*/
	atmLowPrecision,
	/*
	 * The timing operations are running in high-precision mode providing the
	 * best possible timer resolution. The actual resolution is hardware and
	 * platform dependent and can vary on different configurations
	*/
	atmHighPrecision,
};

/*
 * High accuracy timing and sleep implementation that can be used 
 * on different platforms including Win32, Win64 and Mac OS
*/
class CASCTiming
{
public:
	CASCTiming();
	~CASCTiming();

	// The timing mode at which the component currently operates
	CASCTimingMode GetTimingMode();

	/*
	 * Returns the current timer counter represented as 64-bit floating-point number. 
	 * The resulting value is specified in milliseconds and fractions of thereof. 
	 * The value should only be used for calculating differences because it can wrap
	 * (from very high positive value back to zero or even some negative value) 
	 * after prolonged time intervals
	*/
	ASCDouble GetTimeValue();

	/*
	 * Returns the current timer counter represented as 32-bit unsigned integer.
	 * The resulting value is specified in milliseconds. The value should only be
	 * used for calculating differences because it can wrap(from very high
	 * positive value back to zero) after prolonged time intervals. The wrapping
	 * usually occurs upon reaching High(Cardinal) but depending on each
	 * individual platform, it can also occur earlier
	*/
	ASCUInt32 GetTickCount();

	/*
	 * Causes the calling thread to sleep for a given number of milliseconds.
	 * The sleep can actually be interrupted under certain conditions(such as
	 * when a message is sent to the caller's thread)
	*/
	void Sleep(ASCInt nMilliSeconds);
private:
	CASCTimingMode	m_Mode;
#ifdef MSWINDOWS
	LARGE_INTEGER	m_nHighFrequency;
#endif
};

/*
 * A running instance of CASCTiming that is created when the application is 
 * executed and freed upon termination; therefore, this class can be used 
 * with its timing functions without having to explicitly create it elsewhere
*/
extern inline CASCTiming* ASCTiming();