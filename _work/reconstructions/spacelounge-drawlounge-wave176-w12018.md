# SpaceLounge::drawLounge — draw/order/local sweep (wave 176, w12018)

Status: research note (2026-11-11). Worktree `wt/w12018`, branch `wave176-w12018`,
base `b21088985`. **No `src/` change landed**: every size-exact shape tested keeps
the committed raw/aligned score. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **175**, aligned **41**, size **1948/1948**, official pct **95.7**
* after: unchanged (note only)
* gate: `RATCHET PASS` — byte 4507 / linked 4507, size 4524/4524, data 843/843,
  imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior work: `spacelounge-drawlounge-w4118.md`, `-w4818.md`, `-wave125-w7636.md`
(and the older notes they cite). This note adds a systematic draw/order/local
sweep and two new RA data points; it supersedes nothing.

## 1. Residual (re-confirmed)

`tools/fn_diff.py` on the linked `.so`:

```text
_ZN11SpaceLounge10drawLoungeEv   raw=175 aligned=41 size=1948/1948 insn=741/741
```

The structural diff is the same one pinned in wave 125: the clamp/join at
`0x1712b2..0x17131e` plus the downstream `x` uses. Original vs HEAD registers:

```text
value        original        HEAD
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

HEAD's 4-instruction right arm (`movgt r1,#0; subgt.w r1,r1,fp,lsl#1;
subgt r1,r1,r6; addgt.w r9,r1,r0`) is *closer in raw bytes* than the faithful
2-instruction arm (`addgt.w r1,r6,sl; subgt r5,r0,r1`), because the faithful
form makes the compiler materialise `mov r5,fp` in both arms. Faithful measured
**raw 445 / aligned 54**; HEAD is **175 / 41**.

## 2. Sweeps performed

All measurements are the object-level oracle (`w3692_oracle.py`, official
compile command, normalised instruction diff) plus linked `fn_diff.py` for the
candidates that stayed size 1948.

### 2.1 Statement / declaration order

Permuted `{pad load, SetColor, centerX, top}` (24 orders, pad before top) and
`{Layout *layout, int x}` (2 orders), with `lineHeight` kept after the clamp.
Every order that compiles to the same schedule scores **95.455 / 1948** with
`pad=fp, x=r9`. Moving `pad` after `SetColor`/`centerX`/`top` drops to 92.756;
moving `SetColor` before `pad` drops to 92.756. `lineHeight` before the clamp
grows to 1952.

Swapping `name`/`role`/`width` construction order also scores 95.455 (the
compiler schedules identically). A `switch` or if/else chain for `role`
grows to 2016/2028 and drops to ~68/76.

### 2.2 Clamp expression sweep

90 combinations of the right-arm association (15 forms, including
`-(width + pad*2)`, `-(pad*2 + width)`, `-width - pad - pad`,
`+(-width - pad - pad)`, `-(width + (pad+pad))`, …) × the condition sum order
(6 forms). The best is the committed flat form at **95.455 / 1948**; the
faithful `-(width + pad*2)` family is 93.750. No association moved `pad` to
`r9`.

### 2.3 Local types

`pad` as `short`/`unsigned`/`long`/`const int`/`register int`/`volatile int`/
`auto`/`decltype`/`long long`; `x` as `unsigned`/`long`/`short`/`int32_t`/
`size_t`/`long long`; `width`, `lineHeight`, `centerX`, `top` likewise. Only
two variants change the colouring:

| variant | size | pct | pad | x |
|---|---:|---:|---|---|
| `long long pad` (all uses 64-bit) | 2008 | 77.973 | **r9** | **r5** |
| `long long pad`, top only 64-bit | 1980 | 80.198 | **r9** | **r5** |
| `long long pad`, all uses cast to int | 1948 | 95.455 | fp | r9 |
| `long long pad`, top cast to int | 1976 | 89.753 | fp | r9 |

The 64-bit pad proves the target colouring is a natural RA outcome one notch
of live-range shape away, but the 64-bit arithmetic costs 32–60 B. There is no
size-1948 64-bit-pad shape (2048 cast masks swept; none hit 1948 with
`pad=r9, x=r5`).

### 2.4 Direct field accesses / draw-order locals

Making individual `pad` uses read `layout->field_0x94` (or
`Globals::layout->field_0x94`) was swept one use at a time and in combinations.
The only size-1948 shapes that reach `pad=r9, x=r5` are the ones that reload
`field_0x94` for `DrawRectangle`:

| variant | size | pct | raw | aligned |
|---|---:|---:|---:|---:|
| HEAD | 1948 | 95.455 | **175** | **41** |
| `DrawRectangle(x - field_0x94, top - field_0x94, …)` | 1948 | 91.038 | 310 | 90 |
| same + `lineHeight` in `DrawRectangle` | 1948 | 91.103 | 310 | 103 |
| same via local `layout` pointer | 1948 | 91.038 | — | — |
| `xp/tp/bw` locals before `drawBox` | 1948 | 95.455 | 175 | 41 |
| `xp/tp/bw` locals + `lineHeight` in `DrawRectangle` | 1948 | 88.604 | — | — |

The reload is exactly what makes `pad` survive in `r9` (the reloaded value is
a second, short-lived interval), but it also forces the compiler to recompute
`x-pad`/`top-pad` straight into the call-argument registers and shifts the
`lineHeight`/`agent` spills. The extra raw bytes (310 vs 175) swamp the gain.

### 2.5 Branch weights / expects

`__builtin_expect` on the width-add anchor with values 0/1/2/3/4/7/8/16/100/
1000, on the final `isKnown` branch, on the clamp conditions, on the
`hoverAgent` test and on the `mode` test. Only the committed anchor keeps the
body at 1948 with the 95.455 schedule; every other expect either grows the
body (1952–1980) or drops the score (68–94). `plain_hover` is size 1948 but
68.559.

## 3. Conclusion

The committed flat clamp remains the best size-exact shape: **raw 175 /
aligned 41 / 1948 B / pct 95.7**. The residual is the greedy-RA fact that
`pad` (unhinted `0xA0000000+6400`) is selected after `x`'s re-enqueue, so `x`
takes `r9` whole and `pad` later takes `fp` (and is copied to `r5` at the
join). Every source shape found that forces the target `pad=r9, x=r5`
colouring either adds a `field_0x94` reload (raw 310) or uses 64-bit pad
arithmetic (size 2008). No size-exact raw win was found.

## 4. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
# object-level oracle (patched ROOT/ORIG for this worktree)
python3 /home/debian/GalaxyOnFire2/scratch/w12018/oracle.py \
  --src src/game/menu/SpaceLounge.cpp --fn _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12018` (`oracle.py`,
`variant.py`, `batch*.py`, `llsearch.py`, `rawtest.py`, `block_base.txt`,
`base.txt`, `orig.asm`, `ours.asm`).
