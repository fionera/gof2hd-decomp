# `HangarWindow::render` — wave 132 (w7715): CFG match via goto + `assume`, reload is the last 4 bytes

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7715`,
base `70e451ffe` (wave-131 PlayerJumpgate; `render` 94.747001 %, gate byte 4506 /
linked 4506).

This pass: **94.747001 -> 94.747001** (raw difflib over the reconciled instruction
stream; 141 diffops, 3626/3627 insns). No source change landed; the deliverable is
this evidence record. `check` is green (byte 4506, linked 4506, size 4524, data 843,
imports 207, lint CLEAN, drift 0).

## New hypothesis (from wave 129)

Wave 129 pinned the blocker as the *asymmetric* Jump Threading: the ship-equal edge
is threaded to the join, the case-0 edge is not, and a uniform cost bump blocks both.
The suggested lever was to form the switch late, or to make the ship-equal edge reach
the join directly. This wave tried the second lever directly.

## Finding 1: a `goto` to a label after the upgrade block is a no-op by itself

```cpp
if (tab != 0) {
    if (tab == 3 && Globals::status->getShip() == this->selectedItem->ship)
        goto ship_done;
    { ...button 1 body... }
}
if (this->upgradeMode && tab == 1) { ...buttons 2,10... }
ship_done: ;
```

Result: **94.747001 %, 9700 B, 141 diffops — byte/insn-identical to the landed
source**. The compiler merges the empty `ship_done:` block into the join, so the
ship-equal edge still reaches the upgrade block and JT still threads it. The ship
branch stays:

```
ldr  r0,[sp,#108]
ldrd r5,r4,[sp,#132]
cmp  r0,#0 ; beq.w join      <- case 0 threaded (wrong)
cmp  r0,#3 ; bne.n body
... getShip() first ...
beq.w join                    <- ship-equal threaded (right)
```

## Finding 2: `goto` + a non-foldable `assume` reproduces the original CFG exactly

Adding `__builtin_assume(tab < 6)` to the upgrade condition raises the JT
duplication cost of the upgrade block from 5 to 7 (the assume is an intrinsic call:
+1 base +1 intrinsic; the `icmp` is +1), so JT stops threading. With the `goto`
above, the ship-equal edge is already a direct edge to the join, so **only** the
case-0 edge remains at the upgrade block:

```
ldr  r0,[sp,#108]
ldrd r5,r4,[sp,#132]
cbz  r0, upgrade             <- case 0 -> upgrade test (MATCHES original)
cmp  r0,#3 ; bne.n body
... getShip() first ...
beq.w join                    <- ship-equal -> join (MATCHES original)
```

This is the first source shape found that reproduces the original's asymmetric
threading. The same CFG is obtained with the base operand order
(`selectedItem->ship == getShip()`) and with any non-foldable assume
(`tab < 6`, `tab != 5`, `tab < 100`, `Globals::status != 0`, `this != 0`).

## Finding 3: the remaining 4 bytes are the original's `ldrd r5,r4` reload

The original's ship branch is 38 bytes (`0x149b1e`..`0x149b44`); the goto+assume
branch is 34 bytes (`0x149b1e`..`0x149b40`). The whole function differs by exactly
those 4 bytes — every instruction after the ship branch is identical, just shifted:

```
orig:  149b2a ldr r0,[pc,#340]       ours: 149b2a ldr r0,[pc,#336]
       149b2c ldr r1,[r4,#104]             149b2c add r0,pc
       149b2e add r0,pc                    149b2e ldr r0,[r0,#0]
       149b30 ldr r0,[r0,#0]               149b30 ldr r0,[r0,#0]
       149b32 ldr r4,[r1,#12]              149b32 blx getShip
       149b34 ldr r0,[r0,#0]               149b36 ldr r1,[r4,#104]
       149b36 blx getShip                  149b38 ldr r1,[r1,#12]
       149b3a cmp r4,r0                    149b3a cmp r0,r1
       149b3c ldrd r5,r4,[sp,#132]  <--    149b3c beq.w join
       149b40 beq.w join
```

The missing instruction is the reload of the `(layout, this)` pair. It exists in
the original because the original allocator put the ship value in **r4** (the same
register as `this`), clobbering `this`, and then rematerialises both from
`[sp,#132]`. Ours never clobbers `this`:

| form | `ldrd` pair | ship value | reload |
|---|---|---|---|
| base order (`ship == getShip()`) | `ldrd r6,r5` (layout r6, this r5) | r4 | no |
| flipped order (`getShip() == ship`) | `ldrd r5,r4` (layout r5, this r4) | r1 (after call) | no |
| original | `ldrd r5,r4` | r4 (before call) | **yes** |

The base order gets the right evaluation order (ship loaded before the call) but
the allocator keeps `this` in r5; the flipped order gets the right pair but loads
the ship after the call into r1. The original is the combination: right pair **and**
ship-before-call, with `this` spilled across the call.

## Finding 4: the 4-byte shift costs 116 branch-target diffops

Of the 246 diffops in the goto+assume variant, **116 are branch-target-only**
(same mnemonic, target shifted by 4). The other 130 are the pre-existing
`sl`/`r9` and `r5`/`r2` allocation ties that the 94.7 % base already has. So if the
4-byte reload could be recovered, the variant would score ~130 diffops — slightly
better than the landed 141 — and the CFG would be exact.

The 4-byte deficit also means the variant cannot be landed as-is: `.text` is
currently size-exact, and a 4-byte shrink would break the section-size match.

## Variants measured this wave (all reverted)

| variant | pct | size | note |
|---|---:|---:|---|
| landed base | **94.747001** | 9700 | 141 diffops |
| `goto` alone (either operand order) | 94.747001 | 9700 | merged away, no-op |
| `goto` + `assume(tab<6)`, flip order | 90.3 | 9696 | CFG exact, 116 branch-shift diffops |
| `goto` + `assume(tab<6)`, base order | 90.0 | 9696 | CFG exact, pair r6,r5 |
| `goto` + `assume(tab!5 / tab<100 / status!=0 / this!=0)` | 90.0–90.3 | 9696 | same |
| `goto` + `assume(tab==tab)` | 94.747001 | 9700 | folds to true, no-op |
| `goto` + `assume` + no `padX` | 88.9 | 9696 | `this`/`layout` from stack every use |
| `goto` + `assume` + `padX` = `(volatile int*)this/item/&tab` | 90.0 | 9696 | no pair change |
| `goto` + `assume` + `volatile int *volatile padX` | 88.2 | 9704 | pair r8,r5 |
| upgrade test re-reads `hangarList->getCurrentTab()` (no goto) | 90.1 | 9696 | case 0 unthreaded via a real `getCurrentTab()` PLT call, ship-equal also unthreaded |
| upgrade test `*(volatile int*)&tab == 1` + goto | 55.6 | 9656 | address-taken `tab` kills promotion; whole function reallocates |
| `Ship *sel = selectedItem->ship;` + `getShip() == sel` + goto/assume | 86.8 | 9688 | ship in r4, but reloads `item` (r4) from `[sp,#120]`, body uses this=r5 |

## Conclusion / open questions

The asymmetric JT is now reproducible from source: `goto ship_done` (ship-equal
direct to the join) + a non-foldable `assume` in the upgrade block (cost 5 -> 7,
blocks the case-0 thread). The remaining delta is exactly the original's
`ldrd r5,r4` reload, which is a register-allocation tie-break: the original spills
`this` across the `getShip()` call and reuses r4 for the ship value; our allocator
keeps `this` in a callee-saved register (r5 in the base order, r4 in the flip).

Next lever: force the allocator to spill `this` across the call while keeping the
ship value in r4. Source forms that add a live-across-call local, change the
`(layout, this)` pair order, or perturb `padX`'s liveness have not done it yet
(the pair is strongly tied to the operand order). Alternatively, find a cost bump
that does not perturb the pre-ship-branch allocation at all, so the base order's
`ldrd r6,r5` may still be recoverable to `r5,r4`.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

The goto+assume variant (not landed) is:

```cpp
if (tab != 0) {
    if (tab == 3 && Globals::status->getShip() == this->selectedItem->ship)
        goto ship_done;
    { ...button 1 body... }
}
if (__builtin_assume(tab < 6), this->upgradeMode && tab == 1) { ...buttons 2,10... }
ship_done: ;
```

Scratch (not committed): `/dev/shm/w7715/{score.py, try.py, v2.py, v3.py,
variants.py, batch*.py, sweep_assume.py, HangarWindow.base.cpp, pa2.txt,
JumpThreading.cpp, SimplifyCFG.cpp, LazyValueInfo.cpp}`.
