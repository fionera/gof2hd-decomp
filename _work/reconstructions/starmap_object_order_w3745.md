# Section-order removal: StarMap object order (w3745)

Status: research note + landed source move, 2026-11-02. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `2bd4fdbc` (the tip after the
`ArrayAdd<RadioMessage*>` pass). Every number below was measured in this
worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 / gold 1.12.

This note follows `_work/reconstructions/section_order_removal_w3745.md` §7.
That section left the plain no-order frontier at `Array<AbstractGun*>` inside
`Level.cpp`; the intervening passes (`a0bb3b1d`, `9d2d5fe5`, `8263270c`) moved
it through `Level` to `StarMap`. This pass converges the `StarMap.cpp` object.

## TL;DR

* Baseline at this HEAD: **byte_exact 4497 / linked_exact 4497**, `.text`
  `0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift `0`. (The brief's 4496 is stale;
  the current baseline is 4497/4497, as recorded in the parent note.)
* Plain no-order baseline: **byte_exact 2992 / linked 4497**, **3193 / 4524**
  exported functions at their original address, exact-address prefix **2377**.
  First mismatch: `StarMap::~StarMap()` original `0xc749c`, ours `0xc7cbc`.
* Cause: clang emits `StarMap::initStarSystem()` before `StarMap::~StarMap()`.
  `StarMap::init()` calls `initStarSystem()`; `init` is pulled forward by the
  constructor's trailing `init(...)` call, and its body is scanned at its
  definition (which precedes the destructor definition). The original binary
  and the iOS 1.1.19 symbol dump both have `init`, `~StarMap`, `initStarSystem`.
* Fix: move the `StarMap::~StarMap()` definition above `StarMap::init()` in
  `src/game/world/StarMap.cpp`. This is a pure move (41 lines moved, no body
  change).
* Gate after the move: **byte 4497 / linked 4497**, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift `0`. Plain no-order after: **byte_exact 2995 /
  linked 4497**, **3196 / 4524** addresses, prefix **2408**.
* The whole `StarMap` block is now address-exact in the plain no-order link
  (`0xc67a4..0xcba2c`), including the three weak copies that `StarMap.o` is the
  first object to define. The next plain no-order mismatch is
  `RecordHandler::readRecord` (`0xcd25c` original, `0xce858` ours), a different
  TU.

## 1. Baseline and commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section fingerprint of the gate `.so`:

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78   (original 0x26298; known -0xc delta)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order measurement (same helper as the parent note):

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/star/noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/star/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/star/noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/star/noorder.so
```

```text
compared 4524  avg 99.9%  byte_exact 2992  linked_exact 4497  fuzzy_perfect 4497
/tmp/w3745/star/noorder.so: 3193/4524 same; prefix 2377;
  first (816284, '_ZN7StarMapD1Ev', 818364)
```

`816284 = 0xc749c` (original `StarMap::~StarMap`), `818364 = 0xc7cbc` (ours).

## 2. Original StarMap order

`nm -nDS --defined-only` on the original, restricted to the StarMap block:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nDS --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | awk '$1>="000c6700" && $1<="000cb000"'
```

```text
000c67a4 000003fc T _ZN7StarMapC1EbP7Missionbi
000c67a4 000003fc T _ZN7StarMapC2EbP7Missionbi
000c6ba0 0000001c W _ZN5ArrayIPN11AbyssEngine6AEMath6VectorEEC2Ev
000c6bbc 00000034 W _Z14ArraySetLengthIPN11AbyssEngine6AEMath6VectorEEvjR5ArrayIT_E
000c6bf0 000008ac T _ZN7StarMap4initEbP7Missionbi
000c749c 000000e0 T _ZN7StarMapD1Ev
000c749c 000000e0 T _ZN7StarMapD2Ev
000c7580 0000081c T _ZN7StarMap14initStarSystemEv
000c7d9c 0000003c T _ZN7StarMap10initLightsEv
000c7dd8 00000034 T _ZN7StarMap8setStartEii
000c7e0c 00000234 T _ZN7StarMap6departEb
000c8040 0000000c T _ZN7StarMap14isInPlanetModeEv
000c804c 0000000a T _ZN7StarMap14setJumpMapModeEbb
000c8056 00000002 T _ZN7StarMap8renderBGEv
000c8058 00000090 T _ZN7StarMap6renderEv
000c80e8 0000026c T _ZN7StarMap7drawKeyEv
000c8354 00000da0 T _ZN7StarMap16drawOnScreenInfoEib
000c90f4 00000a54 T _ZN7StarMap4drawEv
000c9b48 00000006 T _ZN7StarMap14missionChangedEv
000c9b50 0000003c T _ZN7StarMap24askForJumpIntoAlienWorldEv
000c9b90 00000c3c T _ZN7StarMap6updateEi
000ca7cc 00000012 W _Z12ArrayReleaseIbEvR5ArrayIT_E
000ca7e0 0000032c T _ZN7StarMap12OnTouchBeginEii
000cab0c 00000384 T _ZN7StarMap11OnTouchMoveEii
000cae90 00000b9c T _ZN7StarMap10OnTouchEndEii
```

The strong-function order is exactly the source order
`ctor, init, ~StarMap, initStarSystem, initLights, setStart, depart, ...`.
The two weak copies `Array<AEMath::Vector*>::Array()` and
`ArraySetLength<AEMath::Vector*>` sit between the ctor and `init`, and
`ArrayRelease<bool>` sits between `update` and `OnTouchBegin`.

The iOS 1.1.19 layout dump (`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios`, `Classes/Game/StarMap.cpp`) lists the same strong
sequence:

```text
_StarMap::StarMap(bool, Mission*, bool, int)
_StarMap::init(bool, Mission*, bool, int)
_StarMap::StarMap(bool, Mission*, bool, int)
_StarMap::~StarMap()
_StarMap::~StarMap()
_StarMap::initStarSystem()
_void ArrayReleaseClasses<Station*>(Array<Station*>&)
_StarMap::initLights()
...
```

That is, `init` before `~StarMap` before `initStarSystem`, with
`ArrayReleaseClasses<Station*>` (emitted from `initStarSystem`'s body)
immediately after `initStarSystem`.

## 3. Object order before the fix

Dump the object's `.text.*` section-header order and map each name to its
position in `cmake-build-match/section_order.txt` (the original-address
witness):

```sh
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/StarMap.cpp.obj \
  | awk '$2 ~ /^\.text\./ {print $2}' | sed 's/^\.text\.//'
```

Before (only the first 16 sections shown; position = target rank in
`section_order.txt`):

```text
obj#  target#  section
  0    2484   StarMap::StarMap
  1    1547   Array<AEGeometry*>::Array()
  2    2485   Array<AEMath::Vector*>::Array()
  3    2201   ArraySetLength<AEGeometry*>
  4    2486   ArraySetLength<AEMath::Vector*>
  5    2487   StarMap::init
  6    2490   StarMap::initStarSystem     <-- emitted too early
  7    2244   ArrayReleaseClasses<Station*>
  8    2245   Array<Station*>::~Array()
  9    2237   Array<Station*>::Array()
 10    2489   StarMap::~StarMap            <-- emitted too late
 11    1643   ArrayReleaseClasses<AEMath::Vector*>
 12    1644   Array<AEMath::Vector*>::~Array()
 ...
```

The plain no-order link places the surviving sections in object order, so
`initStarSystem` lands at `0xc74a0` and `~StarMap` at `0xc7cbc` instead of
`0xc7580` / `0xc749c`.

## 4. The fix

Pure move in `src/game/world/StarMap.cpp`: cut the `StarMap::~StarMap()`
definition (41 lines) from after `StarMap::init()` and paste it immediately
before `StarMap::init()`.

```diff
@@ after the StarMap_* static tables
+StarMap::~StarMap() {
+    ... unchanged body ...
+}
+
 void StarMap::init(bool jumpMapMode, Mission *mission, bool param3, int param4) {
@@ after StarMap::init()
-StarMap::~StarMap() {
-    ... unchanged body ...
-}
-
 void StarMap::initStarSystem() {
```

Why this is the right position: clang emits functions in first-textual-reference
order, and a pulled-forward function's body is scanned at its definition
(parent note §8.3). The constructor's last statement is `init(...)`, so `init`
is pulled forward and emitted second. If `~StarMap` is defined before `init`,
its definition is emitted third; then `init`'s body is scanned at its
definition (after `~StarMap`) and pulls `initStarSystem` fourth. That gives
`ctor, init, ~StarMap, initStarSystem`, exactly the original order. Keeping
`~StarMap` after `init` makes `init`'s body scan pull `initStarSystem` before
the destructor definition, giving `ctor, init, initStarSystem, ~StarMap`.

After the move the object order is:

```text
obj#  target#  section
  0    2484   StarMap::StarMap
  1    1547   Array<AEGeometry*>::Array()
  2    2485   Array<AEMath::Vector*>::Array()
  3    2201   ArraySetLength<AEGeometry*>
  4    2486   ArraySetLength<AEMath::Vector*>
  5    2487   StarMap::init
  6    2489   StarMap::~StarMap
  7    1643   ArrayReleaseClasses<AEMath::Vector*>
  8    1644   Array<AEMath::Vector*>::~Array()
  9    2490   StarMap::initStarSystem
 10    2244   ArrayReleaseClasses<Station*>
 ...
```

The low target ranks (`1547`, `2201`, `1643`, `1644`, `2244`, ...) belong to
weak copies that an earlier object also defines; gold keeps the first
definition, so `StarMap.o`'s copies are discarded in the plain link. Restricting
to the 23 sections that `StarMap.o` is the first object to define (the strong
`StarMap::*` functions plus `Array<AEMath::Vector*>::Array()`,
`ArraySetLength<AEMath::Vector*>`, and `ArrayRelease<bool>`), the target ranks
are strictly increasing:

```text
2484, 2485, 2486, 2487, 2489, 2490, 2491, 2492, 2493, 2494, 2495,
2496, 2497, 2498, 2499, 2500, 2501, 2502, 2503, 2504, 2505, 2506, 2507
```

Before the move this list had exactly one inversion (`2490, 2489`); after the
move it has zero. The first-definer check is reproducible with:

```sh
python3 - <<'PY'
import struct, subprocess, os
NM='.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
objs=[l.strip() for l in open('cmake-build-match/exidx_markers/link_order.rsp') if l.strip()]
first={}
for i,p in enumerate(objs):
    r=subprocess.run([NM,'--defined-only',p],capture_output=True,text=True)
    for ln in r.stdout.splitlines():
        parts=ln.split()
        if len(parts)==3 and parts[2] not in first:
            first[parts[2]]=i
obj='cmake-build-match/CMakeFiles/gof2.dir/src/game/world/StarMap.cpp.obj'
data=open(obj,'rb').read()
shoff,=struct.unpack_from('<I',data,0x20)
shentsize,shnum,shstrndx=struct.unpack_from('<HHH',data,0x2e)
hdrs=[struct.unpack_from('<IIIIIIIIII',data,shoff+i*shentsize) for i in range(shnum)]
stroff=hdrs[shstrndx][4]
def nm(name):
    e=data.index(b'\0',stroff+name); return data[stroff+name:e].decode()
secs=[]
for h in hdrs:
    name,typ,flags,addr,off,size,link,info,align,entsize=h
    n=nm(name)
    if typ==1 and (flags&0x4) and n.startswith('.text.'):
        secs.append(n)
order=[l.strip() for l in open('cmake-build-match/section_order.txt') if l.strip()]
pos={n:i for i,n in enumerate(order)}
prev=-1; inv=0
for i,s in enumerate(secs):
    sym=s[len('.text.'):]
    if first.get(sym)==103:          # StarMap.cpp.obj is o103
        p=pos.get(s,-1)
        if p<=prev: inv+=1
        print(f'{i:3d} {p:5d} {s}')
        prev=max(prev,p)
print('inversions among surviving:',inv)
PY
```

## 5. Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| variant | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline (`~StarMap` after `init`) | **4497 / 4497** | **2992** | 3193 / 4524 | 2377 | `StarMap::~StarMap` `0xc749c` vs `0xc7cbc` |
| `~StarMap` moved before `init` | **4497 / 4497** | **2995** | 3196 / 4524 | 2408 | `RecordHandler::readRecord` `0xcd25c` vs `0xce858` |

Delta: **+3 byte_exact**, **+3 exact addresses**, **+31 exact-address prefix**,
gate unchanged. The gate `.text` size, `.rodata`/`.data` exactness (843/843),
sodiff allowlist (0), imports (207), lint, and drift are all unchanged. No
baseline update was made.

After the move the entire StarMap block is address-exact in the plain link:

```text
000c67a4 000003fc T _ZN7StarMapC1EbP7Missionbi
000c67a4 000003fc T _ZN7StarMapC2EbP7Missionbi
000c6ba0 0000001c W _ZN5ArrayIPN11AbyssEngine6AEMath6VectorEEC2Ev
000c6bbc 00000034 W _Z14ArraySetLengthIPN11AbyssEngine6AEMath6VectorEEvjR5ArrayIT_E
000c6bf0 000008ac T _ZN7StarMap4initEbP7Missionbi
000c749c 000000e0 T _ZN7StarMapD1Ev
000c749c 000000e0 T _ZN7StarMapD2Ev
000c7580 0000081c T _ZN7StarMap14initStarSystemEv
000c7d9c 0000003c T _ZN7StarMap10initLightsEv
...
000ca7cc 00000012 W _Z12ArrayReleaseIbEvR5ArrayIT_E
000ca7e0 0000032c T _ZN7StarMap12OnTouchBeginEii
000cab0c 00000384 T _ZN7StarMap11OnTouchMoveEii
000cae90 00000b9c T _ZN7StarMap10OnTouchEndEii
```

The three weak copies that `StarMap.o` is the first object to define land at
their original addresses (`0xc6ba0`, `0xc6bbc`, `0xca7cc`). The other weak
copies in the object (`Array<AEGeometry*>`, `Array<Station*>`, `Array<bool>`,
`Array<int>`, `String(int)`, ...) are first defined by earlier objects
(`LODManager.o`, `RepairBeam.o`, `Status.o`, `GameText.o`, `String.o`,
`CutScene.o`) and are discarded from `StarMap.o` in the plain link.

## 6. Reordered-all control

Re-measured the parent note's control (permute every object's own `.text.*`
sections into `section_order.txt` order, then relink without the ordering
file) with the landed source:

```sh
mkdir -p /tmp/w3745/star/reordered2
python3 - <<'PY'
import os, subprocess
src = 'cmake-build-match/exidx_markers/link_order.rsp'
lines = [l.strip() for l in open(src) if l.strip()]
out = []
for i, p in enumerate(lines):
    dst = f'/tmp/w3745/star/reordered2/{i:03d}_{os.path.basename(p)}'
    subprocess.run(['python3',
                    '_work/reconstructions/w3696/reorder_elf_sections.py',
                    p, dst, 'cmake-build-match/section_order.txt'], check=True)
    out.append(dst)
open('/tmp/w3745/star/reordered2/link_order.rsp', 'w').write('\n'.join(out) + '\n')
PY
python3 _work/reconstructions/w3696/relink_noorder.py \
  /tmp/w3745/star/reordered2/noorder.so /tmp/w3745/star/reordered2/link_order.rsp
# verify.py + addr_match.py as in §1
```

```text
compared 4524  avg 99.9%  byte_exact 4497  linked_exact 4497  fuzzy_perfect 4497
4524/4524 same; prefix 4524; first None
```

So the per-object section permutation still reproduces the ordering-file gate
exactly after the move.

## 7. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0.
* The plain no-order baseline is `byte 2992 / linked 4497`, 3193/4524 exact
  addresses, prefix 2377; the first mismatch is `StarMap::~StarMap`
  (`0xc749c` vs `0xc7cbc`), with `initStarSystem` at `0xc74a0` instead of
  `0xc7580`.
* The original address order and the iOS 1.1.19 symbol dump both put
  `init`, `~StarMap`, `initStarSystem` in that order.
* Moving the `~StarMap` definition above `init` is gate-neutral
  (`4497/4497`, lint `CLEAN`, drift 0) and improves the plain no-order build
  to `2995 / 4497`, 3196/4524 addresses, prefix 2408.
* After the move, the 23 sections that `StarMap.o` is the first object to
  define have zero inversions relative to `section_order.txt`, and the whole
  StarMap block `0xc67a4..0xcba2c` is address-exact in the plain link.
* The next plain no-order mismatch is `RecordHandler::readRecord`
  (`0xcd25c` vs `0xce858`), not a StarMap symbol.
* The reordered-all control is still `4497/4497`, 4524/4524.

**Hypothesis / open**

* The `RecordHandler` frontier is a separate TU problem: `readRecord` (a
  4-byte function) is emitted late in our object, so `loadResolutionValue`
  starts at `0xcd25c` (4 bytes before its original `0xcd260`) and the rest of
  the block shifts. It was not investigated here.
* No claim is made that the ordering file can be dropped now; the plain link is
  still `2995`, and the parent note's weak-copy TU-membership blockers remain.
