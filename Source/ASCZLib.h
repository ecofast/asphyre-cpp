/*******************************************************************************************
 *
 *  Copyright (C) LinZhenqun / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCZLib.h
 *  Content: ASC(Asphyre Sphinx for C++) ZLib import helper
 *  Date   : 2013/11/26
 *  Desc   : Helper for import zlib
 *
 *  Note: 需增加 zlib 库搜索路径(Library/zlib/lib/slib 或 Library/zlib/lib/dlib)
 *
 *******************************************************************************************/

#pragma once

#include "ASCConfig.h"

#ifdef _DEBUG
	#pragma comment(lib, "zlibd.lib")
#else
	#pragma comment(lib, "zlib.lib")
#endif

#include "../Library/zlib/zlib.h"