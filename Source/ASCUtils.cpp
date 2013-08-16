#include "ASCUtils.h"

ASCInt ASCRound(ASCDouble f)
{
	return (ASCInt)(f + 0.5);
}

RECT ASCBounds( ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight )
{
	RECT rcResult;
	rcResult.left = nLeft;
	rcResult.top = nTop;
	rcResult.right = nLeft + nWidth;
	rcResult.bottom = nTop + nHeight;
	return rcResult;
}

void ASCPoint4FromRect(const RECT Rc, CASCPoint4& Rtn)
{
	Rtn[0].X = (ASCSingle)Rc.left;
	Rtn[0].Y = (ASCSingle)Rc.top;
	Rtn[1].X = (ASCSingle)Rc.right;
	Rtn[1].Y = (ASCSingle)Rc.top;
	Rtn[2].X = (ASCSingle)Rc.right;
	Rtn[2].Y = (ASCSingle)Rc.bottom;
	Rtn[3].X = (ASCSingle)Rc.left;
	Rtn[3].Y = (ASCSingle)Rc.bottom;
}

void ASCColor4FromColor(ASCColor uColor, CASCColor4& Rtn)
{
	Rtn[0] = uColor;
	Rtn[1] = uColor;
	Rtn[2] = uColor;
	Rtn[3] = uColor;
}

void ASCPoint4FromLTWH(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCSingle fHeight, CASCPoint4& Rtn)
{
	Rtn[0].X = fLeft;
	Rtn[0].Y = fTop;
	Rtn[1].X = fLeft + fWidth;
	Rtn[1].Y = fTop;
	Rtn[2].X = fLeft + fWidth;
	Rtn[2].Y = fTop + fHeight;
	Rtn[3].X = fLeft;
	Rtn[3].Y = fTop + fHeight;
}

void ASCColor4From4Color(ASCColor uColor1, ASCColor uColor2, ASCColor uColor3, ASCColor uColor4, CASCColor4& Rtn)
{
	Rtn[0] = uColor1;
	Rtn[1] = uColor2;
	Rtn[2] = uColor3;
	Rtn[3] = uColor4;
}