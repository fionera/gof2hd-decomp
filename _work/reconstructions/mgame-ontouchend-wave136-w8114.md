# MGame::OnTouchEnd — wave 136 (w8114): moving the wingman body off the join test's PredBB

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8114`,
base `2e51d765d`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8114`. **No source change landed**: every shape that moves the wingman
body either folds back to the base bytes or re-lays-out the function and loses
size/score.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

Baseline (verified this wave, full `check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
raw fuzzy (difflib, autojunk off): 81.9607 %   size 8080/8080   insns 2774/2775
```

## 1. Hypothesis under test (wave 134 w7758 §5 lead 3)

> The `flags == 0` join test's `PredBB` is the wingman tail. If the wingman body
> could be moved out of the block-list slot immediately before the join test
> (without changing the CFG), `PredBB` would be a different block. The
> `goto hbMenu*` shapes change the merge point instead of `PredBB`.

The idea is that `BranchFolder::CreateCommonTailOnlyBlock`
(`BranchFolding.cpp:784`) would then pick the 0x400 `initHudMenu(2)` body as the
shared-`blx` survivor (the original's ownership) instead of the 0x200 body.

## 2. Ground truth re-confirmed (objdump, original)

```
17ad4c  ldr.w r0,[fp,#248]; tst #512; bne 17adc0      ; 0x200 test first
17ad56  lsls #20; bmi 17ae8c                           ; 0x800
17ad5c  lsls #21; bmi 17afd8                           ; 0x400
17adc0  ldrd r0,r2,[fp,#116]; movs r1,#1; b 17afde     ; 0x200 body -> shared tail
17afd8  ldrd r0,r2,[fp,#116]; movs r1,#2               ; 0x400 body, falls through
17afde  blx Hud::initHudMenu                           ; shared tail (0x400 owns it)
17afe2  ldr flags; cbz 17b01a                          ; join test: flags == 0
```

## 3. The base block order (real NDK clang 7.0.2, `-mllvm -print-after-all`)

Pre-CFO (After Prologue/Epilogue Insertion) MIR, hud cluster:

```
bb.425 (ir 2419)  0x200 test      -> bb.427 (0x800 test) / bb.426 (0x200 body)
bb.426 (ir 2426)  0x200 body      -> bb.463 (join test)
bb.427 (ir 2433)  0x800 test
bb.428 (ir 2436)  0x800 body      -> bb.463
bb.429 (ir 2445)  0x400 test
bb.430 (ir 2448)  0x400 body      -> bb.463
...
bb.449 (ir 2489)  weaponSlot body
bb.451 (ir 2511)  weaponSlot tail -> bb.471 (epilogue)
bb.452 (ir 2520)  wingman body (getEnemies + loop)
bb.462 (ir 2558)  wingman tail    -> bb.471 (epilogue)
bb.463 (ir 2567)  join test       -> bb.464 (flags body) / bb.471 (epilogue)
```

So the join test's **block-list predecessor is `bb.462`, the wingman tail**
(confirmed: `bb.462` is printed immediately before `bb.463`). The join test's
*CFG* predecessors are `[bb.426 (0x200 body), bb.428 (0x800 body),
bb.430 (0x400 body), bb.447 (0x1000 test), bb.448 (0x1000 body)]`.
`SameTails` for the shared `blx` is `[bb.430, bb.426]`; the base tie-break keeps
the last (`bb.426` = 0x200 body).

## 4. Variants measured this wave (all reverted; base restored)

| variant | pct (raw) | size | notes |
|---|---:|---:|---|
| base | **82.0** (81.9607) | 8080 | join test PredBB = wingman tail; 0x200 owns `blx` |
| **swapAB**: wingman `else if` before weaponSlot | 82.0 (81.9607) | 8080 | **byte-identical to base**; PredBB becomes the weaponSlot tail (`bb.451`), still not in `SameTails` |
| `goto hbWingman` body after the join test | 77.8 (77.7778) | 8064 | wingman test optimized away (flags==0 implies wingmanCommand==-1); -16 B |
| defer 0x200 body after the join test | 80.8 (80.7713) | 8084 | 0x200 body branches straight to epilogue (skips join test); +4 B |
| defer 0x200 body to end of dispatch chain (`else goto`) | 76.2 (76.2282) | 8108 | +28 B, re-layout |
| move `flags == 0` case into the dispatch chain after 0x400 | 76.1 (76.0634) | 8084 | +4 B; **tail merge disappears** (0x400 body keeps its own `blx`) |

### Key negative result

**Moving the wingman body does not flip the survivor.** `swapAB` is
byte-identical: the join test's PredBB changes from the wingman tail
(`bb.462`) to the weaponSlot tail (`bb.451`), but neither block is in
`SameTails`, so `CreateCommonTailOnlyBlock` still falls through to the
`EstimateRuntime` tie-break and keeps the 0x200 body. The PredBB lever only
matters if the new predecessor is *one of the two `initHudMenu` bodies*.

## 5. Why `PredBB == 0x400 body` is not reachable from the wingman body

`PredBB` is `std::prev(I)` for `IBB` = the join test (`BranchFolding.cpp:1123`,
`1230`). It is the *IR/ISel block-list* predecessor, not the MBP layout
predecessor. The join test is the last block of the second `if/else-if` chain,
so its predecessor is always the last block of that chain (wingman tail in the
base, weaponSlot tail after `swapAB`). The 0x400 body (`bb.430`) is in the
*first* chain, separated from the join test by `bb.431..bb.462`. To make it the
predecessor the join test itself must move up to immediately after `bb.430`
(measured: "flags==0 case in the dispatch chain after 0x400" -> 8084 B, 76.1,
and the tail merge is lost because the join test's predecessor list changes).

## 6. Open question: the EstimateRuntime lever looks inconsistent

Wave 134 (w7777) reported that a 0x200 prefix made *strictly more expensive*
than the 0x400 prefix (`hudMenuOpen = 1;` store in the 0x200 body: 8 vs 5) still
picks the 0x200 body, which would require `PredBB == 0x200 body` (the (A) early
break in `CreateCommonTailOnlyBlock`). We reproduced that variant in this
worktree: **8084 B, raw 80.7568, 0x200 body owns the `blx`** — but the pre-CFO
dump still shows the join test's predecessor is the wingman tail (`bb.462`), not
the 0x200 body. So either the `SameTails` order / `getTailStartPos()` differs
from the printed MIR, or the early (A) break is reached through a different
`IBB`. This is the one unresolved mechanism; it is not a source lever that
produced a win.

## 7. Reproduction

```
cp /dev/shm/w8114/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8114/score.sh          # 82.0 / 8080
python3 /dev/shm/w8114/rawscore2.py      # 81.9607 / 8080 / 2774:2775
```

Variant sources: `/dev/shm/w8114/variant_swapAB.cpp`,
`variant_gotowingman.cpp`, `variant_defer200.cpp`, `variant_defer200mid.cpp`,
`variant_flagsinchain.cpp`, `variant_vstore.cpp`.

MIR dumps: `/dev/shm/w8114/pa.sh` (`-mllvm -print-after-all`, filtered to
`_ZN5MGame10OnTouchEndEiiPv`); `pre_cfo.txt` / `post_cfo.txt` / `mbp.txt` are the
base dumps.

The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; the
original 0x200/0x400 bodies are at `0x17adc0` (`ldrd; movs r1,#1; b 0x17afde`)
and `0x17afd8` (`ldrd; movs r1,#2`; falls through to `blx` at `0x17afde`).
