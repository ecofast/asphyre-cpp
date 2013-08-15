/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCTimer.h
 *  Content: ASC(Asphyre Sphinx for C++) platform independent timer with external interface
 *  Date   : 2013/08/12
 *  Desc   : High-precision / Event-based multimedia timer for Windows, MacOS, iOS and so on
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"

/*
 * The implementation of ASC's event-based multimedia timer running on
 * a variety of platforms including Windows, Mac OS and iOS. This class is not
 * independent, it requires the application to call NotifyIdle method,
 * which will provide similar functionality of CASCWindowsTimer class
*/
class CASCTimer
{
public:
	CASCTimer()
	{
		m_fSpeed = 60.0;
		m_nMaxFPS = 4000;
	}
protected:
private:
	ASCInt				m_nMaxFPS;
	ASCDouble			m_fSpeed;
	ASCBoolean			m_bEnabled;
	CASCNotifyEvent		m_OnTimer;
	ASCInt				m_nFrameRate;
	CASCNotifyEvent		m_OnProcess;
	ASCBoolean			m_bProcessed;

	ASCDouble			m_fPrevValue;

	ASCDouble			m_fLatency;
	ASCDouble			m_fMinLatency;
	ASCDouble			m_fSpeedLatency;
	ASCDouble			m_fSampleLatency;
	ASCInt				m_nSampleIndex;

	ASCDouble			m_fDelta;
	ASCDouble			m_fDeltaCounter;

	ASCBoolean			m_bSingleCallOnly;

	ASCDouble RetrieveLatency()
	{

	}

	void SetSpeed(ASCSingle fValue)
	{

	}

	void SetMaxFPS(ASCInt nValue)
	{

	}
};