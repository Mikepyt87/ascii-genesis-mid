# Third-party source and licenses

| Material | Location | License / notice |
| --- | --- | --- |
| Firmware and project additions | firmware/, tools/ | [GPL-3.0-or-later](LICENSE) |
| Asciiquarium source and adapted artwork | art/ | [GPL-2.0-or-later](art/LICENSE.txt), original notices retained |
| Browser ocean renderer inherited from Mini | preview/ocean.js | GPL-2.0-or-later, retained source notice |
| Surface scenery and adapted Asciiquarium sea monster | `preview/ocean-scenes.js`, generated scene drawing strings in `firmware/main/ocean_scenes.h` | GPL-2.0-or-later; authorship in [CREDITS.md](CREDITS.md). Generated firmware logic remains GPL-3.0-or-later |
| Adafruit GFX bitmap font | preview/font.js, generated assets | [BSD notice](preview/FONT-LICENSE.txt) |
| cJSON 1.7.19~2 | Managed component `espressif/cjson`, downloaded during build | [MIT notice](docs/CJSON-LICENSE.txt) |

Generated firmware assets include the artwork and font notices. Source copies are included. ESP-IDF and esptool are obtained separately and retain their own licenses. The GUITION board layer uses documented pin assignments and panel register values; it does not bundle a third-party display driver. See [CREDITS.md](CREDITS.md).
