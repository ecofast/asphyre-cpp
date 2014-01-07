/*******************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / adelphicoder.  All Rights Reserved.
 *
 *  File   : ASCConfig.h
 *  Content: ASC(Asphyre Sphinx for C++) compiler configuration
 *  Date   : 2013/08/13
 *  Desc   : Compiler configuration for ASC framework
 *
 *******************************************************************************************/

#pragma once

#define ALLOWPRECISEFLOAT

#define MSWINDOWS
#ifdef MSWINDOWS
	// Provide shorter keyword for Windows platform
	#define WINDOWS
#endif