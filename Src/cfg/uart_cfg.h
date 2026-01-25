#pragma once 
# include "stm32H7xx.h"
struct DMACommon
{
  DMA_Typedef *dma;
  DMA_
};

struct UartCommon
{
    
};



struct cfgStream
{
    DMACommon  dma;
    UartCommon uart;
    cfgBuf  buf;
};

