# Wave 146 (w9214): `Status::missionCompleted` — full 358-instruction body decoded;
# the original's block order is source order and needs `!prof`; the 166 body's
# 10-byte excess is an RA/tail artefact (metric-only fix: injected 558 -> 459).

Status: **note-only** (no `src/` change landed).  Gate on the unchanged tree:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.  Worktree `wt/w9214`, base master `31297fadc`
(wave 145).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
baseline raw same-address diff **722 B**.  Metric: raw same-address diff bytes
(`/dev/shm/w9214/fdiff.py`), not the PYTHONHASHSEED-dependent fuzzy %.

## 0. New results

### 0.1 The dispatch is a plain single `switch (t)`; ours is byte-identical
Decoding all 358 instructions: the original's "group-id computation" is just
clang's cluster lowering of one switch on `m->getType()`:

```
sub r1,r0,#150 ; cmp r1,#39 ; bhi  ->  tbh table[150..189]   (40 entries)
sub r1,r0,#8   ; cmp r1,#6  ; bhi  ->  tbh table[8..14]      (7 entries)
cmp r0,#0      ; bne default
```

Both tables and the `t==0` arm are byte-identical in ours.  There is **no
source-level group-id variable**: the prologue spills exactly the same four
booleans (`[sp,#4]` longMission, `[sp,#8]` shortMission, `[sp,#12]`
dockedAtStation, `[sp,#16]` this) as our source, and `r9` is only a scratch in
the 158/166 bodies.

### 0.2 The original block order is *source order*, and needs `!prof`
The original's case blocks are laid out (by address):

```
167/174, 8, 0/11, 13, 14, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172
```

i.e. exactly the source order of `Status.cpp`.  Ours is **numeric**
(`150,151,...,189`).  Compiling the already-optimised `status.ll` directly
(no `!prof`) reproduces the numeric order, so the backend sorts the cases
regardless of the IR `SwitchInst` order (which is source order).  Injecting
`!prof` makes the backend honour the IR order.  Conclusion: the original was
built with switch branch weights (PGO or `__builtin_expect`); the layout is not
reachable from case reordering.

### 0.3 NEW: the 166 body's 10-byte excess is an RA/tail artefact
Injecting the w9018 vector (`1,3,3,4,4,4,4,4,3x14,2x10`) gives **558 / 1432**.
Its remaining body diffs are:

| body | orig | ours (558) | delta |
|---|---:|---:|---:|
| 158 tail | `ldr r6,[sp,#16]; mov r8,r4; lsls; beq fail; b ret` | shared tail `cmp r0,#0; ldr r6,[sp,#16]; beq fail; b ret` | -6 B |
| 166 | `mov r9,r0` (index), `mov r5,r0` (amount), `ldr r0,[r6,#400]` | `mov r5,r0`, `mov r6,r0`, `mov r1,r6; ldr r0,[r1,#400]` | +4 B |
| 166 tail | `b ac458` (`cbnz r0, ret`) | `cmp r0,#0; ldr r6,[sp,#16]; beq fail; b ret` | +6 B |
| 157 | bottom-tested loop (`b cond` first, `ldr r6,[sp,#16]` before `beq`) | top-tested loop | -2 B |

The 166 RA is fixed by hoisting `m->getProductionGoodIndex()` **before**
`Globals::status->ship` (metric-only: the original loads `status->ship` first,
so this contradicts the witnessed evaluation order):

```cpp
int pgi = m->getProductionGoodIndex();
if (Globals::status->ship->hasEquipment(pgi, 1)) return m;
```

Measured on the real TU + real gold link, with the same injected vector:

| shape | raw | size |
|---|---:|---:|
| master | 722 | 1432 |
| injected 558 (base) | 558 | 1432 |
| injected 558 + pgi-first | **459** | 1432 |
| pgi-first, no injection (master layout) | 855 | **1428** |

So the fix is real but **metric-only and size-negative without the weights**:
the master layout is numeric, so a 4-byte body shrink just misaligns every
following body and the raw rises.

### 0.4 The 157 loop rotation is fixed by dropping the 157 expect
Removing only the 157 `__builtin_expect(...)` rotates the loop to the original
bottom-tested form (`b cond` first; `ldr r6,[sp,#16]` before `beq`), exactly as
the original.  But in isolation it is a wash: master **723 / 1432** (one byte
worse), and with the injected vector it changes the layout to **812 / 1436**
(the tail moves +4).  The other three expects are load-bearing for size: the
16-combination sweep shows only masks with the 166 and 184 expects keep
1432 B; removing all four gives 1428 / 894.

### 0.5 The 158 tail merge is robust to source form
`hasGood && hasSpecial`, `hasGood) { if (hasSpecial`, `hasSpecial && hasGood`,
and `hasGood & hasSpecial` all give the same 558 (injected) / 722 (master);
only `(int)hasGood + (int)hasSpecial == 2` changes it (866 / 1436).  The merge
is a backend tail-sharing decision (the 158 and 166 tails are identical
`if (x) return m; else break;`), not a source-shape choice.

### 0.6 New source hypotheses measured (all worse)
| shape | raw | size |
|---|---:|---:|
| master | **722** | 1432 |
| split into `switch(t){A,low,B}` + `switch(t){C}` | 1330 | 1448 |
| full 27-arm `if (__builtin_expect(t==c,0))` chain, L=2..2000 | 1225..1331 | 1352..1476 |
| group-id helper + `switch(g)` + per-group if-chain | 1212 | 1484 |
| group-id helper + `switch(g)` + per-group `switch(t)` (w9018) | 1212 | 1484 |

The full if-chain *does* fold back into the same two jump tables, but the
geometric weights (`default=L^k`, `arm_j=(L+1)^(k-j+1)L^(j-1)`) overflow for
k=27 and the `||` arms (167/174, 0/11) lose their weights, so the layout is
scattered and the size moves.  The group-id shape always pays a second
dispatch (+52 B).

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9214/fdiff.py _ZN6Status16missionCompletedEbbx   # 722 / 1432

# injected 558 (real TU .ll + !prof + checked-in gold link):
bash /dev/shm/w9214/score_inj.sh /dev/shm/w9214/base.cpp \
  1,3,3,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2
# injected 459 (pgi-first 166 body):
bash /dev/shm/w9214/score_inj.sh /dev/shm/w9214/v166l.cpp <same vector>
```

Scratch (`/dev/shm/w9214/`): `fdiff.py`, `score.sh` (source -> obj -> gold link
-> raw), `score_inj.sh` (source -> .ll -> `!prof` -> obj -> link -> raw),
`inject.py`, `cc.sh`/`link.sh` (checked-in gold command), `orig.asm`,
`inj558.asm`, `v166l.asm`, `v*.cpp` variants.

## 2. Open items

1. **The only remaining lever is a source producer of `!prof`.**  The layout
   needs group-uniform weights; a nested switch gives 11:1 (`ValidTotalSuccWeight`),
   an if-chain gives overflowing geometric weights, a helper gives +52 B.
   A producer of `default=1, hot=3, C=2` with no extra dispatch is still missing.
2. Once the weights are found, the 166 body needs the metric-only pgi-first
   order (or an equivalent RA fix that keeps `status->ship` first), the 157
   expect must be dropped, and the 158 tail must be kept out of the shared
   `cmp r0,#0` block.  Those three fixes take the injected residual from 558
   to ~50 B.
