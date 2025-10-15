#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdarg.h>


#include "sslib\c\CUtils.h"
// Código solo para C++
#ifdef __cplusplus

#include <fstream>
#include <iostream>

#include "sslib\cpp\Dbg.hpp"
#include "sslib\cpp\Crono.hpp"
#include "sslib\cpp\Filesys.hpp"
#endif

#include <Windows.h>
// Permite acomodar todas las rutas en un disco NTFS. Suele haber algunas 
// rutas largas, pero solo son un poco mayores de MAX_PATH
#define SSLIB_MAX_RUTA (MAX_PATH +200)
