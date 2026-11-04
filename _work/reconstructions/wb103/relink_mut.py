#!/usr/bin/env python3
"""relink_mut.py OUT MODE [args]
MODE: rm <token>... | rep <old> <new> | move_after <token> <anchor> | move_before <token> <anchor>
"""
import shlex, subprocess, sys, os
s=open('/dev/shm/wb103/link_cmd_line.txt').read().strip()
parts=s.split(' && '); cd=parts[0][3:]
gold=shlex.split(parts[1]); strip=shlex.split(parts[2]); reorder=shlex.split(parts[3])
ri=next(i for i,x in enumerate(reorder) if x.endswith('reorder_shstrtab.py')); reorder=reorder[:ri+1]
out=sys.argv[1]; mode=sys.argv[2]; a=sys.argv[3:]
un=out+'.unstripped.so'; oi=gold.index('-o'); gold[oi+1]=un
if mode=='rm':
    for x in a: gold=[y for y in gold if y!=x]
elif mode=='rep':
    gold=[a[1] if y==a[0] else y for y in gold]
elif mode=='move_after':
    tok,anchor=a
    gold.remove(tok); i=gold.index(anchor)+1; gold[i:i]=[tok]
elif mode=='move_before':
    tok,anchor=a
    gold.remove(tok); i=gold.index(anchor); gold[i:i]=[tok]
else: raise SystemExit('bad mode')
r=subprocess.run(gold,cwd=cd,capture_output=True,text=True)
if r.returncode: print('LINK FAIL',r.stderr[:1000]); sys.exit(1)
subprocess.run([strip[0],'--strip-all','-o',out,un],check=True)
subprocess.run(reorder+[un,out],check=True); os.remove(un); print('ok',out)
