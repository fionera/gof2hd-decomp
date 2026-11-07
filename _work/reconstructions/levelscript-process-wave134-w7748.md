# LevelScript::process — wave 134 / w7748 (switch case-order permutation audit + mission-5/26 order fix)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `ec0acea1e`.

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.8
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        20169 / 20044     20169 / 20044
process .text diffbytes  54465             54419            (-46)
whole .text diffbytes    85143             85097            (-46)
.ARM.extab diffbytes     1416              1416
.note.gnu.build-id       20                20
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The fuzzy pct dips 0.1 while the function's own differing-byte count improves by 46 B — the
same trade-off recorded for waves 129/132 (a faithful reorder re-rolls block placement). The
hard gate (byte_exact / linked_exact / section sizes) is unchanged.

## New hypothesis tested: switch case-order permutations

w7727 audited the literal pool and `tbh` tables but did not test whether reordering the `case`
labels changes clang's `tbh` edge order or the literal-pool word order.

**Result: it does not.** Reordering the mission-0 `switch (m_nState)` case bodies from
`6,7,8,9,10,11,12,13,14,15` to `15,14,13,12,11,10,9,8,7,6` produces **byte-identical**
`LevelScript::process` code (57232 B, `diffbytes 0`). Every tested permutation
(`6,7,8,9,10,11,12,13,15,14`; `6,10,7,8,9,11,12,13,14,15`; `10,7,8,9,11,12,13,15,14,6`;
reverse) produces the same 57232 B body and the same 58.9 pct. clang 7.0.2's switch lowering
orders the `tbh` edges by **case value**, not by source order, so this lever is exhausted.

Evidence (original `tbh` @0x139b22, 10 entries for states 6..15, decoded from the halfword
table at 0x139b24):

| state | target |
|---|---|
| 6  | 0x139b6e |
| 7  | 0x13d020 |
| 8  | 0x13d13a |
| 9  | 0x13d1dc |
| 10 | 0x13cf74 |
| 11 | 0x13d2d2 |
| 12 | 0x13d3b6 |
| 13 | 0x13d43e |
| 14 | 0x13d4f2 |
| 15 | 0x13d4c4 |

Sorted by address the original body layout is `6,10,7,8,9,11,12,13,15,14`. Ours (any source
order) has a 12-entry table (states 4..15) whose sorted layout is
`5,10,7,8,9,11,12,13,14,15,6,4` — the interior `10,7,8,9,11,12,13` already matches; the
remaining deltas are (a) states 4/5 folded into the table (fall-through state 5 instead of
state 6) and (b) 14/15 swapped.

## Landed change: mission-5 checked before mission-26

The original's post-tail mission-special chain is checked in the order **5, 26, 50, 51**
(disasm: `0x13812a cmp r0,#5; bne 0x1382dc`, then `0x1382dc cmp r0,#26; bne 0x1383e4`).
Our source checked 26 first. Swapping the two `else if` blocks is a pure faithfulness fix
(no semantic change, both branches are mutually exclusive) and moves the mission-5 body's
literal-pool block earlier:

```
ORIG pool 23..27: 5000.0 | 30000.0 | pi | 10000.0 | 10000.0
BASE pool 23..27: 10000.0 | 10000.0 | <addr> | <addr> | 5000.0   (mission-5 at 29)
SWAP pool 23..27: 10000.0 | 10000.0 | <addr> | 5000.0 | 30000.0   (mission-5 at 27)
```

The remaining 4-word gap is the mission-0x51 body being emitted before the mission-5 body;
that is a block-placement consequence of the tail's `else if` structure (see below).

## Measured experiments (all semantic-preserving, size 57232 unless noted)

| variant | size | pct | process diffbytes | mission-0 tbh |
|---|---:|---:|---:|---|
| base (26 then 5) | 57232 | 58.9 | 54465 | 4..15 |
| **swap 5 before 26 (landed)** | **57232** | **58.8** | **54419** | 4..15 |
| case order 6,10,7,8,9,11,12,13,15,14 | 57232 | 58.9 | 54465 | 4..15 |
| case order reversed 15..6 | 57232 | 58.9 | 54465 | 4..15 (identical bytes) |
| mission-0 branch order 4,3,5 | 57256 | 56.5 | — | **6..15** |
| 4,3,5 + case reorder | 57256 | 56.5 | — | 6..15 |
| 4,3,5 + swap 5/26 | 57256 | 56.4 | — | 6..15 |
| tail `else if`->`if` semantic fix + swap | 57468 | 57.4 | — | 4..15 |

Key negative results:

* **Case-order permutation is a no-op** (byte-identical output). The `tbh` edge order is fixed
  by case values.
* The faithful **4,3,5** branch order does produce the original's **6..15** `tbh`, but costs
  **+24 B** (57256) and the fuzzy score falls to 56.5. It still needs a -24 B companion
  (w7727 open lead #1).
* The genuine semantic fix (`if (mission==5)` instead of `else if`, so the specials run even
  when `m_bStartSequence != 0`, matching `0x137f82 beq 0x13812a`) costs **+236 B** (57468)
  and scores 57.4. It is the only change that makes the tail fall through to the mission-5
  check, so it is the right direction for pool order, but needs a -236 B companion.

## Open leads (for the next wave)

1. The `if (mission==5)` semantic fix (+236 B) is the correct CFG shape; find a -236 B
   companion (missing statements in the mission-0/1 bodies) to land it at 57232 B. That
   should move the mission-5 pool block ahead of mission-0x51 and close the first pool
   divergence.
2. The 4,3,5 branch order (+24 B, correct 6..15 `tbh`) needs a -24 B companion.
3. The interior `tbh` body order already matches (`10,7,8,9,11,12,13`); only the 14/15
   swap and the folded states 4/5 remain.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 _work/reconstructions/w7748/score.py             # 58.8
python3 _work/reconstructions/w7748/bytediff.py          # 54419
python3 _work/reconstructions/w7748/secdiff.py           # .text 85097, extab 1416
```

Tracked helpers live in `_work/reconstructions/w7748/`: `score.py`, `bytediff.py`,
`secdiff.py`, `analyze.py` (pool + `tbh` dump), `reorder_cases.py` (case-body permutation),
`reorder_branches.py` (mission-0 if/else permutation), `swap_26_5.py` (the landed change).
`analyze.py` and the reorder scripts assume the repo root as cwd.
