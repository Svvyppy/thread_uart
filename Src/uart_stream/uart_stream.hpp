#pragma once
#include "hydrolib_ring_queue.h"
#include <cstdint>
#include <cstring>
#include "Hwinit.hpp"

namespace hydrolib::stream
{

class UartStream
{
public:
    UartStream(UART_HandleTypeDef *huart) : huart_(huart)
    {
        hydrolib_RingQueue_Init(&rxQueue_, rxQueueBuf_, sizeof(rxQueueBuf_));
        hydrolib_RingQueue_Init(&txQueue_, txQueueBuf_, sizeof(txQueueBuf_));
    }

    int read(void *dest, unsigned length)
    {
        uint16_t len = hydrolib_RingQueue_GetLength(&rxQueue_);
        if (length > len)
        {
            hydrolib_RingQueue_Pull(&rxQueue_, dest, length);
            return len;
        }
        else
        {
            hydrolib_RingQueue_Pull(&rxQueue_, dest, length);
            return length;
        }
    }

    int write(const void *src, unsigned length)
    {
        if (uint16_t len = hydrolib_RingQueue_GetCapacity(&txQueue_) -
                           hydrolib_RingQueue_GetLength(&txQueue_);
            len < length)
        {
            hydrolib_RingQueue_Push(&txQueue_, src, len);
            return len;
        }
        else
        {
            hydrolib_RingQueue_Push(&txQueue_, src, length);
            return length;
        }
    }

    void poll()
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart_, rxBuf_, rxSize_);
        if (hydrolib_RingQueue_GetLength(&txQueue_) != 0)
        {
            hydrolib_RingQueue_Pull(&txQueue_, txBuf_, sizeof(txBuf_));
            HAL_UART_Transmit_DMA(huart_, txBuf_, sizeof(txBuf_));
        }
    }

    void recieveFrame(uint16_t size)
    {
        hydrolib_RingQueue_Push(&rxQueue_, rxBuf_, size);
    }

    UART_HandleTypeDef *handle() { return huart_; }

private:
    UART_HandleTypeDef *huart_;
    uint8_t rxQueueBuf_[512] = {};
    uint8_t rxBuf_[64] = {};
    uint16_t rxSize_ = 64;
    hydrolib_RingQueue rxQueue_;

    uint8_t txQueueBuf_[512] = {};
    uint8_t txBuf_[64] = {};
    uint16_t txSize_ = 64;
    hydrolib_RingQueue txQueue_;
};
template <typename T>
inline int write(T &stream, const void *src, unsigned len)
{
    return stream.write(src, len);
}

template <typename T>
inline int read(T &stream, void *dst, unsigned len)
{
    return stream.read(dst, len);
}

} // namespace hydrolib::stream
