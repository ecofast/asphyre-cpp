#include "ASCEvents.h"

/*
 * Definition of important ASC events
 * to do: error checking handler...
*/
CASCEventProvider* ASCCreateEvent()
{
	static CASCEventProvider gASCCreateEvent = *(ASCEventProviders()->Add());
	return &gASCCreateEvent;
}

CASCEventProvider* ASCDestroyEvent()
{
	static CASCEventProvider gASCDestroyEvent = *(ASCEventProviders()->Add());
	return &gASCDestroyEvent;
}

CASCEventProvider* ASCDeviceInitEvent()
{
	static CASCEventProvider gASCDeviceInitEvent = *(ASCEventProviders()->Add());
	return &gASCDeviceInitEvent;
}

CASCEventProvider* ASCDeviceCreateEvent()
{
	static CASCEventProvider gASCDeviceCreateEvent = *(ASCEventProviders()->Add());
	return &gASCDeviceCreateEvent;
}

CASCEventProvider* ASCDeviceDestroyEvent()
{
	static CASCEventProvider gASCDeviceDestroyEvent = *(ASCEventProviders()->Add());
	return &gASCDeviceDestroyEvent;
}

CASCEventProvider* ASCDeviceResetEvent()
{
	static CASCEventProvider gASCDeviceResetEvent = *(ASCEventProviders()->Add());
	return &gASCDeviceResetEvent;
}

CASCEventProvider* ASCDeviceLostEvent()
{
	static CASCEventProvider gASCDeviceLostEvent = *(ASCEventProviders()->Add());
	return &gASCDeviceLostEvent;
}

CASCEventProvider* ASCBeginSceneEvent()
{
	static CASCEventProvider gASCBeginSceneEvent = *(ASCEventProviders()->Add());
	return &gASCBeginSceneEvent;
}

CASCEventProvider* ASCEndSceneEvent()
{
	static CASCEventProvider gASCEndSceneEvent = *(ASCEventProviders()->Add());
	return &gASCEndSceneEvent;
}

CASCEventProvider* ASCTimerResetEvent()
{
	static CASCEventProvider gASCTimerResetEvent = *(ASCEventProviders()->Add());
	return &gASCTimerResetEvent;
}