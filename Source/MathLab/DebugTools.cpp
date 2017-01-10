#include "MathLab.h"
#include "DebugTools.h"
#include "Engine.h"

DebugTools::DebugTools()  {}
DebugTools::~DebugTools() {}



#ifdef _UE_BUILD_DEBUG_FLAG_
BOOL d_ASSERT(BOOL bCond, LPCSTR pszFile, int nLine)
{
   if(!bCond)
   {
      PRINTERR("Pointer = NULL, Line %d", nLine);
      //if(nResult == IDABORT)
      //{
      //   
      //   //Game StopPlay()
      //}
      //else if(nResult == IDRETRY)
      //{
      //   return false;
      //}
   }
   return true;
}


DEFINE_LOG_CATEGORY(MLog);
DEFINE_LOG_CATEGORY(MLogScreen);

void debug_uescreen(FString s, FColor color)
{
   GEngine->AddOnScreenDebugMessage(-1, 30, color, s);
   UE_LOG(MLogScreen, Log, TEXT("%s"), *s);
}

#endif
