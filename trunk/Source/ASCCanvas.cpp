#include "ASCCanvas.h"
#include "ASCEvents.h"

CASCCanvas::CASCCanvas()
{
	m_nCacheStall	= 0;

	ASCDeviceCreateEvent()->Subscribe(typeid(*this).name(), OnDeviceCreate);
	ASCDeviceDestroyEvent()->Subscribe(typeid(*this).name(), OnDeviceDestroy);
	ASCDeviceResetEvent()->Subscribe(typeid(*this).name(), OnDeviceReset);
	ASCDeviceLostEvent()->Subscribe(typeid(*this).name(), OnDeviceLost);
	ASCBeginSceneEvent()->Subscribe(typeid(*this).name(), OnBeginScene);
	ASCEndSceneEvent()->Subscribe(typeid(*this).name(), OnEndScene);

}

CASCCanvas::~CASCCanvas()
{
	ASCEventProviders()->UnSubscribe(typeid(*this).name());
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

void CASCCanvas::OnDeviceCreate(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled)
{
	ASCBoolean bSucced = HandleDeviceCreate();
	if (pParam)
	{
		*((ASCBoolean*)pParam) = *((ASCBoolean*)pParam) && bSucced;
	}
}

void CASCCanvas::OnDeviceDestroy(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled)
{
	HandleDeviceDestroy();
}

void CASCCanvas::OnDeviceReset(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled)
{
	ASCBoolean bSucced = HandleDeviceReset();
	if (pParam)
	{
		*((ASCBoolean*)pParam) = *((ASCBoolean*)pParam) && bSucced;
	}
}

void CASCCanvas::OnDeviceLost(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled)
{
	HandleDeviceLost();
}

void CASCCanvas::OnBeginScene(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled)
{
	m_nCacheStall = 0;
	HandleBeginScene();
}

void CASCCanvas::OnEndScene(const void* pSender, const CASCPointer pParam, ASCBoolean* bHandled)
{
	HandleEndScene();
}