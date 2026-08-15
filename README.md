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
2. `main.c` dosyanın en üstüne `#include "adxl345.h"` ekle.
3. `main.c` içerisinde sensörü başlat ve verileri oku:

// Örnek Kullanım Kodu:
ADXL345_t adxl345;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  /* ADXL345 Başlatma */
  if (ADXL345_Initialization(&adxl345, &hspi1, GPIOB, GPIO_PIN_6)) {
      // Sensör bulundu ve başarıyla başlatıldı!
  } else {
      // Sensör başlatılamadı
  }

  while (1)
  {
      /* Ham Eksen Verilerini Oku */
      if (adxl345.found) {
          ADXL345_Read_XYZ(&adxl345);
          
          // adxl345.x_raw
          // adxl345.y_raw
          // adxl345.z_raw
      }
      
      HAL_Delay(100);
  }
}

---

## 📄 Lisans
Bu proje açık kaynaklıdır ve [MIT Lisansı](LICENSE) altında sunulmaktadır.
