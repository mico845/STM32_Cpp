//
// Created by Whisky on 12/29/2023.
//

#include "Delay.hpp"
#include "Platform-Operation/Platform-Operation-STM32_RCC.h"

Delay::Delay() {
    SysTick->CTRL|=1<<2;
    fac_us = Platform_RCC_GetSystemClockFreq();
}

Delay::Delay(uint16_t sysclk) {
    SysTick->CTRL|=1<<2;                    //SYSTICK使用内核时钟源,同CPU同频率
    fac_us = sysclk;
}

void Delay::us(uint32_t nUs) {
    uint32_t temp;
    SysTick->LOAD=nUs*fac_us; 				//时间加载
    SysTick->VAL=0x00;        				//清空计数器
    SysTick->CTRL|=1<<0 ;          			//开始倒数
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达
    SysTick->CTRL&=~(1<<0) ;       			//关闭SYSTICK
    SysTick->VAL =0X00;       				//清空计数器
}


//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=2^24 *1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对400M条件下,nms<=41.94ms
void Delay::delay_xms(uint16_t nms) {
    uint32_t temp=0;
    SysTick->LOAD=(uint32_t)nms*fac_us*1000;		//时间加载(SysTick->LOAD为24bit)
    SysTick->VAL =0x00;           			//清空计数器
    SysTick->CTRL|=1<<0 ;          			//开始倒数
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达
    SysTick->CTRL&=~(1<<0) ;       			//关闭SYSTICK
    SysTick->VAL =0X00;     		  		//清空计数器
}

//延时nms
//nms:0~65535
void Delay::ms(uint16_t nMs) {
    uint16_t repeat=nMs / 30;						//这里用30,是考虑到可能有超频应用,
    //比如500Mhz的时候,delay_xms最大只能延时33.5ms左右了
    uint16_t remain=nMs % 30;
    while(repeat)
    {
        delay_xms(30);
        repeat--;
    }
    if(remain)delay_xms(remain);
}


