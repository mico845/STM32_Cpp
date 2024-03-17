
#ifndef __PLATFORM_ASSERT_H__
#define __PLATFORM_ASSERT_H__

#include <stdint.h>

#define USE_PLATFORM_ASSERT

#ifdef  USE_PLATFORM_ASSERT
    /**
     * @brief 断言宏，用于检查表达式是否为真，否则执行断言函数
     *
     * 函数Platform_AssertFailed中可更改断言函数。
     * @param expr 待检查的表达式
     */
    #define ASSERT_PARAM(expr) ((expr) ? (void)0U : Platform_AssertFailed((uint8_t *)__FILE__, __LINE__))
    /**
     * @brief 断言宏，用于检查表达式是否为真，否则执行断言函数，并输出错误信息
     * @param expr 待检查的表达式
     * @param error 错误信息字符串
     */
    #define ASSERT_THROW(expr, error) ((expr) ? (void)0U : Platform_AssertFailed((uint8_t *)__FILE__, __LINE__, error))


    void Platform_AssertFailed(uint8_t* file, uint32_t line);
    void Platform_AssertFailed(uint8_t* file, uint32_t line, const char *error);
#else
    #define assert_param(expr) ((void)0U)
#endif //USE_PLATFORM_ASSERT

#endif //__PLATFORM_ASSERT_H__
