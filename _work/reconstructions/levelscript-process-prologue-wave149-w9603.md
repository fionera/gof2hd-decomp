# LevelScript::process — wave 149 / w9603: the faithful prologue accessor
# order, and where the +72 B actually lives (it is NOT the messages spill)

Status: **code change landed** (a size-exact accessor permutation that is a
small `align_diff` win); the faithful `messages/player/enemies` order is
re-confirmed at **-426 aligned / +72 B** but could not be brought back to
57,232 B.  Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Base master `7eed2966e` (wave 149).
Scratch in `/dev/shm/w9603/` (not tracked): `fdiff.py`, `align_diff.py`,
`insdiff2.py`, `gap.py`, `try.sh`, `LevelScript.base.cpp`, `so.base`,
`so.reorder`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,898            54,898            (0)
process aligned insns    15,802            15,795            (-7)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text size         0x1337c8          0x1337c8          (exact)
.ARM.extab               0x26298           0x26298           (exact)
.rodata / .data / .bss   exact             exact
.rel.plt                 0 diff            0 diff
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The landed edit is a **semantically-neutral accessor permutation**:

```diff
     Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
-    PlayerEgo *player = m_pLevel->getPlayer();
     Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    PlayerEgo *player = m_pLevel->getPlayer();
```

i.e. `enemies, messages, player`.  The three accessors are pure out-of-line
getters, so the order is behaviour-free.  It is size-exact and drops the
global `align_diff` total by 7 instructions (15,802 -> 15,795).  It is **not**
the faithful order; it is the best variant that keeps the gate green.

## The faithful order (re-measured, still +72 B)

`messages, player, enemies` (w9507's variant) reproduces exactly:

```
raw 55,251   aligned 15,376   size 57,304 (+72)
```

The prologue itself is **-2 B** vs the base (the `strd r4,r0,[sp,#80]` pair
replaces the base's separate `str r5,[sp,#80]` + `str r4,[sp,#84]`), and the
`messages`/`delta` stack slots become the original's
(`[sp,#72]=messages`, `[sp,#76]=delta`; the base has them swapped).  The
mission-1 block is +2 B and the mission-0 block is -6 B, so the two builds
are at the *same address* (`0x137f7c`) at the start of the shared tail.

## Where the +72 is: whole-function codegen, not a messages spill

The wave-149 mission hypothesis ("the messages spill is the extra stack slot")
is **falsified** by direct measurement:

* Both builds use the same 432-byte frame (`sub sp,#432`) and the same number
  of `str`/`ldr` to the `sp+72`/`sp+76` pair.  The base does 185 loads of
  `[sp,#72]` (delta) + 43 of `[sp,#76]` (messages); the reorder does 43 + 184.
  It is a pure **slot swap**, not an extra slot.
* Instruction counts: base 20,675 instructions / 57,232 B; reorder 20,650
  instructions / 57,304 B.  The reorder has **25 fewer instructions but 72
  more bytes** (16-bit encodings replaced by 32-bit ones).
* The function's trailing literal pool is the same 56 bytes in both builds
  (base `0x145c70..0x145ca8`, reorder `0x145cb8..0x145cf0`); the +72 is all in
  the code before the pool.
* The gap opens gradually (first at `0x13886c`, +4 = one alignment `nop` plus
  one hoisted `ldr r6,[sp,#76]`), then grows through literal-pool
  relocations: the case-125 pool moves from `0x13a194` (40 B) to `0x13afd0`
  (52 B) for +56, and the EH cleanup region grows by ~4 B.  The remaining
  difference is distributed instruction-width/regalloc noise.

So the +72 is the same whole-function register-allocation gap documented by
wave-121 w7104 (`status` in `r9` instead of the original's `r4`; `this` kept
in `sl` instead of being spilled to `[sp+88]`).  It cannot be removed by a
local prologue edit.

## Variants measured (all on wave-149 HEAD)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD `enemies,player,messages` | 57232 | 54,898 | 15,802 |
| **`enemies,messages,player` (landed)** | **57232** | **54,898** | **15,795** |
| `enemies,messages,player` = EMP | 57232 | 54,898 | 15,795 |
| `messages,player,enemies` (faithful) | 57304 | 55,251 | 15,376 |
| `messages,enemies,player` | 57304 | 55,263 | 15,383 |
| `player,messages,enemies` | 57304 | 55,263 | 15,386 |
| `player,enemies,messages` | 57220 | 55,376 | 15,496 |
| `enemies,player,messages` (base) | 57232 | 54,898 | 15,802 |
| faithful + `__builtin_expect(mission==0,1)` | 57372 | 55,647 | 21,945 |
| faithful + `__builtin_expect(mission==0,0)` | 57460 | 55,402 | 23,797 |
| faithful + local `Status *status` (all uses) | 55492 | 53,861 | 24,127 |
| faithful + `volatile` messages | 57216 | 54,927 | 18,712 |
| faithful + anchor removed (`delta==0x7d`) | 57288 | 55,327 | 15,368 |
| faithful + case-80 `dockNavPtr` (line 3067) removed | 57296 | 55,284 | 15,371 |
| faithful + both of the above | 57280 | — | — |

Declaration order (declaring the variables in one order and assigning in
another) has **no effect**; only the assignment/call order matters.  The
`messages`-first orders are all +72 B; `player`-first is -12 B; `enemies`-first
is size-exact.

## Why the faithful order cannot be landed at 57,232 B in this pass

Reaching 57,232 B from the faithful order needs a -72 B companion.  The known
size levers on this base are small: the `delta==0x7d` PLT anchor is -16 B but
breaks `.rel.plt` byte-identity (26-byte diff), the case-80 `dockNavPtr` at
line 3067 is -8 B, the faithful case-154 tail is -4 B.  Even all three
together give only -28 B.  The case-125 literal-pool relocation (+56 B) is the
biggest single component and would need the original's outlined case-125
state-1/2 blocks (w9303) to reproduce; that is a separate pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9603/fdiff.py _ZN11LevelScript7processEi        # 54,898 / 57,232
python3 /dev/shm/w9603/align_diff.py _ZN11LevelScript7processEi  # 15,795
```

## Open questions

1. The faithful order's +72 is dominated by the case-125 pool relocation
   (+56 B).  Reproducing the original's outlined case-125 state-1/2 blocks
   (w9303: `0x13b43e`/`0x13b518`) should recover most of it.
2. Getting `status` into `r4` (instead of `r9`) in the prologue is the
   remaining 4-byte prologue difference; it is a whole-function allocation
   choice and was not reachable from any local source shape tried.
