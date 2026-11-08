# LevelScript::process — wave 149 / w9602: `.rel.plt` byte-identical at
# 57,232 B with a size-exact case-125 independent-if shape (no +12 B companion)

Status: **code change landed**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9602`, base master `7eed2966e` (wave 149).  Scratch in
`/dev/shm/w9602/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,898            54,791            (-107)
process align_diff       15,802            17,611            (+1,809)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    86,449            86,342            (-107)
.ARM.extab diffbytes     1,309             1,311             (+2)
.ARM.exidx               0                 0
.rel.plt                 4 bytes / 2 swap  0 bytes           **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The mission target is met: `.rel.plt` is byte-identical (29,392 bytes, 0
differing), the function is still exactly 57,232 B, and **no +12 B cold
strong-arm companion is needed** (unlike w9506).  The raw same-address metric
also improves by 107 bytes and the whole `.text` diff drops by 107.

## The swap and the first-call-order mechanism

The two `.rel.plt` entries are adjacent and only their order differs:

```
orig : 2835 PlayerStation::setVisible   0x13da1e
       2836 PlayerEgo::deleteHackingGame 0x13ded4
ours : 2835 PlayerEgo::deleteHackingGame 0x13d7da
       2836 PlayerStation::setVisible   0x13d9b0
```

The whole binary has exactly one call site of each, both in
`LevelScript::process`.  `.rel.plt` is emitted in first-reference order, so
the fix is purely "make the `setVisible` call site address lower than the
`deleteHackingGame` call site address".  `setVisible` lives in the case-42
state-7 block (`0x13d8fc`); `deleteHackingGame` lives in the case-125
`default` (hacking) block.  In the baseline the compiler places the hacking
block at `0x13d754`, **before** state-7; the original places it at
`0x13de44`, **after** state-7.

## The landed change

The case-125 `switch (m_nState)` is replaced by three independent `if`
blocks in the order **2, 3, default (4..6)**:

```cpp
if (m_nState == 2) { ...case 2 body... }
if (__builtin_expect(m_nState == 3, 1)) { ...case 3 body... }
if (m_nState >= 4 && m_nState <= 6) {
    if (__builtin_expect(player->isDockedToDockingPoint() && player->hackingWon()
                         && player->getHackingGameDockIndex() >= 0, 1)) {
        ...hacking body...
    }
}
```

The arms are mutually exclusive, so the reorder is semantics-preserving.
The independent-if form detaches the hacking block from the state-3 chain,
and MachineBlockPlacement sinks it after the case-42 state-7 block.  The
resulting call sites are:

```
ours (landed): setVisible 0x13d6fc  <  deleteHackingGame 0x13e996
```

The two `__builtin_expect(..., 1)` hints are placement levers for the
post-restructure MBP decision; `state3_hot` alone already gives the same
`.rel.plt` 0 at 57,232 B (raw 54,800 / aligned 17,592), and `hack_hot` alone
gives raw 54,782 / aligned 17,884.  The landed pair is the best raw+aligned
combination found that stays size-exact.

## Why this is cheaper than w9506

w9506's landed shape is the **2, default, 3** independent-if order plus a
`+12 B` cold strong-arm companion.  On this base that order is 57,224 B
before the companion (raw 55,279 / aligned 17,869); the companion restores
57,232 B but keeps the +2,071 aligned cost.  The **2, 3, default** order is
size-exact *on its own* and improves the raw metric.  It is a different
order from w9506, not a re-run of it.

## Swept variants (all built + linked + `.rel.plt`-checked)

| variant | size | raw | align | rel.plt | setVisible / deleteHackingGame |
|---|---:|---:|---:|---:|---|
| baseline switch | 57232 | 54898 | 15802 | 2 | 0x13d9b0 / 0x13d7da |
| **landed: indep 2,3,d + s3h + hackh** | **57232** | **54791** | **17611** | **0** | **0x13d6fc / 0x13e996** |
| indep 2,3,d (no expects) | 57232 | 54791 | 17865 | 0 | 0x13d6fc / 0x13e996 |
| indep 2,3,d + state3 hot | 57232 | 54800 | 17592 | 0 | 0x13d6fc / 0x13e996 |
| indep 2,3,d + hack hot | 57232 | 54782 | 17884 | 0 | 0x13d6fc / 0x13e996 |
| indep 2,3,d + outer hot + s3h | 57232 | 54803 | 17599 | 0 | 0x13d6fc / 0x13e996 |
| indep 2,d,3 (w9506 order) | 57224 | 55279 | 17869 | 0 | 0x13d6f4 / 0x13e7f8 |
| indep 3,2,d | 57364 | 55400 | 20138 | 0 | 0x13d904 / 0x13ea06 |
| indep 3,d,2 | 57552 | 55409 | 20246 | 18 | 0x13d9e4 / 0x13a0d8 |
| indep d,2,3 | 57220 | 55164 | 17520 | 18 | 0x13d7f4 / 0x139f32 |
| indep d,3,2 | 57752 | 55383 | 20811 | 18 | 0x13da4c / 0x139f70 |
| if/else 2,3,else default | 57232 | 54898 | 15802 | 2 | 0x13d9b0 / 0x13d7da |
| if/else 3,2,else default | 57224 | 55307 | 16076 | 2 | 0x13d9ac / 0x13d7bc |
| swap case labels 3,2 | 57224 | 55307 | 16076 | 2 | 0x13d9ac / 0x13d7bc |
| default moved after switch | 57308 | 55265 | 18211 | 2 | 0x13d9d8 / 0x13d800 |
| default moved before switch | 57400 | 55400 | 20533 | 837 | 0x13d90a / — |
| case 4/5/6 explicit | 57548 | 55066 | 19797 | 18 | 0x13d858 / 0x139f46 |
| case-42 arm order 7,6,0 | 57208 | 55351 | 15974 | 9 | 0x13bfbc / 0x13d96a |
| case-42 arm order 7,0,6 | 57220 | 54922 | 16361 | 13 | 0x13bf80 / 0x13d7d8 |
| case-42 arm order 0,7,6 | 57196 | 55316 | 15953 | 9 | 0x13bfc8 / 0x13d972 |
| case-42 arm order 0,6,7 | 57204 | 55324 | 15759 | 2 | 0x13e7cc / 0x13d7f2 |
| case-42 arm order 6,0,7 (base) | 57208 | 55363 | 15795 | 2 | 0x13d9b0 / 0x13d7da |
| case-42 arm order 6,7,0 | 57212 | 55300 | 15819 | 2 | 0x13e9c0 / 0x13d7ea |
| hoist setVisible to top of case 42 | 57256 | 55071 | 15810 | 11 | 0x13ab9a / 0x13d82a |
| `__builtin_expect` on hacking cond (0) | 57252 | 55372 | 19443 | 2 | 0x13d95c / 0x13d7e4 |
| `__builtin_expect` on hacking cond (1) | 57232 | 54914 | 15824 | 2 | 0x13d9b0 / 0x13d7da |
| deleteHackingGame moved to block end | 57688 | 55430 | 17739 | 2 | 0x13da3a / 0x13d8dc |

Case-42 arm reorders 7,6,0 / 7,0,6 / 0,7,6 do fix the target pair
(`setVisible < deleteHackingGame`) but rotate 9-13 other `.rel.plt` entries,
so they are not usable.  The independent-if 2,3,d shape is the only one
found that fixes the pair, stays 57,232 B, and leaves every other `.rel.plt`
entry in place.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9602/fdiff.py _ZN11LevelScript7processEi        # 54791 / 57232
python3 /dev/shm/w9602/align_diff.py _ZN11LevelScript7processEi  # 17611
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## Open questions

1. The aligned metric is still +1,809 because the hacking block lands at
   `0x13e996` instead of the original `0x13de44`.  A source shape that puts
   it immediately after state-7 (gap `0xe4`) is still open; `state3_cold`
   moves it to `0x13e810` but costs +16 B.
2. The `state3_hot` hint is the best aligned lever found (17,592), but
   `hack_hot` is the best raw lever (54,782).  An intermediate weight is not
   expressible with clang 7's 1:2000 `__builtin_expect`.
3. `.ARM.extab` is +2 bytes (1,309 -> 1,311); the LSDA header/call-site
   table is otherwise unchanged and the section size stays exact.
