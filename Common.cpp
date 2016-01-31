#include <stdlib.h>
#include "Common.h"

#ifdef WIN32

static char FormatBuffer[4096]; 
static CRITICAL_SECTION CriticalSection;
static bool CriticalSectionInitialized = false;

void DbgPrint(const char* fmt, ...)
{
	va_list args;

	if (!CriticalSectionInitialized)
	{
		InitializeCriticalSection(&CriticalSection);
		CriticalSectionInitialized = true;
	}

	EnterCriticalSection(&CriticalSection);

	va_start(args, fmt);
	vsprintf_s(FormatBuffer, 4096, fmt, args);
	OutputDebugStringA(FormatBuffer);

	LeaveCriticalSection(&CriticalSection);
}

#else

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <Arduino.h>

void DbgPrint(const char* fmt, ...)
{
        char FormatBuffer[128]; 
	va_list args;
	va_start(args, fmt);
        vsprintf(FormatBuffer, fmt, args);
       
        char c;
        char* addr = FormatBuffer;
      
        while ((c = *addr++))
        {
          Serial.print(c);
        }
}

#endif 
