# Layout::Layout C1/C2 — Ghidra mid-block `(int)(float)` evidence and a
# size-neutral `field_0x134` improvement (w4267, pass 9)

Worker **w4267** (wave 92), branch `wave92-w4267`, base `b379d00d`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF `0xd1028`, original size **7072 B**
(`.text` `0x69440..0x1cb808`; the ctor's literal pools are *inside* `.text`).
Original binary: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Ghidra image base `0x10000`; `Layout::Layout` at Ghidra VMA `0xe1028`.

This note continues `layout-layout-c1-c2-hd-block-order-w4221.md`,
`layout-layout-c1-c2-hd-vector-w4101.md` and `layout-layout-c1-c2-sweeps-w4001b.md`.

## Result

Landed one **size-neutral** source-shape change: `field_0x134` written as an
explicit `if/else` chain instead of a flat nested ternary.

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` `usize` | 7072 | **7072** |
| raw verifier ratio (unrounded) | 47.6550079491256 | **47.66448022261976** |
| displayed pct (verify rounds to 1 dp) | 47.7 | **47.7** |
| `byte_exact` | 4499 / 4524 | **4499 / 4524** |
| `linked_exact` | 4499 / 4524 | **4499 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`. The raw ratio is a strict improvement; the displayed one-decimal
pct is unchanged because the verifier rounds (`tools/verify/verify.py:score`).
The size is unchanged, so `byte_exact`/`linked_exact` are untouched.

## 1. New evidence: Ghidra decompilation of the mid block

`analyzeHeadless ... -process '*' -noScript -postScript DecompAt2.java` with
`ADDR=0xe1028` (script `/home/debian/ghidra_scripts/DecompAt2.java`, project
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`,
project name `android_2.0.16_libgof2hdaa.so_ghidra`) reproduces the mid block.
Verbatim excerpts (Ghidra names the scalar `vcvt.f32.s32`/`vcvt.s32.f32` pair
`VectorSignedToFloat`; `fVar69` is a reused **float** local, `uVar45`/`uVar50`
are reused **int** locals):

```c
    *(int *)(this + 0x150) = (int)fVar69;
    ...
      uVar50 = 0x88;
      fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x158) = (int)fVar69;
    ...
      uVar45 = 0xd;
      fVar69 = (float)VectorSignedToFloat(uVar50,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x15c) = (int)fVar69;
    ...
    fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
    *(int *)(this + 0x160) = (int)fVar69;
    ...
      fVar69 = (float)VectorSignedToFloat(uVar45,(byte)(in_fpscr >> 0x16) & 3);
      *(int *)(this + 0x170) = (int)fVar69;
```

and elsewhere in the same function:

```c
    *(int *)(this + 0x9c)  = (int)fVar69;
    *(int *)(this + 0x1e4) = (int)fVar69;
    *(int *)(this + 0x3f8) = (int)fVar69;
```

So the original source really does store an **`int` field through a float
variable**: `field = (int)f;` where `f` was assigned `f = (float)int_expr;`
(or a float constant select). This confirms the `(int)(float)` family of §4 of
the w4221 note from the decompiler side, not just from the instruction pattern.

## 2. Why our `(int)(float)` folds — micro-test matrix (clang 7.0.2, `-Oz`)

Exact project flags:
`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -Oz`.

The round-trip `vmov s0,rN / vcvt.f32.s32 / vcvt.s32.f32 / vstr` survives only
when the converted int is **not** a pure constant select/phi. Measured:

| source shape | round-trip? |
|---|---|
| `s->x = (int)(float)(a ? 70 : 52);` | no (folds) |
| `s->x = a ? 70 : (int)(float)(b ? 100 : 14);` | no |
| `int v = a?70:52; s->x=(int)(float)v;` | no |
| `int v = a?70:52; s->y=v; s->x=(int)(float)v;` | **yes** |
| `int v = a?70:52; s->x=(int)(float)v; s->y=v;` | **yes** |
| `s->y = a?70:52; s->x=(int)(float)s->y;` | **yes** |
| `int v; if(a)v=70;else v=52; s->y=v; s->x=(int)(float)v;` | **yes** |
| `int v; if(a)v=70;else v=52; s->x=(int)(float)v;` | no |
| `float f = a?8.0f:4.0f; s->x=(int)f;` | no |
| `float f; if(a)f=8.0f;else f=4.0f; s->x=(int)f;` | no |
| `float f; if(a){s->y=50;f=50;}...; s->x=(int)f;` | no |
| `static const int K[]={...}; s->x=(int)(float)K[i];` | **yes** |
| `s->x = (int)(float)((s->y + 17) << a);` (`s->y` a member) | **yes** |

The rule is: the int value must be **materialised as an int** (a direct member
store, or an opaque load) at the point of the float conversion. A pure
constant select is folded by InstCombine; an `int` local that is *also* stored
to a member is not.

This is exactly what the Ghidra listing shows: in the `else` (iPadLarge) branch
the original writes `*(this+0x154) = 0x32;` (the same `uVar45` value that is
then converted for `0x158`), which pins the value as an int and keeps the
round-trip alive on the other paths too.

## 3. Literal-pool group decode (HD block)

The HD block's `vld1.64` groups are loaded from `.text` literal pools. Reading
the pool words at each first-referenced address gives the group contents
(`gparse.poolfirst` + `Elf32.bytes_at`):

```
0xd1308 {9,11,5,253}      -> 0xc0,0xc4,0xc8,0xcc
0xd1320 {1,21,78,151}     -> 0x128,0x12c,0x130,0x134
0xd1330 {15,18,2,189}     -> 0x140,0x144,0x148,0x14c
0xd1340 {5,70,70,206}     -> 0x150,0x154,0x158,0x15c
0xd1350 {68,0,35,184}     -> 0x160,0x164,0x168,0x16c
0xd1360 {174,84,84,217}   -> 0x170,0x174,0x178,0x17c
0xd1370 {23,163,23,244}   -> 0x180,0x184,0x188,0x18c
0xd1380 {-6,8,16,90}      -> 0x190,0x194,0x198,0x19c
0xd1390 {18,22,22,243}    -> 0x1a0,0x1a4,0x1a8,0x1ac
0xd13a0 {30,12,28,7}      -> 0x14,0x18,0x1c,0x20
0xd13b0 {21,42,40,52}     -> 0x4,0x8,0xc,0x10
0xd13c0 {7,7,4,42}        -> 0x24,0x28,0x2c,0x30
0xd13d0 {8,9,82,140}      -> 0x34,0x38,0x3c,0x40
0xd13e0 {7,15,7,125}      -> 0x44,0x48,0x4c,0x50
0xd13f0 {487,341,63,90}   -> 0x54,0x58,0x5c,0x60
0xd1400 {112,566,191,63}  -> 0x64,0x68,0x6c,0x70
0xd1410 {63,126,63,112}   -> 0x2c8,0x2cc,0x2d0,0x2d4
0xd1420 {12,14,112,42}    -> 0x7c,0x80,0x84,0x88
0xd1430 {18,42,46,1}      -> 0x1b0,0x1b4,0x1b8,0x1bc
0xd1440 0.462222, 0xd1444 1.40625   (0x90 and the HD scale)
0xd1448 {4,20,25,11}      -> 0xb0,0xb4,0xb8,0xbc
0xd1458 {56,112,281,562}  -> 0xec,0xf0,0xf4,0xf8
0xd1468 {281,118,16,16}   -> 0x294,0x298,0x29c,0x2a0
0xd1478 {154,140,147,42}  -> 0x2a4,0x2a8,0x2ac,0x2b0
0xd1488 {-2,1,2,1}        -> 0x108,0x10c,0x110,0x114
0xd1498 {9,35,21,1}       -> 0x118,0x11c,0x120,0x124
0xd14a8 {92,26,40,50}     -> 0x1c0,0x1c4,0x1c8,0x1cc
0xd14b8 28.125, 0xd14bc 14.0625
0xd14c8 {7,17,5,14}       -> 0x1d0,0x1d4,0x1d8,0x1dc
```

The first *store* group is `0x14..0x20` (`vst1.32 {d2-d3},[r0+0x14]` at
`d10a4`), but the first *loaded* group is `0xc0..0xcc` (`vld1.64 {d0-d1}` at
`d1046`, stored at `d1158`). The load order is therefore the scheduler's order,
not the source order; it is not a reliable source-order oracle for the HD block
(the w4221 §6 hypothesis does not survive the decode above). The `9 11 5 253`
words are the `0xc0..0xcc` group, not the first source statement.

## 4. Measured variants (official verifier, unrounded ratio)

Base (HEAD): `7072 B`, raw **47.6550079491256**.

| variant | usize | raw |
|---|---:|---:|
| `field_0x134` as if/else (**landed**) | 7072 | **47.66448022261976** |
| `0x3f8` restructure (w4221 A) | 7024 | 47.84517158818834 |
| A + `0x9c` ternary (B) | 7024 | 47.96488427773344 |
| A + B + `0x150` `(int)(float)` (E) | 7040 | 47.717759617301176 |
| A + B + `0x158` `<<retina` shift (C) | 7032 | 47.86597526924611 |
| A + B + `0x310` if/else + E | 7060 | 47.80099502487562 |
| A + B + `0x310` + C + E | 7068 | 47.742192162323455 |
| A + B + `0x310` + C + E + `0x134` | 7068 | 47.71189812972543 |
| A + B + `0xac` + `0x310` + E | 7068 | 47.713717693836976 |
| A + B + `0xac` + `0x310` + E + `0x134` | 7068 | 47.72320540862995 |
| A + B + `0xac` + `0x310` + E + C | 7076 | 47.6550079491256 |
| A + B + `0x2cc` + `0x310` + `0xac` | 7076 | 47.68527717067356 |
| A + B + `0xac` + `0x2cc` + C + E | 7072 | 47.5641 |
| `0x98` if/else body | 7016 | 47.79148428173498 |
| `field_0x134` if/else on `0x98` body | 7024 | 47.76297474647047 |

All `7072`-byte combinations found in the subset searches are listed above; the
landed one is the only one with raw > base. The `0x3f8` restructure is the
right *shape* (tail 49.4 → 49.9 %) but is 48 B short; every attempt to add the
missing 48 B via `(int)(float)` wraps or float locals either folds (no bytes) or
loses more ratio than it gains. Of the seven missing `vstr`s:

* `0x9c` and `0x150` are **float-typed** in the original (`vldr` from a pool /
  `vmov.f32` immediate + `vcvt.s32.f32`). A float local keeps `0x150`'s
  round-trip (vstr 336 appears) but the ratio drops to ~47.4; `0x9c`'s float
  select still folds in the flat-ternary source.
* `0x158`, `0x15c`, `0x160`, `0x170` are **int→float→int**. `0x15c` survives
  the `<<retinaDisplay` shift form (vstr 348 appears, +8 B, ratio 47.58); the
  others fold unless the int is first materialised (member store / opaque load).
* `0x1e4` is a **hoisted float→int** in the common tail; a float local keeps it
  (vstr 484) but the ratio drops to ~42.
* `0x3f8` is int→float→int on the `rewardBoxY` **member load** and does survive
  (`(int)(float)((rewardBoxY + 17) << retina)` adds vstr 1016), but the
  restructure that exposes it is 48 B short and the ratio drops to ~46.7.

## 5. Reproduction

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# score the current body with the unrounded ratio
python3 /dev/shm/w4267/score5.py

# Ghidra decompile of the ctor
ADDR=0xe1028 /tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra \
  android_2.0.16_libgof2hdaa.so_ghidra -process '*' -noanalysis \
  -postScript DecompAt2.java
```

Scratch (`/dev/shm/w4267`): `score5.py` (unrounded), `vstr.py` (vstr set),
`gparse.py` (pool groups), `mkvariant.py`/`measure.py` (build+score a source
variant), `allifelse.json` (185 ternary→if/else conversions), `trans.json`
(restructure/shift/wrap transforms), `decomp2.txt` (Ghidra output).

## 6. Remaining work

1. Reproduce the six int→float→int `vstr`s (`0x9c 0x158 0x15c 0x170 0x1e4
   0x3f8`). The decompiler shows the original uses a reused float local and a
   reused int local, and in the iPadLarge branch stores the same int directly to
   `0x154` before converting it for `0x158`. A source shape that materialises the
   int (a member store or an opaque load) before the conversion is required;
   micro-tests in §2 give the exact conditions.
2. The HD-block register allocation (original prologue saves 6 regs, ours 7;
   original uses 16-bit `cbz` at flag tests, ours `cmp;beq`) is still the
   dominant mismatch. It follows the exact Android 2.0.16 statement order and
   expression types, which are not yet recovered.
