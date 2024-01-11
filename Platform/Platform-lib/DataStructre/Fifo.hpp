//
// Created by Whisky on 12/30/2023.
//

#ifndef CODE_FIFO_H
#define CODE_FIFO_H

#ifndef __TYPEDEF_LOCK_FUN__
    typedef void (*lock_fun)(void);
#endif

template<typename T, uint32_t MAX_SIZE>
class Fifo
{
private:
    T buf[MAX_SIZE];
    uint32_t size;          /* 有效数据大小 */
    uint32_t pwriteIndex;   /* 写索引 */
    uint32_t preadIndex;    /* 读索引 */
    void (*lock)(void);	    /* 互斥上锁 */
    void (*unlock)(void);   /* 互斥解锁 */
public:

    Fifo(lock_fun lock = nullptr, lock_fun unlock = nullptr);
    ~Fifo();

    uint32_t Put(const T &data);
    uint32_t Get(T &data);
    uint32_t Puts(T *pData, uint32_t num);
    uint32_t Gets(T *pData, uint32_t num);
    uint32_t Size(void);
    uint32_t Get_FreeSize(void);
    void Clear(void);
};

/**
 * Constructor,Initialize the queue
 */
template<typename T, uint32_t MAX_SIZE>
Fifo<T,MAX_SIZE>::Fifo(lock_fun Lock_fun, lock_fun Unlock_fun)
        :size(0), pwriteIndex(0), preadIndex(0), lock(Lock_fun), unlock (Unlock_fun)
{

}

template<typename T, uint32_t MAX_SIZE>
Fifo<T, MAX_SIZE>::~Fifo() {
    size = 0;
    pwriteIndex = 0;
    preadIndex = 0;
    lock = nullptr;
    unlock = nullptr;
}


/**
 * input one node to buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::Put(const T &c)
{
    if(Get_FreeSize() == 0)
        return 0;
    if (lock != nullptr)
        lock();
    buf[pwriteIndex++] = c;
    if (pwriteIndex >= MAX_SIZE)
        pwriteIndex = 0;
    size++;
    if (unlock != nullptr)
        unlock();
    return 1;
}


/**
 * input multi nodes to buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::Puts(T *pData, uint32_t num)
{
    uint32_t w_size, free_size;
    if (num==0)
        return 0;

    free_size = Get_FreeSize();
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
        size++;
    }
    if (unlock != nullptr)
        unlock();
    return num;
}

/**
 * get one node from buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::Get(T &data)
{
    if(size == 0)
        return 0;
    if (lock != nullptr)
        lock();
    data = buf[preadIndex++];
    if (preadIndex >= MAX_SIZE)
        preadIndex = 0;
    size--;
    if (unlock != nullptr)
        unlock();
    return 1;
}

/**
 * get multi nodes from buffer
 */
template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::Gets(T *pData, uint32_t num)
{
    uint32_t r_size, occupy_size;
    if (num==0)
        return 0;

    occupy_size = size;

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
        size--;
    }
    if (unlock != nullptr)
        unlock();
    return num;
}

template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T,MAX_SIZE>::Size()
{
    return (size);
}

/**
 *clear all nodes in buffer
 */
template<typename T, uint32_t MAX_SIZE>
void Fifo<T,MAX_SIZE>::Clear()
{
    pwriteIndex = 0;
    preadIndex = 0;
    size = 0;
}

template<typename T, uint32_t MAX_SIZE>
uint32_t Fifo<T, MAX_SIZE>::Get_FreeSize(void) {
    return (MAX_SIZE - size);
}


#endif //CODE_FIFO_H
