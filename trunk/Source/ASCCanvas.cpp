#include <math.h>
#include "ASCCanvas.h"
#include "ASCEvents.h"
#include "ASCUtils.h"
#include "ASCConsts.h"

const ASCSingle cMinAllowedCanvasScale = 0.001f;

CASCCanvas::CASCCanvas()
{
	m_nCacheStall = 0;
	m_fDeviceScale = 1.0;
	m_fExternalScale = 1.0;
	UpdateInternalScale();

	ASCDeviceCreateEvent()->Subscribe((wchar_t*)typeid(*this).name(), std::bind(&CASCCanvas::OnDeviceCreate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	ASCDeviceDestroyEvent()->Subscribe((wchar_t*)typeid(*this).name(), std::bind(&CASCCanvas::OnDeviceDestroy, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	ASCDeviceResetEvent()->Subscribe((wchar_t*)typeid(*this).name(), std::bind(&CASCCanvas::OnDeviceReset, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	ASCDeviceLostEvent()->Subscribe((wchar_t*)typeid(*this).name(), std::bind(&CASCCanvas::OnDeviceLost, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	ASCBeginSceneEvent()->Subscribe((wchar_t*)typeid(*this).name(), std::bind(&CASCCanvas::OnBeginScene, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	ASCEndSceneEvent()->Subscribe((wchar_t*)typeid(*this).name(), std::bind(&CASCCanvas::OnEndScene, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

CASCCanvas::~CASCCanvas()
{
	ASCEventProviders()->UnSubscribe((wchar_t*)(typeid(*this).name()));
}

ASCBoolean CASCCanvas::HandleDeviceCreate()
{
	return true;
}

void CASCCanvas::HandleDeviceDestroy()
{

}

ASCBoolean CASCCanvas::HandleDeviceReset()
{
	return true;
}

void CASCCanvas::HandleDeviceLost()
{

}

void CASCCanvas::OnDeviceCreate(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	ASCBoolean bSucced = HandleDeviceCreate();
	if (pParam)
	{
		*((ASCBoolean*)pParam) = *((ASCBoolean*)pParam) && bSucced;
	}
}

void CASCCanvas::OnDeviceDestroy(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	HandleDeviceDestroy();
}

void CASCCanvas::OnDeviceReset(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	ASCBoolean bSucced = HandleDeviceReset();
	if (pParam)
	{
		*((ASCBoolean*)pParam) = *((ASCBoolean*)pParam) && bSucced;
	}
}

void CASCCanvas::OnDeviceLost(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	HandleDeviceLost();
}

void CASCCanvas::OnBeginScene(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	m_nCacheStall = 0;
	HandleBeginScene();
}

void CASCCanvas::OnEndScene(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	HandleEndScene();
}

ASCInt CASCCanvas::GetCacheStall()
{
	return m_nCacheStall;
}

void CASCCanvas::NextRenderCall()
{
	m_nCacheStall++;
}

void CASCCanvas::ResetStates()
{

}

ASCSingle CASCCanvas::GetDeviceScale()
{
	return m_fDeviceScale;
}

ASCSingle CASCCanvas::GetExternalScale()
{
	return m_fExternalScale;
}

void CASCCanvas::UpdateInternalScale()
{
	m_fInternalScale = m_fDeviceScale / m_fExternalScale;
}

void CASCCanvas::SetDeviceScale(const ASCSingle fValue)
{
	ASCSingle fVal = fValue;
	if (fVal < cMinAllowedCanvasScale)
	{
		fVal = cMinAllowedCanvasScale;
	}

	if (fVal != m_fDeviceScale)
	{
		Flush();

		m_fDeviceScale = fVal;
		UpdateInternalScale();
	}
}

void CASCCanvas::SetExternalScale(const ASCSingle fValue)
{
	ASCSingle fVal = fValue;
	if (fVal < cMinAllowedCanvasScale)
	{
		fVal = cMinAllowedCanvasScale;
	}

	if (fVal != m_fDeviceScale)
	{
		Flush();

		m_fExternalScale = fVal;
		UpdateInternalScale();
	}
}

void CASCCanvas::RenderPixel(ASCSingle fX, ASCSingle fY, ASCColor uColor)
{
	RenderPixel(CASCFloatVector2D(fX, fY), uColor);
}

void CASCCanvas::RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor)
{
	RenderLine(Src, Dest, uColor, uColor);
}

void CASCCanvas::RenderLine(ASCSingle fX1, ASCSingle fY1, ASCSingle fX2, ASCSingle fY2, ASCColor uColor)
{
	RenderLine(CASCFloatVector2D(fX1, fY1), CASCFloatVector2D(fX2, fY2), uColor);
}

void CASCCanvas::FillTriangle(const CASCFloatVector2D p1, const CASCFloatVector2D p2, const CASCFloatVector2D p3, 
							  ASCColor uColor1, ASCColor uColor2, ASCColor uColor3, CASCBlendingEffect Effect /*= abeNormal*/)
{
	ASCInt Indices[3] = {0, 1, 2};

	CASCFloatVector2D Vertices[3];
	ASCUInt32 uColors[3];

	Vertices[0] = p1;
	Vertices[1] = p2;
	Vertices[2] = p3;

	uColors[0] = uColor1;
	uColors[1] = uColor2;
	uColors[2] = uColor3;

	RenderIndexedTriangles(&Vertices[0], &uColors[0], &Indices[0], 3, 1, Effect);
}

void CASCCanvas::FillQuad(const CASCPoint4 Pts, const CASCColor4 Colors, CASCBlendingEffect Effect /*= abeNormal*/)
{
	ASCInt Indices[6] = {2, 0, 1, 3, 2, 1};

	CASCFloatVector2D Vertices[4];
	ASCColor uColors[4];

	Vertices[0] = Pts[0];
	Vertices[1] = Pts[1];
	Vertices[2] = Pts[3];
	Vertices[3] = Pts[2];

	uColors[0] = Colors[0];
	uColors[1] = Colors[1];
	uColors[2] = Colors[3];
	uColors[3] = Colors[2];

	RenderIndexedTriangles(&Vertices[0], &uColors[0], &Indices[0], 4, 2, Effect);
}

void CASCCanvas::FillRect(const RECT Rc, const CASCColor4 Colors, CASCBlendingEffect Effect /*= abeNormal*/)
{
	CASCPoint4 Pt4;
	ASCPoint4FromRect(Rc, Pt4);
	FillQuad(Pt4, Colors, Effect);
}

void CASCCanvas::FillRect(const RECT Rc, ASCColor uColor, CASCBlendingEffect Effect /*= abeNormal*/)
{
	CASCColor4 Color4;
	ASCColor4FromColor(uColor, Color4);
	FillRect(Rc, Color4, Effect);
}

void CASCCanvas::FillRect(ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight, ASCColor uColor, CASCBlendingEffect Effect /*= abeNormal*/)
{
	FillRect(ASCBounds(nLeft, nTop, nWidth, nHeight), uColor, Effect);
}

void CASCCanvas::FrameRect(const CASCPoint4 Pts, const CASCColor4 Colors, CASCBlendingEffect Effect /*= abeNormal*/)
{
	ASCInt Indices[24] = {0, 1, 4, 4, 1, 5, 1, 2, 5, 5, 2, 6, 2, 3, 6, 6, 3, 7, 3, 0, 7, 7, 0, 4};

	CASCFloatVector2D Vertices[8];
	ASCColor uColors[8];

	for (ASCInt i = 0; i < 4; i++)
	{
		Vertices[i] = Pts[i];
		uColors[i] = Colors[i];
		uColors[i + 4] = Colors[i];
	}

	Vertices[4] = CASCFloatVector2D((ASCSingle)(Pts[0].X + 1.0), (ASCSingle)(Pts[0].Y + 1.0));
	Vertices[5] = CASCFloatVector2D((ASCSingle)(Pts[1].X - 1.0), (ASCSingle)(Pts[1].Y + 1.0));
	Vertices[6] = CASCFloatVector2D((ASCSingle)(Pts[2].X - 1.0), (ASCSingle)(Pts[2].Y - 1.0));
	Vertices[7] = CASCFloatVector2D((ASCSingle)(Pts[3].X + 1.0), (ASCSingle)(Pts[3].Y - 1.0));

	// RenderIndexedTriangles(&Vertices[0], &uColors[0], &Indices[0], 8, 8, Effect);
	RenderIndexedTriangles(Vertices, uColors, Indices, 8, 8, Effect);
}

void CASCCanvas::FrameRect(const RECT Rc, const CASCColor4 Colors, CASCBlendingEffect Effect /*= abeNormal*/)
{
	CASCPoint4 Pt4;
	ASCPoint4FromRect(Rc, Pt4);
	FrameRect(Pt4, Colors, Effect);
}

void CASCCanvas::FrameRect(ASCInt nLeft, ASCInt nTop, ASCInt nWidth, ASCInt nHeight, ASCColor uColor, CASCBlendingEffect Effect /*= abeNormal*/)
{
	CASCColor4 Color4;
	ASCColor4FromColor(uColor, Color4);
	FrameRect(ASCBounds(nLeft, nTop, nWidth, nHeight), Color4, Effect);
}

void CASCCanvas::RenderHorizLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCColor uColor1, ASCColor uColor2, CASCBlendingEffect Effect /*= abeNormal*/)
{
	CASCPoint4 Pt4;
	ASCPoint4FromLTWH(fLeft, fTop, fWidth, 1.0, Pt4);
	CASCColor4 Color4;
	ASCColor4From4Color(uColor1, uColor2, uColor2, uColor1, Color4);
	FillQuad(Pt4, Color4, Effect);
}

void CASCCanvas::RenderHorizLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fWidth, ASCColor uColor, CASCBlendingEffect Effect /*= abeNormal*/)
{
	RenderHorizLine(fLeft, fTop, fWidth, uColor, uColor, Effect);
}

void CASCCanvas::RenderVertLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fHeight, ASCColor uColor1, ASCColor uColor2, CASCBlendingEffect Effect /*= abeNormal*/)
{
	CASCPoint4 Pt4;
	ASCPoint4FromLTWH(fLeft, fTop, 1.0, fHeight, Pt4);
	CASCColor4 Color4;
	ASCColor4From4Color(uColor1, uColor2, uColor2, uColor1, Color4);
	FillQuad(Pt4, Color4, Effect);
}

void CASCCanvas::RenderVertLine(ASCSingle fLeft, ASCSingle fTop, ASCSingle fHeight, ASCColor uColor, CASCBlendingEffect Effect /*= abeNormal*/)
{
	RenderVertLine(fLeft, fTop, fHeight, uColor, uColor, Effect);
}

void CASCCanvas::RenderQuadHole(const CASCFloatVector2D Pos, const CASCFloatVector2D Size, const CASCFloatVector2D Center, const CASCFloatVector2D Radius, 
								ASCColor uOutColor, ASCColor uInColor, ASCInt nSteps, CASCBlendingEffect Effect /*= abeNormal*/)
{
	CASCFloatVector2D* pVertices = new CASCFloatVector2D[nSteps * 2];
	ASCColor* pColors = new ASCColor[nSteps * 2];
	ASCInt* pIndices = new ASCInt[(nSteps - 1) * 6];

	ASCInt nBase;
	for (ASCInt i = 0; i < nSteps - 1; i++)
	{
		nBase = i * 6;

		pIndices[nBase + 0] = i;
		pIndices[nBase + 1] = i + 1;
		pIndices[nBase + 2] = i + nSteps;
		pIndices[nBase + 3] = i + 1;
		pIndices[nBase + 4] = i + nSteps + 1;
		pIndices[nBase + 5] = i + nSteps;
	}

	ASCSingle fTheta, fAngle;

	for (ASCInt i = 0; i < nSteps; i++)
	{
		fTheta = (ASCSingle)i / (nSteps - 1);

		pVertices[i].X = Pos.X + fTheta * Size.X;
		pVertices[i].Y = Pos.Y;
		pColors[i] = uOutColor;

		fAngle = (ASCSingle)(cASC_PI * 0.25 + cASC_PI * 0.5 - fTheta * cASC_PI * 0.5);

		pVertices[nSteps + i].X = Center.X + cos(fAngle) * Radius.X;
		pVertices[nSteps + i].Y = Center.Y - sin(fAngle) * Radius.Y;
		pColors[nSteps + i] = uInColor;
	}

	RenderIndexedTriangles(&pVertices[0], &pColors[0], &pIndices[0], /*Length(pVertices)*/nSteps * 2, /*Length(pIndices) / 3*/(nSteps - 1) * 6 / 3, Effect);

	for (ASCInt i = 0; i < nSteps; i++)
	{
		fTheta = (ASCSingle)i / (nSteps - 1);

		pVertices[i].X = Pos.X + Size.X;
		pVertices[i].Y = Pos.Y + fTheta * Size.Y;
		pColors[i] = uOutColor;

		fAngle = (ASCSingle)(cASC_PI * 0.25 - fTheta * cASC_PI * 0.5);

		pVertices[nSteps + i].X = Center.X + cos(fAngle) * Radius.X;
		pVertices[nSteps + i].Y = Center.Y - sin(fAngle) * Radius.Y;
		pColors[nSteps + i] = uInColor;
	}

	RenderIndexedTriangles(&pVertices[0], &pColors[0], &pIndices[0], /*Length(pVertices)*/nSteps * 2, /*Length(pIndices) / 3*/(nSteps - 1) * 6 / 3, Effect);

	for (ASCInt i = 0; i < nSteps; i++)
	{
		fTheta = (ASCSingle)i / (nSteps - 1);

		pVertices[i].X = Pos.X;
		pVertices[i].Y = Pos.Y + fTheta * Size.Y;
		pColors[i] = uOutColor;

		fAngle = (ASCSingle)(cASC_PI * 0.75 + fTheta * cASC_PI * 0.5);

		pVertices[nSteps + i].X = Center.X + cos(fAngle) * Radius.X;
		pVertices[nSteps + i].Y = Center.Y - sin(fAngle) * Radius.Y;
		pColors[nSteps + i] = uInColor;
	}

	RenderIndexedTriangles(&pVertices[0], &pColors[0], &pIndices[0], /*Length(pVertices)*/nSteps * 2, /*Length(pIndices) / 3*/(nSteps - 1) * 6 / 3, Effect);

	for (ASCInt i = 0; i < nSteps; i++)
	{
		fTheta = (ASCSingle)i / (nSteps - 1);

		pVertices[i].X = Pos.X + fTheta * Size.X;
		pVertices[i].Y = Pos.Y + Size.Y;
		pColors[i] = uOutColor;

		fAngle = (ASCSingle)(cASC_PI * 1.25 + fTheta * cASC_PI * 0.5);

		pVertices[nSteps + i].X = Center.X + cos(fAngle) * Radius.X;
		pVertices[nSteps + i].Y = Center.Y - sin(fAngle) * Radius.Y;
		pColors[nSteps + i] = uInColor;
	}

	RenderIndexedTriangles(&pVertices[0], &pColors[0], &pIndices[0], /*Length(pVertices)*/nSteps * 2, /*Length(pIndices) / 3*/(nSteps - 1) * 6 / 3, Effect);

	delete [] pVertices;
	delete [] pIndices;
	delete [] pColors;
}

void CASCCanvas::RenderTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, 
							   CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		CASCPoint4 pt4;
		ASCPoint4FromLTWH(fX, fY, (ASCSingle)(pTexture->GetWidth()), (ASCSingle)(pTexture->GetHeight()), pt4);
		CASCPoint4 maps;
		ASCPoint4From8Values(0, 0, 1, 0, 1, 1, 0, 1, maps);
		CASCColor4 c4;
		ASCColor4From4Color(Color1, Color2, Color3, Color4, c4);
		RenderTexture(pTexture, pt4, maps, c4, Effect);
	}
}

void CASCCanvas::RenderTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, CASCBlendingEffect Effect /*= abeNormal*/)
{
	RenderTexture(fX, fY, pTexture, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, Effect);
}

void CASCCanvas::RenderTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCColor Color, CASCBlendingEffect Effect /*= abeNormal*/)
{
	RenderTexture(fX, fY, pTexture, Color, Color, Color, Color, Effect);
}

void CASCCanvas::RenderScaledTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fScale, 
									ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		CASCPoint4 pt4;
		ASCPoint4FromLTWHScaled(fX, fY, (ASCSingle)(pTexture->GetWidth()), (ASCSingle)(pTexture->GetHeight()), fScale, pt4);
		CASCPoint4 maps;
		ASCPoint4From8Values(0, 0, 1, 0, 1, 1, 0, 1, maps);
		CASCColor4 c4;
		ASCColor4From4Color(Color1, Color2, Color3, Color4, c4);
		RenderTexture(pTexture, pt4, maps, c4, Effect);
	}
}

void CASCCanvas::RenderScaledTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fScale, 
									 ASCColor Color /*= cASCColor32White*/, CASCBlendingEffect Effect /*= abeNormal*/)
{
	RenderScaledTexture(fX, fY, pTexture, fScale, Color, Color, Color, Color, Effect);
}

void CASCCanvas::RenderStretchTexture(ASCSingle fLeft, ASCSingle fTop, ASCSingle fRight, ASCSingle fBottom, CASCTexture* pTexture, 
									  ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		CASCPoint4 pt4;
		ASCPoint4From8Values(fLeft, fTop, fRight, fTop, fRight, fBottom, fLeft, fBottom, pt4);
		CASCPoint4 maps;
		ASCPoint4From8Values(0, 0, 1, 0, 1, 1, 0, 1, maps);
		CASCColor4 c4;
		ASCColor4From4Color(Color1, Color2, Color3, Color4, c4);
		RenderTexture(pTexture, pt4, maps, c4, Effect);
	}
}

void CASCCanvas::RenderStretchTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fWidth, ASCSingle fHeight, ASCSingle fScaleX, ASCSingle fScaleY, 
									  ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		CASCPoint4 pt4;
		ASCPoint4FromLTWHScaledXY(fX, fY, fWidth, fHeight, fScaleX, fScaleY, pt4);
		CASCPoint4 maps;
		ASCPoint4From8Values(0, 0, 1, 0, 1, 1, 0, 1, maps);
		CASCColor4 c4;
		ASCColor4From4Color(Color1, Color2, Color3, Color4, c4);
		RenderTexture(pTexture, pt4, maps, c4, Effect);
	}
}

void CASCCanvas::RenderPartTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCInt nSrcX1, ASCInt nSrcY1, ASCInt nSrcX2, ASCInt nSrcY2, ASCSingle fScaleX, 
								   ASCSingle fScaleY, ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		CASCPoint4 pt4;
		ASCPoint4FromLTWHScaledXY((ASCSingle)fX, (ASCSingle)fY, (ASCSingle)(nSrcX2 - nSrcX1), (ASCSingle)(nSrcY2 - nSrcY1), fScaleX, fScaleY, pt4);
		CASCPoint4 maps;
		maps[0].X = (ASCSingle)nSrcX1 / pTexture->GetWidth();
		maps[0].Y = (ASCSingle)nSrcY1 / pTexture->GetHeight();
		maps[1].X = (ASCSingle)nSrcX2 / pTexture->GetWidth();
		maps[1].Y = (ASCSingle)nSrcY1 / pTexture->GetHeight();
		maps[2].X = (ASCSingle)nSrcX2 / pTexture->GetWidth();
		maps[2].Y = (ASCSingle)nSrcY2 / pTexture->GetHeight();
		maps[3].X = (ASCSingle)nSrcX1 / pTexture->GetWidth();
		maps[3].Y = (ASCSingle)nSrcY2 / pTexture->GetHeight();
		CASCColor4 c4;
		ASCColor4From4Color(Color1, Color2, Color3, Color4, c4);
		RenderTexture(pTexture, pt4, maps, c4, Effect);
	}
}

void CASCCanvas::RenderRotateTexture(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCSingle fAngle, ASCSingle fScaleX, ASCSingle fScaleY, 
									 ASCColor Color1, ASCColor Color2, ASCColor Color3, ASCColor Color4, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		CASCPoint4 pt4;
		ASCPoint4RotatedCentered(CASCFloatVector2D(fX, fY), CASCFloatVector2D((ASCSingle)(pTexture->GetWidth()), (ASCSingle)(pTexture->GetHeight())), fAngle, fScaleX, fScaleY, pt4);
		CASCPoint4 maps;
		ASCPoint4From8Values(0, 0, 1, 0, 1, 1, 0, 1, maps);
		CASCColor4 c4;
		ASCColor4From4Color(Color1, Color2, Color3, Color4, c4);
		RenderTexture(pTexture, pt4, maps, c4, Effect);
	}
}

void CASCCanvas::RenderTextureWaveX(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCInt nWidth, ASCInt nHeight, ASCInt nAmp, ASCInt nLen, ASCInt nPhase, 
									ASCColor Color /*= cASCColor32White*/, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		ASCInt nW = pTexture->GetWidth();
		ASCInt i;
		for (ASCInt j = 0; j < nWidth; j++)
		{
			i = ASCTrunc(j * nW / nWidth);
			RenderPartTexture(fX + j, (ASCSingle)(fY + nAmp * sin((nPhase + j) * cASC_PI * nWidth / nLen / 256)), pTexture, i, 0, i + 1, nHeight, 1, 1, 
				Color, Color, Color, Color, Effect);
		}
	}
}

void CASCCanvas::RenderTextureWaveY(ASCSingle fX, ASCSingle fY, CASCTexture* pTexture, ASCInt nWidth, ASCInt nHeight, ASCInt nAmp, ASCInt nLen, ASCInt nPhase, 
									ASCColor Color /*= cASCColor32White*/, CASCBlendingEffect Effect /*= abeNormal*/)
{
	if (pTexture)
	{
		ASCInt nH = pTexture->GetHeight();
		ASCInt i;
		for (ASCInt j = 0; j < nHeight; j++)
		{
			i = ASCTrunc(j * nH / nHeight);
			RenderPartTexture((ASCSingle)(fX + nAmp * sin((nPhase + j) * cASC_PI * nHeight / nLen / 256)), fY + j, pTexture, 0, i, nWidth, 1, 1, 1, 
				Color, Color, Color, Color, Effect);
		}
	}
}