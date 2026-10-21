# emu.py: tiny Thumb2/VFP/NEON emulator over capstone for straight-line-ish leaf functions.
# usage: emu.py SYM flag=val ... -> prints stores to this (r0) as offset:size:value
import sys,struct
sys.path.insert(0,'/home/debian/GalaxyOnFire2/wt/w4e/tools/verify')
import verify as V
from capstone import *
from capstone.arm import *
SO='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
e=V.Elf32(SO)
got=e.got_map()
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB); md.detail=True
FAKE=0x70000000
symaddr={}
def fake(name):
    if name not in symaddr: symaddr[name]=FAKE+0x100*len(symaddr)
    return symaddr[name]
THIS=0x50000000; STK=0x60000000
class M:
    def __init__(s,flags):
        s.mem={}; s.flags=flags; s.stores={}; s.trace=[]; s.pc=0
    def rb(s,a):
        if a in s.mem: return s.mem[a]
        for n,fa in symaddr.items():
            if fa<=a<fa+0x100:
                short=n.replace('_ZN7Globals','')
                for k,v in s.flags.items():
                    if short.endswith(k+'E') and short[:-len(k)-1].isdigit():
                        s.trace.append((s.pc,'LOAD '+k,1)); return v if a==fa else 0
                raise Exception('unknown global read '+n)
        if THIS<=a<THIS+0x10000: return 0
        if STK-0x10000<=a<STK+0x100: return 0
        b=e.bytes_at(a,1)
        if len(b)!=1: raise Exception('bad read %x'%a)
        return b[0]
    def r(s,a,n):
        if n==4 and a in got and got[a][0]=='sym' and a not in s.mem: return fake(got[a][1])
        return int.from_bytes(bytes(s.rb(a+i) for i in range(n)),'little')
    def w(s,a,n,v):
        for i in range(n): s.mem[a+i]=(v>>(8*i))&0xff
        if THIS<=a<THIS+0x10000:
            for i in range(n): s.stores[a-THIS+i]=(v>>(8*i))&0xff
            s.trace.append((s.pc,a-THIS,n))
def run(sym,flags):
    fn=e.functions()[sym]; base=fn['value']; size=fn['size']
    code=e.bytes_at(base,size)
    insns={}
    m=M(flags)
    R=[0]*16; R[0]=THIS; R[13]=STK; R[14]=0xdead0000
    vf=bytearray(256)  # d0..d31
    N=Z=C=Vf=0
    it=[]
    pc=base
    def cond(cc):
        return {ARM_CC_EQ:Z,ARM_CC_NE:not Z,ARM_CC_HS:C,ARM_CC_LO:not C,ARM_CC_MI:N,ARM_CC_PL:not N,
                ARM_CC_HI:C and not Z,ARM_CC_LS:(not C) or Z,ARM_CC_GE:N==Vf,ARM_CC_LT:N!=Vf,
                ARM_CC_GT:(not Z) and N==Vf,ARM_CC_LE:Z or N!=Vf,ARM_CC_AL:1,ARM_CC_INVALID:1}[cc]
    def rid(r):
        n=md.reg_name(r)
        return {'sb':9,'sl':10,'fp':11,'ip':12,'sp':13,'lr':14,'pc':15}.get(n, int(n[1:]) if n[0]=='r' else None)
    def vreg(r):
        n=md.reg_name(r)
        return n[0],int(n[1:])
    def vget(r):
        k,i=vreg(r)
        if k=='s': return bytes(vf[4*i:4*i+4])
        if k=='d': return bytes(vf[8*i:8*i+8])
        if k=='q': return bytes(vf[16*i:16*i+16])
    def vset(r,b):
        k,i=vreg(r); o={'s':4,'d':8,'q':16}[k]*i; vf[o:o+len(b)]=b
    steps=0
    while True:
        steps+=1
        if steps>100000: raise Exception('loop')
        if pc==0xdead0000: break
        if pc not in insns: insns[pc]=next(md.disasm(code[pc-base:pc-base+4],pc,1))
        i=insns[pc]; npc=pc+i.size; m.pc=pc-base
        mn=i.mnemonic.split('.')[0]; ops=i.operands
        execute=True
        in_it=bool(it)
        if it:
            execute=it.pop(0)
        cc=i.cc
        if execute and cc not in (ARM_CC_AL,ARM_CC_INVALID) and mn not in ('b','it','ite','itt','itte','ittt','itttt','iteet','itee','ittee','itet'):
            execute=cond(cc)
        if mn.startswith('it') and len(mn)<=5 and mn[:2]=='it':
            c=cond(cc)
            pat=mn[1:]
            it=[ (c if ch=='t' else (not c)) for ch in pat]
            pc=npc; continue
        mn2=mn
        for suf in ('eq','ne','hs','lo','mi','pl','hi','ls','ge','lt','gt','le','cs','cc'):
            if mn2.endswith(suf) and mn2[:-2] in ('mov','movw','add','sub','mvn','vmov','vldr','str','ldr','orr','addw','movt','vstr','strd','movs','b'):
                mn2=mn2[:-2]; break
        mn=mn2
        def opv(o):
            if o.type==ARM_OP_REG:
                x=rid(o.reg); v=R[x] if x!=15 else ((pc+4)&~3 if (mn in('add','addw','adr','sub','subw') and ops[-1].type==ARM_OP_IMM) else pc+4)
                if o.shift.type!=0:
                    st,sv=o.shift.type,o.shift.value
                    if st==ARM_SFT_LSL: v=(v<<sv)&0xffffffff
                    elif st==ARM_SFT_LSR: v=v>>sv
                    elif st==ARM_SFT_ASR: v=((v-(1<<32) if v&0x80000000 else v)>>sv)&0xffffffff
                    else: raise Exception('shift '+i.op_str)
                return v
            if o.type==ARM_OP_IMM: return o.imm&0xffffffff
            raise Exception('op '+i.op_str)
        def addr(o):
            b=rid(o.mem.base); bv=R[b] if b!=15 else (pc+4)&~3
            if mn in ('vld1','vst1'): return bv
            if o.mem.index: 
                iv=R[rid(o.mem.index)]
                if o.mem.lshift: iv<<=o.mem.lshift
                bv+=iv*( -1 if o.subtracted else 1)
            return (bv+o.mem.disp)&0xffffffff
        def setnz(v):
            nonlocal N,Z
            N=(v>>31)&1; Z=int(v==0)
        sets = i.update_flags and not (in_it and i.size==2)
        if not execute: pc=npc; continue
        if mn in ('push','stmdb','vpush'):
            pass
        elif mn in ('pop',) or (mn=='ldm' and rid(ops[0].reg)==13):
            if any(o.type==ARM_OP_REG and rid(o.reg)==15 for o in ops): break
        elif mn=='bx' and rid(ops[0].reg)==14: break
        elif mn in ('mov','movs','movw','mvn','mvns'):
            d=rid(ops[0].reg); v=opv(ops[1])
            if mn.startswith('mvn'): v=(~v)&0xffffffff
            R[d]=v
            if sets: setnz(v)
        elif mn=='adr':
            R[rid(ops[0].reg)]=(((pc+4)&~3)+ops[1].imm)&0xffffffff
        elif mn=='movt':
            d=rid(ops[0].reg); R[d]=(R[d]&0xffff)|(opv(ops[1])<<16)
        elif mn in ('add','adds','addw','sub','subs','subw','rsb','rsbs','orr','orrs','and','ands','eor','eors','bic','bics','orn','lsl','lsls','lsr','lsrs','asr','asrs','mul','muls'):
            d=rid(ops[0].reg)
            if len(ops)==3: a=opv(ops[1]); b=opv(ops[2])
            else: a=R[d]; b=opv(ops[1])
            base_=mn.rstrip('s') if mn not in ('asrs','lsls','lsrs') else mn[:-1]
            if mn in ('adds','subs','orrs','ands','eors','bics','muls','rsbs'): base_=mn[:-1]
            if base_ in('add','addw'): v=a+b; 
            elif base_ in('sub','subw'): v=a-b
            elif base_=='rsb': v=b-a
            elif base_=='orr': v=a|b
            elif base_=='and': v=a&b
            elif base_=='eor': v=a^b
            elif base_=='bic': v=a&~b
            elif base_=='orn': v=a|(~b)
            elif base_=='lsl': v=a<<(b&0xff)
            elif base_=='lsr': v=a>>(b&0xff)
            elif base_=='asr': v=(a-(1<<32) if a&0x80000000 else a)>>(b&0xff)
            elif base_=='mul': v=a*b
            else: raise Exception(mn)
            if base_ in ('add','addw') and (sets): C=int(v>0xffffffff)
            if base_ in ('sub','subw') and sets: C=int(a>=b)
            v&=0xffffffff; R[d]=v
            if sets: setnz(v)
        elif mn in ('neg','negs'):
            d=rid(ops[0].reg); v=(-opv(ops[1]))&0xffffffff; R[d]=v
            if sets: setnz(v)
        elif mn=='clz':
            d=rid(ops[0].reg); v=opv(ops[1]); R[d]=32-v.bit_length()
        elif mn in ('cmp','cmn','tst','teq'):
            a=opv(ops[0]); b=opv(ops[1])
            if mn=='cmp': r=(a-b)&0xffffffff; C=int(a>=b); 
            elif mn=='cmn': r=(a+b)&0xffffffff; C=int(a+b>0xffffffff)
            elif mn=='tst': r=a&b
            else: r=a^b
            if mn in('cmp','cmn'):
                sa=a-(1<<32) if a>>31 else a; sb=b-(1<<32) if b>>31 else b
                sr=sa-sb if mn=='cmp' else sa+sb
                Vf=int(sr<-(1<<31) or sr>=(1<<31))
            setnz(r)
        elif mn in ('ldr','ldrb','ldrh','ldrsb','ldrsh','ldrd'):
            o=ops[-1]
            if len(ops)>=2 and ops[-1].type==ARM_OP_IMM:  # post-index
                o=ops[-2]
            a=addr(o)
            n={'ldr':4,'ldrb':1,'ldrh':2,'ldrsb':1,'ldrsh':2,'ldrd':4}[mn]
            if mn=='ldrd':
                R[rid(ops[0].reg)]=m.r(a,4); R[rid(ops[1].reg)]=m.r(a+4,4)
            else:
                v=m.r(a,n)
                if mn=='ldrsb' and v&0x80: v|=0xffffff00
                if mn=='ldrsh' and v&0x8000: v|=0xffff0000
                R[rid(ops[0].reg)]=v
            if i.writeback:
                b=rid(o.mem.base); R[b]=a if ops[-1].type!=ARM_OP_IMM else (R[b]+ops[-1].imm)&0xffffffff
        elif mn in ('str','strb','strh','strd'):
            o=ops[-1]; post=False
            if ops[-1].type==ARM_OP_IMM: o=ops[-2]; post=True
            a=addr(o) if not post else R[rid(o.mem.base)]
            if mn=='strd':
                m.w(a,4,R[rid(ops[0].reg)]); m.w(a+4,4,R[rid(ops[1].reg)])
            else:
                n={'str':4,'strb':1,'strh':2}[mn]; m.w(a,n,R[rid(ops[0].reg)])
            if i.writeback:
                b=rid(o.mem.base); R[b]=a if not post else (R[b]+ops[-1].imm)&0xffffffff
        elif mn in ('stm','stmia','stmea'):
            b=rid(ops[0].reg); a=R[b]
            for o in ops[1:]:
                m.w(a,4,R[rid(o.reg)]); a+=4
            if i.writeback: R[b]=a
        elif mn in ('ldm','ldmia'):
            b=rid(ops[0].reg); a=R[b]
            for o in ops[1:]:
                R[rid(o.reg)]=m.r(a,4); a+=4
            if i.writeback: R[b]=a
        elif mn in ('b',):
            if cond(cc): npc=ops[0].imm
        elif mn in ('cbz','cbnz'):
            v=R[rid(ops[0].reg)]
            if (v==0)==(mn=='cbz'): npc=ops[1].imm
        elif mn=='vld1':
            regs=[o for o in ops if o.type==ARM_OP_REG]; mo=[o for o in ops if o.type==ARM_OP_MEM][0]
            a=addr(mo)
            if '[' in i.op_str.split('}')[0]: raise Exception('vld1 lane')
            for k,o in enumerate(regs): vset(o.reg,bytes(m.rb(a+8*k+j) for j in range(8)))
            if i.writeback: R[rid(mo.mem.base)]=(a+8*len(regs)) if ops[-1].type!=ARM_OP_REG or len(regs)==len(ops)-1 else a
        elif mn=='vst1':
            regs=[o for o in ops if o.type==ARM_OP_REG]; mo=[o for o in ops if o.type==ARM_OP_MEM][0]
            a=addr(mo); bts=b''.join(vget(o.reg) for o in regs[:len(regs)])
            # writeback register (last reg op after mem) is the increment
            vr=[o for o in regs if md.reg_name(o.reg)[0] in 'dq']
            import re as _re
            lanes=_re.findall(r'd\d+\[(\d+)\]',i.op_str)
            if lanes:
                es=int(i.mnemonic.split('.')[1])//8
                bts=b''.join(vget(o.reg)[int(l)*es:(int(l)+1)*es] for o,l in zip(vr,lanes))
            else:
                bts=b''.join(vget(o.reg) for o in vr)
            for j,x in enumerate(bts): m.w(a+j,1,x)
            if i.writeback:
                inc=[o for o in ops[ops.index(mo)+1:] if o.type==ARM_OP_REG]
                R[rid(mo.mem.base)]=(a+(R[rid(inc[0].reg)] if inc else len(bts)))&0xffffffff
        elif mn=='vldr':
            a=addr(ops[1]); k,ix=vreg(ops[0].reg); n=4 if k=='s' else 8
            vset(ops[0].reg,bytes(m.rb(a+j) for j in range(n)))
        elif mn=='vstr':
            a=addr(ops[1]); b=vget(ops[0].reg)
            for j,x in enumerate(b): m.w(a+j,1,x)
        elif mn=='vmov':
            dt=i.mnemonic
            if len(ops)==2 and ops[1].type in (ARM_OP_IMM,ARM_OP_FP):
                k,ix=vreg(ops[0].reg)
                if '.f32' in dt:
                    vset(ops[0].reg,struct.pack('<f',ops[1].fp if ops[1].type==ARM_OP_FP else float(ops[1].imm)))
                elif '.i32' in dt:
                    n={'d':2,'q':4}[k]; vset(ops[0].reg,struct.pack('<I',ops[1].imm&0xffffffff)*n)
                else: raise Exception(i.mnemonic+' '+i.op_str)
            elif len(ops)==2 and '[' in i.op_str:
                import re as _re
                lane=int(_re.search(r'\[(\d+)\]',i.op_str).group(1))
                a,b=ops; na=md.reg_name(a.reg)
                es=4
                for t,n in (('8',1),('16',2),('32',4)):
                    if i.mnemonic.endswith(t): es=n
                if na[0] in 'dq':
                    buf=bytearray(vget(a.reg)); buf[es*lane:es*lane+es]=(R[rid(b.reg)]&((1<<(8*es))-1)).to_bytes(es,'little'); vset(a.reg,bytes(buf))
                else:
                    v=int.from_bytes(vget(b.reg)[es*lane:es*lane+es],'little')
                    if '.s' in i.mnemonic and v>>(8*es-1): v-=1<<(8*es)
                    R[rid(a.reg)]=v&0xffffffff
            elif len(ops)==2:
                a,b=ops
                na=md.reg_name(a.reg); nb=md.reg_name(b.reg)
                if na[0]=='r' or na in('sl','fp','ip','lr','sb'):
                    R[rid(a.reg)]=struct.unpack('<I',vget(b.reg))[0]
                elif nb[0]=='r' or nb in('sl','fp','ip','lr','sb'):
                    vset(a.reg,struct.pack('<I',R[rid(b.reg)]))
                else: vset(a.reg,vget(b.reg))
            elif len(ops)==3 and md.reg_name(ops[2].reg)[0]=='d':
                v=vget(ops[2].reg); R[rid(ops[0].reg)]=struct.unpack('<I',v[:4])[0]; R[rid(ops[1].reg)]=struct.unpack('<I',v[4:])[0]
            elif len(ops)==3 and md.reg_name(ops[0].reg)[0]=='d':
                vset(ops[0].reg,struct.pack('<II',R[rid(ops[1].reg)],R[rid(ops[2].reg)]))
            else: raise Exception(i.mnemonic+' '+i.op_str)
        elif mn=='vorr' and len(ops)==3:
            a=vget(ops[1].reg); b=vget(ops[2].reg); vset(ops[0].reg,bytes(x|y for x,y in zip(a,b)))
        elif mn in ('vbic','vorr') and len(ops)==2 and ops[1].type==ARM_OP_IMM:
            d=vget(ops[0].reg); es=int(i.mnemonic.split('.')[1][1:])//8
            imm=(ops[1].imm&((1<<(8*es))-1)).to_bytes(es,'little')*(len(d)//es)
            vset(ops[0].reg,bytes(((x&~y) if mn=='vbic' else (x|y))&0xff for x,y in zip(d,imm)))
        elif mn in ('vceq','vand','veor','vmvn','vbic','vorr','vorn') :
            es=int(i.mnemonic.split('.')[1][1:])//8 if '.' in i.mnemonic else 1
            d=vget(ops[1].reg) if len(ops)>1 and ops[1].type==ARM_OP_REG else vget(ops[0].reg)
            if mn=='vmvn': vset(ops[0].reg,bytes((~x)&0xff for x in d))
            else:
                o2=ops[-1]
                if o2.type==ARM_OP_IMM: b=(o2.imm&((1<<(8*es))-1)).to_bytes(es,'little')*(len(d)//es)
                else: b=vget(o2.reg)
                if mn=='vceq':
                    out=b''
                    for k in range(0,len(d),es): out+=(b'\xff'*es if d[k:k+es]==b[k:k+es] else b'\0'*es)
                    vset(ops[0].reg,out)
                else:
                    fn={'vand':lambda x,y:x&y,'veor':lambda x,y:x^y,'vbic':lambda x,y:x&~y,'vorr':lambda x,y:x|y,'vorn':lambda x,y:x|~y}[mn]
                    vset(ops[0].reg,bytes(fn(x,y)&0xff for x,y in zip(d,b)))
        elif mn=='vbsl':
            d=vget(ops[0].reg); a=vget(ops[1].reg); b=vget(ops[2].reg)
            vset(ops[0].reg,bytes((x&y)|(~x&z)&0xff for x,y,z in zip(d,a,b)))
        elif mn=='vdup':
            v=R[rid(ops[1].reg)]; k,ix=vreg(ops[0].reg); n={'d':8,'q':16}[k]
            if '.16' in i.mnemonic: vset(ops[0].reg,struct.pack('<H',v&0xffff)*(n//2))
            elif '.32' in i.mnemonic: vset(ops[0].reg,struct.pack('<I',v)*(n//4))
            else: vset(ops[0].reg,bytes([v&0xff])*n)
        elif mn=='vmovl':
            src=vget(ops[1].reg); hs=struct.unpack('<4H',src); vset(ops[0].reg,struct.pack('<4I',*hs))
        elif mn=='vshl':
            if '.s32' not in i.mnemonic and '.i32' not in i.mnemonic and '.u32' not in i.mnemonic: raise Exception('vshl '+i.mnemonic)
            src=vget(ops[1].reg); xs=struct.unpack('<%dI'%(len(src)//4),src)
            if ops[2].type==ARM_OP_IMM:
                sh=ops[2].imm; r=[(x<<sh)&0xffffffff for x in xs]
            else:
                ss=struct.unpack('<%dI'%(len(src)//4),vget(ops[2].reg)); r=[]
                for x,s in zip(xs,ss):
                    s=s&0xff; s=s-256 if s>127 else s
                    if s>=0: r.append((x<<s)&0xffffffff)
                    else:
                        sx=x-(1<<32) if (x>>31 and '.s32' in i.mnemonic) else x
                        r.append((sx>>(-s))&0xffffffff)
            vset(ops[0].reg,struct.pack('<%dI'%len(xs),*r))
        elif mn=='vneg':
            src=vget(ops[1].reg); xs=struct.unpack('<%dI'%(len(src)//4),src)
            vset(ops[0].reg,struct.pack('<%dI'%len(xs),*[(-x)&0xffffffff for x in xs]))
        elif mn=='vcvt':
            src=vget(ops[1].reg); dt=i.mnemonic
            if 's32.f32' in dt: vset(ops[0].reg,struct.pack('<i',int(struct.unpack('<f',src)[0])))
            elif 'f32.s32' in dt: vset(ops[0].reg,struct.pack('<f',float(struct.unpack('<i',src)[0])))
            else: raise Exception(dt)
        elif mn in ('vmul','vadd','vsub','vdiv','vmla','vmls'):
            f=lambda r: struct.unpack('<f',vget(r))[0]
            a=f(ops[1].reg); b=f(ops[2].reg); d=f(ops[0].reg)
            v={'vmul':a*b,'vadd':a+b,'vsub':a-b,'vdiv':a/b if b else 0,'vmla':d+a*b,'vmls':d-a*b}[mn]
            vset(ops[0].reg,struct.pack('<f',v))
        elif mn=='nop': pass
        else: raise Exception('unhandled %x %s %s'%(pc-base,i.mnemonic,i.op_str))
        pc=npc
    run.last=m
    return m.stores
if __name__=='__main__':
    sym=sys.argv[1]; flags={}
    for a in sys.argv[2:]:
        k,v=a.split('='); flags[k]=int(v)
    st=run(sym,flags)
    offs=sorted(st)
    o=0
    words={}
    for a in offs: words.setdefault(a&~3,0)
    for w in sorted(words):
        v=sum(st.get(w+j,0)<<(8*j) for j in range(4)); present=''.join('x' if w+j in st else '.' for j in range(4))
        print('%04x %s %08x %d %s'%(w,present,v,struct.unpack('<i',struct.pack('<I',v))[0],repr(struct.unpack('<f',struct.pack('<I',v))[0])))
