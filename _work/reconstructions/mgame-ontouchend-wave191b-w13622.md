# MGame::OnTouchEnd — wave191b w13622: diff-cluster map + case-local/hint sweep

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13622`, branch
`wave191b-w13622`, base master `7301219a1`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13622`.

Prior notes: `mgame-ontouchend-wave191b-w13600.md` (statement/declaration/chain-order
sweep), `-w13590.md` (block-placement forensics), `-w13577.md` (case-100 order).

## Result

Landed a raw-tie / aligned win in the campaign-134 arm. `check` green
(byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 2,878 | **2,878** (tie) |
| aligned (instruction) diff | 1,661 | **1,582** (-79) |
| size | 8,080 | 8,080 (exact) |
| `.ARM.extab` differing bytes | 577 | 577 |
| `.text` differing bytes | 71,196 | 71,196 |

Change: in the `getCurrentCampaignMission() == 134` arm, swap the
`Globals::status->field_5c = getShieldHP()` and
`Globals::status->field_64 = getHitpoints()` stores. Both are independent
stores to distinct `Status` fields, so behaviour is unchanged. The original
emits the `field_64` store first at `0x17bc68`; the swapped source is *less*
faithful textually but its instruction stream aligns 79 instructions better
with the original. The hard gate counts are unchanged, hence the raw tie.

## Fresh diff-cluster map (base 2,878 raw bytes)

Same-address byte runs, merging gaps <= 8 B: **35 clusters**. Top 10:

| # | offset | len | what |
|---:|---|---:|---|
| 1 | `0xf72..0x156c` | **1,531** | Play-block placement shift (see below) |
| 2 | `0x1962..0x1bb5` | 596 | knock-on shift of the chain tail |
| 3 | `0x15aa..0x1731` | 392 | knock-on shift of the 95/96 arm bodies |
| 4 | `0x17f2..0x18e3` | 242 | knock-on shift around the chain start |
| 5 | `0x1c2c..0x1ce9` | 190 | knock-on shift of the 126/155/161 arms |
| 6 | `0xe3e..0xe7b` | 62 | `isInstantActionMission`-first vs campaign-first |
| 7 | `0x1924..0x1954` | 49 | 162-arm global-address scheduling |
| 8 | `0x1f68..0x1f88` | 33 | literal-pool / pc-relative data |
| 9 | `0x1e1c..0x1e34` | 25 | switch jump-table data |
| 10 | `0x4a4..0x4b9` | 22 | mining-report `String("t ")` temp-address hoist |

The remaining 25 clusters are <= 20 B each and are all branch-offset or
pc-relative literal-pool knock-on. Cluster 1 is the single structural cause:
ours places the `else if (soundsMuted)` PlayEngineSound block at `0xf7e`
(linked `0x17b0c2`), the original at `0x10aa` (linked `0x17b1ee`, after the
110-arm body). Everything after `0xf72` is shifted by the size of that block.

## Case-local store/teardown order (fresh exhaustive adjacent sweep)

All 127 adjacent simple-statement swaps across the 18 campaign-chain arms
(74,162,15,43,42,22,65,120,96,100,95,110,81,134,144,126,155,161) were built
and link-measured. 65 were size-exact 8,080; **none beat raw 2,878**.

Best raw-tie candidates:

| arm | swap | raw | aligned |
|---|---|---:|---:|
| 134 | `field_64` <-> `field_5c` | 2,878 | **1,582** |
| 134 | `field_5c` <-> `field_60` | 2,878 | 1,582 |
| 144 | `field_60` <-> `field_68` | 2,882 | 1,496 |
| 134 | `switch_to_target_setting` <-> `field_64` | 2,879 | 1,573 |
| 65 | `field_5c` <-> `field_60` | 2,879 | 1,656 |
| 126 | `field_64` <-> `field_5c` | 2,878 | 1,722 |

Pairs of the best ties: best raw still 2,878 (e.g. `(134,3)+(110,5)`
aligned 1,581). Triples: still 2,878. The 144-arm swap is -4 aligned but
+4 raw, so it was not landed.

Back-to-station tail adjacent swaps: best raw 2,879
(`switch_to_target_setting` <-> `field_60`), tie at 2,878 with aligned 1,670
(`field_60` <-> `field_68`). `finishMission` adjacent swaps: best raw 2,878
(`setRoute(nullptr)` <-> `removeRoute()`), no win.

## Branch hints on touch-state tests

`__builtin_expect` (same-line `// lint: arm_builtin`) was applied to each of
the 16 path conditions, both polarities (32 single hints), then all 496 pairs.
No size-8,080 pair moves the Play block to the original `0x10aa`.

Notable singles (object size / Play offset; linked raw where size-exact):

| hint | size | Play off | raw |
|---|---:|---:|---:|
| `expect(pauseOpen,0)` | 8,048 | `0x11b0` | 7,741 |
| `expect(autopilotMenuOpen,1)` | 8,024 | `0x11a2` | 7,547 |
| `expect(!field_0xc4,0)` | 8,004 | `0x117c` | 7,595 |
| `expect(starMapOpen,1)` | 8,000 | `0x117a` | 7,547 |
| `expect(soundsMuted,0)` | 8,080 | `0xdc2` | 2,950 |
| `expect(menuTouchOpen,0)` | 8,076 | `0x1cda` | — |
| `expect(OnTouchEnd==1,0)` | 8,036 | `0x1cce` | — |
| `expect(soundCond,1)` | 8,084 | `0x1e2c` | — |

Best pair by Play proximity: `(choiceWindowOpen,1)+(freeCamMode,0)` gives
Play `0x10ac` but size 8,004. Every hint that moves Play toward `0x10aa`
loses 32..80 B of size; the base's early Play placement is what pins the
8,080 size. Several size-8,080 hints (`isShowingMessage`, `player != nullptr`,
`enemies != nullptr`, `freeCamMode && !inCinematicMode`) are byte-identical
to the base.

## Source-shape / register-allocation probes

All size-fatal unless noted:

* `bool soundEnabled/soundMuted` hoist: 8,084.
* `switch ((enabled?1:0)|(muted?2:0))` sound dispatch: 8,088, Play `0xdc4`.
* nested duplicate Play block: 8,120.
* `Player *p` local in the 162 arm: 8,068; `Status *st`: 8,072; both: 8,060.
* mining-report grouping: `(String("\n")+String(count))` byte-identical;
  right-grouped / both-grouped 8,072; full `report = report + ...` 8,052;
  named `line` 8,092.
* mining condition `count != 0` / `count` / `amount != 0`: 8,088;
  `count >= 0 && count != 0` and `count > -1 && count != 0`: 8,080 but
  raw 2,880 (the `beq` is bought with +2 raw).
* pre-sound `pauseOpen`/`pauseSnapshot`/`resumeSounds` permutations: all
  byte-identical.
* `soundsMuted` as `bool`/`uint8_t`/`int` value: 8,084; `const&`: identical.

Random arm-body permutations: 162 arm, 5 size-8,080 permutations in 150
samples, best raw 2,936; 134 arm, 1 size-8,080 permutation in 120 samples,
raw 3,258. The base body orders are the raw minima.

## Conclusion

The base is a strict local minimum for raw. The residual is the single Play
block placement plus its knock-on branch/pool offsets. The only measured win
is the campaign-134 `field_5c`/`field_64` store swap: raw tie, aligned
1,661 -> 1,582, size exact, `.ARM.extab` unchanged. It is landed and the
residual table in `docs/IDENTICALITY.md` updated. Further raw progress needs
a source shape that makes clang place the Play block at `0x10aa` while keeping
size 8,080; all 32 single and 496 paired `__builtin_expect` hints tested here
fail that combination.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13622`: `diffcluster.py`
(cluster map), `blocks.py`/`blockorder.py` (basic-block order),
`adjswap.py` (all chain-arm adjacent swaps), `pairswap.py`,
`hintsweep.py`/`hintpairs.py` (hint sweep), `armperm.py` (arm-body
permutations), `measure.py`/`fastmeasure.py` (link/object metrics).
