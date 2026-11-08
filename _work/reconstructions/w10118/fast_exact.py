import subprocess,re,sys
RE='/home/debian/GalaxyOnFire2/wt/w10118/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf'
def load(p):
    out=subprocess.check_output([RE,'-sW',p],text=True)
    syms={}
    for l in out.splitlines():
        q=l.split()
        if len(q)>=8 and q[3]=='FUNC' and q[6]!='UND':
            try: a=int(q[1],16); s=int(q[2],0)
            except: continue
            syms[q[7]]=(a,s)
    sec=subprocess.check_output([RE,'-SW',p],text=True)
    secs={}
    for l in sec.splitlines():
        m=re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)',l)
        if m: secs[m.group(1)]=(int(m.group(3),16),int(m.group(4),16),int(m.group(5),16),m.group(2))
    return syms,secs,open(p,'rb').read()
def bytes_at(syms,secs,data,name):
    a,s=syms[name]
    for n,(addr,off,size,typ) in secs.items():
        if typ!='NOBITS' and addr<=a and a+s<=addr+size:
            return data[off+(a-addr):off+(a-addr)+s]
    return None
o=load(sys.argv[1]); u=load(sys.argv[2])
common=set(o[0])&set(u[0])
be=le=0; n=0
for name in common:
    oa,os_=o[0][name]; ua,us=u[0][name]
    if os_==0 or us==0: continue
    n+=1
    ob=bytes_at(*o,name); ub=bytes_at(*u,name)
    if ob==ub: be+=1
    if ob is not None and ub is not None and len(ob)==len(ub): le+=1
print(f'compared {n} byte_exact {be} size_exact {le}')
