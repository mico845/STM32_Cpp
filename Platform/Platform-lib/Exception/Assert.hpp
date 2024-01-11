//
// Created by Whisky on 12/25/2023.
//
#ifndef CODE_ASSERT_H
#define CODE_ASSERT_H
#include "Platform-STM32.h"

#define USE_PLATFORM_ASSERT


#ifdef  USE_PLATFORM_ASSERT
    #define ASSERT_PARAM(expr) ((expr) ? (void)0U : Platform_AssertFailed((uint8_t *)__FILE__, __LINE__))
    #define ASSERT_THROW(expr, error) ((expr) ? (void)0U : Platform_AssertFailed((uint8_t *)__FILE__, __LINE__, error))
    void Platform_AssertFailed(uint8_t* file, uint32_t line);
    void Platform_AssertFailed(uint8_t* file, uint32_t line, const char *error);
#else
    #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#endif //CODE_ASSERT_H
