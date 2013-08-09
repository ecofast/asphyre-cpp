#include "ASCUtils.h"

ASCInt ASCRound(ASCSingle f)
{
	return (ASCInt)(f + 0.5);
}

RECT ASCBounds( ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight )
{
	RECT rcResult;
	rcResult.left	= nLeft;
	rcResult.top	= nTop;
	rcResult.right	= nLeft + nWidth;
	rcResult.bottom	= nTop + nHeight;
	return rcResult;
}