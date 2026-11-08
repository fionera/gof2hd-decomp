# w9310 (wave 146): `Level::createMission` — alien-gun loop register map
# (`this` r5 vs r4): root cause is the sret-temp hoist, not the anchor

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9310`, based on master
`9c9744ca5` (wave 146). Continues `level-createmission-wave146-w9208.md`,
`…-wave145b-w9014.md`, `…-wave146-w9209.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w9310/fdiff.py`); instruction-index alignment is
`_work/reconstructions/align_diff.py`.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5560   align 728
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Alien-gun loop register map, original vs ours

The loop is `0x32c2..0x33bc` (function-relative) in the original; ours is
`0x32b4..0x33be` (14 B earlier because the preceding code is shorter).

| value | original | ours (HEAD) |
|---|---|---|
| `this` | **r4** (spilled at `[sp,#124]`, reloaded `b336e`) | **r5** (kept live) |
| `gun` | **r6** | **r4** |
| `i` | **fp** | **r4** (spilled `[sp,#116]`) |
| `k*24` | r9 | r9 |
| `t` (table) | **sl** | **r6** |
| sret temp `sp+212` | **r5** (hoisted in preheader, `b328a add r5,sp,#212`) | **fp** (rematerialised in body, `b32be add.w fp,sp,#212`) |
| `&rotation` / `&v` | r4 (reused after `this` is spilled) | sl |
| `alienGuns` | sl (reloaded at loop end, `b3386`) | r8 (loaded mid-body, `b3328`) |

Key original instructions:

```text
b3232  ldr r4,[sp,#124]        ; this, before the alienGuns test
b3274  ldr.w r0,[r4,#248]      ; this->enemies (getPosition)
b3286  add.w r2,r4,#352        ; this+352, after the call
b328a  add r5,sp,#212          ; sret temp HOISTED into r5
b328c  mov.w r9,#0             ; k
b3294  add.w fp,r3,r2          ; i in fp
b329a  mov r0,r4               ; body: this still r4
b32bc  ldr.w r6,[r0,fp,lsl#2]  ; gun in r6
b32da  add r4,sp,#312          ; r4 reused for &rotation
b336e  ldr r4,[sp,#124]        ; this reloaded at body end
```

Ours:

```text
b3240  ldr r4,[sp,#124]        ; this, inside the alienGuns block
b3260  ldr.w r0,[r4,#248]      ; getPosition
b3274  add.w r2,r4,#352        ; this+352
b3284  ldr r5,[sp,#124]        ; this MOVED to r5
b3288  adds r4,r3,r2           ; i takes r4
b328c  mov r0,r5               ; body: this r5
b32ae  ldr.w r4,[r0,r1,lsl#2]  ; gun takes r4
b32be  add.w fp,sp,#212        ; sret temp rematerialised into fp
```

## 2. Root cause: the sret temp is hoisted in the original, not in ours

The greedy allocator (dump via `-mllvm -print-after-all`) assigns the loop
intervals in this order: `i` (loop phi) gets r4, then `this` gets r5, then
`gun` gets r4 (after `i` is spilled), then `t` gets r6, then the sret temp
gets fp, then `&rotation`/`&v` get sl.

In the original the sret temp (`sp+212`, the hidden return slot of
`MatrixRotateVector` and the argument slot of `setPosition`) is **hoisted into
r5 in the preheader**. r5 is therefore occupied, `fp` is free, and `i` goes
to `fp`; r4 stays `this`; `gun` goes to r6; `t` goes to sl. The chain is

```text
temp=r5  ->  i=fp  ->  this=r4  ->  gun=r6  ->  t=sl
```

In ours the temp is rematerialised inside the body (`add.w fp,sp,#212`), so
r5 is free, `this` moves to r5, `i` takes r4, `gun` reuses r4, `t` takes r6,
and `&rotation`/`&v` take sl. The `(i < 0)` anchor is **not** the cause: every
anchor form tried leaves `this` in r5 (see §3).

The `i` spill (`[sp,#116]`, 4 instructions / 8 B) is a consequence, not a
cause. The original has no `i` spill.

## 3. Variants measured (all 17,172 B unless noted)

| variant | size | raw | align | alien map (this,i,gun,t) |
|---|---:|---:|---:|---|
| **HEAD (landed)** | **17172** | **5560** | **728** | r5, r4, r4, r6 |
| setScaling after setPosition (`Dlast`) | 17172 | 5630 | **723** | r5, **fp**, r4, r6 |
| `Dlast` + decl `v M k b` (w9014 order) | 17172 | 5627 | 730 | r5, fp, r4, r6 |
| `Dlast` + cargo/shipGroup swapped | 17172 | 5630 | — | r5, fp, r4, r6 |
| `Dlast` + `setMaxHitpoints` after select | 17188 | 6480 | — | r5, fp, r4, r6 |
| `Dlast` + `t` declared outside loop | 17172 | 5630 | — | r5, fp, r4, r6 |
| `Dlast` + `t` via if/else | 17172 | 5630 | — | r5, fp, r4, r6 |
| `Dlast` + `t` alias | 17172 | 5630 | — | r5, fp, r4, r6 |
| `Dlast` + `__builtin_expect(...,0/1)` | 17172 | 5630 | — | r5, fp, r4, r6 |
| `Dlast` + `(this == nullptr)` anchor | 17172 | 5630 | — | r5, fp, r4, r6 |
| `Dlast` + `(gun == nullptr)` anchor | 17204 | 6552 | — | r5, fp, r4, r6 |
| `Dlast` + `(base.x == 12345)` anchor | 17212 | 6569 | — | r5, fp, r4, r6 |
| `Dlast` + `(this->field_164)` anchor | 17196 | 6527 | — | r5, fp, r4, r6 |
| `Dlast` + `(this->enemies == nullptr)` | 17188 | 6479 | — | r5, fp, r4, r6 |
| `Dlast` + `unsigned i` | 17172 | 5630 | — | r5, fp, r4, r6 |
| `Dlast` + `k = i - first` | 17188 | 6311 | — | sl, r9, r4, — |
| `Dlast`, decl `M V B K` (v before base) | 17172 | 5629 | — | r5, fp, r4, r6 |
| `Dlast`, decl `V M B K` / `V M K B` | 17172 | 5627 | — | r5, fp, r4, r6 |
| `Dlast`, decl `B M V K` / `B V K M` | 17172 | 5632 | — | r5, fp, r4, r6 |
| `Dlast`, decl `K M B V` | 17172 | 5630 | — | r5, fp, r4, r6 |
| `p` temp (w9014) on current body | 17172 | 5649 | 736 | r5, r4, r4, r6 |
| `p` temp on `Dlast` body | 17188 | 6482 | — | r5, fp, r4, r6 |
| `Vector tmp` outside, both rotates + setPosition | 17308 | 16584 | — | fp, r4, —, — |
| `Vector tmp` outside, setPosition only | 17308 | 16581 | — | sl, fp, —, — |
| no anchor (plain `bigAlienWave ?`) | 17140 | 6587 | 1395 | r5, r4, —, — |
| `(i & 0x80000000)` anchor | 17172 | 5656 | 731 | r5, r4, r4, r6 |
| `(i >> 16)` / `(i >> 30)` anchor | 17172 | 5560 | 728 | r5, r4, r4, r6 |
| `(i & 0xffff0000)` anchor | 17180 | 5803 | — | r5, r4, r4, r6 |
| `(i / 65536)` anchor | 17188 | 6483 | — | r9, r4, r4, r6 |
| `k & 0x40000000` anchor | 17172 | 6474 | 1209 | r8, r5, r4, r6 |
| volatile `bigAlienWave` | 17148 | 12936 | 1105 | r5, r9, r4, r6 |
| volatile select read | 17140 | 12979 | 1106 | r5, r4, r4, r6 |
| `PlayerTurret *gun` instead of `KIPlayer *` | 17172 | 5560 | 728 | r5, r4, r4, r6 |
| `Level *self = this` alias | 17172 | 5560 | 728 | r5, r4, r4, r6 |

Notes:
* `Dlast` (setScaling after setPosition) is the only variant that moves `i`
  to `fp`, matching the original. It also matches the original's witnessed
  call order (`setPosition` `b3364`, `setScaling` `b336a`, `cargo` `b3372`,
  `shipGroup` `b337e`). It is an **align win (728 -> 723) but a raw loss
  (5560 -> 5630)**; not landed because raw is the primary metric.
* `(i >> 16)` and `(i >> 30)` are raw-identical to HEAD (clang folds them to
  the same `asr #31` form).
* The `t` register is tied to the **conditional-move** select (`movne r6,r0`);
  the original's `cbz` branch produces `t` in `sl`. Every source spelling of
  the select tried still if-converts to a cmov.
* `__builtin_expect` (allowed source, per w9214) does not change the map.

## 4. Why the temp is not hoisted in ours

In the IR the sret slot is `%stack.9` (fi#9); the address is
`t2ADDri %stack.9, 0` and the allocator rematerialises it in the body. The
original hoists the same address into r5 in the preheader. The hoist is a
greedy-allocator rematerialisation choice, not a source-order effect: moving
`v`/`base`/`k` declarations, the `setScaling` call, `cargo`/`shipGroup`, the
`setMaxHitpoints` call, or the table select all leave the temp in `fp`/r9.

The remaining structural difference (original `this=r4, gun=r6, i=fp,
t=sl, temp=r5` vs ours `this=r5, gun=r4, i=r4, t=r6, temp=fp`) is therefore a
single allocator decision: **hoist the sret temp into r5**. Once that is
achieved the rest of the chain follows, and the w9014 candidate (b)
(`fortressGuns` in `fp`, `ldr.w fp,[sp,#40]`, +4 B in the raid prologue) should
become reachable.

## 5. Gate counts (unchanged; no source landed)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv raw diff 5560 -> 5560, align 728 -> 728, size 17172
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9310/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9310/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9310/{fdiff.py,align_diff.py,regmap.py,sweep.py,
sweep2.py,sweep3.py,sweep4.py,sweep5.py,sweep6.py,sweep7.py,sweep8.py,
sweep9.py,sweep10.py,Level.base.cpp,patch.py,measure.sh,afterall.log}`.

## Open questions for the next wave

1. Force the sret temp (`sp+212`) into r5 in the preheader. Candidate source
   shapes: a named `Vector` temp whose address is taken before the loop and
   reused for both rotates and `setPosition` without growing the frame (the
   `Vector tmp` attempts grew it to 17,308 B); or a `Vector &` alias.
2. Once `this=r4`, re-test w9014 candidate (b): `fortressGuns` in `fp`
   (`ldr.w fp,[sp,#40]`, +4 B) and `Globals::status` in r8
   (`ldr.w r8,[sp,#108]`) in the raid prologue.
3. The select's if-conversion (`movne`) vs the original's `cbz` branch is the
   reason `t` is r6 rather than sl; a branch-producing source shape (or PGO
   branch weights, per w9214) may be needed.
