# Maintainer release instructions

Source commits do not publish a device update. Documentation-only corrections normally need no firmware release.

1. Update the firmware version, README, installation guide, changelog, and validation record together. Use a new numeric X.Y.Z version.
2. Run the [development checks](../DEVELOPMENT.md), including generated assets and documentation links, then wait for **Check Mid** on the intended main commit to pass.
3. Open **Actions → Publish Mid release → Run workflow** on main. Enter the version and leave Publish unchecked to build a test package.
4. Download and extract the workflow artifact. Confirm it includes the complete USB image, application, manifest, checksums, build information, release notes, and packaged source.
5. Test the affected paths on the exact board when available: display, touch, offline startup, parade, home Wi-Fi/hotspot reconnect, and wireless installation. Record software and physical results separately in [Validation](../VALIDATION.md). Do not mark unperformed hardware tests as passed.
6. Run the same workflow with Publish checked, from the same tested main commit. If main changed, check and test the intended source again first.
7. Confirm the numbered release and stable manifest are public. Download the application and verify board ID, exact versioned URL, byte length, SHA-256, and embedded project/version. Confirm the USB image contains the same application.

The workflow publishes `mid-vX.Y.Z` before updating the manifest on `mid-stable`. Numbered releases must remain immutable; use a higher version for a correction. The app must fit a 6 MiB slot and retain project identity `ascii_genesis_mid` and board ID `guition-esp32-4848s040`. Never point the Mid channel at Mini firmware.

For a documentation-only change, run the link and illustration checks, keep release history accurate, and publish source. Existing release ZIPs remain historical snapshots; the repository’s main branch contains current instructions.
