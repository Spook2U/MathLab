
#pragma once

class DebugTools
{
public:
   DebugTools();
   ~DebugTools();
};

#define UE_BUILD_DEBUG_2

#ifdef UE_BUILD_DEBUG_2
#define printLog(p)       debug_uelog(p, 0)
#define printWarning(p)   debug_uelog(p, 1)
#define printError(p)     debug_uelog(p, 2)
#define printScreen(p, c) debug_uescreen(p, c)
#else
#define printLog(p) 
#define printWarning(p) 
#define printError(p) 
#define printScreen(p, c)
#endif

#ifdef UE_BUILD_DEBUG_2

DECLARE_LOG_CATEGORY_EXTERN(MLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(MLogScreen, Log, All);

void debug_uelog(int     i, int mode);
void debug_uelog(float   f, int mode);
void debug_uelog(double  d, int mode);
void debug_uelog(FString s, int mode);

void debug_uescreen(int     i, FColor color);
void debug_uescreen(float   f, FColor color);
void debug_uescreen(double  d, FColor color);
void debug_uescreen(FString s, FColor color);

#endif
