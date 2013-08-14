/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCWindowsTimer.h
 *  Content: ASC(Asphyre Sphinx for C++) event-based multimedia timer
 *  Date   : 2013/08/12
 *  Desc   : ASC high-precision / event-based multimedia timer for Windows
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCTypes.h"

/*
 * The implementation of ASC's event-based multimedia timer running on
 * Windows platforms. This class hooks into application's idle event, 
 * so only once instance of this class should be used. For that purpose, 
 * G_ASCWindowsTimer variable exists that is automatically created on application's start
*/
class CASCWindowsTimer
{
public:
	CASCWindowsTimer();

	void AppIdle()/*(ASCBoolean* pDone)*/;

	/*
	 * This method should only be called from within OnTimer event to do
	 * constant object movement and animation control. Each time this method 
	 * is called, OnProcess event may(or may not) occur depending on the
	 * current rendering frame rate(see FrameRate) and the desired processing 
	 * speed(see Speed). The only thing that is assured is that OnProcess event 
	 * will occur exactly Speed times per second no matter how fast OnTimer
	 * occurs(that is, the value of FrameRate)
	*/
	void Process();

	/*
	 * Resets internal structures of the timer and starts over the timing
	 * calculations. This can be useful when a very time-consuming task was
	 * executed inside OnTimer event that only occurs once. Normally, it 
	 * would stall the timer making it think that the processing takes too long
	 * or the rendering is too slow; calling this method will tell the timer that
	 * it should ignore the situation and prevent the stall
	*/
	void Reset();

	void SetSpeed(ASCSingle fValue);
	void SetMaxFPS(ASCInt nValue);
	void SetEnabled(ASCBoolean bValue);
	void SetSingleCallOnly(ASCBoolean bValue);
	void SetOnTimer(CASCNotifyEvent OnTimer);
	void SetOnProcess(CASCNotifyEvent OnProcess);

	ASCDouble GetLatency();
	ASCInt GetFrameRate();
	ASCSingle GetSpeed();
	ASCInt GetMaxFPS();
	ASCBoolean GetEnabled();
	ASCBoolean GetSingleCallOnly();
	CASCNotifyEvent GetOnTimer();
	CASCNotifyEvent GetOnProcess();
private:
	/*
	 * The maximum allowed frame rate at which OnTimer should be
	 * executed. This value is an approximate and the resulting frame rate 
	 * may be quite different(the resolution can be as low as 10 ms). 
	 * It should be used with reasonable values to prevent the application 
	 * from using 100% of CPU and GPU with unnecessarily high frame rates 
	 * such as 1000 FPS. A reasonable and default value for this property is 200
	*/
	ASCInt				m_nMaxFPS;

	/*
	 * The speed of constant processing and animation control in frames per
	 * second. This affects both Delta property and occurence of OnProcess event
	*/
	ASCSingle			m_fSpeed;

	/*
	 * Determines whether the timer is enabled or not.
	 * The internal processing may still be occurring independently of this value,
	 * but it controls whether OnTimer event occurs or not
	*/
	ASCBoolean			m_bEnabled;

	/*
	 * This event occurs when m_bEnabled is set to True and as fast as
	 * possible(only limited approximately by m_nMaxFPS. In this event, all
	 * rendering should be made. Inside this event, at some location it is
	 * recommended to call Process method, which will invoke OnProcess event 
	 * for constant object movement and animation control. The idea is to 
	 * render graphics as fast as possible while moving objects and 
	 * controlling animation at constant speed
	*/
	CASCNotifyEvent		m_OnTimer;

	/*
	 * The current frame rate in frames per second. This value is calculated
	 * approximately two times per second and can only be used for informative
	 * purposes(e.g. displaying frame rate in the application). For precise
	 * real-time indications it is recommended to use Latency property instead
	*/
	ASCInt				m_nFrameRate;

	/*
	 * This event occurs when calling Process method inside OnTimer event. 
	 * In this event all constant object movement and animation control
	 * should be made. This event can occur more than once for each call to
	 * Process or may not occur, depending on the current FrameRate and Speed.
	 * For instance, when frame rate is 120 FPS and speed set to 60, this event 
	 * will occur for each second call to Process; on the other hand, 
	 * if frame rate is 30 FPS with speed set to 60, this event will occur twice 
	 * for each call to Process to maintain constant processing. An alternative 
	 * to this is doing processing inside OnTimer event using Delta as coefficient 
	 * for object movement. If the processing takes too much time inside this event 
	 * so that the target speed cannot be achieved, the timer may stall(that is, 
	 * reduce number of occurences of this event until the balance is restored)
	*/
	CASCNotifyEvent		m_OnProcess;
	ASCBoolean			m_bProcessed;

	/*
	 * Average time(in milliseconds) calculated between previous frame and the
	 * current one. This can be a direct indicator of rendering performance
	 * as it indicates how much time it takes on average to render(and possibly
	 * process) the frame. This parameter is typically updated much more
	 * frequently than FrameRate
	*/
	ASCDouble			m_fLatency;

	ASCSingle			m_fDeltaAccum;
	ASCDouble			m_fProcessTimeScale;
	ASCDouble			m_fMinInvokeInterval;
	ASCDouble			m_fQuickSampleTime;
	ASCDouble			m_fSlowSampleTime;
	ASCInt				m_nQuickFrameCount;
	ASCInt				m_nSlowFrameCount;

	/*
	 * If this property is set to True, it will prevent the timer from trying to
	 * fix situations where the rendering speed is slower than the processing
	 * speed(that is, m_nFrameRate is lower than m_fSpeed). Therefore, faster rendering 
	 * produces constant speed, while slower rendering slows the processing down. 
	 * This is particularly useful for dedicated servers that do no rendering 
	 * but only processing; in this case, the processing cannot be technically
	 * any faster than it already is
	*/
	ASCBoolean			m_bSingleCallOnly;
};

/*
 * Instance of CASCWindowsTimer that is ready to use in applications
 * without having to create that class explicitly
*/
extern inline CASCWindowsTimer* ASCWindowsTimer();