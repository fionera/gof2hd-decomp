# LevelScript::process — wave 139 / w8408 (LSDA call-site table reconstructed; faithful
# case-125 two-chain landed at 57232 B with .ARM.extab unchanged via the case-91 nested test)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bf123bd47` (wave-138 docs).
Scratch helpers in `/dev/shm/w8408/` (not tracked): `lsda.py`, `lsdadump.py`,
`widths.py`, `w2.py`, `mapentries.py`, `tbl.py`, `mkvariant.py`, `gen.py`,
`gen2.py`, `measure.sh`, `fdiff.py`.

## Result

```
                         before (master)   after (landed)
official verify pct      57.4              57.4
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        55,072            55,256           (+184)
whole .text diffbytes    85,476            85,476?          (same size; content shifts)
.ARM.extab size          156,312           156,312          (exact)
.ARM.extab table         0x146             0x146            (fits <= 0x147)
.ARM.exidx               exact             exact
.rel.plt                 byte-identical    byte-identical
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The raw process diff rises 55,072 -> 55,256 because the faithful two-chain re-rolls
`MachineBlockPlacement`; the official fuzzy pct is unchanged at 57.4 and every section
size and the gate stay exact.  The two source edits are both faithfulness fixes against
the original's Thumb.

## The exact LSDA (new decode)

The process `.ARM.exidx` entry is at `0x1a12a0` (`w0=0x7ff96a78` -> fn `0x137d18`,
`w1=0x21854` -> extab `0x1c2af8`).  The generic-model extab entry is 4 words
(personality `0x7fe9c068` -> `__gxx_personality_v0` at `0x5eb60`, then 3 unwind
opcode words), so the **LSDA starts at `0x1c2b08`**:

```
LPStart enc 0xff (omit -> function start 0x137d18)
TType   enc 0xff (omit)
call-site enc 0x01 (uleb128)
call-site table length 0x144  (46 entries, 23 invoke + 23 no-lp)
```

The 24 distinct landing pads are the cleanup blocks at `0x145bf0..0x145c6c`
(`operator delete`, `String::~String`, `_Unwind_Resume`).  The four String temporaries
of the case-125 state-1 loop

```cpp
port->setName(*Globals::gameText->getText(3212) + String(" ", false) + String(i - 7));
```

are the original's call-site entries `[15..18]` (`start 0x3790/0x37a2/0x37aa/0x37b6`,
`lp 0xdf4a/0xdf40/0xdf36/0xdf2c`), i.e. the `operator+` result, the `String(i-7)` ctor,
the second `operator+`, and the `setName` call.  The `new Route` pair is `[6]`
(operator new, `lp=0`) + `[7]` (`Route::Route`, `lp=0xdf10` = delete cleanup).  The
scope is structurally identical in the base (46 entries, same 23 invoke/no-lp split);
only the addresses differ.

## Why the else-if blocked — the exact byte accounting

The table length is the sum of the uleb128 widths of `start`, `length`, `landing_pad`
plus 1 action byte per entry.  Decoding both tables:

| build | table | entries | entries with `start < 0x4000` |
|---|---:|---:|---:|
| original | **0x144** | 46 | 20 |
| master (switch) | 0x146 | 46 | 18 |
| two-chain (w8203) | 0x148 | 46 | 16 |
| **two-chain + case-91 nested (landed)** | **0x146** | 46 | 18 |

Every extra byte is a `start` field that crosses the 14-bit uleb boundary `0x4000`
(2-byte -> 3-byte).  The two-chain's four extra bytes were exactly:

| entry | original | two-chain | block |
|---|---|---|---|
| `[16]` | `0x22c2` (2 B) | `0x4472` (3 B) | `Route::Route` ctor (case-125 state 0) |
| `[17]` | (follows) | `0x447a` (3 B) | no-lp after the ctor |
| `[18]` | `0x2f82` (2 B) | `0x4be8` (3 B) | case-91 state-0 `setName(getText(3211))` |
| `[19]` | (follows) | `0x4bf0` (3 B) | no-lp after the setName |

The **master switch** only paid the last two (+2 -> 0x146) because its
`if (m_nState == 0 && ...)` is followed by a *separate* `switch`, so clang kept the
state-0 body inline.  The **two-chain** `else if (m_nState == 1)` makes clang outline
the state-0 body to `0x4442` (`Route::Route` at `0x4472`), adding the first two bytes.

## The landed fix — case-91 nested state-0 test

The original's case-91 state-0 dispatch (`0x13ac42`) is

```
13ac42  ldr r0,[sp,#88] ; ldr r0,[r0,#28]   ; m_nState
13ac46  cmp r0,#0
13ac48  bne.w 13ebe8                        ; state != 0 -> cold
13ac4c  ... messages[3]->isTriggered() ...
13ac58  beq.w 13ebe4                        ; !triggered -> cold
13ac5c  state 0 body (inline, setName at 0x13ac9a)
```

i.e. the body is the **fallthrough** of both tests (the `if (m_nState == 0 && ...)`
shape).  Our source had the same `&&`, but clang outlined the body (the `setName`
block sat at `0x4946`).  Writing it as the equivalent nested form

```cpp
if (m_nState == 0) {
    if (((RadioMessage *) ((*messages)[3]))->isTriggered()) {
        ... state 0 ...
    }
} else if (m_nState == 1 && player->isDockedToDockingPoint()) { ... }
```

keeps the body inline (`setName` back at `0x2f82`-relative, below `0x4000`), matching
the original's Thumb, and drops the call-site table from `0x148` to `0x146` — which
fits the 0x15c-byte extab entry, so `.ARM.extab` stays 156,312 B.

## The landed two-chain (case 125)

The original's case-125 dispatch is two `if`/`else if` chains (Thumb, base `0x137d18`):

```
139f9c  ldr r1,[r0,#28] ; cmp r1,#0 ; bne.w 13b43e   ; state 0 hot, else cold
139fba  state 0 body (inline) ... b.w 13b518         ; -> state-2 test
13b43e  cmp r1,#1 ; bne.n 13b518                     ; state 1
13b444  state 1 body ... falls through to 13b518
13b518  cmp r0,#2 ; bne.w 13dcb6                     ; state 2
13b522  state 2 body ... b.w 144a0c                  ; shared m_nState++ tail
13dcb6  cmp r0,#3 ; bne.w 13de44                     ; state 3
13de44  subs r0,#4 ; cmp r0,#2 ; bhi tail            ; states 4..6
```

The source now expresses exactly that structure:

```cpp
if (m_nState == 0 && msg1->isTriggered()) { state0; }
else if (m_nState == 1) { state1; }
if (m_nState == 2) { state2; }
else if (m_nState == 3) { state3; }
else if (m_nState >= 4 && m_nState <= 6 && ...) { state4..6; }
```

plus the w8203 loop declaration order (`awake, pirate, playerPos, sideZ, offsetY,
offsetZ, sideX`) which pays back the +16 B that the `else if` costs on this shape.
The case-125 state-0 body is still outlined by clang (`Route::Route` at `0x4472`,
original `0x22c2`), but the table now fits because the case-91 block moved down.

## Variants measured (all built + linked + LSDA-decoded)

| variant | size | diffbytes | table |
|---|---:|---:|---:|
| **master (switch 1..3+default)** | **57232** | **55072** | **0x146** |
| two-chain (w8203) | 57232 | 55278 | 0x148 |
| two-chain + `m_nState++` (case 3) | 57232 | 55270 | 0x148 |
| two-chain + `__builtin_expect(state==0,1)` | 57216 | 55201 | 0x14b |
| two-chain + `__builtin_expect(cond,1)` | 57232 | 55278 | 0x148 |
| nested `if(state==0){if(cond)}` (case 125) | 57240 | 55213 | 0x14b |
| single else-if chain (nested in else) | 57376 | 55364 | 0x14d |
| hybrid `if/else-if(1)` + `switch(2,3,default)` | 57248 | 55129 | 0x148 |
| case-125 moved before case 92 | 57232 | 55277 | 0x148 |
| two-chain + `expect(state==0)` on case 91 | 57236 | 55389 | 0x146 |
| **two-chain + nested case-91 (landed)** | **57232** | **55256** | **0x146** |

## Conclusion

The LSDA scope was already faithful (46 entries, same invoke/no-lp split, same
cleanups); the else-if's +4 was purely two outlined blocks crossing `0x4000`.  The
case-91 nested test (a genuine faithfulness fix — the original's body is inline)
moves one of them back below `0x4000`, which lets the faithful case-125 two-chain land
at exactly 57,232 B with `.ARM.extab` unchanged and the ratchet green.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8408/lsdadump.py                       # decode both call-site tables
python3 /dev/shm/w8408/w2.py                             # per-entry uleb width diff
python3 /dev/shm/w8408/measure.sh /dev/shm/w8408/v_c91nest.cpp
```
