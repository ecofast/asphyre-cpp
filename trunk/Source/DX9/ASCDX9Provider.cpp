#include "ASCDX9Provider.h"

CASCDX9Provider* ASCDX9Provider()
{
	static CASCDX9Provider gASCDX9Provider;
	return &gASCDX9Provider;
}