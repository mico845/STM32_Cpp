#ifndef __PLATFORM_LOCKFUN_H__
#define __PLATFORM_LOCKFUN_H__

/**
 * @brief 系统锁 锁定状态
 */
typedef enum Lock_TypeDef{
    UnLocked,
    Locked,
}Lock_TypeDef;

#ifndef __LOCK
/**
 * @brief 锁定宏，用于尝试锁定资源
 * @param __HANDLE__ 锁定状态
 */
#define __LOCK(__HANDLE__)                                                 \
                                do{                                        \
                                    if((__HANDLE__) == Locked)             \
                                    {                                      \
                                       return false;                       \
                                    }                                      \
                                    else                                   \
                                    {                                      \
                                       (__HANDLE__) = Locked;              \
                                    }                                      \
                                  }while (0)
#endif

#ifndef __UNLOCK
/**
 * @brief 解锁宏，用于释放资源的锁定状态
 * @param __HANDLE__ 资源锁定状态
 */
#define __UNLOCK(__HANDLE__)                                                \
                                  do{                                       \
                                      (__HANDLE__) = UnLocked;              \
                                    }while (0)
#endif

#endif //__PLATFORM_LOCKFUN_H__
