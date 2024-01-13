//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_USART_H
#define CODE_USART_H
#include "Platform-STM32.h"
#include "DataStructre/Fifo.hpp"

#ifndef USART_TX_BUFFER_SIZE
    #define USART_TX_BUFFER_SIZE        (1024)                   //USART BUFFER FIFO SIZE
#endif
#ifndef USART_RX_BUFFER_SIZE
    #define USART_RX_BUFFER_SIZE        (256)                    //USART BUFFER FIFO SIZE
#endif

#ifndef USART_DMA_TX_BUFFER_SIZE
    #define USART_DMA_TX_BUFFER_SIZE    (512)                    //USART DMA BUFFER SIZE
#endif
#ifndef USART_DMA_RX_BUFFER_SIZE
    #define USART_DMA_RX_BUFFER_SIZE    (128)                   //USART DMA BUFFER SIZE
#endif

class USART
{
private:
    /* Variables */
    USART_TypeDef* usart;
    DMA_TypeDef* tx_dma;
    DMA_TypeDef* rx_dma;
    uint32_t tx_dma_stream;
    uint32_t rx_dma_stream;
    unsigned char precision = 3; // 小数显示位数
    bool isBusySend = false;
    bool isRxFinished = false;
    bool isUseDma;
    bool isUseIrq;
    Fifo<uint8_t, USART_TX_BUFFER_SIZE> bufferTx;
    Fifo<uint8_t, USART_RX_BUFFER_SIZE> bufferRx;
    uint8_t bufferTxDma[USART_DMA_TX_BUFFER_SIZE];
    uint8_t bufferRxDma[USART_DMA_RX_BUFFER_SIZE];
    uint32_t last_dmarx_size = 0;
    void (USART:: *pSendData) (uint8_t *pbuffer, uint32_t size);
    /* Functions */
    void Set_SendMethod(void);
    void SendData_Polling(uint8_t *pbuffer, uint32_t size);
    void SendData_IT(uint8_t *pbuffer, uint32_t size);
    void SendData_DMA(uint8_t *pbuffer, uint32_t size);
    void DMA_TX_Enable(uint8_t *mem_addr, uint32_t mem_size);
    void DMA_RX_Enable(uint8_t *mem_addr, uint32_t mem_size);
public:
    /* Variables */

    /* Constructor */
    ~USART() = default;

    USART(uint8_t USART_ID = 1, bool IsUseIrq = false);
    USART(uint8_t USART_ID, uint8_t TX_DMA_ID, uint8_t TX_STREAM_ID,
          uint8_t RX_DMA_ID, uint8_t RX_STREAM_ID);
    /* Functions */

    /* Receive */

    bool Receive_Start_DMA(void);
    void Receive_Stop_DMA(void);
    uint32_t Get_ReceivedSize(void);
    bool GetReceivedData(uint8_t * buffer, uint32_t number);
    bool Is_ReceivedFinished(void);
    /* Send */
    bool Transmit_Start_DMA(void);
    void SendData(uint8_t *pbuffer, uint32_t size);
    void ClearSendBuffer(void);

    USART& operator <<(const char* pStr);
    USART& operator <<(const char charactor);
    USART& operator <<(uint32_t val);
    USART& operator <<(int val);
    USART& operator <<(double val);

    void Irq(void);
    void Irq_TX_DMA(void);
    void Irq_RX_DMA(void);
};

#endif //CODE_USART_H
