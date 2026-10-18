# Validation record

**ASCII Genesis Mid 0.1.2**, prepared October 17, 2026.

| Check | Result |
| --- | --- |
| ESP-IDF 6.1 / ESP32-S3 compile and link | Passed |
| Application fits its 6 MB partition | Passed |
| Browser: one hour, six habitats, creature clearance, pause, controls, 40 randomized starts, ordered two-creature parades | Passed |
| Generated artwork/font assets match source | Passed |
| Browser layout, Pause, and Settings interaction | Visually checked |
| C touch decoder and parade tests | Passed in v0.1.1 GitHub builds; rerun for v0.1.2 |
| C Wi-Fi status, timeout, and retry checks | Added for v0.1.2 release checks |
| Physical LCD display | Owner confirmed the aquarium appeared |
| Physical GT911 reads and button alignment | Owner confirmed touch controls work in v0.1.1; serial log reports GT911 ID 911, 480 × 480, and touch presses |
| Physical home Wi-Fi login and saved-network reconnect | v0.1.2 startup log confirms authentication, association, and an IP address from the home network |
| Power cycle and 30-second startup validation | Pending board test |

v0.1.1 fixed touch, confirmed by the owner. v0.1.2 was flashed to the connected Mid board; a controlled restart confirmed saved-network login and a home-network IP address. Longer-term Wi-Fi behavior remains unverified. The Mini has its own v0.1.8 release for the matching parade/startup changes; the Pi edition is unchanged.
