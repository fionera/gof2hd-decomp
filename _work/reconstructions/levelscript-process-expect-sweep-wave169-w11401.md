# LevelScript::process — wave 169 / w11401: full expect-cell re-sweep on the wave-168 base

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11401`,
base master `ec3ceb07c` (wave 168).  Scratch `/dev/shm/w11401/`.

Continues `levelscript-process-expect-sweep-wave168-w11301.md` (cells 1-4
landed there).  This pass re-parsed the whole `process()` body (lines 605-5308)
and re-tested **every** `if`/`while`/`for` condition plus every `&&`/`||`
sub-expression at any nesting depth: **899 cells x 2 weights = 1,798
compiles**.

## Baseline (wave-168 master, gate green)

```
process raw diffbytes    54,015
process aligned insns    17,788
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          82,327
.ARM.extab diffbytes     668
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Method

`/dev/shm/w11401/gen.py` tokenizes the `process` body (comment/string aware),
finds every `if`/`while`/`for` condition (middle clause for `for`), and builds
an expression tree over `||`, `&&` and `!` so every nested operand is a cell.
`/dev/shm/w11401/sweep.py` compiles each `__builtin_expect((cell), e)` variant
with the exact NDK r18b clang 7.0.2 `-Oz` command (4-way parallel, ~1 s each)
and keeps only cells whose `process` symbol is still 57,232 B.

**763 / 1,798 cells are size-exact.**  Of those, 203 change the object's
`process` section bytes; the other 560 are byte-identical to the baseline
object (no codegen effect).  The 203 were linked and raw-measured with
`tools/fn_diff.py`; `.rel.plt` was compared byte-for-byte for each.

## Landed (this pass, one commit)

Two cells, additive:

| # | line | cell | e | raw | aligned | rel.plt |
|---|---|---|---:|---:|---:|---|
| 1 | 3153 | `m_nState == 11` (mission-94 state arm) | 1 | 54,006 | 17,776 | identical |
| 2 | 5067 | `messages[15]->isOver()` (mission-106 cold tail) | 0 | 54,008 | 17,793 | identical |

Combined:

```
process raw diffbytes    54,015 -> 53,999   (-16)
process aligned insns    17,788 -> 17,781   (-7)
orig / ours size         57232 / 57232      (size-exact)
.text diffbytes          82,327 -> 82,311   (-16)
.ARM.extab diffbytes     668 -> 669         (+1)
.rel.plt                 0                  (BYTE-IDENTICAL)
RATCHET PASS (byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN)
```

## Second commit: remove a stale expect (removal sweep)

The 55 existing `__builtin_expect` sites in `process` were also swept in the
opposite direction (strip the wrapper, keep the condition).  33 of the 55
removals stay at 57,232 B; all 33 were linked and raw-measured.  One win:

| line | removed expect | raw | aligned | rel.plt |
|---|---|---:|---:|---|
| 4601 | `__builtin_expect(m_nScriptTimerA >= 1001, 0)` (mission-106 state-4 arm) | **53,984** | 17,797 | identical |

It is the only removal below 53,999 (next best: line 1030 `!isFading()`
neutral at 53,999; line 4371 `>= 1001` neutral; line 4629 `shipGroup == 9`
neutral; line 4680 `>= 2001` 53,999).  Adding any other removal on top of it
does not improve it (53,984/53,985).

Cumulative after both commits:

```
process raw diffbytes    54,015 -> 53,984   (-31)
process aligned insns    17,788 -> 17,797   (+9)
orig / ours size         57232 / 57232      (size-exact)
.text diffbytes          82,327 -> 82,296   (-31)
.ARM.extab diffbytes     668 -> 669         (+1)
.rel.plt                 0                  (BYTE-IDENTICAL)
RATCHET PASS (byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN)
```

## Cell table — all size-exact cells that change object bytes (203)

Baseline raw 54,015 / aligned 17,788.  Only the cells at or below baseline are
listed individually; every other measured cell was worse.  `rel.plt` is
identical for every row unless marked.

| idx | line | cell | e | raw | aligned |
|---|---|---|---:|---:|---:|
| 474 | 3153 | `m_nState == 11` | 1 | **54,006** | 17,776 |
| 839 | 5067 | `messages[15]->isOver()` | 0 | **54,008** | 17,793 |
| 21 | 615 | `mission == 0` | 0 | 54,015 | 17,788 |
| 133 | 1368 | `m_nScriptTimerA >= 3001` | 0 | 54,015 | 17,788 |
| 462 | 3113 | `m_nScriptTimerA >= 8001` | 0 | 54,015 | 17,788 |
| 577 | 3705 | `i == 3` | 1 | 54,015 | 17,788 |
| 724 | 4371 | `m_nScriptTimerA >= 1001` | 1 | 54,015 | 17,788 |
| 788 | 4812 | `m_nScriptTimerB <= 6999` | 0 | 54,015 | 17,788 |
| 795 | 4866 | `isRunningHDonWeakDevice` | 0 | 54,015 | 17,788 |
| 313 | 2217 | `isAlwaysFriend()` | 1 | 54,016 | 17,785 |
| 152 | 1437 | `m_nState == 2` | 1 | 54,016 | 17,786 |
| 239 | 1774 | `msg2->isTriggered() && m_nState == 0` | 1 | 54,016 | 17,787 |
| 241 | 1774 | `m_nState == 0` | 1 | 54,016 | 17,787 |
| 71 | 1029 | `m_nState == 3` | 1 | 54,016 | 17,788 |
| 118 | 1335 | `!layout->isFading()` | 0 | 54,016 | 17,788 |
| 83 | 1118 | `m_nScriptTime >= 27001` | 0 | 54,017 | 17,786 |
| 108 | 1288 | `m_nScriptTime >= 38001` | 1 | 54,020 | 17,788 |
| 110 | 1294 | `m_nScriptTime >= 39001` | 1 | 54,020 | 17,788 |
| 76 | 1079 | `m_nScriptTime >= 7901` | 0 | 54,024 | 17,760 |
| 68 | 1000 | `m_nScriptTime >= 14001` | 1 | 54,031 | 17,790 |
| 27 | 713 | `speedBits == 2.0f` | 1 | 54,088 | 17,791 |
| 85 | 1200 | `m_nState == 2` | 0 | 54,111 | 17,761 |
| 82 | 1118 | `m_nScriptTime >= 27001` | 1 | 54,151 | 17,796 |
| 19 | 713 | `speedBits == 2.0f` | 1 | 54,839 | 17,796 |
| 84 | 1200 | `m_nState == 2` | 1 | 54,912 | 17,750 |
| 96 | 1239 | `m_nState >= 5 && m_nState <= 8` | 1 | 55,019 | 17,926 |
| 93 | 1239 | `m_nState >= 5 && m_nState <= 8` | 1 | 54,794 | 17,779 |
| 146 | 2060 | `ship != nullptr && ship->shipGroup == 9` | 1 | 54,344 | 18,706 |
| 119 | 1335 | `!layout->isFading()` | 1 | 55,138 | 18,766 |
| 138 | 2060 | `ship != nullptr && ship->shipGroup == 9` | 1 | 54,344 | 18,706 |
| 147 | 2060 | `ship != nullptr && ship->shipGroup == 9` | 0 | 54,894 | 17,796 |
| 873 | 5210 | `m_nScriptTimerA >= 1001` | 1 | 54,465 | 20,966 |
| 878 | 5230 | `m_nScriptTimerA >= 2001` | 1 | 54,871 | 17,789 |

`.rel.plt` rotated (NOT landable) for 12 measured cells: idx 648, 294, 646,
170, 183, 184, 293, 295, 296, 273, 572, 243.

## Combinations

On the landed pair, adding any third cell re-worsens raw: 313:1 -> 54,000,
152:1 -> 54,000, 239:1 -> 54,000, 241:1 -> 54,000, 71:1 -> 54,000, 83:0 ->
54,001, 108:1 -> 54,004, 110:1 -> 54,004, 118:0 -> 54,000, 76:0 -> 54,008.
The pair is the minimum of the tested combination space.

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j2
```
