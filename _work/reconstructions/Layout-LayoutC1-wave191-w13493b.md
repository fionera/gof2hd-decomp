# `Layout::Layout` C1/C2 — wave 191b / worker w13493b

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13493`, base master `7c06dae7a`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,243,
aligned 2,728, size 7,072/7,072, insn 2,521/2,552).**  Gate re-run green:
`RATCHET PASS`, byte_exact 4507/4524, linked_exact 4507/4524, size-exact
4524/4524, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (29,392 B, base/off identical), `.ARM.extab`
size/base identical (156,312 B).

This is the fourth independent Layout lens.  It records only cells **not**
already in `Layout-LayoutC1-wave190-w13420.md`, `...-w13443.md`,
`...-w13458.md`, `...-w13467.md`, `...-w13473.md`, `...-w13377-lens2.md`,
`...-w13383.md`, `...-w13385.md`, `...-w13390.md` (read those first).

## 1. Branch-encoding profile (new lens)

Both functions have **exactly 135 branch instructions** and **87 conditional
branches**, but they are encoded differently:

| mnemonic | original | ours |
|---|---:|---:|
| `cbz`  | **28** | **14** |
| `beq*` | 36 | 52 |
| `bne*` | 7 | 1 |
| `b*` (uncond) | 59 | 62 |

The original tests flag bytes with `ldrb rL,[rP]; cbz rL,target` (2-byte `cbz`
after a 2-byte `ldrb`), while ours uses `ldrb.w r1,[ip]; cmp r1,#0; beq`
(`cmp`+`beq` = 4 B, and the `ldrb.w` is 4 B).  The original's flag pointers
live in `r8` (retina) and `sl`/`fp` (HD/n9), so the loads are 2-byte `ldrb
rL,[r8]`; ours keeps the retina pointer in `ip`, forcing `ldrb.w`.  This is the
same callee-saved-rotation residue as w13420 §3, now quantified as a
branch-encoding difference: it is worth roughly (28-14)=14 extra `cbz`-sized
savings in the original, which the original spends elsewhere (see §2).

## 2. Literal-pool head and the `field_0x8c` island

| | code end | code bytes | pool start | pool words | pool bytes |
|---|---:|---:|---:|---:|---:|
| original | `0xd2852` (+nop) | 6,188 | `0xd2854` | **221** | 884 |
| ours | `0xd28d6` | 6,320 | `0xd28d8` | **188** | 752 |

Pool head (word-aligned, decoded from the linked `.so`):

```
orig: 0.325 0.65 100.0 50.0 GOT GOT GOT GOT 281.25 400.0 GOT 200.0 150.0 GOT GOT ...
ours: 64.0  32.0 GOT   GOT  GOT 281.25 400.0 GOT 200.0 150.0 ...
```

`0.325/0.65` are `field_0x90`, `100/50` are `field_0x9c`; `64/32` are
`field_0x8c`.  In the **original** the `field_0x8c` pair is *not* in the tail
pool: it lives in an inline island at `0xd1d94/0xd1d98`, referenced by
`addw r5, pc, #1156` at `0xd190e` (the next unconditional branch is `b.n
0xd1da6` at `0xd1d7a`).  In **ours** the same pair is the first tail-pool
entry at `0xd28d8`, referenced by `addw r1, pc, #3884` at `0xd19a8`.  So the
5-word pool-head misalignment documented in w13420 §1 is exactly this
island-vs-tail-pool placement of `field_0x8c`.

Pool-word multiset difference (useful target list): ours has extra
`64.0, 32.0, 15.0, 1.5(x2), 12.0, 82.0, 41.0` and the original has extra
`0.325, 0.65, 100.0, 50.0` plus one more GOT word.  The `82.0/41.0` pair is
`field_0xa8`'s float arms (see §3).

## 3. `field_0xa8` is a float round-trip of the `field_0xac` int in the original

Original `0xd19b6..0xd19d0`:

```
movs r2,#122 / movs r2,#61 / movs r2,#82 / moveq r2,#41   ; int chain
...
vmov s0, r2
vcvt.f32.s32 s0, s0
vcvt.s32.f32 s0, s0
str.w r2, [r0, #172]   ; 0xac  (int)
vstr  s0, [r0, #168]   ; 0xa8  (int)(float) of the same r2
```

i.e. the original computes the chain **once as int** (`field_0xac`), then
`field_0xa8 = (int)(float)field_0xac` reusing the same register.  Ours instead
computes a second, float-typed chain (`int v = ...61.0f...82.0f:41.0f...;
field_0xa8 = (int)(float)v`), which is why `82.0/41.0` are in our pool.

Measured variants of this cell (all semantic-value identical on all 32 flag
combinations):

| variant | size | raw proxy |
|---|---:|---:|
| `int v = HD ? (int)(61*1.40625f) : ...; ac=v; a8=(int)(float)v;` | 7,008 | 5,483 |
| `float v = HD ? 61.0f*1.40625f : ...; ac=(int)v; a8=(int)(float)v;` | 7,048 | — |
| `int v = HD ? (int)(61.0f*1.40625f) : ...; ac=v; a8=(int)(float)v;` | 7,008 | 5,483 |
| same + `field_0x130` explicit if/else | 7,000 | — |
| `field_0xac` HD arm `(int)(61*1.40625f)` cast only (no reuse) | 7,072 | **5,240 neutral** |

So the original's reuse structure is worth about **-64 B** in our TU; the
original must recover those 64 B elsewhere (the three extra outlined cold
tails in §4 are the natural candidate).  Alone it is size-fatal.

## 4. Branch-target layout: 5 outlined cold tails vs 2

Original cold tails at `0xd2802..0xd2852` (80 B): `field_0x130/0x134/0x138`
default (`iPad?56:40`), `field_0x2c8` default (`iPad?45:30`),
`field_0x2d0` default (`iPad?45:30`), `field_0x60` default (`iPad?64:44`),
each reached by `beq.w`/`b.w` and returning with `b.w`.  Ours outlines only
`field_0x18c`'s retina/default arm (`0xd28b8`) and `field_0x60`'s default
(`0xd28ca`).  The three missing outlines are exactly the `iPad ? x : y` final
arms that clang if-converts in our TU (`it eq; moveq`).  `__builtin_expect`
on those `iPad` tests (`0` and `1`) is **neutral** (if-converted, no branch to
influence), and the explicit-if/else forms change size (see below), so the
outlining is a layout consequence, not directly source-controllable.

## 5. Exhaustive searches run this pass (all negative)

Real `Layout.cpp` TU, exact checked-in clang 7.0.2 command, 4 workers,
`ulimit -v 12000000`; proxy = function bytes from the `.o` vs the original at
`0xd1028` (base proxy 5,240, linked 5,243, i.e. +3 from relocation sites).

| search | candidates | best |
|---|---:|---:|
| single-unit moves, units 30..200 → every position | 34,030 | base (5,240) |
| contiguous block length 2, start 0..130 → every position | 25,870 | base |
| contiguous block length 3, start 0..130 → every position | 25,740 | base |
| random 2-move combinations (seed 777) | 4,000 | base |
| random 3-move combinations (seed 1234) | 4,000 | base |
| all 24 permutations of pool-head group `{0x9c,0x90,eventBannerScale,0x8c}` (real link) | 24 | identity 5,243; next 5,337 |

These fill the gaps left by w13467 (single moves ±12, all 2-opt swaps),
w13458 (block moves length 2-3, start 130..201), w13473 (early-block single
moves/perms) and w13377 (random block moves 1-4, `{8,9,10,11,12,18}` perms).

## 6. Structural variants measured (all size-fatal or neutral)

| change | size | raw |
|---|---:|---:|
| `field_0x8c` if/else chain (HD/n9/retina/default) | 7,024 | — |
| `field_0x9c` if/else chain | 7,072 | 5,397 |
| `field_0x90` if/else chain (`retina||iPad||n9` → 0.325) | 7,072 | **5,240 neutral** |
| `field_0x130` else → explicit `if (iPad) {...} else {...}` | 7,064 | — |
| `field_0x34` moved next to `field_0x38` (fixes the SLP group to `{0x34..0x40}`) | 7,008 | 5,970 |
| `field_0xa8` direct expression (no `v` local) | 6,888 | — |
| `field_0xa8` before `field_0xac` | 7,032 | — |

The `field_0x34` result is the key structural datum: reproducing the
original's `{0x34,0x38,0x3c,0x40}` SLP group costs **-64 B** in our TU, and
the original is already 132 B *smaller* than us, so the SLP fix can only be
cashed in together with the outlined cold tails (+64 B) and the `field_0xa8`
reuse (-64 B) — a three-way structural swap, not a one-line win.

## 7. Conclusion

The current hill-climbed statement order is a sharp local optimum under the
real TU: every single-unit move to every position, every length-2/3 block move
in the early/mid region, and random 2/3-move combinations are non-improving.
The remaining residue is structural: (a) callee-saved rotation (`r8` retina /
`sl` HD) that changes 14 `cbz` into `cmp+beq`, (b) the `field_0x8c` constant
island vs tail pool, and (c) three missing outlined cold tails.  A future pass
must reproduce (a)+(b)+(c) together while keeping the total at 7,072 B.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13493
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5243 aligned=2728 size=7072/7072 insn=2521/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13493/`: `harness.py`,
`units3.py` (201-unit span splitter), `sweep.py` (real-TU proxy scorer),
`runsingle.py`, `runblock.py`, `runrand.py`, `linktest.py`, `permtest.py`,
`exp2..exp9.py`, `orig.dis`, `ours.dis`, `align.py`.
