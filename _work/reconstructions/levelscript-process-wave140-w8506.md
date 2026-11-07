# LevelScript::process — wave 140 / w8506 (case-125 state 0..6: if/else-if shapes vs the LSDA table)

Status: **note only, source reverted to master**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `d4857f174` (wave-139 docs head).
Scratch helpers in `/dev/shm/w8506/` (not tracked): `fdiff.py` (raw same-address diff),
`gen.py` (case-125 variant generator), `sweep2.sh` (build + raw diff + LSDA table length),
`lsda3.py` (ARM EHABI LSDA parser), `cmp_lsda.py`, `tdist.py` (LSDA-table distance to original).

## Result

Baseline (master) is unchanged and the gate is green:

```
process raw diffbytes    54,262       (57232 / 57232, size-exact)
.ARM.extab               0x26298      (size exact; LSDA table 0x146)
byte_exact / linked      4507 / 4507
```

The new hypothesis produced a **size-exact, raw-diff-improving** case-125 shape
(`s1first_cases_r2`, raw 54,058, **-204**) and a **layout-improving** shape
(`nested_r2`, raw 54,124, -138; LSDA-table distance 85,910 vs baseline 183,586),
but neither can be landed: both grow the process LSDA call-site table from
0x146 to 0x148/0x14b, which grows `.ARM.extab` by 4, shifts `.rodata` by 0x10
(0x1c9e10 -> 0x1c9e20), and drops `byte_exact` from 4507 to 4044 (all literal
pools that point into `.rodata` change). The gate is therefore **not green** for
either variant, so the source is left at master and the variants are recorded here.

## The structural finding

The original case-125 dispatch is **not a switch**. It is an if/else-if chain
(Thumb, original base 0x137d18):

```
139f9c  ldr r0,[sp,#88] ; ldr r1,[r0,#28] ; cmp r1,#0 ; bne.w 13b43e
139fba  state-0 body inline  ...  b.w 13b518
13b43e  cmp r1,#1 ; bne.n 13b518
13b444  state-1 body inline  ...  falls through
13b518  cmp r0,#2 ; bne.w 13dcb6
13b522  state-2 body inline  ...  b.w 144a0c
13dcb6  cmp r0,#3 ; bne.w 13de44
13dcbc  state-3 body inline
13de44  subs r0,#4 ; cmp r0,#2 ; bhi ...
13de4c  state 4..6 body
```

i.e. `if (state==0 && msg) {...} else if (state==1) {...}` followed by
`if (state==2) {...} else if (state==3) {...} else if (4..6) {...}`.
Ours is a `switch (m_nState)` whose compare chain is emitted **3,2,1 descending**
with the state-1 body as the fallthrough; the original emits **1,2,3 ascending**
with each test adjacent to its body.

## Variants measured (all on the wave-139 base, size-exact unless noted)

`raw` = same-address diff bytes for `process`; `table` = LSDA call-site table
length (master 0x146, original 0x144; `<= 0x147` keeps `.ARM.extab` at 0x26298).

| variant | size | raw | table | note |
|---|---:|---:|---:|---|
| master (`switch`, cases 1,2,3) | 57232 | 54,262 | 0x146 | compare chain 3,2,1 |
| `switch` cases 1,3,2 / 2,1,3 / 3,1,2 / 3,2,1 | 57232 | 54,262 / 54,215 / 54,337 / 54,337 | 0x146 / 0x14b / 0x14b / 0x14b | two outcomes only |
| `switch` with `case 0:` (msg inside) | 57208 | 54,830 | 0x14b | -24 |
| `if(0){if(msg)}` + `switch(1,2,3)` | 57232 | 54,262 | 0x146 | byte-identical to master |
| `twochain` (faithful if/else-if) | 57248 | 54,815 | 0x148 | state-0 body outlined |
| `nested` (`if(0){if(msg)} else if(1)`) | 57240 | 54,750 | 0x14b | state-0 body inline, state-1 test first |
| **`nested_r2`** = `nested` + `player->geometry->setDirection(-dir,...)` | **57232** | **54,124** | 0x14b | best table distance (85,910) |
| `twochain_exp0` (`__builtin_expect(state==0,1)`) | 57232 | 54,300 | 0x14b | state-0 body inline, state-1 test outlined |
| **`s1first_cases_r2`** = state-1 test/body first, state-0 in the `else if`, then `switch(2,3,default)` + R2 | **57232** | **54,058** | 0x148 | best raw diff (-204) |
| `twochain_s1first_r2` (same, if/else-if instead of switch) | 57232 | 54,058 | 0x148 | identical codegen |
| `s1first_2last_r2` (state-2 checked last) | 57204 | 54,949 | 0x148 | -28 |
| `nested_loop_r2` (faithful ambush-loop declaration order) | 57216 | 55,071 | 0x14b | -16, worse raw |
| `nested_cold2_r2` (`__builtin_expect(state==2,0)`) | 57176 | 55,319 | 0x14b | -56 |
| `twochain_hot2` (`__builtin_expect(state==2,1)`) | 57212 | 55,319 | 0x148 | -20 |

`R2` is the already-known faithful receiver fix at source line 3855
(`m_pLevel->getPlayer()->geometry->setDirection(-dir, ...)` ->
`player->geometry->setDirection(-dir, ...)`, -8 B; original `ldr r4,[r4,#8]`
with `r4 = player`). It is required to keep `nested`/`s1first` at 57232 B.

### `s1first_cases_r2` source shape (best raw diff)

```cpp
case 125: {
    if (m_nState == 1) {
        ... state 1 body ...
    } else if (m_nState == 0 && ((RadioMessage *) ((*messages)[1]))->isTriggered()) {
        ... state 0 body ...
    }
    switch (m_nState) {
    case 2: { ... } break;
    case 3: { ... } break;
    default: { ... } break;
    }
    break;
}
```

This puts the state-1 body inline at ~0x2300 (matching the original's state-1
body placement), keeps the state-0 body as the `else if`, and leaves the
state-2/3/default switch lowering identical to master. Raw diff 54,058
(-204 vs master). LSDA table 0x148 (needs 0x147).

### `nested_r2` source shape (best block-layout distance)

```cpp
case 125: {
    if (m_nState == 0) {
        if (((RadioMessage *) ((*messages)[1]))->isTriggered()) {
            ... state 0 body ...
        }
    } else if (m_nState == 1) {
        ... state 1 body ...
    }
    if (m_nState == 2) { ... } else if (m_nState == 3) { ... } else if (4..6) { ... }
    break;
}
```

This keeps the state-0 body inline (like the original) and moves the state-1
test+body out of line; the LSDA-table distance to the original drops from
183,586 to 85,910. Raw diff 54,124 (-138). LSDA table 0x14b (needs 0x147).

## Why the LSDA table blocks the landing (the key new result)

The process LSDA entry is at `.ARM.extab` 0x1c2af8 (personality 0x5eb60,
generic model). Layout: 4-byte personality + 12-byte unwind opcodes + 5-byte
LSDA header (`ff ff 01 c4 02`) + call-site table + pad. The entry is
`0x15 + tablelen`, rounded up to 4 bytes, so the entry stays **0x15c** only if
`tablelen <= 0x147`. Master is 0x146 (1 byte spare); the original is 0x144.

The table is 46 entries of `(uleb start, uleb length, uleb landing-pad, uleb action)`
relative to the function start. Any case-125 layout that inlines the huge
state-2 body (~0x2400 B) pushes the following call-site starts past 0x4000,
turning five 2-byte ulebs into 3-byte ulebs: +5 bytes (0x14b). The
`s1first` family pushes only two entries past 0x4000: +2 bytes (0x148).
A 0x148/0x14b table makes the entry 0x160, `.ARM.extab` 0x2629c (+4),
`.rodata` 0x1c9e20 (+0x10), and every function whose literal pool points into
`.rodata` stops being byte-exact: `byte_exact 4507 -> 4044`, `RATCHET FAIL`.

Per-entry uleb-cost diff (s1first vs original): entries 12/14/16/18/19 are one
byte larger, entry 15 one byte smaller; net +4 vs original, +2 vs master.

## Next steps (new, not yet tried)

1. **Shave 1 table byte from `s1first_cases_r2`.** Its entry 16 start is 0x4906
   (3-byte uleb); bringing it below 0x4000 (or making one of the 3-byte
   landing pads 2-byte) gives table 0x147 and a green gate at raw 54,058.
   The state-2 body must start ~0x900 earlier or be outlined; the `switch`
   form already outlines it, so the lever is the code between the state-1 body
   and the state-2 test.
2. **Shave 4 table bytes from `nested_r2`.** Its entries 13-19 sit at
   0x4886..0x4d62; the state-1 body is outlined there. Moving the state-1 body
   inline (before the state-2 body) would put its String call sites at ~0x2500
   and drop the table to ~0x146, but every shape tried so far either outlines
   state-0 or state-1, never both inline.
3. **Match the original's early layout first.** The original's first table entry
   is `0..0x16c8`; ours is `0..0x1622` (-0xa6). That 166-byte prologue/early
   deficit is independent of case 125 and is the next global lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
python3 /dev/shm/w8506/gen.py s1first_cases_r2   # or nested_r2
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8506/fdiff.py _ZN11LevelScript7processEi
python3 /dev/shm/w8506/lsda3.py cmake-build-match/libgof2hdaa.so | head -1
```
