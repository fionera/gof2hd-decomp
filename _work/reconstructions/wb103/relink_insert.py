#!/usr/bin/env python3
import shlex,subprocess,sys,os
s=open('/dev/shm/wb103/link_cmd_line.txt').read().strip(); parts=s.split(' && '); cd=parts[0][3:]
gold=shlex.split(parts[1]); strip=shlex.split(parts[2]); reorder=shlex.split(parts[3])
ri=next(i for i,x in enumerate(reorder) if x.endswith('reorder_shstrtab.py')); reorder=reorder[:ri+1]
out=sys.argv[1]; un=out+'.unstripped.so'; oi=gold.index('-o'); gold[oi+1]=un
# insert before token A: tokens...
mode=sys.argv[2]
if mode=='before':
    anchor=sys.argv[3]; ins=sys.argv[4:]; i=gold.index(anchor); gold[i:i]=ins
elif mode=='after':
    anchor=sys.argv[3]; ins=sys.argv[4:]; i=gold.index(anchor)+1; gold[i:i]=ins
r=subprocess.run(gold,cwd=cd,capture_output=True,text=True)
if r.returncode: print('LINK FAIL',r.stderr[:1000]); sys.exit(1)
subprocess.run([strip[0],'--strip-all','-o',out,un],check=True)
subprocess.run(reorder+[un,out],check=True); os.remove(un); print('ok',out)
