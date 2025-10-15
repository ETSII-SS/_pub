#include <Windows.h>
#include "..\SSLib.h"

// Esta pragma sólo existe en Visual C++
#pragma region Definición de tablas de características

// Se ha modificado la definición de las tablas de características originales
//		las 4 tablas en una tabla de 2 dimensiones.
// Todas las tablas son definiciones "strong" de variables globales.



static Caracteristica_t TablasAGrabar[]= 
// Tablas correspondientes a AMD (sep 2010)
// Datos tomados de http://support.amd.com/us/Embedded_TechDocs/25481.pdf

{
	{31, "RAZ", "Reservad for use by hypervisor to indicate guest status"}, 
	{29, "F16C", "half-precision convert instruction support"}, 
	{28, "AVX", "AVX instruction support"}, 
	{27, "OSXSAVE", "XSAVE (and related) instructions are enabled"}, 
	{26, "XSAVE", "XSAVE (and related) instructions are supported by hardware"}, 
	{25, "AES", "AES instruction support"}, 
	{23, "POPCNT", "POPCNT instruction"}, 
	{20, "SSE42", "SSE4.2 instruction support"}, 
	{19, "SSE41", "SSE4.1 instruction support"}, 
	{13, "CMPXCHG16B", "CMPXCHG16B instruction"}, 
	{12, "FMA", "FMA instruction support"}, 
	{9, "SSSE3", "supplemental SSE3 instruction support"}, 
	{3, "MONITOR", "MONITOR/MWAIT instructions"}, 
	{1, "PCLMULQDQ", "PCLMULQDQ instruction support"}, 
	{0, "SSE3", "SSE3 instruction support"}, 
	{-1, "",        "" },	// Indica final de la tabla
//},
//Caracteristica_t TablaCaracteristicasEDXAMD[] =
//{
	// Estas son las caracteristicas que se devuelven en el registro EDX
	{28, "HTT", "hyper-threading technology"}, 
	{26, "SSE2", "SSE2 instruction support"}, 
	{25, "SSE", "SSE instruction support"}, 
	{24, "FXSR", "FXSAVE and FXRSTOR instructions"}, 
	{23, "MMX", "MMX™ instructions"}, 
	{19, "CLFSH", "CLFLUSH instruction support"}, 
	{17, "PSE36", "page-size extensions"}, 
	{16, "PAT", "page attribute table"}, 
	{15, "CMOV", "conditional move instructions"}, 
	{14, "MCA", "machine check architecture"}, 
	{13, "PGE", "page global extension"}, 
	{12, "MTRR", "memory-type range registers"}, 
	{11, "SysEnterSysExit", "SYSENTER and SYSEXIT instructions"}, 
	{9, "APIC", "advanced programmable interrupt controller"}, 
	{8, "CMPXCHG8B", "CMPXCHG8B instruction"}, 
	{7, "MCE", "Machine check exception"}, 
	{6, " PAE", "physical-address extensions"}, 
	{5, "MSR", "AMD model-specific registers"}, 
	{4, "TSC", "time stamp counter"}, 
	{3, "PSE", "page-size extensions"}, 
	{2, "DE", "debugging extensions"}, 
	{1, "VME", "virtual-mode enhancements"}, 
	{0, "FPU", "x87 floating point unit on-chip"}, 
	{-1, "",        "" },	// Indica final de la tabla
//};

// Tablas correspondientes a Intel (may 2012)
// Datos tomados de http://code.google.com/p/computer-performance-test/source/browse/cpuid/CPUFunctionPrint.cpp?spec=svnc1de392bb3a7de963a893d2dd0e397762040f365&r=c1de392bb3a7de963a893d2dd0e397762040f365
//Caracteristica_t TablaCaracteristicasECXIntel[] =
//{
	{  0, "SSE3",           "Streaming SIMD Extensions 3" },
	{  1, "PCLMULDQ",       "PCLMULDQ Instruction" },
	{  2, "DTES64",         "64-Bit Debug Store" },
	{  3, "MONITOR",        "MONITOR/MWAIT" },
	{  4, "DS-CPL",         "CPL Qualified Debug Store" },
	{  5, "VMX",            "Virtual Machine Extensions" },
	{  6, "SMX",            "Safer Mode Extensions" },
	{  7, "EIST",           "Enhanced Intel SpeedStep® Technology" },
	{  8, "TM2",            "Thermal Monitor 2" },
	{  9, "SSSE3",          "Supplemental Streaming SIMD Extensions 3" },
	{ 10, "CNXT-ID",        "L1 Context ID" },
	{ 12, "FMA",            "Fused Multiply Add" },
	{ 13, "CX16",           "CMPXCHG16B" },
	{ 14, "xTPR",           "xTPR Update Control" },
	{ 15, "PDCM",           "Perfmon and Debug Capability" },
	{ 17, "PCID",           "Process Context Identifiers" },
	{ 18, "DCA",            "Direct Cache Access" },
	{ 19, "SSE4.1",         "Streaming SIMD Extensions 4.1" },
	{ 20, "SSE4.2",         "Streaming SIMD Extensions 4.2" },
	{ 21, "x2APIC",         "Extended xAPIC Support" },
	{ 22, "MOVBE",          "MOVBE Instruction" },
	{ 23, "POPCNT",         "POPCNT Instruction" },
	{ 24, "TSC-DEADLINE",   "Time Stamp Counter Deadline" },
	{ 25, "AES",            "AES Instruction Extensions" },
	{ 26, "XSAVE",          "XSAVE/XSTOR States" },
	{ 27, "OSXSAVE",        "OS-Enabled Extended State Management" },
	{ 28, "AVX",            "Advanced Vector Extensions" },
	{ 29, "F16C",           "16-bit floating-point conversion instructions" },
	{ 30, "RDRAND",         "RDRAND instruction supported" },
	{-1, "",        "" },	// Indica final de la tabla
//};
//Caracteristica_t TablaCaracteristicasEDXIntel[] =
//{
	// Estas son las caracteristicas que se devuelven en el registro EDX
	{ 0, "FPU",        "Floating-point Unit On-Chip" },
	{ 1, "VME",        "Virtual Mode Extension" },
	{ 2, "DE",         "Debugging Extension" },
	{ 3, "PSE",        "Page Size Extension" },
	{ 4, "TSC",        "Time Stamp Counter" },
	{ 5, "MSR",        "Model Specific Registers" },
	{ 6, "PAE",        "Physical Address Extension" },
	{ 7, "MCE",        "Machine-Check Exception" },
	{ 8, "CX8",        "CMPXCHG8 Instruction" },
	{ 9, "APIC",       "On-chip APIC Hardware" },
	{11, "SEP",        "Fast System Call" },
	{12, "MTRR",       "Memory Type Range Registers" },
	{13, "PGE",        "Page Global Enable" },
	{14, "MCA",        "Machine-Check Architecture" },
	{15, "CMOV",       "Conditional Move Instruction" },
	{16, "PAT",        "Page Attribute Table" },
	{17, "PSE-36",     "36-bit Page Size Extension" },
	{18, "PSN",        "Processor serial number is present and enabled" },
	{19, "CLFSH",      "CLFLUSH Instruction" },
	{21, "DS",         "Debug Store" },
	{22, "ACPI",       "Thermal Monitor and Software Controlled Clock Facilities" },
	{23, "MMX",        "MMX technology" },
	{24, "FXSR",       "FXSAVE and FXSTOR Instructions" },
	{25, "SSE",        "Streaming SIMD Extensions" },
	{26, "SSE2",       "Streaming SIMD Extensions 2" },
	{27, "SS",         "Self-Snoop" },
	{28, "HTT",        "Multi-Threading" },
	{29, "TM",         "Thermal Monitor" },
	{31, "PBE",        "Pending Break Enable" },
	{-1, "",        "" },	// Indica final de la tabla
};
#pragma endregion

/* Escribe en el archivo Tablas.dat el contenido de las 4 tablas de la librería (TablaCaracteristicasECXIntel,
TablaCaracteristicasEDXIntel, TablaCaracteristicasECXAMD  y TablaCaracteristicasEDXAMD). 
La función debe ejecutarse fuera de la librería (en el proyecto TestLib), por lo que el tamaño de las tablas no se conoce. 
Se debe usar las funciones CreateFile, CloseHandle y WriteFile vistas en clase.
*/
void EscribeTablas(void)
{
	int i, j;
	BOOL Resultado;
	HANDLE hArchivo;
	DWORD Escritos;

	hArchivo = CreateFile("Tablas.dat", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// Termina si hay error
	CheckError(hArchivo == INVALID_HANDLE_VALUE, "ERROR abriendo archivo para escribir", -1, TRUE);
	
	j= 0;
	for (i= 0; i<4; i++)	// Hay 4 tablas
	{
		do 
		{
			Resultado= WriteFile(hArchivo, 
							&TablasAGrabar[j], 
							sizeof(Caracteristica_t), 
							&Escritos, NULL);
			CheckError(Resultado==FALSE || Escritos != sizeof(Caracteristica_t), "ERROR escribiendo archivo de tablas", -1, TRUE);
			j++;
   		} while (TablasAGrabar[j-1].NroBit != -1);
	}
	// Cerrar archivo
    CloseHandle (hArchivo);
}