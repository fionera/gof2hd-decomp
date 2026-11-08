import os,sys,re,time,difflib
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
import profgen_w10514 as H
from profgen_w10514 import Prof, write_profile, merge, compile_use, obj_path, obj_text_sections, PREFIX_OFF, ORIG, OBJDUMP, text_bytes, sym, disasm

_cache={}
def get(tu,mangle):
    k=(tu,mangle)
    if k not in _cache: _cache[k]=Prof(tu,mangle)
    return _cache[k]

def measure_obj(p,obj):
    # find .text.<mangle> in object
    secs=obj_text_sections(obj)
    key='.text.'+p.mangle
    if key not in secs:
        # maybe .text.<mangle>.1 etc
        cand=[n for n in secs if n.startswith(key)]
        if not cand: raise SystemExit('no section '+key+' in '+str(list(secs)))
        key=cand[0]
    y=secs[key]
    a=sym(ORIG,p.mangle)
    x=text_bytes(ORIG,*a)
    n=min(len(x),len(y))
    raw=sum(q!=r for q,r in zip(x[:n],y[:n]))
    # disasm from object section: need vma/offset. Use objdump -d on object limited to section
    out=H.run([OBJDUMP,'-d','--no-show-raw-insn','--section='+key,obj]).stdout
    rows=[]
    for ln in out.split('\n'):
        m=re.match(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$',ln)
        if m: rows.append((int(m.group(1),16),m.group(2)+' '+m.group(3).strip()))
    oi=[(r[0],r[1]) for r in disasm(ORIG,*a)]
    ui=rows
    oidx={ad:i for i,(ad,_) in enumerate(oi)}; uidx={ad:i for i,(ad,_) in enumerate(ui)}
    def norm(t,idx):
        def rep(m):
            ad=int(m.group(1),16); return 'I%d'%idx[ad] if ad in idx else 'X'
        t=re.sub(r'\b([0-9a-f]{4,8})\b',rep,t); t=re.sub(r'<[^>]*>','',t)
        return re.sub(r'\s+',' ',t).strip()
    ot=[norm(t,oidx) for _,t in oi]; ut=[norm(t,uidx) for _,t in ui]
    sm=difflib.SequenceMatcher(None,ot,ut,autojunk=False)
    aligned=sum(max(i2-i1,j2-j1) for tag,i1,i2,j1,j2 in sm.get_opcodes() if tag!='equal')
    return dict(raw=raw,aligned=aligned,size=len(y))

def build(p,vals,tag,extra=(),measure_only=True):
    txt=os.path.join(H.SCRATCH,tag+'.proftext'); dat=os.path.join(H.SCRATCH,tag+'.profdata')
    write_profile(p.mangle,p.meta['fhash'],p.meta['ncounters'],vals,txt)
    merge(txt,dat)
    obj=os.path.join(H.SCRATCH,tag+'.obj')
    compile_use(p.meta['cmd'],dat,obj,PREFIX_OFF+list(extra))
    return measure_obj(p,obj), obj

def compile_plain(p,extra,tag):
    m=re.search(r'-o (\S+)',p.meta['cmd'])
    obj=os.path.join(H.SCRATCH,tag+'.obj')
    c=p.meta['cmd'].replace(m.group(1),obj)
    c=c.replace(' -c ',' '+' '.join(extra)+' -c ',1)
    H.run(c,cwd=H.BUILD)
    return measure_obj(p,obj),obj

def build_plain_link(p,extra,tag):
    m=re.search(r'-o (\S+)',p.meta['cmd'])
    obj=os.path.join(H.SCRATCH,tag+'.obj')
    c=p.meta['cmd'].replace(m.group(1),obj)
    c=c.replace(' -c ',' '+' '.join(extra)+' -c ',1)
    H.run(c,cwd=H.BUILD)
    so=os.path.join(H.SCRATCH,tag+'.so')
    H.link_with([(H.obj_path(p.tu),obj)],so)
    return H.measure(so,p.mangle)
