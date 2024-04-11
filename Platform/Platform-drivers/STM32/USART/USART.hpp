
#ifndef __PLATFORM_STM32_USART_H__
#define __PLATFORM_STM32_USART_H__

#include "platform-drivers/STM32/DMA/DMA.hpp"
#include "platform-drivers/STM32/System/Sys.hpp"
#include "platform-lib/DataStructre/Fifo.hpp"

#ifndef COUT_USARTx
    #define COUT_USART_ID               1               /**< Platform库串口输出流（cout）所使用的串口号 */
    #define _COUT_USART_LINK(AA,BB)     AA##BB
    #define __COUT_USART_LINK(AA,BB)    _COUT_USART_LINK(AA,BB)
    #define COUT_USARTx  __COUT_USART_LINK(USART, COUT_USART_ID)
#endif // !COUT_USARTx

namespace STM32
{

#ifndef USART_TX_BUFFER_SIZE
    #define USART_TX_BUFFER_SIZE        (256)                   /**< USART的发送FIFO缓存区大小 */
#endif
#ifndef USART_RX_BUFFER_SIZE
    #define USART_RX_BUFFER_SIZE        (256)                   /**< USART的接收FIFO缓存区大小 */
#endif

#ifndef USART_DMA_TX_BUFFER_SIZE
    #define USART_DMA_TX_BUFFER_SIZE    (512)                   /**< USART的发送DMA缓存区大小 */
#endif
#ifndef USART_DMA_RX_BUFFER_SIZE
    #define USART_DMA_RX_BUFFER_SIZE    (128)                   /**< USART的接收DMA缓存区大小 */
#endif

#ifndef USART_STREAM_FLOAT_PRECISION
#define USART_STREAM_FLOAT_PRECISION    (3)                   /**< USART发送流的小数显示位数 */
#endif

    /**
    * @brief USART类
    *
    * 用于控制USART通信。
    */
    class USART
    {
    private:
        USART_TypeDef* usart;
        unsigned char precision = USART_STREAM_FLOAT_PRECISION;
        bool isUseDma;
        bool isUseIrq;
        bool isBusySend = false;
        bool isRxFinished = false;
        DMA_TypeDef* tx_dma;
        DMA_TypeDef* rx_dma;
        DMA_Stream tx_dma_stream;
        DMA_Stream rx_dma_stream;
        Fifo<uint8_t, USART_TX_BUFFER_SIZE> bufferTx;
        Fifo<uint8_t, USART_RX_BUFFER_SIZE> bufferRx;
        uint8_t bufferTxDma[USART_DMA_TX_BUFFER_SIZE];
        uint8_t bufferRxDma[USART_DMA_RX_BUFFER_SIZE];
        uint32_t last_dmarx_size = 0;  

        void setSendMethod(void);
        void sendData_Polling(uint8_t *pbuffer, uint32_t size);
        void sendData_IT(uint8_t *pbuffer, uint32_t size);
        void sendData_DMA(uint8_t *pbuffer, uint32_t size);
        void enableTXDMA(uint8_t *mem_addr, uint32_t mem_size);
        void (USART:: *pSendData) (uint8_t *pbuffer, uint32_t size);
    public:
        USART() = default;
        ~USART() = default;
        /**
         * @brief USART类构造函数
         *
         * 无DMA模式。
         *
         * @param USART USART指针
         * @param IsUseIrq 是否使用IRQ，默认为false
         */
        USART(USART_TypeDef* USART, bool IsUseIrq = false);
        /**
         * @brief USART类构造函数
         *
         * DMA模式。
         *
         * @param USART USART指针
         * @param TX_DMA 发送DMA指针
         * @param TX_DMA_Stream 发送DMA流
         * @param RX_DMA 接收DMA指针
         * @param RX_DMA_Stream 接收DMA流
         */
        USART(USART_TypeDef* USART, DMA_TypeDef* TX_DMA, DMA_Stream TX_DMA_Stream, 
                                    DMA_TypeDef* RX_DMA, DMA_Stream RX_DMA_Stream);

        /**
         * @brief 启动接收DMA
         * @return true 成功启动
         * @return false 未开启DMA模式
         */
        bool startReceive_DMA(void);
        /**
         * @brief 停止接收DMA
         */
        void stopReceive_DMA(void);

        /**
        * @brief 获取接收到的数据大小
        * @return uint32_t 接收到的数据大小
        */
        uint32_t getReceivedSize(void);
        /**
         * @brief 获取接收到的数据
         * @param buffer 缓冲区指针
         * @param number 数据个数
         * @return true 成功获取到数据
         * @return false 所需数据超过接收FIFO缓冲区数据数量
         */
        bool getReceivedData(uint8_t * buffer, uint32_t number);

        /**
        * @brief 检查接收是否完成
        * @return true 接收完成
        * @return false 接收尚未完成
        */
        bool isReceivedFinished(void);

        /**
         * @brief <<运算符重载，用于发送字符串
         * @param pStr 字符串指针
         * @return USART& 返回USART引用
         */
        USART& operator <<(const char* pStr);
        /**
         * @brief <<运算符重载，用于发送字符
         * @param charactor 字符
         * @return USART& 返回USART引用
         */
        USART& operator <<(const char charactor);
        /**
         * @brief <<运算符重载，用于发送无符号整数
         * @param val 无符号整数 uint32_t
         * @return USART& 返回USART引用
         */
        USART& operator <<(uint32_t val);
        /**
         * @brief <<运算符重载，用于发送整数
         * @param val 整数 int
         * @return USART& 返回USART引用
         */
        USART& operator <<(int val);
        /**
          * @brief <<运算符重载，用于发送整数
          * @param val 整数 int32_t
          * @return USART& 返回USART引用
          */
        USART& operator <<(int32_t val);
        /**
         * @brief <<运算符重载，用于发送浮点数
         *
         * 修改宏USART_DMA_TX_FLOAT_PRECISION，可以扩展小数显示位数
         *
         * @param val 浮点数
         * @return USART& 返回USART引用
         */
        USART& operator <<(double val);
        /**
         * @brief <<运算符重载，用于发送换行符
         * @param e 换行符
         * @return USART& 返回USART引用
         */
        USART& operator <<(ENDL& e);
        /**
         * @brief USART中断服务函数
         */
        void irq(void);
        /**
         * @brief USART发送的DMA中断服务函数
         */
        void irq_TXDMA(void);
        /**
         * @brief USART接收的DMA中断服务函数
         */
        void irq_RXDMA(void);
    };

    /**
     * @brief Platform库的串口输出流的实例对象，cout
     *
     * 可以使用流操作简化串口输出。
     */
    extern USART cout;
} //namespace STM32



#endif //__PLATFORM_STM32_USART_H__
