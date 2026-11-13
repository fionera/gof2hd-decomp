# MGame::OnTouchEnd — wave191b w13611: diff-cluster map + case-local reorder / hint sweep (note only)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13611`, branch
`wave191b-w13611`, base master `fcb09ff2c`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13611`.

Prior notes: `mgame-ontouchend-wave191b-w13600.md` (statement/declaration/chain-order
sweep; base a strict local min), `-wave191-w13571.md`, `-wave191b-w13577.md`,
`-wave191-w13539.md`.

## Result

Base unchanged. `check` green (byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370). `MGame::OnTouchEnd` raw **2,878**, aligned **1,661**,
size **8,080/8,080**, `.ARM.extab` **156,312** (unchanged).

No candidate beat 2,878. The best near-miss was the pause-menu
`setCinematicMode(true)` / `hudTouchFlags = 0` store swap at **2,879** (+1 raw,
−6 aligned). The base is a strict local minimum for this pass as well.

## New evidence: correct source-line attribution

The earlier note's line map was taken from the `.o` with `objdump -dl`; because every
`-ffunction-sections` section has VMA 0, objdump maps offsets to the wrong section's
line rows (it reported lines 600–1080 for OnTouchEnd). For this pass the link command
was replayed without the post-link `strip` to produce a debug `.so`
(`scratch/w13611/debug.so`), and `arm-linux-androideabi-addr2line` was used on the
final absolute addresses. That gives the real source line for each diff byte.

## (a) Fresh diff-cluster map — top 10 of 249 clusters

Raw diff bytes **2,878** across **249** contiguous clusters. First diff at `0x48b`
(the mining-report `count >= 0` branch), last at `0x1f88`. All clusters of length
>= 38:

| bytes | ours offset | source line / construct |
|---:|---|---|
| 91 | `0x1614..0x166f` | 1867 — `!freeCamMode && inCinematicMode()` → `setCinematicMode(true)` block |
| 90 | `0x11e1..0x123b` | 1533 — failed-mission cargo `isUnsaleable()/getIndex()` loop |
| 84 | `0x124b..0x129f` | 1598 — mission-42 `delete objectivesB/objectivesA` block |
| 81 | `0x1181..0x11d2` | 1749 — "back to the station" `getHitpoints/getShieldHP/getArmorHP/getGammaHP` block |
| 73 | `0x1474..0x14bd` | 1614 — mission-65 arm (`loadStation(100)` etc.) |
| 65 | `0x0f72..0x0fb3` | reward block (`setFreelanceMission` / `showMissionRewardMessage`) |
| 62 | `0x1840..0x187e` | 2096 — weapon-slot / mission-120 tail-merge block |
| 59 | `0x14c8..0x1503` | 1836 — `particleQuality` / `enableParticleEffects` block |
| 59 | `0x108a..0x10c5` | 1583 — mission-162 arm |
| 58 | `0x1c2c..0x1c66` | 1706 — mission-126 arm |

The next clusters are 58 B at `0x1a1c` (1672, mission-100), 47 B at `0x1b18`
(1965, dock/auto-pilot block), 46 B at `0x1536` (1848, cinematic-steer switch),
45 B at `0x19b7` (1664, mission-95), 45 B at `0x16b3` (1629, mission-96/127),
44 B at `0x1962`, 40 B at `0x1504` (1841), 38 B at `0x1a74`, 38 B at `0x104c`
(1579, mission-162 head), 35 B at `0x13be` (1994).

**Key structural observation (new):** most of these clusters are not local code
differences. Block content is byte-identical but placed at a different offset.
Examples verified instruction-by-instruction:

- failed-mission cargo loop: original `0x12a2..0x12d0`, ours `0x11ee..0x121c`,
  identical instructions.
- mission-42 `delete objectives` block: original `0x10f0..0x1120`, ours
  `0x125e..0x128e`, identical instructions.
- pause-button block: original `0x5f0..0x6ae`, ours `0x5f2..0x6ac`, identical
  except a 2-byte shift.

So the residual is a global MachineBlockPlacement permutation, not a local
scheduling/register-allocation error in most clusters.

## (b) Case-local store/teardown order swaps

All size-exact unless noted. `.rel.plt` unchanged.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,661 | 8,080 |
| reward: `setFreelanceMission` before `showMissionRewardMessage` | 2,880 | 1,661 | 8,080 |
| back-to-station: `switch=0` first, getters in source order, `active=0` last | 2,888 | 1,648 | 8,080 |
| back-to-station: only `switch=0` first | 2,884 | 1,650 | 8,080 |
| back-to-station: only `active=0` last | 2,885 | 1,659 | 8,080 |
| pause-menu `setCinematicMode(true)` before `hudTouchFlags = 0` | **2,879** | 1,655 | 8,080 |
| pause-menu `setCinematicMode(true); initLight(); hudTouchFlags=0` | 3,855 | 1,643 | 8,060 (fatal) |
| particleQuality: `menuTouchOpen/touch0/touch1/activeTouchId` store order | 2,878 | 1,661 | 8,080 |
| particleQuality: stores first, `initLight`, then `particleQuality`/`enableParticleEffects` | 2,885 | 1,661 | 8,080 |
| particleQuality: `initLight` before `enableParticleEffects` | 7,779 | 2,170 | 8,140 (fatal) |
| pause teardown: `hudMenuOpen=0` before `closeHudMenu()` | 2,890 | 1,665 | 8,080 |
| weapon-slot teardown: `closeHudMenu()` before `resumeSounds()` | 2,879 | 1,661 | 8,080 |
| `_bc8` teardown: `pauseOpen=0` before `resumeSounds()` | 2,880 | 1,661 | 8,080 |
| `frameTime`/`resetAnalogStick`/`releaseAllKeys` reorder | 2,884 | 1,664 | 8,080 |
| `pauseOpen=0` / `pauseSnapshot=0` / `resumeSounds()` reorder | 2,897 / 2,901 | 1,667 / 1,671 | 8,080 |
| sound block moved after `particleQuality` | 4,324 | 1,504 | 8,072 (fatal) |
| sound block moved after cinematic-steer switch | 4,387 | 1,736 | 8,056 (fatal) |
| `touch0Id`/`touch1Id` store swap | 2,878 | 1,661 | 8,080 |
| `soundsMuted` declaration moved after the `OnTouchEnd` test | 2,878 | 1,661 | 8,080 |

## (c) Branch hints on touch-state tests

`__builtin_expect` (same-line `// lint: arm_builtin`). Only size-exact rows are
interesting; most hints are size-fatal.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,661 | 8,080 |
| `isHacking()` expect 1 | 2,878 | 1,661 | 8,080 (already predicted true) |
| `isHacking()` expect 0 | 5,908 | 1,723 | 8,088 (fatal) |
| `hudTouchFlags & 4` expect 1 | 2,878 | 1,661 | 8,080 |
| `hudTouchFlags & 0x80` expect 1 | 2,878 | 1,661 | 8,080 |
| `hudTouchFlags & 0x40` expect 1 / 0 | 6,829 / 7,572 | 2,384 / 2,892 | 8,076 / 7,964 (fatal) |
| `hudTouchFlags == 0` expect 1 / 0 | 4,703 / 4,228 | 1,719 / 1,721 | 8,092 / 8,088 (fatal) |
| `isAutoPilot() && !wasAutoPilot` expect 0 | 4,020 | 1,682 | 8,080 |
| `menuWindow->OnTouchEnd(...) == 1` expect 1 / 0 | 4,115 / 6,100 | 1,752 / 1,887 | 8,076 / 8,036 (fatal) |
| `cinematicSteerActive` expect 0 | 3,983 | 1,731 | 8,080 |
| `!freeCamMode && inCinematicMode()` expect 0 | 4,016 | 1,649 | 8,080 |
| `soundsMuted` else-if expect 0 | 2,878 | 1,661 | 8,080 (identical) |
| `!IsCategoryEnabled(2) && !soundsMuted` expect 0 / 1 | 3,937 / 4,411 | 1,665 / 1,745 | 8,088 / 8,084 (fatal) |
| outer `pauseOpen` expect 1 | 4,724 | 1,915 | 8,088 (fatal) |
| outer `choiceWindowOpen` expect 1 | 7,530 | 2,709 | 8,004 (fatal) |
| outer `!field_0xc4` expect 1 | 6,271 | 2,252 | 8,088 (fatal) |

## (d) Source shapes that change register allocation

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,661 | 8,080 |
| `int count = 0 - amount` / `amount * -1` / `-1 * amount` | 2,878 | 1,661 | 8,080 |
| `amount -= 1` / `amount = amount - 1` / `--amount` | 2,878 | 1,661 | 8,080 |
| `String("t ", false)` / `String((int) count)` / `(getText(...))` | 2,878 | 1,661 | 8,080 |
| `report = report + ...` | 7,495 | 2,876 | 8,052 (fatal) |
| `count > 0` / `count >= 1` / `0 < count` / `amount < 0` | 2,880 | 1,662 | 8,080 |
| `count != 0` / `if (count)` / `amount != 0` / `amount` | 6,638 | 2,188 | 8,088 (fatal) |
| `unsigned count` + `!= 0` (with `(int) count` cast) | 6,638 | 2,188 | 8,088 (fatal) |
| `count >= 0 && count != 0` | 2,880 | 1,662 | 8,080 |
| loop index `int i` (all three sound loops) | 2,878 | 1,661 | 8,080 |
| `jumpActive != 0 ? ...` | 2,878 | 1,661 | 8,080 |
| `reinterpret_cast<uint8_t &>(field_0x1a4)` | 2,878 | 1,661 | 8,080 |
| weapon-chain adjacent arm swaps (8 pairs) | 2,880..4,640 | 1,663..1,838 | 8,080..8,092 |
| hud-menu `0x800`/`0x400` arm swap | 4,732 | 1,934 | 8,092 (fatal) |
| hud-menu `0x200`/`0x800` arm swap | 3,106 | 1,688 | 8,080 |

The mining-report branch is the one local instruction mismatch in the
byte-identical prefix: original `cmp r9,#0; beq` (skip when `count == 0`), ours
`cmp r9,#0; bgt` (skip when `count > 0`). The semantically faithful `!= 0` form
emits the right `beq` but costs +8 B (register allocation moves `count` from
`r9` to `r8` and reloads `r9`), so it is size-fatal. No size-exact form of
`!= 0` was found.

## Chain-order search (revisited)

The chain was parsed into its 18 `else if` arms and rebuilt programmatically
(`scratch/w13611/chainperm.py`), with identity checked against the base source.

| order | raw | aligned | size |
|---|---:|---:|---:|
| base `74,162,15,43,42,22,65,120,96/127,100,95,110,81,134,144,126,155,161` | **2,878** | 1,661 | 8,080 |
| faithful original `15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,155,161,162` | 3,960 | **1,227** | 8,076 (fatal) |
| `162` moved to end | 3,891 | 1,459 | 8,080 |
| `15` moved to front | 2,893 | 1,661 | 8,080 |
| swap `74`/`162` | 2,886 | 1,661 | 8,080 |
| `81` before `120` | 3,573 | 1,673 | 8,080 |
| `65` after `81` | 4,708 | 1,880 | 8,088 (fatal) |

All 17 adjacent arm swaps and all 306 single-arm moves (take one arm, insert at
any other position) were measured. Best raw was a tie at **2,878** (swap
`15`/`43`, and its inverse); every other move was >= 2,884. The faithful order
gives the best *aligned* score (1,227) but is 4 B short and 1,082 raw bytes
worse, i.e. the block placement is better but the layout is not size-exact.

## Conclusion

The base remains a strict local minimum. The 2,878 residual is dominated by
global cold-block placement: the original interleaves the pause-menu
`soundsMuted` Play/Stop block (original `0x10a4`) inside the campaign chain,
where ours places it at `0x0f72` before the reward block; the chain, the
`setCinematicMode` block and the mission arms are then all shifted. All local
statement/store reorders, branch hints, register-allocation shapes, chain
permutations (adjacent, single-arm, and targeted faithful orders) either tie at
2,878 or lose. A further pass needs a way to pin MachineBlockPlacement's cold
block order (e.g. reconstructing the exact original top-level statement order
around the pause-menu sound handling), not more local permutations.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13611`:
`measure.sh` (build + fn_diff + extab), `cluster.py`/`clusters3.py` (raw
diff-cluster map; `clusters3.py` uses `debug.so` + addr2line for correct source
lines), `hist.py`, `blocks.py`/`oursblocks.py`/`origlines.py` (basic-block and
line maps), `sweep.py`/`sweep2.py` (single/multi-replacement build-measure
harness), `chainperm.py` (18-arm chain parser/rebuilder, identity-checked),
`permsearch.py` (adjacent and single-arm permutation search), `chainsweep.py`/
`chainsweep2.py` (targeted chain orders), `cand*.py` (each experiment batch),
`link_debug.sh` (replays the gold link without strip to produce `debug.so`).
