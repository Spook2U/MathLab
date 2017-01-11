
#pragma once

class ADebugTools
{
public:
   ADebugTools();
   ~ADebugTools();
};



#define _UE_BUILD_DEBUG_FLAG_

#ifdef _UE_BUILD_DEBUG_FLAG_
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define POINTERTEST(x) doTest((x) != 0, __FILENAME__, __LINE__, __func__, #x)

void doTest(bool valid, LPCSTR file, int line, LPCSTR func, LPCSTR name);

#else
#define POINTERTEST(x)
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
