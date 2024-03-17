#ifndef __PLATFORM_ARRAY_H__
#define __PLATFORM_ARRAY_H__

#include "platform-lib/System/Assert.hpp"

/**
 * @brief 数组模板类
 *
 * 用于实现可变长数组。
 *
 * @tparam T 数组元素类型
 * @tparam MAX_SIZE 数组最大大小
 */
template<typename T, uint32_t MAX_SIZE>
class Array
{
public:
    /* parameter */
    T Data[MAX_SIZE];   /**< 数组数据 */
    uint32_t current_size;      /**< 数组大小 */
    /* constructor */
    /**
     * @brief Array类构造函数
     *
     * 初始化数组大小为最大大小。
     */
    Array() : current_size(MAX_SIZE) { }
    /**
     * @brief Array类构造函数
     *
     * 指定数组大小。
     *
     * @param size 数组大小
     */
    Array(uint32_t size) : current_size(size) { ASSERT_THROW((!(size > MAX_SIZE)), "Array size out of range"); }
    /**
     * @brief Array类构造函数
     *
     * 指定数组大小和初始数据。
     *
     * @param buf 初始数据数组
     * @param size 数组大小
     */
    Array(T buf[], uint32_t size) : current_size(size)
    {
        ASSERT_THROW((!(size > MAX_SIZE)), "Array size out of range");
        for (uint32_t i = 0; i < size; ++i) {
            Data[i] = buf[i];
        }
    }
    /* operator */
    /**
     * @brief 重载[]运算符
     *
     * 用于访问数组元素。
     *
     * @param Index 元素索引
     * @return T& 对应索引的数组元素的引用
     */
    T& operator [] (uint32_t Index)
    {
        ASSERT_THROW((!(Index >= MAX_SIZE)), "Array Index out of range");
        return Data[Index];
    }


    /**
     * @brief 设置数组缓冲区和大小
     * @param buf 缓冲区指针
     * @param num 数组大小
     */
    void setBuffer(const T *buf, uint32_t num);
};

template<typename T, uint32_t MAX_SIZE>
void Array<T, MAX_SIZE>::setBuffer(const T *buf, uint32_t num) {
    ASSERT_THROW((!(num > MAX_SIZE)), "Array size out of range");
    current_size = num;
    for (uint32_t i = 0; i < current_size; ++i) {
        Data[i] = buf[i];
    }
}

#endif //__PLATFORM_ARRAY_H__
