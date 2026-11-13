# Radar::draw wave 191b (w13656) — target-bracket / literal-68 / loop-form sweeps, no size-exact raw win

**Status:** complete, 2026-11-13. Worktree `wt/w13656`, branch `wave191b-w13656`,
base `master` `fda02cb7e`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13647.md` (target-bracket/blip geometry),
`-w13644.md` (blip-draw/store order), `-w13636.md` (lock-image order landed),
`-w13632.md` (four structural divergences), `-w13597.md` (r10/r11 pinned to the
literal-68 vreg), `-w13614.md`, `-w13606.md`.
**No source change landed**: `src/` is untouched; every measured cell is raw-neutral
(6,013) or worse, size-exact unless noted.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,961 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,201 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.  `check` is green on the untouched base:
byte_exact 4,508 / linked_exact 4,508 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370.

## 1. Angle (a): faithful target-bracket block, identical operand/store order

Reconstructed the planet target bracket from the Thumb disassembly
(`0x12e4f2`-`0x12e55a`).  The original is X-first with **exact** operand order
(`screenX > left`, `screenX < left + size`, `screenY > top`, `screenY < top + size`);
writing `top + size > screenY` instead of `screenY < top + size` produces a
different `cmp` operand order and a different (raw-better) cell, so both were
measured.  The true-arm store order in the original is `field_0x18` then
`planetDockIndex` (source already has that order, but our codegen emits
`planetDockIndex` first); swapping the source order (`ss2`) was measured too.

| cell | raw | aligned | size | insn | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base | **6,013** | **1,961** | 9,908 | 3541 | 71,201 | 577 |
| `P` exact X-first (`screenY < top + size`) | 6,019 | 2,000 | 9,908 | 3540 | 71,207 | 577 |
| `P'` X-first with `top + size > screenY` | 6,014 | 1,950 | 9,908 | 3541 | 71,202 | 577 |
| `ss2` field_0x18/planetDockIndex source swap | 6,014 | 1,960 | 9,908 | 3541 | 71,202 | 577 |
| `P`+`ss2` | 6,018 | 1,999 | 9,908 | 3540 | 71,206 | 577 |
| `top_first` (left/top computation order) | 6,013 | 1,961 | 9,908 | 3541 | 71,201 | 577 |
| `L` landmark exact X-first | 6,020 | 1,992 | 9,908 | 3540 | 71,208 | 577 |
| `A` asteroid `slotCount` before `field_0x1b0` | 6,029 | 1,966 | 9,908 | 3541 | 71,217 | 577 |
| `P`+`L`+`A` | 6,042 | 2,019 | 9,908 | 3539 | 71,230 | 577 |

All are genuine source-order fixes (confirmed from disassembly) and all are
raw-worse.  The `L`/`P` cells remove one instruction each (3540) but keep the
9,908 size because the removed 16-bit encodings are replaced by 32-bit ones.

## 2. Angle (b): loop/register forms that change r10/r11

The residual is the single greedy-RA cascade: `{shared zero/mbp, campaign} ×
{r10, r11}`.  A loop form that adds one long-lived callee-saved vreg flips the
colour to the original (`zero=fp`, `campaign=sl`) but is not size-exact.

| cell | raw | aligned | size | insn | zero | `.ARM.extab` |
|---|---:|---:|---:|---:|---|---:|
| base | **6,013** | **1,961** | 9,908 | 3541 | sl | 577 |
| planet loop `for (i=0,n=size(); i<n; ++i)` | 9,184 | 2,215 | **9,948** | 3528 | **fp** | 1,336 |
| planet loop, `n` declared before loop | 9,184 | 2,215 | **9,948** | 3528 | **fp** | 1,336 |
| planet loop, `n` as `int` / `!=` / `+=1` / `(uint32_t)` | 9,184-9,186 | 2,215 | **9,948** | 3528 | **fp** | 1,336 |
| planet pointer cache `pt = this->planetTargets` | 9,182 | 2,215 | **9,948** | 3528 | **fp** | 1,336 |
| enemy loop size cache | 9,282 | 2,501 | **9,964** | 3539 | sl | 1,334 |
| landmark loop size cache | 9,466 | 2,130 | **9,880** | 3518 | r2 | 1,243 |
| asteroid loop size cache | 6,816 | 2,055 | **9,908** | 3544 | sl | 577 |
| gas loop size cache | 8,359 | 2,531 | **9,912** | 3541 | sl | 577 |
| sparks loop size cache | 8,350 | 2,831 | **9,900** | 3537 | sl | 577 |

`lc_asteroid` is the only size-exact loop-cache cell, but it does **not** flip
(zero stays `sl`) and raw rises to 6,816.  Every flipping loop form costs ≥ +40 B.
Combining the planet-loop cache with the semantically-correct fixes never returns
to 9,908 (`lc+P` 9,948, `lc+L` 9,960, `lc+P+L+A` 9,960).

### 2.1 Type flips (re-measured on this base)

| cell | raw | size | zero | campaign |
|---|---:|---:|---|---|
| `int turretMode` | 9,226 | **9,860** | fp | sl |
| `bool otherLock` | 9,249 | **9,860** | fp | sl |
| `int alwaysEnemySeen` | 9,191 | **9,952** | fp | spill |
| `int bossSeen` | 9,186 | **9,924** | fp | spill |
| `int otherLockFlag` | 9,447 | **9,896** | r2 | sl |
| `bool alienOrbit` | 6,022 | 9,908 | sl | spill |
| `unsigned campaign` | 6,014 | 9,908 | sl | spill |
| `unsigned notTurretMode` / `notOtherLock` | 6,013 | 9,908 | sl | spill |

`int turretMode` and `bool otherLock` both give the original colouring at
9,860 (−48 B); no size-exact spelling of either was found.  `bool alienOrbit`
is size-exact but does not flip.

## 3. Angle (c): literal-68 vreg source spelling

The literal 68 (`17, 68` image-frame args, 14 call sites) is materialised 12×
in both binaries; the original uses `sl` (r10) at four enemy-region sites, ours
uses `fp` (r11) at two and `r6` at four.  Every spelling that produces the same
`i32 68` IR constant is byte-identical to the base:

| cell | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| `17, 0x44` | 6,013 | 1,961 | 9,908 | 71,201 | 577 |
| `17, 68u` | 6,013 | 1,961 | 9,908 | 71,201 | 577 |
| `17, (int)68` | 6,013 | 1,961 | 9,908 | 71,201 | 577 |
| `17, (int)(unsigned char)68` | 6,013 | 1,961 | 9,908 | 71,201 | 577 |
| `17, (int)(unsigned short)68` | 6,013 | 1,961 | 9,908 | 71,201 | 577 |
| `17, 68 & 0xff` | 6,013 | 1,961 | 9,908 | 71,201 | 577 |
| local `const int blipH = 68` at all sites | 6,013 | 1,961 | 9,908 | 71,201 | 577 |
| `0x11, 68` | 6,013 | 1,961 | 9,908 | 71,201 | 577 |

The 4-vs-2 `sl`/`fp` materialisation split is a post-RA rematerialisation effect
of the same cascade, not a source-spelling difference.

## 4. Why no local win: the resync regions only differ by shift-dependent bytes

Per-`0x100` differing-byte profile of the base (`.text` section, function range):

```
12e050   9   12e150   1   12e250   1   12e350 147   12e450 246
12e550 245   12e650 237   12e750 239   12e850 230   12e950 245
12ea50 242   12eb50 249   12ec50 246   12ed50 241   12ee50 246
12ef50 177   12f050 134   12f150 229   12f250 243   12f350 226
12f450 180   12f550 219   12f650 108   12f750   2   12f850   4
12f950   0   12fa50   3   12fb50   4   12fc50 228   12fd50  76
12fe50 141   12ff50 247   130050  51   130150   3   130250 184
130350 227   130450 240   130550 167   130650  96
```

The 0-4-byte "resync" windows (`12f750`, `12f850`, `12f950`, `12fa50`,
`12fb50`, `130150`) are **not** fixable locally: the remaining bytes are branch
displacements to blocks inside the shifted region (`12f794 ca/be`, `12f8ae
e5ba/e5b8`, `12fa9c 8125/8122`, `12fb5a 80bd/80ba`, `130150 8244/81c8`,
`13022e 2e/45`) plus one stack-slot pair (`12f814 9018/9017`, `12f850
9818/9817` — orig `sp+0x60`, ours `sp+0x5c`).  Both are downstream of the 4-byte
`12e3d4` shift.  The first divergence after the last resync is the asteroid
blip x/y block at `0x12fc5a` (`field_0x11e`/`field_0x11f` branch layout and the
hoisted `screenX` load); reordering the source `if (tractorMode2 && field_0x1c
== nullptr && wreck)` to the original's evaluation order (`tm2_first`) changes
it but is raw +2 (6,015, aligned 1,951).

The first 9 differing bytes of the whole function (`12e050`-`12e150`) are all
the cascade itself: `mode` slot `sp+0x68` vs `sp+0x64`, `mov.w sl,#0` vs
`mov.w fp,#0`, the two `strb.w` zero stores, and `str r0,[sp,#0x90]` vs
`mov sl,r0` for `campaign`.  There is no local win above the cascade.

## 5. Combination search

270 random subsets (1-4 edits) of a 21-edit pool {`os`, `mus`, `snd`, `f`, `P`,
`L`, `A`, `gasd`, `gas76`, `stb`, `enb`, `tm2_first`, `ss2`, `top_first`,
`alien_bool`, `unsigned campaign`, `swap zero stores`, `notTurretMode`/
`notOtherLock` decl swap, `17,0x44`, ...} were measured.  Every size-exact cell
bottoms out at **raw 6,013** (best: `decl_swap`, i.e. base bytes); no cell
reached 6,012 or below.  The base is a raw local minimum.

## 6. Conclusion

Three fresh angles were exhausted on this base:

* (a) the faithful target-bracket block (exact X-first operand order, store
  order) is a real source fix but raw +1…+6;
* (b) every loop/register form that flips r10/r11 (`zero=fp`, `campaign=sl`)
  changes size (+40 for the planet-loop size/pointer cache; −48 for the two
  `bool`→`int` flips);
* (c) the literal-68 spelling is IR-invariant and byte-identical to the base.

The residual remains the single greedy-RA cascade pinned in w13597, and no
size-exact source spelling flips it.  `src/` is untouched and the worktree is
clean.  `.ARM.extab` stays 577 for every size-exact cell; the flipping cells
grow it to 1,336 (loop cache) or shrink it to 1,239/1,243 (bool flips), so even
ignoring size they are not net `.ARM.extab` wins.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1961 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13656/`: `cell.py` (1 s
compile+link+strip+`fn_diff`+section-diff cell harness with zero-reg readout),
`Radar.base.cpp` (pristine source), `batch1.json`-`batch16.json` (the cells
above), `search.py` (random subset search), `search1.log` / `search2.log`
(270 cells), `adiff2.py` / `cmpdis.py` (r10/r11-normalised instruction
alignment), `secdiff.py`, `orig.dis` / `ours.dis`, `mir.txt` (full
`-print-machineinstrs`; pre-RA at 395123, greedy at 404141, rewriter at
413436), `radar_prera.txt` / `radar_greedy.txt` / `radar_rewriter.txt`.
