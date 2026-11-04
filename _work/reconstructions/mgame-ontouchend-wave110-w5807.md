# MGame::OnTouchEnd (8080 B) — w5807 (wave 110): `char refinedAny`
# recovers the original `lsls` flag test; gate-safe 78.8829 -> 78.9189

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `390f58052`, branch
`wave110-w5807`.

## Result

One-line source change landed (real C++, no attributes/asm):

```
src/game/menu/MGame.cpp, MGame::OnTouchEnd, mining-refine block:
-                    int refinedAny = 0;
+                    char refinedAny = 0;
```

| shape | size | fuzzy (exact) | structdiff | .rel.plt diff | gate |
|---|---:|---:|---:|---:|---|
| HEAD | 8080 | 78.8829 | 1437/2775 | 0 | RATCHET PASS |
| **this pass (`char`)** | **8080** | **78.9189** | **1434/2775** | **0** | **RATCHET PASS** |
| `bool` (faithful) | 8076 | 76.3518 | 1687/2775 | 0 | size −4 (fails size-exact) |
| `int`/`unsigned`/`long`/`wchar_t` | 8080 | 78.8829 | 1437/2775 | 0 | code-identical to HEAD |
| `signed char`/`unsigned char`/`short`/`unsigned short`/`int8_t`/`uint8_t`/`int16_t`/`uint16_t` | 8080 | 78.9189 | 1434/2775 | 0 | same code as `char` |

`check` after the change: byte_exact **4500** / linked_exact **4500**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
**RATCHET PASS**.

## 1. Why `char` is the right spelling

The original's mining flag test (0x17a666) is

```
17a666: 980a       ldr   r0, [sp, #40]      ; refinedAny
17a668: f8db 4094  ldr.w r4, [fp, #148]
17a66c: 07c0       lsls  r0, r0, #31        ; bit-0 test  -> bool
17a66e: f000 84f5  beq.w 0xf18
```

`lsls #31` is clang's bit-0 test for `bool`.  Our HEAD used
`int refinedAny`, which emits `cmp r0,#0`; `char refinedAny` emits
`lsls r0,r0,#24` (low-byte test), i.e. the same `lsls`-flag shape as the
original instead of a `cmp`.  That single instruction change is worth 3
structdiff slots (1437 -> 1434) and is size-neutral (8080 B).

`bool refinedAny` is the *faithful* type (`lsls #31`) but is 8076 B: the
`bool` store makes clang strength-reduce the mining counter and swap the
`fee`/`amount` registers, which perturbs the allocation enough to remove one
4-byte instruction.  Because any `.text` length change shifts `.rodata`
(`.text` is `0x1337c8`, `.rodata` starts at `0x1c9e10`) and tanks `byte_exact`
(4500 -> ~3102), `bool` cannot pass the gate.  `char` is the closest
size-neutral spelling.

## 2. What was re-verified this pass (all measured, base = HEAD)

| variant | size | fuzzy (exact) | .rel.plt | note |
|---|---:|---:|---:|---|
| `char/short/uint8_t/int8_t/int16_t/uint16_t refinedAny` | 8080 | 78.9189 | 0 | **best gate-safe** |
| `bool refinedAny` | 8076 | 76.3518 | 0 | faithful, −4 B |
| `long/unsigned/wchar_t refinedAny` | 8080 | 78.8829 | 0 | code-identical to HEAD |
| `refinedAny = 1` / `= (char)1` / `= false` | 8080 | 78.9189 | 0 | same as `char = 0` |
| `refinedAny = 0xff` | 8080 | 78.9189 (1435) | 0 | one extra diff |
| `if (amount != 0)` / `if (amount)` / `if (count)` | 8072 | 70.9 | 0 | strength-reduces to up-counter |
| `if (amount < 0)` | 8080 | 78.8829 | 0 | code-identical to HEAD |
| `unsigned amount` (+ `(int)count` in String) | 8072 | 70.9 | 0 | up-counter |
| `char/short amount`, `char/short/int8_t count` | 8076–8084 | 65–75 | 0 | worse |
| `disp_hud0elsews` (+ `refbool`/`cnt_ne`/`refchar`) | 8080–8084 | 76–80.3 | 4 | `.rel.plt` rotation |
| `split127_disp` | 8080 | 79.8559 | 4 | `.rel.plt` rotation |
| `bool refinedAny` + `short/int8_t/int16_t amount` | 8080 | 75.5 | 0 | worse than `char` |
| `bool refinedAny` + `char count` | 8080 | 74.5 | 0 | worse |

`score.py` rounds to 78.9 for both HEAD and `char`; the 4-decimal exact
ratio (`scorex.py`, same `tools/verify` Resolver/SequenceMatcher) is
78.8829 -> 78.9189.

## 3. Proven vs hypothesis

**Proven from the binary**

* The `refinedAny` test at 0x17a666 is `lsls r0,r0,#31` (bit-0), i.e. the
  original declaration is `bool`; HEAD's `int` (`cmp #0`) is the only
  non-faithful part of the mining flag.
* The mining loop keeps a *down* counter in `r9`
  (`sub.w r9,r9,#1` at 0x17a5b6, `rsb r1,r9,#0` at 0x17a5da) and tests
  `cmp.w r9,#0; beq` at 0x17a5ca.  `if (count > 0)` is the only spelling
  found that keeps the down counter; `amount != 0` / `count != 0` make clang
  strength-reduce to an up counter (`add.w r8,r8,#1`) and lose 8 B.
* `.rel.plt[3324..3327]` order (reset, removeObjectives, goingToWaypoint,
  removeRoute) is unchanged; `MGame::successCheck` stays byte-exact.

**Hypothesis / tuned**

* `char` (rather than `bool`) is a tuned but ordinary C++ spelling chosen to
  keep the function at exactly 8080 B.  It is closer to the original than
  `int` (same `lsls`-flag instruction) but not identical (`#24` vs `#31`).
* The remaining 21 % is still MachineBlockPlacement: the `hudTouchFlags == 0`
  cluster (`test -> isAutoPilot tail -> epilogue -> hud0 body`) is emitted at
  the end of our function (0x17f0/0x17f6/0x1812/0x1828) instead of at
  0x0e9e/0x0ea4/0x0ec0/0x0ed6.  That is the source of the 14
  `b.w 0xec0` vs `b.w 0x1812` epilogue-target diffs and of most of the
  campaign-chain shift.  Wave-107/109 exhaustive searches found no source
  spelling that moves it without changing the length or permuting `.rel.plt`.

## 4. Section deltas introduced

None.  All ELF sections are unchanged from the wave-108 state:

```
.text       0x1337c8  exact (base+size exact)
.ARM.extab  0x26294   (original 0x26298, 4 B short — pre-existing)
.ARM.exidx  0x06f70   exact
.rodata     0x041b67  exact
.jcr        0x0000c0  (192-byte Status::missionCompleted rank table — pre-existing)
.dynsym/.hash/.rel.dyn/.rel.plt/.got/.data/.bss  exact
```

No padding, no new sections, no new imports (207).

## 5. Exact commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5807
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5807/scorex.py                        # exact 4-decimal fuzzy
python3 /dev/shm/w5807/plt.py                           # .rel.plt diff count
python3 /dev/shm/w5807/v2.py refchar                    # apply + build + score
python3 /dev/shm/w5807/sweep2.py                        # refinedAny/count/amount type sweep
python3 /dev/shm/w5807/sweep3.py                        # bool-base compensation sweep
```

Scratch (not tracked): `/dev/shm/w5807/` (`v2.py`, `score.py`, `scorex.py`,
`plt.py`, `sweep2.py`, `sweep3.py`, `mksbs.py`, `orig.s`, `ours.s`,
`sbs_cur.txt`, `MGame.base.cpp`, `MGame.base.h`).
