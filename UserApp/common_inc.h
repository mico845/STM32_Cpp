//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_COMMON_INC_H
#define CODE_COMMON_INC_H

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/
#include "stdint-gcc.h"
/* System Main Function */
void Main();

#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/
/* Includes */
#include "Machine.hpp"
using namespace Platform::Machine;
/* System Initialize Function */
void Init(void);
void Loop(void);
#endif

#endif //CODE_COMMON_INC_H
