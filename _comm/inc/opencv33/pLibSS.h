#pragma message("********>> Debe compilar el proyecto usando su propia biblioteca nusuario-SSLib")
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>

// Versión de CheckError curso 2022-23
BOOL CheckError(BOOL ErrorCondition, LPCTSTR userMessage,
	DWORD exitCode);

double MideRetraso(LPCTSTR pMensaje);