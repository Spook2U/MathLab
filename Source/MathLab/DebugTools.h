
#pragma once

class DebugTools
{
public:
   DebugTools();
   ~DebugTools();
};



#define _UE_BUILD_DEBUG_FLAG_

#ifdef _UE_BUILD_DEBUG_FLAG_
#define ASSERT(x) d_ASSERT((x) != 0, __FILE__, __LINE__ );
BOOL d_ASSERT(BOOL bCond, LPCSTR pszFile, int nLine);

#else
#define ASSERT( x )
#endif


#ifdef _UE_BUILD_DEBUG_FLAG_

#define printLog(string, ...)     UE_LOG(MLog, Log,     TEXT("%s"), *FString::Printf(TEXT(string),##__VA_ARGS__))
#define printWarning(string, ...) UE_LOG(MLog, Warning, TEXT("%s"), *FString::Printf(TEXT(string),##__VA_ARGS__))
#define printError(string, ...)   UE_LOG(MLog, Error,   TEXT("%s"), *FString::Printf(TEXT(string),##__VA_ARGS__))
#define printScreen(string, ...)  debug_uescreen(FString::Printf(TEXT(string),##__VA_ARGS__))
#else
#define printLog(string, ...) 
#define printWarning(string, ...) 
#define printError(string, ...) 
#define printScreen(string, ...)
#endif

#ifdef _UE_BUILD_DEBUG_FLAG_

DECLARE_LOG_CATEGORY_EXTERN(MLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(MLogScreen, Log, All);

void debug_uelog(FString s, int mode);

void debug_uescreen(FString s, FColor color);

#endif
