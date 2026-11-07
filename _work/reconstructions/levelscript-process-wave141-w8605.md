# LevelScript::process — wave 141 / w8605: exact LSDA call-site table + case-125 shape sweep

Status: **note only, source left at master**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `e18e0c2a4` (wave-140 docs head).
Scratch helpers in `/dev/shm/w8605/` (not tracked): `fdiff.py`, `measure.py`,
`lsda3.py` / `tablediff.py` / `callseq2.py` (LSDA decode + row diff + invoke census),
`gen.py` (case-125 shape generator), `edit.py` (faithful-fix harness).

## Result

Baseline (master) is unchanged and the gate is green:

```
process raw diffbytes    54,138        (57232 / 57232, size-exact)
.ARM.extab               156,312       (size/base exact; process table 0x146)
byte_exact / linked      4507 / 4507
imports                  207
RATCHET                  PASS
```

Every case-125 source shape tried on the **current** base is *worse* than the
master `switch` shape. The best landable shape is the master shape itself
(raw 54,138, table 0x146). The wave-140 `s1first` win (raw 54,058) does **not**
reproduce on the current base: it gives raw 54,879 / 57,240 B / table 0x148
(measured on both the current base and the wave-139 base `d4857f174`, so the
difference is not the wave-140 source edits).

## 1. The exact original call-site table (reconstructed)

`.ARM.exidx` has one entry for the function: `0x1a12a0` (orig) → extab record
`0x1c2af8`. Decode:

| field | value |
|---|---|
| personality word | `0x7fe9c068` (personality `0x5eb60`) |
| unwind opcodes | `c9539702 f0800085 b0b0b0ab` |
| LPStart | `0xff` (omit) |
| TType encoding | `0xff` (cleanup only, no type table) |
| call-site encoding | `0x01` (ULEB128) |
| call-site table bytes | **`0x144` (324)** |
| rows | **46** (24 with a landing pad) |
| record length | `0x15c` (16 + 5 + 0x144, 4-aligned) |

Full 46-row table (offsets relative to `0x137d18`; `lp=0` rows are the gaps
between invokes):

```
row start  len   lp     row start  len   lp
 0  0000  16c8  0000    23  619a  046c  0000
 1  16c8  0006  df1c    24  6606  000c  df08
 2  16ce  02ec  0000    25  6612  0aea  0000
 3  19ba  0008  def8    26  70fc  0008  df14
 4  19c2  0660  0000    27  7104  0148  0000
 5  2022  0008  df12    28  724c  0008  dee8
 6  202a  0298  0000    29  7254  0228  0000
 7  22c2  0008  df10    30  747c  000a  df02
 8  22ca  02ce  0000    31  7486  1a02  0000
 9  2598  0008  def2    32  8e88  0004  df22
10  25a0  0136  0000    33  8e8c  0a42  0000
11  26d6  0006  df1a    34  98ce  0006  df00
12  26dc  08a6  0000    35  98d4  09c4  0000
13  2f82  0008  df0a    36  a298  0008  deec
14  2f8a  0806  0000    37  a2a0  0008  0000
15  3790  000e  df4a    38  a2a8  0008  deea
16  37a2  0008  df40    39  a2b0  15b4  0000
17  37aa  000c  df36    40  b864  0008  dee0
18  37b6  0008  df2c    41  b86c  09a6  0000
19  37be  1656  0000    42  c212  0010  deda
20  4e14  0006  defe    43  c222  1294  0000
21  4e1a  1378  0000    44  d4b6  0008  ded8
22  6192  0008  dee2    45  d4be  0ad2  0000
```

The 24 landing pads resolve to the same cleanup stubs as ours (delete r5 ×9,
delete r6 ×3, delete r4/r8/[sp+72]/fp ×1, String sp+112/220/232/124 ×1, and the
4-String sp+280→368→268→136 cascade) — see the wave-132 w7708 note.

## 2. Why the master table is 0x146 and the original is 0x144

Both tables have 46 rows and the same 24 invokes in the same *multiset*. The
2-byte delta is purely ULEB width: a start/length value needs 3 bytes once it
reaches `0x4000`.

* Original: the first `>= 0x4000` field is row 20 (`start = 0x4e14`). Rows
  0–19 are all 2-byte starts.
* Ours (master): the first `>= 0x4000` field is row 18 (`start = 0x495e`, the
  default case's `setName`). Rows 18 and 19 therefore each cost one extra byte
  (`0x146 = 0x144 + 2`).

To make the table exactly `0x144`, the default-case `setName` (currently at
rel `0x495e`, after the ~`0xf3c`-byte state-2 body) would have to move below
`0x4000`, i.e. the default case body would have to be laid out *before* the
state-2 body. That is a `MachineBlockPlacement` property, not a statement.

The entry size is `16 + 5 + table` rounded up to 4 bytes, so the process record
stays `0x15c` (and `.ARM.extab` stays 156,312) only while `table <= 0x147`.
`table = 0x148` already grows the record to `0x160` (+4), shifts `.rodata` by
`0x10`, and drops `byte_exact` from 4507 to ~4044. This is the hard constraint
on any case-125 shape.

## 3. The original case-125 dispatch (re-confirmed from Thumb)

```
139f9c  ldr r0,[sp,#88] ; ldr r1,[r0,#28] ; cmp r1,#0 ; bne.w 13b43e
139fba  state-0 body (inline) ... b.w 13b518
13b43e  cmp r1,#1 ; bne.n 13b518
13b444  state-1 body (inline) ... falls through
13b518  ldr r0,[sp,#88] ; ldr r0,[r0,#28] ; cmp r0,#2 ; bne.w 13dcb6
13b522  state-2 body (inline) ... b.w 144a0c
13dcb6  cmp r0,#3 ; bne.w 13de44
13dcbc  state-3 body
13de44  subs r0,#4 ; cmp r0,#2 ; bhi 144e36
13de4c  state 4..6 body
```

i.e. two chains: `if (s==0 && msg) {...} else if (s==1) {...}` then
`if (s==2) {...} else if (s==3) {...} else if (s>=4 && s<=6) {...}`.
The state-1 body is **inline** (rel `0x3726`), state-2 body inline
(rel `0x3800`), state-3 inline (rel `0x5f9e`), default inline (rel `0x612c`).

## 4. Case-125 shapes measured on the current base

`raw` = same-address diff bytes; `table` = LSDA table length; all size-exact
unless noted. Master = `switch (s) { case 1; case 2; case 3; default; }` after
the `if (s==0 && msg)` block.

| variant | size | raw | table | note |
|---|---:|---:|---:|---|
| **master (switch 1,2,3,d)** | **57232** | **54,138** | **0x146** | best landable |
| switch 1,3,2,d | 57232 | 54,138 | 0x146 | identical codegen |
| switch 2,1,3,d | 57232 | **54,089** | 0x14b | best raw, table grows |
| switch 2,3,1,d | 57232 | 54,089 | 0x14b | same |
| switch 3,1,2,d / 3,2,1,d | 57232 | 54,212 | 0x14b | |
| twochain (faithful two-chain) | 57248 | 54,850 | 0x148 | state-0 outlined |
| twochain_elseif (range merged) | 57248 | 54,850 | 0x148 | identical |
| s1first (state-1 first, else-if 0, switch 2,3,d) | 57240 | 54,879 | 0x148 | wave-140 shape |
| s1sep (separate ifs 1 and 0) | 57216 | 54,983 | 0x145 | |
| nested (`if(0){if(msg)} else if(1)`) | 57240 | 54,943 | 0x14b | |
| exp0 (`__builtin_expect(s==0&&msg,1)`) | 57244 | 55,121 | 0x14b | |
| chain_all (single else-if 0,1,2,3,d) | 57388 | 55,280 | 0x14d | |
| s0s1_sep (if 0; if 1; switch 2,3,d) | 57224 | 55,077 | 0x146 | |
| switch default-first (`d,1,2,3`) | 57180 | 55,194 | **0x144** | exact table, -52 B |
| switch 1,d,2,3 | 57152 | 54,934 | 0x146 | |
| switch 1,2,d,3 | 57312 | 55,229 | 0x145 | |

None of the `table <= 0x147` shapes beats master's raw 54,138. The only exact
`0x144` table (`default` first) costs 52 B of size and 1,056 raw bytes.

## 5. Faithful statement fixes (outside case 125) also hurt this base

The wave-139 `setCollide` receiver fix (`player->setCollide(false)` →
`m_pLevel->getPlayer()->setCollide(false)`, original `0x138d86..0x138d96`,
confirmed) is +24 B on the current base and raw 54,891. The faithful state-2
ambush-loop declaration order (`awake, pirate, playerPos, sideX, offsetY,
offsetZ, sideZ`, confirmed from `0x13b578..0x13b5c6`) is -16 B and raw 55,058.
Both shift the global block layout away from the original's addresses.

## 6. Conclusion / next lever

The case-125 statement sequence is already faithful; the residual diff is
global `MachineBlockPlacement`. The master `switch` shape is a strong local
optimum on the current base: every case-125 restructure moves the cold blocks
and increases the same-address diff. The next real lever is the **early
prologue deficit** (original first invoke at rel `0x16c8`, ours at `0x1622`,
`-0xa6`; by the state-0 `new Route` it is `-0x24a`), which is independent of
case 125. Matching that would shift the whole tail toward the original and
make the case-125 table comparison meaningful.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8605/measure.py                    # raw / size / table / extab
python3 /dev/shm/w8605/gen.py switch_213d            # a shape variant
python3 /dev/shm/w8605/tablediff.py                  # row-by-row LSDA diff
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```
