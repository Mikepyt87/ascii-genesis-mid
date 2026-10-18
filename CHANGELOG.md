# Changes

## 0.1.2

- Corrected WPA3 SAE login configuration to support both derivation methods. The owner's saved home network now connects and receives an IP address.
- Updates setup status after success or disconnection instead of leaving "Connecting to home Wi-Fi..." on screen.
- Shows specific login, network, security, and timeout messages. A connection has a 20-second deadline, with later retries at 45-second intervals.
- Removed the duplicate immediate connection attempt at startup and after provisioning.
- Keeps the working touch controls, named ordered parades, and randomized ocean starts.

## 0.1.1

- Fixed GT911 register address byte order so touches can be read and acknowledged correctly. Added controller identification and touch-press logs.
- Faster parade: six-second crossings, up to two creatures in separate bands, and all 28 designs in the original order. Total: 87 seconds.
- Displays names at the top right, matching the two parade bands.
- Randomizes starting creatures, directions, positions, and habitat at startup and New World. The parade order is unchanged.
- USB installation and updates remain separate from the Mini.

## 0.1.0

- Initial GUITION ESP32-4848S040 port. User confirmed the aquarium display appeared. Touch did not respond; fixed in 0.1.1.
