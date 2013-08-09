#include "ASCDX9Provider.h"

CASCDX9Provider* ASCDX9Provider()
{
	static CASCDX9Provider gASCDX9Provider;
	return &gASCDX9Provider;
}

CASCDX9Provider::CASCDX9Provider() : CASCProvider()
{
	m_uProviderID = ASC_PROVIDERID_DX9;
	ASCFactory()->Subscribe(this);
}

CASCDX9Provider::~CASCDX9Provider()
{
	ASCFactory()->UnSubscribe(this, true);
}

CASCDevice* CASCDX9Provider::CreateDevice()
{
	return (new CASCDX9Device());
}

CASCCanvas* CASCDX9Provider::CreateCanvas()
{
	return (new CASCDX9Canvas());
}

CASCLockableTexture* CASCDX9Provider::CreateLockableTexture()
{
	return (new CASCDX9LockableTexture());
}

CASCRenderTargetTexture* CASCDX9Provider::CreateRenderTargetTexture()
{
	return (new CASCDX9RenderTargetTexture());
}