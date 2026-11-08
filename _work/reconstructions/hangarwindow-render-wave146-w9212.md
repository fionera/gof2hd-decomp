# `HangarWindow::render` — wave 146 (w9212): iconX/iconY lifetime markers vs the 9700-byte RA

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w9212`,
base `31297fadc` (wave 145). Metric: raw same-address diff bytes
(`/dev/shm/w9212/fdiff.py`). Baseline (committed tree) **1719**, size 9700,
`.ARM.extab` 156312 exact.

## Result

**No source change landed.** The requested shape (borderX + merged free-credits
iconX/iconY) exists at 9700 B but **not** with `.ARM.extab` 156312: every shape
that merges the extra call-site row changes the whole-function register
allocation and shrinks `render` to 9660/9664 B. The gate is green on the
pristine tree (byte 4507 / linked 4507 / size 4524 / data 843 / imports 207).

## Why the extra row exists (new mechanism)

With borderX applied but iconX/iconY left as locals inside the
`if (count == 4 && Globals::showBestDeal)` body:

| variant | fn size | `.text` | `.ARM.extab` | extab diff | raw |
|---|---:|---|---:|---:|---:|
| base (committed) | 9700 | 0x1337c8 | 156312 | 762 | 1719 |
| borderX | 9700 | 0x1337c8 | **156316** | **687** | **1686** |
| borderX + iconX/iconY at loop top | **9664** | 0x1337a8 | 156312 | — | 8569 |
| borderX + inline iconX/iconY | **9664** | 0x1337a8 | 156312 | — | 8569 |
| borderX + iconX/iconY before the `if` | 9700 | 0x1337c8 | **156316** | 687 | 1686 |
| borderX + inner-scope tmpX/tmpY | 9700 | 0x1337c8 | **156316** | 687 | 1686 |

Raw CodeGen IR (`-Xclang -disable-llvm-passes`) shows the cause: each `int`
local emits `llvm.lifetime.start`/`llvm.lifetime.end`; the `lifetime.end` is a
`CallLifetimeEnd` EH cleanup. When iconX/iconY are declared after the
`setPosition/replaceTextKeepSize/draw/setSplitText/DrawImage2D(tabIcons)` calls,
the if-body `DrawImage2D(scrollHint)` invoke unwinds to a **separate
landingpad** (its cleanup stack contains the two lifetime.end entries, the
earlier invokes' does not). That landingpad is a distinct call-site row:

```
orig 126 (6824, 210, 9654)              ; one row
ours 126 (6826,  96, 9656)              ; tabIcons call
ours 127 (7004,  30, 9602)              ; free-credits DrawImage2D
```

Declaring iconX/iconY *before* the earlier invokes (loop top) puts their
lifetime.end on the same cleanup stack for every invoke, so the landingpad
merges and the record is 948 B (156 rows, extab exact). But the earlier
`lifetime.start` changes the global greedy allocation: the `this` pointer stays
in `fp` (instead of being spilled to `[sp,#136]`) and `fp` is no longer free for
the `mov.w fp,#17` constant, so the function is 36 B shorter (9664 with
borderX, 9660 without). `.text` then shrinks by 32 B and `.rodata` shifts, so
`byte_exact` collapses (the 9660 variant measured 8577 raw and
`byte_exact` would drop exactly like the borderX +4 case).

All 24 declaration-position/form permutations tried give the same bimodal
result: *inside/just-before the if* -> 9700 + 157 rows; *anywhere before the
first invoke* -> 9660/9664 + 156 rows. Forms tried: `int iconX; int iconY;`,
`int iconX, iconY;`, zero-init, `= x`/`= y` init, `volatile`, `register`,
`static`, comma-operator braceless if, nested inner scope with tmpX/tmpY,
condition swap, nested if. Positions: before description, between the Strings,
after price, after setVisible, after `lay`, after switch, after row/col/x/y,
before setPosition, before the if, before the for, buyCredits block, function
top.

## borderX alone is not committable

`borderX` (the w8818 local) gives raw **1686** at 9700 B, but `.ARM.extab`
156316 (+4) shifts `.rodata` from 0x1c9e10 to 0x1c9e20. `check` on the current
base: **byte_exact 4507 -> 4044, RATCHET FAIL**. So borderX stays a candidate
until a size-neutral row merge is found.

## Open lead

The 36 B RA delta is *not* an alloca/stack-slot difference: frame size is 376 in
both. It is the `this`-in-`fp` vs `this`-spilled choice at 0x14911a. A lever
that forces `this` to spill while keeping iconX/iconY's lifetime.start before
the first invoke (so the landingpad merges) would give 9700 + 156312. Adding a
dummy int local before the if does not do it (tested: still 9700 + 157 rows);
the spill decision is made at function entry from the whole live-interval set.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9212
python3 /dev/shm/w9212/fdiff.py _ZN12HangarWindow6renderEv      # 1719, 9700
# borderX (9700, extab +4, check FAIL):
python3 /dev/shm/w9212/variants.py borderx && /dev/shm/w9212/measure.sh
# loop-top hoist (9664, extab exact, size regression):
python3 /dev/shm/w9212/variants.py borderx_hoist_loop_top && /dev/shm/w9212/measure.sh
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch (not committed), `/dev/shm/w9212/`: `fdiff.py`, `measure.sh`,
`variants.py`, `sweep_icon.py`, `sweep2.py`..`sweep7.py`, `cc.sh`, `cc0.sh`,
`HangarWindow.base.cpp`, `hw*.ll`, `raw*.ll`, `dis_*.txt`.
