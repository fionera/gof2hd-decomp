# Layout::Layout mid-block `vstr` (`_ZN6LayoutC1Ev`) — wave 110 / worker w5802

Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`, original size **7072 B**, source
`src/game/ui/Layout.cpp`. Baseline at start: `check` = RATCHET PASS, byte_exact
**4500**, linked_exact **4500**, avg 99.92, data 843/843, imports 207, lint
CLEAN, drift 0. Unrounded baseline score **48.269001193792285 %** (report
rounds to 48.3 %).

## Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5802
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5802/fscore.py _ZN6LayoutC1Ev         # unrounded score
```

`fscore.py` is the same disassembly/`SequenceMatcher` pipeline as
`tools/verify/verify.py`, but prints the unrounded ratio. It is reproduced at
the bottom of this note.

## Proven binary shape (original, non-HD path)

Raw Thumb (`0xd1996..0xd19de`):

```
d1996 str   r6,[r0,#148]      ; 0x94
d199a str   r1,[r0,#152]      ; 0x98
d199e vcvt.s32.f32 s0,s0
d19a8 vstr  s0,[r0,#156]      ; 0x9c
d19ca vmov  s0,r2
d19ce str   r2,[r0,#172]      ; 0xac  <- int phi in a general register
d19d6 vcvt.f32.s32 s0,s0
d19da vcvt.s32.f32 s0,s0
d19de vstr  s0,[r0,#168]      ; 0xa8  <- (int)(float)v round trip
```

So the original shares **one int `v`** between `0xac` (stored as int) and
`0xa8` (`(int)(float)v`), and the value lives in a general register (`r2`),
converted through `s0`.  The iPadHD path constant-folds both to `0x55` (85).
Ghidra/REA (`decomp.c` lines 405–431) agrees:

```c
*(undefined4 *)(this + 0xac) = uVar13;
fVar33 = (float)VectorSignedToFloat(uVar13, ...);
*(int *)(this + 0xa8) = (int)fVar33;
```

The original prologue saves `{r6,r7,r8,r9,sl,fp}`; the current master body
saves `{r5,r6,r7,r8,r9,sl,fp}`.

## Measured variants (all real `link_so` builds)

| variant | size | unrounded score | notes |
|---|---:|---:|---|
| current master body (landable) | **7072** | **48.269001193792285** | baseline |
| `int v` (int arms, 85), `0x98` int arms, `0x98` first | 6992 | 48.291025384769135 | faithful int phi |
| `float v`, `0xac=(int)v`, `0xa8=(int)v`, `0x98` int arms | 7008 | 48.302037554934080 | |
| same + `0x98` float arms (`420*1.40625f`) | 7020 | 48.352964663605505 | best score seen |
| 6800 all-ternary body | 6800 | 50.788515972503035 | unlandable |
| 6800 body + int phi | 6808 | 50.050535678188800 | |
| base + `(int)(float)(int)` on `0x6c`+`0x204` | 7088 | 48.270377733598409 | +16 B |
| base + `(int)(float)(int)` on `0x6c`+`0x204`, uncast `0x138`+`0x194` | 7072 | 48.269001193792285 | ties base |
| move `0x94` into the block (remove line 296) | 7064 | 45.126569663145304 | |
| move `0x94` into the block + `0x6c` cast | 7072 | 45.228133094241876 | |

The target-block disassembly of the 6992 variant is essentially the original:

```
vmov s0, r1
cmp.w fp, #0
vcvt.f32.s32 s0, s0
vcvt.s32.f32 s0, s0
str.w r1, [r0, #172]     ; 0xac
vstr s0, [r0, #168]      ; 0xa8
```

versus original

```
vmov s0, r2
str.w r2, [r0, #172]
cmp.w fp, #0
vcvt.f32.s32 s0, s0
vcvt.s32.f32 s0, s0
vstr s0, [r0, #168]
```

Only the store/`cmp` order and the register number differ.

## Why the faithful variants are unlandable

The gate is size-sensitive.  Any Layout size change shifts `.text` and
collapses `byte_exact`:

```
6992 B -> verify: byte 2454  linked 4500  -> RATCHET FAIL (byte_exact 4500 -> 2454)
```

Only **7072 B** is landable.  The faithful int-phi shape is 80 B *shorter*
than the original, because the original source has an outer
`if (Globals::iPadHD) { ... } else { ... }` structure (Ghidra `decomp.c`
line 68/313) that duplicates the branch code, while the flat-ternary
reconstruction merges the branches.  The original has 2529 instructions; the
6992 variant has 2474 (55 fewer, all in the non-HD path).

## Search performed (all 7072-B results <= baseline)

* exhaustive adjacent-statement swaps over 205 units (handoff, re-verified);
* ~400 single-cast additions (`(int)(float)(int)` on every `this->X = ...`
  line) — none at 7072 beats 48.269001; many are byte-neutral and score-neutral;
* all pairs of cast removals from the 7088 variant that net -16 B — best is
  48.269001 (tie);
* moving `field_0x94` to every assignment position in the ctor — best is the
  current position (296), 48.269001;
* reordering the `0x16c/0x170/0x174/0x178/0x17c` block — compiler reorders
  independent stores, score unchanged;
* 400 hill-climbing iterations from the base with move/cast/uncast/swap
  mutations — no improvement at 7072.

## Proven vs hypothesis

**Proven:** the original mid-block is `int v = <int ternary>; field_0xac = v;
field_0xa8 = (int)(float)v;`, with `v` in a general register and the
int→float→int conversion through `s0`; the iPadHD arm is 85.  The current
master body instead computes `0xac` and `v` with two separate float ternaries
and stores `0xa8` from a float `v`.

**Hypothesis:** the remaining 80 B gap is the original's outer
`if (Globals::iPadHD)` source structure (duplicated HD/non-HD blocks), not the
mid-block expression itself.  Reconstructing that nested structure is the next
step; it is a large edit and was not attempted here.

**Not landed:** no 7072-B variant with a score strictly above
48.269001193792285 was found.  `src/game/ui/Layout.cpp` is unchanged.

## `fscore.py`

```python
#!/usr/bin/env python3
import sys, difflib
sys.path.insert(0, "/home/debian/GalaxyOnFire2/wt/w5802/tools/verify")
import verify
from elf import Elf32
REPO="/home/debian/GalaxyOnFire2/wt/w5802"
ORIG="/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so"
OURS=REPO+"/cmake-build-match/libgof2hdaa.so"
OBJ=REPO+"/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump"
def get(name):
    of=Elf32(ORIG);oe=Elf32(OURS);orig=of.functions();ours=oe.functions();common=set(orig)&set(ours)
    def ar(e):
        m=[s for s in e.sections if s["addr"]];lo=next((s["addr"] for s in m if s["name"]==".plt"),min(s["addr"] for s in m));return lo,max(s["addr"]+s["size"] for s in m)
    orr=verify.Resolver(orig,common,of.plt_map(),ar(of));our=verify.Resolver(ours,common,oe.plt_map(),ar(oe))
    co={o[2] for o in of.objects()}&{o[2] for o in oe.objects()}
    for r,e in ((orr,of),(our,oe)): r.attach_data(e,co)
    imp=({v for k,v in of.got_map().values() if k=="sym"}&{v for k,v in oe.got_map().values() if k=="sym"});orr.imports=our.imports=imp
    o,u=orig[name],ours[name]
    _,on=verify.disassemble(OBJ,of.bytes_at(o["value"],o["size"]),o["thumb"],o["value"],orr)
    _,un=verify.disassemble(OBJ,oe.bytes_at(u["value"],u["size"]),u["thumb"],u["value"],our)
    on,un=verify.reconcile(on,un)
    pct=100.0 if on==un else difflib.SequenceMatcher(None,on,un,autojunk=False).ratio()*100
    return o["size"],u["size"],pct
for name in sys.argv[1:] or ["_ZN6LayoutC1Ev"]:
    os_,us,p=get(name); print(f"{name}: orig {os_}B ours {us}B score={p:.15f}")
```
