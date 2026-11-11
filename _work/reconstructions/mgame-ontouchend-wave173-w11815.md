# MGame::OnTouchEnd — wave 173 (w11815): epilogue placement attack

Status: analysis only, no code win landed, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w11815`, branch `wave173-w11815`, base `3fafa5b03`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11815`.

Entry/exit state (src left at base): raw **3,773**, aligned **1,453**, size
**8,080/8,080**, `.rel.plt` byte-identical (0), gate byte_exact **4507**,
linked_exact **4507**, size-exact 4524, stub/missing/wrong_type/extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.
Original epilogue `popeq` `0x17b014`; ours `0x17ba32`.

## Root cause (new, precise)

The epilogue address is a **downstream symptom of one tail-merge decision** in the
`if (this->hudMenuOpen)` dispatcher, not an independent lever.

The dispatcher has two arms that call `Hud::initHudMenu` with a different menu id:

```
0x200 arm: this->hud->initHudMenu(1, this->level);
0x400 arm: this->hud->initHudMenu(2, this->level);
```

`BranchFolder` merges the trailing `blx initHudMenu` into one shared block. Which
arm *falls through* into that shared block decides the whole tail layout.

Original (`0x17ad46..0x17b050`):

```
17ad4c  tst  #0x200 ; bne.n 17adc0          ; 0x200 arm
17ad56  bmi.w 17ae8c                        ; 0x800 arm
17ad5c  bmi.w 17afd8                        ; 0x400 arm
...     (0x2000..0x100000 arms)
17ad9c  bpl.w 17afe2                        ; 0x1000 arm -> test
17adc0  0x200 arm: ldrd r0,r2,[fp,#116] ; movs r1,#1 ; b.n 17afde   <-- branches to shared blx
17adc8  won chain ...
17ae8c  0x800 arm ... ; b.n 17afe2
17aeac  maneuverTouchEnd ... ; b.n 17afe8
17aebc  menuTouchOpen block ...
17af44  starMap block ... ; b.n 17b004
17af5a  won chain (cont) ... ; b.n 17b0e2
17afc0  literal pool
17afd8  0x400 arm: ldrd r0,r2,[fp,#116] ; movs r1,#2          <-- FALLS THROUGH
17afde  blx Hud::initHudMenu                                     <-- shared tail
17afe2  ldr r0,[fp,#248] ; cmp #0 ; cbz r0,17b01a               <-- hudTouchFlags==0 test
17afe8  final if(player->isAutoPilot() && !wasAutoPilot)         <-- FALLS THROUGH
17b004  shared epilogue (ldr [sp,#200] / canary / popeq 0x17b014)
17b016  blx __stack_chk_fail
17b01a  hudTouchFlags==0 body ... ; b.n 17afe8
```

Ours (base):

```
17ad54  bne.n 17adc4                        ; 0x200 arm
17ad5c  bmi.w 17afaa                        ; 0x400 arm
17ad9c  bpl.w 17aeb2                        ; 0x1000 arm -> test
17adc4  0x200 arm: ldrd r0,r2,[fp,#116] ; movs r1,#1
17adca  blx Hud::initHudMenu                <-- shared tail lives HERE
17adce  b.n 17aeb2
17ae94  0x800 arm ... ; bne.w 17ba06
17aeb2  ldr r0,[fp,#248] ; cmp #0 ; bne.w 17ba06   <-- test, falls through
17aebc  hudTouchFlags==0 body ... ; b.w 17ba06
17afaa  0x400 arm: ldrd r0,r2,[fp,#116] ; movs r1,#2 ; b.n 17adca  <-- branches BACK
...
17b9c0  weaponSlot>=0 body ... (falls through)
17ba06  final if(player->isAutoPilot() && !wasAutoPilot)
17ba22  shared epilogue (popeq 0x17ba32)
```

So the difference is exactly two chain assignments:

1. **Which arm owns the merged `blx`**: original = `0x400` arm; ours = `0x200` arm.
2. **The `hudTouchFlags == 0` test's fallthrough**: original = the final
   isAutoPilot block (join); ours = the `hudTouchFlags == 0` body.

Consequence: in the original the chain
`0x400 arm -> blx -> test -> final -> epilogue` puts the epilogue at `0x17b004`
and pushes the `hudTouchFlags == 0` body to a later chain (`0x17b01a`, right
after the epilogue). In ours the chain is `0x200 arm -> blx` and
`0x800 arm -> test -> body`, and the final block is claimed by the weaponSlot
block, so the epilogue lands at `0x17ba22`.

Corroborating detail: the original's `weaponSlot >= 0` body ends
`17ba24: b.w 17afe8` (it does **not** fall through to the final block); ours
ends `17ba04: blx closeHudMenu` and falls through to the final block at
`0x17ba06`.

The prefix up to the divergence is essentially perfect. `alignmap.py`
(difflib over normalised instruction streams) shows delta 0 through
`0x17a5e8`, then only +2/+4/+8..+110 shifts; the only real codegen diff before
the layout divergence is the ore-refine `String` temp scheduling at `0x17a5e8`
(22 B, `add r0,sp,#116` vs hoisted `add.w r8,sp,#80`) plus the wave-171
`bgt`-vs-`beq` at `0x17a5cf` (1 B). Everything else in
`0x17a144..0x17ad46` is byte-identical.

## What was measured this wave

All shapes below are applied to `src/game/menu/MGame.cpp` on top of the base
source; size 8,080 unless noted, `.rel.plt` 0 throughout.

| shape | raw | aligned | size | epilogue popeq |
|---|---:|---:|---:|---:|
| base | **3,773** | 1,453 | 8,080 | 0x17ba32 |
| chain order `wing,flags,slot` (`wfs`) | 4,472 | 1,595 | 8,080 | 0x17b038 |
| chain order `flags,slot,wing` (`fsw`) | 5,496 | 1,696 | 8,056 | 0x17b098 |
| chain order `flags,wing,slot` (`fws`) | 4,949 | 1,666 | 8,112 | 0x17b0a8 |
| chain order `wing,slot,flags` (`wsf`) | 3,773 | 1,453 | 8,080 | 0x17ba32 |
| `wfs` + `expect(flags==0,0)` | 4,664 | 1,607 | 8,092 | **0x17b00e** |
| `wfs` + `expect(flags==0,1)` | 6,100 | 1,679 | 8,084 | 0x17afda |
| `wfs` + `expect(wingmanCommand>=0,0/1)` | 4,472 | 1,595 | 8,080 | 0x17b038 |
| `wfs` + `expect(weaponSlot>=0,0/1)` | 4,472 | 1,595 | 8,080 | 0x17b038 |
| `expect(hudTouchFlags==0,0)` | 5,920 | 1,602 | 8,076 | 0x17af90 |
| `expect(hudTouchFlags==0,1)` | 3,773 | 1,453 | 8,080 | 0x17ba32 |
| `expect(player->isAutoPilot() && !wasAutoPilot, 0)` | 4,063 | 1,464 | 8,084 | 0x17ba28 |
| `expect(player->isAutoPilot() && !wasAutoPilot, 1)` | 4,377 | 1,486 | 8,076 | 0x17ba30 |
| separate `if` (flags guard) before slot chain | 5,461 | 1,732 | 8,072 | 0x17b074 |
| separate `if` (flags guard) after slot chain | 6,083 | 1,614 | 8,084 | 0x17afe2 |
| nested `else { if (flags==0) ... }` | 3,773 | 1,453 | 8,080 | 0x17ba32 |
| `expect(mission->isCampaignMission(),1)` (base order) | 4,468 | 1,555 | 8,088 | 0x17ba02 |
| `expect(mission->isCampaignMission(),0)` (base order) | 4,262 | 1,526 | 8,092 | 0x17ba32 |
| `expect(mission->isInstantActionMission(),0)` | 5,961 | 1,884 | 8,084 | 0x17af92 |
| `expect(mission->isInstantActionMission(),1)` | 5,413 | 2,051 | 7,856 | 0x17b416 |
| `expect(won,1)` | 4,744 | 1,365 | 8,084 | 0x17ba38 |
| `expect(won,0)` | 5,382 | 2,208 | 7,828 | 0x17b370 |

Also codegen-identical to base (3,773 / 1,453 / 8,080 / 0x17ba32):
`!hudTouchFlags`, `0 == hudTouchFlags`, `hudMenuOpen != 0`,
`weaponSlot != -1` / `> -1` / `0 <= weaponSlot`, `wingmanCommand != -1` /
`> -1`, local `int htf = hudTouchFlags` (that one is 4,720 / 8,076),
`amount != 0`-family (see below), and swapping the two `initHudMenu` arms
(`0x400` first) which instead changes the dispatcher test order
(`tst #0x400`) and gives 3,776.

## The `bool Mission::hasWon` route (epilogue lands exactly, but costs the gate)

Changing `Mission::won` to `bool` and `Mission::hasWon()` to `bool` (body stays
`ldrb r0,[r0,#5]; bx lr`) makes the `won` computation emit the original's
`cmp r0,#0` instead of `cmp r0,#1`. Combined with the "campaign dispatch first" source reorder:

```cpp
if (mission->isCampaignMission()) {
    Globals::status->nextCampaignMission(true);
} else if (!mission->isInstantActionMission()) {
    Globals::status->setFreelanceMission(Mission::empty);
    Globals::layout->showMissionRewardMessage(mission->getReward() + mission->getBonus(), false);
} else {
    Globals::switch_to_target_setting = 2;
    this->active = 0;
    this->applicationManager->SetCurrentApplicationModule(1);
    return;
}
```

gives raw **3,896**, aligned **1,350**, size **8,092**, and the shared epilogue
`popeq` at **`0x17b014` — exactly the original address**. The +12 B is a `nop`
literal-pool alignment pad at `0x17afbe` plus one extra instruction.

But `bool hasWon` is **not free**: a full `check` with only the Mission change
gives `byte_exact 4507 -> 4487` (linked_exact stays 4507) because ~20 other
same-size functions (MGame OnUpdate/OnRender/…, MeshMerger) call `hasWon()` and
their `cmp r0,#1` vs `cmp r0,#0` codegen changes. That is a hard ratchet
failure, so the route cannot be landed without also fixing those callers — and
even then OnTouchEnd raw is 3,896 > 3,773.

Also, landing the epilogue at the right address does **not** make the tail
match: with the shape above the diff buckets are ~200–300 B per 256 B from
`0x17af00` to `0x17b000` (the mission-reward chain and the campaign `==N`
dispatch are still laid out differently), so the raw total stays well above
base.

## Ore-refine guard (`count`) re-check

`count >= 0` (wave-171, landed) is still the best form: raw 3,773.
`count > 0` (wave-161, semantically correct) = 3,775. Every `!= 0` spelling
(`amount != 0`, `amount`, `count != 0`, `count`) flips clang's counter to a
positive one, shrinks the function to 8,064 and re-lays the tail:
raw 6,635, epilogue `0x17af92`. The 22-byte `String` temp scheduling diff at
`0x17a5e8` is present for both `count >= 0` and `count > 0`, so it is
independent of the guard; splitting/rewriting the `report +=` expression
(`String line = …; report += line`, `report = report + …`, stepwise `+=`,
explicit parens) all regress badly (4,092–7,474 raw, sizes 8,016–8,092).

## Conclusion

No size-exact shape was found that moves the epilogue and lowers raw. Every
shape that moves the epilogue (`wfs`, `expect(flags==0,0)`, `expect(inst,0)`,
campaign-first, …) re-lays the reward chain / campaign dispatch and loses more
raw than it gains. The epilogue is not an independent lever: it is the
fallthrough of whichever chain owns the merged `Hud::initHudMenu` tail, and
matching it requires the original's exact `if (hudMenuOpen)` source shape
(shared `blx` owned by the `0x400` arm, `hudTouchFlags == 0` body demoted to a
later chain). `src/` is therefore left at the base commit.

## Reproduction

```
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
python3 /home/debian/GalaxyOnFire2/scratch/w11815/epi.py
cmake --build cmake-build-match --target check -j2
```

Scratch helpers (all under `/home/debian/GalaxyOnFire2/scratch/w11815`):
`measure.sh` (link + fn_diff + epi), `exp2.py` (batch source-variant runner:
`python3 exp2.py <json>` where the json is a list of
`{name, reps:[[old,new],...]}` applied to `MGame.base.cpp`), `epi.py`,
`dis.sh`, `diffranges.py`, `alignmap.py`, `MGame.base.cpp`, `orig.dis`,
`oursbase.dis`, plus the `exp*.json` experiment definitions.
