# GUITION board configuration

The identification photo shows **GUITION ESP32-4848S040**, **ESP32-S3 N16R8**, a 40-pin LCD ribbon, touch ribbon, BOOT/RST buttons, and a CH340 USB-to-serial bridge. The controller and pin configuration below come from published support for that model. Their operation on this assembly is pending a board test.

| Part | Configuration |
| --- | --- |
| Processor | ESP32-S3, 240 MHz |
| Memory | 16 MB flash, 8 MB octal PSRAM at 80 MHz |
| Display | 480 × 480, ST7701, 16-bit RGB bus |
| LCD commands | 9-bit software SPI: CS39, SCK48, MOSI47 |
| RGB sync | DE18, VSYNC17, HSYNC16, PCLK21 |
| Red, low to high | 11, 12, 13, 14, 0 |
| Green, low to high | 8, 20, 3, 46, 9, 10 |
| Blue, low to high | 4, 5, 6, 7, 15 |
| Backlight | GPIO38, active high |
| Touch | GT911: SDA19, SCL45, 400 kHz |
| Touch address | Probe 0x5D, then 0x14 |
| Reset/interrupt | No GPIO used; software LCD reset and touch polling |
| Console | UART0, TX43/RX44, 115200 baud; native USB console disabled |

The RGB panel uses 10 MHz PCLK, horizontal front/pulse/back 10/8/50, vertical 10/8/20, and a positive clock edge. A 10-line internal bounce buffer feeds a PSRAM framebuffer. The ST7701 receives Guition/type-9 initialization values; pixel transfer uses RGB565 without SPI byte swapping. Its pixel-format register is 0x60, matching the known Guition sequence.

GT911 register addresses are sent high byte first (0x81, 0x4E), while coordinate values are decoded low byte first. Touch reads status at 0x814E and the first point's X/Y at 0x8150. Fresh reports are acknowledged by clearing status. A report with no points releases touch; no fresh report preserves its previous state. Picture enlargement and touch conversion use the same 480-to-320 ratio.

The screen scans continuously while the app copies frames. Check for visible tearing during the board test. Timing and orientation are provisional. The initial port uses the unrotated Guition sequence; display and touch may need matched transforms if the assembly's orientation differs.

## References

- [GUITION specification](https://www.guition.com/ku/icms/upload/fb081940d6fc11f09850077a33e1404f/FTPData/UEditor/file/2026121/1768961092477/ESP32-4848S040%20Specifications-EN.pdf)
- [Arduino_GFX: Guition ST7701 type-9 initialization](https://github.com/moononournation/Arduino_GFX/blob/master/src/display/Arduino_RGB_Display.h)
- [ESPHome Guition pin definition](https://github.com/esphome/esphome/blob/dev/esphome/components/mipi_rgb/models/guition.py)
- [ESPHome board configuration and touch wiring](https://devices.esphome.io/devices/guition-esp32-s3-4848s040/)
- [Espressif RGB LCD driver](https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32s3/api-reference/peripherals/lcd/rgb_lcd.html)

The board layer is a new ESP-IDF implementation using documented register values. It does not bundle Arduino_GFX or ESPHome source code.
