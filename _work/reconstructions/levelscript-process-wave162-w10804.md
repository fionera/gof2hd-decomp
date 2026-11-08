# LevelScript::process — wave 162 / w10804: mission-0 dispatch + state-3 else-if

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90), size-exact.
Worktree `wt/w10804`, base master `29248c1d0` (wave 161). Scratch `/dev/shm/w10804/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    55,013
process aligned insns    17,308
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          85,374
.ARM.extab diffbytes     725
.rel.plt                 0        (BYTE-IDENTICAL)
.rodata/.data/.bss/.ARM.exidx  byte-identical
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Landed: case-125 state-3 else-if (hacking block first)

`src/game/world/LevelScript.cpp` case 125 (~L3992): the two independent `if`s

```c++
if (__builtin_expect(m_nState == 3, 1)) { ...state-3... }
if (m_nState >= 4 && m_nState <= 6) { ...hacking... }
```

become a mutually-exclusive chain with the **hacking block first**:

```c++
if (m_nState >= 4 && m_nState <= 6) { ...hacking... }
else if (m_nState == 3) { ...state-3... }
```

This matches the original's `else if` semantics (both exits jump to the shared
tail; the hacking block is reached only when `state != 3`) while the check
order is `4..6` first (the original tests `state == 3` first, which is 16 B
short — see w10703).

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,013            55,016            (+3)
process aligned insns    17,308            17,066            (-242)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,374            85,377            (+3)
.ARM.extab diffbytes     725               723               (-2)
.rel.plt                 0                 0                 (BYTE-IDENTICAL)
```

`check` green, byte_exact 4507 / linked_exact 4507.

## Mission-0 dispatch: the 13-entry `tbh` (analysis, NOT landed)

The extra 13-entry table at `0x139064` is the mission-0 `m_nState` dispatch.
Baseline compare chain:

```
1389b0  cmp r1,#3 ; beq.w 13905c     ; state 3 body far
1389b6  cmp r1,#5 ; beq.w 138f30     ; state 5 body far
1389bc  cmp r1,#4 ; bne.w 13b134     ; state 4 falls through (inline)
...
13b134  subs r0,r1,#6 ; cmp r0,#9 ; bhi ; tbh   ; 10 entries 6..15
```

Wait — on the current base the chain is actually `cmp 5 / cmp 4 / 13-entry
tbh 3..15` (the state-3 arm is folded into the switch).  The original's
chain is **4, 3, 5**:

```
1389c8  cmp r1,#4 ; beq.w 138d2a     ; state 4 body far
1389ce  cmp r1,#3 ; bne.w 138fd8     ; state 3 falls through (inline)
1389d4  <state-3 body>
138fd8  cmp r1,#5 ; bne 139b1a       ; state 5 falls through (inline)
138fde  <state-5 body>
139b1a  subs r0,r1,#6 ; cmp r0,#9 ; bhi ; tbh   ; 10 entries 6..15
```

Swapping the state-3 arm before the state-5 arm (**4,3,5**) removes the extra
table (10-entry `subs #6; cmp #9` at `0x13b134`) and matches the original's
compare chain.  But on the **current** base (which contains w10702's
`data_`-guard drop and the case-4 8000 transition) the swap costs:

| variant | raw | aligned | size | tbh table | .rel.plt |
|---|---:|---:|---:|---|---:|
| HEAD (4,5,3) | 55,013 | 17,308 | 57,232 | 13-entry 3..15 | 0 |
| swap (4,3,5) | 55,508 | 17,978 | **57,240 (+8)** | 10-entry 6..15 | 10 |
| swap + pure else-if | **55,359** | 17,966 | **57,232** | 10-entry 6..15 | 10 |
| swap + hacking-first | 55,482 | 17,744 | **57,240 (+8)** | 10-entry 6..15 | 10 |
| pure else-if (orig order) | 55,459 | 17,308 | 57,216 (-16) | 13-entry | 0 |

On the wave-160 base the same swap was 55,059 / 57,232 (w10704); w10702's
change is what makes it +8 B and +449 raw on the current base.

**The pair `swap + pure else-if` is size-exact (57,232) and improves raw over
each component alone (55,359 vs 55,508 / 55,459) — it is the original's
`4,3,5` + `if(state==3) else if(4..6)` source shape.**  It is NOT landed
because it rotates five `.rel.plt` `R_ARM_JUMP_SLOT` entries (10 bytes):
`PlayerStation::setVisible` moves from after `startSmokeEmission` to before
`switchPlanetForIntro`, because the swap pushes the mission-0 state-9 block
later than the case-42 state-7 block.  The original has
`setVisible` last, and HEAD reproduces it.

## Other measured shapes (all fatal unless noted)

| variant | raw | aligned | size | .rel.plt |
|---|---:|---:|---:|---:|
| mission-0 `expect(state==3,1)` on 4,5,3 | 55,372 | 18,612 | 57,508 (+276) | 30 |
| mission-0 `expect(state==3,0)` on 4,5,3 | 55,038 | 18,915 | 57,256 (+24) | 0 |
| pure else-if + `expect(state==3,0)` | **54,998** | 18,696 | 57,252 (+20) | 0 |
| swap + `expect(state==4,0)` | 55,543 | 18,864 | 57,248 (+16) | 18 |
| swap + `expect(state==5,1)` | 55,279 | 19,505 | 57,280 (+48) | 13 |
| perm 3,5,4 + hacking-first | 55,450 | **16,936** | 57,224 (-8) | 7 |

The `pure else-if + expect(state==3,0)` shape is the only one that beats the
baseline raw (54,998, -15) but it is +20 B (gate-fatal).

## Reproduce

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10804/fdiff.py _ZN11LevelScript7processEi        # 55,016 / 57,232
python3 _work/reconstructions/align_diff.py _ZN11LevelScript7processEi  # 17,066
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. The mission-0 `4,3,5` swap + pure else-if is size-exact and structurally
   the original, but rotates `.rel.plt` (10 B).  Fixing it needs the mission-0
   state-9 block placed before the case-42 state-7 block (the original's PGO
   layout), which the current non-PGO build does not reproduce.
2. `pure else-if + expect(state==3,0)` is raw -15 but +20 B; shaving 20 B
   would give the first raw win on this function in several waves.
