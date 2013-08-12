#include "ASCWindowsConnector.h"

CASCWindowsConnector* ASCWindowsConnector()
{
	static CASCWindowsConnector gASCWindowsConnector;
	return &gASCWindowsConnector;
}