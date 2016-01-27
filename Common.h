#ifndef COMMON_H__
#define COMMON_H__

#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#include <assert.h>

#define ASSERT(x) assert(x)
void DbgPrint(const char* fmt, ...);

#else

#define ASSERT(x) ((void)0)
void DbgPrint(const char* fmt, ...);
#define NULL 0

#endif

extern "C"
{
extern unsigned long micros();
extern void delayMicroseconds(unsigned int);
}

#endif // COMMON_H__
