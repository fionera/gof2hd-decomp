# Radar::draw wave 190 (w13437) — raw-win hunt: no size-exact raw win found;
# the 5 "zero-shift" bytes are branch-target immediates, not `strh` registers

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13437`, branch
`wave190-w13437`, base `master` `1d72f95c9`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13426.md` (planet-lock store order, landed), `-w13422.md`
(`camZ` CSE), `-w13412.md` (planet-lock y test + wreck/tractor analysis), `-w13404.md`
(music block) and `-w13402.md` (expect-hint removal).

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function address | `0x12e050` |
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,288** |
| instruction-index alignment | 1,913 |
| instruction count | 3,543 ours / 3,532 orig |
| `.text` differing bytes (whole `.so`) | 72,879 |
| `.ARM.extab` differing bytes | 601 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.rel.plt` | byte-identical |
| total differing bytes | 73,500 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. The five "zero-shift" diff bytes are branch-target immediates

The session-1 handoff described the five single-byte zero-shift diffs as `strh`
register-number differences. That is a misread: all five are the **low byte of a
`b<cond>.w` target immediate**, and all five differ by exactly **+6** (ours targets are
6 B earlier):

| branch site | original target | ours target | delta |
|---|---|---:|---:|
| `12f982 beq.w` | `12fdb4` | `12fdae` | -6 |
| `12fa9e bcs.w` | `12fcea` | `12fce4` | -6 |
| `12fac6 bne.w` | `12fcd2` | `12fccc` | -6 |
| `12fad2 bne.w` | `12fcd2` | `12fccc` | -6 |
| `12fb5c beq.w` | `12fcd8` | `12fcd2` | -6 |

So there is **one** root cause (a net -6 B shift of the block at `12fccc`), not five
independent register-allocation cells. Fixing it would also align the
`12fd48..12fe13` region (274 differing bytes at shift -6, per `shiftstat.py`).

## 2. Where the -6 comes from (two independent contributions)

Byte-counting the `12fc40..12fcd6` region against the original:

| block | original | ours | delta |
|---|---:|---:|---:|
| wreck x/y select (`12fc5a..`) | 82 B | 70 B | **-12** |
| `DrawImage2D` arg setup | 12 B | 12 B | 0 |
| tractor guard/body | 26 B | 32 B | **+6** |
| loop increment `add r9,#1; b` | 6 B | 6 B | 0 |
| `asteroidHit` cold block | 18 B | 18 B | 0 |

Net **-6**. The wreck block is 12 B short because clang **hoists the `screenX` load**
(`12fc5e ldr r2,[r4,#0xfc]`) and reuses it in four arms; the original issues four
separate `ldr r2,[r4,#0xfc]` (`12fc6a/70/88/94`). The tractor guard is 6 B long because
clang folds `field_0x1c == nullptr` and `wreck` into one predicate
(`eor r1,sl,#1; cmp; it ne; movne; orrs; bne`) where the original branches on the
pointer (`cbnz r0`) and if-converts only `wreck` (`eors r0,sl,#1; itttt eq`).

A second, independent -4 B sits inside the `int best` loop: ours reloads
`ldr.w fp,[sp,#140]` (`&this->radarPos`) at `12fd3a`, where the original keeps `fp`
loaded once at `12fcf6`. That is why the shift goes -6 -> -2 -> -6 across
`12fd30..12fd48`.

## 3. Why the obvious fix does not land

To zero the shift at `12fccc` we need **exactly +6 B** in `12fc40..12fccc` (not +12).
Forcing a `volatile` read of `screenX` in any arm makes clang drop the hoist and issue
all four loads, reproducing the original 82-byte wreck block exactly — but that is
**+12 B**, so the shift becomes +6 (not 0) and raw *rises* to 6,927 (size 9,920).
Reproducing the tractor guard's 26-byte shape from source was not achievable: every
condition order/spelling still yields the 32-byte folded form.

## 4. Sweeps measured this pass (all reverted; full `link_so -j2` + `fn_diff`)

Every row is a full incremental build (`ulimit -v 12000000`, `-j2`; scratch
`/home/debian/GalaxyOnFire2/scratch/w13437/`). The base is a very strong local optimum:
**no variant produced raw < 6,288 with size 9,908**.

| sweep | variants | best raw | note |
|---|---:|---:|---|
| tractor guard operand orders (all 6) + nested/`!ptr` | 10 | 6,288 | neutral or 6,289; `wreck`-first = 9,940 |
| tractor body store orders | 3 | 6,288 | neutral |
| `volatile` `screenX` in each arm (1/2/3/4/else) | 5 | 6,288 | arm1 neutral; others +12 B, raw 6,921-6,961 |
| `volatile` `screenY` in arm3/arm4/y-block | 3 | 6,288 | neutral |
| wreck x/y rewrites (`x = 0`, `y = 0`, `y = screenY`, `x += 11`, `11 + x`, `x = x - 11`, `?:`) | 15 | 6,288 | `y = screenY` = 9,948; `clean` form = 9,940 |
| wreck arm reorders (swap arm3/arm4, merge, else-first) | 5 | 6,288 | neutral |
| `__builtin_expect` on wreck conditions (0 and 1) | 8 | 6,288 | `e11d 0` = 6,746 (size-exact); `e11e 1` = 9,900 |
| `PaintCanvas *canvas` / `unsigned int image` declaration moves | 5 | 6,288 | `image` first = 6,294 |
| `int best` loop: `radarPos` alias (ref/ptr), statement orders, `float distf`, `bestDistance` literal, `k < 5` | 10 | 6,288 | `playerPos` first = 6,291; `radarPos = a - b` = 9,900 |
| `camZ` spellings (`float`, `const float`, `auto`, `int`) | 4 | 6,288 | `int camZ` = 9,916 |
| `camZ` removed (use `this->cameraZ` at the call) | 1 | 6,363 | aligned 1,891; gas-block pool shrinks 12 -> 8 B |
| music-block `stop` / `inAlienOrbit` operand orders | 3 | 6,303 | 6,303 / 6,311 / 6,311 |
| music `field_0x1bc >= 1` expect removal (+ music reorders) | 3 | 6,469 | 9,916 / 9,916 / 7,268 (size-exact) |
| gas-lock `__builtin_expect` on `gasHit` / `> field-200` / `> 500` (0 and 1) | 5 | 7,065 | all size-fatal |
| wreck image-select `e` -> `enemy`, arm swap | 2 | 6,291 | `enemy` = 9,900 |
| declaration/type toggles from the session-1 "unreliable" set (`decl_swap`, `pl_int`, `en_int`, `gas_int`, `ast_int`, `lm_i2`, `notTurret_*`, `hasScanner_*`, `size_shift`, `rowHeight`, `offset_int`, `s3`) | 13 | 6,288 | `gas_int`/`ast_int`/`s3` = 6,289 |

## 5. Literal-pool placement (the `12f488` island)

The `+12` shift over `12f48c..12f5ee` (285 differing bytes — the single largest shift
group) is a constant-island placement difference, not a code difference: ours emits a
3-word pool at `12f488` (`{GOT 0x000e10d8, 24000.0f, -24000.0f}`) right after
`b.n 12f5ae`; the original puts `24000.0f/-24000.0f` in a 6-word pool at `12f200`
(`{4 GOT words, 24000.0f, -24000.0f}`) after `b.n 12f15c`. Source-level spellings of
the constants (`const float far = 24000.0f`, reordered comparisons) are byte-identical;
the island position is a function of the surrounding block lengths, not of the source.
Removing the `const auto camZ` hoist shrinks the island to 8 B (shift `+8`) but costs
more elsewhere (raw 6,363).

The pool at `0xd2854` (`0.325f, 0.65f, 100.0f, 50.0f`) is **not** in `Radar::draw` (it
is >0x5b000 B away, out of `vldr` range); it belongs to `Layout::Layout`, as noted in
w13426 §2.3. The `Radar::draw` end pool is at `0x1306f0` in both.

## 6. Other structural differences confirmed (not fixed)

* `12f4xx` gas-block prologue: ours hoists `cameraZ` into `s16`
  (`vldr s16,[r4,#580]`) and needs a third constant (`vldr s20`); the original loads
  `s16/s18` from the `12f210` pool. Removing the hoist is the w13422 `camZ` cell.
* `12f106..12f1fa` wreck image selection: original lays the
  `carriesFriendCargoFlag` arm at `12f106` and the fallback at `12f13e`; ours emits a
  different arm order and a different `ldrb [r8,#0x24]` prologue. Any reorder tried so
  far moves 8-40 B and loses raw.
* `12ffca..130080` gas-lock cold block: ours places the
  `!lockConfirmed && !sound->isPlaying(0)` / `lockedGasCloud = lockedGas` block
  out-of-line at `130006`; the original places it inline at `12ff4c`. This is the
  `-126` shift group (~200 bytes). All `__builtin_expect` weights tried on the
  enclosing `gasHit` / `> field_0x1b8 - 200` / `> 500` conditions are size-fatal.
* Function tail `1306b0..130704`: the EH cleanup landing pads are laid out in a
  different order (ours puts the `sp+232` destructor last, the original first), giving
  the `-2` shift over the last 14 bytes.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6288 aligned= 1913 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13437/`: `tryvar.py` (build+measure
harness), `shiftstat.py` (per-byte shift histogram + ranges), `shiftmap.py`,
`zerodiff.py`, `regraw.py`, `t30.py`..`t53.py` (all sweeps above), `sweep_exp.log`.
