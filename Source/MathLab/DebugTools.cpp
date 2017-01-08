#include "MathLab.h"
#include "DebugTools.h"
#include "Engine.h"

DebugTools::DebugTools()  {}
DebugTools::~DebugTools() {}



#ifdef UE_BUILD_DEBUG_2

DEFINE_LOG_CATEGORY(MLog);
DEFINE_LOG_CATEGORY(MLogScreen);

void debug_uelog(int     i, int mode) { debug_uelog(FString::FromInt(i), mode); }
void debug_uelog(float   f, int mode) { debug_uelog(FString::SanitizeFloat(f), mode); }
void debug_uelog(double  d, int mode) { debug_uelog(FString::SanitizeFloat(d), mode); }
void debug_uelog(FString s, int mode)
{  
   switch(mode)
   {
      case 0:  UE_LOG(MLog, Log, TEXT("%s"), *s); break;
      case 1:  UE_LOG(MLog, Warning, TEXT("%s"), *s); break;
      case 2:  UE_LOG(MLog, Error, TEXT("%s"), *s); break;
   }
}

void debug_uescreen(int     i, FColor color) { debug_uescreen(FString::FromInt(i), color); }
void debug_uescreen(float   f, FColor color) { debug_uescreen(FString::SanitizeFloat(f), color); }
void debug_uescreen(double  d, FColor color) { debug_uescreen(FString::SanitizeFloat(d), color); }
void debug_uescreen(FString s, FColor color)
{
   GEngine->AddOnScreenDebugMessage(-1, 30, color, s);
   UE_LOG(MLogScreen, Log, TEXT("%s"), *s);
}

#endif
