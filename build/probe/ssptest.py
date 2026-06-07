#!/usr/bin/env python3
# ssptest.py — does adding -fstack-protector to the global flags help or hurt? Recompile every
# matches.tsv source at -Oz (baseline) and -Oz -fstack-protector, and count how many stay exact.
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
    entries.append(dict(name=name,vaddr=vaddr,n=int(n),sym=sym,src=src,
                        hand=("__stack_chk_guard" in (open(src).read() if os.path.exists(src) else ""))))

def pct(src,sym,vaddr,n,extra):
    o="/tmp/ssp.o"
    r=subprocess.run([CXX]+BASE+extra+["-c",src,"-o",o],capture_output=True,text=True)
    if r.returncode!=0: return None
    d=subprocess.run([sys.executable,"tools/gofdiff.py","--so",SO,"--vaddr",vaddr,"--n",str(n),"--obj",o,"--sym",sym],capture_output=True,text=True)
    m=re.search(r"MATCH ([0-9.]+)%",d.stdout); return float(m.group(1)) if m else None

base_exact=ssp_exact=0; regressed=[]; hand_n=0
for e in entries:
    if not os.path.exists(e["src"]): continue
    if e["hand"]: hand_n+=1
    b=pct(e["src"],e["sym"],e["vaddr"],e["n"],[])
    s=pct(e["src"],e["sym"],e["vaddr"],e["n"],["-fstack-protector"])
    if b and b>=100: base_exact+=1
    if s and s>=100: ssp_exact+=1
    if b and b>=100 and (not s or s<100): regressed.append((e["name"],e["hand"],round(s or 0,1)))

print("of %d sources:" % len(entries))
print("  -Oz                 exact = %d" % base_exact)
print("  -Oz -fstack-protector exact = %d" % ssp_exact)
print("  hand-written-canary sources = %d" % hand_n)
print("  regressed by adding -fstack-protector = %d" % len(regressed))
for nm,h,s in regressed[:40]:
    print("    %-34s hand=%s  ssp%%=%s" % (nm,h,s))
