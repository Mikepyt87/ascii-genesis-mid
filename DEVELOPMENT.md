# Preview, build, and checks

Use Node.js 20 or newer. No npm dependencies are required.

```text
node preview/serve.mjs
node preview/check.mjs
node tools/generate-scenes.mjs --check
node tools/generate-assets.mjs --check
```

Open **http://127.0.0.1:4175**. The server binds only to loopback. Stop it with Ctrl+C.

Edit drawings in `art/original-fish.mjs`, then run `node art/restore-sprites.mjs` and `node tools/generate-assets.mjs`. Commit artwork and generated `firmware/main/assets.h` together, with their notices.

## Firmware

Use **ESP-IDF 6.1** for ESP32-S3. Open its terminal, then:

```text
cd firmware
idf.py set-target esp32s3
idf.py build
```

The component manager downloads `espressif/cjson` 1.7.19~2, pinned in `firmware/dependencies.lock`. Hardware interfaces use ESP-IDF's supplied drivers. `sdkconfig.defaults` configures flash, octal PSRAM, UART, and RGB scan recovery. The secondary native USB console must stay disabled because touch uses GPIO19.

The build produces `ascii_genesis_mid.bin` and its matching bootloader, partition table, and OTA metadata. To package a complete USB image, run from the project root:

```text
node tools/package.mjs firmware/build
```

Run this in the ESP-IDF terminal so `python -m esptool` is available. Pass a different build directory as the first argument if needed. Output goes to `release/`, which is ignored by Git.

To flash from source, first save the factory firmware as described in INSTALL.md, then run `idf.py -p COM7 flash monitor` in the firmware folder. Use this build's offsets rather than the Mini's.

The partition layout keeps two 6 MB app slots. The updater fetches `manifest.json` from the `mid-stable` GitHub release. It accepts only newer numeric X.Y.Z versions, the GUITION board ID, and the exact Mid release asset path. HTTPS certificate checks, size, SHA-256, embedded project name, and embedded version must pass before switching slots. Startup validation accepts the app after 30 seconds of successful operation; rollback is enabled. Wi-Fi credentials use the separate `wifi4848` NVS namespace. The setup page accepts connections only on the temporary setup interface and checks a per-session form token.

`tools/policy-check.c` tests version ordering, malformed metadata, wrong boards, wrong repositories, and invalid download sizes. Compile it with the native compiler flags below. `tools/make-manifest.mjs VERSION APP.bin OUTPUT.json` creates the channel manifest from the built application. The release workflow publishes that app and the USB image before updating the stable channel. Never point Mid at the Mini channel.

## C touch decoder check

With a native C compiler:

```text
gcc -std=c11 -Wall -Wextra -Werror tools/touch-check.c -o touch-check
./touch-check
```

This tests report status, release, report length, invalid coordinates, and all 230,400 physical touch positions. Compilation alone cannot verify I2C or physical alignment. See VALIDATION.md for checks actually run.

`tools/wifi-check.c` checks connection messages and retry/timeout policy. Compile it with the same native compiler flags. A connection attempt has a 20-second deadline and a 45-second retry interval; an in-progress attempt is never immediately restarted. WPA3 SAE explicitly supports both password-element derivation methods. Connection event logs include reason codes without passwords or network names.
