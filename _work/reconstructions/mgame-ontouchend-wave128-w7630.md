# MGame::OnTouchEnd — wave 128 (w7630): weaponSlot body statement order

Status: one landed source tweak, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7630`,
base `4bbe88776`, owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w7630*` (not committed).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | before | after |
|---|---|---|
| fuzzy match (report) | 81.9 % | **81.9 %** |
| body size (orig/ours) | 8080 / 8080 | 8080 / 8080 |
| `.text` differing bytes | 85,520 | **85,519** |
| `.rel.plt` differing bytes | 0 | **0** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.ARM.extab` differing bytes | 1,436 | 1,436 |

## Change (statement order only, size-neutral)

```diff
             }
-            resumeSounds();
             this->pauseOpen = 0;
             this->hudMenuOpen = 0;
+            resumeSounds();
             this->hud->closeHudMenu();
         } else if (wingmanCommand >= 0) {
```

Original weaponSlot body (`0x17ba0c`):

```
17ba0c: movs r0,#0
17ba0e: strb r0,[fp,#93]      ; pauseOpen = 0
17ba12: strb r0,[fp,#214]     ; hudMenuOpen = 0
17ba16: mov  r0,fp
17ba18: blx  resumeSounds
17ba1c: ldr  r0,[fp,#116]
17ba20: blx  closeHudMenu
17ba24: b.w  17afe8
```

The landed order now matches instruction-for-instruction (the previous order emitted
`resumeSounds` first, then the two stores). The 1-byte `.text` delta is the moved
`strb`/`blx` pair; the fuzzy score rounds to the same 81.9 %.

## Gate (full `check`)

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` CLEAN, `drift` 0. `.rel.plt`, `.ARM.exidx`, `.rodata`, `.data`, `.bss`, `.dynsym`,
`.dynstr`, `.hash`, `.rel.dyn` all byte-identical.

## Remaining root cause (verified against LLVM 7.0.2 sources)

The dominant remaining difference is **block placement of the function tail**
(`if (player->isAutoPilot() && !wasAutoPilot)`, original `0x17afe8`, ours `0x17b976`) and of the
`hudTouchFlags == 0` cold block (original `0x17b01a`, ours `0x17aebe`).

The original's layout around the hudMenu region is:

```
17ad4c  hudTouchFlags lsls chain (0x200,0x800,0x400,0x2000..0x1000)
17adc0  0x200 case: ldrd; movs r1,#1; b 17afde      <- branches forward
17ae8c  0x800 case: ...; b 17afe2
17afd8  0x400 case: ldrd; movs r1,#2                 <- falls through
17afde  blx Hud::initHudMenu                         <- shared tail
17afe2  ldr hudTouchFlags; cbz r0, 17b01a            <- test, cold branch
17afe8  isAutoPilot tail
17b004  canary return
17b01a  hudTouchFlags == 0 cold block
17b07e  failed type==12
17b0aa  resetStartSequenceOver
17b0b6  campaign chain ...
```

Ours merges the other way: the **0x200** body owns the shared `blx` and the 0x400 body branches
back; the test therefore sits right after the 0x200 body (`0x17aeb4`), the 0x800 body is the
fallthrough, and the function tail is pushed after the whole campaign chain (`0x17b976`).

The merge survivor is chosen by `BranchFolder::CreateCommonTailOnlyBlock`
(`llvm/lib/CodeGen/BranchFolding.cpp`, release_70):

* `ComputeSameTails` fills `SameTails` from the highest block number down, so
  `SameTails[0]` is the **higher** block number.
* `CreateCommonTailOnlyBlock` iterates `SameTails` and on a runtime-estimate tie
  (`t <= TimeEstimate`) keeps the **last** index, i.e. the **lowest** block number.
* It prefers `PredBB` (the test block's layout predecessor) if that block is one of the tails.
  `PredBB = &*std::prev(I)` where `I` is the merge-target block (the test).

Both `initHudMenu` prefixes are `ldrd r0,r2,[fp,#116]` (mayLoad, 2) + `movs r1,#N` (1) = 3, so the
estimate always ties. In our build the test block's layout predecessor is the 0x800 `useCloak`
body (not a tail), so the tie rule picks the lower block number = the **0x200** body. The original
must have had the **0x400** body as the test's layout predecessor (or a lower block number).

Every source-level lever that changes the case order also changes the emitted `lsls` test order
(the original is 0x200, 0x800, 0x400, 0x2000, ...), so the two are coupled.

## Variants measured this pass (all reverted)

| variant | size | pct | `.text` diff | notes |
|---|---:|---:|---:|---|
| **landed (weaponSlot reorder)** | **8080** | **81.9** | **85,519** | `.rel.plt` 0 |
| base | 8080 | 81.9 | 85,520 | |
| nested `else { if (wingman) ... if (hud0) ... }` | 8080 | 81.8 | 85,524 | original CFG shape, layout worse |
| `else if (!hudTouchFlags)` | 8080 | 81.9 | 85,520 | canonicalised to base |
| `__builtin_expect(hudTouchFlags == 0, 1)` | 8080 | 81.9 | 85,520 | no-op |
| `__builtin_expect(hudTouchFlags == 0, 0)` | 8076 | 78.1 | — | size change |
| `__builtin_expect(isAutoPilot && !wasAutoPilot, 0)` | 8080 | 80.6 | — | worse |
| `__builtin_expect(isAutoPilot && !wasAutoPilot, 1)` | 8076 | 80.8 | — | size change |
| `!wasAutoPilot && isAutoPilot()` swap | 8076 | 80.8 | — | size change |
| separate `if (hudTouchFlags == 0)` after chain | 8076 | 79.0 | — | size change |
| `hudTouchFlags == 0` as first chain branch | 8076 | 75.3 | — | size change |
| 0x400 case moved to end of chain | 8076 | 77.5 | — | test order changes |
| `won` bare `hasWon()` (drop `== true`) | 8064 | 77.0 | — | size change |
| `keepObjectives` bare `hasWon()` | 8080 | 81.9 | 85,520 | canonicalised |
| mining `if (amount != 0)` / `String(-amount)` | 8064 | 74.3 | — | flips counter, size |
| mining `if (count != 0)` | 8064 | 74.3 | — | flips counter |
| mining `String(0 - amount)` | 8064 | 74.3 | — | flips counter |

## Mining guard (unchanged this pass)

Original `0x17a5ca`: `cmp.w r9,#0 ; beq` (`if (amount != 0)`), then `rsb r1,r9,#0`
(`String(-amount)`), with `amount` decremented (`sub.w r9,r9,#1`). Ours emits
`cmp.w r9,#-1 ; bgt` (`int count = -amount; if (count > 0)`). Every `amount != 0` spelling lets
clang 7.0.2 canonicalise the decrementing counter to an incrementing one and shrinks the body to
8064 B, so the guard cannot be fixed without a size-compensating anchor.

## Next attack

The tail-merge survivor is the whole lever. A shape that makes the **0x400** body the test's
layout predecessor (or gives it a lower MachineBasicBlock number) while keeping the emitted
`lsls` test order 0x200/0x800/0x400 would flip the merge, pull the test to `0x17afe2`, and with it
the `isAutoPilot` tail and the cold blocks — the ~1,800-line side-by-side cascade. Reordering the
`else if` chain does not work because it changes the test order.
