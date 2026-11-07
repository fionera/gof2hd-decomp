import sys,os
sys.path.insert(0,'/home/debian/GalaxyOnFire2/wt/w7727/tools/verify')
import verify as V
from elf import Elf32
from pathlib import Path
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS=sys.argv[1] if len(sys.argv)>1 else '/home/debian/GalaxyOnFire2/wt/w7727/cmake-build-match/libgof2hdaa.so'
OBJ='/home/debian/GalaxyOnFire2/wt/w7727/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
import tempfile
tempfile.tempdir='/dev/shm/w7727'
of=Elf32(ORIG); oe=Elf32(OURS)
orig=of.functions(); ours=oe.functions(); common=set(orig)&set(ours)
def ar(e):
    mapped=[s for s in e.sections if s['addr']]
    lo=next((s['addr'] for s in mapped if s['name']=='.plt'),min(s['addr'] for s in mapped))
    return lo,max(s['addr']+s['size'] for s in mapped)
orr=V.Resolver(orig,common,of.plt_map(),ar(of)); our=V.Resolver(ours,common,oe.plt_map(),ar(oe))
co={o[2] for o in of.objects()}&{o[2] for o in oe.objects()}
for r,e in ((orr,of),(our,oe)): r.attach_data(e,co)
imp=({v for k,v in of.got_map().values() if k=='sym'}&{v for k,v in oe.got_map().values() if k=='sym'})
orr.imports=our.imports=imp
name=sys.argv[2] if len(sys.argv)>2 else '_ZN11LevelScript7processEi'
o,u=orig[name],ours[name]
ob=of.bytes_at(o['value'],o['size']); ub=oe.bytes_at(u['value'],u['size'])
oraw,on=V.disassemble(OBJ,ob,o['thumb'],o['value'],orr)
uraw,un=V.disassemble(OBJ,ub,u['thumb'],u['value'],our)
on,un=V.reconcile(on,un)
print('size',o['size'],u['size'],'insns',len(on),len(un))
print('pct',V.score(on,un))
