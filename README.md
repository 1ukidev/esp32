# ESP32

Alguns experimentos com o ESP32.

## Esquema atual:

```
                    +-------------------------+
                    |         ANTENA          |
                    |                         |
   Sem uso -- EN ---| EN                  D23 |--- GPIO 23 -- Sem uso
   Sem uso -- VP ---| GPIO 36             D22 |--- GPIO 22 -- OLED SCL
   Sem uso -- VN ---| GPIO 39             TX0 |--- GPIO  1 -- Serial USB
   Sem uso ---------| GPIO 34             RX0 |--- GPIO  3 -- Serial USB
   Sem uso ---------| GPIO 35             D21 |--- GPIO 21 -- OLED SDA
   Sem uso ---------| GPIO 32             D19 |--- GPIO 19 -- DHT11 DATA
   Sem uso ---------| GPIO 33             D18 |--- GPIO 18 -- Botão
   Sem uso ---------| GPIO 25              D5 |--- GPIO  5 -- Sem uso
   Sem uso ---------| GPIO 26             TX2 |--- GPIO 17 -- Sem uso
   Sem uso ---------| GPIO 27             RX2 |--- GPIO 16 -- Sem uso
PIR OUT ------------| GPIO 14              D4 |--- GPIO  4 -- Sem uso
   Sem uso ---------| GPIO 12              D2 |--- GPIO  2 -- LED da placa
Buzzer (+) ---------| GPIO 13             D15 |--- GPIO 15 -- Sem uso
GND comum ----------| GND                 GND |--- GND comum
PIR VCC ------------| VIN                 3V3 |--- Alimentação 3.3 V
                    |                         |
                    |  [EN]   [USB]   [BOOT]  |
                    +------------+------------+
                                 |
                            Cabo USB 5 V
```

## Configuração:

```c++
namespace Config {
    constexpr int oledReset = -1;
    constexpr unsigned int screenWidth = 128;
    constexpr unsigned int screenHeight = 64;
    constexpr unsigned int oledAddress = 0x3C;
    constexpr unsigned int sdaPin = 21;
    constexpr unsigned int sclPin = 22;
    constexpr unsigned int dhtPin = 19;
    constexpr unsigned int buttonPin = 18;
    constexpr unsigned int pirPin = 14;
    constexpr unsigned int ledPin = 2;
    constexpr unsigned int buzzerPin = 13;
    constexpr unsigned int buzzerChannel = 2;
    constexpr unsigned int buzzerFrequency = 2000;
    constexpr unsigned long buzzerDuration = 50;
    constexpr unsigned long serialBaudRate = 115200;
    constexpr unsigned long sensorInterval = 4000;
    constexpr unsigned long pirWarmupDuration = 60000;
    constexpr unsigned long pirClearDelay = 3000;
    constexpr unsigned long screenTimeout = 30000;
    constexpr unsigned long buttonDebounce = 50;
    constexpr unsigned long reconnectInterval = 15000;
    constexpr unsigned long serverRetryInterval = 5000;
    constexpr unsigned int pageRefreshSeconds = 4;
    constexpr const char* wifiSSID = "(SSID)";
    constexpr const char* wifiPassword = "(Password)";
}
```
