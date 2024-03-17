#ifndef __PLATFORM_FIFO_H__
#define __PLATFORM_FIFO_H__

#include <stdint.h>

#ifndef __TYPEDEF_LOCK_FUN__
    typedef void (*lock_fun)(void); /**< 互斥锁函数类型定义 */
#endif

/**
 * @brief 环形Fifo模板类
 *
 * 用于实现环形FIFO数据结构。
 *
 * @tparam T 元素类型
 * @tparam MAX_SIZE FIFO最大大小
 */
template<typename T, uint32_t MAX_SIZE>
class Fifo
{
private:
    T buf[MAX_SIZE];         /**< 存储数据的缓冲区 */
    uint32_t current_size;   /**< 有效数据大小 */
    uint32_t pwriteIndex;   /**< 写索引 */
    uint32_t preadIndex;    /**< 读索引 */
    void (*lock)(void);	    /**< 互斥上锁函数指针 */
    void (*unlock)(void);   /**< 互斥解锁函数指针 */
public:

    /**
     * @brief Fifo类构造函数
     * @param Lock_fun 互斥锁函数指针，默认为nullptr
     * @param Unlock_fun 互斥解锁函数指针，默认为nullptr
     */
    Fifo(lock_fun lock = nullptr, lock_fun unlock = nullptr);
    ~Fifo();

    /**
     * @brief 向队列中添加一个元素
     * @param data 要添加的元素
     * @return uint32_t 添加是否成功，成功返回1，失败返回0
     */
    uint32_t put(const T &data);
    /**
     * @brief 从队列中取出一个元素
     * @param data 存储取出的元素
     * @return uint32_t 取出是否成功，成功返回1，失败返回0
     */
    uint32_t get(T &data);

    /**
     * @brief 向队列中添加多个元素
     * @param pData 要添加的元素数组指针
     * @param num 要添加的元素个数
     * @return uint32_t 添加成功的元素个数
     */
    uint32_t puts(T *pData, uint32_t num);
    /**
     * @brief 从队列中取出多个元素
     * @param pData 存储取出的元素的数组指针
     * @param num 要取出的元素个数
     * @return uint32_t 实际取出的元素个数
     */
    uint32_t gets(T *pData, uint32_t num);
    /**
     * @brief 获取当前队列中的元素个数
     * @return uint32_t 当前队列中的元素个数
     */
    inline uint32_t size(void);
    /**
     * @brief 获取当前队列中的空闲空间大小
     * @return uint32_t 当前队列中的空闲空间大小
     */
    inline uint32_t getFreeSize(void);
    /**
     * @brief 清空队列
     */
    inline void clear(void);
};

/**
 * Constructor,Initialize the queue
 */
template<typename T, uint32_t MAX_SIZE>
Fifo<T,MAX_SIZE>::Fifo(lock_fun Lock_fun, lock_fun Unlock_fun)
        :current_size(0), pwriteIndex(0), preadIndex(0), lock(Lock_fun), unlock (Unlock_fun)
{

}

template<typename T, uint32_t MAX_SIZE>
Fifo<T, MAX_SIZE>::~Fifo() {
    current_size = 0;
    pwriteIndex = 0;
    preadIndex = 0;
    lock = nullptr;
    unlock = nullptr;
}


/**
 * input one node to buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::put(const T &c)
{
    if(getFreeSize() == 0)
        return 0;
    if (lock != nullptr)
        lock();
    buf[pwriteIndex++] = c;
    if (pwriteIndex >= MAX_SIZE)
        pwriteIndex = 0;
    current_size++;
    if (unlock != nullptr)
        unlock();
    return 1;
}


/**
 * input multi nodes to buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::puts(T *pData, uint32_t num)
{
    uint32_t w_size, free_size;
    if (num==0)
        return 0;

    free_size = getFreeSize();
    if(free_size == 0)
        return 0;

    if(free_size < num)
        num = free_size;

    w_size = num;
    if (lock != nullptr)
        lock();
    while(w_size-- > 0)
    {
        buf[pwriteIndex++] = *pData++;
        if (pwriteIndex >= MAX_SIZE)
            pwriteIndex = 0;
        current_size++;
    }
    if (unlock != nullptr)
        unlock();
    return num;
}

/**
 * get one node from buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::get(T &data)
{
    if(current_size == 0)
        return 0;
    if (lock != nullptr)
        lock();
    data = buf[preadIndex++];
    if (preadIndex >= MAX_SIZE)
        preadIndex = 0;
    current_size--;
    if (unlock != nullptr)
        unlock();
    return 1;
}

/**
 * get multi nodes from buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::gets(T *pData, uint32_t num)
{
    uint32_t r_size, occupy_size;
    if (num==0)
        return 0;

    occupy_size = current_size;

    if(occupy_size == 0)
        return 0;

    if(occupy_size < num)
        num = occupy_size;

    if (lock != nullptr)
        lock();
    r_size = num;
    while(r_size-- > 0)
    {
        *pData++ = buf[preadIndex++];
        if (preadIndex >= MAX_SIZE)
            preadIndex = 0;
        current_size--;
    }
    if (unlock != nullptr)
        unlock();
    return num;
}

template<typename T, uint32_t MAX_SIZE>
inline uint32_t Fifo<T,MAX_SIZE>::size()
{
    return (current_size);
}

/**
 *clear all nodes in buffer
 */
template<typename T, uint32_t MAX_SIZE>
inline void Fifo<T,MAX_SIZE>::clear()
{
    pwriteIndex = 0;
    preadIndex = 0;
    current_size = 0;
}

template<typename T, uint32_t MAX_SIZE>
inline uint32_t Fifo<T, MAX_SIZE>::getFreeSize(void) {
    return (MAX_SIZE - current_size);
}

#endif //__PLATFORM_FIFO_H__
