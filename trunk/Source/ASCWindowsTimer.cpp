#include "ASCWindowsTimer.h"
#include "ASCTiming.h"
#include "ASCUtils.h"

CASCWindowsTimer* ASCWindowsTimer()
{
	static CASCWindowsTimer gASCWindowsTimer;
	return &gASCWindowsTimer;
}

/*
 * Maximum number of processing ticks that can be added to delta accumulator.
 * In cases where the time elapsed between sequential frames is many times
 * higher than the desired frame rate, instead of calling huge number of
 * processing calls the number is limited to this value. This particularly
 * helps when actual processing takes too long so that even sacrificing
 * rendering frames does not help, it will not overhelm the application and
 * make it unresponsive
*/
const ASCDouble cASCDeltaIncreaseLimit = 8.0;

/*
 * Minimal number of milliseconds required to pass before calculating
 * user-given frame rate
*/
const ASCDouble cASCSlowSampleTimeMax = 1000.0;

/*
 * Minimal number of milliseconds required to pass before calculating
 * internal latency required for render-independent processing
*/
const ASCDouble cASCQuickSampleTimeMax = 1.0;

CASCWindowsTimer::CASCWindowsTimer()
{
	m_fSpeed = 60.0;
	m_nMaxFPS = 4000;

	// Application.OnIdle:= AppIdle;

	m_fQuickSampleTime = ASCTiming()->GetTimeValue();
	m_fSlowSampleTime = m_fQuickSampleTime;
	m_nQuickFrameCount = 0;
	m_nSlowFrameCount = 0;

	m_nFrameRate = 0;
	m_fLatency = 0.0;
	m_fDeltaAccum = 0.0;
	m_bProcessed = false;

	m_bSingleCallOnly = false;

	m_OnTimer = 0;
	m_OnProcess = 0;
}

void CASCWindowsTimer::AppIdle()/*(ASCBoolean* pDone)*/
{
	// pDone = false;

	ASCDouble fCurrSampleTime = ASCTiming()->GetTimeValue();

	// If Timer is disabled, wait a little to avoid using 100% of CPU
	if (!m_bEnabled)
	{
		m_fQuickSampleTime = fCurrSampleTime;
		m_fSlowSampleTime = fCurrSampleTime;
		m_nQuickFrameCount = 0;
		m_nSlowFrameCount = 0;

		ASCTiming()->Sleep(5);
		return;
	}

	// If wrap-around occurs, consider this frame unreliable and skip it
	if ((fCurrSampleTime < m_fQuickSampleTime) || (fCurrSampleTime < m_fSlowSampleTime))
	{
		Reset();
		return;
	}

	m_nQuickFrameCount++;
	m_nSlowFrameCount++;

	ASCDouble fQuickSampleDiff = fCurrSampleTime - m_fQuickSampleTime;
	ASCDouble fSlowSampleDiff = fCurrSampleTime - m_fSlowSampleTime;

	if (fSlowSampleDiff >= cASCSlowSampleTimeMax)
	{
		m_nFrameRate = ASCRound((1000.0 * m_nSlowFrameCount) / fSlowSampleDiff);

		m_fSlowSampleTime = fCurrSampleTime;
		m_nSlowFrameCount = 0;
	}

	if (fQuickSampleDiff > cASCQuickSampleTimeMax)
	{
		if (m_nQuickFrameCount > 0)
		{
			m_fLatency = fQuickSampleDiff / m_nQuickFrameCount;
		} 
		else
		{
			m_fLatency = 0.0;
		}

		ASCDouble fDelta = fQuickSampleDiff * m_fProcessTimeScale;
		// Provide Delta limit to prevent auto-loop lockup
		if (fDelta > cASCDeltaIncreaseLimit)
		{
			fDelta = cASCDeltaIncreaseLimit;
		}

		if (m_bProcessed)
		{
			m_fDeltaAccum = m_fDeltaAccum + fDelta;
			m_bProcessed = false;
		}

		m_fQuickSampleTime = fCurrSampleTime;
		m_nQuickFrameCount = 0;
	}

	if (m_OnTimer)
	{
		m_OnTimer();
	}

	if (m_fMinInvokeInterval > cASCQuickSampleTimeMax + 1.0)
	{
		fCurrSampleTime = ASCTiming()->GetTimeValue();
		if ((fCurrSampleTime < m_fQuickSampleTime) || (fCurrSampleTime < m_fSlowSampleTime))
		{
			Reset();
			return;
		}

		fQuickSampleDiff = fCurrSampleTime - m_fQuickSampleTime;
		if (fQuickSampleDiff < m_fMinInvokeInterval)
		{
			ASCInt nWaitAmount = ASCRound(m_fMinInvokeInterval - fQuickSampleDiff);
			if (nWaitAmount > 0)
			{
				ASCTiming()->Sleep(nWaitAmount);
			}
		}
	}
}

void CASCWindowsTimer::Process()
{
	m_bProcessed = true;

	ASCInt nInvokeCnt = (ASCInt)m_fDeltaAccum;
	if (nInvokeCnt < 1)
	{
		return;
	}

	if (m_bSingleCallOnly)
	{
		nInvokeCnt = 1;
		m_fDeltaAccum = 0.0;
	}

	if (m_OnProcess)
	{
		for (ASCInt i = 1; i < nInvokeCnt + 1; i++)
		{
			m_OnProcess();
		}	
	}

	m_fDeltaAccum = m_fDeltaAccum - (ASCInt)m_fDeltaAccum;
}

void CASCWindowsTimer::Reset()
{
	m_fDeltaAccum = 0.0;

	m_fQuickSampleTime = ASCTiming()->GetTimeValue();
	m_fSlowSampleTime = m_fQuickSampleTime;
	m_nQuickFrameCount = 0;
	m_nSlowFrameCount = 0;
}

void CASCWindowsTimer::SetSpeed(ASCSingle fValue)
{
	m_fSpeed = fValue;
	if (m_fSpeed < 1.0)
	{
		m_fSpeed = 1.0;
	}

	m_fProcessTimeScale = m_fSpeed / 1000.0;
}

void CASCWindowsTimer::SetMaxFPS(ASCInt nValue)
{
	m_nMaxFPS = nValue;
	if (m_nMaxFPS < 1)
	{
		m_nMaxFPS = 1;
	}

	m_fMinInvokeInterval = 1000.0 / m_nMaxFPS;
}

ASCDouble CASCWindowsTimer::GetLatency()
{
	return m_fLatency;
}

ASCInt CASCWindowsTimer::GetFrameRate()
{
	return m_nFrameRate;
}

ASCSingle CASCWindowsTimer::GetSpeed()
{
	return m_fSpeed;
}

ASCInt CASCWindowsTimer::GetMaxFPS()
{
	return m_nMaxFPS;
}

void CASCWindowsTimer::SetEnabled(ASCBoolean bValue)
{
	m_bEnabled = bValue;
}

ASCBoolean CASCWindowsTimer::GetEnabled()
{
	return m_bEnabled;
}

ASCBoolean CASCWindowsTimer::GetSingleCallOnly()
{
	return m_bSingleCallOnly;
}

void CASCWindowsTimer::SetSingleCallOnly(ASCBoolean bValue)
{
	m_bSingleCallOnly = bValue;
}

CASCNotifyEvent CASCWindowsTimer::GetOnTimer()
{
	return m_OnTimer;
}

void CASCWindowsTimer::SetOnTimer(CASCNotifyEvent OnTimer)
{
	m_OnTimer = OnTimer;
}

void CASCWindowsTimer::SetOnProcess(CASCNotifyEvent OnProcess)
{
	m_OnProcess = OnProcess;
}

CASCNotifyEvent CASCWindowsTimer::GetOnProcess()
{
	return m_OnProcess;
}