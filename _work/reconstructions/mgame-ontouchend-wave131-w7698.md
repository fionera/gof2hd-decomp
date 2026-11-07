# MGame::OnTouchEnd — wave 131 (w7698): mining-counter spellings, chain order, won/keepObjectives shapes

Status: one landed source tweak (the w7676 weaponSlot statement swap, re-landed here), 2026-11-07.
Worktree `/home/debian/GalaxyOnFire2/wt/w7698`, base `7c6933c2d`, owned TU
`src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w7698` (not committed).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | before | after |
|---|---|---|
| raw fuzzy (difflib, autojunk off) | 81.9247 % | **81.9607 %** |
| report pct | 81.9 | 81.9 |
| body size (orig/ours) | 8080 / 8080 | 8080 / 8080 |
| `.text` differing bytes | 85,379 | 85,379 |
| `.ARM.extab` differing bytes | 1,435 | 1,435 |
| `.rel.plt` / `.ARM.exidx` differing bytes | 0 / 0 | 0 / 0 |

## Landed change (statement order only, size-neutral)

The w7676 worktree's weaponSlot-body swap was not yet in this tree; re-landing it here:

```diff
             }
-            this->pauseOpen = 0;
-            this->hudMenuOpen = 0;
+            this->hudMenuOpen = 0;
+            this->pauseOpen = 0;
             resumeSounds();
             this->hud->closeHudMenu();
         } else if (wingmanCommand >= 0) {
```

Original emission at `0x17ba0c`:

```
17ba0c: movs r0,#0
17ba0e: strb r0,[fp,#93]      ; pauseOpen = 0
17ba12: strb r0,[fp,#214]     ; hudMenuOpen = 0
17ba16: mov  r0,fp
17ba18: blx  resumeSounds
17ba1c: ldr  r0,[fp,#116]
17ba20: blx  closeHudMenu
```

The source order `hudMenuOpen; pauseOpen` (not the emitted order) is what makes our emission
match instruction-for-instruction; the two stores are byte-identical either way, so the
whole-`.text` differing-byte count is unchanged (85,379) while the fuzzy score rises.

## Gate (full `check`)

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` CLEAN, `drift` 0. `.rel.plt`, `.ARM.exidx`, `.rodata`, `.data`, `.bss` byte-identical.
Section deltas unchanged: `.text` 85,379, `.ARM.extab` 1,435, `.note.gnu.build-id` 20.

## New hypotheses tested this wave (all reverted; base = 81.9607 / 8080)

The mining guard is still the only known source-level switch that flips the tail layout
(`if (amount != 0)` → 0x400 body owns the shared `blx`, original order), but it also flips the
register allocation (`this` moves `sl` → `r9`, `Globals::status` `r9` → `sl`), which is a
whole-function diff and drops the score to ~73.5. This wave tried to defeat the counter
canonicalisation without changing the register allocation.

### Mining guard / counter spellings

Every spelling below emits the original `cmp #0; beq` guard but canonicalises the counter to the
incrementing `add r8,r8,#1` + `mov r1,r8` form and shrinks the body to **8064 B**:

| spelling | size | pct |
|---|---:|---:|
| base `int count = -amount; if (count > 0)` | 8080 | 81.9607 |
| `if (amount != 0) String(-amount)` | 8064 | 73.4090 |
| `bool hasAmount = amount != 0; if (hasAmount)` | 8064 | 73.4090 |
| `if (amount != 0) { int count = -amount; ... }` | 8064 | 73.4090 |
| `if (0 != amount)` | 8064 | 73.4090 |
| `if ((unsigned) amount != 0)` | 8064 | 73.4090 |
| `if (amount - 0 != 0)` | 8064 | 73.4090 |
| `if ((amount \| 0) != 0)` | 8064 | 73.4090 |
| `if ((amount ^ 0) != 0)` | 8064 | 73.4090 |
| `int count = -amount;` before guard, `if (amount != 0)` | 8064 | 73.4090 |

The canonicalisation is triggered by the `icmp ne`/`icmp eq` predicate on the decrementing phi;
`icmp slt` (the base's `count > 0`) keeps the decrementing `sub r9,r9,#1`. It happens below IR
(the `.ll` is already `%amount = phi [0], [%amount-1]` + `icmp eq %amount, 0`).

### Counter type / width

| variant | size | pct |
|---|---:|---:|
| `short amount = 0` + base guard | 8084 | 77.5996 |
| `short amount = 0` + `amount != 0` | 7948 | 48.6044 |
| `unsigned char amount = 0` + base guard | 7876 | 65.2643 |

`short` adds `sxth.w r8,r9` (4 B) and still emits `cmp #-1; bgt`; it does not help.

### Control-flow shapes (all 8080 unless noted)

| variant | size | pct |
|---|---:|---:|
| base (landed swap) | 8080 | **81.9607** |
| inverted `if (!won) { resetStartSequenceOver(); } else { ... }` | 8080 | 81.9607 |
| `keepObjectives` as `bool = false; if (campaign->hasWon()) ...` | 8080 | 81.9607 |
| `hasFailed()` bare | 8088 | 72.6847 |
| `hasWon()` second operand bare | 8064 | 76.3210 |
| `menuWindow->OnTouchEnd(...)` bare | 8080 | 80.5335 |
| drop `campaign` temp (3× `getCampaignMission()`) | 8084 | 80.9009 |
| separate `if (hudTouchFlags == 0)` | 8076 | 79.0916 |
| separate `if` + `hasFailed()` bare | 8080 | 76.4017 |
| 0x400 case swapped before 0x200 case | 8060 | 77.5032 |
| `failed`+`menu` bare | 8080 | 72.0000 |
| `failed`+`won` bare | 8068 | 73.1338 |
| `won`+`menu` bare | 8064 | 75.8161 |
| `failed`+`won`+`menu` bare | 8064 | 72.5077 |

The inverted `won` and the `keepObjectives` if-without-else canonicalise to the same IR/codegen
as the base (identical score to 4 decimals), so they are not levers.

## Remaining root cause (unchanged, re-confirmed)

Original hudMenu/tail layout:

```
17ad4c  hudTouchFlags lsls chain (0x200, 0x800, 0x400, 0x2000 .. 0x1000)
17adc0  0x200 case: ldrd; movs r1,#1; b 17afde       <- branches forward
17adc8  won prologue (hasWon / getCampaignMission / keepObjectives / mission)
17ae8c  0x800 case: ...; b 17afe2
17af5a  keepObjectives else (mov.w sl,#0) + mission selection
17afd8  0x400 case: ldrd; movs r1,#2                 <- falls through
17afde  blx Hud::initHudMenu                         <- shared tail
17afe2  ldr hudTouchFlags; cbz r0, 17b01a            <- test, cold branch
17afe8  isAutoPilot tail
17b004  canary return
17b016  stack_chk_fail
17b01a  hudTouchFlags == 0 cold block
17b07e  failed type==12
17b0aa  resetStartSequenceOver
17b0b6  campaign chain ...
```

Ours places the 0x200 body + shared `blx` first, the test right after it, the cold block right
after the test, and the function tail after the whole campaign chain. The BranchFolder
`commonTailIndex` selection (`llvm/lib/CodeGen/BranchFolding.cpp` release_70, ~1009–1035) picks
the survivor from `PredBB` (the test block's layout predecessor). Instrumented `llc` on this
tree reports:

```
TMB: IBB=471 PredBB=434 pots: 434(h298562) 436(h53826) 438(h298562) 455(h6528) 456(h53826)
ST: n=2 pred=434 cti=1 tails: 438(imm=2) 434(imm=1)
```

i.e. the 0x200 body (434) is the test's layout predecessor, so the merge keeps it. The original
must have the 0x400 body (438) as the test's layout predecessor. `MachineBlockPlacement` runs
*after* BranchFolder, so the decision is made on the pre-placement IR order after the first
`OptimizeFunction` iteration (`OptimizeBranches`/`HoistCommonCode`).

## Next attack

1. Find a source spelling that keeps `sub.w r9,r9,#1` + `cmp #0; beq` without moving `this` off
   `sl`. The predicate is the trigger; the rewrite is below IR (SelectionDAG). Candidates: an
   opaque use of `amount` that blocks the DAG's `sub 0, x` reassociation without adding code,
   or a loop shape whose induction phi is not a simple `add -1`.
2. Or find a non-mining lever that makes the 0x400 body the test's layout predecessor while
   keeping the emitted `lsls` test order 0x200/0x800/0x400 and the 8080 B size.
