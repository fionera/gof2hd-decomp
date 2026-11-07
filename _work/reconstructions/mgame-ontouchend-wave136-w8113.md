# MGame::OnTouchEnd — wave 136 (w8113): the EstimateRuntime lever is dead; `PredBB` decides

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8113`,
base `2e51d765d`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8113`. **No source change landed** (base is byte-identical to HEAD and the
full gate is green). This note *falsifies* the wave-134/135 hypothesis that a
size-neutral lengthening of the 0x200 prefix would flip the BranchFolder survivor.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

Baseline (verified this wave, full `check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
.text 84,960   .ARM.extab 1,404   .ARM.extab size 156,312
.ARM.exidx 0   .rel.plt 0   .rodata 0   .data 0   size 8080/8080   raw fuzzy 81.9607 %
```

## 1. Hypothesis under test

Wave 134 §5 / wave 135 §3 proposed: `BranchFolder::CreateCommonTailOnlyBlock`
picks the 0x200 `initHudMenu(1)` body over the 0x400 `initHudMenu(2)` body only
because `EstimateRuntime` ties (both prefixes `LDR + LDR + MOVi` = 5) and the
`t <= TimeEstimate` loop keeps the last (`SameTails[1]` = lower-numbered block =
0x200 body).  Therefore a *size-neutral* way to make the 0x200 prefix estimate
strictly larger than the 0x400 prefix should flip the survivor to 0x400 while
keeping the 0x200-first dispatch order and 8080 B.

## 2. Result: the estimate is NOT consulted in the base — `PredBB` wins first

`CreateCommonTailOnlyBlock` checks `SameTails[i].getBlock() == PredBB` *before*
the estimate and `break`s:

```cpp
for (unsigned i = 0, e = SameTails.size(); i != e; ++i) {
    if (SameTails[i].getBlock() == PredBB) { commonTailIndex = i; break; }   // <-- taken
    unsigned t = EstimateRuntime(...);
    if (t <= TimeEstimate) { TimeEstimate = t; commonTailIndex = i; }
}
```

In the base MIR at the CFO pass, **`PredBB` is the 0x200 body (bb.404)**, so the
estimate loop is never reached.  Evidence (`-mllvm -print-after-all` CFO dump,
`/dev/shm/w8113/pa_base2.txt` line 681236):

```
bb.404 (%ir-block.2426):                 ; 0x200 body
  renamable $r0 = t2LDRi12 renamable $r11, 116
  renamable $r2 = t2LDRi12 renamable $r11, 120
  $r1 = t2MOVi 1
bb.405 (%ir-block.2567):                 ; split-off common tail (new block)
  tBL ... @_ZN3Hud11initHudMenuEiP5Level
bb.406 (%ir-block.2567):                 ; join test (hudTouchFlags == 0)
  renamable $r0 = t2LDRi12 renamable $r11, 248
  t2CMPri killed renamable $r0, 0
  t2Bcc %bb.472, 1
```

`SplitMBBAt` inserts the new tail block immediately after its source, so
pre-CFO the join test (ir-block 2567) sat **immediately after the 0x200 body**
(ir-block 2426) in the MachineFunction block list; hence
`PredBB = &*std::prev(I)` = the 0x200 body.  (The PEI dump's `bb.N` numbers are
creation-order IDs, not list positions — this is what misled wave 135 into
reporting `PredBB = bb.462`, the wingman tail.)

The swap variant confirms the mechanism: with the 0x400 case first in source the
CFO dump has `bb.404` = the 0x400 body (`t2MOVi 2`) directly before the join
test, and the 0x400 body owns the tail — again via `PredBB`, not the estimate.

## 3. Variants measured this wave (all reverted; base restored)

All variants changed only the 0x200 body (or the chain shape); `owner` says which
body falls through into the shared `blx` (`arg 1 -> blx` = 0x200 owns,
`arg 2 -> b.n` = 0x200 owns the `blx` too in every case below).

| variant (0x200 body unless noted) | size | pct | owner | note |
|---|---:|---:|---|---|
| base | 8080 | **82.0** | 0x200 | reference |
| `initHudMenu(this->hudMenuOpen, level)` | 8080 | 82.0 | 0x200 | **byte-identical** — clang folds the bool to 1 |
| `initHudMenu(this->hudMenuOpen ? 1 : 0, level)` | 8080 | 82.0 | 0x200 | byte-identical |
| `initHudMenu((int)this->hudMenuOpen, level)` | 8080 | 82.0 | 0x200 | byte-identical |
| `Hud *hud = this->hud; hud->initHudMenu(1, level)` | 8080 | 82.0 | 0x200 | byte-identical |
| `Level *lvl = this->level; ...initHudMenu(1, lvl)` | 8080 | 82.0 | 0x200 | byte-identical |
| `initHudMenu(1, this->level ? this->level : nullptr)` | 8080 | 82.0 | 0x200 | byte-identical |
| `initHudMenu(+1, level)` | 8080 | 82.0 | 0x200 | byte-identical |
| nested `else { if (flags & 0x400) ... }` chain | 8080 | 82.0 | 0x200 | byte-identical |
| `initHudMenu(*(volatile bool*)&this->hudMenuOpen, level)` | 8080 | 81.9 | **0x200** | real `ldrb.w r1,[fp,#214]` (+2 est), still PredBB |
| `this->hudMenuOpen = 1;` then call | 8084 | 80.8 | **0x200** | `strb.w` (+4 B), still PredBB |
| `this->field_0x18 = 1;` then call | 8084 | 80.8 | **0x200** | still PredBB |
| `this->cameraMode = 1;` then call | 8084 | 80.8 | **0x200** | still PredBB |
| `pauseSounds();` then call | 8084 | 80.8 | **0x200** | call (+10 est), still PredBB |
| swap 0x200/0x400 cases | 8080 | 81.9 | **0x400** | flips via PredBB; dispatch order becomes 0x400/0x800/0x200 |

The four "extra instruction" variants raise the 0x200 estimate from 5 to 6/8/15
and still leave the 0x200 body as the survivor — decisive proof that
`EstimateRuntime` is unreachable here.

## 4. Consequence for the next wave

The 0x200-first dispatch order and the 0x400-survivor ownership cannot both be
obtained by touching the 0x200 *body*.  The only remaining levers are:

1. Make the **join-test block's list predecessor** the 0x400 body.  That means
   changing the IR block order so the `hudTouchFlags == 0` block follows the
   0x400 body (currently it follows the 0x200 body).  Every `goto`/nesting shape
   tried so far is normalised back by clang (byte-identical).
2. Make the 0x200 body **not a member of `SameTails`** (e.g. break the common
   `tBL`/`t2B` tail), which costs a second `initHudMenu` call (+4 B).

## 5. Reproduction

```
cp /dev/shm/w8113/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8113/score.py          # 82.0 / 8080 / 2775:2774
python3 /dev/shm/w8113/owner.py          # arg 1 ... -> blx   (0x200 owns)
/dev/shm/w8113/pa.sh /dev/shm/w8113/pa_base2.txt   # -mllvm -print-after-all
# CFO dump at line 681236: bb.404 (0x200 body) -> bb.405 (tail) -> bb.406 (join test)
```

Variant runner: `/dev/shm/w8113/v1.py`, `v2.py` (uses `batch.py`).

The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; the
original 0x200/0x400 bodies are at `0x17adc0` (`ldrd; movs r1,#1; b 0x17afde`)
and `0x17afd8` (`ldrd; movs r1,#2; fallthrough 0x17afde`).
