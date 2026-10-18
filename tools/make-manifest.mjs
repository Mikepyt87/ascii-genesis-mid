import fs from 'node:fs/promises';
import crypto from 'node:crypto';
const [version,binary,output]=process.argv.slice(2);
if(!/^\d{1,5}\.\d{1,5}\.\d{1,5}$/.test(version||'')||version.split('.').some(n=>Number(n)>65535))throw Error('Use a stable numeric X.Y.Z version');
const data=await fs.readFile(binary);
if(data.length<256||data.length>0x600000||data[0]!==0xe9)throw Error('Expected ESP32 application image, at most 6 MiB');
const manifest={board:'guition-esp32-4848s040',version,size:data.length,
    sha256:crypto.createHash('sha256').update(data).digest('hex'),
    url:`https://github.com/Mikepyt87/ascii-genesis-mid/releases/download/mid-v${version}/ascii-genesis-mid.bin`};
await fs.writeFile(output,JSON.stringify(manifest,null,2)+'\n');
