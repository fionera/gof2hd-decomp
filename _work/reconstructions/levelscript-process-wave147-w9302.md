# LevelScript::process — wave 147 / w9302: prologue accessor order + the
# status-register premise is FALSE on the wave-146 base (both orders put
# `Globals::status` in r9; the +60 is the player spill, not status)

Status: **analysis only, no code change landed** (source restored to HEAD
`9c9744ca5`). Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, size-exact. Owned TU
`src/game/world/LevelScript.cpp`. Scratch in `/dev/shm/w9302/` (not tracked):
`fdiff.py`, `measure.sh`, `try.py` (named-edit build harness).

## Result

```
                         HEAD (wave 146)   this pass
process raw diffbytes    54,869            54,869   (source pristine)
orig / ours size         57232 / 57232     57232 / 57232
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

## 1. The faithful accessor order is +60, and it does NOT move status to r9

Re-measured on the wave-146 HEAD.  All six accessor permutations (the call
order is the only thing that matters; declaration order is dead — see §3):

| accessor order | size | raw | prologue status load |
|---|---:|---:|---|
| enemies, player, messages (HEAD) | 57232 | **54,869** | `f8d0 9000 ldr.w r9,[r0]` |
| enemies, messages, player | 57232 | 54,869 | `ldr.w r9,[r0]` |
| player, enemies, messages | 57216 | 55,369 | `ldr.w r9,[r0]` |
| player, messages, enemies | 57292 | 55,379 | `ldr.w r9,[r0]` |
| messages, enemies, player | 57292 | 55,325 | `ldr.w r9,[r0]` |
| **messages, player, enemies (witnessed)** | **57292** | **55,313** | `ldr.w r9,[r0]` |

The original is `137d5a: 6804  ldr r4,[r0]` (2 B).  **Our HEAD already puts
status in r9**, so the reorder does not change the status register; the +60
is elsewhere.  The faithful order's prologue is instruction-identical to the
original through `strd r4,r0,[sp,#80]` / `ldr r0,[r4,#0]` / `getEnemies`, and
differs only at the status load (`r9` vs `r4`).

The +60 is **not** the status register: the status value is 48
`ldr.w rX,[r9]` in *both* HEAD and the faithful order.  The faithful order
spills `player` to `[sp,#84]` (matching the original) while HEAD keeps it in
`r4`; player reloads are original 163, faithful 165, HEAD 160, but that is
only ~10 B.  The remaining ~50 B is spread over the body's register-allocation
differences (the same global decision that leaves status in `r9`).

## 2. Getting status into r4 (measured; none is size-exact)

`__builtin_expect` is the only lever that moved the status register.  All
variants start from the faithful `messages, player, enemies` order.

| variant | size | raw | status reg |
|---|---:|---:|---|
| faithful (no expect) | 57292 | 55,313 | r9 |
| `expect(mission==0,0)` | 57488 | 55,496 | r9 |
| `expect(mission==0,0)` + `expect(mission==1,1)` | 57444 | 55,516 | r8 |
| **`expect(mission==0,1)` + `expect(mission==1,0)`** | **57380** | **55,721** | **r4** |
| `expect(mission==0,1)` + `expect(mission==1,1)` | 57300 | 55,611 | r0 |
| `expect(m_nState<=2,0)` | 57260 | 55,385 | r9 |
| `expect(m_nState==4,1)` | 57252 | 55,114 | r9 |
| `expect(m_nState==5,0)` | 57304 | 55,360 | r9 |
| `expect(m_nState==3,0)` | 57296 | 55,305 | r9 |
| `expect(m_nState==4,1)`+`==5,0`+`==3,0` | 57252 | **55,088** | r9 |

`expect(mission==0,1)` is the only shape that lands status in r4, but it also
makes the mission-0 block fall-through and grows the function to 57,380 B
(+148).  The m_nState expects move the size in 8/16-byte quanta but leave
status in r9.

## 3. Local `Status *status` / `Level *level` — no effect on the register

| variant | size | raw | status reg |
|---|---:|---:|---|
| `Status *status = Globals::status;` after accessors, first call only | 57292 | 55,313 | r9 (identical codegen) |
| same, first two calls | 57292 | 55,312 | r9 (vtable in r4) |
| `Status *status;` before accessors, assign after, first two calls | 57292 | 55,313 | r9 |
| `Status *status = Globals::status;` **before** accessors | 57308 | 55,334 | r9 (load before getMessages) |
| `Level *level = m_pLevel;` then `level->get*()` | 57292 | 55,387 | r9 |
| `Level *level` inserted after getMessages | 57292 | 55,366 | r9 |
| local `status` for **all** 105 uses (declared after accessors) | 56984 | 55,192 | r9 (player→r5, messages→r6, enemies→fp) |
| local `status` for all uses (declared at top) | 56984 | 55,192 | r9 (load at top) |
| local `status` only inside the mission==0 block | 57456 | 55,265 | r5 |
| `int mission = Globals::status->getCurrentCampaignMission();` | 57292 | 55,313 | r9 (identical) |
| `(*Globals::status).getCurrentCampaignMission()` | 57292 | 55,313 | r9 (identical) |
| `((Status *) Globals::status)->...` | 57292 | 55,313 | r9 (identical) |
| split declarations (6 declaration orders, calls fixed) | 57292 | 55,313 | r9 (all identical) |
| swap mission==0/1 branches | 57208 | 55,536 | sl |

`Level *level = m_pLevel;` is optimised back to the `ldr.w r0,[r4,#24]!`
member-address form, so it is prologue-neutral.

## 4. Why r4 vs r9 is a global allocator choice

The status value is one SSA load (only one GOT literal `0xd8400` in the whole
function, at `0x1380a8`).  The original spills it to `[sp,#92]` and reloads it
45 times; our faithful order keeps it in `r9` (48 `ldr.w rX,[r9]`, 22 stack
reloads).  The first live range is short in both (`137d5a..137d76`), and `r4`
is free at that point (it dies at `137d4e` after `getEnemies`).  The greedy
allocator nevertheless picks `r9`; `-print-after-all` shows `%11701` (the
status pointer) assigned `$r9` while `%12280` (`&m_pLevel`) gets `$r4`.
The mission-0 state-0 body then reuses the freed register differently:
original `mov r4,sl` (this→r4) + `mov sl,fp` (guard→sl), ours `mov r9,fp`
(guard→r9) with `this` kept in `sl`.  Matching the original needs the
state-0 body's `m_nState = 1` store to use a low register (`str r0,[r4,#28]`
vs our `str.w r0,[sl,#28]`), which is a body-allocation consequence, not a
prologue source shape.

## Open questions

1. Find a size-57232 companion for `expect(mission==0,1)` (57380) or for the
   faithful order (57292) that keeps status in r4.
2. The `m_nState==4,1` family is the best raw at 57,252 (55,114) but +20 B and
   still r9; it is not landable under the size gate.
3. The status/`this`/guard register triangle in the mission-0 state-0 body
   (`r4`/`sl`/`r9`) is the real upstream difference; it needs the state-0
   body's store forms to match, not a prologue edit.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9302/fdiff.py _ZN11LevelScript7processEi   # 54,869
```
