# LevelScript::process — wave 161 / w10703: state-3 / hacking-block else-if control flow

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10703`, base master
`01c7f92d6` (wave 160).  Scratch `/dev/shm/w10703/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,048            55,104            (+56)
process aligned insns    17,049            17,055            (+6)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,518            85,574            (+56)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

Landed source change (case 125, `src/game/world/LevelScript.cpp` ~L3992):
the `m_nState >= 4 && m_nState <= 6` hacking block is moved **before** the
`m_nState == 3` block, and the state-3 block becomes its `else if`.  This is
the "move the hacking block" hypothesis from the wave-160 note.  It is a
size-exact structural variant; raw/aligned are slightly worse than HEAD, so
the coordinator may prefer to keep HEAD and use this note only.

## The control-flow problem

Original `0x13dcb2`:

```
13dcb2: ldr  r0,[sp,#88]     ; this
13dcb4: ldr  r0,[r0,#28]     ; m_nState
13dcb6: cmp  r0,#3
13dcb8: bne.w 13de44         ; state != 3 -> hacking check
13dcbc..13de3c: state-3 block
13de40: b.w  144e36          ; completion -> shared tail
13de44: subs r0,#4           ; hacking check (state 4..6)
13de46: cmp  r0,#2
13de48: bhi.w 144e36
13de4c..13df56: hacking block
13df56: b.w  144e36
```

So **both** exits of the state-3 block go to the shared tail; the hacking
block is reached only when `m_nState != 3`.  That is `else if` semantics.
HEAD has two independent `if`s, so the state-3 completion falls through into
the hacking check.

## Measured variants (all on wave-160 base, full link)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| HEAD (two independent `if`s) | 55,048 | 17,049 | 57,232 | baseline |
| pure `else if`, no expect | 55,457 | 17,062 | **57,216** | correct order, **-16 gate-fatal** |
| pure `else if` + `expect(state==3,1)` | 55,437 | 21,204 | 57,252 | hacking block moved cold |
| pure `else if` + `expect(state==3,0)` | 55,222 | 17,147 | 57,244 | state-3 block moved cold |
| pure `else if` + `expect(timer>=7001,0)` | 55,419 | 17,075 | 57,232 | completion moved cold |
| pure `else if` + `expect(timer,0)` + `expect(hack inner,1)` | 55,417 | 17,098 | 57,232 | completion cold |
| **hacking block first, `else if (state==3)`** | **55,104** | **17,055** | **57,232** | **landed** |
| ... + `expect(hack inner,1)` | 55,095 | 17,074 | 57,232 | inner hint, lint waiver needed |

The pure `else if` shape is instruction-for-instruction the original's
layout (A first, `b tail`, B inline, `b tail`), but the whole-function codegen
is 16 B short: the state-3 block is 4 B smaller than the original's and the
build emits 3 fewer PC-relative literal-pool loads (787 vs 790).  That gap is
a global register-allocation/pool-placement artifact, not a local source
difference.

## Landed block map (case 125)

```
13e88a: ldr.w r0,[fp,#28]     ; m_nState
13e88e: subs  r1,r0,#4
13e890: cmp   r1,#2
13e892: bhi.w 13e9a4          ; state not in 4..6 -> state-3 check
13e896..13e9a0: hacking block (inline) -> b.w 144e5e (tail)
13e9a4: cmp   r0,#3
13e9a6: bne.w 144e5e          ; state != 3 -> tail
13e9aa..: state-3 block (inline) -> b.w 144e5e (tail)
```

Both blocks are inline and both exits go to the shared tail, i.e. the
mutually-exclusive (else-if) semantics of the original.  The only structural
difference from the original is the **check order**: the original tests
`state == 3` first (`bne` to the hacking block); this variant tests
`state in 4..6` first (`bhi` to the state-3 block).  That reordering is what
buys back the 16 B.

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10703/fdiff.py _ZN11LevelScript7processEi        # 55,104 / 57,232
python3 /dev/shm/w10703/align_diff.py _ZN11LevelScript7processEi  # 17,055
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. The pure `else if` (original check order) is 16 B short.  Locating the
   three missing PC-relative pool loads / the 4 B state-3-block difference
   would let the faithful order land size-exact.
2. The `this`/canary callee-saved tie (`this` in `fp` vs original `sl`) still
   dominates the raw diff and is unchanged by this pass.
