# Validation record

## Current release: v0.1.9 — October 18, 2026

Local checks cover all four bottom dwellers in both directions, visible foot contact, scrolling terrain, no swimming bob, reserved bands, and grounded parade placement. Physical-board confirmation of this version is pending. The [automated checks](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39133542315) and [release build](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39133586263) passed for source commit `80cbf5fdce10c4aea53f5354b8c514783e4ee669`, including the native C terrain-contact tests. The public stable manifest, application, and complete USB image were downloaded and verified: correct board, exact versioned URL, embedded project/version, **1,120,192 application bytes**, SHA-256, and matching application bytes in the USB image at 0x20000. The four grounded designs were visually checked in the browser preview.

## Previous release: v0.1.8 — October 18, 2026

Local simulation and renderer checks passed, including creature identity and unchanged swimming lanes throughout waterline transitions. Native waterline clearance is checked alongside the parade geometry. This version has not been confirmed on the physical board. The [automated checks](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39132054114) and [release build](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39132090599) passed for source commit `3b63aa8a49f9acb7ae39d71a879ebebb96cbb87f`. The public stable manifest, application, and complete USB image were downloaded and verified: correct board, exact versioned URL, embedded project/version, **1,115,648 application bytes**, SHA-256, and matching application bytes in the USB image at 0x20000.

## Previous release: v0.1.7 — October 18, 2026

The shared artwork now uses balanced creature proportions; see [drawing sizes](docs/CREATURE-SIZES.md). Local simulation and renderer checks passed for the new artwork, including full parade entry/exit, non-overlapping bands, bottom-creature contact, and existing surface/bubble limits. This release has not been confirmed on the physical board. The [automated checks](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39129955723) and [release build](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39130027034) passed for source commit `11fde0a79043f6ed83f24391daec330c14456a36`. Downloaded and verified the public stable manifest, application, and complete USB image: correct board, embedded project/version, exact asset URL, **1,115,616 application bytes**, SHA-256, and matching application bytes in the USB image at 0x20000.

## Earlier release: v0.1.6 — October 18, 2026

The release was built from commit `c50da06d92452afcb5d5ebd63ef1823e0550002d`. [Check Mid](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39127569705) and the [release build](https://github.com/Mikepyt87/ascii-genesis-mid/actions/runs/39127630424) both passed.

- ESP-IDF 6.1 compiled and linked the board-specific firmware; the application fits its OTA slot.
- Existing preview checks passed, including one simulated hour, 40 randomized openings, ordered parades, spacing, and controls.
- Six additional ten-minute runs checked every surface visitor, deep/shallow transitions, bottom-third landmarks, the shared two-bubble limit, waterline disappearance, pause, and parade isolation.
- The preview artwork was visually inspected after correcting the monster and Mini proportions. The generated scene header matches the source artwork.
- Downloaded the public stable manifest, application, complete USB image, and matching source ZIP. Verified board, exact versioned URL, embedded project/version, byte count **1,116,416**, SHA-256, and identical application bytes in the USB image at 0x20000.
- The new v0.1.6 scenes and installation have **not yet been confirmed on the physical board**. Earlier hardware results below remain historical evidence, not v0.1.6 hardware tests.

## Earlier hardware validation: v0.1.3

**ASCII Genesis Mid 0.1.3**, prepared October 17, 2026.

| Check | Result |
| --- | --- |
| ESP-IDF 6.1 / ESP32-S3 compile and link | Passed |
| Application fits its 6 MB partition | Passed |
| Browser: one hour, six habitats, creature clearance, pause, controls, 40 randomized starts, ordered two-creature parades | Passed |
| Generated artwork/font assets match source | Passed |
| Browser layout, Pause, and Settings interaction | Visually checked |
| C touch decoder, parade, Wi-Fi status, timeout, and retry checks | Passed in v0.1.3 GitHub release build |
| C update policy: board separation, versions, HTTPS asset path, digest and size | Passed in v0.1.3 GitHub release build |
| Published manifest, app size, and SHA-256 | Passed; downloaded stable manifest, application, USB image, and source ZIP; USB app matches wireless app |
| Physical wireless installation and restart | Passed; board fetched the live manifest, verified 1,112,960 bytes, restarted into v0.1.3, and rejoined home Wi-Fi; owner confirmed the ocean returned |
| Physical LCD display | Owner confirmed the ocean appeared |
| Physical GT911 reads and button alignment | Owner confirmed touch controls work in v0.1.1; serial log reports GT911 ID 911, 480 × 480, and touch presses |
| Physical home Wi-Fi login and saved-network reconnect | v0.1.2 startup log confirms authentication, association, and an IP address from the home network |
| 30-second startup validation and controlled restart | Passed; after running past validation, a reset loaded v0.1.3 from the new slot at 0x620000 without rollback; unplug/replug power test remains unperformed |

v0.1.1 fixed touch, confirmed by the owner. v0.1.2 fixed saved-network login. For v0.1.3, a development bootstrap with the updater and a lower version number was installed once by USB, preserving saved Wi-Fi credentials. The owner then used Settings to download and install the public v0.1.3 release over Wi-Fi. Serial logs confirmed certificate validation, the live manifest, download verification, restart, the new version, and a home-network IP address. Interrupted-download and forced-rollback hardware tests have not been performed. Longer-term Wi-Fi behavior remains unverified. The Mini has its own v0.1.8 release for the matching parade/startup changes; the Pi edition is unchanged.
