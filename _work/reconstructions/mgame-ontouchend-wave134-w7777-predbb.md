# MGame::OnTouchEnd — wave 134 (w7777): EstimateRuntime vs PredBB

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7777`,
base `bbffef482` (pristine source restored; **no source change landed**). Scratch
`/dev/shm/w7777`. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

Baseline (verified this wave, full `check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
raw fuzzy (difflib, autojunk off): 81.9607 %   size 8080/8080   insns 2775/2774
```

## 1. The w7758 hypothesis and why it does not hold

w7758 lead 2: make the 0x200 `initHudMenu(1)` prefix *longer* than the 0x400
`initHudMenu(2)` prefix so `BranchFolder::CreateCommonTailOnlyBlock`
(`BranchFolding.cpp:784`) flips the survivor to the 0x400 body.

The tie-break is real:

```cpp
unsigned TimeEstimate = ~0U;
for (i = 0; i < SameTails.size(); ++i) {
  if (SameTails[i].getBlock() == PredBB) { commonTailIndex = i; break; }   // (A)
  unsigned t = EstimateRuntime(begin(), getTailStartPos());
  if (t <= TimeEstimate) { TimeEstimate = t; commonTailIndex = i; }        // (B)
}
```

But **(A) fires first** in this build.  At the CFO merge the join block
(`hudTouchFlags == 0`, `%ir-block.2567`) has the **0x200 body as its layout
predecessor**, so `PredBB == bb.404` (the 0x200 body) and the loop breaks before
`EstimateRuntime` is ever consulted.  The 0x200 body therefore wins
*unconditionally*, independent of prefix cost.

Evidence (MIR, real NDK clang 7.0.2, `-mllvm -print-after-all`):

| variant | 0x200 prefix at PEI | 0x400 prefix at PEI | chosen block | size | raw fuzzy |
|---|---|---|---:|---:|---:|
| base | `LDR 116; LDR 120; MOVi 1` (5) | `LDR 116; LDR 120; MOVi 2` (5) | 0x200 | 8080 | 81.9607 |
| vstore (`hudMenuOpen = 1;` in 0x200 body) | `LDR;LDR;MOVi 1;STRB;MOVi 1` (8) | `LDR;LDR;MOVi 2` (5) | **0x200** | 8084 | 80.7568 |
| v6 (hoist `Hud*`/`Level*` for 0x400 body) | `MOVi 1` (1) | `MOVi 2` (1) | 0x200 | 8072 | 80.1514 |
| v5 (hoist only `Level*` for 0x400 body) | `LDR;LDR;MOVi 1` (5) | `LDR;MOVi 2` (3) | **0x200** | 8084 | 80.6126 |

`vstore` and `v5` are the decisive ones: in both the 0x200 prefix is strictly
*more* expensive than the 0x400 prefix (8 vs 5 and 5 vs 3), so (B) alone would
have to pick the 0x400 body, yet the shared tail is still split out of the
0x200 body.  (A) dominates.  `v6` is the control: hoisting both loads for the
0x400 body collapses *both* prefixes to a single `MOVi` (tie), and the tie
again goes to the 0x200 body as the last `SameTails` entry.  This also explains
why w7758's `hudMenuOpen = 1` store did not flip here: it lengthens the 0x200
prefix but (A) ignores it.

## 2. All "different argument evaluation order" shapes are byte-identical

Every size-neutral re-spelling of `this->hud->initHudMenu(N, this->level)` was
compiled and compared; all produced the exact base bytes (8080 B, 81.9607):

* `Level *lvl = this->level; this->hud->initHudMenu(N, lvl);`
* `Hud *hud = this->hud; hud->initHudMenu(N, this->level);`
* both locals at once
* `Level **lvl = &this->level; ... *lvl`
* `Level &lvl = *this->level; ... &lvl`
* `Level *lvl = (&this->level)[0];`
* `(this->hud)->initHudMenu(N, (this->level))`
* a ternary menu type `(flags & 0x200) ? 1 : 0`
* swapping the `weaponSlot`/`wingmanCommand` declarations
* `(void)this->level;` / `(void)this->hud;` before the call
* `goto hbMenu1`/`goto hbMenu2` bodies after the chain (clang folds them)
* inverting the chain to `if (!(flags & 0x200)) { ... } else { initHudMenu(1); }`

The IR builder/optimizer normalises all of them to the same block order and the
same MIR.

## 3. The only shape that flips: the source-level case swap

`if (0x400) initHudMenu(2); else if (0x800) ...; else if (0x200) initHudMenu(1);`
puts the 0x400 body first in the IR, so it is lower-numbered and becomes the
CFO survivor (0x400 body falls through into the shared `blx`; 0x200 body
branches forward).  It keeps 8080 B and `.rel.plt` exact, but the dispatch test
order becomes `tst #1024` (0x400), `lsls #20` (0x800), `lsls #22` (0x200)
instead of the original `tst #512` (0x200), `lsls #20`, `lsls #21`:

```
raw fuzzy 81.8526 %   size 8080/8080
17adc4  ldrd r0,r2,[fp,#116]; movs r1,#2; blx initHudMenu   ; 0x400 owns blx
```

That is 0.108 raw points *below* the base, so it was not kept.

## 4. Remaining attack

The survivor is decided by `PredBB`, i.e. by the **layout position of the join
block relative to the bodies after `HoistCommonCode`/`OptimizeBranches`**, not
by `EstimateRuntime`.  To flip it without changing the dispatch order one needs
the 0x400 body to become the join block's layout predecessor while the 0x200
test stays first.  In a plain `if/else-if` chain the body order follows the
test order, so this likely requires a source shape that survives CFG
simplification with the 0x400 body emitted before the 0x200 body (a `switch`
lowering, or a `goto` chain that clang does not fold).  All `goto`/inversion
shapes tried here were folded back to the base CFG.

## 5. Reproduction

```
cp /dev/shm/w7777/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7777/score.py        # 81.9607 / 8080 / 2775:2774
```

MIR dumps (real NDK clang, filtered to OnTouchEnd):

```
/dev/shm/w7777/mir.sh                  # -mllvm -print-after-all
# PEI  = pa.log around "After Prologue/Epilogue Insertion"
# CFO  = pa.log around "After Control Flow Optimizer"
```

Key MIR facts (base): PEI 0x200 body `bb.426` (`%ir-block.2426`), 0x400 body
`bb.430` (`%ir-block.2448`); CFO shared tail `bb.405` is split out of `bb.404`
(the 0x200 body), with `bb.435` (0x400 body) branching to it.  The join block
`bb.406` (`%ir-block.2567`) follows `bb.404` in the CFO layout, so
`PredBB == bb.404`.

Original authoritative binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(0x200 body `0x17adc0` branches to shared `blx` `0x17afde`; 0x400 body
`0x17afd8` falls through into it; join test `0x17afe2`).
