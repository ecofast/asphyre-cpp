/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCWindowsConnector.h
 *  Content: ASC(Asphyre Sphinx for C++) Native Connection Manager
 *  Date   : 2013/08/12
 *  Desc   : ASC compatibility hook management
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

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
	CASCWindowsConnector();
	~CASCWindowsConnector();

	/*
	 * Initializes the component and calls the respective ASC events where
	 * ASC components should be created. This function returns True in case
	 * of success and False otherwise. If the component is already initialized,
	 * this function does nothing and returns True; it can be called as many
	 * times as possible in timer events to make sure that ASC components are properly created
	*/
	ASCBoolean Init();

	/*
	 * Finalizes the component and calls events where ASC components should be released
	*/
	void Done();

	/*
	 * Determines whether the component has been successfully initialized
	*/
	ASCBoolean IsInitialized();
private:
	ASCBoolean	m_bInitialized;
};

extern inline CASCWindowsConnector* ASCWindowsConnector();