# `Layout::Layout` C1/C2 — wave 124 / worker w7403

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`
(constructor only).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Base = wave-123 final master `6f8ee2320`.

## Result — source win landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.894202 % | **48.934051 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |
| symbol parity | 5370/5370 | 5370/5370 |
| `.ARM.exidx` differing bytes | 0 | **0** (byte-identical) |
| `.ARM.extab` size/base | 0x26298 @ 0x1a3b78 | **0x26298 @ 0x1a3b78** |
| `.text` differing bytes | 87,014 | **87,015** (+1) |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`
(`verify: byte 4506 linked 4506 stubs 0 missing 0 extra 0 avg 99.94`,
`data 843/843`, `sodiff extra 0 missing 0`, `imports 207`).
`--target drift` → `0 field(s) across 0 class(es)`.  No section size or
base changed; only the `.text` content-delta byte count moves 87,014 ->
87,015, which is updated in `docs/IDENTICALITY.md` and
`docs/VALIDATION.md` in the same commit.

## The change (one line)

```cpp
// before
this->field_0x158 = Globals::iPadHD ? 70 : (Globals::iPadLarge ? 100 : (Globals::iPad ? 50 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 28 : 14))));
// after
this->field_0x158 = Globals::iPadHD ? 70 : (Globals::iPadLarge ? 100 : (Globals::iPad ? 50 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 28 : this->field_0x154 - 1))));
```

Only the innermost default arm changes.  `field_0x154` is assigned earlier in
the constructor (line 142) and is `15` on that arm, so `15 - 1 == 14` — the
same value the literal arm produced.  The rewrite is semantically exact for
all 32 `iPadHD`/`iPadLarge`/`iPad`/`n9`/`retinaDisplay` combinations.

## Evidence

Original `0x158` store (`0xd1db8`) is one of the 21 `(int)(float)`
round-trips:

```
d1d9c: 210e         movs r1, #14
d1d9e: 240d         movs r4, #13
d1da0: f8c0 1154    str.w r1, [r0, #340]   ; 0x154
d1da4: 2100         movs r1, #0
d1da6: ee00 4a10    vmov s0, r4
d1db0: eeb8 0ac0    vcvt.f32.s32 s0, s0
d1db4: eebd 0ac0    vcvt.s32.f32 s0, s0
d1db8: ed80 0a56    vstr s0, [r0, #344]   ; 0x158
```

The original's `0x158` value is an int phi (`r4 = 13` n9 / `14` default)
across the store-bearing branch, so clang keeps the int→float→int pair.

Our landed body does not add a round-trip (it still emits a plain integer
store), but it changes the default arm from a literal to a memory-derived
`sub`, which fixes the surrounding register/instruction shape:

```
d209c: 229e         movs r2, #158
d209e: 231c         movs r3, #28
d20a0: f1b8 0f00    cmp.w r8, #0
d20a4: bf08         it eq
d20a6: 224f         moveq r2, #79
d20a8: bf08         it eq
d20aa: 1e4b         subeq r3, r1, #1      ; field_0x154 (15) - 1
d20ac: e9c0 3256    strd r3, r2, [r0, #344] ; 0x158, 0x15c
```

`r1` holds the just-stored `field_0x154` value (`15` on the default arm), so
the `subeq` reproduces `14` with a single 2-byte instruction and lets the
`0x158`/`0x15c` pair share one `strd`.

## Defeating clang 7's `(int)(float)` folding — what was tried

The residual is clang 7.0.2 constant-folding `(int)(float)(select-of-const)`
to a plain int.  Only values that come from memory survive the round-trip.
Reproducer (`/dev/shm/w7403/rep2.cpp`): `(int)(float)(t + (g & 0x80))` and
`(int)(float)(t + (g >> 1))` keep the `vcvt` pair but add an `and`/`lsr`;
`(int)(float)(t + (g == 2))` keeps the pair with only a `cmp`; a member
loaded right after its own store still folds.

Variants measured on the real constructor (all real `link_so` builds, exact
unrounded ratio):

| variant | size | unrounded % |
|---|---:|---:|
| **base** | 7072 | 48.894202 |
| **landed: `0x158` default = `field_0x154 - 1`** | **7072** | **48.934051** |
| `0x170` default = `field_0x16c - 5` | 7032 | 48.931496 |
| `0x148` default = `field_0x154 - 13` | 7080 | 48.904819 |
| `0x60` default = `field_0x1c + 24` | 7080 | 48.903946 |
| `0x70` default = `field_0x6c - 101` | 7080 | 48.645418 |
| `0x6c` default = `field_0x68 - 172` | 7080 | 48.486056 |
| `0x15c` default = `field_0x158 + 65` | 7096 | 48.398010 |
| `0x68` default = `field_0x60 + 259` | 7080 | 48.277236 |
| `0x154` default = `field_0x150 + 11` | 7104 | 48.289578 |
| `0x20_top` default = `field_0x1c - 15` | 7064 | 43.158313 |
| `0x1e4 = field_0x150` | 7000 | 40.431482 |
| v138 as `float` local | 7112 | 48.131955 |
| `0x158` whole ternary in `(int)(float)` | 7096 | 48.477612 |
| `0x158` n9 arm also `field_0x154 - 1` | 7072 | 48.694959 |
| `0x158` all arms memory-derived | 7080 | 48.356901 |
| `0x158` as `if/else` chain (same code) | 7072 | 48.934051 |
| `0x158` `if/else` + int local + `(int)(float)` | 7096 | 48.477612 |
| `0x158` default `+ (n9 == 2)` (cmp trick) | 7072 | 48.934051 (same code) |
| all 157 auto default-arm rewrites | — | only `0x148` beat base |
| all 155 auto rewrites + the `0x158` win | — | none beat 48.934051 |

The `(t + (g == 2))` trick from the handoff's minimal reproducer is a codegen
no-op here (identical 48.934051 / 7072 B): clang hoists the always-zero term
away in this context.  A `volatile` round-trip explodes the size
(7156–7192 B).

## Remaining root cause

The residual is clang register allocation / SLP on a field-major body.  The
original's 21 `(int)(float)` round-trips at `0x60 0x68 0x6c 0x70 0x8c 0x9c
0xa8 0x138 0x150 0x158 0x15c 0x160 0x168 0x170 0x1a8 0x1e4 0x2c8 0x2d0 0x320
0x3e0 0x3f8` come from int phis across store-bearing branches; ours still
fold six of them (`0x138 0x158 0x15c 0x170 0x1e4 0x3f8`) and add six
(`0x20 0x10c 0x148 0x1f4 0x204 0x3e8`).  The `field_0x158` rewrite is a
code-shape win, not a round-trip win.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7403
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
python3 /dev/shm/w7403/score_layout.py                  # 7072 / 48.934051
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd209c --stop-address=0xd20b0 cmake-build-match/libgof2hdaa.so
```
