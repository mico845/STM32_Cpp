#ifndef __PLATFORM_CALLINITFUN_H__
#define __PLATFORM_CALLINITFUN_H__

#include "platform-lib/System/Assert.hpp"

typedef void (*InitFunction)();

#define MAX_INIT_FUNCTIONS 10   /**< 初始化函数指针数组最大大小 */
extern InitFunction InitFunctions[];
extern int InitFunctionCount;

#define INIT_FUNCTION(name) \
    void name(); \
    void __attribute__((constructor)) int_##name() { \
        ASSERT_THROW(!(InitFunctionCount >= MAX_INIT_FUNCTIONS), "Init Functions are filled!\r\n");\
        InitFunctions[InitFunctionCount++] = &name; \
    } \
    void name()

void callInitFunctions(void);

#endif //__PLATFORM_CALLINITFUN_H__