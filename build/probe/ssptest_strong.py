#!/usr/bin/env python3
# Does switching -fstack-protector -> -fstack-protector-strong (at -Oz) keep the matched set exact?
# 3 agents independently report the target uses -strong (canaries on 4-byte address-taken scalars).
# Recompile every matches.tsv source with each and count exacts. Run in the guest.
import subprocess, os, sys, re
REPO="/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"; os.chdir(REPO)
SO="../_work/bins/android_2.0.16_libgof2hdaa.so"
TC="/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64"; CXX=TC+"/bin/clang++"
BASE="-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti -Iinclude".split()
entries=[]
for ln in open("matches.tsv"):
    if ln.startswith("#") or not ln.strip(): continue
    name,vaddr,n,obj,sym,status=ln.rstrip("\n").split("\t")
    src="src/"+obj[len("build/obj/"):].rsplit(".o",1)[0]+".cpp"
    if os.path.exists(src): entries.append(dict(name=name,vaddr=vaddr,n=int(n),sym=sym,src=src,status=status))
def pct(e,flag):
    o="/tmp/sx.o"
    r=subprocess.run([CXX]+BASE+[flag,"-c",e["src"],"-o",o],capture_output=True,text=True)
    if r.returncode!=0: return None
    d=subprocess.run([sys.executable,"tools/gofdiff.py","--so",SO,"--vaddr",e["vaddr"],"--n",str(e["n"]),"--obj",o,"--sym",e["sym"]],capture_output=True,text=True)
    m=re.search(r"MATCH ([0-9.]+)%",d.stdout); return float(m.group(1)) if m else None
base_ok=strong_ok=0; regressed=[]
for e in entries:
    need=100 if e["status"]=="exact" else 90
    b=pct(e,"-fstack-protector"); s=pct(e,"-fstack-protector-strong")
    if b and b>=need: base_ok+=1
    if s and s>=need: strong_ok+=1
    if b and b>=need and (not s or s<need): regressed.append((e["name"],round(s or 0,1)))
print("of %d sources:" % len(entries))
print("  -fstack-protector        ok = %d" % base_ok)
print("  -fstack-protector-strong ok = %d" % strong_ok)
print("  regressed by -strong = %d" % len(regressed))
for nm,s in regressed[:60]: print("    %-34s strong%%=%s" % (nm,s))
