#include "ASCProvider.h"

CASCFactory* ASCFactory()
{
	static CASCFactory gASCFactory;
	return &gASCFactory;
}