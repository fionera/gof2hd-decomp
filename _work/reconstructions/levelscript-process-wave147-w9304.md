# LevelScript::process — wave 147 / w9304: `__builtin_expect` / `!prof` on the
# case-154/case-157 mission tests and the campaign-switch scrutinee

Status: **note-only, no source win** (source restored pristine).  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9304`, base master `9c9744ca5` (wave 146).  Scratch in
`/dev/shm/w9304/` (not tracked).

```
                         baseline (HEAD)   this pass
process raw diffbytes    54,869            54,869   (source pristine)
orig / ours size         57232 / 57232     57232 / 57232
align_diff TOTAL         15,737            15,737
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

Metric note: `/dev/shm/fdiff.py` in the shared scratch points at the *master*
worktree (`gof2-decomp/cmake-build-match`), not this worktree.  All numbers
below were measured with a local copy of the same logic against
`wt/w9304/cmake-build-match/libgof2hdaa.so` (`/dev/shm/w9304/harness.py`).

## 0. New results

### 0.1 clang 7 DOES emit switch branch weights from `__builtin_expect` on the scrutinee

Minimal reproducer compiled with the exact match flags:

```cpp
int g(int m) { switch (__builtin_expect(m, 154)) {
    case 154: return f(1); case 157: return f(2); case 158: return f(3); default: return 0; } }
```

IR:

```
switch i32 %0, label %7 [ i32 154, label %4  i32 157, label %2  i32 158, label %3 ], !prof !5
!5 = !{!"branch_weights", i32 1, i32 2000, i32 1, i32 1}
```

So the scrutinee form produces `branch_weights` = `default=1, case(154)=2000,
every other case=1`.  It *does* change the real function's layout (size moves
off 57232), so the mechanism is live.

### 0.2 The switch case-body order is already IDENTICAL between original and ours

Parsing the linear dispatch chain (`cmp r0,#N; beq.w <target>`) in both
binaries gives the **same address order** of case bodies:

```
158, 29, 41, 80, 56, 125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16
```

So the switch case order is not a layout lever: the residual is per-case body
*size* and the global cold-block ordering.  Per-case body start deltas
(`ours - orig`):

| case | orig | ours | delta |
|---|---:|---:|---:|
| 29 | 0x139ca2 | 0x139a64 | -574 |
| 41 | 0x139d56 | 0x139b34 | -546 |
| 80 | 0x139e34 | 0x139c32 | -514 |
| 56 | 0x139f14 | 0x139d16 | -510 |
| 125 | 0x139f9c | 0x139d98 | -516 |
| 92 | 0x139ffe | 0x13a152 | +340 |
| 157 | 0x13a308 | 0x13a46c | +356 |
| 40 | 0x13a4d8 | 0x13a63a | +354 |
| 102 | 0x13a658 | 0x13a794 | +316 |
| 154 | 0x13a76e | 0x13a81a | +172 |
| 24 | 0x13a930 | 0x13a8fa | -54 |
| 14 | 0x13a9b2 | 0x13a98e | -36 |
| 21 | 0x13ab24 | 0x13aad2 | -82 |
| 42 | 0x13abaa | 0x13ab58 | -82 |
| 91 | 0x13ac42 | 0x13abf4 | -78 |
| 94 | 0x13acac | 0x13ac66 | -70 |
| 105 | 0x13aea4 | 0x13ae90 | -20 |
| 114 | 0x13afde | 0x13af92 | -76 |
| 16 | 0x13b014 | 0x13afc8 | -76 |

The biggest single body-size delta is **case 125**: original `0x139f9c..0x139ffe`
= 98 B (it ends `b.w 0x13b518`, outlining its nested `switch (m_nState)`);
ours is 954 B because the nested switch is kept inline.  Case 154 is also
inverted: the original keeps the state-0 body inline (`cmp r0,#0; bne <tbh>`
at `0x13a76e`) and outlines states 1..10 to the `tbh` at `0x13e8e8`; ours
baseline outlines state 0 (`cmp r0,#0; beq 0x13c654`) and keeps the states
1..10 `tbh` inline at `0x13a81a`.

### 0.3 `__builtin_expect` on the case-154 state-0 test fixes the *structure* but not the metric

`if (__builtin_expect(m_nState == 0, 1) && ...)` in `case 154` makes the
state-0 body the fall-through again, exactly like the original:

```
13a83c: ldr.w r0,[fp,#28] ; cmp r0,#0 ; bne.w 13c79c   ; state 0 inline, states 1..10 -> tbh
```

but the function grows to **57324 B** and raw rises to **55191**.  The body
start moves to `0x13a83c` (further from the original's `0x13a76e`) because the
total size grew.  `expect(...,0)` gives 57324 / 55353.

### 0.4 `__builtin_expect` on the case-157 state-0 test is size-exact but worse

`if (__builtin_expect(m_nState == 0, 1))` (the `m_pGeometry4 = new AEGeometry`
block) is the only size-exact variant found: **57232 B**, raw **55225**,
align_diff **17801** (baseline 15737).  `expect(...,0)` gives 57216 / 55401.

### 0.5 Case-102 state-0 expect: worse

`if (__builtin_expect(m_nState == 0, 1) && ...)` in `case 102`: 57388 / 55177.
`expect(...,0)`: 57220 / 55434.

### 0.6 Switch-scrutinee sweep (all 31 case values, size / raw)

`switch (__builtin_expect(Globals::status->getCurrentCampaignMission(), V))`:

```
V    size   raw        V    size   raw        V    size   raw
14   57228  55210      56   57348  55597      125  57144  55446
16   56976  55334      64   57772  55482      131  57688  55524
21   57216  55219      67   57076  55341      135  57336  55391
24   57200  55551      69   57176  55296      139  57728  55609
29   57100  55397      70   57168  55286      142  57252  55370
40   57360  55312      73   57388  55151      144  57820  55598
41   57304  55490      80   57312  55674      145  57320  55472
42   57224  55518      91   57184  55455      154  57412  55510
                    92   57256  55054      157  57268  55479
                                          158  57664  55479
```

Baseline is 57232 / **54869**; every value is worse and none is size-exact.
The best raw is `V=92` (55054) but at 57256 B.  So the crude one-hot weight
that `__builtin_expect` can express is not the original's profile.

### 0.7 `!prof` injection diagnostics (confounded by the `.ll` round-trip)

Injecting `!prof` directly into the optimised `LevelScript.ll` (w9214 method):

* the campaign-switch `branch_weights` (uniform, source-order, and one-hot
  variants) do **not** change the case-body layout at all — the switch is
  lowered through a compare chain/jump table whose block order is not
  weight-driven;
* `!prof` on the case-157 `isRunningHDonWeakDevice` branch changes the function
  (57240 -> 57224 B in the round-trip build) but not toward the original.

Caveat: recompiling the emitted `.ll` is **not** byte-equivalent to compiling
the `.cpp` (the round-trip base is `0x137e60 / 57240 B`, raw 55297, vs the
direct compile `0x137d18 / 57232 B`, raw 54869), so the injection deltas are
indicative only.

## 1. Conclusion

The `!prof` mechanism is confirmed to exist for `__builtin_expect` on the
switch scrutinee, but it is not the lever that moves `LevelScript::process`
toward the original: the case-body order already matches, and the residual is
the per-case body size (case 125 +856 B) plus the global cold-block placement
(case-157 state-5 strong arm at `0x144a14` orig vs `0x144366` ours).  No
`__builtin_expect` placement tried here improves raw while keeping 57232 B.

## 2. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9304/harness.py            # (54869, 57232, 57232)
# variants: /dev/shm/w9304/variants.py, /dev/shm/w9304/inject.py, inject2.py
cmake --build cmake-build-match --target check -j6
```

## 3. Open items

1. Case 125: find a source shape that outlines the nested `switch (m_nState)`
   to the original's `0x13b518` (the original's state-0/1 head is only 98 B).
   That is the single largest body-size delta (-856 B of cumulative shift).
2. Case 154: the state-0-inline structure is reachable with
   `__builtin_expect(m_nState == 0, 1)`, but the size grows +92 B; a companion
   that removes 92 B elsewhere would make it size-exact and may then align the
   whole `0x13a7xx` block.
3. The cold-block order (case-157 state 5..12) is not reachable from any
   `__builtin_expect` placement tried; it needs a source-shape change in the
   state chain itself (not a weight).
