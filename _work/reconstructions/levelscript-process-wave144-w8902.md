# LevelScript::process — wave 144 / w8902 (case-70 `break` + state-1-first
# companion recovers the layout; size-exact 57232)

Status: **code change landed, 2026-11-08**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-143 head `f5093474f`.
Scratch helpers in `/dev/shm/w8902/` (not tracked): `fdiff.py`, `apply.py`
(named-edit harness), `calls.py` (call census), `secdiff.py`, `regions.py`,
`case91.py`, `orig.s`/`ours_*.s` disassembly dumps.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,619            54,697            (+78)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    85,851            85,929            (+78)
.ARM.extab diffbytes     1,499             1,498             (-1)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical, 29,392 B)
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The faithful `break` (w8802's open item)

The original's case-70 state-0 body ends `0x139aba str.w r8,[r6,#28]` then
`0x139abe b.w 0x144e36` (the post-switch tail).  Our source let the block fall
through into the state-1 `else if`; the comment claimed that was a no-op.  It
is not the original's shape.  Adding `break;` after `m_nState = 1;` is faithful.

**Break alone is a raw regression**: it re-colours the state-1 body from the
hot fall-through position (`0x13b9d8`) to the cold section (`0x13edc2`) and
flips the dispatch to `cmp #1 / beq cold; cmp #2 / fall-through`, the opposite
of the original `cmp #2 / beq cold; cmp #1 / fall-through` (`0x13c04e`).
Measured on this base: `57240 B`, raw `55,159` (+540).

## The companion: write the state-1 arm first

The two arms are mutually exclusive, so swapping them is behaviour-neutral:

```cpp
if (m_nState == 1) { ...state-1... } else if (m_nState == 2) { ...state-2... }
```

With the state-1 arm written first, clang emits *exactly* the original dispatch
and fall-through:

```
ours  0x13b91a cmp r1,#2 ; beq.w 0x13ee3a (state-2, cold)
      0x13b920 cmp r1,#1 ; bne.w 0x144e9c (tail)
      0x13b926 state-1 body (fall-through)
orig  0x13c04e cmp r1,#2 ; beq.w 0x13ec3c (state-2, cold)
      0x13c054 cmp r1,#1 ; bne.w 0x144e36 (tail)
      0x13c05a state-1 body (fall-through)
```

The state-1 body is now instruction-for-instruction identical to the original
except for the delta stack slot (`[sp,#72]` vs the original's `[sp,#76]`) and
the branch target.  The whole function is again exactly 57232 B.

Measured variants (all on wave-143 HEAD):

| edit | size | raw diff |
|---|---:|---:|
| HEAD | 57232 | 54,619 |
| `break` only | 57240 | 55,159 |
| **`swap` + `break` (landed)** | **57232** | **54,697** |
| `swap` only | 56728 | 54,959 |
| `swap` + `break` + `m4bc` | 57240 | 55,099 |

So the companion recovers the layout (raw 55,159 -> 54,697) and restores the
exact size.  The residual +78 vs the unfaithful HEAD is the 4-byte stack-slot
shift for `delta` (`[sp,#72]` vs `[sp,#76]`) plus the moved tail address; the
original's own state-1 body is otherwise reproduced byte-for-byte.

## `TransformGetTransform` (case 91, not case 157) — still merged

The two-call deficit is in **case 91** (`0x141164`/`0x141184`), not case 157:
the `field_178 > 7 && m_nScriptTime <= 179999` arm calls
`TransformGetTransform(geometry->transform)` and the `else` arm calls
`TransformGetTransform(geometry->childTransform)`, each followed by
`->Update(delta, true)`.

The IR already has two separate calls; the merge is machine-level branch
folding of the identical tail (`blx TransformGetTransform; <Update setup>;
blx Update`).  The original does **not** merge it because its if arm keeps
`delta` in `r6` across the call while the else arm reloads `[sp,#76]`, so the
two Update setups differ (`movs r1; asrs r3,r6; mov r2,r6; str` vs
`ldr r2,[sp,#76]; movs r1; str; asrs r3,r2`).  Our register allocator spills
`delta` to `[sp,#72]` on both paths, making the tails identical.

Tried and measured (all size-exact 57232, raw 54,697 — no change): local
`Transform *t` per arm (A), `break` after the if arm's Update (B), same plus
local (D), `longlong d = delta` in the else arm (E), explicit `unsigned int idx`
(F), local in the if arm (G), local in the else arm (H), transform local +
delta local in the else arm (W), delta local in the if arm (X).  A `volatile`
local in the else arm does split the calls but costs +16 B (57248, raw 55,024),
so it is not usable.  Forcing the split needs the original's global register
allocation, not a local source edit; left open.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8902/fdiff.py _ZN11LevelScript7processEi   # 54,697
python3 /dev/shm/w8902/apply.py swap break                  # re-apply the edits
```

## Open questions

* The `delta` stack slot is 4 bytes lower than the original (`[sp,#72]` vs
  `[sp,#76]`); the frame size is identical (432).  Finding the local that owns
  the other half of the original's `strd` pair would flip the pair order and
  likely recover most of the +78.
* The two `TransformGetTransform` calls need the original's register
  allocation (delta in a callee-saved register on the if arm).
