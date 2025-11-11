#ifndef __GC9A01_H
#define __GC9A01_H

#include "main.h"

#define GC9A01_WIDTH  240
#define GC9A01_HEIGHT 240

// extern SPI handle
extern SPI_HandleTypeDef hspi1;

// Pin definice
#define GC9A01_DC_Pin     GPIO_PIN_4
#define GC9A01_DC_Port    GPIOA
#define GC9A01_RST_Pin    GPIO_PIN_6
#define GC9A01_RST_Port   GPIOA

// Makra pro řízení pinů
#define GC9A01_DC_Command() HAL_GPIO_WritePin(GC9A01_DC_Port, GC9A01_DC_Pin, GPIO_PIN_RESET)
#define GC9A01_DC_Data()    HAL_GPIO_WritePin(GC9A01_DC_Port, GC9A01_DC_Pin, GPIO_PIN_SET)
#define GC9A01_RST_Low()    HAL_GPIO_WritePin(GC9A01_RST_Port, GC9A01_RST_Pin, GPIO_PIN_RESET)
#define GC9A01_RST_High()   HAL_GPIO_WritePin(GC9A01_RST_Port, GC9A01_RST_Pin, GPIO_PIN_SET)

void GC9A01_Init(void);
void GC9A01_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void GC9A01_FillScreen(uint16_t color);

#endif
