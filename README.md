# ASCII Genesis Mid

A touch-controlled ASCII aquarium for the **GUITION ESP32-4848S040**, with a **4-inch, 480 × 480 square screen**. This is the middle-size edition of ASCII Genesis, between the Mini and Raspberry Pi versions.

**v0.1.1** adds faster named parades, randomized ocean starts, and a GT911 touch-address fix. The display and touch controls were confirmed working on the owner's board. See [validation](VALIDATION.md) for remaining checks.

![ASCII Genesis Mid browser preview](docs/preview.png)

The image above is the browser preview, not a photograph of the board.

The ocean has 28 colored creature and vehicle designs, six changing habitats, rising bubbles, swaying plants, and a scrolling seabed. Three creatures swim at a time. Tap the water to reveal **Pause · New World · Parade · Settings**. The parade keeps all 28 designs in their original order. Each crosses in six seconds, with up to two in separate bands and their names at the top right. A full Mid parade takes **87 seconds (1 minute 27 seconds)**, then returns to the preserved ocean. Startup and New World randomize the regular ocean’s opening creatures, directions, positions, and habitat.

The board starts on USB power and works offline. It does not need a Raspberry Pi, SD card, API key, or internet connection. Wi-Fi setup is included, but **this first version installs and updates by USB**. There is no Mid wireless update service yet.

## Start here

- [Install or recover the board](INSTALL.md)
- [Board identification, pin map, and source references](HARDWARE.md)
- [Preview, firmware build, and checks](DEVELOPMENT.md)
- [What has been checked](VALIDATION.md)
- [Credits](CREDITS.md) and [licenses](THIRD-PARTY.md)

## Hardware

Use the GUITION board marked **ESP32-4848S040**, with an **ESP32-S3 N16R8** module, **ST7701 RGB display**, and **GT911 capacitive touch**. This matches the board photo from the identification chat. The [manufacturer's specification](https://www.guition.com/ku/icms/upload/fb081940d6fc11f09850077a33e1404f/FTPData/UEditor/file/2026121/1768961092477/ESP32-4848S040%20Specifications-EN.pdf) describes this board family.

The Mini's 1.47-inch Waveshare board uses different display and touch hardware. Install only firmware built for this Mid board. The existing Mini and Pi projects are unchanged.

## Try it on your computer

With Node.js 20 or newer, open this folder in a terminal and run:

```text
node preview/serve.mjs
```

Open **http://127.0.0.1:4175**. The preview is a design simulation; it does not connect to a board or collect Wi-Fi passwords.

The aquarium draws a 320 × 320 square scene and enlarges it evenly to the physical 480 × 480 screen. Creature shapes keep their proportions. The square edition adds water depth instead of stretching the Mini's wide scene.

## Project status

Mid has its own [repository and releases](https://github.com/Mikepyt87/ascii-genesis-mid). Mid has its own firmware identity, and its code does not fetch or install Mini updates.

Firmware and project additions are GPL-3.0-or-later; artwork is GPL-2.0-or-later. The bitmap font keeps its BSD notice.
