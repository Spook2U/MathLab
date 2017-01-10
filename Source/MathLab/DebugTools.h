
#pragma once

class DebugTools
{
public:
   DebugTools();
   ~DebugTools();
};



#define _UE_BUILD_DEBUG_FLAG_

#ifdef _UE_BUILD_DEBUG_FLAG_
#define POINTERTEST(x) d_ASSERT((x) != 0, __FILE__, __LINE__ )
BOOL d_ASSERT(BOOL bCond, LPCSTR pszFile, int nLine);

#else
#define ASSERT( x )
#endif


#ifdef _UE_BUILD_DEBUG_FLAG_

#define PRINTLOG(string, ...)        UE_LOG(MLog, Log,     TEXT("%s"), *FString::Printf(TEXT(string),##__VA_ARGS__))
#define PRINTWAR(string, ...)        UE_LOG(MLog, Warning, TEXT("%s"), *FString::Printf(TEXT(string),##__VA_ARGS__))
#define PRINTERR(string, ...)        UE_LOG(MLog, Error,   TEXT("%s"), *FString::Printf(TEXT(string),##__VA_ARGS__))
#define PRINTSCN(color, string, ...) debug_uescreen(FString::Printf(TEXT(string),##__VA_ARGS__), color)
#else
#define PRINTLOG(string, ...) 
#define PRINTWAR(string, ...) 
#define PRINTERR(string, ...) 
#define PRINTSCN(color, string, ...)
#endif

#ifdef _UE_BUILD_DEBUG_FLAG_

DECLARE_LOG_CATEGORY_EXTERN(MLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(MLogScreen, Log, All);

void debug_uescreen(FString s, FColor color);

#endif
