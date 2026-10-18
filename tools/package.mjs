import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {spawnSync} from 'node:child_process';
import {fileURLToPath} from 'node:url';
const root=fileURLToPath(new URL('../',import.meta.url));
const build=path.resolve(process.argv[2]||path.join(root,'firmware/build'));
const out=path.join(root,'release');
const flash=JSON.parse(fs.readFileSync(path.join(build,'flasher_args.json'),'utf8'));
const project=JSON.parse(fs.readFileSync(path.join(build,'project_description.json'),'utf8'));
if(project.project_name!=='ascii_genesis_mid')throw Error('This is not a Mid build');
fs.mkdirSync(out,{recursive:true});
const files=[];
for(const [offset,file] of Object.entries(flash.flash_files)){
 const source=path.resolve(build,file);
 if(!source.startsWith(build+path.sep))throw Error('Build file outside build directory');
 const target=path.join(out,file);fs.mkdirSync(path.dirname(target),{recursive:true});fs.copyFileSync(source,target);
 files.push(offset,target);
}
const python=process.env.IDF_PYTHON_ENV_PATH?path.join(process.env.IDF_PYTHON_ENV_PATH,process.platform==='win32'?'Scripts/python.exe':'bin/python'):'python';
const merged=path.join(out,'ascii-genesis-mid-usb.bin');
const result=spawnSync(python,['-m','esptool','--chip','esp32s3','merge-bin','--output',merged,...flash.write_flash_args,...files],{stdio:'inherit'});
if(result.status!==0)throw Error('esptool image merge failed');
fs.writeFileSync(path.join(out,'flasher_args.json'),JSON.stringify(flash,null,2)+'\n');
// Include the corresponding source and its notices with the install image.
for(const dir of ['art','preview','docs','tools','.github','firmware/main'])fs.cpSync(path.join(root,dir),path.join(out,dir),{recursive:true});
for(const f of ['INSTALL.md','README.md','DEVELOPMENT.md','HARDWARE.md','VALIDATION.md','CHANGELOG.md','LICENSE','THIRD-PARTY.md','CREDITS.md','.gitignore','.gitattributes','firmware/CMakeLists.txt','firmware/partitions.csv','firmware/sdkconfig.defaults']){
 fs.mkdirSync(path.dirname(path.join(out,f)),{recursive:true});fs.copyFileSync(path.join(root,f),path.join(out,f));
}
const sha=filename=>crypto.createHash('sha256').update(fs.readFileSync(filename)).digest('hex');
const names=[...Object.values(flash.flash_files),'ascii-genesis-mid-usb.bin'];
const idfPath=project.idf_path;
const versionSource=process.env.MID_IDF_VERSION?'':fs.readFileSync(path.join(idfPath,'tools/cmake/version.cmake'),'utf8');
const idf=process.env.MID_IDF_VERSION||['MAJOR','MINOR','PATCH'].map(key=>versionSource.match(new RegExp('set\\(IDF_VERSION_'+key+'\\s+(\\d+)\\)'))?.[1]??'?').join('.');
const manifest={name:'ASCII Genesis Mid',version:project.project_version,board:'guition-esp32-4848s040',status:'release',idf,project:'ascii_genesis_mid',usb_address:'0x0',wireless_updates:false,files:names.map(file=>({file,size:fs.statSync(path.join(out,file)).size,sha256:sha(path.join(out,file))}))};
fs.writeFileSync(path.join(out,'build-info.json'),JSON.stringify(manifest,null,2)+'\n');
fs.writeFileSync(path.join(out,'SHA256SUMS.txt'),manifest.files.map(f=>`${f.sha256}  ${f.file}`).join('\n')+'\n');
console.log(`Packaged ASCII Genesis Mid v${manifest.version}: ${merged}`);
