#!/usr/bin/env bash
# Build the decompiled engine as a NATIVE macOS arm64 dylib (mac/libgof2.dylib).
# Pipeline: compile every class+runtime .cpp for arm64-apple-macos -> dedupe symbols defined in
# multiple .o (overloads/D1-D2 dtors/agent dups: keep first) -> weak-stub all still-undefined symbols
# (agent extern names, unrecovered data, platform calls; weak so real libc/OpenGL win) -> link.
# Run on the Mac host: bash mac/build_dylib.sh
set -u; cd "$(dirname "$0")/.."
SDK=$(xcrun --show-sdk-path)
F="-target arm64-apple-macos11 -std=c++14 -O1 -fno-exceptions -fno-rtti -w -include mac/macshim.h -Wno-error -isysroot $SDK"
mkdir -p mac/obj; : > mac/arm64.log
echo "[1/4] compiling for arm64..."
compile(){ d=$(dirname "$1"); o="mac/obj/$(echo "$1"|sed 's#[/.]#_#g').o"
  clang++ $F -I"$d" -Iinclude -c "$1" -o "$o" 2>"$o.err" && echo OK || echo "FAIL $1"; }
export -f compile; export F
{ find work/classes -path '*/src/*.cpp'; find work/runtime -name '*.cpp' 2>/dev/null; } | \
  xargs -P10 -I{} bash -c 'compile "$@"' _ {} > mac/arm64.log
echo "     OK=$(grep -c '^OK' mac/arm64.log) FAIL=$(grep -c '^FAIL' mac/arm64.log)"

echo "[2/4] dedupe duplicate-symbol objects..."
python3 - <<'PY'
import re,os,subprocess
allo=subprocess.run("ls mac/obj/*.o",shell=True,capture_output=True,text=True).stdout.split()
excl=set()
for _ in range(15):
    keep=[o for o in allo if os.path.basename(o) not in excl]
    open('mac/keep.rsp','w').write("\n".join(keep))
    r=subprocess.run(["clang++","-target","arm64-apple-macos11","-dynamiclib","-undefined","dynamic_lookup",
                      "-o","/dev/null","@mac/keep.rsp"],capture_output=True,text=True)
    pairs=re.findall(r"duplicate symbol '[^']*' in:\n\s*\S+\.o\n\s*(\S+\.o)", r.stderr)
    if not pairs: break
    for p in pairs: excl.add(os.path.basename(p))
print("     excluded %d dup-definer objects"%len(excl))
PY

echo "[3/4] weak-stub undefined symbols..."
xargs nm -gjU < mac/keep.rsp 2>/dev/null | sort -u > mac/defined.txt
xargs nm -gju < mac/keep.rsp 2>/dev/null | sort -u > mac/undef.txt
comm -23 mac/undef.txt mac/defined.txt > mac/need.txt
python3 - <<'PY'
ls=[l.strip() for l in open('mac/need.txt') if l.strip()]
with open('mac/stubs.s','w') as f:
    f.write('.section __DATA,__data\n')
    for s in ls: f.write('.globl %s\n.weak_definition %s\n.p2align 3\n%s:\n.space 512\n'%(s,s,s))
print("     stubbed %d symbols"%len(ls))
PY
clang -target arm64-apple-macos11 -c mac/stubs.s -o mac/stubs.o

echo "[4/4] link libgof2.dylib..."
clang++ -target arm64-apple-macos11 -dynamiclib -undefined dynamic_lookup -framework OpenGL \
  -o mac/libgof2.dylib $(tr '\n' ' ' < mac/keep.rsp) mac/stubs.o
[ -f mac/libgof2.dylib ] && echo "=== libgof2.dylib: $(ls -la mac/libgof2.dylib|awk '{print $5}') bytes ===" || echo "LINK FAILED"
