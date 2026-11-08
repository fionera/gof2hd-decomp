# LevelScript::process — wave 147 / w9301: `.rel.plt` byte-identical
# (setVisible before deleteHackingGame) at the exact 57,232 B / 0x144 LSDA

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `9c9744ca5` (wave-146
docs head). Scratch helpers in `/dev/shm/w9301/` (not tracked): `measure.py`,
`measure2.py`, `align_diff.py`, `fdiff.py`, the `sweep*.py` variant harnesses.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,869            55,263            (+394)
process align_diff       15,737            17,904            (+2,167, indicative)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
process LSDA             0x144             0x144             (header words identical)
whole .text diffbytes    85,928            86,322            (+394)
.ARM.extab diffbytes     1,393             1,397             (+4)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 4 bytes           0 bytes           **BYTE-IDENTICAL**
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
parity (sodiff)          5370/5370         5370/5370
RATCHET                  PASS              PASS
```

The mission target is met: `.rel.plt` is now **byte-identical** (29,392 bytes,
0 differing), the function is still exactly 57,232 B, and the LSDA header /
call-site table length are unchanged (the first five LSDA words are identical;
`.ARM.extab` size stays exact at 156,312).

## The landed change (one edit)

The case-125 inner `switch (m_nState)` is replaced by **three independent
`if` blocks in the order 2, default(4..6), 3**:

```cpp
if (m_nState == 2) { ...case 2 body... }
if (m_nState >= 4 && m_nState <= 6) {
    if (player->isDockedToDockingPoint() && player->hackingWon()
           && player->getHackingGameDockIndex() >= 0) { ...hacking body... }
}
if (m_nState == 3) { ...case 3 body... }
```

The arms are mutually exclusive, so the reorder is semantics-preserving.

Why this works: the original's case-125 dispatch tests `state == 2` **first**
and falls through to the case-2 body (`cmp r0,#2; bne else; case2...`); the
`else` chain (state-3 test + case-3 body + hacking default) is a separate
chain that MachineBlockPlacement sinks to *after* the case-42 state-7 block.
Our base `switch` lowered the other way (`cmp r0,#3; beq case3; cmp r0,#2;
bne default`), which kept the case-3 body + hacking default in the case-125
chain and placed them *before* state-7.  Splitting the switch into independent
`if`s makes case 2 the fall-through and detaches the state-3/hacking chain,
so the compiler reproduces the original's block order:

```
orig : setVisible 0x13da1e   deleteHackingGame 0x13ded4   lookBehind 0x13e132
ours : setVisible 0x13d6f4   deleteHackingGame 0x13e7f8   lookBehind 0x13eb52
```

`setVisible < deleteHackingGame < lookBehind` holds, so the first `.rel.plt`
reference order is correct and the whole section is byte-identical.

The `if (m_nState >= 4 && m_nState <= 6)` wrapper is the *placement* lever;
the inner guard is the original's player/hacking condition with the redundant
`m_nState` range removed (it now lives in the outer `if`).

## Honest metric note

This is a **`.rel.plt`-first** change: the whole `.rel.plt` section becomes
byte-identical (worth 4 bytes and, more importantly, removes the last
non-`lookBehind` relocation-order defect), but the same-address raw diff rises
by 394 (54,869 -> 55,263) and `.ARM.extab` diff by 4 (1,393 -> 1,397) because
the hacking block is now laid out after state-7 (the original's position)
instead of before it.  The align_diff metric is "indicative only" for this
function; it rises 15,737 -> 17,904.

### The size-exact alternative that is *not* landed

`if (state==2) b2; if (state==3) b3; else bd` (the same independent-if shape
but with the default as the `else` of the state-3 test) gives a *better* raw
and the correct layout, but is **57,228 B (-4)**:

| variant | size | raw | `.rel.plt` | setVisible / deleteHackingGame |
|---|---:|---:|---:|---|
| **landed (2, default, 3)** | **57232** | **55,263** | **0** | 0x13d6f4 / 0x13e7f8 |
| `2, 3, else default` | 57228 | 54,608 | 0 | 0x13d6fc / 0x13e98e |
| base `switch` (HEAD) | 57232 | 54,869 | 4 | 0x13d9b0 / 0x13d7da |

The -4 is a literal-pool / branch-width artefact (the `2,3,else` shape has one
fewer 4-byte literal); every attempt to add exactly 4 bytes (unique literal,
`m_nState = m_nState`, guard reorder, `m_nState >= 6`, etc.) either stayed at
57,228 or jumped to 57,236/57,240.  The landed 57,232 form is preferred
because section sizes must stay exact.

## What was swept (all built + linked, all `.rel.plt`-checked)

* Split the case-125 `switch` into independent `if`s in all 6 orders:
  2,3,default = 57,240; 2,default,3 = **57,232**; 3,2,default = 57,356;
  3,default,2 = 57,540; default,2,3 = 57,228; default,3,2 = 57,736.
* `if/else if/else` forms (2,3,default): 57,312 (`.rel.plt` 2) - the
  `else` keeps the state-3 test and default in one chain, so it does not move
  the hacking block far enough.
* `__builtin_expect` on the case-125 conditions: hot state-2 pushes the
  whole else chain to the cold end (deleteHackingGame at 0x145bae); cold
  state-2 pushes it early (0x139f46); neither lands at 57,232.
* `__builtin_expect` on the case-42 state tests (state 6/7 hot/cold): the
  state-6-cold form does move setVisible before deleteHackingGame, but it also
  reorders mission-89 state 4 and mission-0 state 4, leaving 14 `.rel.plt`
  entries swapped and size 57,236.
* Default-body guard reorders (`player->isDocked...` first, `<=6 && >=4`,
  `hackingWon` before `isDocked`, etc.): size 57,228 or 57,236.
* `m_nState == 4 || == 5 || == 6`, `(unsigned)(m_nState-4) <= 2`,
  `m_nState > 3 && m_nState < 7`: 57,236 / 57,228 / 57,228.
* Case-42 arm orders 607/670/706/076/067/760 re-measured on this base:
  none fixes the swap (matches w9203).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9301/measure.py                        # size 57232, raw 55263, rel.plt 0
python3 /dev/shm/w9301/align_diff.py _ZN11LevelScript7processEi   # 17904
```

## Open leads

1. The `2,3,else default` shape is 57,228 B with raw 54,608 and the exact
   `.rel.plt`; finding the missing 4-byte literal (it is one literal-pool
   entry / branch-width flip) would give a strictly better variant at 57,232.
2. The raw/align_diff cost is entirely the block shift; a source shape that
   places the hacking block immediately after state-7 (as the original does,
   `0x13de44` right after `0x13de40`) rather than after the case-3 body would
   recover most of the raw diff.
3. `__builtin_expect` on the case-42 state tests is a real block-placement
   lever but currently costs too many other `.rel.plt` entries; a combined
   lever (state-6 cold + mission-89/0 state-4 hot) might close both.
