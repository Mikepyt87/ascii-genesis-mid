import {checkParade} from './parade-check.mjs';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import '../art/fish-art.js';
import './font.js';
import './ocean-scenes.js';
import './ocean.js';
const {MiniOcean,render,bandsClear,W,H,SWIM_BOTTOM}=globalThis.MiniAquarium;
const sprites=globalThis.AsciiquariumSprites,ocean=new MiniOcean(sprites,4848);
assert.equal(W,320);assert.equal(H,320);assert.equal(sprites.length,56);
const visited=new Set();let rendered=0;
const ctx={fillRect(){rendered++;},save(){},restore(){},beginPath(){},rect(){},clip(){}};
for(let frame=0;frame<36000;frame++){
 ocean.step(.1);visited.add(ocean.habitat);
 assert.ok(ocean.fish.length<=3);
 for(const f of ocean.fish){assert.ok(f.y>=23&&f.y+f.height+2<=SWIM_BOTTOM);assert.ok(f.speed>=9&&f.speed<18);}
 for(let i=0;i<ocean.fish.length;i++)for(let j=i+1;j<ocean.fish.length;j++)assert.ok(bandsClear(ocean.fish[i].y,ocean.fish[i].height,ocean.fish[j].y,ocean.fish[j].height));
 if(frame%1000===0)render(ctx,ocean,MiniFont);
}
assert.equal(visited.size,6);assert.ok(rendered>1000);
ocean.paused=true;const paused=JSON.stringify(ocean);ocean.step(.2);assert.equal(JSON.stringify(ocean),paused);ocean.paused=false;
ocean.controls=false;assert.equal(ocean.action(10,301),'show');assert.equal(ocean.action(10,301),'show');assert.ok(ocean.paused);ocean.paused=false;
assert.equal(ocean.action(150,301),'reset');assert.equal(ocean.action(250,301),'settings');
checkParade(globalThis.MiniAquarium,sprites);
const firmware=fs.readFileSync(new URL('../firmware/main/main.c',import.meta.url),'utf8');
assert.ok(firmware.includes('320*320*2'));assert.ok(firmware.includes('y>=288'));assert.ok(!firmware.includes('320*172'));
const network=fs.readFileSync(new URL('../firmware/main/network.c',import.meta.url),'utf8');
assert.ok(network.includes('CHANNEL_URL')&&network.includes('esp_ota_begin'));
const policy=fs.readFileSync(new URL('../firmware/main/update_policy.h',import.meta.url),'utf8');
assert.ok(policy.includes('guition-esp32-4848s040')&&policy.includes('mid-stable')&&!policy.includes('ascii-genesis-mini'));
assert.ok(network.includes('ASCII-Genesis-Mid'));
console.log('PASS: one simulated hour, square layout, all habitats, creature clearance, pause, controls, 28-creature parade, and board-specific wireless update policy.');

const {checkScenes}=await import('./scenes-check.mjs');checkScenes(globalThis.MiniAquarium,sprites);
