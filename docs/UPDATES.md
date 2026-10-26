# Wireless updates

Current release: **v0.1.10**, with three [signature fish and entrance celebrations](SIGNATURE-FISH.md), grounded crabs, lobsters, mantis shrimp, and octopuses, plus the existing waterline fix, balanced creature proportions, surface visitors, castles, ruins, and sparse bubbles. See [what changed](../CHANGELOG.md) and the [new scenes guide](SURFACE-SCENES.md).

## Install an offered update

1. Connect the ocean to home Wi-Fi or a **2.4 GHz** hotspot. See [Wi-Fi setup](WIFI.md).
2. Tap the water, then **Settings**. Read the installed version at the top.
3. Tap **Check for updates**. A check does not install or restart anything by itself.
4. If a newer compatible version is offered, tap **Install vX.Y.Z & restart**.
5. Leave power and Wi-Fi connected during the download. The screen reports progress.
6. After the ocean restarts, leave it powered for at least 30 seconds. Open Settings and confirm the new version.

The board also checks after startup and once per day while connected. Installing always requires a tap. Saved Wi-Fi settings remain in place during a normal wireless update.

## Messages

| Message | Meaning / next step |
| --- | --- |
| You have the current version | The published version is the same as, or older than, the running firmware |
| Update ready | A newer release for this exact board was found; tap Install |
| Offline | Connect the board to Wi-Fi/hotspot; the ocean still works |
| Clock not ready | Wait 1–2 minutes with internet available, then check again |
| Update server connection failed | The secure request failed; check internet and retry |
| Update download incomplete | The connection ended before the complete manifest arrived; retry |
| Update server returned invalid data | The returned text was not valid release data; retry and report if persistent |
| No compatible release available | The manifest does not match this board/version policy, check that this is Mid firmware and report persistent errors |
| Update failed - current version kept | Download verification failed; keep the current installation and retry later |

## Older Mid installations

Boards running **v0.1.2 or earlier** need one [complete USB installation](../INSTALL.md) of current Mid firmware. Version 0.1.3 introduced wireless updates. Mini and Pi have separate update systems; their files cannot update Mid.

## What the updater verifies

The channel manifest identifies the board, numeric version, exact versioned HTTPS download URL, binary size, and SHA-256 digest. The board accepts only a newer version for the expected model and repository path. It verifies the embedded application name/version before switching boot slots. It does not install Raspberry Pi files or downgrade the device.

The download is written to an inactive application slot. If a download fails, the current application stays selected. A new image is accepted after its display transfers, PSRAM, touch probe, and render loop run successfully for 30 seconds. A crash/reset before acceptance can cause rollback on the next boot. Wi-Fi availability is not required to accept an otherwise working ocean.

Rollback does not detect every visual or behavior problem. A USB data connection remains the recovery route. Firmware signing and secure boot are not enabled; publishing rights to the update channel authorize firmware for devices using it.
