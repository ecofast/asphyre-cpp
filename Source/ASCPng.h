/*******************************************************************************************
 *
 *  Copyright (C) LinZhenqun / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCPng.h
 *  Content: ASC(Asphyre Sphinx for C++) libpng import helper
 *  Date   : 2013/11/26
 *  Desc   : Helper for import libpng
 *
 *  Note: 需增加 libpng 库搜索路径(Library/libpng/lib/slib 或 Library/libpng/lib/dlib)
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#include "ASCZLib.h"

#ifdef _DEBUG
	#pragma comment(lib, "libpngd.lib")
#else
	#pragma comment(lib, "libpng.lib")
#endif

#include "../Library/libpng/png.h"