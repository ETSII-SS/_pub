#pragma once

#include <Windows.h>
#include "LibInfoCPU\LibInfoCPU.h"

// está definido cuando se compila el proyecto como DLL
#ifdef _WINDLL
#define _EXPORT __declspec (dllexport)
#else
#define _EXPORT 
#endif

_EXPORT BOOL CheckError (BOOL Test, LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage);