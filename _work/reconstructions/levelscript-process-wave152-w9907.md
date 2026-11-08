# LevelScript::process — wave 152 / w9907: c154 s9 completion immediately
# before the c157 s5 strong arm at exactly 57,232 B, mission order preserved

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Base master `4a6104a43` (wave 152).
Scratch in `/dev/shm/w9907/`.

## Result (landed)

```
                         HEAD (wave 152)   landed (this pass)
process raw diffbytes    55,347            55,391            (+44)
process align_diff       17,223            17,401            (+178)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The mission target is met: the case-154 state-9 completion block is
**immediately before** the case-157 state-5 strong arm (completion tail
`strd; b.w` then `movs r5,#11; movs r4,#0`, gap **8 B** = the `b.w`), at
exactly 57,232 B, with the original mission case-entry order.

## 1. The -44 adjacency pair

`w9805`'s pair, unchanged:

```cpp
if (__builtin_expect(m_nScriptTimerA >= 2001, 0)) { ... }        // case 154 s9
if (__builtin_expect(Globals::isRunningHDonWeakDevice, 1)) { ... } // case 157 s5
```

Measured alone: **57,188 B**, raw 55,471, aligned 17,541, completion
`strd; b.w` immediately before `movs r5,#11` (gap 8), mission order
preserved.  It is -44 B, so it needs a +44 companion.

## 2. The +44 companion (5 levers, all order-preserving)

The +44 is **not a single lever**: every single positive lever in the
case-154/157 region is smaller than 44 B, and the effects are strongly
non-additive (the same edit is +32 alone and +0 in a different layout).
The landed set is:

| # | line | edit | region |
|---|------|------|--------|
| 1 | 1816 | `m_nState = 2` -> `m_nState++` (wormhole state-1 arm; state is 1) | case 24/14 |
| 2 | 4640 | `messages[6]->isOver()` -> `__builtin_expect(..., 1)` | c154 s6 |
| 3 | 4669 | `expect(m_nScriptTimerA >= 2001, 1)` -> `..., 0)` | c154 s9 |
| 4 | 4798 | `m_nState = 4` -> `m_nState++` (c157 state 3) | c157 s3 |
| 5 | 4867 | `expect(m_nScriptTimerB >= 25001, 1)` -> plain `if` | c157 s5 |
| 6 | 4928 | `m_nScriptTimerA <= 9999` -> `__builtin_expect(..., 1)` | c157 s7 |
| 7 | 4972 | `m_nState = 9` -> `m_nState++` (c157 state 8) | c157 s8 |

(Edit 1 is the only lever outside the strict c154/c157 source range; it is a
semantically identical `m_nState++` in a state-1 arm.  The equivalent
c157 state-1 `m_nState = 2` -> `++` (line 4769) gives 57,224, not 57,232.)

Individual sizes on the -44 base (57,188): msg6w +24, s3inc +16, s1inc
(line 1816) +24, s8inc +4, s7timer1 -8.  The sum is 60; the composed layout
is 57,232 (+44).

## 3. Case-entry table (deliverable)

Original and landed — identical:

```
158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,157,40,
102,154,24,14,21,42,91,94,105,114,16
```

`casetable.py` (scratch) extracts the table by parsing the mission dispatch
compare chain + the two `tbh` tables and sorting the case-body addresses.
Baseline (wave 152) and the landed variant both match the original exactly.

## 4. Negative results (measured, not landed)

| variant | size | raw | align | gap | order |
|---|---:|---:|---:|---:|---|
| -44 pair alone | 57188 | 55471 | 17541 | 8 | ok |
| **landed (5 levers)** | **57232** | **55391** | **17401** | **8** | **ok** |
| + `expect(m_nState == 0, 1)` (c154 s0) | 57196 | 55351 | 17626 | 8 | ok |
| + `expect(messages[0]->isTriggered(), 1)` (c154 s0) | 57204 | 55522 | 17718 | **0x30e** | ok |
| + c157 s0 `m_nState = 1` -> `++` | 57220 | 55490 | 18715 | 8 | **reordered** |
| + msg7w (c154 s9 `messages[7]`) | 57188 | 55492 | 17550 | 8 | ok |
| + s5timer1 (c157 s5 timer) | 57196 | 55400 | 17543 | 8 | ok |
| + s7timer1 (c157 s7 timer) | 57180 | 55436 | 17487 | 8 | ok |
| + `expect(destroyed >= 9 && ...)` (c157 s5) | 57136 | 55307 | 17770 | 0x3a | ok |
| D+msg7w (msgtrig+s3+s1+s8+msg7) | 57232 | 55447 | 17582 | **0x31a** | ok |
| G+msg2+s7t | 57232 | 55336 | 17503 | 0x12 | ok |

Key facts:
* `msgtrig` (weighting the c154 state-0 `messages[0]->isTriggered()` test)
  moves the completion block away from the strong arm (gap 0x30e); it is the
  only inner-condition weight found that breaks the adjacency.
* `s0inc` (c157 state-0 `m_nState = 1` -> `++`) preserves the adjacency but
  **reorders the mission case entries** (swaps 135/145, 142/144, 92/125,
  40/154/157, 24/102, 14/21) — the same reorder w9805 saw from state-guard
  weights.
* The equivalent inverted form `expect(m_nScriptTimerA < 2001, 1)` is
  57,228 but is semantically wrong (it runs the completion when the timer is
  small); `>= 2001` with any semantically equivalent spelling
  (`2001 <= x`, `!(x < 2001)`, `x - 2001 >= 0`, `x > 2000`) is 57,188.
* `m_nState = m_nState + 1`, `++m_nState`, and a `LevelScript *self = this`
  alias all fold to the same shared `m_nState++` tail; none adds bytes.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9907/fdiff.py _ZN11LevelScript7processEi       # 55391 / 57232
python3 /dev/shm/w9907/align_diff.py _ZN11LevelScript7processEi # 17401
python3 /dev/shm/w9907/gap.py                                   # marker 0x145bcc strong 0x145bd4 gap 0x8
python3 /dev/shm/w9907/casetable.py                             # order matches original
cmake --build cmake-build-match --target check -j6              # RATCHET PASS
```

## 6. Open questions

1. The completion block is still 44 B smaller than the original's
   (392 vs 348 code bytes, excluding the original's 46 B literal pool): the
   original keeps `this` in `sl` (4-byte `ldr.w`/`str.w` for every member
   access) and inlines `m_nState++`; ours keeps `this` in `r4` (2-byte
   encodings) and tail-merges the increment.  A prologue shape that puts
   `this` in `sl` (the w9807 open question) would add the 44 B *inside* the
   block and make the adjacency byte-identical.
2. The original's case-154 state dispatch tests state 0 first
   (`cmp r0,#0; bne <tbh>`) and keeps the state-0 body inline; ours tests
   state 0 cold (`cmp r0,#0; beq <outlined body>`) then falls into the tbh.
   A source shape that makes state 0 hot without reordering the mission
   cases is still open.
