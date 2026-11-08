#!/usr/bin/env python3
"""Instruction-index alignment diff for the residual functions.

Usage: python3 _work/reconstructions/align_diff.py <mangled> [<mangled> ...]

Unlike the same-address raw byte diff (which counts every byte after a single
shift as different), this aligns the original and our instruction streams with
difflib after normalising branch targets to the *target instruction index*, so
a uniformly shifted but otherwise identical block still matches.  The total
"non-matching instructions" is a better structural progress metric than raw
bytes for large shifted functions; it is still sensitive to the greedy
alignment on very large functions (LevelScript::process), so treat that one as
indicative only.

Written by wave-146 w9220; committed as a shared tool by the coordinator.
"""
import subprocess, sys, re, collections, difflib
OBJ='/home/debian/GalaxyOnFire2/gof2-decomp/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS='/home/debian/GalaxyOnFire2/gof2-decomp/cmake-build-match/libgof2hdaa.so'

def sym(path,name):
    out=subprocess.check_output([OBJ,'-T',path],text=True)
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name: return int(p[0],16), int(p[4],16)
    return None

def disasm(path,addr,size):
    out=subprocess.check_output([OBJ,'-d','--start-address=%d'%addr,'--stop-address=%d'%(addr+size),path],text=True)
    ins=[]
    for l in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$',l)
        if not m: continue
        a=int(m.group(1),16); t=m.group(2).strip()
        if not t: continue
        ins.append((a,t))
    return ins

def opcode(t): return t.split()[0] if t else ''

def classify(o,u):
    if o is None: return 'extra-ours:'+opcode(u)
    if u is None: return 'extra-orig:'+opcode(o)
    om,um=opcode(o),opcode(u)
    ob=om.replace('.w','').replace('.n',''); ub=um.replace('.w','').replace('.n','')
    if om==um:
        oo=o[len(om):].strip(); uo=u[len(um):].strip()
        if re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b','R',oo)==re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b','R',uo):
            return 'regalloc:'+om
        if re.sub(r'#-?0x[0-9a-f]+|#-?\d+','#I',oo)==re.sub(r'#-?0x[0-9a-f]+|#-?\d+','#I',uo):
            return 'imm:'+om
        if re.sub(r'\[[^\]]*\]','[M]',oo)==re.sub(r'\[[^\]]*\]','[M]',uo):
            return 'memaddr:'+om
        return 'operand:'+om
    if ob==ub: return 'width:'+om+'/'+um
    fam={'ldr':'load','ldrb':'load','ldrh':'load','str':'store','strb':'store','strh':'store',
         'mov':'mov','movs':'mov','movw':'mov','movt':'mov','mvn':'mov','mvns':'mov',
         'add':'add','adds':'add','sub':'sub','subs':'sub','cmp':'cmp','cmn':'cmp','tst':'cmp','teq':'cmp'}
    if fam.get(ob)==fam.get(ub) and fam.get(ob): return 'family:'+om+'/'+um
    return 'opcode:'+om+'/'+um

def main(names):
    agg=collections.Counter(); per={}
    for name in names:
        a=sym(ORIG,name); b=sym(OURS,name)
        if not a or not b: print('MISSING',name); continue
        oi=disasm(ORIG,*a); ui=disasm(OURS,*b)
        # address -> index
        oidx={ad:i for i,(ad,t) in enumerate(oi)}
        uidx={ad:i for i,(ad,t) in enumerate(ui)}
        def norm(t,idx,base):
            # replace branch target address with target instruction index
            def rep(m):
                ad=int(m.group(1),16)
                if ad in idx: return 'I%d'%idx[ad]
                return 'X'
            t2=re.sub(r'\b([0-9a-f]{4,8})\b',rep,t)
            t2=re.sub(r'<[^>]*>','',t2)
            return re.sub(r'\s+',' ',t2).strip()
        ot=[norm(t,oidx,a[0]) for _,t in oi]
        ut=[norm(t,uidx,b[0]) for _,t in ui]
        sm=difflib.SequenceMatcher(None,ot,ut,autojunk=False)
        cats=[]
        for tag,i1,i2,j1,j2 in sm.get_opcodes():
            if tag=='equal': continue
            if tag=='replace':
                for k in range(max(i2-i1,j2-j1)):
                    c=classify(ot[i1+k] if i1+k<i2 else None, ut[j1+k] if j1+k<j2 else None)
                    cats.append(c); agg[c]+=1
            elif tag=='delete':
                for i in range(i1,i2): c='extra-orig:'+opcode(ot[i]); cats.append(c); agg[c]+=1
            else:
                for j in range(j1,j2): c='extra-ours:'+opcode(ut[j]); cats.append(c); agg[c]+=1
        per[name]=cats
    return per,agg

if __name__=='__main__':
    names=sys.argv[1:]
    per,agg=main(names)
    for name,cats in per.items():
        c=collections.Counter(cats)
        print('%-70s %5d  %s'%(name,sum(c.values()),' '.join('%s=%d'%(k,v) for k,v in c.most_common(10))))
    print('='*100)
    print('TOTAL',sum(agg.values()))
    for k,v in agg.most_common(50): print('%6d  %s'%(v,k))
    # per-function presence, excluding mega
    mega={'_ZN6LayoutC1Ev','_ZN6LayoutC2Ev','_ZN11LevelScript7processEi'}
    pres=collections.Counter(); cnt=collections.Counter()
    for name,cats in per.items():
        if name in mega: continue
        c=collections.Counter(cats)
        for k,v in c.items():
            pres[k]+=1; cnt[k]+=v
    print('='*100)
    print('LONG TAIL (excluding Layout C1/C2, LevelScript): category  funcs  count')
    for k,v in cnt.most_common(40): print('%-30s %3d %6d'%(k,pres[k],v))
