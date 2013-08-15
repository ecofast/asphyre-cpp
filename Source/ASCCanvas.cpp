#include "ASCCanvas.h"
#include "ASCEvents.h"

CASCCanvas::CASCCanvas()
{
	m_nCacheStall = 0;

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

void CASCCanvas::RenderLine(const CASCFloatVector2D Src, const CASCFloatVector2D Dest, ASCColor uColor)
{
	RenderLine(Src, Dest, uColor, uColor);
}

void CASCCanvas::RenderLine(ASCSingle fX1, ASCSingle fY1, ASCSingle fX2, ASCSingle fY2, ASCColor uColor)
{
	RenderLine(CASCFloatVector2D(fX1, fY1), CASCFloatVector2D(fX2, fY2), uColor);
}
