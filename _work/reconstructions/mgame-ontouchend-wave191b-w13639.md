# MGame::OnTouchEnd — wave191b w13639: non-campaign-134 cluster forensics (note only)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13639`, branch
`wave191b-w13639`, base master `57ce1a4c8`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13639`.

Prior notes: `mgame-ontouchend-wave191b-w13622.md` (campaign-134 store swap, landed),
`-w13611.md` (diff-cluster map + case-local reorder/hint sweep), `-w13600.md`
(statement/declaration/chain-order sweep), `-w13590.md` (block-placement forensics).

## Result

Base unchanged. `check` green (byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370). `MGame::OnTouchEnd` raw **2,878**, aligned **1,582**,
size **8,080/8,080**, `.ARM.extab` **156,312** (unchanged).

No candidate beat 2,878 raw. Best size-exact aligned candidate was the 126-arm
store move at raw 2,881 / aligned 1,560 (−22 aligned, +3 raw). The base is a
strict local minimum for raw in every cluster worked this pass.

## Fresh diff-cluster map (base 2,878 raw bytes, 35 clusters)

Same-address byte runs, merging gaps <= 8 B:

| # | offset | len | what |
|---:|---|---:|---|
| 1 | `0x0f72..0x156c` | **1,531** | Play-dispatch/Play-body placement shift |
| 2 | `0x1962..0x1bb5` | 596 | knock-on shift of the chain tail |
| 3 | `0x15aa..0x1731` | 392 | knock-on shift of the 95/96 arm bodies |
| 4 | `0x17f2..0x18e3` | 242 | knock-on shift around the chain start |
| 5 | `0x1c2c..0x1ce9` | 190 | knock-on shift of the 126/155/161 arms |
| 6 | `0x0e3e..0x0e7b` | 62 | `isInstantActionMission`-first vs campaign-first |
| 7 | `0x1924..0x1954` | 49 | **not local** — ours' 95 arm sits where the original's 110 arm sits |
| 8 | `0x1f68..0x1f88` | 33 | literal-pool function pointers (EH cleanup) |
| 9 | `0x1e1c..0x1e34` | 25 | shared `dockToAsteroid` tail placed at a different address |
| 10 | `0x04a4..0x04b9` | 22 | mining-report `String("t ")` temp-address rematerialisation |

Remaining 25 clusters are <= 20 B and are branch-offset / pc-relative pool
knock-on (verified: `0x0ba4`, `0x17e0`, `0x1d04` are branch-target bytes only).

## New evidence: cluster 7 (0x1924) is arm-order knock-on, not a local diff

At offset `0x1924` the original is inside its **110** arm
(`cmp r0,#110` at `0x18f6`), ours is inside its **95** arm
(`cmp r0,#95` at `0x18f6`). The instruction bytes are the same kind of
getter/store sequence; the cluster is a consequence of the source arm order
(`74,162,15,43,42,22,65,120,96/127,100,95,110,81,134,144,126,155,161`), not a
wrong statement. Confirmed by the fact that the original's 95 arm is at
`0x1560` and ours at `0x18f6`.

## New evidence: cluster 9 (0x1e1c) is a shared-tail placement, not duplication

Both binaries contain exactly **one** `PlayerEgo::dockToAsteroid` call in this
function:

| | original | ours |
|---|---|---|
| shared `blx dockToAsteroid` | `0x1bb0` | `0x1e1c` |
| `isDockingToAsteroid` branch | `b.n 0x1bb0` | `b.n 0x1e1c` |
| `isDockingToStream` branch | `b.n 0x1bb0` | `b.n 0x1e1c` |

So cluster 9 is MachineBlockPlacement choosing a different anchor for the same
merged tail, not a missing/duplicated call.

## New evidence: cluster 10 (0x4a4) is rematerialisation, not a source error

Original emits the address of the `String("t ")` temporary **after** the first
`operator+` call (`add.w r8,sp,#80` at `0x4b2`); ours rematerialises it before
(`0x4a4`). The expression is the base one:

```cpp
report += String("\n") + String(count) + String("t ") +
          *Globals::gameText->getText(1274 + refined);
```

Every form that changes the temporary's spelling ties or loses:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,582 | 8,080 |
| `const char *unit = "t "; ... String(unit)` | 2,878 | 1,582 | 8,080 |
| `String("t\x20")` | 2,878 | 1,582 | 8,080 |
| `(String("\n") + String(count) + String("t ")) + ...` | 2,878 | 1,582 | 8,080 |
| `String unit("t "); ... + unit + ...` | 2,916 | 1,730 | 8,080 |
| `report += String("\n") + String(count); report += String("t ") + ...` | 6,437 | 2,163 | 8,064 |
| right-grouped `+ (String("t ") + getText(...))` | 3,194 | 1,639 | 8,072 |
| named `String head = String("\n") + String(count)` | 6,529 | 1,916 | 8,084 |

## Store-order sweep: other campaign arms (new, beyond w13622's adjacent swaps)

The original emits `switch_to_target_setting = 1` **first** in arms 95/81/126;
the base source has it after the getters. Moving it first is locally faithful
but shifts the global block layout:

| variant | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---:|
| base | **2,878** | 1,582 | 8,080 | 156,312 |
| 95: `switch=1` first | 2,918 | 1,574 | 8,080 | 156,312 |
| 81: `switch=1` first + getters reordered | 2,883 | 1,602 | 8,080 | 156,312 |
| 126: `switch=1` first | 2,881 | **1,560** | 8,080 | 156,312 |
| 161: `switch=0` before `initStream=0` | 2,880 | 1,584 | 8,080 | 156,312 |
| 110: `enterSpaceLounge=1` last (faithful) | 2,901 | 1,581 | 8,080 | 156,312 |
| back-to-station: faithful (switch, getters, active, module) | 2,888 | 1,569 | 8,080 | 156,312 |
| back-to-station: switch first only | 2,884 | 1,571 | 8,080 | 156,312 |
| 95+126 | 2,921 | 1,552 | 8,080 | 156,312 |
| 95+81+126 | 2,926 | 1,542 | 8,080 | 156,312 |
| 126+161 | 2,883 | 1,562 | 8,080 | 156,312 |
| 126+back-to-station | 2,891 | 1,547 | 8,080 | 156,312 |

All `.ARM.extab` sizes are identical to the base (156,312), so none of these is
a net `.ARM.extab` win either.

## Particle / `initLight` block (cluster at `0x14c8`)

Original order is: `menuTouchOpen=0`, `touch0Id=0`, `touch1Id=0`,
`activeTouchId=0`, `getStarSystem()->initLight()`, load `options[40]`,
`enableParticleEffects(...)`. Ours computes `particleQuality` first and calls
`enableParticleEffects` before `initLight`.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,582 | 8,080 |
| stores first, keep `enableParticleEffects` before `initLight` | 2,878 | 1,582 | 8,080 |
| full faithful (stores, `initLight`, `particleQuality`, `enableParticleEffects`) | 2,885 | 1,582 | 8,080 |
| stores, `particleQuality`, `enableParticleEffects`, `initLight` | 2,882 | 1,582 | 8,080 |
| `initLight` before `particleQuality`, stores in base order | 7,779 | 2,170 | 8,140 (fatal) |

## Campaign-chain order: faithful original order re-measured

Rebuilt the 18-arm chain programmatically in the original physical order
`15,22,43,42,65,81,74,95,96/127,100,110,120,126,134,144,155,161,162`
(identity-checked against the base):

| order | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,582 | 8,080 |
| faithful original | 3,961 | **1,217** | 8,076 (4 B short) |

The faithful order is by far the best *aligned* layout (1,217) but is size-fatal
(8,076) and raw-worse. 150 random disjoint 2-swaps of the base order were
measured; the best size-8,080 result was raw 2,898 / aligned 1,579, i.e. still
worse than base on both counts.

## `__builtin_expect` on the sound dispatch (new probes)

| hint | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,582 | 8,080 |
| `expect(IsCategoryEnabled(2), 0)` | 4,005 | 1,627 | 8,084 (fatal) |
| `expect(!IsCategoryEnabled(2), 1)` | 4,014 | 1,614 | 8,084 (fatal) |
| `expect(IsCategoryEnabled(2), 1)` | 4,014 | 1,614 | 8,084 (fatal) |
| `expect(!IsCategoryEnabled(2), 0)` | 4,005 | 1,627 | 8,084 (fatal) |
| `expect(soundsMuted, 1)` | 3,937 | 1,587 | 8,088 (fatal) |
| `expect(soundsMuted, 0)` | 2,950 | 1,582 | 8,080 |

## Conclusion

The residual is still the single Play-dispatch/Play-body placement decision
(1,531 of the 2,878 raw bytes). The other clusters are either knock-on shifts,
arm-order artefacts, shared-tail anchors, or the mining-temporary
rematerialisation. Every locally faithful store-order fix in the 95/81/110/126/
161/back-to-station arms and the particle block is raw-positive; the best
aligned-only win (126 arm, aligned 1,560) costs +3 raw and was not landed.
A future pass needs to pin MachineBlockPlacement's cold-block order, not more
statement permutations.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13639`:
`clusters.py` (same-address cluster map), `dis.py` (side-by-side disassembly),
`adiff.py` (normalised instruction alignment diff), `arms.py` (per-arm body
extraction), `armparse3.py`/`reorder2.py` (chain parse + faithful-order rebuild),
`sweep.py`/`measure.sh` (build + raw/aligned/size + `.ARM.extab`),
`permsearch.py` (random arm-body permutations), `faithful.cpp` (faithful chain
source).
