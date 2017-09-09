// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ECENGINELIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ECENGINELIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ECENGINELIB_EXPORTS
#define ECENGINELIB_API __declspec(dllexport)
#else
#define ECENGINELIB_API __declspec(dllimport)
#endif

// This class is exported from the ECEngineLib.dll
class ECENGINELIB_API CECEngineLib {
public:
	CECEngineLib(void);
	// TODO: add your methods here.
};

extern ECENGINELIB_API int nECEngineLib;

ECENGINELIB_API int fnECEngineLib(void);
