# Validation record

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
| Physical wireless installation and restart | Passed; board fetched the live manifest, verified 1,112,960 bytes, restarted into v0.1.3, and rejoined home Wi-Fi; owner confirmed the aquarium returned |
| Physical LCD display | Owner confirmed the aquarium appeared |
| Physical GT911 reads and button alignment | Owner confirmed touch controls work in v0.1.1; serial log reports GT911 ID 911, 480 × 480, and touch presses |
| Physical home Wi-Fi login and saved-network reconnect | v0.1.2 startup log confirms authentication, association, and an IP address from the home network |
| 30-second startup validation and controlled restart | Passed; after running past validation, a reset loaded v0.1.3 from the new slot at 0x620000 without rollback; unplug/replug power test remains unperformed |

v0.1.1 fixed touch, confirmed by the owner. v0.1.2 fixed saved-network login. For v0.1.3, a development bootstrap with the updater and a lower version number was installed once by USB, preserving saved Wi-Fi credentials. The owner then used Settings to download and install the public v0.1.3 release over Wi-Fi. Serial logs confirmed certificate validation, the live manifest, download verification, restart, the new version, and a home-network IP address. Interrupted-download and forced-rollback hardware tests have not been performed. Longer-term Wi-Fi behavior remains unverified. The Mini has its own v0.1.8 release for the matching parade/startup changes; the Pi edition is unchanged.
