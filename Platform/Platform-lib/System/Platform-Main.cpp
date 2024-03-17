#include "platform-lib/System/callInitFunctions.hpp"
#include "common_inc.h"
#include "platform-lib/System/Assert.hpp"

InitFunction InitFunctions[MAX_INIT_FUNCTIONS];
int InitFunctionCount = 0;

void callInitFunctions(void) {
    for (int i = 0; i < InitFunctionCount; ++i)
        InitFunctions[i]();
}

void Main(void)
{
    callInitFunctions();
    Init();
    while (1)
    {
        Loop();
    }
}