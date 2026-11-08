# w9208 (wave-146): `Level::createMission` — the 17,168 B (-4) tree composes;
# the +4 B companion is dropping the tail `rnd` local; extab record anatomy

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9208`, based on master
`31297fadc` (wave 145). Continues `level-createmission-wave145-w9013.md`,
`…-wave145-w9014.md`, `…-wave144-w8912.md`, `…-wave143-w8913.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w9208/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5592
.text 85,651   .ARM.extab 1,370   .rel.plt 0   .note.gnu.build-id 20
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. The w9014 17,168 B experiment reproduces exactly

The wave-144 tree (`basefirst` + `cachedgun` + `status`) rebuilt verbatim from
`/dev/shm/w9014/Level.cur17168.cpp` measures **17168 B / raw 5512** — identical
to w9014. The tree's 5512 is *not* directly comparable to 5592 because the
4-byte deficit phase-shifts the whole tail (region `0x3994..` alone is 1817
diff vs pristine's 1980).

The `-4` is caused by **`cachedgun` alone** (cached `gun` for `setScaling`,
`setRotation` before it, `cargo` before `shipGroup`):

| variant (on pristine) | size | raw |
|---|---:|---:|
| pristine | 17172 | 5592 |
| `basefirst` | 17172 | 5589 |
| `cachedgun` | **17168** | 5542 |
| `status` | 17172 | 5617 |
| `basefirst`+`cachedgun` | 17168 | 5541 |
| tree = `basefirst`+`cachedgun`+`status` | 17168 | 5512 |

## 2. The +4 B companion: dropping the tail `AERandom *rnd` local

`tree + (remove R)` = **17172 B / raw 5560** (R = the `AERandom *rnd =
Globals::rnd;` local before the `case4Tail` loop, added in wave 143 w8812).
This is a genuine, faithful composition: the two edits are in disjoint
regions (alien loop vs `case4Tail`), and removing R does **not** change the
RNG call order. The original still caches `Globals::rnd` in a register
(`b4168 ldr.w sl,[r0]`); with `cachedgun`'s changed register pressure clang
now CSEs `Globals::rnd` into `r8` on its own (`b4162 ldr.w r8,[r0]`), so the
explicit local is no longer needed. It is therefore not a metric trick.

Region map of the landed build (function-relative offsets; original addresses):

| region | bytes | raw diff (pristine -> landed) |
|---|---:|---:|
| `0x0000..0x32c2` pre-alien | 12994 | 2586 -> 2587 |
| `0x32c2..0x33bc` alien-gun loop | 250 | 233 -> 215 |
| `0x33bc..0x3426` alien tail | 106 | 100 -> 17 |
| `0x3426..0x3504` pirate-fortress loop | 222 | 198 -> 196 |
| `0x3504..0x377a` pirate tail | 630 | 15 -> 196 |
| `0x377a..0x3994` raid block | 538 | 480 -> 484 |
| `0x3994..0x4314` tail | 2432 | 1980 -> 1865 |
| **total** | | **5592 -> 5560** |

The `0x3504..0x377a` (pirate tail) regression is the price of the exact-size
tail alignment; it is more than paid for by `0x33bc` (100 -> 17) and `0x3994`
(1980 -> 1865).

### Other +4 candidates measured (all worse than 5560)

| variant | size | raw |
|---|---:|---:|
| tree + remove R (landed) | **17172** | **5560** |
| tree + `alien_scale_reload` | 17172 | 5617 |
| tree + `vol_pad_raid` | 17172 | 5684 |
| tree + `alien_scale_reload` + `vol_pad_raid` | 17172 | 5611 |
| tree + remove R + `threshold` (METRIC-ONLY) | 17168 | 5278 |
| tree + remove R + `threshold` + `alien_scale_reload` | 17172 | 5537 |
| tree + remove R + `AERandom *rnd` (re-add R) | 17168 | 6480 |

`tree + remove R + threshold` is the best raw overall (5278) but is 4 B short
*and* METRIC-ONLY: `int threshold = hard ? 40 : 20;` before
`rnd->nextInt(100)` contradicts the original's post-`nextInt` order
(`b3796..b37a0`), and `hard` dies before the call so clang emits
`cmp r0,#0` instead of the original's `cmp.w r8,#0` + `mov r8,r0` (+4 B).
Adding `alien_scale_reload` (which is *unfaithful*: the original uses the
cached `gun` for `setScaling`, w8813) recovers 17172 at 5537, but that is a
METRIC-ONLY pairing and is not landed.

### Candidate (b) from w9014 is unreachable

The w9014 note's second candidate — force `fortressGuns`/`status` into high
registers in the raid prologue (`ldr.w fp,[sp,#40]`, `ldr.w r8,[sp,#108]`,
+4 B) — is blocked by the same alien-loop register map: the original holds
`this` in `r4`, so `fortressGuns` goes to `fp`; ours holds `this` in `r5`
(the `(i<0)` anchor's cost), so `fortressGuns` fits in `r4` (`ldr r4,[sp,#48]`,
2 B). No raid-block source reordering (7 permutations) or `Status *const`
changed the size. Fixing it needs the alien-loop `this`/`gun` swap first.

## 3. `.ARM.extab` record (reclen 956, exidx fn 0xaffd8)

Record at `.ARM.extab` file offset `0x1a9ac4` (next record `0x1a9e80`).
Decoded with `_work/reconstructions/extab_lsda_decode.py`:

```text
generic personality 0x5eb60, 3 unwind words, lpstart omit, ttype omit,
call-site enc uleb128, 148 rows (identical row count in orig and ours)
```

* Pristine: **158** diff bytes; landed tree: **159** (+1).
* All 148 rows have identical `(start, length, action)` except **12 rows**
  whose *length* differs — these are real code-size differences between
  call sites, not encoding noise:

| row | start | orig len | ours len |
|---|---:|---:|---:|
| 37 | 5056 | 250 | 252 |
| 49 | 6038 | 556 | 552 |
| 53 | 6628 | 168 | 170 |
| 119 | 11710 | 240 | 232 |
| 121 | 11970 | 264 | 258 |
| 125 | 12394 | 1768 | 1782 |
| 129 | 14196 | 974 | 970 |
| 131 | 15178 | 674 | 678 |
| 133 | 15860 | 190 | 194 |
| 141 | 16524 | 130 | 124 |
| 145 | 16756 | 142 | 138 |
| 147 | 16912 | 260 | 266 |

* The remaining rows differ only in the landing-pad ULEB (`lp`), by
  `-18/-10/-8/-6/-4/+4` bytes. The landing pads are the `b.n` cleanup
  trampolines at the very end of `.text` (`~0xb42a0`), so their offsets are a
  pure downstream function of the tail code, not reachable independently.
* Row 37 is the case-183 convoy loop (`0xb1398..`): our block layout is
  2 B longer (the `case2`/`case1` body swap + the default/join split).
  Row 49 is the `case-183` post-loop region.

**Conclusion:** the extab record cannot be reduced below the body's layout
mismatch. The row *count* (148) and the encode (`uleb128`) already match; the
residual is 12 code-length deltas plus the tail landing-pad offsets. The +1
vs pristine is the `cachedgun` change's tail shift; it is not gated.

## 4. Landed change

```cpp
// alien-gun loop (was HEAD's reload + wrong order):
Vector base = ...getPosition();        // base before v
Vector v = {0,0,0};
...
const float (*t)[6] = (bigAlienWave + (i < 0)) ? ... ;
v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
gun->geometry->setRotation(v);
((PlayerTurret *) gun)->setScaling(6.0f);   // cached gun
v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
gun->setPosition(...);
gun->cargo = nullptr;                  // cargo before shipGroup
gun->shipGroup = localRace;

// raid block: cache Status
Status *status = Globals::status;

// case4Tail: drop the explicit `AERandom *rnd` local (clang CSEs it)
```

## 5. Gate counts (before -> after)

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
.text diff      85,651 -> 85,619   (-32)
.ARM.extab      1,370  -> 1,371    (+1, not gated)
.rel.plt        0      -> 0
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw diff 5592 -> 5560, size 17172
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9208/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9208/rdiff.py _ZN5Level13createMissionEv 0x0-0x32c2 0x32c2-0x33bc ...
python3 _work/reconstructions/extab_lsda_decode.py <orig> <ours>
```

Scratch: `/dev/shm/w9208/{fdiff.py,rdiff.py,secdiff.py,dis.py,align.py,
firstshift.py,shiftmap.py,mkvar.py,measure.sh,raidsweep.py,raidsweep2.py,
sweep3.py,Level.pristine.cpp,Level.tree_noR.cpp,extab_orig.bin,extab_ours.bin}`.

## Open questions for the next wave

1. Row 37 (case-183 loop, +2 B) is a self-contained 250 B block; fixing its
   `case2`/`case1` body swap would drop both a `.text` hunk and one extab row.
2. The alien loop is still 262 B vs the original's 250 B: the `i` spill
   (`str r4,[sp,#116]` / `ldr r1,[sp,#116]`, +4) plus the `(i<0)` anchor's
   `cmp.w/it/movne` (20 B vs the original's 22 B `cbz` diamond) and the
   `this`/`gun` register swap. Recovering `this=r4, gun=r6, i=fp` would also
   unlock the w9014 candidate (b) (+4 B in the raid prologue).
