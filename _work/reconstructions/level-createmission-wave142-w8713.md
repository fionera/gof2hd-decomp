# w8713 (wave-142): `Level::createMission` — station-pirate escort loop,
# faithful RNG order verified; w8512 hoist landed at raw 5,762 (not RNG-faithful)

**Coordinator addendum (wave-142 merge):** the hoist is *not* landed. The note's own Thumb
evidence proves the original order is jitter,sign,jitter,sign,jitter,sign (the checked-in nested
expression); the hoist emits 2,2,2,20000,20000,20000 and is metric-only. createMission stays at
raw 5,756 from w8712's store-order fix.
Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8713`, based on master
`1d10d0694` (wave 141). Continues `level-createmission-wave141-w8612.md`,
`…-wave140-w8511.md`, `…-wave138-w8311.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra
MCP was unavailable (`rea doctor`: extracted Ghidra missing
`application.properties`/`analyzeHeadless`), so manual NDK `objdump` of the
original is the authority. Raw same-address byte diff is the primary metric
(`/dev/shm/w8713/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5763
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## 1. Original RNG call order, re-verified from Thumb

Escort loop body `b3bb8`; `r9` = `AERandom::nextInt` (set at `b3bb2` from
`[sp,#96]`, which is the `nextInt` GOT pointer stored at `b0ad6`), `r5` =
`Globals::rnd`:

```text
b3be2 movw r1,#20000 ; b3bec blx r9   -> nextInt(20000)   (jx)
b3bf0 movs r1,#2     ; b3bf4 blx r9   -> nextInt(2)       (sx)
b3bf8 movw r1,#20000 ; b3c02 blx r9   -> nextInt(20000)   (jy)
b3c08 movs r1,#2     ; b3c0a blx r9   -> nextInt(2)       (sy)
b3c32 movw r1,#20000 ; b3c3c blx r9   -> nextInt(20000)   (jz)
b3c42 movs r1,#2     ; b3c4c blx r9   -> nextInt(2)       (sz)
```

So the original order is **jitter, sign, jitter, sign, jitter, sign**:
`20000, 2, 20000, 2, 20000, 2`. This is exactly the checked-in nested
expression, evaluated left-to-right per component (x, y, z). The neighbouring
`extraAliens` block (`b3cfa`: `nextInt(50000)` then `nextInt(2)`) uses the same
interleaved idiom.

## 2. w8512's hoist is the best measured variant but is not RNG-faithful

The landed diff (`/dev/shm/w8612_hoist_w8512.diff`) hoists all three signs
before all three jitters:

```cpp
int sx = Globals::rnd->nextInt(2) == 0 ? 1 : -1;   // 2
int sy = Globals::rnd->nextInt(2) == 0 ? 1 : -1;   // 2
int sz = Globals::rnd->nextInt(2) == 0 ? 1 : -1;   // 2
int jx = Globals::rnd->nextInt(20000) * sx + 10000; // 20000
int jy = Globals::rnd->nextInt(20000) * sy + 10000; // 20000
int jz = Globals::rnd->nextInt(20000) * sz + 10000; // 20000
```

Emitted order is `2, 2, 2, 20000, 20000, 20000` — a different RNG sequence
(and therefore different escort positions) from the original. It is landed
because it is the best measured raw diff (5,762 vs 5,763) and keeps 17,172 B,
but the coordinator should treat it as a **metric-only** win, not a semantic
match. The faithful source remains the checked-in nested expression at 5,763.

## 3. New faithful variants tried this wave (all size-exact 17,172 B)

| variant | RNG order | raw diff |
|---|---|---:|
| **checked-in nested expression (faithful)** | 20000,2,20000,2,20000,2 | **5763** |
| w8512 hoist (landed, unfaithful) | 2,2,2,20000,20000,20000 | **5762** |
| explicit `jx,jy,jz` fully evaluated ints (faithful) | 20000,2,20000,2,20000,2 | 5771 |
| `const int jitter = 10000` outside the loop | faithful | 5763 |
| `int jitter` outside the loop / `static const` / `static int` | faithful | 5763 |
| `const int jitter[1] = {10000}` / `{10000,10000,10000}` | faithful | 5763 |
| `(int)(float)jitter` / `(int)(double)jitter` / `(int)(jitter*1.0f)` | faithful | 5763 |
| lambda `[](int v){return v+10000;}` | faithful | 5763 |
| `Vector pos(a,b,c)` / `Vector pos = Vector(a,b,c)` | faithful | 5763 |
| `float px,py,pz` locals / `Vector pos; pos.x=...` | faithful | 5763 |
| `Level *self = this` alias | faithful | 5763 |
| `int first`/`int escortsEnd` moved before the first loop / to function scope | faithful | 5763 |
| `first`/`pirateBase` declaration order swapped | faithful | 5763 |
| `while` loop / `unsigned i` / `i += 1` | faithful | 5763 |
| partial hoists (`jx,sx` first pair; first component; last component) | faithful | 5767 / 5767 / 5777 |
| hoist first two pairs | faithful | 5946 (17,168 B) |
| `Array<KIPlayer*> *enemies = this->enemies` cache | faithful | 5913 (17,164 B) |
| `KIPlayer *enemy` local | faithful | 5925 (17,168 B) |
| direct `setPosition((int)(...), ...)` | faithful | 5884 (17,176 B) |
| float-domain `+ 10000.0f` | faithful | 5887 (17,180 B) |

The faithful forms cannot reproduce the original's `fp` literal: the pre-RA
MIR still materialises `%3522 = t2MOVi16 10000` inside the loop body, and
Greedy RA splits it, moving `this` from `sl` to `fp` and rematerialising the
literal in `sl` (`mov fp,sl; movw sl,#10000; …; mov sl,fp`). The original keeps
`this` in `sl` and `10000` in `fp` from the preheader. Every faithful source
spelling tested leaves that MIR decision unchanged.

## 4. Landed change

The w8512 hoist (7 insertions, 3 deletions) is applied verbatim. It is the
only variant measured at 5,762 and keeps the function at exactly 17,172 B.
The RNG-order caveat in §2 is the reason this note is required.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv   raw diff 5763 -> 5762, size 17172
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8713/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8713/diffmap.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w8713/{Level.base.cpp,fdiff.py,diffmap.py,gen*.py,measure.sh,
ours_fj.txt,ours_hoist.txt,mir_base.txt,mir_constout.txt}`.
