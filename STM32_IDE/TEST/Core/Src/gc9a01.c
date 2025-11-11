#include "gc9a01.h"
#include "stm32f4xx_hal.h"

static void GC9A01_SendCommand(uint8_t cmd)
{
    GC9A01_DC_Command();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
}

static void GC9A01_SendData(uint8_t data)
{
    GC9A01_DC_Data();
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
}

static void GC9A01_SendData16(uint16_t data)
{
    uint8_t temp[2] = {data >> 8, data & 0xFF};
    GC9A01_DC_Data();
    HAL_SPI_Transmit(&hspi1, temp, 2, HAL_MAX_DELAY);
}

void GC9A01_Reset(void)
{
    GC9A01_RST_Low();
    HAL_Delay(20);
    GC9A01_RST_High();
    HAL_Delay(150);
}

void GC9A01_Init(void)
{
    GC9A01_Reset();

    // Init sekvence (ověřená z Laskakit + Waveshare)
    GC9A01_SendCommand(0xEF);
    GC9A01_SendCommand(0xEB); GC9A01_SendData(0x14);
    GC9A01_SendCommand(0xFE); GC9A01_SendCommand(0xEF);
    GC9A01_SendCommand(0xEB); GC9A01_SendData(0x14);
    GC9A01_SendCommand(0x84); GC9A01_SendData(0x40);
    GC9A01_SendCommand(0x85); GC9A01_SendData(0xFF);
    GC9A01_SendCommand(0x86); GC9A01_SendData(0xFF);
    GC9A01_SendCommand(0x87); GC9A01_SendData(0xFF);
    GC9A01_SendCommand(0x88); GC9A01_SendData(0x0A);
    GC9A01_SendCommand(0x89); GC9A01_SendData(0x21);
    GC9A01_SendCommand(0x8A); GC9A01_SendData(0x00);
    GC9A01_SendCommand(0x8B); GC9A01_SendData(0x80);
    GC9A01_SendCommand(0x8C); GC9A01_SendData(0x01);
    GC9A01_SendCommand(0x8D); GC9A01_SendData(0x01);
    GC9A01_SendCommand(0x8E); GC9A01_SendData(0xFF);
    GC9A01_SendCommand(0x8F); GC9A01_SendData(0xFF);

    GC9A01_SendCommand(0xB6); GC9A01_SendData(0x00); GC9A01_SendData(0x20);
    GC9A01_SendCommand(0x36); GC9A01_SendData(0x08);
    GC9A01_SendCommand(0x3A); GC9A01_SendData(0x05); // 16-bit color

    GC9A01_SendCommand(0x21); // Display inversion ON
    GC9A01_SendCommand(0x11); // Sleep out
    HAL_Delay(120);
    GC9A01_SendCommand(0x29); // Display ON
    HAL_Delay(50);
}

void GC9A01_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    GC9A01_SendCommand(0x2A);
    GC9A01_DC_Data();
    uint8_t data_x[4] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
    HAL_SPI_Transmit(&hspi1, data_x, 4, HAL_MAX_DELAY);

    GC9A01_SendCommand(0x2B);
    uint8_t data_y[4] = {y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF};
    HAL_SPI_Transmit(&hspi1, data_y, 4, HAL_MAX_DELAY);

    GC9A01_SendCommand(0x2C);
}

void GC9A01_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= GC9A01_WIDTH) || (y >= GC9A01_HEIGHT)) return;
    GC9A01_SetAddressWindow(x, y, x, y);
    GC9A01_SendData16(color);
}

void GC9A01_FillScreen(uint16_t color)
{
    GC9A01_SetAddressWindow(0, 0, GC9A01_WIDTH - 1, GC9A01_HEIGHT - 1);
    GC9A01_DC_Data();
    uint8_t data[2] = {color >> 8, color & 0xFF};

    for (uint32_t i = 0; i < GC9A01_WIDTH * GC9A01_HEIGHT; i++) {
        HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    }
}
