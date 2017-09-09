// ECEngineLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ECEngineLib.h"


// This is an example of an exported variable
ECENGINELIB_API int nECEngineLib=0;

// This is an example of an exported function.
ECENGINELIB_API int fnECEngineLib(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ECEngineLib.h for the class definition
CECEngineLib::CECEngineLib()
{
    return;
}
