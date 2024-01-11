//
// Created by Whisky on 1/12/2024.
//

#ifndef HELLOWORLD_ARRAY_H
#define HELLOWORLD_ARRAY_H

#include "Exception/Assert.hpp"

template<typename T, uint32_t MAX_SIZE>
class Array
{
public:
    /* parameter */
    T Data[MAX_SIZE];
    uint32_t Size;
    /* constructor */
    Array() : Size(MAX_SIZE) { }
    Array(uint32_t size) : Size(size) { ASSERT_THROW((!(size >= MAX_SIZE)), "Array Size out of range"); }
    Array(T buf[], uint32_t size) : Size(size)
    {
        ASSERT_THROW((!(size >= MAX_SIZE)), "Array Size out of range");
        for (uint32_t i = 0; i < size; ++i) {
            Data[i] = buf[i];
        }
    }
    /* operator */
    T& operator [] (uint32_t Index)
    {
        ASSERT_THROW((!(Index >= MAX_SIZE)), "Array Index out of range");
        return Data[Index];
    }

    void SetBuffer(T *buf, uint32_t num);
};

template<typename T, uint32_t MAX_SIZE>
void Array<T, MAX_SIZE>::SetBuffer(T *buf, uint32_t num) {
    ASSERT_THROW((!(num >= MAX_SIZE)), "Array Index out of range");
    Size = num;
    for (uint32_t i = 0; i < Size; ++i) {
        Data[i] = buf[i];
    }
}

#endif //HELLOWORLD_ARRAY_H
