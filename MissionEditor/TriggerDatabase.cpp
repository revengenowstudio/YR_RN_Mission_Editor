#include "StdAfx.h"
#include "TriggerDef.h"
#include "TriggerDatabase.h"

TriggerDatabase& TriggerDatabase::Instance()
{
    static TriggerDatabase inst;
    return inst;
}
