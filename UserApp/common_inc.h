#ifndef CODE_COMMON_INC_H
#define CODE_COMMON_INC_H


#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/

/* System Initialize and Loop Function */
void Init(void);
void Loop(void);
/* System Main Function */
void Main(void);

#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/
/* Includes */
#include "Machine.hpp"

#endif

#endif //CODE_COMMON_INC_H

