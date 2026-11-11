# LevelScript::process — wave 171 / w11704: re-land w11604 alias set on the wave-171 base

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11704`, branch
`wave169-w11704`, base master `41ef4c8b6` (wave 171).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11704`.

Follow-up to `levelscript-process-alias-wave170-w11604.md` and
`levelscript-process-expect-sweep-wave170-w11601.md`.  On the wave-171 base
(w11601's expects landed, w11604's aliases dropped by `c1f945835`) the whole
w11604 alias set costs +194 raw (53,812 -> 54,006), but individual blocks were
re-measured one at a time and the non-regressing ones re-landed.

## Baseline (wave-171 master, gate green)

```
process raw diffbytes    53,812
process aligned insns    17,932
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          81,661
```

## Method

The 28 hunks of `c1f945835` (the commit that dropped w11604's set) were split
into the 16 alias blocks (A..P) and reverse-applied one block at a time onto
the current base; each candidate was built with `link_so -j2` and measured with
`tools/fn_diff.py`.  Every alias is a block-scoped `Type *x = m_pX;` copy of a
member that is only assigned in the constructor, so the rewrite is semantics-
preserving.  Size was checked on every candidate; only 57,232 B shapes were
kept.

## Individual block results on the wave-171 base (raw 53,812)

| blk | site | alias | raw | delta |
|---|---|---|---:|---:|
| A | mission-1 state-1 msg6 block | m_pCamera | 53,811 | -1 |
| B | mission-1 state-3 block | m_pRadar | 53,817 | +5 |
| C | case 8 rumble block | m_pCamera | 53,811 | -1 |
| D | mission-0 timer>=6001 block | m_pGeometry5 | 54,017 | +205 |
| E | state-9 msg3 block | m_pCamera | 53,807 | -5 |
| F | case-29 intro block | m_pCamera | 53,832 | +20 |
| G | mission-42 state-7 particle block | m_pParticleGeom0 | 53,819 | +7 |
| H | mission-80 state-0 block | m_pGeometry1 | 53,809 | -3 |
| I | state-9 msg10 block | m_pCamera | 53,810 | -2 |
| J | state-4 leader block | m_pCamera | 53,808 | -4 |
| K | mission-92 state-7 dock block | m_pRadar | 54,324 | +512 (size 57,224!) |
| L | state-3 reframe block | m_pCamera | 53,809 | -3 |
| M | state-4 msg10 block | m_pCamera | 53,808 | -4 |
| N | m_pGeometry6==nullptr block | m_pCamera | 53,809 | -3 |
| O | mission-157 state-7 block | m_pGeometry4 | 53,807 | -5 |
| P | mission-70 state-11 block | m_pCamera | 53,807 | -5 |

The 11 non-regressing blocks (A,C,E,H,I,J,L,M,N,O,P) combine **exactly
additively**: raw 53,812 -> 53,776 (-36), size 57,232, aligned 17,932 -> 18,217,
insn 20,724 -> 20,727.  Adding any of B,D,F,G on top gives +5/+212/+23/+7;
B+G is +12 (additive), so the 11-block set is a local optimum among the 16.

K is size-changing on this base (57,224 B) even block-scoped, so it cannot be
used; the mission-42 state-7 alias (G) must stay block-scoped (hoisting it also
shrinks the function to 57,224 B).

## Second sweep: new block aliases on the 53,776 base

A brace-block parser enumerated all 232 (block, member) pairs with >=2 uses
(members m_pCamera/m_pRadar/m_pLevel/m_pHud/m_pGeometry0..6/m_pExplosion/
m_pParticleGeom0..2, skipping blocks with an assignment to the member or an
existing alias of it).  Three new `m_pCamera` block aliases are raw wins:

| site | alias | raw | delta |
|---|---|---:|---:|
| mission-0 `case 2` block (translate + setPosition) | m_pCamera | 53,773 | -3 |
| mission-0 `case 10` block (translate + first setRumblePercentage) | m_pCamera | 53,775 | -1 |
| `case 41` state-0 block (enableFirstPersonCam + setLookAtCam) | m_pCamera | 53,772 | -4 |

Combined: raw 53,776 -> **53,768** (-8, additive), size 57,232, aligned 18,231,
insn 20,730.

Placement variants (all worse): case-41 alias at the first use instead of the
block head +4; alias around the later setTarget/setPosition/translate run +4;
alias covering all 5 uses of the first if block +516 (size 57,224); case-10
alias covering the third use +1,124 (size 57,240); case-41 else-if (state 1)
alias +303; case-41 state-3 alias +642 (size 57,224).

## Neutral

On the 53,768 base, re-measured raw-neutral size-exact aliases (no raw change):
`case 70` m_pHud (aligned 18,231 -> 18,027) plus m_pCamera/m_pRadar/m_pLevel
aliases at five other blocks.  All six together stay raw 53,768 (aligned
18,039).  The case-70 `Hud *hud` alias was landed (raw-neutral, aligned -204);
the other five were not (raw neutral and no aligned gain worth the extra local).

## Landed

| # | commit | change | raw | aligned |
|---|---|---|---:|---:|
| 1 | `ddf50627b` | re-land 11 non-regressing w11604 block aliases | 53,812 -> 53,776 | 17,932 -> 18,217 |
| 2 | `dee995b7a` | 3 new m_pCamera block aliases (mission-0 case 2/10, case 41) | 53,776 -> 53,768 | 18,217 -> 18,231 |
| 3 | (case-70 commit) | `Hud *hud` block alias, raw-neutral | 53,768 -> 53,768 | 18,231 -> 18,027 |

Final: **raw 53,768 / aligned 18,027 / size 57,232 / `.text` 81,617** (gate
byte 4507 / linked 4507 / size 4524 / data 843 / imports 207, lint CLEAN,
drift 0, parity 5370/5370).  `.rel.plt` byte-identical.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53768 aligned= 18027 size= 57232/57232 insn= 20729/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
