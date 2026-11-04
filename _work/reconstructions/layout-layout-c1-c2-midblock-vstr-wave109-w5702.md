# Layout::Layout C1/C2 — mid-block `vstr` intphi, wave-109 sweep (w5702)

Worker **w5702**, branch `wave109-w5702`, base `7c34c4098` (wave-108 master).
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`, ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Source `src/game/ui/Layout.cpp`.

This pass is **note-only**: no source change landed, because every size-exact
variant measured at or below the wave-108 landed body. The landed body is
unchanged at **48.049363057325 %**, 7072 B.

## Result

| metric | value |
|---|---:|
| `_ZN6LayoutC1Ev` size | 7072 |
| `_ZN6LayoutC1Ev` unrounded `difflib` ratio | 48.049363057325 % |
| normalised insns (orig/ours) | 2529 / 2495 |
| `byte_exact` / `linked_exact` | 4500 / 4500 |
| `size_exact` / `data_exact` | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No section change: 28 sections (the wave-108 `Status::missionCompleted` rank
table `.jcr` section is untouched), `.ARM.extab` unchanged at `0x26294`.

## The mid-block `vstr` fingerprint (re-verified this pass)

Original non-HD block (`0xd1996..0xd19de`), from the Thumb disassembly:

```
d1996: str.w  r6, [r0, #148]   ; 0x94
d199a: str.w  r1, [r0, #152]   ; 0x98   (int)
d199e: vcvt.s32.f32 s0, s0
d19a8: vstr   s0, [r0, #156]   ; 0x9c   (floatphi, int->float round trip)
...
d19ca: vmov   s0, r2
d19ce: str.w  r2, [r0, #172]   ; 0xac   (int phi, general register)
d19d6: vcvt.f32.s32 s0, s0
d19da: vcvt.s32.f32 s0, s0
d19de: vstr   s0, [r0, #168]   ; 0xa8   (intphi round trip, same r2)
```

So the real source shape for the mid block is one int phi (`r2`), stored to
`0xac`, then re-used for `0xa8` through `vmov s0,r2; vcvt.f32.s32;
vcvt.s32.f32; vstr`. The wave-108 landed body reproduces the `0xa8` half with
a local `int v = <float ternary>; field_0xa8 = (int)(float)v;` but keeps a
separate `0xac` expression, because the faithful shared-`int` form reorders
the whole body and collapses the fuzzy score (see table).

Original `0x16c`/`0x170` chain (`0xd1f24..0xd1f34`) is the *same* fingerprint:

```
d1f24: vmov   s0, r1
d1f28: mov    r1, r4
d1f2c: vcvt.f32.s32 s0, s0
d1f30: vcvt.s32.f32 s0, s0
d1f34: vstr   s0, [r0, #368]   ; 0x170
```

i.e. `0x170` is an intphi fed by the `0x16c` chain (`0x8a/0x45`, `0x73/0x115`,
`0x6a`, `0x80/0x40`), with `0x174`/`0x17c` stored in between. Adding that
round trip to the landed body is size-neutral (`+0 B`, same 2495 insns) and
scores exactly 48.0494 — it is already what the compiler emits for the
landed `0x170` assignment.

Original `0x2d4`/`0x2d8` are stored as one `strd` pair (`0xd18b6`), which the
landed body already produces (0x2d8 assignment precedes the 0x2d4 if/else).

## Measured table (real `link_so` builds, unrounded linked score)

| variant | size | % |
|---|---:|---:|
| **landed body (wave-108)** | **7072** | **48.049363057325** |
| `0x2d8` `(int)(float)` + remove `0x194` cast | 7072 | 48.009554140127 |
| `0x170` `(int)(float)` cast (size-neutral) | 7072 | 48.049363057325 |
| `0x204` int-ified iPadHD arm | 7072 | 47.751691205730 |
| `0x2d8` `(int)(float)` alone | 7080 | 47.871070433745 |
| `0x10c` if/else conversion | 7080 | 42.105263157895 |
| `0x128` if/else conversion | 7088 | 42.777556705133 |
| shared `int v` for `0xac`+`0xa8` (faithful REA shape) | 7000 | 47.801758593125 |
| remove `0x194`+`0x1a4` casts | 7036 | 48.235997608132 |
| remove `0x1a4` cast only | 7048 | 48.167330677291 |
| wave-89 all-ternary body (unlandable) | 6800 | 50.788500000000 |

The 6800 B all-ternary body is the highest fuzzy score but is **not
landable**: the ctor must stay exactly 7072 B. Verified this pass with the
`0x194`+`0x1a4` cast-removal body (7036 B): the full `check` reports
`byte_exact 4500 -> 2582` and `RATCHET FAIL`, because the 36-byte `.text`
shrink moves every following symbol and invalidates the per-function byte
comparison. So a candidate must be size-exact 7072 B *and* beat 48.0494.

## Sweeps run this pass (all real builds)

* Every simple 4-way ternary in the landed body converted to an `if/else`
  chain (≈65 statements): the ones that stay 7072 B produce **identical**
  code (same 2495 insns, same 48.0494); the rest are smaller/larger and score
  lower. Best non-base 7072 B result: 48.0096 (`2d8c+194`).
* Size-neutral pairs of `+8/-8`, `+16/-16`, `+24/-24`, `+60/-60` conversions
  (e.g. `0x10c`+`0x1bc`, `0x128`+`0x124`, `0x188`+`0x168`,
  `orbitInfoSystemY`+`0x1e4`): all ≤ 47.80.
* All 206 adjacent single-line statement swaps: no 7072 B variant above
  48.0494 (most are byte-identical; the rest lower).
* Big `if (iPadHD) {…} else {…}` restructure of the first 5/10/15/20/25/30/36
  ternary statements: 6952–7064 B, 33.8–44.2 % (worse).
* Reverse direction — convert the landed body's 12 `if/else` blocks back to
  the 6800-body ternaries: 7056–7120 B, 43.9–47.7 % (worse).
* 6800 all-ternary body + the landed body's 13 `if/else` conversions:
  6888 B, 43.04 % (the conversions hurt in that ordering).
* 6800 + 13 conversions + the landed body's float-literal (`37.5f`, `49.0f`,
  `(int)(float)`) round trips: 6864–6904 B, 43.0–43.6 % (worse).
* ~4000 random multi-edit combinations (if/else conversions, cast
  add/remove, iPadHD float→int) from the prior w5702 run: zero hits above
  48.0494 at exactly 7072 B.

## Proven vs hypothesis

**Proven (binary / measured):**

* The original mid-block `0xa8` and `0x170` stores are int→float→int round
  trips (`vmov`/`vcvt`/`vcvt`/`vstr`) sharing a general-register int phi.
* 7072 B is a hard local optimum for the landed statement set: no adjacent
  swap, per-statement `if/else` conversion, cast edit, or size-neutral pair
  beats 48.0494.
* Any non-7072 B body fails the ratchet (`byte_exact` collapses to 2582 for a
  36-byte shrink), so the 50.79 % all-ternary body cannot be landed.

**Hypothesis (not proven):**

* The original source is a single top-level `if (Globals::iPadHD) { …HD
  constants… } else { …nested n9/retina/iPad conditions… }` tree (the Thumb
  has `beq.w` at `0xd103e` and a `b.w` rejoin at `0xd1268`), not the flat
  ternary list we compile. Reproducing it faithfully would need a full-body
  rewrite and is not guaranteed to land at exactly 7072 B.
* The faithful shared-`int` form for `0xac`/`0xa8` is the real source shape
  but is metrically hostile here (7000 B, 47.80 %).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5702
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS

# unrounded per-function score (difflib autojunk=False on reconciled insns)
python3 /dev/shm/w5702/fscore.py _ZN6LayoutC1Ev
# _ZN6LayoutC1Ev: orig 7072B ours 7072B  score=48.049363057325  insns 2529/2495
```

Scratch harness: `/dev/shm/w5702/{fscore.py,harness.py,Layout.base.cpp}`.
`Layout.base.cpp` is the pristine landed ctor; `Layout.6800base.cpp` is the
wave-89 all-ternary body.
