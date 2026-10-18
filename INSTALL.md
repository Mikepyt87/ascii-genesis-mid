# Install ASCII Genesis Mid

This guide is for the **GUITION ESP32-4848S040, ESP32-S3 N16R8**. v0.1.1 was installed on the owner's board, with its display and touch controls confirmed working. See VALIDATION.md for the checks performed.

## What you need

- The assembled GUITION display.
- A USB data cable and computer.
- Python with Espressif's flashing tool: `python -m pip install esptool`.
- The extracted Mid installation package.

Unplug the board, check the computer's serial-port list, then plug it in and find the new port. On Windows it will usually have a name such as COM7. The board in the photo uses a USB-to-serial chip; choose that port.

If it will not connect, hold **BOOT**, press and release **RST**, then release BOOT. Try again with a data cable.

## Save the factory software first

From the extracted installation folder, run the following command, replacing COM7 with your port:

```text
python -m esptool --chip esp32s3 --port COM7 read-flash 0 0x1000000 factory-backup.bin
```

Keep that full 16 MB backup somewhere safe. It may contain settings from the existing software, so keep it private.

## Install

The package includes **ascii-genesis-mid-usb.bin**, a combined image containing the bootloader, partition table, startup metadata, and application. Write it at **0x0**:

```text
python -m esptool --chip esp32s3 --port COM7 --baud 460800 write-flash --flash-size 16MB 0x0 ascii-genesis-mid-usb.bin
```

This replaces the board's current program. Leave power connected. Press RST if it does not restart automatically. The ocean should appear and its controls should fade after eight seconds. GPIO19 is reserved for touch; the firmware uses the UART bridge instead of the native USB console.

The separate application file is for developers. It is not a complete installation image and must not be flashed at address zero.

## First board check

1. Confirm the picture is upright and its colors look right.
2. Tap the water, then try Pause, New World, Parade, and Settings.
3. Confirm touch lines up with all four buttons.
4. Leave it powered for at least 30 seconds, restart it, and check it returns to the ocean.
5. Try Wi-Fi setup if wanted.

Record any screen shift, flicker, mirrored picture, wrong touch position, or restart. These are the main things the first hardware test needs to confirm.

## Wi-Fi setup

In **Settings → Set up Wi-Fi with phone**, join **ASCII-Genesis-Mid-XXXX** with the password shown on screen. Open **http://192.168.4.1**, enter your 2.4 GHz network name and password, and save. Rejoin your regular network and check the display's connection status. Setup closes after ten minutes or when you return to the ocean.

Wi-Fi is optional. v0.1.1 has no online features beyond joining your network and the temporary local setup page. **USB Updates Only** is a status button; it cannot install another board's firmware.

## Recover or restore

To reinstall Mid, repeat the installation command. To restore the factory backup:

```text
python -m esptool --chip esp32s3 --port COM7 write-flash --flash-size 16MB 0x0 factory-backup.bin
```

For a startup log, run `python -m serial.tools.miniterm COM7 115200` if pyserial is installed, then press RST. Close the monitor before flashing again.
