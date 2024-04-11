#include "platform-drivers/STM32/USART/USART.hpp"
#include "platform-lib/System/callInitFunctions.hpp"

using namespace STM32;

USART::USART(USART_TypeDef* USART, bool IsUseIrq) : usart(USART), isUseDma(false), isUseIrq(IsUseIrq)
{
    setSendMethod();
}

USART::USART(USART_TypeDef* USART, DMA_TypeDef* TX_DMA, DMA_Stream TX_DMA_Stream, 
                 DMA_TypeDef* RX_DMA, DMA_Stream RX_DMA_Stream)
                 :   usart(USART),  tx_dma(TX_DMA), tx_dma_stream(TX_DMA_Stream), 
                                    rx_dma(RX_DMA), rx_dma_stream(RX_DMA_Stream),
                                    isUseDma(true), isUseIrq(true)
{
    setSendMethod();
    //配置串口方向，发送和接收都要
    LL_USART_SetTransferDirection(usart,LL_USART_DIRECTION_TX_RX);
    //设置外设地址->发送
    LL_DMA_SetPeriphAddress(tx_dma, tx_dma_stream, LL_USART_DMA_GetRegAddr(usart, LL_USART_DMA_REG_DATA_TRANSMIT));
    //使能DMA串口发送
    LL_USART_EnableDMAReq_TX(usart);
    //使能DMA发送完成中断
    LL_DMA_EnableIT_TC(tx_dma, tx_dma_stream);
}

void USART::setSendMethod(void) {
    if (isUseIrq)
    {
        if(isUseDma)//使用DMA并且没有忙于发送，则开启发送
            pSendData = &USART::sendData_DMA;
        if(!isUseDma)//使用中断发送
            pSendData = &USART::sendData_IT;
    }
    else  // 轮询发送
        pSendData = &USART::sendData_Polling;
}

void USART::sendData_Polling(uint8_t *pbuffer, uint32_t size) {
    uint32_t index;
    for (index = 0; index < size; index++)
    {
        while (!LL_USART_IsActiveFlag_TXE(usart)); //USART数据发送有两个标志，一个是TXE=1,发送缓冲区空标志：说明可以往数据寄存器写入数据了，但并不代码数据发送完成了
        LL_USART_TransmitData8(usart, pbuffer[index]);
    }
    while (!LL_USART_IsActiveFlag_TC(usart));	//另一个是TC=1发送结束。
}

void USART::sendData_IT(uint8_t *pbuffer, uint32_t size) {
    bufferTx.puts(pbuffer, size);//将需要发送的数据放入缓冲区队列
    if (isBusySend)
        return;

    if(bufferTx.size() > 0)//数据区确实存在数据
    {
        LL_USART_ClearFlag_TC(usart);//清标记，防止第一个数据发送不出去的情况，一定是要先清再打开
        LL_USART_EnableIT_TC(usart);
        isBusySend = true;

        static uint8_t dataToSend = 0;
        bufferTx.get(dataToSend);
        LL_USART_TransmitData8(usart,dataToSend);
    }
}

void USART::sendData_DMA(uint8_t *pbuffer, uint32_t size) {
    bufferTx.puts(pbuffer, size);//将需要发送的数据放入缓冲区队列
    if (isBusySend)
        return;
    //断发送队列里是否还存在数据，如果存在，继续发送，否则关闭DMA
    if(bufferTx.size() > 0)
    {
        isBusySend = true;                               //标记忙于发送
        if(bufferTx.size() <= USART_DMA_TX_BUFFER_SIZE)//队列中的数据长度小于tx_dma缓冲区的最大长度
        {
            uint32_t last_size = bufferTx.size();
            bufferTx.gets(bufferTxDma, last_size);
            enableTXDMA(bufferTxDma, last_size);//将剩下的数据放到tx_dma缓冲区
        }
        else
        {
            bufferTx.gets(bufferTxDma, USART_DMA_TX_BUFFER_SIZE);
            enableTXDMA(bufferTxDma, USART_DMA_TX_BUFFER_SIZE);//将数据放到tx_dma缓冲区
        }
    }
}

void USART::enableTXDMA(uint8_t *mem_addr, uint32_t mem_size)
{
    LL_DMA_SetMemoryAddress(tx_dma, tx_dma_stream, (uint32_t)mem_addr);
    LL_DMA_SetDataLength(tx_dma, tx_dma_stream, mem_size);

    Platform_DMA_ClearFlag_TC(tx_dma, tx_dma_stream);

    LL_DMA_EnableIT_TC(tx_dma, tx_dma_stream);
    LL_DMA_EnableStream(tx_dma, tx_dma_stream);
}

bool USART::startReceive_DMA(void)
{
    if(isUseDma) {
        LL_DMA_SetPeriphAddress(rx_dma, rx_dma_stream, LL_USART_DMA_GetRegAddr(usart, LL_USART_DMA_REG_DATA_RECEIVE));

        LL_DMA_SetMemoryAddress(rx_dma, rx_dma_stream, (uint32_t)bufferRxDma);
        LL_DMA_SetDataLength(rx_dma, rx_dma_stream, USART_DMA_RX_BUFFER_SIZE);
        LL_DMA_EnableStream(rx_dma, rx_dma_stream);

        LL_DMA_EnableIT_HT(rx_dma, rx_dma_stream);
        LL_DMA_EnableIT_TC(rx_dma, rx_dma_stream);
        LL_USART_EnableDMAReq_RX(usart);
        LL_USART_EnableIT_IDLE(usart);
        return true;
    }
    return false;
}

void USART::stopReceive_DMA(void)
{
    LL_USART_DisableDMAReq_RX(usart);
    LL_USART_DisableIT_IDLE(usart);
}

bool USART::isReceivedFinished(void) {
    if (isRxFinished)
    {
        isRxFinished = false;
        return true;
    }
    else
        return false;
}

uint32_t USART::getReceivedSize(void) {
    return bufferRx.size();
}

bool USART::getReceivedData(uint8_t *buffer, uint32_t number)
{
    if(bufferRx.size() < number)//没有足够长的数据
        return false;
    else
    {
        bufferRx.gets(buffer, number);//数据出队
        return true;
    }
}

USART &USART::operator<<(const char *pStr) {
    unsigned int length=0;
    for(int i = 0; pStr[i] != '\0'; ++i)
    {
        ++length;
    }
    (this->*pSendData)((uint8_t *)pStr,length);
    return *this;
}

USART &USART::operator<<(const char charactor) {
    (this->*pSendData)((uint8_t *)&charactor,1);
    return *this;
}

USART &USART::operator<<(uint32_t val) {
    uint8_t len = 0,data[10];
    do
    {
        len++;
        data[10-len] = val%10 + '0';
        val = val/10;
    }while(val);
    (this->*pSendData)(data+10-len,len);
    return *this;
}

USART &USART::operator<<(int val) {
    uint8_t sign = 0, len = 0,data[10];
    if(val<0)
    {
        sign = 1;
        val = -val;
    }
    do
    {
        len++;
        data[10-len] = val%10 + '0';
        val = val/10;
    }while(val);
    if(sign==1)
        data[10-(++len)] = '-';
    (this->*pSendData)(data+10-len,len);
    return *this;
}

USART &USART::operator<<(int32_t val) {
    uint8_t sign = 0, len = 0,data[15];
    if(val<0)
    {
        sign = 1;
        val = -val;
    }
    do
    {
        len++;
        data[15-len] = val%10 + '0';
        val = val/10;
    }while(val);
    if(sign==1)
        data[15-(++len)] = '-';
    (this->*pSendData)(data+15-len,len);
    return *this;
}

USART &USART::operator<<(double val) {
    uint8_t sign = 0, len = 0,data[20];
    if(val<0)
    {
        sign = 1;
        val = -val;
    }
    uint8_t prec = precision;
    while(prec--)
        val *= 10;
    uint32_t t = val;
    do
    {
        if(++len==precision+1) data[20-len] = '.';
        else
        {
            data[20-len] = t%10 + '0';
            t = t/10;
        }
    }while(t || len<precision+2);
    if(sign==1)
        data[20-(++len)] = '-';
    (this->*pSendData)(data+20-len,len);
    return *this;
}

USART &USART::operator<<(ENDL& e)
{
    if (isUseIrq)
        bufferTx.clear();
    
    uint8_t str[2] = {'\r', '\n'};
    (this->*pSendData)(str, 2);
    return *this;
}

void USART::irq_TXDMA(void){
    if (LL_DMA_IsEnabledIT_TC(tx_dma, tx_dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(tx_dma, tx_dma_stream))
    {
        //清除Stream TC中断
        Platform_DMA_ClearFlag_TC(tx_dma, tx_dma_stream);
        //关闭DMA Stream
        LL_DMA_DisableStream(tx_dma, tx_dma_stream);

        //发送完成，判断发送队列里是否还存在数据，如果存在，继续发送，否则关闭DMA
        if(bufferTx.size() > 0)
        {
            isBusySend = true;                               //标记忙于发送
            if(bufferTx.size() <= USART_DMA_TX_BUFFER_SIZE)//队列中的数据长度小于tx_dma缓冲区的最大长度
            {
                uint32_t last_size = bufferTx.size();
                bufferTx.gets(bufferTxDma, last_size);
                enableTXDMA(bufferTxDma, last_size);//将剩下的数据放到tx_dma缓冲区
            }
            else
            {
                bufferTx.gets(bufferTxDma, USART_DMA_TX_BUFFER_SIZE);
                enableTXDMA(bufferTxDma, USART_DMA_TX_BUFFER_SIZE);//将数据放到tx_dma缓冲区
            }
        }
        else
            isBusySend = false;               //将忙标志
    }
}

void USART::irq(void) {
    // 串口发送完成中断
    if (LL_USART_IsEnabledIT_TC(usart) && LL_USART_IsActiveFlag_TC(usart))
    {
        static uint8_t dataToSend=0;
        if(bufferTx.size() > 0)
        {
            bufferTx.get(dataToSend);
            LL_USART_TransmitData8(usart,dataToSend);
        }
        if(bufferTx.size() == 0)//发送完毕，关发送中断
        {
            LL_USART_DisableIT_TC(usart);
            isBusySend = false;
        }

        LL_USART_ClearFlag_TC(usart);
    }
    // 串口空闲中断处理
    if (LL_USART_IsEnabledIT_IDLE(usart) && LL_USART_IsActiveFlag_IDLE(usart))
    {
        uint32_t recv_total_size;
        uint32_t recv_size;
        // RX_DMA缓冲区长度 - dmarx_buf_remain_size
        recv_total_size = USART_DMA_RX_BUFFER_SIZE - LL_DMA_GetDataLength(rx_dma, rx_dma_stream);
        recv_size = recv_total_size - last_dmarx_size;
        SCB_InvalidateDCache_by_Addr((uint32_t *)&bufferRxDma[last_dmarx_size], recv_size);
        bufferRx.puts((uint8_t *) (&bufferRxDma[last_dmarx_size]), recv_size);
        last_dmarx_size = recv_total_size;

        isRxFinished = true;

        LL_USART_ClearFlag_IDLE(usart);
    }
}

void USART::irq_RXDMA(void) {
    if (LL_DMA_IsEnabledIT_HT(rx_dma, rx_dma_stream) && PLATFORM_DMA_IsActiveFlag_HT(rx_dma, rx_dma_stream))
    {
        uint32_t recv_total_size;
        uint32_t recv_size;
        recv_total_size = USART_DMA_RX_BUFFER_SIZE - LL_DMA_GetDataLength(rx_dma, rx_dma_stream);
        recv_size = recv_total_size - last_dmarx_size;
        SCB_InvalidateDCache_by_Addr((uint32_t *)&bufferRxDma[last_dmarx_size], recv_size);
        bufferRx.puts((uint8_t *) (&bufferRxDma[last_dmarx_size]), recv_size);

        last_dmarx_size = recv_total_size;

        Platform_DMA_ClearFlag_HT(rx_dma, rx_dma_stream);
    }

    if (LL_DMA_IsEnabledIT_TC(rx_dma, rx_dma_stream) && PLATFORM_DMA_IsActiveFlag_TC(rx_dma, rx_dma_stream))
    {
        uint32_t recv_size;
        recv_size = USART_DMA_RX_BUFFER_SIZE - last_dmarx_size;
        SCB_InvalidateDCache_by_Addr((uint32_t *)&bufferRxDma[last_dmarx_size], recv_size);
        bufferRx.puts((uint8_t *) (&bufferRxDma[last_dmarx_size]), recv_size);
        last_dmarx_size = 0;

        Platform_DMA_ClearFlag_TC(rx_dma, rx_dma_stream);
    }
}

namespace STM32{
  USART cout; 
}

INIT_FUNCTION(COUT_INIT)
{
    STM32::cout = USART(COUT_USARTx); 
}
