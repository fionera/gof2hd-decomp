import struct
P='/Users/fionera/Downloads/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
data=open(P,'rb').read()
# ELF32 header
e_phoff=struct.unpack_from('<I',data,0x1c)[0]
e_phentsize=struct.unpack_from('<H',data,0x2a)[0]
e_phnum=struct.unpack_from('<H',data,0x2c)[0]
loads=[]
dyn_off=None
for i in range(e_phnum):
    off=e_phoff+i*e_phentsize
    p_type,p_offset,p_vaddr,p_paddr,p_filesz,p_memsz,p_flags,p_align=struct.unpack_from('<IIIIIIII',data,off)
    if p_type==1: loads.append((p_vaddr,p_offset,p_filesz))
    if p_type==2: dyn_off=(p_offset,p_filesz,p_vaddr)
def v2o(v):
    for vaddr,offset,filesz in loads:
        if vaddr<=v<vaddr+filesz:
            return offset+(v-vaddr)
    return None
# parse dynamic
dynamic={}
o,sz,_=dyn_off
i=o
relinfo={}
while True:
    tag,val=struct.unpack_from('<iI',data,i)
    i+=8
    if tag==0: break
    dynamic.setdefault(tag,val)
# DT_REL=17,DT_RELSZ=18,DT_RELENT=19; DT_RELA=7
rel_v=dynamic.get(17); rel_sz=dynamic.get(18)
# build map of GOT addr -> resolved value (R_ARM_RELATIVE=23 means value=addend(=*loc)+base; base=0)
got={}
if rel_v:
    ro=v2o(rel_v)
    cnt=rel_sz//8
    for k in range(cnt):
        r_offset,r_info=struct.unpack_from('<II',data,ro+k*8)
        rtype=r_info&0xff
        symidx=r_info>>8
        if rtype==23: # R_ARM_RELATIVE
            loc=v2o(r_offset)
            addend=struct.unpack_from('<I',data,loc)[0]
            got[r_offset]=addend
        elif rtype==21: # R_ARM_GLOB_DAT
            got[r_offset]=('SYM',symidx)
def readcstr(v):
    o=v2o(v)
    if o is None: return None
    end=data.index(b'\x00',o)
    return data[o:end].decode('latin1')
import sys
# Now process disasm
disasm=open('/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp/_lp_disasm.txt').read().splitlines()
# Map literal pool addr -> word value
# We'll parse each 'add rX,pc' that follows an 'ldr rX,[litaddr]'
lines=disasm
def litword(litaddr):
    o=v2o(litaddr)
    return struct.unpack_from('<I',data,o)[0]
# Resolve: for instruction 'ldr rX,[LIT]' at addrA, and matching 'add rX,pc' at addrB:
#   target = litword(LIT) + (addrB+4)
# Then if subsequently 'ldr rY,[rX,#0]' -> deref GOT -> string ptr, then string.
# Simpler: parse all and print resolved targets for ldr+add+ldr+ldrb chains.
out=[]
parsed=[]
for ln in lines:
    ln=ln.strip()
    if not ln: continue
    addr=int(ln[:8],16)
    rest=ln[9:].strip()
    parsed.append((addr,rest))
# index by addr
byaddr={a:r for a,r in parsed}
# find ldr rX,[lit]; following add rX,pc
import re
pend={}
for a,r in parsed:
    m=re.match(r'ldr(?:\.w)? (r\d+|r1[01]),\[0x([0-9a-f]+)\]',r)
    if m:
        pend.setdefault(m.group(1),[]).append((a,int(m.group(2),16)))
    m2=re.match(r'add (r\d+|r1[01]),pc',r)
    if m2:
        reg=m2.group(1)
        if pend.get(reg):
            la,lit=pend[reg].pop(0)
            val=litword(lit)
            target=(val+a+4)&0xffffffff
            # resolve
            s=None
            if target in got:
                gv=got[target]
                if isinstance(gv,int):
                    s=readcstr(gv)
                    print(f"add@{a:08x} reg={reg} GOTaddr={target:08x} -> ptr {gv:08x} -> {s!r}")
                else:
                    print(f"add@{a:08x} reg={reg} GOTaddr={target:08x} -> SYM {gv}")
            else:
                s=readcstr(target)
                print(f"add@{a:08x} reg={reg} directaddr={target:08x} -> {s!r}")
