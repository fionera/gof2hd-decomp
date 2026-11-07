# MGame::OnTouchEnd — wave 130 (w7676): weaponSlot body order + tail-merge mechanism audit

Status: one landed source tweak, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7676`,
base `003a9623a`, owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w7676` (not committed).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | before | after |
|---|---|---|
| fuzzy match | 81.9247 % | **81.9607 %** (report rounds both to 81.9) |
| body size (orig/ours) | 8080 / 8080 | 8080 / 8080 |
| `.text` differing bytes | 85,441 | 85,441 |
| `.rel.plt` / `.ARM.exidx` differing bytes | 0 / 0 | 0 / 0 |
| `.ARM.extab` differing bytes | 1,427 | 1,427 |

## Landed change (statement order only, size-neutral)

In the `weaponSlot >= 0` body:

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

The original emits this body at `0x17ba0c` as

```
17ba0c: movs r0,#0
17ba0e: strb r0,[fp,#93]      ; pauseOpen = 0
17ba12: strb r0,[fp,#214]     ; hudMenuOpen = 0
17ba16: mov  r0,fp
17ba18: blx  resumeSounds
17ba1c: ldr  r0,[fp,#116]
17ba20: blx  closeHudMenu
```

i.e. the *base* source order, yet the base source order did **not** emit that sequence. The swap
is what makes our emission match the original instruction for instruction.

### Full 24-permutation sweep of the 4-statement body (all 8080 B)

| order | pct |
|---|---:|
| **`hudMenuOpen, pauseOpen, resumeSounds, closeHudMenu` (landed)** | **81.9607** |
| `pauseOpen, hudMenuOpen, resumeSounds, closeHudMenu` (base) | 81.9247 |
| `closeHudMenu, hudMenuOpen, pauseOpen, resumeSounds` | 81.8886 |
| all other 21 permutations | 81.78 – 81.85 |

## Gate (full `check`)

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` CLEAN, `drift` 0. `.rel.plt`, `.ARM.exidx`, `.rodata`, `.data`, `.bss`, `.dynsym`,
`.dynstr`, `.hash`, `.rel.dyn` byte-identical. Section deltas unchanged: `.text` 85,441,
`.ARM.extab` 1,427, `.note.gnu.build-id` 20.

---

# Correction to wave 128: the tail-merge survivor is chosen by the `commonTailIndex` loop

Wave 128 blamed `BranchFolder::CreateCommonTailOnlyBlock`. That is wrong for this pair: both
`initHudMenu` bodies are *whole-block* tails, so `CreateCommonTailOnlyBlock` is never called.
The survivor is chosen by the `commonTailIndex` selection loop in
`llvm/lib/CodeGen/BranchFolding.cpp` (`TryTailMergeBlocks`, release_70, ~lines 1009–1035):

```cpp
    if (SameTails.size() == 2 &&
        SameTails[0].getBlock()->isLayoutSuccessor(SameTails[1].getBlock()) &&
        SameTails[1].tailIsWholeBlock())
      commonTailIndex = 1;
    else if (SameTails.size() == 2 &&
             SameTails[1].getBlock()->isLayoutSuccessor(SameTails[0].getBlock()) &&
             SameTails[0].tailIsWholeBlock())
      commonTailIndex = 0;
    else {
      for (unsigned i = 0, e = SameTails.size(); i != e; ++i) {
        MachineBasicBlock *MBB = SameTails[i].getBlock();
        if (MBB == EntryBB && SameTails[i].tailIsWholeBlock()) continue;
        if (MBB == PredBB) { commonTailIndex = i; break; }
        if (SameTails[i].tailIsWholeBlock()) commonTailIndex = i;   // keeps the LAST
      }
    }
```

`ComputeSameTails` fills `SameTails` from the highest block number down, so `SameTails[0]` is
the higher-numbered block. With an instrumented `llc` (print added at the selection site,
function-gated to `_ZN5MGame10OnTouchEndEiiPv`) our build reports:

```
ST: n=2 pred=434 cti=1 tails: 438(ir=%2282,...,prevop=t2MOVi,imm=2) 434(ir=%2264,...,prevop=t2MOVi,imm=1)
```

* `SameTails[0]` = block 438 = the `0x400` body (`initHudMenu(2)`), higher block number.
* `SameTails[1]` = block 434 = the `0x200` body (`initHudMenu(1)`), lower block number.
* `pred=434`: the test block's fall-through predecessor is the **`0x200`** body, so the
  `SameTails.size()==2` fast path (`SameTails[0]` is the layout successor of `SameTails[1]`,
  `SameTails[1]` whole) picks `cti=1` = the `0x200` body.

That is why ours emits `[0x200 body][blx][b test]` at `0x17adc4/17adca/17adce`, while the
original has `[0x400 body][blx][test]` at `0x17afd8/17afde/17afe2`. The original's test-block
layout predecessor must be the `0x400` body.

`MachineBlockPlacement` is the upstream cause: `buildChain` starts at the function entry and
greedily follows successors, so whichever of the two `initHudMenu` bodies the chain reaches
first owns the test block. Our `bb.204` (`%ir-block.2264`, `movs r1,#1`) falls straight through
into `bb.205` (the shared tail) and then `bb.213` (the test); the `0x400` body is `bb.231`
(`%ir-block.2282`) and only branches to `bb.205`. MBP's first chain pass already places
`… %bb.389, %bb.390, %bb.391 …` contiguously; the second `buildCFGChains()` re-places.

## Remaining root cause (unchanged)

Original layout of the hudMenu region:

```
17ad4c  hudTouchFlags lsls chain (0x200, 0x800, 0x400, 0x2000 .. 0x1000)
17adc0  0x200 case: ldrd; movs r1,#1; b 17afde       <- branches forward
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

Ours merges the other way: the `0x200` body owns the shared `blx`, the test sits right after it
(`0x17aeb4`), the `0x400` body branches back (`0x17afaa: b 0x17adca`), and the function tail is
pushed after the whole campaign chain (`0x17b976`). Flipping the survivor pulls the test to
`0x17afe2` and with it the `isAutoPilot` tail and the cold blocks — the ~1,800-line cascade.

## Size-critical spellings (re-measured this wave, all reverted)

| variant | size | pct |
|---|---:|---:|
| base (landed) | 8080 | 81.9607 |
| `won` bare `hasWon()` | 8064 | 77.0 |
| `failed` bare `hasFailed()` | 8088 | 73.9 |
| mining `if (amount != 0) String(-amount)` | 8064 | 73.4 |
| mining guard `count != 0` / `amount < 0` / `amount <= -1` | 8080 | 81.9607 |
| mining `if (amount < 0)` | 8084 | 71.1 |
| campaign temp removed | 8084 | 80.9 |
| nested `else { if (flag chain) … }` | 8084 | 80.6 |

Combos: ws+W 8064/76.32; ws+W+F 8068/73.13; ws+W+F+M 8072/68.05; ws+M 8064/73.41;
ws+C 8084/80.90; ws+F+C 8088/72.63.

## NEW this wave: the mining guard and the counter direction are coupled in llc

The original's mining block (`0x17a5ca`) is

```
17a5b6: sub.w r9, r9, #1     ; amount--
17a5ca: cmp.w r9, #0
17a5ce: beq   0x17a652       ; if (amount != 0)
17a5da: rsb.w r1, r9, #0     ; String(-amount)
```

Our base is **byte-identical from `0x17a5a0` to `0x17a5da` except the guard**, which is

```
17a5ca: cmp.w r9, #0xffffffff
17a5ce: bgt   0x17a652       ; count > 0  (count = -amount)
```

Both guards are 6 bytes, so the body stays 8080. But *every* `!= 0` spelling tried flips the
loop counter to an **incrementing** one in `llc` (the `.ll` is identical in shape: `sub nsw 0,
%amount` + `icmp eq %amount, 0` — the rewrite happens below the IR, in the codegen pipeline),
which shrinks the body to **8064 B** and wrecks the layout:

| guard spelling | counter | size | pct |
|---|---|---:|---:|
| `count > 0` (base) | `sub.w r9,r9,#1` | 8080 | 81.9607 |
| `count >= 1`, `amount < 0`, `amount <= -1`, `count > 0 && amount != 0` | `sub.w r9,r9,#1` | 8080 | 81.9607 |
| `amount != 0`, `count != 0`, `!(amount == 0)`, `(signed)amount != 0`, `amount << 0` | `add.w r8,r8,#1` | 8064 | 73.409 |
| `amount != 0u`, `amount != 0LL` | `add.w r8,r8,#1` | 8064 | 73.409 |
| `unsigned amount`, `long amount` + `!= 0` | `add.w r8,r8,#1` | 8064 | 73.409 |
| `count++` loop + `if (count != 0) String(-count)` | `add.w r8,r8,#1` | 8064 | 73.409 |

So the guard cannot be fixed without flipping the counter, and the counter flip costs 16 B.
This is the same 16 B lever wave 129 identified from the other direction: **the mining guard is
the only known source-level switch that flips the tail layout, and it costs 16 B.**

## Next attack

1. Find a source spelling that keeps `sub.w r9,r9,#1` *and* emits `cmp #0; beq`. The rewrite is
   below the IR (llc's codegen pipeline), so it must be defeated at the DAG/selection level —
   e.g. make the exit test something the DAG combiner cannot canonicalise to `!= 0` on the
   decrementing phi, without changing the emitted compare.
2. Or find the missing 16 B elsewhere in the function so the faithful mining form (8064) can be
   padded back to 8080 without moving the mining block; then the flipped layout may line up.
