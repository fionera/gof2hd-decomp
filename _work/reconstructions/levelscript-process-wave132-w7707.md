# LevelScript::process — wave 132 / w7707 (pre-switch dispatch source-order sweep)

Status: **note only, no source change landed, 2026-11-07**. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `70e451ffe`.

## Result

```
                         before (master)   after (no code change)
raw ratio (difflib)      59.879643905      59.879643905
official verify pct      59.9              59.9
orig / ours insns        20169 / 20045     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7707/score.py`,
copied from w7689 and re-pointed at this worktree — note the w7689 copy
imported `common` from `/dev/shm/w7689` and silently scored *that* build).

## Hypothesis tested (w7689's open question)

w7689 decoded the original pre-switch chain as the *source* order
`0, 1, shared-tail, 5, 26, 50, 51, 81, 78, 89, 106, 95-group, main-switch`.
Our source emits `0, 1, 81, 78, 89, 106, 95-group, tail, 26, 5, 50, 51`.
The hypothesis: rewriting the source to the original order would move the
emitted branch layout closer and improve the match at 57232 B.

## Control-flow re-derivation (manual Thumb disasm of the original)

Following the *false* (else) edges of the comparison chain gives **two
separate chains**, not one:

```
chain A (else of the mission==1 check, 0x137d7e -> 0x1381b2):
    81 -> 78 -> 89 -> 106 -> 95-group -> shared tail (0x137f7c)
chain B (reached from the shared tail, 0x13812a):
    5 -> 26 -> 50 -> 51 -> main guard (0x13879e)
```

Evidence (each `bne` is the else edge of the preceding `cmp`):

```
137d7e  bne.w 0x49a   -> 1381b2  (mission 81)
1381ba  bne.w 0x614   -> 13832c  (mission 78)
138334  bne.w 0x85c   -> 138574  (mission 89)
13857c  bne.w 0xf62   -> 138c7a  (mission 106)
138c82  bne.w 0x112c  -> 138e44  (mission 95 group)
138ea0  bne.w 0x264   -> 137f7c  (shared tail)

137f82  beq.w 0x412   -> 13812a  (mission 5, when m_bStartSequence == 0)
138134  bne.w 0x5c4   -> 1382dc  (mission 26)
1382e4  bne.w 0x6cc   -> 1383e4  (mission 50)
1383ec  bne.w 0xa54   -> 13876c  (mission 51)
13877e  bne.n 0xa86   -> 13879e  (main guard)
```

So the original source is already the shape our source has:
`if (mission==0) ... else if (mission==1) ... else if (81) ... else if (78) ...
 else if (89) ... else if (106) ... else if (95g) ...;`
`if (m_bStartSequence) {...};`
`if (mission==5) ... else if (mission==26) ... else if (mission==50) ...
 else if (mission==51) ...;`
The **only** source-order difference is that our chain B tests 26 before 5;
the original tests **5 before 26**. w7689's list is the concatenation of the
two chains (5/26/50/51 then 81/78/89/106/95), not a single if/else chain.

## Measured variants (all built and scored; source restored after each)

Baseline: `59.879643905`, 57232 B, 20045 insns.

| variant | raw % | size | note |
|---|---:|---:|---|
| **baseline** (chain A then tail then 26,5,50,51) | **59.879644** | **57232** | current master |
| chain B order 26,50,51,5 (IJLK) | 59.879644 | 57232 | byte-identical (50/51 share the `messages` gate) |
| chain B order **5,26,50,51** (JIKL) | 59.800070 | 57232 | faithful; −0.0796 |
| chain B order 5,26,51,50 | 59.800070 | 57232 | same code as JIKL |
| chain B order 50,51,26,5 | 58.975953 | 57232 | |
| chain B order 26,50,5,51 | 58.040132 | 57248 | |
| user order: `[A,B]` + tail + `[5,26,50,51,81,78,89,106,95]` | 43.389324 | 57448 | +216 B |
| user order, tail after the whole chain | 39.650873 | 57264 | +32 B |
| w7689 shape: `[A..G]` then `else { tail; 5,26,50,51 }` | 52.656188 | 57340 | +108 B |
| w7689 shape, 5/26 swapped | 52.646240 | 57332 | +100 B |
| physical order (5,81,26,78,50,89,51,106,95) single chain | 42.880638 | 57396 | +164 B |
| physical order, tail between 1 and 5 | 42.765385 | 57392 | +160 B |
| physical order, 81 first | 42.295832 | 57440 | +208 B |
| chain A then tail then 5-chain as separate `if` (not `else if`) | 59.879644 | 57232 | byte-identical |
| prologue reorder `messages,player,enemies` | 58.527971 | 57416 | +184 B (known) |
| prologue reorder + 5-before-26 | 58.448374 | 57416 | +184 B |
| local `Status *status = Globals::status;` | 48.181047 | 57112 | −120 B |

The 24 permutations of chain B were swept exhaustively (with chain A fixed):
the current `26,5,50,51` is the unique best at 59.879644; `5,26,50,51` is
second at 59.800070.

## Why the "exact order" loses

The original's *emitted* comparison order is
`0,1,5,81,26,78,50,89,51,106,95` — the two chains are interleaved by
MachineBlockPlacement (A1,B1,A2,B2,A3,B3,A4,B4,B5). A single source chain in
that literal order does reproduce the exact comparison order (verified:
`0,1,5,81,26,78,50,89,51,106`), but it also moves the 81/78/89/106/95
*bodies* after the main switch, which is where the ~17-point fuzzy loss and
the +164..208 B come from. The current two-chain shape keeps those bodies
before the main switch and is the best layout the compiler will emit.

## Conclusion / next lead

The w7689 "source order" hypothesis is a misreading of the interleaved
*emitted* layout. The faithful minimal change is `5` before `26` in chain B
(size-exact, 59.800070 vs 59.879644); it is not landed because the fuzzy
score dips. The remaining root cause is unchanged: the prologue register map
(`messages` r5 vs r4, `enemies` r8 vs r4, `status` r4 vs r9) and the global
MBP permutation. A pass that can force `messages`→r5 / `enemies`→r8 without
the +184 B re-colour is the next real lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7707/score.py                         # 59.879643905
```

Scratch helpers in `/dev/shm/w7707/` (not tracked): `base.cpp`, `mkvar.py`,
`mkvar2.py`, `mkvar3.py`, `testvar*.py`, `search.py`, `score.py`,
`common.py`, `dis.py`, `disours.py`, `opcodes.py`, `orig_full.txt`.
