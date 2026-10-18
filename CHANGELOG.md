# Changes

## 0.1.7

- Reviews all 28 regular designs for a consistent size relationship, shared across Pi, Mini, and Mid.
- Shrinks the jellyfish from 15 × 7 to 6 × 4 characters, and reduces the shore crab, reef lobster, and peacock mantis shrimp.
- Makes the reef shark about 15% longer and the humpback whale about 11% longer without adding height.
- Preserves the original parade order, timing, movement rules, and sparse bubble limits. See [creature proportions](docs/CREATURE-SIZES.md) for all drawing sizes.

## 0.1.6

- Adds occasional surface stretches with sailing boats, surfacing and spouting whales, and a Nessie-like sea monster, one visitor at a time.
- Adds castles and submerged ruins anchored to the scrolling seabed, contained within the bottom third of the screen.
- Uses sparse bubbles from passing creatures or the seabed: at most two, with 7–12 seconds between emission attempts. Bubbles rise and disappear at the waterline.
- Keeps the ordered creature parade separate from surface visitors and landmarks.

## 0.1.5

- Makes parade names 50% larger and brighter in the top-right corner. Both full names fit beside the counter and above the creatures.
- Preserves the original parade order, six-second crossings, two-creature limit, and existing parade duration.

## 0.1.4

- Makes Asciiquarium and its creators, along with OpenGhost's display inspiration, prominent near the top of the GitHub README.
- Uses ocean consistently in the project description, guides, device status messages, and phone Wi-Fi setup. Original project names and historical credits are preserved.
- Keeps the display layout, creatures, ordered parade, touch controls, and Wi-Fi update behavior unchanged.

## 0.1.3

- Adds Check for Updates and Install in Settings, using a separate Mid wireless update channel.
- Checks for new versions after startup and daily. Installation always requires a tap.
- Verifies HTTPS, board identity, version, file size, SHA-256, and the app's embedded identity before changing boot slots. Keeps the previous app for rollback.
- Boards running v0.1.2 or earlier need one USB installation to enable wireless updates. Saved Wi-Fi settings are retained.

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

- Initial GUITION ESP32-4848S040 port. User confirmed the ocean display appeared. Touch did not respond; fixed in 0.1.1.
