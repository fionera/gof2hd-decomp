# LevelScript::process — wave 144 / w8903 (`setStanding` PLT window:
# body-after-case-70-state-0 lands the call at the original 0x139ac2, but the
# shape is +4 B and no size-exact variant was found)

Status: **analysis only, no code change landed** (source restored to the wave-143
HEAD `f5093474f`). Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, size-exact. Owned TU
`src/game/world/LevelScript.cpp`. Scratch helpers in `/dev/shm/w8903/`
(not tracked): `fdiff.py` (raw same-address diff), `reldiff.py` (`.rel.plt`
byte + symbol-order diff), `harness.py` (restore pristine base, apply a named
edit, build, measure size/raw/getStanding/setStanding sites), `variants.py`
(the ~60 variants measured this pass), `LevelScript.base.cpp` (pristine copy).

## Result

```
                         wave-143 HEAD     this pass
process raw diffbytes    54,619            54,619   (unchanged, source pristine)
orig / ours size         57232 / 57232     57232 / 57232
.rel.plt                 0 diff            0 diff
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

The mission's hypothesis is **confirmed in the PLT sense**: the real
`getStanding`/`setStanding` pair can be moved into the window
(`0x13960c..0x139b90`) and `.rel.plt` becomes byte-identical with the
artificial `if (delta == 0x7d)` anchor removed.  The blocker is **size**:
every shape that lands the body in the window compiles to `57228` or `57236`
bytes, never `57232`.  The function size parity flips (base ≡ 0 mod 8, all
body-in-case-70 shapes ≡ 4 mod 8), so no +/-8 store add/remove can reach
57232 — a genuine 4-byte (single 32-bit Thumb instruction) difference is
needed.  The best shape is +4 B and drops `byte_exact` 4507 -> 2741 through
the section shift (measured with `check`), so it was not landed.

## The winning shape (best measured, +4 B)

Place the shared exit-wormhole body **after case 70's state-0 block** (not at
the start of case 70), guarded by a mission test, and delete the
`if (delta == 0x7d)` anchor and the wave-143 metric-only case-80
`m_pRadar->dockNavPtr = nullptr` store:

```cpp
            case 70: { // 0x1399a2
                if (((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0) {
                    ... state-0 teardown ...
                    m_nState = 1; // shared tail 0x139aba (m_nState = 1) -> post-switch tail
                }
                // >>> inserted here <<<
                if (Globals::status->getCurrentCampaignMission() == 50 ||
                    Globals::status->getCurrentCampaignMission() == 51) {
            wormholeExitBody:
                    if (messages->count >= 3 && ((RadioMessage *) ((*messages)[2]))->isOver() && m_nState == 0) {
                        for (unsigned int i = 0; i < enemies->count; ++i) {
                            if (!(*enemies)[i]->isWingMan()) {
                                (*enemies)[i]->player->setAlwaysFriend(false);
                                (*enemies)[i]->player->setAlwaysEnemy(true);
                                (*enemies)[i]->player->turnEnemy();
                            }
                        }
                        ((Standing *) (intptr_t) Globals::status->getStanding())->setStanding(0, 100);
                        m_nState = 1;
                    }
                }
                // The original checks state 2 before state 1
                ...
```

and the pre-switch gate becomes

```cpp
    } else if (Globals::status->getCurrentCampaignMission() == 50 && messages != nullptr) {
        goto wormholeExitBody;
    } else if (Globals::status->getCurrentCampaignMission() == 51 && messages != nullptr) {
        goto wormholeExitBody;
    }
```

Measured (`v_c70a_plain_nodock`):

| metric | value |
|---|---:|
| size | **57236** (+4) |
| raw same-address diff | 55,274 (+655) |
| process `getStanding` | `0x139aba` |
| process `setStanding` | **`0x139ac2`** (original body start; orig call `0x139b0e`) |
| `.rel.plt` | **0 diff, 29,392 B** |
| `byte_exact` (via `check`) | 4507 -> **2741** (section shift) |

The body start lands at `0x139a70`; the whole body is ~0x52 B earlier than the
original, but `setStanding` is at the original body's address and the PLT order
is correct.

## Full measured table (all anchor removed unless stated)

`raw` is the raw same-address diff for `_ZN11LevelScript7processEi`;
`ss` is the in-function `Standing::setStanding` call site; `plt` is the
`.rel.plt` byte diff.

| variant | shape | size | raw | ss | plt |
|---|---|---|---:|---:|---:|---:|
| **HEAD** | body after case 80, anchor present | **57232** | **54,619** | 0x13be28 | 0 |
| `v_c70a_plain` | body after case-70 state-0, `mission==50\|\|==51`, dock kept | 57268 | 54,948 | 0x139ac2 | 0 |
| `v_c70a_plain_nodock` | same, dock removed | **57236** | 55,274 | 0x139ac2 | 0 |
| `v_c70a_eq50_nodock` | same, guard `mission==50` | 57228 | 55,226 | 0x139ab6 | 0 |
| `v_c70a_le51_nodock` | same, guard `mission<=51` | 57228 | 55,226 | 0x139ab6 | 0 |
| `v_c70a_lt60_nodock` | guard `mission<60` | 57228 | 55,226 | 0x139ab6 | 0 |
| `v_c70a_ge50_nodock` | guard `mission>=50` | 57228 | 55,226 | 0x139ab6 | 0 |
| `v_c70a_state7f_nodock` | guard `m_nState==0x7f` | 57236 | 55,227 | **0x138a2e** | **16** (cold) |
| `v_c70g_eq50` | body at **start** of case 70, guard `mission==50`, dock kept | 57244 | 55,154 | 0x13995c | 0 |
| `v_c70g_eq50_nodock` | same, dock removed | 57220 | 55,259 | 0x13995c | 0 |
| `v_c70g_state7f_nodock` | start of case 70, guard `m_nState==0x7f` | 57212 | 55,276 | 0x139958 | 0 |
| `v_case70_elseif_body` | body as `else if` of state-0 | 57284 | 55,090 | 0x138a2e **and** 0x139b82 | 0 (body duplicated) |
| `v_case70_standalone_body` | standalone block in case 70 | 57284 | 55,189 | 0x138a3a + 0x139e8e | 0 (duplicated) |
| `v_c70e_none` | body after whole state-2/1 chain | 57204 | 55,168 | 0x138a20 | 16 (cold) |
| `v_body_in_case70_noexpect_noanchor` | body at start of case 70, no guard | 57204 | 55,168 | 0x138a20 | 16 (cold) |
| `v_body_in_case70_expect_noanchor` | start of case 70, `__builtin_expect(gate,1)` | 57260 | 55,252 | 0x139968 | 0 |
| `v_p_mergegate` | merged gate `(m==50\|\|m==51)&&messages` | 57256 | 55,405 | 0x139aba | 8 |
| `v_p_mergegate_l` | merged gate `m<=51&&messages` | 57220 | 55,323 | 0x138a18 | 24 (cold) |
| `v_g_range` | guard `m>=50 && m<=51` (2 calls) | 57244 | 55,122 | 0x138a2e | 16 (cold) |
| `v_g_sub` | guard `(unsigned)(m-50)<=1` | 57228 | 55,271 | 0x139aba | 0 |

Note: the current source shape (body inline in the mission-51 `else if`) puts
the body after case 80 at `0x13be28`; the anchor at `0x139b78` is what makes
the PLT order correct today.

## Why 57232 is not reachable from these shapes

The base is `57232 ≡ 0 (mod 8)`.  Every body-in-case-70 shape is
`≡ 4 (mod 8)` (57220, 57228, 57236, 57244, 57260, 57268, 57284).  Adding or
removing single 16-bit stores moves the size by 0 or 8 (they are absorbed or
add a pointer load + store), never by 4:

| edit on `v_c70a_le51_nodock` (57228) | size |
|---|---:|
| `m_pHud->visible = 0;` in body | 57236 (+8) |
| `m_pRadar->enabled = 0;` in body | 57236 (+8) |
| `m_pRadar->lockedEnemy = nullptr;` in body | 57236 (+8) |
| `m_bCinematicBreak = 1;` / `player->freeze = 1;` / `m_nScriptTimerA = 0;` in body | 57228 (+0) |
| `setStanding(0, 1000)` (movw) | 57228 (+0) |
| case-80 `lockedEnemy`/`dockNavPtr`/`freeze=0`/`enabled=1`/`hud=1` | 57228/57236 (+0/+8) |

Likewise on `v_c70a_plain_nodock` (57236): removing any single store from the
case-80 block, the mission-157 tail, or adding `m_nState++` reorderings is
size-neutral; the only large lever found is the case-80 `dockNavPtr` store
(worth 32 B in this layout, 8 B in the base layout).

A genuine **single 32-bit Thumb instruction** (a lone `str.w`/`ldr.w`/`blx`
that is not paired with a pointer load) is the remaining candidate for the
last 4 B.  Candidates not yet tried: a 32-bit `movw` immediate in a hot block,
a `b.w`-only tail, or hoisting the body's `m_nState = 1;` into the shared
mission-5 tail at `0x1381aa` (the original body ends `b.w 0x1381aa`; ours ends
inline `movs r0,#1; str r0,[r5,#28]`).

## Faithfulness notes

* The body-after-case-70-state-0 placement is the **most faithful** shape
  found: the original body starts at `0x139ac2`, immediately after case 70's
  state-0 block (`0x1399a2..0x139abe`), and this shape reproduces that
  adjacency (body start `0x139a70`, `setStanding` exactly `0x139ac2`).
* The `if (mission == 50 || mission == 51)` guard is **dead code on the
  fall-through path** (case 70 only runs for mission 70) and exists purely so
  LLVM keeps the block hot and places it after case-70 state 0.  The body is
  reached only via the pre-switch `goto`.  It is a plausible original shape
  (`if (mission == 50 || mission == 51)` guarding a labelled cutscene block)
  but is not proven.
* Removing the wave-143 metric-only case-80 `m_pRadar->dockNavPtr = nullptr`
  store is faithful (the original case-80 block has no `Radar+8` store) and is
  what brings the winning shape from 57268 to 57236.
* The `if (delta == 0x7d)` anchor (16 B) is removed in every variant above.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cd /dev/shm/w8903
python3 -c "import variants,harness; harness.run('v_c70a_plain_nodock', variants.v_c70a_plain_nodock)"
python3 reldiff.py
# -> size 57236 raw 55274 setStanding 0x139ac2, rel.plt 0 diff
```

## Open questions

1. Find the 4-byte (single 32-bit instruction) companion that turns 57236 into
   57232 while keeping `setStanding` at `0x139ac2` and `.rel.plt` byte-identical.
2. Or find a body-in-case-70 shape whose natural size is 57232 (e.g. sharing
   the `m_nState = 1` tail with the mission-5 body at `0x1381aa`, as the
   original does).
3. Once size-exact, the raw metric is expected to be ~+650 vs the current
   anchor shape; the anchor is worth -504 raw on the wave-143 base, so the
   tradeoff must be re-measured and labelled if it is a metric regression.
