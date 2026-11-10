#!/usr/bin/env python3
"""Branch-layout ("branch weight") dump for the GoF2 decomp (wave 167, w11220).

For a function, disassembles the original and our build and compares every
conditional branch by *layout direction*: whether the taken target lies
forward (F) or backward (B) of the branch.  A mismatch means the two builds
placed the successor blocks in a different order -- the observable signature
of different branch weights.

Usage:
  python3 branch_layout_w11220.py <orig.dump> <ours.dump>
where each dump is produced by tools/fn_diff.py-style objdump output:
  arm-linux-androideabi-objdump -d --start-address=<a> --stop-address=<a+n> <so>
The dumps must be pre-filtered to `ADDR INSN` lines (see the note).
"""
import sys,re,difflib
def load(p):
    rows=[]
    for l in open(p):
        m=re.match(r'([0-9a-f]{8}) (.*)',l.strip())
        if m: rows.append((int(m.group(1),16),m.group(2)))
    return rows
COND=re.compile(r'^(beq|bne|bcs|bcc|bhs|blo|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|cbz|cbnz)(\.\w)?\s')
def parse(rows):
    idx={a:i for i,(a,t) in enumerate(rows)}
    out=[]
    for i,(a,t) in enumerate(rows):
        m=COND.match(t)
        if not m: continue
        mm=re.search(r'\b([0-9a-f]{4,8})\b',t)
        if not mm: continue
        tgt=int(mm.group(1),16)
        if tgt not in idx: continue
        out.append((i,m.group(1),idx[tgt]))
    return out
o=load(sys.argv[1]); u=load(sys.argv[2])
bo=parse(o); bu=parse(u)
print('orig cond branches:',len(bo),' ours:',len(bu))
# align by instruction index using the normalized sequence
def norm(rows):
    idx={a:i for i,(a,t) in enumerate(rows)}
    out=[]
    for a,t in rows:
        t=re.sub(r'\b([0-9a-f]{4,8})\b',lambda m:'I%d'%idx[int(m.group(1),16)] if int(m.group(1),16) in idx else m.group(1),t)
        t=re.sub(r'<[^>]*>','',t)
        t=re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b','R',t)
        t=re.sub(r'#-?0x[0-9a-f]+|#-?\d+','#I',t)
        out.append(re.sub(r'\s+',' ',t).strip())
    return out
ot=norm(o);ut=norm(u)
sm=difflib.SequenceMatcher(None,ot,ut,autojunk=False)
omap={}
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        for k in range(i2-i1): omap[i1+k]=j1+k
    elif tag=='replace':
        for k in range(min(i2-i1,j2-j1)): omap[i1+k]=j1+k
od={i:(c,t) for i,c,t in bo}; ud={i:(c,t) for i,c,t in bu}
mism=[]
for i in sorted(od):
    if i not in omap: continue
    j=omap[i]
    if j not in ud: continue
    co,to=od[i]; cu,tu=ud[j]
    do=to-i; du=tu-j
    so='F' if do>0 else 'B'
    su='F' if du>0 else 'B'
    if so!=su:
        mism.append((i,o[i][0],co,to,do,su,du,u[j][0]))
print('direction mismatches:',len(mism))
for m in mism[:40]:
    print('  orig idx%d @%08x %s -> I%d (%+d) | ours %s I%d (%+d) @%08x' % (m[0],m[1],m[2],m[3],m[4],m[5],m[3],m[6],m[7]))
