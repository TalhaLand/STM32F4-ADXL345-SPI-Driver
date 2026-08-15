STM32F4 ADXL345 SPI Driver 🚀

STM32F4 mikrodenetleyiciler için STM32 HAL kullanılarak hazırlanmış, donanımsal SPI tabanlı ADXL345 3 eksenli ivmeölçer sürücüsüdür.

Driver; ADXL345 ile SPI haberleşmesini register seviyesinde gerçekleştirir ve sensörün başlatılması, register okuma/yazma ve X/Y/Z ivme verilerinin okunmasını sağlar.

🚀 Quick Start — Driver'ı Hemen Kullan

ADXL345 driver'ını STM32 projenizde kullanmak için temel olarak 4 adım yeterlidir.

1️⃣ Driver dosyalarını projeye ekle

Dosyaları STM32CubeIDE projenize ekleyin:

adxl345.h → Core/Inc/
adxl345.c → Core/Src/

main.c içerisine:

#include "adxl345.h"

ekleyin.

2️⃣ ADXL345'i STM32'ye bağla

Bu repository'deki örnek bağlantı STM32F401RE + SPI1 içindir:

ADXL345

STM32F401RE

Açıklama

VCC

3.3V

Besleme

GND

GND

Toprak

CS

PB6

Chip Select

SDO

PA6

SPI1 MISO

SDA

PA7

SPI1 MOSI

SCL

PA5

SPI1 SCK

SPI Mode 3 olarak yapılandırılmalıdır.

Farklı bir STM32F4 veya farklı SPI peripheral'ı kullanıyorsanız, SPI pinlerini MCU'nuzun pinout'una göre ayarlayın.

3️⃣ main.c içerisinde driver'ı başlat

Global alanda:

ADXL345_t adxl345;

oluşturun.

MX_GPIO_Init() ve MX_SPI1_Init() sonrasında:

ADXL345_Initialization(
    &adxl345,
    &hspi1,
    GPIOB,
    GPIO_PIN_6
);

çağrısını yapın.

Bu çağrı:

Driver nesnesini SPI ve CS pini ile ilişkilendirir.

ADXL345'in DEVID register'ını kontrol eder.

Sensör bulunursa found = true yapar.

Sensörü ölçüm moduna geçirir.

Veri hızını ve ölçüm aralığını yapılandırır.

4️⃣ X/Y/Z verilerini oku

while(1) içerisinde:

while (1)
{
    if (adxl345.found)
    {
        ADXL345_Read_XYZ(&adxl345);

        int16_t x = adxl345.x_raw;
        int16_t y = adxl345.y_raw;
        int16_t z = adxl345.z_raw;
    }

    HAL_Delay(100);
}

Artık:

adxl345.x_raw
adxl345.y_raw
adxl345.z_raw

üzerinden X, Y ve Z eksenlerinin ham ivme verilerine ulaşabilirsiniz.

Kısaca: ADXL345_Initialization() driver'ı ve sensörü başlatır, ADXL345_Read_XYZ() ise sensörden güncel X/Y/Z verilerini okur.

🧩 Minimum main.c Örneği

Driver'ın temel kullanım şekli:

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

⚙️ STM32CubeMX Ayarları

Driver'ı kullanmadan önce CubeMX'te SPI1'i aktif edin.

SPI

Mode            → Full-Duplex Master
Frame Format    → Motorola
Data Size       → 8 Bits
First Bit       → MSB First
Clock Polarity  → High
Clock Phase     → 2 Edge
NSS             → Software

Bu ayarlar:

SPI Mode 3
CPOL = 1
CPHA = 1

anlamına gelir.

GPIO

CS pini normal GPIO Output olarak ayarlanmalıdır:

PB6 → GPIO_Output

CS'in başlangıçta HIGH olması tercih edilir:

PB6 = HIGH

Böylece STM32 başlatılırken ADXL345 yanlışlıkla seçilmez.

🔌 Donanım Bağlantısı

ADXL345'in SPI pinleri ilk bakışta biraz kafa karıştırabilir:

SCL → SPI Clock (SCK)

SDA → SPI MOSI

SDO → SPI MISO

CS → Chip Select

Bağlantı:

STM32                         ADXL345

PA5  (SPI1_SCK)   --------->  SCL
PA7  (SPI1_MOSI)  --------->  SDA
PA6  (SPI1_MISO)  <---------  SDO
PB6  (GPIO)       --------->  CS

3.3V              --------->  VCC
GND               --------->  GND

CS neden normal GPIO?

Bu driver'da SPI'nin NSS/CS kontrolü donanıma bırakılmaz. CS pini yazılımsal olarak kontrol edilir.

HAL_GPIO_WritePin(adxl->cs_port, adxl->cs_pin, GPIO_PIN_RESET);

CS'i aktif eder:

CS = LOW → Sensör seçili

İşlem tamamlandığında:

HAL_GPIO_WritePin(adxl->cs_port, adxl->cs_pin, GPIO_PIN_SET);

ile CS pasif yapılır:

CS = HIGH → Sensör seçili değil

📁 Driver Dosyaları

Driver iki temel dosyadan oluşur:

adxl345.h
adxl345.c

adxl345.h

Header dosyasında:

Register adresleri

Beklenen cihaz ID'si

ADXL345_t struct'ı

Driver fonksiyonlarının prototipleri

bulunur.

adxl345.c

Asıl SPI haberleşmesi burada gerçekleştirilir.

Register okuma/yazma, CS kontrolü, sensör başlatma ve XYZ veri okuma işlemleri bu dosyada bulunur.

📦 Driver Yapısı

Driver'ın temel veri yapısı:

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t           cs_pin;

    int16_t             x_raw;
    int16_t             y_raw;
    int16_t             z_raw;

    bool                found;
} ADXL345_t;

Bu yapı sensörle ilgili bilgileri tek yerde tutar.

SPI handle

SPI_HandleTypeDef *hspi;

STM32 HAL tarafından oluşturulan SPI peripheral'ının adresidir.

Örneğin:

&hspi1

verilebilir.

CS port ve pin

GPIO_TypeDef *cs_port;
uint16_t cs_pin;

CS pininin hangi GPIO portunda ve hangi pin üzerinde olduğunu belirtir.

Örneğin:

GPIOB
GPIO_PIN_6

XYZ verileri

int16_t x_raw;
int16_t y_raw;
int16_t z_raw;

Sensörden okunan X, Y ve Z eksenlerinin ham değerleridir.

found

bool found;

Sensörün bulunup bulunmadığını belirtir:

true  → ADXL345 bulundu
false → ADXL345 bulunamadı

🧩 Driver'ı Başlatma

Öncelikle bir ADXL345_t değişkeni oluşturulur:

ADXL345_t adxl345;

Daha sonra:

ADXL345_Initialization(
    &adxl345,
    &hspi1,
    GPIOB,
    GPIO_PIN_6
);

çağrılır.

Parametreler:

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

Yani bu çağrı:

"ADXL345 sensörünü SPI1 kullanarak ve PB6'yı Chip Select olarak kullanarak başlat."

anlamına gelir.

🔍 Initialization İçerisinde Ne Oluyor?

ADXL345_Initialization() driver'ın başlangıç noktasıdır.

İlk olarak kullanılacak SPI ve CS bilgileri driver nesnesine aktarılır:

adxl->hspi = hspi;
adxl->cs_port = csPort;
adxl->cs_pin = csPin;

XYZ değerleri başlangıçta sıfırlanır:

adxl->x_raw = 0;
adxl->y_raw = 0;
adxl->z_raw = 0;

Ardından sensörün kimliği kontrol edilir ve ADXL345 yapılandırılır.

🆔 Sensörün Doğrulanması

Driver, sensörün gerçekten ADXL345 olup olmadığını DEVID register'ı üzerinden kontrol eder.

DEVID Register
Address → 0x00

ADXL345 için beklenen değer:

#define ADXL345_DEVID_VALUE 0xE5

Sensörden 0xE5 gelirse:

adxl->found = true;

olur.

Farklı bir değer gelirse:

adxl->found = false;

olur.

Bu kontrol özellikle SPI bağlantısında sorun olduğunda sensörün bulunup bulunmadığını anlamak için kullanışlıdır.

⚙️ ADXL345 Register Yapılandırması

Initialization sırasında üç temel register yapılandırılır.

1. POWER_CTL — 0x2D

Driver:

ADXL345_Write_Register(
    adxl,
    ADXL345_POWER_CTL_REG,
    0x08
);

işlemini gerçekleştirir.

0x08 ile Measure biti 1 yapılır ve sensör ölçüm moduna geçirilir.

POWER_CTL
Address → 0x2D

0x08
 ↓
Measure = 1

2. BW_RATE — 0x2C

Driver:

ADXL345_Write_Register(
    adxl,
    ADXL345_BW_RATE_REG,
    0x0A
);

işlemini gerçekleştirir.

0x0A, veri hızını:

100 Hz

olarak ayarlar.

3. DATA_FORMAT — 0x31

Driver:

ADXL345_Write_Register(
    adxl,
    ADXL345_DATA_FORMAT_REG,
    0x00
);

işlemini gerçekleştirir.

Bu ayarla varsayılan ölçüm aralığı:

±2g

olur.

📖 Register Okuma Mantığı

Temel SPI register okuma fonksiyonu:

uint8_t ADXL345_Read_Register(
    ADXL345_t *adxl,
    uint8_t regAddr
);

ADXL345 SPI protokolünde register okumak için register adresinin 7. biti 1 yapılır:

regAddr | 0x80

Örneğin:

Register = 0x00

0x00 | 0x80
     ↓
0x80

Daha sonra sensörden veriyi almak için bir dummy byte gönderilir:

uint8_t txData[2] = {
    regAddr | 0x80,
    0x00
};

SPI Full-Duplex olduğu için aynı anda veri alınır.

Asıl sensör verisi:

rxData[1]

içerisindedir.

İşlem genel olarak:

CS LOW
   ↓
Register Address + Read Bit
   ↓
Dummy Byte
   ↓
Sensor Response
   ↓
CS HIGH

şeklindedir.

✍️ Register Yazma Mantığı

Register yazmak için:

bool ADXL345_Write_Register(
    ADXL345_t *adxl,
    uint8_t regAddr,
    uint8_t data
);

kullanılır.

Yazma işleminde register adresinin 7. biti 0 olmalıdır:

regAddr & 0x7F

Gönderilen iki byte:

1. byte → Register adresi
2. byte → Yazılacak veri

Örneğin:

ADXL345_Write_Register(
    adxl,
    ADXL345_POWER_CTL_REG,
    0x08
);

şu işlemi yapar:

Register → 0x2D
Data     → 0x08

Genel SPI akışı:

CS LOW
   ↓
Register Address
   ↓
Data
   ↓
CS HIGH

📐 X, Y, Z Verilerinin Okunması

XYZ verilerini okumak için:

ADXL345_Read_XYZ(&adxl345);

kullanılır.

ADXL345 veri register'ları:

Register

Adres

Açıklama

DATAX0

0x32

X Low Byte

DATAX1

0x33

X High Byte

DATAY0

0x34

Y Low Byte

DATAY1

0x35

Y High Byte

DATAZ0

0x36

Z Low Byte

DATAZ1

0x37

Z High Byte

Her eksen 2 byte kullanır:

X → 2 byte
Y → 2 byte
Z → 2 byte

Toplam → 6 byte

🔄 Multi-Byte SPI Okuma

Driver XYZ verilerini tek tek okumak yerine tek SPI işleminde okur.

Başlangıç register'ı:

DATAX0 → 0x32

Register adresine iki kontrol biti eklenir:

0x80 → Read bit
0x40 → Multi-byte bit

Bu nedenle:

txData[0] = ADXL345_DATAX0 | 0x80 | 0x40;

olur.

Sonuç:

0x32
OR 0x80
OR 0x40
------
0xF2

Bu, ADXL345'e:

0x32 register'ından başlayarak oku ve devamındaki register'ları da getir.

anlamına gelir.

SPI transferinde:

1 byte → Register adresi
6 byte → X, Y, Z verileri

olmak üzere toplam 7 byte transfer edilir.

CS LOW
   ↓
7 byte SPI transfer
   ↓
CS HIGH

🧮 Ham XYZ Verilerinin Birleştirilmesi

Her eksen için sensörden iki byte gelir.

Örneğin X:

X0 → Low Byte
X1 → High Byte

Driver bu iki byte'ı 16-bit signed değere dönüştürür:

adxl->x_raw =
    (int16_t)((rxData[2] << 8) | rxData[1]);

Y:

adxl->y_raw =
    (int16_t)((rxData[4] << 8) | rxData[3]);

Z:

adxl->z_raw =
    (int16_t)((rxData[6] << 8) | rxData[5]);

Sonuç olarak:

adxl345.x_raw
adxl345.y_raw
adxl345.z_raw

değişkenlerinde ham ivme değerleri bulunur.

🛠️ Driver Fonksiyonları

Fonksiyon

Görevi

ADXL345_Initialization()

Driver'ı ve sensörü başlatır

ADXL345_Who_Am_I()

ADXL345 cihaz ID'sini okur

ADXL345_Read_Register()

Tek bir register okur

ADXL345_Write_Register()

Tek bir register'a veri yazar

ADXL345_Read_XYZ()

X, Y ve Z eksenlerini okur

📋 Kullanılan Register'lar

Register

Adres

Görevi

DEVID

0x00

Cihaz kimliği

BW_RATE

0x2C

Veri hızı

POWER_CTL

0x2D

Güç ve ölçüm modu

DATA_FORMAT

0x31

Ölçüm formatı / aralığı

DATAX0

0x32

X Low Byte

DATAX1

0x33

X High Byte

DATAY0

0x34

Y Low Byte

DATAY1

0x35

Y High Byte

DATAZ0

0x36

Z Low Byte

DATAZ1

0x37

Z High Byte

🧩 Farklı SPI veya CS Pinine Taşımak

SPI ve CS pinleri driver içerisinde sabitlenmemiştir.

Örneğin SPI2 ve PC4 kullanmak isterseniz:

ADXL345_Initialization(
    &adxl345,
    &hspi2,
    GPIOC,
    GPIO_PIN_4
);

yeterlidir.

Burada:

SPI2 → Haberleşme
PC4  → CS

olarak kullanılır.

Driver'ın geri kalan kodunu değiştirmeniz gerekmez.

📁 Önerilen Proje Yapısı

Core/
├── Inc/
│   ├── main.h
│   └── adxl345.h
│
└── Src/
    ├── main.c
    └── adxl345.c

🧠 Bu Driver'dan Öğrenilebilecek SPI Mantığı

Bu proje sadece ADXL345 kullanmak için değil, SPI tabanlı sensör driver'ı yazma mantığını öğrenmek için de hazırlanmıştır.

Özellikle:

SPI Register Read

CS LOW
   ↓
Register Address + Read Bit
   ↓
Dummy Byte
   ↓
Sensor Response
   ↓
CS HIGH

SPI Register Write

CS LOW
   ↓
Register Address
   ↓
Data
   ↓
CS HIGH

Multi-Byte Read

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

Bu mantık, farklı SPI sensörlerine driver yazarken de büyük ölçüde benzer şekilde kullanılabilir.

🧪 Debug / Sorun Giderme

Sensör çalışmıyorsa şu sırayla kontrol edin:

1. Besleme

VCC → 3.3V
GND → GND

2. SPI pinleri

SCK
MOSI
MISO

3. CS

Örnek bağlantıda:

PB6 → CS

4. SPI Mode

SPI Mode 3
CPOL = High
CPHA = 2 Edge

5. WHO_AM_I

uint8_t id = ADXL345_Who_Am_I(&adxl345);

Beklenen değer:

0xE5

6. found

adxl345.found

Sensör doğru şekilde bulunduysa:

true

olmalıdır.

📚 Kaynak

ADXL345 register yapısı ve SPI haberleşmesi için repository içerisinde bulunan:

ADXL345.pdf

datasheet/reference dokümanı kullanılabilir.
