# LevelScript::process — wave 155 / w10103: w10005+w10006 composition
# at 57,232 B (raw 55,292, aligned 17,171)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Base master `f4e0ea40b` (wave 154, w10007
landed).  Scratch `/dev/shm/w10103/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,327            55,292            (-35)
process aligned insns    17,176            17,171            (-5)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab size          0x26298           0x26298           (exact)
.rel.plt                 0                 0                 **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact
.ARM.exidx               exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. The landed composition (6 edits)

w10005's five edits plus w10006's `F` lever (the wormhole state-1
`m_nState++`).  All are semantically identical and order-preserving:

| # | line | edit | source |
|---|------|------|--------|
| A | 3926 | case-125 state-0/1 `if` -> `else if` | w10005 |
| B | 2103 | case-42 arms reordered `6,0,7` -> `0,6,7` | w10005 |
| C | 2105 | case-42 state-0 `m_nState = 1` -> `m_nState++` | w10005 |
| D | 4236 | case-142 state-0 `m_nState = 1` -> `m_nState++` | w10005 |
| E | 4931 | c157 s7 `m_nScriptTimerA <= 9999` -> `__builtin_expect(..., 1)` | w10005/w10006 |
| F | 1817 | wormhole state-1 `m_nState = 2` -> `m_nState++` | w10006 |

The two shapes are disjoint except `E` (shared).  Applying w10005 then
w10006 or w10006 then w10005 produces **byte-identical** output (the edits
are independent text replacements; the only shared lever is idempotent).

## 2. Why not the full V12 composition

The full composition `A B C D E F G H I K` is **57,228 B (-4), aligned
17,546** — worse than baseline.  The culprit is `H`
(`expect(m_nScriptTimerA >= 2001, 0)` on c154 s9): it costs **+321 aligned**
on top of `ABCDEFI` (17,168 -> 17,489).  The w10006 note's premise was that
`H` is needed for the c154 s9 -> c157 s5 strong-arm adjacency; on the
current master the adjacency is **already present** in the base build:

```
ours base:  145bbc strd r9,r9,[r5]
            145bc0 b.w  13f3b0
            145bc4 movs r5,#11      <- c157 s5 strong arm, gap 0x4
```

so `H` is not needed to keep the adjacency and only hurts the aligned
metric.  `ABCDEF` keeps the base's adjacency (measured gap 0x4).

## 3. Case-entry table (deliverable)

Extracted by decoding the mission dispatch compare chain plus the two TBH
tables (`sub.w rX,r0,#131; cmp rX,#14` and `sub.w rX,r0,#64; cmp rX,#9`)
and sorting the resolved case-body addresses.  Script:
`/dev/shm/w10103/table6.py`.

```
orig: 158,1,131,64,135,145,142,144,139,69,73,67,70,0,29,41,80,56,125,92,
      157,40,102,154,24,14,21,42,91,94,105,114,16,2,3
ours: 158,1,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,
      157,40,102,154,24,14,21,42,91,94,105,114,16,0,2,3
```

The dispatch chain order is identical.  The only physical-order difference
is `case 0` (mission-0 intro body): original `0x139ac2` (a shared sub-block
inside case 70's span), ours `0x13bd22` (a separate block after case 16).
This is **pre-existing** in the base (verified on the w9904 base with the
w10007 status-local reverted) and unchanged by this composition.

## 4. Measured alternatives (full build + link)

| variant | size | raw | aligned | notes |
|---|---:|---:|---:|---|
| base (w10007) | 57,232 | 55,327 | 17,176 | |
| **ABCDEF (landed)** | **57,232** | **55,292** | **17,171** | composition |
| FE (F + E only) | 57,232 | 55,279 | **16,977** | not a case-125 composition |
| ABCDEFI | 57,240 | 55,430 | 17,168 | +8 B |
| ABCDEFW | 57,212 | 55,325 | 17,127 | -20 B |
| ABCDEFIW | 57,220 | 55,388 | 17,128 | -12 B |
| ABCDEFWG | 57,236 | 55,297 | 17,197 | +4 B |
| V12 (full) | 57,228 | 55,413 | 17,546 | -4 B |
| ABCDEFH | 57,196 | 55,360 | 17,492 | H hurts |

`W` = c157 s6 `m_nState = 7` -> `m_nState++`; `I` = c157 s5 timer expect.
`ABCDEFW` (17,127) and `ABCDEFIW` (17,128) are better aligned but not
size-exact; no +12/+20 order-preserving companion was found inside the
budget.  `FE` is the best raw/aligned pair measured but drops the case-125
and case-42/142 fixes.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10103/fdiff.py _ZN11LevelScript7processEi        # 55,292 / 57,232
python3 /dev/shm/w10103/align_diff.py _ZN11LevelScript7processEi  # 17,171
python3 /dev/shm/w10103/relplt.py                                 # .rel.plt 0
python3 /dev/shm/w10103/table6.py                                 # case table
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

## 6. Open questions

1. `ABCDEFW` is 57,212 / 17,127 and `ABCDEFIW` is 57,220 / 17,128; a
   size-exact (+20 / +12) order-preserving companion would land either.
   The `G` (+24) and `H` (-36) levers overshoot and `H` costs +321 aligned.
2. The mission-0 body placement (case 0) is the only remaining case-order
   difference; a source shape that puts it at the original's location
   (inside the case-70 span) would also close a large aligned delta.
3. `FE` (16,977) shows the c157 s7 timer weight plus the wormhole inc is
   worth -199 aligned on its own; the case-125 else-if and case-42/142
   reorder cost +194 aligned in combination.
