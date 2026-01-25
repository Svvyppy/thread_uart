#pragma once

#include "Pins.h"
#include "stm32h7xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif
    extern UART_HandleTypeDef huart4;
    extern DMA_HandleTypeDef hdma_uart4_rx;
    extern DMA_HandleTypeDef hdma_uart4_tx;

    void HwInit(void);
    void MX_DMA_Init(void);
    void MX_GPIO_Init(void);
    void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle);
    void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle);
    void MX_UART4_Init(void);
    void SystemClock_Config(void);
    void Error_Handler(void);
    void MPU_Config(void);
#ifdef __cplusplus
}
#endif