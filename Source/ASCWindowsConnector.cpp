#include "ASCWindowsConnector.h"

CASCWindowsConnector* ASCWindowsConnector()
{
	static CASCWindowsConnector gASCWindowsConnector;
	return &gASCWindowsConnector;
}

CASCWindowsConnector::CASCWindowsConnector()
{
	m_bInitialized = false;
}

CASCWindowsConnector::~CASCWindowsConnector()
{

}

ASCBoolean CASCWindowsConnector::Init()
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

void CASCWindowsConnector::Done()
{
	if (!m_bInitialized)
	{
		return;
	}

	ASCDestroyEvent()->Notify(this);
	m_bInitialized = false;
}

ASCBoolean CASCWindowsConnector::IsInitialized()
{
	return m_bInitialized;
}