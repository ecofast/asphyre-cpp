/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCWindowsConnector.h
 *  Content: ASC(Asphyre Sphinx for C++) Native Connection Manager
 *  Date   : 2013/08/12
 *  Desc   : ASC compatibility hook management
 *
 *******************************************************************************************/

#pragma once

#include "ASCTypes.h"
#include "ASCEvents.h"

/*
 * Native(x86/x64) ASC compatibility hook manager. 
 * This manager works in a similar fashion to other hook managers 
 * such as those for Mac OS X and iOS, but it is meant to be used only on Windows
*/
class CASCWindowsConnector
{
public:
	CASCWindowsConnector()
	{
		m_bInitialized = false;
	}

	~CASCWindowsConnector()
	{

	}

	/*
	 * Initializes the component and calls the respective ASC events where
	 * ASC components should be created. This function returns True in case
	 * of success and False otherwise. If the component is already initialized,
	 * this function does nothing and returns True; it can be called as many
	 * times as possible in timer events to make sure that ASC components are properly created
	*/
	ASCBoolean Init()
	{
		if (m_bInitialized)
		{
			return true;
		}

		m_bInitialized = true;
		ASCCreateEvent()->Notify(this);
		ASCTimerResetEvent()->Notify(this);

		return true;
	}

	// Finalizes the component and calls events where ASC components should be released
	void Done()
	{
		if (!m_bInitialized)
		{
			return;
		}

		ASCDestroyEvent()->Notify(this);
		m_bInitialized = false;
	}

	// Determines whether the component has been successfully initialized
	ASCBoolean IsInitialized()
	{
		return m_bInitialized;
	}
private:
	ASCBoolean	m_bInitialized;
};

extern inline CASCWindowsConnector* ASCWindowsConnector();