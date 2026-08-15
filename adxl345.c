/*
 * adxl345.c
 *
 *  Created on: 15 Ağu 2026
 *      Author: Mansu
 */



#include "adxl345.h"

#include <stdint.h>
#include <stdbool.h>

static void ADXL345_CS_Enable(ADXL345_t *adxl) {
    HAL_GPIO_WritePin(adxl->cs_port, adxl->cs_pin, GPIO_PIN_RESET);
}

static void ADXL345_CS_Disable(ADXL345_t *adxl) {
    HAL_GPIO_WritePin(adxl->cs_port, adxl->cs_pin, GPIO_PIN_SET);
}

bool ADXL345_Initialization(ADXL345_t *adxl, SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort, uint16_t csPin) {
    adxl->hspi = hspi;
    adxl->cs_port = csPort;
    adxl->cs_pin = csPin;
    adxl->x_raw = 0;
    adxl->y_raw = 0;
    adxl->z_raw = 0;

    // Ölçüm moduna al (Measure bit = 1)
        if (!ADXL345_Write_Register(adxl, ADXL345_POWER_CTL_REG, 0x08)) return false;

        // Veri güncelleme hızını 100 Hz yap (0x0A)
        if (!ADXL345_Write_Register(adxl, ADXL345_BW_RATE_REG, 0x0A)) return false; // YENİ EKLENEN SATIR

        // +-2g hassasiyet (Varsayılan)
        if (!ADXL345_Write_Register(adxl, ADXL345_DATA_FORMAT_REG, 0x00)) return false;

    if (ADXL345_Who_Am_I(adxl) == ADXL345_DEVID_VALUE) {
        adxl->found = true;

        // Ölçüm moduna al (Measure bit = 1)
        if (!ADXL345_Write_Register(adxl, ADXL345_POWER_CTL_REG, 0x08)) return false;

        // +-2g hassasiyet (Varsayılan)
        if (!ADXL345_Write_Register(adxl, ADXL345_DATA_FORMAT_REG, 0x00)) return false;
    } else {
        adxl->found = false;
        return false;
    }
    return true;
}

uint8_t ADXL345_Who_Am_I(ADXL345_t *adxl) {
    return ADXL345_Read_Register(adxl, ADXL345_DEVID_REG);
}

bool ADXL345_Read_XYZ(ADXL345_t *adxl) {
    uint8_t txData[7] = {0};
    uint8_t rxData[7] = {0};

    // 0x80: Okuma biti, 0x40: Çoklu Okuma (Multi-byte) biti
    txData[0] = ADXL345_DATAX0 | 0x80 | 0x40;

    ADXL345_CS_Enable(adxl);

    // 1 adres + 6 veri = toplam 7 byte'ı CS kapatmadan tek seferde okuyoruz
    HAL_SPI_TransmitReceive(adxl->hspi, txData, rxData, 7, 100);

    ADXL345_CS_Disable(adxl);

    // rxData[0] adres byte'ının cevabıdır, asıl eksen verileri 1. indisten başlar
    adxl->x_raw = (int16_t)((rxData[2] << 8) | rxData[1]);
    adxl->y_raw = (int16_t)((rxData[4] << 8) | rxData[3]);
    adxl->z_raw = (int16_t)((rxData[6] << 8) | rxData[5]);

    return true;
}

uint8_t ADXL345_Read_Register(ADXL345_t *adxl, uint8_t regAddr) {
    uint8_t txData[2] = {regAddr | 0x80, 0x00}; // 0. byte adres (Read biti 1), 1. byte boş (dummy clock için)
    uint8_t rxData[2] = {0, 0};

    ADXL345_CS_Enable(adxl);
    // Aynı anda hem adresi gönderir hem de cevabı okur
    HAL_SPI_TransmitReceive(adxl->hspi, txData, rxData, 2, 100);
    ADXL345_CS_Disable(adxl);

    return rxData[1]; // Sensörden gelen asıl veri 2. byte'tadır
}

bool ADXL345_Write_Register(ADXL345_t *adxl, uint8_t regAddr, uint8_t data) {
    uint8_t txData[2];
    txData[0] = regAddr & 0x7F; // Write biti (7. bit 0 olacak)
    txData[1] = data;

    ADXL345_CS_Enable(adxl);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(adxl->hspi, txData, 2, 100);
    ADXL345_CS_Disable(adxl);

    return (status == HAL_OK);
}
