# LevelScript::process — wave 115 / w6304 (LSDA / campaign-mission case order, angle B)

Status: **code change landed, 2026-11-05**. Branch `wave115-w6304`, base master
`b847d5d46` (wave-114 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.020546         58.034026      (+0.013480)
orig / ours insns        20169 / 20034     20169 / 20035
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.text                    0x1337c8          0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26298           0x26298
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6304/fast.py`). All 27 section headers, the exact file size and
`.ARM.extab` are unchanged.

## The real campaign-mission case order (jump-table reconstruction)

The mission switch is a linear compare chain plus two `tbh` range tables. Decoding
each target in the original gives the case-entry layout order:

```
158,
131, 64, 135, 145, 142, 144, 139, 69, 73, 67, 70,   (the two tbh ranges)
29, 41, 80, 56, 125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94,
105, 114, 16
```

Details (original addresses):

* linear chain at `0x1387d8..0x13885e`:
  14→`0x13a9b2`, 16→`0x13b014`, 21→`0x13ab24`, 24→`0x13a930`, 29→`0x139ca2`,
  40→`0x13a4d8`, 41→`0x139d56`, 42→`0x13abaa`, 56→`0x139f14`, 80→`0x139e34`,
  91→`0x13ac42`, 92→`0x139ffe`, 94→`0x13acac`, 102→`0x13a658`, 105→`0x13aea4`,
  114→`0x13afde`, 125→`0x139f9c`, 154→`0x13a76e`, 157→`0x13a308`, 158→`0x138862`.
* `tbh` for 131..145 at `0x138a8a` (PC `0x138a8e`): 131→`0x138aac`,
  135→`0x1391c0`, 139→`0x13965c`, 142→`0x139544`, 144→`0x1395e0`,
  145→`0x1392f6`.
* `tbh` for 64..73 at `0x138c3e` (PC `0x138c42`): 64→`0x138c56`,
  67→`0x139892`, 69→`0x139720`, 70→`0x1399a2`, 73→`0x139834`.

Our build already produces **exactly this layout order**: the same compare chain
targets and the same `tbh` entry order are present in `cmake-build-match/libgof2hdaa.so`
(chain targets 29→`0x139a40`, 41→`0x139af4`, 80→`0x139bee`, 56→`0x139cda`,
125→`0x139d20`, 92→`0x13a128`, 157→`0x13a402`, 40→`0x13a5d0`, 102→`0x13a654`,
154→`0x13a6da`, 24→`0x13a7c2`, 14→`0x13a846`, 21→`0x139a88`… i.e. the same
relative order). So the source case order is **not** the remaining difference:
clang's `MachineBlockPlacement` reproduces the original order from the current
ascending source order.

Measured: reordering the 31 source `case` blocks to the layout order above gives
**57.668101 % at 57224 B** (8 B smaller) and an *identical* 46-row LSDA — i.e. the
source order is layout-neutral here. Ascending source order stays best.

## The 46-row LSDA

Both binaries have a 46-row call-site table (`LPStart`/`TType` encodings both `0xff`,
call-site encoding `uleb128`, all actions 0). The row *pattern* (which rows carry a
landing pad) is identical except for the placement of the case-125 four-`String`
loop (source line 3908):

* original rows 15–18: `String sp+280, sp+368, sp+268, sp+136` at code `0x3790..`
  (the loop is inside case 125's `m_nState == 1` arm at `0x13b43e`, immediately
  before the `m_nState == 2` check at `0x13b518`);
* our rows 9–12: the same four `String` cleanups at code `0x20da..` (our
  `m_nState == 1` arm is at `0x139d7c`, right after the `m_nState == 0` arm).

Consequently the original's case-91 `String` temp is `sp+232` and case-92's is
`sp+220`; ours are swapped (`sp+220` / `sp+232`). The table length is 324 B
(original) vs 326 B (ours); the 2-byte delta is entirely ULEB width at rows
10/12/13/14/15/17/18/19 (row 18/19 starts cross `0x4000` in ours: `0x4b02`
3-byte ULEB vs original `0x37b6` 2-byte ULEB). The `.ARM.extab` record is 348 B
in both (the LSDA payload is padded to 4-byte alignment), so the section stays
size-exact.

## Variants measured (all size-exact unless noted)

| variant | % | size |
|---|---:|---:|
| master baseline | 58.020546 | 57232 |
| **landed: case-92 state-4 decl/statement order** | **58.034026** | **57232** |
| + case-92 `setHitpoints`/`m_nState=6` swap | 58.089387 | 57236 (+4) |
| + that swap and `setPosition`/`setHitpoints` order | 58.084413 | 57236 |
| full case reorder to original layout order | 57.668101 | 57224 |
| case 91/92 source swap | 58.000647 | 57232 |
| case-125 `m_nState==1` as `else if` (faithful skip semantics) | 56.535257 | 57248 |
| case-125 nested `if (m_nState==0) { if (msg) … }` | 56.626866 | 57240 |
| case-91 nested `if (m_nState==0) { if (msg) … }` | 56.624617 | 57204 |
| reverse layout order | 57.833603 | 57232 |
| 158 first, then ascending | 57.923932 | 57228 |

The best measured variant overall is **58.089387 %** (57236 B), but it grows the
function by 4 B, which would break size-exact / the exact 2,187,328 B file size, so
the size-neutral 58.034026 % variant was landed.

## What was applied (case 92, `m_nState == 4`, source ~line 2978)

Two adjacent-declaration/statement reorders, both semantically independent:

```c++
m_nScriptTimerA += delta;
AEGeometry *leader = (*enemies)[0]->geometry;
m_nScriptTimerB = (long long) ((float) delta * 0.05f + (float) m_nScriptTimerB);
m_pCamera->setTarget(leader);
AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
```

(master had `m_nScriptTimerB` first and `canvas` before `setTarget`.) The reorder
moves the following `CameraGetLocal`/`MatrixGetPosition` block's register
allocation and literal-pool placement toward the original's; the original's
`CameraGetLocal` block at `0x13c8fc` starts with the `Globals::Canvas` load and
then calls `CameraGetCurrent`/`CameraGetLocal`/`MatrixGetPosition`, and the
`leader` load / timer update sit in the preceding block. Size stays exactly
57232 B.

## Proven vs hypothesis

Proven (measured, deterministic):

* The original's campaign-mission case-entry order is the 31-value list above,
  reconstructed from the compare chain and the two `tbh` tables.
* Our build already emits that same layout order; source-level case reordering is
  layout-neutral and lowers the raw ratio.
* The landed two-swap variant is size-exact and raises the raw ratio by +0.013480;
  the gate is PASS with byte 4500 / linked 4500 / lint CLEAN / drift 0.

Hypothesis / not established:

* The remaining LSDA row-order difference (case-125 four-`String` block, case-91/92
  `String` stack slots) is `MachineBlockPlacement` reacting to the reconstructed
  body shapes, not to case order. Matching it needs the case-125 `m_nState == 1`
  arm to be laid out adjacent to the `m_nState == 2` check (`0x13b43e` vs `0x13b518`
  in the original); the faithful `else if` form that would skip the state-1 arm
  after the state-0 arm sets `m_nState = 1` scores 56.5 % and is +16 B, so it was
  not landed.
* The 2-byte LSDA length delta is a pure ULEB-width artifact of the case-91
  `String` call site sitting above `0x4000` in our layout.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

## Gate at the end of this pass

```
byte_exact 4500  linked_exact 4500  size-exact 4524/4524  data_exact 843/843
imports 207  lint CLEAN  drift 0  RATCHET PASS
sections 27  file size 2187328  .ARM.extab 0x26298
```
