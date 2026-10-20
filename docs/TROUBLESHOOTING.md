# Troubleshooting and USB recovery

Record the version in Settings and the exact message before changing anything. For a report, include the full board marking, what you expected, what happened, and whether it happened on the board or only in the preview. Do not share passwords or private settings.

| Problem | What to try |
| --- | --- |
| Python was not found | Install Python 3.10+, reopen PowerShell, and follow the isolated environment steps in [Install](../INSTALL.md#3-install-the-flashing-tool-on-windows) |
| File not found | Open PowerShell in the folder containing the extracted complete USB image; verify its filename |
| Board lights up but no COM port appears | Try a known data cable, direct USB port, and the board vendor’s serial-driver instructions |
| COM port busy / Access is denied | Close serial monitors and other flashing programs; Ctrl+] exits the ESP-IDF monitor. Recheck the port after reconnecting |
| Flashing cannot connect | Hold BOOT, press/release RST, release BOOT, recheck the port, and retry |
| Write repeatedly fails | Try a shorter cable and 115200 baud instead of 460800 |
| Verified flash but blank or distorted screen | Confirm ESP32-4848S040 N16R8 and the complete Mid image; press RST without BOOT, then collect a startup log |
| Touch does not respond | Tap the water, lift, then tap a visible button. v0.1.0 had a touch bug; install current firmware over USB if necessary |
| USB updates only | v0.1.2 and earlier need one current USB installation to enable Wi-Fi updates |
| Phone connected, but ocean says Offline | The phone may be on the setup network. Enter the home network details, then check Wi-Fi online on the board |
| Connecting to home Wi-Fi | Allow the 20-second attempt to finish. Reopen Settings for status; verify the 2.4 GHz network name and password |
| Setup network disappeared | It closes after saving, Back, or ten minutes. Reopen setup to get a new password |
| Hotspot will not connect | Turn it on after saving its details, use 2.4 GHz, and allow two minutes for retries; see [Wi-Fi](WIFI.md) |
| Clock not ready | Leave internet connected for 1–2 minutes and check again |
| Current version | No newer compatible device release is offered. Source commits alone do not publish firmware |
| Update failed | Keep the current version and retry with reliable power and internet; see [update messages](UPDATES.md#messages) |
| Parade seems empty or stopped | Creatures enter from off screen; allow a moment and check that it is not paused |

## USB recovery

Follow [Install](../INSTALL.md) using **ascii-genesis-mid-usb.bin at 0x0**. Expect to enter Wi-Fi settings again. Do not write the application-only image at 0x0. A normal reinstall does not need a full flash erase. A factory backup can be restored using the instructions there.

## Collect a startup log

In the installation folder’s PowerShell window, after creating the tool environment:

```powershell
.\.flash-tools\Scripts\python.exe -m serial.tools.miniterm COM7 115200
```

Replace COM7 with the actual port. Press RST once to capture startup; Ctrl+] exits the monitor. Close it before flashing. The port is the board’s UART bridge, not a native USB console. Review logs for private information before sharing them in a [GitHub issue](https://github.com/Mikepyt87/ascii-genesis-mid/issues).
