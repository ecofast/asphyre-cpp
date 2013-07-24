#include "ASCFactory.h"

CASCFactory* ASCFactory()
{
	static CASCFactory gASCFactory;
	return &gASCFactory;
}