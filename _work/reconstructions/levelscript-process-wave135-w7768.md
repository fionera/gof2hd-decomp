# LevelScript::process — wave 135 / w7768 (case-125 state switch; case-40 cold-chain probe)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bbffef482`.

## Result

```
                         before (master)   after (landed)
official verify pct      58.2              58.7
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        20169 / 20050     20169 / 20047
process .text diffbytes  54359             54384   (+25, offset-wise)
whole .text diffbytes    85022             85047   (+25)
.ARM.extab diffbytes     1416              1403    (-13)
.ARM.exidx               exact             exact
.rel.plt                 exact             exact   (byte-identical)
.rodata/.data/.bss/.got  exact             exact
.dynsym/.dynstr/.hash    exact             exact
.text size               1259464           1259464 (exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The official fuzzy percentage rises 58.2 -> 58.7 while the hard gate and every
section size stay exact.  The offset-wise `.text` differing-byte count rises by
25; that is the same block-placement trade-off recorded in waves 129/132/134 (a
faithful reorder improves the sequence match but shifts bytes relative to the
original's fixed offsets).

## New hypothesis tested

The handoff asked for two things: (a) a `switch (m_nState)` for case 125's
state chain, and (b) the case-40 cold-chain placement.

### (a) case 125 — the state chain really is a compare-chain switch

The original's case-125 dispatch (`0x139f9c`, `0x13b43e`, `0x13b518`,
`0x13dcb6`, `0x13de44`) is a **linear compare chain**, not a `tbh` table:

```
139fa0  cmp r1,#0 ; bne.w 13b43e        ; state 0 (hot)
13b440  cmp r1,#1 ; bne.n 13b518        ; state 1
13b51c  cmp r0,#2 ; bne.w 13dcb6        ; state 2
13dcb6  cmp r0,#3 ; bne.w 13de44        ; state 3
13de44  subs r0,#4 ; cmp r0,#2 ; bhi tail   ; states 4..6
```

That is exactly clang 7.0.2's lowering of `switch (m_nState)` with cases
`1,2,3` plus a `default` range check — **not** a dense `case 4: case 5: case 6:`
switch (which lowers to a `tbh` jump table, measured at 57380 B / 43.5 %).
Replacing the three `if`/`else if` tests with

```cpp
switch (m_nState) {
case 1: { ... } break;
case 2: { ... } break;
case 3: { ... } break;
default: { if (m_nState >= 4 && m_nState <= 6 && ...) { ... } break; }
}
```

produces the original's compare-chain shape and measures **57248 B, pct 58.5**
(+16 B over master).  It is semantically identical to the old `if` chain for
every state value (state 0 falls into `case 1`, exactly as before).

The +16 B is paid back by dropping the two `particleRenderBoolPtr->reset()` /
`particleEmitBoolPtr->reset()` calls from the fake `if (delta < 0)` first-
reference anchor in `case 3`.  Those two calls are **not** in the original's
case-125 state-3 body (the original body starts at `0x13dcbc` with the
`moveForward` loop); they were only there to pad the function to 57232 B.  With
the switch providing +16 B, removing them lands at exactly **57232 B**, and
`.rel.plt` is still byte-identical (the remaining five anchor calls still emit
the required first references in order).

### (b) case 40 — cold-chain mirror: measured, no win

The original's case-40 state-0 body is **inline** (`0x13a4d2..0x13a61e`,
`cmp r0,#0; bne.w 13e6b8` hot polarity) and its state-1..4 chain is far
(`0x13e6b8`).  Ours is the mirror image: state-0 body far (`0x13c384`,
`cmp r0,#0; beq.w` cold polarity) and state-1 inline (`0x13a602`).

| variant | size | pct |
|---|---:|---:|
| nested `if (m_nState == 0) { if (msg) ... }` (flips polarity to `bne`) | 57232 | 58.1 |
| `switch (m_nState)` 0..1 + `if`/`else if` 2..4 | 57252 | 58.0 |
| whole-chain switch 0..4 | build/brace probe | — |

The nested shape does reproduce the original's hot polarity (the state-0 body
becomes the fallthrough), but it is size-neutral and scores 58.1, i.e. the
global re-roll costs more than the local fix gains.  No size-neutral case-40
variant beat master, so case 40 is left untouched this pass.

## Full experiment table (all measured on pristine master)

| variant | size | pct |
|---|---:|---:|
| **master** | **57232** | **58.2** |
| case-125 `else if` state 1 (faithful CFG) | 57256 | 56.5 |
| case-125 whole switch 0..6 | 57416 | 43.5 |
| case-125 switch 2..6 (state 1 kept as `if`) | 57268 | 35.5 |
| case-125 switch 1..6, explicit cases 4/5/6 (`tbh`) | 57380 | 43.5 |
| case-125 switch 0..1 + `if` 2..6 | 57248 | 56.5 |
| case-125 switch 1..6 + `default` 4..6 | 57248 | 58.5 |
| case-125 switch 1..3 + `default` 4..6 | 57248 | 58.5 |
| **switch 1..3 + `default`, minus 2 particle resets (landed)** | **57232** | **58.7** |
| landed + case-154 state-2 dir fix | 57232 | 58.6 |
| landed + case-70 else-if chain | 57224 | 58.3 |
| case-40 nested state-0 | 57232 | 58.1 |
| case-40 switch 0..1 + `if` 2..4 | 57252 | 58.0 |
| case-154 state-2 dir fix alone | 57232 | 58.1 |
| case-70 else-if alone | 57232 | 58.0 |

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w7768/score.py                          # pct 58.7
python3 /dev/shm/w7768/bytediff.py                       # process 54384
python3 /dev/shm/w7768/secdiff.py                        # .text 85047, extab 1403, rel.plt 0
```

Scratch helpers in `/dev/shm/w7768/` (not tracked): `harness.py`, `score.py`,
`bytediff.py`, `secdiff.py`, `opcodes.py`, `opaddr.py`, `exp1.py`..`exp13.py`,
`final.py`, `base.cpp`.
