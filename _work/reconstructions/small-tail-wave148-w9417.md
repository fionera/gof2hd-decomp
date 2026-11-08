# Small tail wave 148 (w9417): Gun / Generator / MenuTouchWindow / ModStation —
# new hypotheses on the four residuals (no landed `src/` change)

Status: **no landed `src/` change.** Worktree `wt/w9417`, base `cbe3b5f7f`
(wave 147). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave147-w9316.md`, `small-tail-wave146-w9215.md`,
`near-miss-trio-wave146-w9216.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.

## 0. Residual classification (raw same-address bytes / aligned instructions)

| target | size | raw | aligned |
|---|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 57 |
| `Generator::createMission` | 1996 B | 445 | 59 |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 16 |
| `ModStation::ModStation` (C1/C2) | 1008 B | 43 | 12 |

## 1. Gun — the loop at 0x153442 is a **second loop over `enemies`**, and the
##    hoist is tied to loop rotation

Re-decoded the region.  The outer `e` loop is at `0x153512`/`0x153014`
(bound `[r9,#180]` = `this->enemies`, index `r2`).  The `i` loop over
`positions` is at `0x15305e` (bound `[r9,#8]`, data `[r9,#12]`, index `fp`,
`i*3` at `0x15306c`).  The scatter block at `0x153442` is a **third loop**
over `[r9,#180]` (enemies, index `r5`), whose body calls
`Player::getPosition` on `enemies[i]` and computes `VectorLength`.

Original:
```
153442 ldr r0,[r0,#4]        ; enemies->data
153444 add r4,sp,#104        ; sret temp, recomputed each iteration
153446 ldr r8,[r0,r5,lsl#2]  ; enemies[i]
15344a mov r0,r4
15344e blx Player::getPosition
```
Ours hoists `add r4,sp,#104` into both predecessors of the scatter block
(`0x153198`, `0x153260`), which forces `i*3` into `r5` (original `r4`) and
swaps the `sp+44`/`sp+104` StackSlotColoring pair.

New probes this wave (all reverted):

| variant | raw | size |
|---|---:|---:|
| HEAD | **463** | 1984 |
| `Vector playerPosition = enemy->getPosition();` | 1513 | 1976 |
| direct `VectorLength(enemyPosition - getPlayer()->getPosition())` | 467 | 1984 |
| `const Vector &playerPosition = ...` | 463 | 1984 |
| `Vector playerPosition; playerPosition = ...;` | 1809 | 2000 |
| extra scope block around `playerPosition` | 463 | 1984 |

The `enemy->getPosition()` hypothesis is falsified (size 1976).  The hoist
survives every source shape tried; it is coupled to the bottom-tested vs
top-tested rotation of the scatter loop (original bottom-tested at
`0x1534fc`, ours top-tested at `0x153440`).

## 2. Generator — the faithful form's +4 B is one extra live 4-byte frame
##    object; ten more spellings still keep `rem` in `r9`

Re-confirmed: faithful `bonus = rawBonus - rem` is **304 raw / 2000 B**
(frame 104) vs shipped algebraic `bonus -= rem * 2` **445 raw / 1996 B**
(frame 96).

MIR (`-mllvm -print-after=greedy`):
* base: `fi#0..11` live (12 objects);
* faithful: `fi#0..12` live (13 objects) — one extra 4-byte slot.

Original keeps `agent` in `r9` and spills `rem` into the dead `[sp,#32]`;
ours keeps `rem` in `r9` (`mov r9,r1`) and reloads `agent` from `[sp,#36]`.

New probes this wave (all reverted; all 2000 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD algebraic | **445** | **59** |
| faithful | **304** | **112** |
| `bonus = rawBonus + rem; if (bonus % 50) bonus = rawBonus - rem;` | 304 | 112 |
| `((bonus % 50) != 0)` / `rawBonus - (rem)` | 304 | 112 |
| `int bonus2 = rawBonus + rem; ...; bonus = bonus2;` | 304 | 112 |
| ternary `bonus = bonus % 50 != 0 ? rawBonus - rem : bonus;` | 304 | 112 |
| `bonus = rawBonus; bonus -= rem;` (two steps) | 304 | 112 |
| `bonus -= rem; bonus -= rem;` | 304 | 112 (note: earlier 1992 on w9316 base) |
| `volatile int rem` | 702 | 91 |
| `__builtin_expect(bonus % 50 != 0, 1/0)` | 304 | 112/113 |
| `bonus -= rem + rem;` / `bonus -= 2 * rem;` / `bonus -= (rem << 1);` | 445 | 59 (1996 B) |
| `agentName` moved after the bonus block | 1762 | 487 (1928 B) |

The faithful form cannot be landed at 1996 B: the extra live object is the
`agent` spill, and every spelling tested still evicts `agent` instead of
spilling `rem`.  The remaining lever is the same as wave 147: make `rem`
spill (not `agent`); no source spelling found.

## 3. MenuTouchWindow — direct `Globals::layout` for all four accesses is
##    1892 B; the 4-byte loss is the `&Globals::Canvas` fp->r5 move

Re-confirmed the original's case-15 block:
```
12a212 ldr r2,[sp,#16]      ; &Globals::layout
12a218 ldr r2,[r2,#0]       ; Globals::layout
12a21e ldr r5,[r2,#12]      ; leftMargin  (B, fresh)
12a220 ldr r6,[r2,#32]      ; top         (C, fresh)
```
`[sp,#16]` is `&Globals::layout` (not the value): the prologue
`ldr r0,=GOT(Globals::layout); add r0,pc; ldr r1,[r0]; str r1,[sp,#16]`
loads the address.  HEAD caches the *value* in `r6` across the call.

New probes this wave (all reverted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **28** | **16** | 1896 |
| B direct only (A local) | 358 | 76 | 1892 |
| all four direct | 358 | 76 | 1892 |
| two table-pointer locals then ternary (ModStation style) | 43 | 12 | 1008 (ModStation) |

Making B direct flips `&Globals::layout` into `fp` and `Globals::Canvas`
into `r5`; the `ldr.w fp,[r1]` (4 B) becomes `ldr r5,[r1,#0]` (2 B) and
`ldr.w r0,[fp]` becomes `ldr r0,[r5,#0]`, losing 4 B.  The original keeps
`fp = Globals::Canvas` and `[sp,#16] = &Globals::layout`.  No shape found
that keeps Canvas in `fp` while making B direct.

## 4. ModStation — literal-pool order + pre-RA copy, unchanged

Re-confirmed 43 raw / 12 aligned / 1008 B.  Two independent artefacts:
1. literal-pool order: original `YawIPad` (`d56b4`) then `Yaw` (`d56b8`);
   ours `Yaw` then `YawIPad`;
2. `str.w r8,[sp,#184]` / `mov r4,r2` hoisted above the yaw load in ours,
   after it in the original.

New probes this wave (all 1008 B, all 43 raw / 12 aligned):

| variant | raw |
|---|---:|
| HEAD | **43** |
| two table-pointer locals (`yawIPadTable`, `yawDefaultTable`) then ternary | 43 |
| `!iPad ? Yaw : YawIPad` | 43 |
| `if (iPad) yawTable = YawIPad; else yawTable = Yaw;` | 43 |

The literal order is the backend's select materialisation order and does
not follow declaration or operand order; the pre-RA copy is not the
machine scheduler.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9417
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9417/fdiff.py <mangled>
python3 /dev/shm/w9417/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w9417/`: `fdiff.py`, `align_diff.py`,
`sd.py`, `m.sh`, `sweep_gen*.py`, `sweep_gun.py`, `gen_all.txt`,
`gen_base_all.txt`, `base/` (pristine sources).

## 6. Files changed

`_work/reconstructions/small-tail-wave148-w9417.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
