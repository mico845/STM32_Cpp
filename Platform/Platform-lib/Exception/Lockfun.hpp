//
// Created by Whisky on 1/13/2024.
//

#ifndef HELLOWORLD_LOCKFUN_H
#define HELLOWORLD_LOCKFUN_H

typedef enum Lock_TypeDef{
    UnLocked,
    Locked,
}Lock_TypeDef;

#ifndef __LOCK
#define __LOCK(__HANDLE__)                                                 \
                                do{                                        \
                                    if((__HANDLE__) == Locked)             \
                                    {                                      \
                                       return;                             \
                                    }                                      \
                                    else                                   \
                                    {                                      \
                                       (__HANDLE__) = Locked;              \
                                    }                                      \
                                  }while (0)
#endif

#ifndef __UNLOCK
#define __UNLOCK(__HANDLE__)                                                \
                                  do{                                       \
                                      (__HANDLE__) = UnLocked;              \
                                    }while (0)
#endif

#endif //HELLOWORLD_LOCKFUN_H
