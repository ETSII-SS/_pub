//#include <Windows.h>
//
//
//BOOL APIENTRY DllMain( HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved )
//{
//	switch (ul_reason_for_call)       // <-- esto es opcional   
//	{
//	case DLL_PROCESS_ATTACH:   // Posibles causas de entrada en DllMain
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;    // <--esto es obliatorio.
//}