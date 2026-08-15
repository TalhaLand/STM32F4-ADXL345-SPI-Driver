# STM32 ADXL345 SPI Driver

STM32 mikrodenetleyiciler için **ADXL345 3 eksenli ivmeölçer sensörünün SPI üzerinden haberleştirilmesini sağlayan sürücüdür.**

Bu driver ile ADXL345'in:

* SPI üzerinden haberleşmesi
* Device ID kontrolü
* Register okuma/yazma işlemleri
* Ölçüm moduna alınması
* Veri hızının ayarlanması
* ±2g ölçüm aralığının ayarlanması
* X, Y ve Z eksenlerinin ham verilerinin okunması

sağlanmaktadır.

---

# 🚀 Hızlı Başlangıç

Driver'ı ilk kez kullanacak birinin temel olarak yapması gerekenler:

### 1. Dosyaları projeye ekle

Projeye:

```text
adxl345.c
adxl345.h
```

dosyalarını ekleyin.

---

### 2. STM32CubeMX / CubeIDE üzerinden SPI'ı aktif edin

ADXL345 SPI haberleşmesi için STM32'nin SPI çevre birimlerinden birini kullanabilirsiniz.

Örneğin:

```text
STM32
│
├── SPI1
│   ├── SCK
│   ├── MISO
│   └── MOSI
│
└── GPIO
    └── CS
```

**CS (Chip Select)** pini SPI donanımının otomatik NSS pini olmak zorunda değildir.

Bu driver'da CS pini GPIO olarak kontrol edilmektedir.

---

### 3. SPI ayarlarını yapın

ADXL345 SPI haberleşmesinde:

* MSB First
* SPI Mode 3
* 8-bit data
* Full Duplex

kullanılmalıdır.

STM32CubeMX içerisinden SPI'ı buna uygun şekilde yapılandırın.

---

### 4. `main.c` içerisinde driver'ı tanımlayın

Öncelikle header dosyasını ekleyin:

```c
#include "adxl345.h"
```

Daha sonra bir ADXL345 nesnesi oluşturun:

```c
ADXL345_t adxl;
```

---

### 5. Driver'ı başlatın

Örneğin ADXL345'in CS pini `GPIOA Pin 4`, SPI birimi de `SPI1` ise:

```c
ADXL345_Initialization(&adxl, &hspi1, GPIOA, GPIO_PIN_4);
```

Bu fonksiyon:

1. SPI handle'ını driver'a bağlar.
2. CS pinini tanımlar.
3. ADXL345 Device ID'sini kontrol eder.
4. Sensörü measurement mode'a geçirir.
5. Data rate'i 100 Hz yapar.
6. Ölçüm aralığını ±2g olarak ayarlar.

---

### 6. X, Y ve Z verilerini oku

```c
ADXL345_Read_XYZ(&adxl);
```

Okunan değerler driver içerisindeki:

```c
adxl.x_raw
adxl.y_raw
adxl.z_raw
```

değişkenlerine aktarılır.

Örneğin:

```c
while (1)
{
    ADXL345_Read_XYZ(&adxl);

    int16_t x = adxl.x_raw;
    int16_t y = adxl.y_raw;
    int16_t z = adxl.z_raw;

    HAL_Delay(10);
}
```

Bu kadar. 🎯

---

# 🔌 Donanım Bağlantısı

ADXL345'in SPI pinlerini STM32'nin SPI pinlerine bağlayın.

| ADXL345    | STM32             |
| ---------- | ----------------- |
| VCC        | Uygun besleme     |
| GND        | GND               |
| SCL / SCLK | SPI SCK           |
| SDA / MOSI | SPI MOSI          |
| SDO / MISO | SPI MISO          |
| CS         | Herhangi bir GPIO |

> **Not:** ADXL345 modülünüzün besleme gerilimini ve lojik seviyelerini kullandığınız karta göre kontrol edin. Özellikle çıplak ADXL345 entegresi ile hazır modüllerin besleme devreleri aynı değildir.

---

# 🧠 Driver Nasıl Çalışıyor?

Bu driver'ın temel mantığı şu yapı üzerine kuruludur:

```text
main.c
   │
   ▼
ADXL345_Initialization()
   │
   ├── SPI bağlantısını tanımlar
   ├── CS pinini tanımlar
   ├── Device ID kontrolü
   ├── Measurement Mode
   ├── Data Rate
   └── Data Format
   │
   ▼
ADXL345_Read_XYZ()
   │
   ▼
SPI
   │
   ▼
ADXL345
```

Driver doğrudan `main.c` içerisinde SPI işlemlerini yazmak yerine SPI haberleşmesini kendi fonksiyonları içerisinde yönetir.

Bu sayede uygulama tarafında:

```c
ADXL345_Read_XYZ(&adxl);
```

gibi daha anlamlı fonksiyonlar kullanılabilir.

---

# 📁 Dosya Yapısı

```text
Project
│
├── Core
│   ├── Inc
│   │   └── main.h
│   │
│   └── Src
│       └── main.c
│
└── Drivers
    ├── adxl345.h
    └── adxl345.c
```

Driver'ın public API'si `adxl345.h` içerisinde bulunur.

SPI işlemlerinin ve CS kontrolünün gerçek implementasyonu ise `adxl345.c` içerisindedir.

---

# 🧩 ADXL345 Veri Yapısı

Driver içerisinde sensörü temsil etmek için bir `struct` kullanılmıştır:

```c
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t          cs_pin;

    int16_t           x_raw;
    int16_t           y_raw;
    int16_t           z_raw;

    bool              found;
} ADXL345_t;
```

Buradaki temel mantık:

```text
ADXL345_t
│
├── hspi
│     └── Hangi SPI kullanılacak?
│
├── cs_port
│     └── CS hangi GPIO portunda?
│
├── cs_pin
│     └── CS hangi GPIO pininde?
│
├── x_raw
├── y_raw
├── z_raw
│     └── Sensörden okunan ham veriler
│
└── found
      └── Sensör bulundu mu?
```

Bu yaklaşım sayesinde aynı driver ile farklı SPI birimleri veya farklı CS pinleri kullanılabilir.

Örneğin:

```c
ADXL345_t adxl1;
ADXL345_t adxl2;

ADXL345_Initialization(&adxl1, &hspi1, GPIOA, GPIO_PIN_4);
ADXL345_Initialization(&adxl2, &hspi2, GPIOB, GPIO_PIN_12);
```

Aynı driver yapısı farklı SPI/CS kombinasyonlarıyla kullanılabilir.

---

# ⚙️ Initialization Fonksiyonu

Ana başlangıç fonksiyonu:

```c
bool ADXL345_Initialization(
    ADXL345_t *adxl,
    SPI_HandleTypeDef *hspi,
    GPIO_TypeDef *csPort,
    uint16_t csPin
);
```

Parametreler:

| Parametre | Açıklama                        |
| --------- | ------------------------------- |
| `adxl`    | Driver nesnesi                  |
| `hspi`    | Kullanılacak STM32 SPI handle'ı |
| `csPort`  | CS GPIO portu                   |
| `csPin`   | CS GPIO pini                    |

Örneğin:

```c
ADXL345_Initialization(
    &adxl,
    &hspi1,
    GPIOA,
    GPIO_PIN_4
);
```

Burada driver'a:

> "ADXL345 için SPI1'i ve PA4 pinini CS olarak kullan."

denmiş olur.

---

# 🔍 Device ID / WHO AM I Kontrolü

ADXL345'in Device ID register'ı:

```c
#define ADXL345_DEVID_REG       0x00
```

Beklenen değer:

```c
#define ADXL345_DEVID_VALUE     0xE5
```

Driver:

```c
ADXL345_Who_Am_I(&adxl);
```

fonksiyonuyla sensörün Device ID değerini okur.

Eğer:

```text
0xE5
```

gelirse sensörün ADXL345 olduğu kabul edilir.

Başarılı durumda:

```c
adxl.found = true;
```

olur.

Bu kontrol driver yazarken oldukça önemlidir.

Çünkü SPI hattının fiziksel olarak bağlı olması ile gerçekten doğru sensörle haberleşiliyor olması aynı şey değildir.

---

# 📖 Register Okuma Mantığı

ADXL345 register okuma işlemi:

```c
ADXL345_Read_Register()
```

fonksiyonu üzerinden yapılır.

Temel SPI paketi:

```text
Byte 0
┌────────────────────────────┐
│ Register Address + Read Bit│
└────────────────────────────┘

Byte 1
┌────────────────────────────┐
│ Dummy Byte                 │
└────────────────────────────┘
```

Read biti 7. bittir:

```c
regAddr | 0x80
```

Örneğin:

```c
ADXL345_Read_Register(&adxl, 0x00);
```

çağrıldığında SPI üzerinden:

```text
0x80
0x00
```

gönderilir.

İlk byte register adresidir.

İkinci byte sırasında sensör gerçek veriyi MISO hattından gönderir.

Bu yüzden:

```c
return rxData[1];
```

kullanılır.

---

# ✍️ Register Yazma Mantığı

Register yazmak için:

```c
ADXL345_Write_Register()
```

kullanılır.

SPI paketi:

```text
Byte 0
┌────────────────────────────┐
│ Register Address           │
│ Write bit = 0              │
└────────────────────────────┘

Byte 1
┌────────────────────────────┐
│ Data                       │
└────────────────────────────┘
```

Write işlemi için 7. bit sıfır olmalıdır:

```c
txData[0] = regAddr & 0x7F;
```

Örneğin:

```c
ADXL345_Write_Register(
    &adxl,
    ADXL345_POWER_CTL_REG,
    0x08
);
```

sensörü measurement mode'a geçirmek için kullanılır.

---

# 🎛️ Chip Select (CS) Mantığı

SPI haberleşmesinde CS oldukça önemlidir.

Driver içerisinde:

```c
static void ADXL345_CS_Enable(ADXL345_t *adxl)
{
    HAL_GPIO_WritePin(
        adxl->cs_port,
        adxl->cs_pin,
        GPIO_PIN_RESET
    );
}
```

CS'yi LOW yapar.

İşlem bittikten sonra:

```c
static void ADXL345_CS_Disable(ADXL345_t *adxl)
{
    HAL_GPIO_WritePin(
        adxl->cs_port,
        adxl->cs_pin,
        GPIO_PIN_SET
    );
}
```

ile CS tekrar HIGH yapılır.

Temel haberleşme sırası:

```text
CS LOW
   │
   ▼
SPI Transfer
   │
   ▼
CS HIGH
```

Burada özellikle önemli nokta:

**Bir register veya burst transfer işlemi tamamlanmadan CS bırakılmamalıdır.**

---

# 📊 X-Y-Z Verilerinin Okunması

ADXL345'in eksen registerları:

```text
X0 → 0x32
X1 → 0x33

Y0 → 0x34
Y1 → 0x35

Z0 → 0x36
Z1 → 0x37
```

X, Y ve Z değerlerinin her biri 16 bittir.

Ancak SPI üzerinden her eksen:

```text
LOW BYTE
HIGH BYTE
```

şeklinde iki ayrı byte olarak gelir.

Driver bu byte'ları birleştirir:

```c
adxl->x_raw = (int16_t)((rxData[2] << 8) | rxData[1]);
```

Aynı işlem Y ve Z için de yapılır.

---

# 🚀 Multi-Byte / Burst Read

Tek tek:

```text
X0
X1
Y0
Y1
Z0
Z1
```

okumak yerine bütün veriyi tek SPI transaction içerisinde okumak daha verimlidir.

Başlangıç adresi:

```c
ADXL345_DATAX0
```

Read bit:

```c
0x80
```

Multi-byte bit:

```c
0x40
```

Bu nedenle:

```c
txData[0] = ADXL345_DATAX0 | 0x80 | 0x40;
```

olur.

Sonuç olarak:

```text
1 byte → register adresi
6 byte → X, Y, Z verileri
```

toplam:

```text
7 byte
```

SPI transferi gerçekleştirilir.

```text
CS LOW

TX: [ADDRESS] [DUMMY] [DUMMY] [DUMMY] [DUMMY] [DUMMY] [DUMMY]
RX: [------]  [X0]    [X1]    [Y0]    [Y1]    [Z0]    [Z1]

CS HIGH
```

Bu yüzden `rxData[0]` gerçek sensör verisi değildir.

Gerçek veriler:

```text
rxData[1] → X Low
rxData[2] → X High

rxData[3] → Y Low
rxData[4] → Y High

rxData[5] → Z Low
rxData[6] → Z High
```

şeklindedir.

---

# ⚖️ Ölçüm Ayarları

Initialization sırasında:

## Measurement Mode

```c
ADXL345_Write_Register(
    adxl,
    ADXL345_POWER_CTL_REG,
    0x08
);
```

`0x08` değeri ADXL345'i measurement mode'a geçirir.

---

## Data Rate

```c
ADXL345_Write_Register(
    adxl,
    ADXL345_BW_RATE_REG,
    0x0A
);
```

`0x0A`, 100 Hz output data rate seçimini temsil eder.

---

## Measurement Range

```c
ADXL345_Write_Register(
    adxl,
    ADXL345_DATA_FORMAT_REG,
    0x00
);
```

ile varsayılan:

```text
±2g
```

ölçüm aralığı seçilir.

---

# 🔄 Main.c Örneği

En basit kullanım:

```c
#include "main.h"
#include "adxl345.h"

ADXL345_t adxl;

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();

    ADXL345_Initialization(
        &adxl,
        &hspi1,
        GPIOA,
        GPIO_PIN_4
    );

    while (1)
    {
        ADXL345_Read_XYZ(&adxl);

        int16_t x = adxl.x_raw;
        int16_t y = adxl.y_raw;
        int16_t z = adxl.z_raw;

        HAL_Delay(10);
    }
}
```

Gerçek projede initialization sonucunu kontrol etmek daha doğru olacaktır:

```c
if (!ADXL345_Initialization(
        &adxl,
        &hspi1,
        GPIOA,
        GPIO_PIN_4))
{
    // Sensör bulunamadı / haberleşme başarısız
}
```

---

# 🔁 Başka Bir SPI İvmeölçere Adapte Etme

Bu driver'ın önemli bir öğrenme noktası da burada.

Başka bir SPI ivmeölçer kullanıldığında **driver'ın tamamını baştan yazmak zorunda değilsiniz.**

Ancak ADXL345'e özel register ve SPI protokolü değişeceği için bazı bölümler değiştirilmelidir.

Genel olarak iki katman düşünebiliriz:

```text
┌──────────────────────────────┐
│      Sensor Driver           │
│                              │
│ Initialization               │
│ Read XYZ                     │
│ Configure Sensor             │
│ Register Definitions         │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│       SPI HAL Layer          │
│                              │
│ HAL_SPI_Transmit             │
│ HAL_SPI_TransmitReceive      │
│ GPIO CS                      │
└──────────────────────────────┘
               │
               ▼
             STM32
```

STM32'nin SPI kullanım mantığı büyük ölçüde aynı kalabilir.

Fakat yeni sensörün datasheet'ine göre:

* Device ID register
* Device ID değeri
* Register adresleri
* Read bit'i
* Write bit'i
* Multi-byte/burst read biti
* CS davranışı
* Register veri formatı
* X/Y/Z registerları
* Initialization registerları
* SPI mode

yeniden incelenmelidir.

---

## Örneğin LIS3DSH'ye geçmek istersek

Mevcut ADXL345 driver'ını kopyalayıp:

```text
adxl345.c
adxl345.h
```

yerine:

```text
lis3dsh.c
lis3dsh.h
```

oluşturabiliriz.

Daha sonra öncelikle register tanımları değiştirilir:

```c
#define LIS3DSH_WHO_AM_I_REG ...
#define LIS3DSH_CTRL_REG4     ...
#define LIS3DSH_OUT_X_L       ...
```

Ardından:

```c
LIS3DSH_Read_Register()
LIS3DSH_Write_Register()
LIS3DSH_Read_XYZ()
LIS3DSH_Initialization()
```

fonksiyonları yeni sensörün datasheet'ine göre düzenlenir.

**Yani burada asıl tekrar kullanılabilir olan şey SPI haberleşme mantığını öğrenmiş olmamızdır.**

Sensör değişince:

```text
SPI HAL kullanımı
       ↓
büyük ölçüde aynı

Sensör register/protokolü
       ↓
yeniden incelenmeli
```

---

# 🧠 Driver Yazarken Öğrenilen Temel Mantık

Bu projede asıl amaç yalnızca ADXL345'i çalıştırmak değildir.

Driver yazarken şu mantık öğrenilmektedir:

```text
Datasheet
    ↓
Register Map
    ↓
SPI Protocol
    ↓
Read / Write Functions
    ↓
Initialization
    ↓
High-Level Functions
    ↓
Application
```

Örneğin:

```text
Datasheet:
POWER_CTL = 0x2D
Measure bit = 1
        ↓
Driver:
ADXL345_Write_Register(..., 0x2D, 0x08)
        ↓
Application:
ADXL345_Initialization(...)
```

Böylece `main.c` sensörün register detaylarını bilmek zorunda kalmaz.

---

# 🏗️ Driver Mimarisi

Bu projede temel olarak üç farklı seviyeyi ayırmaya çalışıyoruz:

### 1. Application Layer

```c
ADXL345_Read_XYZ(&adxl);
```

Burada uygulama sensörün nasıl çalıştığını bilmez.

---

### 2. Sensor Driver Layer

```c
ADXL345_Read_XYZ()
ADXL345_Read_Register()
ADXL345_Write_Register()
ADXL345_Initialization()
```

Sensörün register ve haberleşme protokolünü burada yönetiyoruz.

---

### 3. STM32 HAL Layer

```c
HAL_SPI_Transmit()
HAL_SPI_TransmitReceive()
HAL_GPIO_WritePin()
```

STM32'nin donanımına HAL üzerinden erişiyoruz.

Bu ayrım, daha büyük embedded projelerde çok önemlidir.

---

# 📌 Public API

Kullanıcının doğrudan kullanabileceği temel fonksiyonlar:

```c
ADXL345_Initialization()
ADXL345_Who_Am_I()
ADXL345_Read_XYZ()
ADXL345_Read_Register()
ADXL345_Write_Register()
```

CS kontrol fonksiyonları ise:

```c
ADXL345_CS_Enable()
ADXL345_CS_Disable()
```

`static` olarak tanımlandığından yalnızca `adxl345.c` içerisinde kullanılabilir.

Bu bilinçli bir tasarımdır.

Çünkü uygulamanın doğrudan:

```c
ADXL345_CS_Enable()
```

çağırmasına gerek yoktur.

CS yönetimini driver kendisi yapmalıdır.

---

# ⚠️ Mevcut Driver İçin Notlar

Bu sürüm öğrenme ve kullanım amacıyla hazırlanmıştır.

İleride driver geliştirilecekse özellikle şu noktalar iyileştirilebilir:

### SPI hata kontrolü

`ADXL345_Read_XYZ()` içerisinde `HAL_SPI_TransmitReceive()` dönüş değeri şu anda kontrol edilmemektedir.

Daha sağlam bir sürümde:

```c
HAL_StatusTypeDef status;
```

kontrol edilerek SPI transferinin gerçekten başarılı olup olmadığı doğrulanabilir.

---

### Initialization kodu

Mevcut sürümde bazı register ayarları `Who_Am_I` kontrolünden önce ve sonra tekrar yazılmaktadır.

Daha temiz bir versiyonda:

```text
1. Device ID kontrolü
2. found = true
3. Power Control
4. Data Rate
5. Data Format
```

şeklinde tek bir initialization akışı oluşturulabilir.

---

### Fiziksel değer dönüşümü

Şu anda:

```c
x_raw
y_raw
z_raw
```

ham ADC/register değerlerini tutmaktadır.

İleride istenirse bunlar:

```text
raw → g
raw → m/s²
```

dönüşümüne çevrilebilir.

Örneğin:

```c
float x_g;
float y_g;
float z_g;
```

gibi değerler eklenebilir.

---

# 🎯 Bu Projeden Sonra Hatırlanması Gerekenler

Bu driver'a ileride tekrar baktığında temel olarak şu akışı hatırlamak yeterlidir:

```text
ADXL345
  │
  │ SPI
  ▼
STM32
  │
  ├── CS GPIO ile kontrol edilir
  │
  ├── Register Read
  │
  ├── Register Write
  │
  ├── Device ID kontrolü
  │
  └── Burst Read
          │
          ▼
      X / Y / Z
```

Ve en önemli fikir:

> **Bir sensör driver'ı yazarken önce datasheet'teki haberleşme protokolünü ve register map'i anlamalı, ardından düşük seviyeli SPI read/write fonksiyonlarını oluşturmalı, daha sonra initialization ve sensöre özel yüksek seviyeli fonksiyonları yazmalıyız.**

Bu yaklaşım yalnızca ADXL345 için değil, **SPI kullanan birçok MEMS sensör ve çevre birimi için uygulanabilir.**

---

# 📚 Kullanılan STM32 Fonksiyonları

Driver'ın STM32 HAL tarafında kullandığı temel fonksiyonlar:

```c
HAL_SPI_Transmit()
HAL_SPI_TransmitReceive()
HAL_GPIO_WritePin()
```

SPI haberleşmesinin temelini anlamak için özellikle şu kavramlara hakim olmak önemlidir:

* SPI Clock
* MOSI
* MISO
* CS / NSS
* SPI Mode
* Clock Polarity (CPOL)
* Clock Phase (CPHA)
* Full Duplex
* Register Address
* Read / Write bit
* Dummy byte
* Multi-byte / Burst transfer

---

# 📜 Lisans

Bu proje eğitim ve kişisel geliştirme amacıyla hazırlanmıştır.

STM32 HAL fonksiyonları ve ADXL345 cihaz özellikleri ilgili üreticilerin dokümantasyonlarına tabidir.
