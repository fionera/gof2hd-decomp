# Level::createShip — wave 101 (wf104): the original uses plain `new`, no try/catch

**Status:** analysis only, no source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). The committed source remains the best
measured shape: verifier **74.379345 %** (report rounds 74.4),
`usize == osize == 2764`, 1007 insns on both sides. This pass continues
`level-createship-pass10-w5006.md` and the earlier pass notes.

Gate before and after (unchanged, no code change):

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
.ARM.extab 0x26294 (-4)  lint CLEAN  drift 0  RATCHET PASS
```

## 0. What this pass establishes

The original function has **57 EH call-site entries, all `action = 0`
(cleanup only), with no catch handler and no `__cxa_begin_catch`**. The
landing pads are exactly the compiler-generated cleanups for **plain
`new` expressions** (`operator new` + constructor; on a constructor
throw, `operator delete` the raw storage). The committed source instead
uses `::operator new` + placement `new` for `PlayerFighter` /
`PlayerFixedObject` and a `try { setEmpData } catch (...) { trap }`
anchor. That anchor is a **source-shape hack**: it reproduces the
original's prologue/RA but not the original's EH structure.

A faithful plain-`new`, no-`try` reconstruction compiles and has the
**same 57-entry cleanup LSDA** as the original, but scores lower
(63.1–64.6 %). The missing piece is the scheduler: with no EH_LABEL
before `setEmpData`, the all-`int` spawn is sunk *below* the call, which
changes the Greedy-RA colouring of the whole function.

## 1. Decoding the original's EH tables (the proof)

`.ARM.exidx` entry for `0xc0214` is at file offset `0x19ff20`:

```text
raw: f4 02 f2 7f  78 b1 00 00
fn  = prel31(0x7ff202f4, 0x19ff20) = 0xc0214
tab = prel31(0x0000b178, 0x19ff20) = 0x1ab098
```

The extab entry at `0x1ab098` is:

```text
0x1ab098: 00 00 00 00              ; (padding / PRD word)
0x1ab09c: c4 3a eb 7f              ; personality prel31 -> 0x5eb60 (__gxx_personality_v0)
0x1ab0a0: c9 4d 97 02 f0 80 00 82 b0 b0 b0 ab   ; 12 B unwind opcodes
0x1ab0ac: ff ff 01 bb 02 ...       ; LSDA: LPStart=omit, TType=omit,
                                   ;       call-site enc=uleb128, table len=0x13b (315 B)
```

The 315-byte call-site table starts at `0x1ab0b1` and has **57 entries**,
every one with `action = 0` (cleanup). The landing-pad offsets are
`0x9b8, 0x9b2, 0x9ac, 0x990, 0x98e, ... 0x976`; they point into the
4-instruction cleanup block at the end of the function:

```text
c0b8a..c0bb8:  b.n c0bcc                 ; 24 trampolines, delete r5
c0bba: mov r8,r0; mov r0,sl; b c0bd0    ; delete sl  (fighter)
c0bc0: mov r8,r0; mov r0,fp; b c0bd0    ; delete fp  (volumes)
c0bc6: mov r8,r0; mov r0,r9; b c0bd0    ; delete r9  (fixed/ship)
c0bcc: mov r8,r0; mov r0,r5; b c0bd0    ; delete r5  (player / current BoundingAAB)
c0bd0: blx _ZdlPv
c0bd4: mov r0,r8
c0bd6: blx _Unwind_Resume
```

Mapping the entries to instructions proves the source form:

```text
entry  start  len   lp      covers
0      0x000  0x196 0       c0214..c03aa  (prologue .. hardCoreMode)
1      0x196  0x01e 0x9b8   c03aa..c03c8  Player ctor          -> delete r5
2      0x1b4  0x05e 0       c03c8..c0426  spawn + setEmpData   -> no landing pad
3      0x212  0x020 0x9b2   c0426..c0446  PlayerFixedObject ctor -> delete r9
4      0x232  0x008 0       c0446..c044e  operator new (Array)
5      0x23a  0x004 0x9ac   c044e..c0452  Array ctor           -> delete fp
6      0x23e  0x04e 0       ...
7      0x28c  0x01e 0x990   c04a0..c04be  BoundingAAB ctor     -> delete r5
...    (24 BoundingAAB ctor entries, all lp -> delete r5)
29     0x4c8  0x020 0x9a6   c06dc..c06fc  PlayerFighter ctor   -> delete sl
...
```

`setEmpData` is in a region with **lp = 0**, i.e. it is not inside a
try block. There is no catch-all landing pad and no `__cxa_begin_catch`
anywhere in the function. This is the signature of plain `new`, not of
`try`/`catch`.

## 2. The faithful source shape

The original source is therefore:

```cpp
Player *player = new Player(armor, hitpoints, 1, 1, 0);
int x = wz + jitterY - 20000;   // wp->x + 1st
int y = wy + jitterZ - 20000;   // wp->y + 2nd
int z = wx + jitterX - 20000;   // wp->z + 3rd
player->setEmpData(empPoints, empTime);
KIPlayer *ship = nullptr;
if (shipClass == 0) {
    PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
    ...
} else if (shipClass == 1) {
    PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
    Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
    ...
}
ship->setLevel(this);
return (PlayerFixedObject *) ship;
```

Replacing the committed source's `::operator new` + placement `new` by
plain `new` and removing the `try`/`catch` produces an object with
**exactly the same LSDA shape**: 57 entries, 315-byte call-site table,
all `action = 0`, 12-byte unwind opcodes, and the same four cleanup
targets (`r5`, `r9`, `fp`, `sl`). This is strong independent proof of
the source form.

**The committed source's spawn is not this.** It is the pass-8 semantic
hack:

```cpp
int x = jitterZ + jitterX - 20000;        // 2nd + 3rd, no waypoint term
float y = (float) (wy + jitterY - 20000); // wp->y + 1st
float z = (float) (wz + wx - 20000);      // wp->x + wp->z
```

i.e. `x` drops the waypoint-`x` term and `z` adds `wp->x` to `wp->z`.
This is what reproduces the original's stack-slot map
(`wy@48 wz@44 wx@40 j1@36 j2@32 j3@28`) and the exact prologue; it is
not semantically equivalent to the original.

## 3. Measured table (official verifier, full `link_so` + `verify.py`)

All rows are `_ZN5Level10createShipEiiiP8Waypointbb` from
`cmake-build-match/verify/report.json` after a full build. `pct` is the
official ratio; `usize` is our body size. The original is `osize=2764`.

| source shape | pct | usize | note |
|---|---:|---:|---|
| committed (`try` + placement new + mixed spawn) | **74.4** | **2764** | current local optimum |
| plain new both, no try, mixed spawn | 64.6 | 2768 | correct LSDA, 4 B long |
| plain new both, no try, all-int spawn | 63.1 | 2788 | correct LSDA, 24 B long |
| plain new PlayerFixedObject only, no try, all-int | 60.6 | 2752 | size not exact |
| plain new PlayerFixedObject only, no try, mixed | 62.7 | 2760 | |
| plain new PlayerFighter only, no try, all-int | 62.8 | 2784 | |
| no try, placement new, mixed | 70.5 | 2776 | |
| all-int spawn + committed try/placement | 61.6 | 2752 | loses prologue |

The committed source is a strict local optimum of the measured space.
The faithful plain-new shape is structurally right but loses ~10 points.

### 3.1 Semantically-correct spawn variants measured this pass

The table above mostly uses the committed (semantically wrong) spawn.
Re-measuring with the **correct** spawn
(`x = wp->x + 1st`, `y = wp->y + 2nd`, `z = wp->z + 3rd`) gives:

| source shape | pct | usize |
|---|---:|---:|
| committed (wrong spawn, `try` + placement new) | **74.4** | **2764** |
| **mixed correct** (`int x = wz+jitterY`, `float y = wy+jitterZ`, `float z = wx+jitterX`), `try` + placement | 73.6 | 2764 |
| same, statement order `zyx` / `zxy` / `xzy` | 73.8 | 2764 |
| all-`float` correct, `try` + placement | 65.0 | 2748 |
| all-`int` correct, `try` + placement | 61.6 | 2752 |
| all-`int` correct, plain new (no `try`) | 63.1 | 2788 |
| all-`float` correct, plain new (no `try`) | 58.4 | 2776 |
| mixed correct, plain new (no `try`) | 64.3 | 2768 |

A full 48-variant sweep over `{int,float}^3 x 6 statement orders` of the
correct spawn found the best result **73.8 %** (`x` int, `y`/`z` float;
order `zyx`, `zxy` or `xzy`); the natural `xyz` order is 73.6 %. So the
committed source's **0.8-point advantage is bought entirely by its wrong
spawn expressions**, not by the `try`/placement hack alone. The faithful
all-`int` shape loses 12.8 points, almost all of it in the prologue/RA
(see §4).

Spawn **placement** also matters (correct spawn, `try` + placement):
after the jitter calls 61.0, after `level` 61.1, after `hitpoints` 61.1,
after `armor` 61.6, after `new Player` 61.6 for all-`int`; for the mixed
shape the same placements are 60.2/61.3/61.3/62.7, and only
**after `new Player`** gives 73.6. Moving the spawn before `new Player`
therefore destroys the prologue match.

## 4. Why the faithful shape loses: scheduler + EH_LABEL

The original's spawn is **before** `setEmpData`:

```text
c03c8..c03e4  spawn x,y,z (all int, in fp/r4/r8)
c03e8..c03ee  setEmpData(player, empPoints, empTime)
```

The committed build's `-print-after-all` MIR shows why:

```text
bb.23:
  %244 = ADDrr ...        ; spawn y
  %245 = ADDrr ...        ; spawn z
  ...
  EH_LABEL                ; <-- try block starts here
  tBL @_ZN6Player10setEmpDataEii
```

The `EH_LABEL` is a scheduling barrier, so the float spawn stays before
the call. In the plain-new build there is no EH_LABEL before
`setEmpData`, and the all-int spawn is sunk below the call:

```text
bb.23:
  tBL @_ZN6Player10setEmpDataEii
  %250 = ADDrr ...        ; spawn z
  ...
```

That changes the live ranges of `x,y,z` across the call and the Greedy-RA
colouring of the whole function. In particular the plain-new build spills
`type` and `race` in the prologue (`str r3,[sp,#60]` / `str r1,[sp,#56]`,
`mov sl,r2`), while the original and the committed build keep
`type` in `fp`, `shipClass` in `r5`, `race` in `[sp,#60]` and `this` in
`r9`:

```text
original / committed : str r1,[sp,#60]  mov r9,r0  mov fp,r3  mov r5,r2
plain new (all-int)  : str r3,[sp,#60]  mov r9,r0  str r1,[sp,#56]  mov sl,r2
```

The committed `try`/`catch` hack accidentally supplies the EH_LABEL that
keeps the prologue byte-identical. The original gets the same prologue
without a try block; how its scheduler keeps the all-int spawn before
the call is the remaining open question.

## 5. Residuals (unchanged from pass 10)

Even with the committed source, the first difference is the `this` spill:
`str.w r9,[sp,#56]` at `0xc031c` (end of `bb.14`) instead of the
original's `0xc0332` (end of `bb.17`). Everything from `0xc03f6` to the
epilogue is the same instruction *sequence* with a different Greedy-RA
colouring:

```text
original : volumes = fp   z = r4   x = sl   y = r8   this = r6   ship = r9
committed: volumes = r9   z = sl   x = r6   y = r8   this = sl   ship = fp
```

The 11 `BoundingAAB` constructions alone account for ~90 of the 757
replaced instructions. Both residuals are downstream of the same RA
decision.

## 6. Sweeps measured this pass (all no better than 74.4)

With the committed source (full build unless noted):

| variant | pct | usize |
|---|---:|---:|
| `if` chain 0x2c,0x33,0x31 (instead of switch) | 74.4 | 2764 |
| `switch` cases reordered 0x33,0x31,0x2c | 73.8 | 2764 |
| mission check as `switch` / range form | 74.4 | 2764 |
| mission check as explicit bitmask | 71.3 | 2760 |
| `armor` as if-form / const / unsigned / bool / reversed ternary | 74.4 | 2764 |
| `hitpoints` as two statements / level-first | 74.4 | 2764 |
| `__builtin_expect` on waypoint / level / gameWon / hardCoreMode / type==14 / missionPtr / type checks / shipClass==0 | 27.5–74.4 | 2732–2768 |
| `volumes` / `fixed` / `fighter` declared at function scope | 74.4 | 2764 |
| `ship = (KIPlayer *) fighter` / `static_cast` / `kiFighter` | 74.4 | 2764 |
| `volumes` declared before `fixed` | 59.3 | 2760 |
| spawn before `new Player` (mixed, committed try) | 64.3 | 2776 |
| spawn before `new Player` (all-int, plain new) | 64.2 | 2796 |
| spawn duplicated in both branches | 54.9 | 2812 |
| spawn in a local `struct`/array (plain new, all-int) | 63.1 | 2788 |
| `try` around spawn+setEmpData (rethrow/empty) | 40.5–61.8 | 2776–2800 |
| whole-body `try`/`catch` (rethrow/empty/trap) | 35.2–36.7 | 2736–2756 |

## 7. Reproduction

Full measurement (the numbers in §3):

```sh
cd /home/debian/GalaxyOnFire2/wt/wf104
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

LSDA decode (original):

```sh
python3 - <<'PY'
import struct
data=open('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
base=0x19cc08; n=0x6f70//8
def prel31(w,addr):
    v=w & 0x7fffffff
    if v & 0x40000000: v -= 0x80000000
    return (addr+v)&0xffffffff
def uleb(p):
    r=0;s=0
    while True:
        b=data[p];p+=1;r|=(b&0x7f)<<s
        if not b&0x80: break
        s+=7
    return r,p
for i in range(n):
    off=base+i*8
    w0,w1=struct.unpack_from('<II',data,off)
    if prel31(w0,off)==0xc0214:
        ea=prel31(w1,off); break
print('extab',hex(ea),data[ea:ea+24].hex())
# personality word at ea+4 (ea has a 4-byte PRD word)
p=ea+4+4
while data[p:p+3]!=b'\xff\xff\x01': p+=1
p+=3; ln,p=uleb(p)
print('LSDA',hex(p-3),'table len',ln)
end=p+ln; i=0
while p<end:
    st,p=uleb(p); l,p=uleb(p); lp,p=uleb(p); act,p=uleb(p)
    print(i,hex(st),hex(l),hex(lp),act); i+=1
PY
```

The script prints 57 entries; every `action` is `0`, and the `lp`
offsets are the four cleanup pads listed in §1.

## 8. Conclusion / next lever

The committed source is a strict local optimum at 74.4 %. The original
source form is now known with high confidence: **plain `new` everywhere,
no `try`/`catch`**. The faithful plain-`new` shape is 63.1–64.6 % because
the all-`int` spawn is scheduled after `setEmpData` (no EH_LABEL
barrier), which changes the Greedy-RA colouring. The next lever is to
make the plain-`new` build keep the all-`int` spawn before the call
without adding a landing pad; no source shape found in this pass does
that. The committed `try`/`catch` + placement-`new` source remains the
best-scoring shape and should not be replaced by the faithful form unless
the scheduler problem is solved.

Two further facts sharpen the picture:

1. The committed spawn is **semantically wrong** (`x = j2 + j3`,
   `z = wp->x + wp->z`); the correct mixed shape is only **0.8 points**
   behind (73.6 %, `usize` still exactly 2764). So if semantic
   correctness is preferred over the last 0.8 fuzzy points, the
   `int x = wz+jitterY; float y = wy+jitterZ; float z = wx+jitterX;`
   form is the one to land: it keeps the byte-exact prologue, the exact
   stack-slot map except `wp->x` (`[sp,#32]` instead of `[sp,#44]`), and
   the correct values. It is still a hybrid shape, not the original
   all-`int` form.
2. The all-`int` faithful form is 61.6 % with the `try` hack and 63.1 %
   with plain `new`; its prologue is
   `str r2,[sp,#60]; mov r9,r3; str r0,[sp,#56]; mov sl,r1`, i.e. the
   pre-RA scheduler sinks the three adds below `setEmpData` and Greedy RA
   then colours `this`/`type`/`race` differently. This is a whole-function
   RA effect, not a local spawn effect; fixing it requires reconstructing
   the rest of the function faithfully enough that the scheduler makes
   the original choice.
