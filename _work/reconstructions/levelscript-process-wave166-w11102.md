# LevelScript::process — wave 166 / w11102: mission-type switch shape (re-check)

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11102`, base master `85c1f88df`.  Scratch
`/dev/shm/w11102/`.

## Baseline (HEAD, unchanged, tree left clean)

```
process raw diffbytes    54,247
process aligned insns    17,532
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Correction to the wave-164 w11001 note: the outer campaign switch is MATCHED

w11001 reported the 131..145 physical body order as
original `131,135,145,144,142,139` vs ours `131,135,144,139,145,142`.
That is wrong (its "ours" addresses came from decoding the tbh at the
wrong base).  Re-decoded this pass from the actual `tbh` at `0x138a8a`
(base `0x138a8e`, **same address in both binaries**):

```
orig 131->0x138aac 135->0x1391c0 139->0x13965c 142->0x139544 144->0x1395e0 145->0x1392f6
ours 131->0x138aac 135->0x139130 139->0x1395b4 142->0x13947a 144->0x139538 145->0x139266
```

Physical order is **identical**: `131,135,145,142,144,139` in both.
Likewise the 64..73 prong (`tbh` @`0x138c3e`, base `0x138c42`):

```
orig 64->0x138c56 67->0x139892 69->0x139720 70->0x1399a2 73->0x139834
ours 64->0x138c56 67->0x1397d4 69->0x139662 70->0x1398d6 73->0x139776
```

Physical order `64,69,73,67,70` in both; entry order (`0,3,5,6,9` non-tail)
identical.  The 20-value linear compare chain is also identical.  So the
outer "mission-type" switch (20 linear + 6 + 5 = 31 physical bodies) is
**structurally matched**; only per-body sizes differ.

## The mission-0 state switch is the remaining mismatch

Original: `tbh` 10 entries, states **6..15**, physical body order
**6,10,7,8,9,11,12,13,15,14**; state 16 is the `bhi` default.
HEAD (4,5,3 source order): `tbh` 13 entries, states **3..15**, physical
order **9,15,6,7,8,10,11,12,13,14**.

The original source order is **4, 3, 5**:
```
if (state <= 2) ... else if (state == 4) ... else if (state == 3) ...
else if (state == 5 && msg12->isOver()) ... else switch (state) { 6..15 }
```
Emitted test order is `test4, test3, body3 inline, body4 out-of-line,
test5, body5 inline` (0x1389c8 / 0x1389ce / 0x1389d4 / 0x138d2a / 0x138fd8
/ 0x138fde).  The state-5 arm must be last because its `&&` is what keeps
state 5 out of the switch (10-entry table).

## Measured shapes this pass (full links, `tools/fn_diff.py`)

| shape | raw | aligned | size | tbh |
|---|---:|---:|---:|---|
| **HEAD 4,5,3** | **54,247** | **17,532** | **57,232** | 13 (3..15) |
| 4,3,5 | 55,312 | 18,234 | 57,240 (+8) | 10 (6..15) |
| 4,3,5 + `default:` state-16 inside switch | 55,362 | 18,397 | 57,264 (+32) | 10 |
| 4,3,5 + `++m_nState` in cases 6..15 | 55,330 | 18,390 | 57,248 (+16) | 10 |
| HEAD + `default:` state-16 inside switch | 55,332 | 18,634 | 57,260 (+28) | 13 |
| HEAD + `++m_nState` in cases 6..15 | 55,113 | 18,531 | 57,260 (+28) | 13 |
| HEAD − dead `m_bCinematicBreak = 0` (mission-2 state-1 arm) | 54,529 | 17,539 | 57,224 (−8) | 13 |
| 4,3,5 − dead store | 54,865 | 18,242 | 57,224 (−8) | 10 |

Notes:
* `++m_nState` is semantically identical in every mission-0 case (all
  transitions are +1) but clang does **not** fold it back to the constant
  store: it emits a reload/add instead, +28 B.
* Moving the `if (m_nState == 16)` block into the switch as `default:`
  also costs +28 B and regresses.
* The dead `m_bCinematicBreak = 0` at src line 1685 is worth **−8 B**
  (not −4 as w11001 guessed) and is a raw regression on its own.

## Conclusion

* Exact original mission-0 source order: **4, 3, 5**; `tbh` **10 entries
  states 6..15**; physical order **6,10,7,8,9,11,12,13,15,14**.
* Exact original outer campaign switch order: linear chain
  `14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,114,125,154,157,158`,
  range 131..145 physical `131,135,145,142,144,139`, range 64..73 physical
  `64,69,73,67,70` — **all matched by HEAD**.
* Best measured shape on this base is still **HEAD 4,5,3** (54,247 /
  17,532 / 57,232).  Every 10-entry-table shape regresses raw because the
  wave-164 `__builtin_expect` cells are tuned to the HEAD block layout and
  the mission-0 case bodies are not yet byte-faithful.  Nothing landed;
  tree left clean.
