# STM32F4-ADXL345-SPI-Driver
Hardware SPI driver for ADXL345 accelerometer using STM32 HAL library.


# STM32F4 ADXL345 SPI Sürücüsü 🚀

Bu proje, STM32 mikrodenetleyicileri için HAL kütüphanesi kullanılarak yazılmış, modüler ve güvenilir bir **ADXL345 3 Eksen İvmeölçer** Donanımsal SPI sürücüsüdür.

---

## 📌 Özellikler
- **Arayüz:** Donanımsal SPI (Full-Duplex Master)
- **Veri Transferi:** XYZ eksenlerini hızlı ve senkronize okumak için çoklu bayt (Multi-byte) okuma desteği
- **Cihaz Doğrulama:** `WHO_AM_I` kaydedicisi (`0xE5`) üzerinden otomatik sensör varlık kontrolü
- **Yapılandırılabilir:** Ayarlanabilir veri güncelleme hızı (Varsayılan: 100 Hz) ve ölçüm aralığı

---

## 🛠️ Donanım Bağlantısı (Pinout)

STM32CubeMX üzerindeki SPI konfigürasyonuna göre bağlantılar şu şekildedir:

| ADXL345 Pin | STM32 Pin (Örn: STM32F401RE) | İşlev |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Güç Beslemesi |
| **GND** | GND | Toprak |
| **CS** | PB6 | GPIO Output (Yazılımsal Chip Select) |
| **SDO** | PA6 | SPI1_MISO |
| **SDA** | PA7 | SPI1_MOSI |
| **SCL** | PA5 | SPI1_SCK |

---

## ⚙️ STM32CubeMX Konfigürasyonu

CubeMX üzerindeki SPI ayarlarını şu parametrelere göre yapmalısın:

* **Mode:** Full-Duplex Master
* **Frame Format:** Motorola
* **Data Size:** 8 Bits
* **First Bit:** MSB First
* **Clock Polarity (CPOL):** **High**
* **Clock Phase (CPHA):** **2 Edge** (SPI Mode 3)
* **NSS Signal Type:** Software

---

## 🚀 Kullanım Adımları

1. `adxl345.h` dosyasını `Core/Inc` klasörüne, `adxl345.c` dosyasını ise `Core/Src` klasörüne ekle.
2. `main.c` dosyanın en üstüne kütüphaneyi dahil et:

```c
#include "adxl345.h"
