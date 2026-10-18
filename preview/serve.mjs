import http from 'node:http';
import fs from 'node:fs/promises';
const files=new Map([['/',['preview.html','text/html']],['/ocean.js',['ocean.js','text/javascript']],['/font.js',['font.js','text/javascript']],['/fish-art.js',['../art/fish-art.js','text/javascript']]]);
http.createServer(async(req,res)=>{
 const f=files.get(req.url);
 if(req.method!=='GET'||!f){res.writeHead(404);res.end();return;}
 try{const data=await fs.readFile(new URL(f[0],import.meta.url));res.writeHead(200,{'Content-Type':f[1]+'; charset=utf-8','Cache-Control':'no-store'});res.end(data);}catch{res.writeHead(500);res.end('Preview unavailable');}
}).listen(4175,'127.0.0.1',()=>console.log('ASCII Genesis Mid: http://127.0.0.1:4175'));
