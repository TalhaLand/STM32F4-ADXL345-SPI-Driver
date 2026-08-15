# STM32F4 ADXL345 SPI Driver 🚀
# 🚀 Quick Start — Driver'ı Hemen Kullan

ADXL345 driver'ını STM32 projenizde kullanmak için aşağıdaki **4 adım** yeterlidir.

### 1️⃣ Driver dosyalarını projeye ekle

Dosyaları STM32CubeIDE projenize ekleyin:

```text
adxl345.h → Core/Inc/
adxl345.c → Core/Src/
```

`main.c` içerisine:

```c
#include "adxl345.h"
```

ekleyin.

---

### 2️⃣ ADXL345'i STM32'ye bağla

Örnek bağlantı:

| ADXL345 | STM32F401RE     |
| ------- | --------------- |
| VCC     | 3.3V            |
| GND     | GND             |
| CS      | PB6             |
| SDO     | PA6 (SPI1_MISO) |
| SDA     | PA7 (SPI1_MOSI) |
| SCL     | PA5 (SPI1_SCK)  |

SPI, **Mode 3** olarak yapılandırılmalıdır.

---

### 3️⃣ `main.c` içerisinde driver nesnesini oluştur

Global alana:

```c
ADXL345_t adxl345;
```

ekleyin.

Daha sonra `main()` içerisinde SPI ve GPIO initialization işlemlerinden sonra:

```c
ADXL345_Initialization(
    &adxl345,
    &hspi1,
    GPIOB,
    GPIO_PIN_6
);
```

çağrısını yapın.

Bu çağrı:

* Driver'ı başlatır.
* SPI1'i ADXL345 için kullanır.
* PB6'yı CS pini olarak ayarlar.
* ADXL345'in `WHO_AM_I` register'ını kontrol eder.
* Sensörü ölçüm moduna geçirir.
* Veri hızı ve ölçüm aralığını yapılandırır.

---

### 4️⃣ Sensörden XYZ verilerini oku

`while(1)` içerisinde:

```c
while (1)
{
    if (adxl345.found)
    {
        ADXL345_Read_XYZ(&adxl345);

        // Ham ivme değerleri
        int16_t x = adxl345.x_raw;
        int16_t y = adxl345.y_raw;
        int16_t z = adxl345.z_raw;
    }

    HAL_Delay(100);
}
```

artık:

```c
adxl345.x_raw
adxl345.y_raw
adxl345.z_raw
```

üzerinden X, Y ve Z eksenlerinin ham ivme verilerine ulaşabilirsiniz.

---

## 🧩 Minimum `main.c` Örneği

Driver'ın en basit kullanım şekli:

```c
#include "main.h"
#include "adxl345.h"

ADXL345_t adxl345;

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();

    /* ADXL345 Driver'ını başlat */
    ADXL345_Initialization(
        &adxl345,
        &hspi1,
        GPIOB,
        GPIO_PIN_6
    );

    while (1)
    {
        if (adxl345.found)
        {
            /* X, Y ve Z verilerini oku */
            ADXL345_Read_XYZ(&adxl345);

            /*
             * Ham veriler:
             *
             * adxl345.x_raw
             * adxl345.y_raw
             * adxl345.z_raw
             */
        }

        HAL_Delay(100);
    }
}
```

> **Kısaca:** `ADXL345_Initialization()` driver'ı aktif eder, `ADXL345_Read_XYZ()` ise sensörden yeni X/Y/Z verilerini okur.

---

## ⚙️ CubeMX'te Gerekli Ayarlar

Driver'ı kullanmadan önce CubeMX'te:

```text
SPI1
├── Mode        → Full-Duplex Master
├── Data Size   → 8 Bits
├── First Bit   → MSB First
├── Clock Polarity → High
├── Clock Phase → 2 Edge
└── NSS         → Software
```

CS pini:

```text
PB6 → GPIO Output
```

olarak ayarlanmalıdır.

**SPI Mode 3** kullanılmaktadır.

---

# 📌 Bundan Sonra

Yukarıdaki işlemleri yaptıktan sonra driver çalışmaya hazırdır.

README'nin devamındaki bölümlerde:

* ADXL345 register yapısı
* SPI Read/Write işlemlerinin nasıl çalıştığı
* `WHO_AM_I` kontrolü
* `POWER_CTL`, `BW_RATE` ve `DATA_FORMAT` ayarları
* XYZ verilerinin nasıl okunduğu
* Multi-byte SPI okuma
* Ham verilerin nasıl birleştirildiği
* Driver'ın farklı SPI ve CS pinleriyle nasıl kullanılacağı

detaylı olarak açıklanmaktadır.

Böylece README hem **"hemen nasıl çalıştırırım?"** sorusunu cevaplar hem de ileride kodu tekrar açtığında driver'ın çalışma mantığını hatırlamanı sağlar. 🚀


STM32 mikrodenetleyiciler için **HAL kütüphanesi kullanılarak hazırlanmış, donanımsal SPI tabanlı ADXL345 3 eksenli ivmeölçer sürücüsüdür.**

Bu driver'ın temel amacı, ADXL345 ile SPI haberleşmesinin register seviyesinde nasıl gerçekleştirildiğini anlaşılır ve tekrar kullanılabilir bir yapıda göstermektir.

Driver içerisinde:

* SPI üzerinden register okuma
* SPI üzerinden register yazma
* `WHO_AM_I` register'ı ile sensör doğrulama
* ADXL345'i ölçüm moduna alma
* Veri hızını yapılandırma
* Ölçüm aralığını yapılandırma
* X, Y ve Z eksenlerini tek SPI işleminde okuma
* Yazılımsal Chip Select (CS) kontrolü

işlemleri gerçekleştirilir.

---

## 📌 Kullanılan Donanım

| Bileşen       | Açıklama     |
| ------------- | ------------ |
| MCU           | STM32F4      |
| Örnek MCU     | STM32F401RE  |
| Sensör        | ADXL345      |
| Haberleşme    | Hardware SPI |
| SPI Mode      | Mode 3       |
| Kütüphane     | STM32 HAL    |
| IDE           | STM32CubeIDE |
| Konfigürasyon | STM32CubeMX  |

> Bu repository'deki pin örneği STM32F401RE ve SPI1 üzerinden hazırlanmıştır. Farklı bir STM32F4 kullanıyorsanız SPI pinlerini kendi MCU'nuzun pinout'una göre değiştirebilirsiniz.

---

# 🔌 Donanım Bağlantısı

ADXL345'in SPI pinlerini STM32'ye aşağıdaki şekilde bağlayabilirsiniz:

| ADXL345 | STM32F401RE | Açıklama    |
| ------- | ----------- | ----------- |
| VCC     | 3.3V        | Besleme     |
| GND     | GND         | Toprak      |
| CS      | PB6         | Chip Select |
| SDO     | PA6         | SPI1 MISO   |
| SDA     | PA7         | SPI1 MOSI   |
| SCL     | PA5         | SPI1 SCK    |

### SPI bağlantısının mantığı

ADXL345 tarafındaki pin isimleri ilk bakışta biraz kafa karıştırabilir:

* **SCL → SPI Clock (SCK)**
* **SDA → SPI MOSI**
* **SDO → SPI MISO**
* **CS → Chip Select**

Yani SPI haberleşmesi şu şekilde gerçekleşir:

```text
STM32                         ADXL345

PA5  (SPI1_SCK)   --------->  SCL
PA7  (SPI1_MOSI)  --------->  SDA
PA6  (SPI1_MISO)  <---------  SDO
PB6  (GPIO)       --------->  CS

3.3V              --------->  VCC
GND               --------->  GND
```

### ⚠️ CS neden normal GPIO?

Bu driver'da SPI'nin NSS/CS kontrolü donanıma bırakılmamıştır.

CubeMX'te:

```text
NSS Signal Type → Software
```

olarak ayarlanır.

CS pini normal bir GPIO olarak tanımlanır ve driver içerisinde:

```c
HAL_GPIO_WritePin(adxl->cs_port, adxl->cs_pin, GPIO_PIN_RESET);
```

ile aktif edilir.

İşlem tamamlandığında:

```c
HAL_GPIO_WritePin(adxl->cs_port, adxl->cs_pin, GPIO_PIN_SET);
```

ile tekrar pasif yapılır.

Yani:

```text
CS = LOW  → Sensör seçili
CS = HIGH → Sensör seçili değil
```

---

# ⚙️ STM32CubeMX Ayarları

Öncelikle `SPI1` aktif edilmelidir.

### SPI Mode

```text
Mode → Full-Duplex Master
```

### SPI parametreleri

```text
Frame Format       → Motorola
Data Size          → 8 Bits
First Bit          → MSB First
Clock Polarity     → High
Clock Phase        → 2 Edge
NSS                 → Software
```

Bu ayarlar ADXL345 için:

```text
SPI Mode 3
CPOL = 1
CPHA = 1
```

anlamına gelir.

### GPIO

CS pini:

```text
PB6 → GPIO_Output
```

olarak ayarlanmalıdır.

CS'in başlangıçta HIGH olması tercih edilir:

```text
PB6 = HIGH
```

Böylece STM32 başlatılırken ADXL345 yanlışlıkla seçilmez.

---

# 📁 Driver Dosyaları

Driver iki temel dosyadan oluşur:

```text
adxl345.h
adxl345.c
```

### `adxl345.h`

Header dosyasında:

* Register adresleri
* Beklenen cihaz ID'si
* `ADXL345_t` struct'ı
* Driver fonksiyonlarının prototipleri

bulunur.

### `adxl345.c`

Asıl SPI haberleşmesi burada gerçekleştirilir.

Register okuma/yazma, CS kontrolü, sensör başlatma ve XYZ veri okuma işlemleri bu dosyadadır.

---

# 📦 Driver Yapısı

Driver'ın temel veri yapısı:

```c
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t          cs_pin;

    int16_t            x_raw;
    int16_t            y_raw;
    int16_t            z_raw;

    bool               found;
} ADXL345_t;
```

Bu yapı driver'ın sensörle ilgili bilgilerini tek yerde tutar.

### SPI handle

```c
SPI_HandleTypeDef *hspi;
```

STM32 HAL tarafından oluşturulan SPI nesnesinin adresidir.

Örneğin:

```c
&hspi1
```

verilebilir.

### CS port ve pin

```c
GPIO_TypeDef *cs_port;
uint16_t cs_pin;
```

CS pininin hangi GPIO portunda ve hangi pin üzerinde olduğunu belirtir.

Örneğin:

```c
GPIOB
GPIO_PIN_6
```

### XYZ verileri

```c
int16_t x_raw;
int16_t y_raw;
int16_t z_raw;
```

Sensörden okunan ham X, Y ve Z eksen verileridir.

### found

```c
bool found;
```

Sensörün doğru şekilde bulunduğunu belirtir.

```text
true  → ADXL345 bulundu
false → ADXL345 bulunamadı
```

---

# 🚀 Driver'ı Projeye Ekleme

STM32CubeIDE projenize:

```text
adxl345.h
```

dosyasını:

```text
Core/Inc
```

klasörüne,

```text
adxl345.c
```

dosyasını:

```text
Core/Src
```

klasörüne ekleyebilirsiniz.

Daha sonra `main.c` içerisine:

```c
#include "adxl345.h"
```

eklenmelidir.

---

# 🧩 Driver'ı Başlatma

Öncelikle bir `ADXL345_t` değişkeni oluşturulur:

```c
ADXL345_t adxl345;
```

Daha sonra initialization fonksiyonu çağrılır:

```c
ADXL345_Initialization(
    &adxl345,
    &hspi1,
    GPIOB,
    GPIO_PIN_6
);
```

Buradaki parametrelerin anlamı:

```text
&adxl345
    ↓
Driver nesnesi

&hspi1
    ↓
Kullanılacak SPI peripheral'ı

GPIOB
    ↓
CS portu

GPIO_PIN_6
    ↓
CS pini
```

Yani:

```c
ADXL345_Initialization(&adxl345, &hspi1, GPIOB, GPIO_PIN_6);
```

şu anlama gelir:

> "ADXL345 sensörünü SPI1 kullanarak ve PB6'yı Chip Select olarak kullanarak başlat."

---

# 🔍 Initialization İçerisinde Ne Oluyor?

`ADXL345_Initialization()` fonksiyonu driver'ın başlangıç noktasıdır.

Fonksiyon içerisinde önce driver'a hangi SPI ve CS pininin kullanılacağı aktarılır:

```c
adxl->hspi = hspi;
adxl->cs_port = csPort;
adxl->cs_pin = csPin;
```

Daha sonra XYZ değerleri sıfırlanır:

```c
adxl->x_raw = 0;
adxl->y_raw = 0;
adxl->z_raw = 0;
```

Ardından sensör yapılandırılır.

---

# ⚙️ ADXL345 Register Yapılandırması

## 1. POWER_CTL — `0x2D`

Driver:

```c
ADXL345_Write_Register(adxl, ADXL345_POWER_CTL_REG, 0x08);
```

işlemini gerçekleştirir.

`0x08` içerisindeki Measure biti `1` yapılarak ADXL345 ölçüm moduna alınır.

```text
POWER_CTL
Address → 0x2D

0x08
 ↓
Measure = 1
```

Yani sensör artık ivme ölçmeye başlar.

---

## 2. BW_RATE — `0x2C`

Driver:

```c
ADXL345_Write_Register(adxl, ADXL345_BW_RATE_REG, 0x0A);
```

işlemini gerçekleştirir.

`0x0A`, ADXL345'in veri hızını:

```text
100 Hz
```

olarak ayarlar.

Bu nedenle driver'ın varsayılan veri güncelleme hızı 100 Hz'dir.

---

## 3. DATA_FORMAT — `0x31`

Driver:

```c
ADXL345_Write_Register(adxl, ADXL345_DATA_FORMAT_REG, 0x00);
```

işlemini gerçekleştirir.

Bu ayarla varsayılan:

```text
±2g
```

ölçüm aralığı kullanılır.

---

# 🆔 Sensörün Doğrulanması

Driver, sensörün gerçekten ADXL345 olup olmadığını kontrol etmek için:

```text
DEVID Register
Address → 0x00
```

register'ını okur.

ADXL345 için beklenen değer:

```c
#define ADXL345_DEVID_VALUE 0xE5
```

Sensörden:

```text
0xE5
```

gelirse:

```c
adxl->found = true;
```

yapılır.

Aksi durumda:

```c
adxl->found = false;
```

olur ve initialization başarısız kabul edilir.

Bu kontrol sayesinde SPI bağlantısında problem olduğunda sensörün bulunup bulunmadığını anlayabiliriz.

---

# 📖 Register Okuma Mantığı

Driver'ın temel SPI işlemlerinden biri:

```c
uint8_t ADXL345_Read_Register(
    ADXL345_t *adxl,
    uint8_t regAddr
);
```

fonksiyonudur.

ADXL345 SPI protokolünde register okumak için adresin 7. biti `1` yapılır:

```c
regAddr | 0x80
```

Örneğin:

```text
Register = 0x00

0x00 | 0x80
     ↓
0x80
```

Daha sonra sensörden veriyi almak için bir dummy byte gönderilir.

Bu yüzden:

```c
uint8_t txData[2] = {
    regAddr | 0x80,
    0x00
};
```

şeklinde iki byte gönderilir.

SPI Full-Duplex olduğu için aynı anda veri alınır.

Asıl sensör verisi:

```c
rxData[1]
```

içerisindedir.

---

# ✍️ Register Yazma Mantığı

Register yazmak için:

```c
bool ADXL345_Write_Register(
    ADXL345_t *adxl,
    uint8_t regAddr,
    uint8_t data
);
```

kullanılır.

Yazma işleminde register adresinin 7. biti `0` olmalıdır:

```c
regAddr & 0x7F
```

Ardından:

```text
1. byte → Register adresi
2. byte → Yazılacak veri
```

gönderilir.

Örneğin:

```c
ADXL345_Write_Register(
    adxl,
    ADXL345_POWER_CTL_REG,
    0x08
);
```

şu işlemi yapar:

```text
Register: 0x2D
Data:     0x08
```

---

# 📐 X, Y, Z Verilerinin Okunması

XYZ verilerini okumak için:

```c
ADXL345_Read_XYZ(&adxl345);
```

kullanılır.

ADXL345'in veri register'ları:

```text
DATAX0 → 0x32
DATAX1 → 0x33

DATAY0 → 0x34
DATAY1 → 0x35

DATAZ0 → 0x36
DATAZ1 → 0x37
```

şeklindedir.

Her eksen **2 byte** kullanır.

Toplam:

```text
X → 2 byte
Y → 2 byte
Z → 2 byte

Toplam → 6 byte
```

okunur.

---

# 🔄 Multi-Byte SPI Okuma

Driver XYZ verilerini tek tek okumak yerine tek SPI işleminde okur.

Başlangıç register'ı:

```c
ADXL345_DATAX0
```

yani:

```text
0x32
```

Adresine:

```c
0x80
```

okuma biti ve:

```c
0x40
```

multi-byte biti eklenir:

```c
txData[0] = ADXL345_DATAX0 | 0x80 | 0x40;
```

Sonuç:

```text
0x32
OR 0x80
OR 0x40
------
0xF2
```

olur.

Bu byte sensöre:

> "0x32 register'ından başlayarak oku ve devam eden register'ları da getir."

anlamına gelir.

Ardından toplam 7 byte transfer edilir:

```text
1 byte → Register adresi
6 byte → X, Y, Z verileri
```

Bu işlem sırasında CS LOW tutulur.

```text
CS LOW
   ↓
7 byte SPI transfer
   ↓
CS HIGH
```

Bu nedenle driver:

```c
ADXL345_CS_Enable(adxl);

HAL_SPI_TransmitReceive(
    adxl->hspi,
    txData,
    rxData,
    7,
    100
);

ADXL345_CS_Disable(adxl);
```

şeklinde çalışır.

---

# 🧮 Ham XYZ Verilerinin Birleştirilmesi

Sensörden her eksen için iki byte gelir.

Örneğin X:

```text
X0 → Low Byte
X1 → High Byte
```

Driver bunları:

```c
adxl->x_raw =
    (int16_t)((rxData[2] << 8) | rxData[1]);
```

şeklinde birleştirir.

Aynı işlem Y ve Z için de yapılır:

```c
adxl->y_raw =
    (int16_t)((rxData[4] << 8) | rxData[3]);

adxl->z_raw =
    (int16_t)((rxData[6] << 8) | rxData[5]);
```

Sonuç olarak:

```text
adxl345.x_raw
adxl345.y_raw
adxl345.z_raw
```

değişkenlerinde ham ivme verileri bulunur.

---

# 🧪 Basit Kullanım Örneği

Aşağıdaki örnek, driver'ın minimum kullanımını gösterir:

```c
#include "main.h"
#include "adxl345.h"

ADXL345_t adxl345;

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();

    /* ADXL345'i başlat */
    if (ADXL345_Initialization(
            &adxl345,
            &hspi1,
            GPIOB,
            GPIO_PIN_6))
    {
        /* Sensör bulundu */
    }
    else
    {
        /* Sensör bulunamadı */
    }

    while (1)
    {
        if (adxl345.found)
        {
            ADXL345_Read_XYZ(&adxl345);

            /*
             * Ham veriler:
             *
             * adxl345.x_raw
             * adxl345.y_raw
             * adxl345.z_raw
             */
        }

        HAL_Delay(100);
    }
}
```

---

# 🧠 Kısaca Driver'ı Kullanma Sırası

Bu driver'ı kullanırken temel akış şudur:

```text
1. CubeMX'te SPI'ı yapılandır
            ↓
2. CS pinini GPIO Output yap
            ↓
3. adxl345.h / adxl345.c dosyalarını projeye ekle
            ↓
4. ADXL345_t değişkeni oluştur
            ↓
5. ADXL345_Initialization()
            ↓
6. found == true ?
            ↓
7. ADXL345_Read_XYZ()
            ↓
8. x_raw / y_raw / z_raw değerlerini kullan
```

---

# 🛠️ Driver Fonksiyonları

| Fonksiyon                  | Görevi                        |
| -------------------------- | ----------------------------- |
| `ADXL345_Initialization()` | Driver'ı ve sensörü başlatır  |
| `ADXL345_Who_Am_I()`       | ADXL345 cihaz ID'sini okur    |
| `ADXL345_Read_Register()`  | Tek bir register okur         |
| `ADXL345_Write_Register()` | Tek bir register'a veri yazar |
| `ADXL345_Read_XYZ()`       | X, Y ve Z eksenlerini okur    |

---

# 📋 Register Özeti

Driver'da kullanılan temel register'lar:

| Register      |  Adres | Görevi                  |
| ------------- | -----: | ----------------------- |
| `DEVID`       | `0x00` | Cihaz kimliği           |
| `BW_RATE`     | `0x2C` | Veri hızı               |
| `POWER_CTL`   | `0x2D` | Güç ve ölçüm modu       |
| `DATA_FORMAT` | `0x31` | Ölçüm formatı / aralığı |
| `DATAX0`      | `0x32` | X Low Byte              |
| `DATAX1`      | `0x33` | X High Byte             |
| `DATAY0`      | `0x34` | Y Low Byte              |
| `DATAY1`      | `0x35` | Y High Byte             |
| `DATAZ0`      | `0x36` | Z Low Byte              |
| `DATAZ1`      | `0x37` | Z High Byte             |

---

# ⚠️ Dikkat Edilmesi Gerekenler

### 1. SPI Mode 3 kullanılmalı

```text
CPOL = High
CPHA = 2 Edge
```

olmalıdır.

Yanlış SPI mode kullanılırsa register değerleri yanlış okunabilir.

### 2. CS doğru pine bağlanmalı

Örnekte:

```text
PB6 → CS
```

kullanılmıştır.

Farklı bir GPIO kullanırsanız initialization fonksiyonuna onu vermelisiniz:

```c
ADXL345_Initialization(
    &adxl345,
    &hspi1,
    GPIOC,
    GPIO_PIN_4
);
```

### 3. SPI handle doğru verilmelidir

Örnekte:

```c
&hspi1
```

kullanılmıştır.

SPI2 kullanıyorsanız:

```c
&hspi2
```

verebilirsiniz.

Ancak ilgili SPI peripheral'ının CubeMX tarafından doğru yapılandırılmış olması gerekir.

### 4. Sensör 3.3V ile kullanılmalıdır

ADXL345 breakout kartınızın voltaj özelliklerini kontrol edin. STM32F4 GPIO'ları için uygun lojik seviyeler kullanılmalıdır.

---

# 🧩 Farklı SPI veya CS Pinine Taşımak

Driver'ın güzel taraflarından biri, SPI ve CS pinlerinin kodun içine sabitlenmemiş olmasıdır.

Örneğin:

```c
ADXL345_Initialization(
    &adxl345,
    &hspi2,
    GPIOC,
    GPIO_PIN_4
);
```

yaparak:

```text
SPI2
CS → PC4
```

kullanabilirsiniz.

Driver'ın geri kalan kısmını değiştirmeniz gerekmez.

Bu yapı sayesinde aynı driver farklı projelerde kolayca tekrar kullanılabilir.

---

# 📁 Önerilen Proje Yapısı

STM32CubeIDE projesinde:

```text
Core/
├── Inc/
│   ├── main.h
│   └── adxl345.h
│
└── Src/
    ├── main.c
    └── adxl345.c
```

şeklinde kullanılabilir.

---

# 🧠 Bu Driver'dan Öğrenilebilecek SPI Mantığı

Bu proje sadece ADXL345 okumak için değil, SPI driver yazma mantığını anlamak için de hazırlanmıştır.

Özellikle şu konulara örnek oluşturur:

### 1. SPI register read

```text
CS LOW
   ↓
Register Address + Read Bit
   ↓
Dummy Byte
   ↓
Sensor Response
   ↓
CS HIGH
```

### 2. SPI register write

```text
CS LOW
   ↓
Register Address
   ↓
Data
   ↓
CS HIGH
```

### 3. Multi-byte read

```text
CS LOW
   ↓
Start Register + Read + Multi-byte
   ↓
X0
X1
Y0
Y1
Z0
Z1
   ↓
CS HIGH
```

Bu yapı farklı SPI sensörlerine driver yazarken de oldukça benzer şekilde kullanılabilir.

---

# 🧪 Debug İçin Önerilen Kontroller

Sensör çalışmıyorsa ilk olarak:

### 1. Beslemeyi kontrol et

```text
VCC → 3.3V
GND → GND
```

### 2. SPI pinlerini kontrol et

```text
SCK
MOSI
MISO
```

### 3. CS pinini kontrol et

Örnekte:

```text
PB6
```

### 4. SPI Mode'u kontrol et

```text
SPI Mode 3
```

### 5. `WHO_AM_I` değerini kontrol et

```c
uint8_t id = ADXL345_Who_Am_I(&adxl345);
```

Beklenen:

```text
0xE5
```

### 6. `found` değerini kontrol et

```c
adxl345.found
```

Sensör doğru bağlandıysa:

```text
true
```

olmalıdır.

---

# 📚 Kaynak

ADXL345 register yapısı ve SPI haberleşmesi için repository içerisinde bulunan:

```text
ADXL345.pdf
```

datasheet/reference dokümanı kullanılabilir.

---

# 📄 License

This project is licensed under the MIT License.

---

## 👨‍💻 Author

**Talha Mansur**

STM32 / Embedded Systems çalışmalarım kapsamında hazırlanmıştır.
