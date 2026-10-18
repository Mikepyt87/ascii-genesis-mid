import fs from 'node:fs/promises';
const [version,output]=process.argv.slice(2);
if(!/^\d+\.\d+\.\d+$/.test(version||'')||!output)throw Error('Supply version and output');
const changelog=await fs.readFile(new URL('../CHANGELOG.md',import.meta.url),'utf8');
const section=changelog.split(`## ${version}\n`)[1]?.split('\n## ')[0]?.trim();
if(!section)throw Error('Missing changelog entry');
await fs.writeFile(output,`For **GUITION ESP32-4848S040**, ESP32-S3 N16R8, 4-inch 480 × 480 ST7701/GT911 display.\n\n${section}\n\n## Installation\n\nDownload **ascii-genesis-mid-usb.bin** and follow the [installation guide](https://github.com/Mikepyt87/ascii-genesis-mid/blob/main/INSTALL.md). The complete USB image goes at **0x0**. The ZIP includes the same firmware, source, notices, preview, and instructions.\n\nMid v0.1.1 was installed on the owner's board: the display and touch controls were confirmed working. See [validation](https://github.com/Mikepyt87/ascii-genesis-mid/blob/main/VALIDATION.md) for checks and remaining limits.\n\nUpdates use USB. This release is separate from the Mini's firmware and wireless update channel.\n`);
