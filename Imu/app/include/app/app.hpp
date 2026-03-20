#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "spi.h"
#include "i2c.h"
// MPU6500 CS pin
#define MPU6500_CS_LOW() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)
#define MPU6500_CS_HIGH() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)

// MPU6500 レジスタアドレス
#define MPU6500_WHO_AM_I 0x75
#define MPU6500_READ_FLAG 0x80

// IST8310 RST pin
#define IST8310_RST_LOW() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET)
#define IST8310_RST_HIGH() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET)

// IST8310 RST pin
#define IST8310_RST_LOW() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET)
#define IST8310_RST_HIGH() HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET)
// IST8310
#define IST8310_I2C &hi2c3
#define IST8310_ADDR 0x0E << 1 // HALはアドレスを1bitシフトする
#define IST8310_WHO_AM_I 0x00
#define IST8310_DATAXL 0x03 // データレジスタ開始アドレス
// データレジスタ
#define MPU6500_ACCEL_XOUT_H 0x3B
#define MPU6500_GYRO_XOUT_H 0x43
    void setup();
    void loop();

#ifdef __cplusplus
}
#endif