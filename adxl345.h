/*
 * adxl345.h
 *
 *  Created on: 15 Ağu 2026
 *      Author: Mansu
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "main.h"
#include <stdint.h>   // Sayısal tipler (uint8_t vb.) için
#include <stdbool.h>  // bool tipi için
/* REGISTER ADRESLERİ */
#define ADXL345_DEVID_REG       0x00
#define ADXL345_POWER_CTL_REG   0x2D
#define ADXL345_DATA_FORMAT_REG 0x31

#define ADXL345_DATAX0          0x32
#define ADXL345_DATAX1          0x33
#define ADXL345_DATAY0          0x34
#define ADXL345_DATAY1          0x35
#define ADXL345_DATAZ0          0x36
#define ADXL345_DATAZ1          0x37

#define ADXL345_BW_RATE_REG 0x2C

/* BEKLENEN DEĞERLER */
#define ADXL345_DEVID_VALUE     0xE5

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t          cs_pin;

    int16_t           x_raw;
    int16_t           y_raw;
    int16_t           z_raw;

    bool              found;
} ADXL345_t;

bool ADXL345_Initialization(ADXL345_t *adxl, SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort, uint16_t csPin);
uint8_t ADXL345_Who_Am_I(ADXL345_t *adxl);
bool ADXL345_Read_XYZ(ADXL345_t *adxl);
uint8_t ADXL345_Read_Register(ADXL345_t *adxl, uint8_t regAddr);
bool ADXL345_Write_Register(ADXL345_t *adxl, uint8_t regAddr, uint8_t data);

#endif /* INC_ADXL345_H_ */



