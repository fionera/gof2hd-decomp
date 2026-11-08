# LevelScript::process — wave 143 / w8803: case-91 chain order (state 6 before 5)
# lands at 57232 B; the case-91 nested test is -12 B with no +12 companion on this base

Status: **code change landed**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `bf083ab41` (wave-142
docs head). Scratch helpers in `/dev/shm/w8803/` (not tracked): `fdiff.py`,
`measure.py` (raw + LSDA table), `trymod.py` (named-edit harness), `sweep.py` /
`sweep2.py` (subset sweeps).

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,691            54,594            (-97)
whole .text diffbytes    85,119            85,022            (-97)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,395             1,395             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0 (byte-identical) 0 (byte-identical, 29,392 B)
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## The landed change: case-91 state 5/6 chain order

The original case-91 else-if chain is not source order 1,2,3,4,5,6. Thumb at
`0x13f316..0x13f37a`:

```
13f316  cmp  r0, #4
13f318  beq.n 13f350          ; state 4 body
13f31a  cmp  r0, #3
13f31c  bne.n 13f36e
13f31e  ... state 3 body ...
13f36e  cmp  r0, #6
13f370  beq.w 141002          ; state 6 body
13f378  cmp  r0, #5
13f37a  bne.w 1410e2
13f37e  ... state 5 body ...
```

i.e. the compare chain is **1, 2, 4, 3, 6, 5**: state 6 is tested *before*
state 5. The source had state 5 before state 6. Swapping the two `else if`
blocks is a genuine statement-order fix (it matches the original's witnessed
compare order) and is **size-neutral**:

* `process` raw same-address diff **54,691 -> 54,594 (-97)**;
* `.text` **85,119 -> 85,022 (-97)**;
* `process` stays exactly 57232 B and the LSDA call-site table stays `0x146`
  (the `.ARM.extab` record is unchanged);
* `.rel.plt` is byte-identical (verified with `readelf -rW`; the rotation seen
  in an intermediate build was from a different variant, not this one).

The swap does not change the call multiset, so the 207-import ratchet is
untouched.

## The case-91 nested test: measured on the current base

w8702's lead was that writing the state-0 body as

```cpp
if (m_nState == 0) {
    if (msg3->isTriggered()) { ... state 0 ... }
} else if (m_nState == 1 && docked) { ... }
```

forces the state-0 body inline (as in the original `0x13ac42`) and shrinks the
process LSDA call-site table. On the **current** base (after the wave-142
merge fix restored `case-144 setStation`):

| variant | size | raw | LSDA table |
|---|---:|---:|---:|
| master (flat `state==0 && msg`) | 57232 | **54,691** | 0x146 |
| nested case-91 | 57220 (-12) | 55,151 | **0x144** |
| nested + `setActive` before `setName` | 57220 | 55,150 | 0x144 |
| nested + state-1 split into nested `if` | 57220 | 55,149 | 0x144 |
| nested + `else { chain }` | 57220 | 55,151 | 0x144 |
| nested + state-1 as a separate `if` | 57224 | 55,272 | 0x144 |
| nested + `__builtin_expect(state==0, 0)` | **57232** | 55,257 | 0x144 |
| nested + `__builtin_expect(state==0, 1)` | 57252 | 55,500 | 0x144 |

The wave-142 note's "nested raw 54,169" does **not** reproduce on this base:
the merge fix's restored `setStation` re-rolled `MachineBlockPlacement`, and
the nested form is now **+460** raw versus master, not +61.

## The +12 B companion search

The nested form is -12 B, so it needs a genuine +12 B companion. Measured
deltas on the nested base (all built + linked):

| edit | size | raw |
|---|---:|---:|
| nested alone | 57220 | 55,151 |
| `move4b` (mission-0 state-4/5 teardown move) | 57228 (+8) | **54,778** |
| `case157vis` (`m_pGeometry2->setVisible(false)`) | 57216 (-4) | 55,142 |
| `nonull` (drop two `ship != nullptr` checks) | 57256 (+36) | 55,370 |
| missing `m_pCamera->translate(-10000,5000,25000)` | 57252 (+32) | 55,189 |
| `s5gp` (case-91 state-5 fresh `getPlayer()->setCollide`) | 57220 (0) | 55,151 |
| `move4b` + `case157vis` | 57224 | 55,090 |
| `move4b` + `case157vis` + `s5gp` | **57232** | 55,253 |
| nested + `swap56` | 57228 | 54,815 |
| nested + `swap56` + `move4b` + `case157vis` | **57232** | 55,114 |

No nested 57232 B combination beats master's raw 54,691: the best is
`nested + swap56 + move4b + case157vis` at 55,114. The nested form is therefore
**not landed**; the table `0x144` remains a lead for a future pass that also
matches the surrounding block placement.

## Why the flat `swap56` is the right landing

`swap56` alone (flat case-91 form) is the unique size-exact, `.rel.plt`-safe
variant found that improves the raw metric: 54,691 -> 54,594. It is a faithful
statement-order fix (the original's compare chain tests state 6 before state 5)
and leaves the LSDA table at `0x146`, so `.ARM.extab` stays exactly 156,312 B.
The remaining table `+2` is the known row-alignment difference (ours rows 18/19
start above `0x4000` because the case-125 `setName` block is emitted after the
case-91 block); it is a block-placement property, not a statement.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8803/measure.py _ZN11LevelScript7processEi   # raw 54,594
python3 /dev/shm/w8803/trymod.py swap56                        # re-apply the edit
readelf -rW cmake-build-match/libgof2hdaa.so | \
  awk '$3=="R_ARM_JUMP_SLOT"{print $5}' | diff - /dev/shm/w8803/relplt_orig.txt
```
