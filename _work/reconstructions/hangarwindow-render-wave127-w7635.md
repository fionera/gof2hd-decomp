# `HangarWindow::render` — wave 127 (w7635): ship-branch comparison operand flip

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7635`,
base `4bbe88776` (wave-126 final; `render` 94.388529 %).

This pass: **94.388529 -> 94.747001** (raw difflib over the reconciled
instruction stream). Size stays 9700 B, insn count stays 3626/3627.

## Landed source change

Ship branch only, 1 line:

```diff
-                                if (!(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {
+                                if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
```

The two pointer comparisons are semantically identical; only the C++ operand
order (and therefore clang's left-to-right evaluation order) changes.

## Result

| metric | before (HEAD 4bbe88776) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 94.388529 | **94.747001** |
| `render` size | 9700 | 9700 |
| normalized insns | 3626 / 3627 | 3626 / 3627 |
| difflib diff ops | 248 (wave-126) | **141** |
| difflib matched insns | 3423 (wave-126) | **3436** |
| byte_exact / linked_exact | 4506 / 4506 | 4506 / 4506 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`check`: **RATCHET PASS** — `verify: byte 4506 linked 4506 stubs 0 missing 0
extra 0 avg 99.94`; `data: exact 843 of 843`; `sodiff: imports 207`.

## Why the operand flip matters (register allocation, not the fold)

Before the change the ship branch allocated `layout = r6`, `this = r5`; the
original has `layout = r5`, `this = r4`. That one-register shift cascaded
through the whole ship body (`this`/`layout` every dereference), ~40
structural diff ops. After the flip the pair is:

```
149b20: ldrd r5, r4, [sp,#132]      ; r5 = &Globals::layout, r4 = this   (matches orig)
```

and every body use (`ldr r0,[r4,#36]`, `ldr r3,[r5,#0]`, ...) is now
instruction-identical. The flip changes the order in which clang emits the
`getShip()` call and the `this->selectedItem->ship` load, which perturbs the
Greedy allocator's live ranges for the two reload vregs and lands the original's
`r4`/`r5` assignment.

## Remaining ship-branch residual (3 independent differences)

Original `0x149b1e`..`0x149b44` vs ours now:

```
149b24: cbz  r0, 149b96             |  cmp  r0,#0 ; beq.w 14a6d0   ; tab==0 edge
149b2a: ldr  r0,[pc,#340]           |  ldr  r0,[pc,#336]           ; getShip first in ours
149b2c: ldr  r1,[r4,#104]           |  add  r0,pc
149b2e: add  r0,pc                  |  ldr  r0,[r0,#0]
149b30: ldr  r0,[r0,#0]             |  ldr  r0,[r0,#0]
149b32: ldr  r4,[r1,#12]            |  blx  getShip
149b34: ldr  r0,[r0,#0]             |  ldr  r1,[r4,#104]
149b36: blx  getShip                |  ldr  r1,[r1,#12]
149b3a: cmp  r4,r0                  |  cmp  r0,r1
149b3c: ldrd r5,r4,[sp,#132]        |  (no reload)
149b40: beq.w 14a6d0                |  beq.w 14a6d0
```

1. **`tab==0` fold**: ours threads the switch case-0 edge straight to the join
   (`cmp/beq.w`, 6 B); the original keeps it on the `tab==1` test (`cbz`, 2 B)
   and pays 4 B for a `ldrd r5,r4` reload. Net size is equal.
2. **evaluation order**: the original loads `selectedItem->ship` *before* the
   `getShip()` call (source `ship == getShip()`); ours now calls `getShip()`
   first (source `getShip() == ship`). This is the price of the register win.
3. **no reload**: because ours calls `getShip()` first, the ship pointer lands
   in `r1` and `this` (r4) survives, so the `ldrd r5,r4` reload is absent.

The base form (`ship == getShip()`) has the original's evaluation order and
reload but the wrong `r5`/`r6` registers; the flipped form has the right
registers but the wrong order. The register win is worth far more than the
3-instruction order/reload difference, hence the +0.36 %.

## Root cause of the fold (re-confirmed this wave)

`-mllvm -print-after-all` on the current source: the ship switch is formed in
the first `Simplify the CFG` with `case 0 ->` the upgrade-test block, survives
inlining/SROA/EarlyCSE, and is re-targeted to the `buttonOffset` join by
**Jump Threading** (not the inliner — wave 126 localized it one pass late).
The upgrade-test block is 5 IR instructions (gep, load, icmp, icmp, and), below
`-jump-threading-threshold` (default 6), so Jump Threading threads the edge.

## Variants measured this wave (all on the wave-126 base)

| variant | pct | size |
|---|---:|---:|
| base `ship == getShip()` | 94.388529 | 9700 |
| **`getShip() == ship` (LANDED)** | **94.747001** | 9700 |
| `getShip() == (*this).selectedItem->ship` | 94.747001 | 9700 |
| `tab != 3 \|\| getShip() != ship` | 94.747001 | 9700 |
| `ship != getShip()` inverted / `!(ship != getShip())` | 94.388529 | 9700 |
| `Ship *selShip = this->selectedItem->ship;` local (either order) | 86.45 / 86.43 | 9684 |
| `goto` ship_done form | 94.388529 | 9700 |
| explicit empty `if (tab==3 && ship==getShip()) {} else {body}` | 94.388529 | 9700 |
| `if (tab != 0 && !(...))` compound | BUILDFAIL (brace) | - |
| `padX = &tab;` / `(void)padX;` | 94.388529 | 9700 |
| `upgradeMode != 0` | 94.388529 | 9700 |
| nested `if (upgradeMode) { if (tab==1) ... }` | 94.388529 | 9700 |
| nested `if (tab==1) { if (upgradeMode) ... }` | 94.333379 | 9700 |
| `bool upTab = upgradeMode && tab==1;` | 86.620690 | 9684 |
| `upgradeMode && tab==1 && padX==0` | 94.388529 | 9700 |
| `volatile int *volatile padX` | 86.809097 | 9708 |

Fold-block cost probes (all on the landed base; none moved the fold):
`((tab==1)&(tab!=2))`, `tab>=1`, `tab<=1`, `-tab==-1`, `(unsigned)tab==1u`
all 94.747001 / 9700. `tab==1 && (tab&1)` re-allocates (90.197160 / 9696).
Adding a real second test (`hangarList != 0`, `layout != 0`, `buttons != 0`)
does block the fold but adds 4-8 B and drops to ~90.0.

## Other regions probed this wave (no win; all on the landed base)

Buy-credits (`strd sl,r9` vs `str.w r9` layout spill) — none moved the tie:
`buyCreditsActive != 0`, `listModeFlag != 0`, `4 == count`,
`showBestDeal && count == 4`, `count < 5`, `String("",false)`,
`String price` first (94.636702). Reordering the options chain (`opts_flip`)
regressed to 94.471253.

Free-credits (`sl`/`r9` roles + volatile-canvas spill) — the wave-125 shape is
still locally optimal: dropping the `rh` local (or moving it after `getWidth`)
gives 94.719426, dropping `volatile` from the canvas gives 94.347946, both
94.719426. The original loads rowHeight *after* `getWidth` (0x14ad32), but the
`rh`-before-width form still scores higher overall.

Blueprint `ldrd sl,r9` swap: not probed further this wave; the ship-branch
operand flip did not perturb it.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7635/{score.py,bs.sh,try.py,try2.py,
HangarWindow.base.cpp,HangarWindow.cmpflip.cpp,show_base.txt,
show_cmpflip.txt,pa.txt,hw.ll,hw_raw.ll}`.

## Remaining residual (unchanged elsewhere)

The four wave-124 ties are now three: the ship register tie is fixed. The
blueprint `ldrd sl,r9` swap, the buy-credits `&Globals::layout` spill, and the
free-credits `sl`/`r9` role swap remain.
