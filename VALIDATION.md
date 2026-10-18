# Validation record

**ASCII Genesis Mid 0.1.1**, prepared October 17, 2026.

| Check | Result |
| --- | --- |
| ESP-IDF 6.1 / ESP32-S3 compile and link | Passed |
| Application fits its 6 MB partition | Passed |
| Browser: one hour, six habitats, creature clearance, pause, controls, 40 randomized starts, ordered two-creature parades | Passed |
| Generated artwork/font assets match source | Passed |
| Browser layout, Pause, and Settings interaction | Visually checked |
| C touch decoder test | Included; native execution pending |
| Physical LCD display | Owner confirmed the aquarium appeared |
| Physical GT911 reads and button alignment | Owner confirmed touch controls work in v0.1.1; serial log reports GT911 ID 911, 480 × 480, and touch presses |
| Physical Wi-Fi setup and reconnect | Pending board test |
| Power cycle and 30-second startup validation | Pending board test |

v0.1.1 was flashed to the connected Mid board. Its startup log and touch reports were checked, and the owner confirmed working touch. Wi-Fi setup and longer-term behavior remain unverified. The Mini has its own v0.1.8 release for the matching parade/startup changes; the Pi edition is unchanged.
