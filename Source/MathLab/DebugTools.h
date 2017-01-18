
#pragma once

class ADebugTools
{
public:
   ADebugTools();
   ~ADebugTools();
};

#define _UE_BUILD_DEBUG_FLAG_



#ifdef _UE_BUILD_DEBUG_FLAG_
#define __FILENAME__       (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define MLD_FUNCTION_NAME  *FString(__func__)
#define MLD_CLASS_NAME     *FString(__FILENAME__)

DECLARE_LOG_CATEGORY_EXTERN(MLogCode, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(MLogBlue, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(MLogScrn, Log, All);
#endif



#ifdef _UE_BUILD_DEBUG_FLAG_
enum MLog
{
   Code,
   Scrn,
   Blue
};

#define MLD_PTR_CHECK(x) doTest((x) != 0, __FILENAME__, __LINE__, __func__, #x)

#define MLD_LOG(string, ...)        UE_LOG(MLogCode, Log,     TEXT("%s"), *debug_BuildText(MLD_CLASS_NAME, FString::Printf(TEXT(string),##__VA_ARGS__)))
#define MLD_WAR(string, ...)        UE_LOG(MLogCode, Warning, TEXT("%s%s"), *debug_ClassPrefix(MLD_CLASS_NAME), *FString::Printf(TEXT(string),##__VA_ARGS__))
#define MLD_ERR(string, ...)        UE_LOG(MLogCode, Error,   TEXT("%s%s"), *debug_ClassPrefix(MLD_CLASS_NAME), *FString::Printf(TEXT(string),##__VA_ARGS__))

#define MLD_SCN(color, string, ...) debug_uescreen(*FString::Printf(TEXT(string),##__VA_ARGS__), debug_ClassPrefix(MLD_CLASS_NAME), color, MLog::Scrn)
#define MLD_BLP(color, string, ...) debug_uescreen(*FString::Printf(TEXT(string),##__VA_ARGS__), debug_ClassPrefix(MLD_CLASS_NAME), color, MLog::Blue)

#define MLD_CALLTEST(string, ...)   UE_LOG(MLogCode, Log,     TEXT("%s%s() %s"), *debug_ClassPrefix(MLD_CLASS_NAME), MLD_FUNCTION_NAME, *FString::Printf(TEXT(string),##__VA_ARGS__))
#else
#define MLD_PTR_CHECK(x)
#define MLD_LOG(string, ...) 
#define MLD_WAR(string, ...) 
#define MLD_ERR(string, ...) 
#define MLD_SCN(color, string, ...)
#define MLD_BLP(color, string, ...)
#define MLD_CALLTEST(string, ...)
#endif



#ifdef _UE_BUILD_DEBUG_FLAG_
FString debug_ClassPrefix(FString classname);
FString debug_BuildText(FString classname, FString s);
void doTest(bool valid, LPCSTR file, int line, LPCSTR func, LPCSTR name);
void debug_uescreen(FString s, FString classname, FColor color, int mode);
#endif

