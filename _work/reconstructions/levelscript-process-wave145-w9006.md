# LevelScript::process — wave 145 / w9006: the genuine case-91 state-3
# `strd r1,r1,[r2,#144]` is the 64-bit `m_nScriptTimerA = 0`; the w8904
# raw-win + w8903 body-in-case-70 composition does not fit the current base

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `eb13ada9b` (wave-144
docs head). Scratch helpers in `/dev/shm/w9006/` (not tracked): `fdiff.py`,
`lsda.py`, `lsda_rows.py`, `lsda_cmp.py`, `relplt.py`, `h.py` (named-edit
build harness), `w143comp.py` (wave-143 composition harness).

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,909            54,906            (-3)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
process LSDA table       0x144             0x144             (EXACT)
whole .text diffbytes    85,994            85,991            (-3)
.ARM.extab diffbytes     1,370             1,370             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical, 29,392 B)
.note.gnu.build-id       20                19
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The landed change (1 line)

```cpp
// case 91, state 3 tail (line ~2996)
 for (int i = 0; i < 3; i++) { ... setCloakingPossible / cloak ... }
 m_nScriptTimerA = 0;
-m_pRadar->dockNavPtr = nullptr;
+m_nScriptTimerB = 0;
 m_nState++;
```

The original tail `0x13fed0..0x13fede` is

```
13fed0 ldr  r2,[sp,#88]        ; this
13fed2 movs r1,#0
13fed4 ldr  r0,[r2,#28]        ; m_nState
13fed6 strd r1,r1,[r2,#144]    ; m_nScriptTimerA = 0 (8-byte store)
13feda adds r0,#1
13fedc str  r0,[r2,#28]
13fede b.w  144e36
```

`m_nScriptTimerA` is a `long long` at `0x90`, so the `strd` is exactly the
64-bit `m_nScriptTimerA = 0`.  The w8905 landing had put the metric-only
`m_pRadar->dockNavPtr = nullptr` here as a size companion; replacing it with
the second timer reset restores the `strd` shape, keeps 57232 B, and is a
small raw win.  `m_nScriptTimerB` is at `0x98`; the compiler does not emit a
`#152` store (it is dead on this path), so the B reset acts as a **layout
companion** — it is not directly witnessed at this site.  The emitted case-91
tail is `strd r1,r1,[r2,#144]; adds; str [r2,#28]`, matching the original.

## The mission composition: measured, does not fit the current base

The requested composition is `w8904 raw-win alternative` (flat case-91 test +
`setActive` before `setName` + case-125 `setName` before `setActive`, no
`s2posfff`) + `w8905 genuine timer store` + `w8903 body-in-case-70 placement`.
On the current base (which already contains w8902's `break` + state-1-first,
w8904's nested case-91 and fff camera seats, and w8905's case-80 store removal)
the pieces do **not** compose to 57232 B / 0x144:

| variant | size | raw | LSDA table | `.rel.plt` |
|---|---:|---:|---:|---|
| **HEAD (nested, timer+dock)** | **57232** | **54,909** | **0x144** | identical |
| w8904 raw-win (`flat91+vecpos`) | 57232 | **54,676** | 0x146 | identical |
| `flat91` alone | 57236 | 54,810 | 0x146 | identical |
| `flat91+vecpos+nodock91` | 57216 | 55,212 | 0x146 | identical |
| w8903 body70 (`body70+anchor_rm`) | 57360 | 55,524 | 0x143 | 10 name diffs |
| `flat91+vecpos+body70+anchor_rm` | 57336 | 55,479 | 0x145 | 10 name diffs |
| `flat91+vecpos+nodock91+body70+anchor_rm` | 57320 | 55,253 | 0x145 | 10 name diffs |
| `body70+anchor_rm+nobreak70` | 57352 | 55,482 | 0x143 | identical |
| `body70+anchor_rm+nobreak70+swapback70` | 57352 | 55,485 | 0x144 | identical |
| `flat91+vecpos+body70+anchor_rm+nobreak70+swapback70` | 57320 | 55,132 | 0x146 | identical |
| **timerB91 (LANDED)** | **57232** | **54,906** | **0x144** | **identical** |
| `timerB91+flat91+vecpos` | 57232 | **54,673** | 0x146 | identical |
| `timerB91+body70+anchor_rm` | 57360 | 55,527 | 0x143 | 10 name diffs |
| `timerB91+flat91+vecpos+body70+anchor_rm` | 57336 | 55,479 | 0x145 | 10 name diffs |
| `swapback70` (METRIC-ONLY) | 57232 | **54,867** | 0x144 | identical |
| **`swapback70+timerB91` (METRIC-ONLY, best raw)** | **57232** | **54,864** | **0x144** | **identical** |
| `lockedEnemy_nodock91` | 57232 | 54,909 | 0x144 | identical |

### Why body70 is +128 B on the current base, not +4 B

On the wave-143 base `w8903` measured `v_c70a_plain_nodock` at 57236 B.  I
reproduced that exactly (`git show f5093474f:...` + body70 + case-80 dock
removal = **57268 -> 57236**), so the body70 edit itself is right.  The
current base differs in four ways that re-roll `MachineBlockPlacement`:
w8902's case-70 `break` + state-1-first, w8904's nested case-91 + fff camera
seats, and w8905's case-80 store removal.  On the current base the same body70
shape is **57360 B** (+128), and its real `setStanding` call lands after
`TargetFollowCamera::setRotationAroundTarget`, so `.rel.plt` has 10 name diffs
(the anchor removal is only PLT-safe in the wave-143 placement).  Reverting
w8902 (`nobreak70+swapback70`) still gives 57352 B.  The body70 placement is
therefore not landable on this base without a separate -128 B companion.

### Why flat case-91 cannot keep the 0x144 table

The 0x144/0x146 delta is structural, not just offset widths: the nested
`if (m_nState == 0) { if (msg->isTriggered()) ... }` creates a separate
landing-pad row.  In the flat form the `isTriggered()` call and the body calls
merge into one 5604-byte call-site region (rows 12-19 of the call-site table),
changing the row widths by +2 overall.  body70 does not move case-91 relative
to function start (the wormhole body is inserted before case 91 but the switch
start also moves earlier by the same amount), so it cannot fix the table.
`flat91+vecpos` is the best raw number found (54,676) but keeps 0x146.

## The faithful vs metric-only choice

`timerB91` is landed because it keeps the exact 0x144 table, the size, and
`.rel.plt`, and it restores the witnessed `strd` shape.  The B reset is a
layout companion (not witnessed at this site).  `swapback70+timerB91` is
**METRIC-ONLY**: it reverts w8902's faithful state-1-first case-70 dispatch
(the original is `cmp #2 / beq cold; cmp #1 / fall-through`, i.e. state-1
fall-through) for a further -42 raw (54,864).  `flat91+vecpos` is also
metric-only (nested test is witnessed at `0x13ac42`) and keeps the +2 table.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9006/fdiff.py _ZN11LevelScript7processEi   # 54,906
python3 /dev/shm/w9006/lsda.py cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 /dev/shm/w9006/relplt.py
```

## Open questions

* The `m_nScriptTimerB = 0` layout companion is dead in the emitted code; a
  genuinely witnessed +16 B statement elsewhere would let it be dropped.
* `flat91+vecpos` (54,676) is the best raw variant; it needs a source change
  that restores the nested landing-pad row without re-introducing the nested
  test, or a different block placement that moves the flat case-91 call sites
  below `0x4000`.
* body70 needs a -128 B companion on the current base (or a re-derivation of
  w8903's placement against w8902/w8904/w8905).
