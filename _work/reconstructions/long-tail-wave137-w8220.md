# Long-tail wave 137 (w8220): Gun / Generator / SpaceLounge / near-miss trio /
# Radar extab — six new negative probes, no source win

Status: **no landed `src/` change.** Worktree `wt/w8220`, base `655e3bcc5`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave135-w8118.md`, `long-tail-wave134-w7762.md`,
`gun-calccharactercollision-wave133-w7739.md`,
`spacelounge-generator-wave133-w7740.md`, `near-miss-wave133-w7741.md`,
`extab_lsda_scope_w7742.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.text` 84,709 differing bytes,
`.ARM.extab` 1,395 (unchanged).

## 0. Targets attempted

| target | result |
|---|---|
| `Gun::calcCharacterCollision` (93.4 %, 1984 B) | **new negative:** `__builtin_expect(this->geometries != nullptr, 1)` shrinks the body to **1956 B** (610 diff lines) — the expect re-rolls the whole i-loop layout |
| `Generator::createMission` (97.2 %, 1996 B) | **new:** `volatile int rem` puts `bonus` at **sp+16** (the original slot) and spills `rem`, but at **sp+44** with an extra reload and a branch instead of `it ne`; 2000 B |
| `SpaceLounge::drawLounge` (95.7 %, 1948 B) | **new:** `const int pad` / `pad` declared after `centerX` / `pad*2` top forms all stay pad=fp at 1948 B; the boxW lever still costs 20 B (confirms w7740/w8118) |
| `MenuTouchWindow::OnTouchBegin` (98.1 %, 1896 B) | not reached (time-boxed after the other five) |
| `ModStation::ModStation` (99.2 %, 1008 B) | faithful `far = pitch` re-measured **18 lines** (baseline 6) — confirms w7719/w7762 |
| `MGame::OnUpdate` (99.9 %, 11440 B) | **new:** deleting the `Status *st = Globals::status;` local (the only non-`Globals::status` use) shrinks to **11392 B** / 1920 lines — the local is load-bearing |
| `.ARM.extab` `Radar::Radar` (7 B) | **new:** playerGuns init-statement / compound = 7 (tie); enemyGuns init/compound = 13; both-compound = 30; `__builtin_expect` on the loop guards = 11..57. Baseline 7 is the local optimum |

## 1. Gun: the geometries `__builtin_expect` is a body-size lever, not a hoist lever

The handoff hypothesis was "stop the k-loop sret add hoist".  Adding
`__builtin_expect(this->geometries != nullptr, 1)` at the `else if` does
change the block weights, but it does **not** stop the hoist: the function
drops to **1956 B** (28 B short) and the diff explodes to 610 lines.  The
i-loop body layout is re-rolled, so the shape is not size-exact.  Reverted.

MIR facts from w7739 still hold: the `expr` grouping is the shipped slot
grouping (`delta/view/k-loop` at sp+104, `pp/MST` at sp+44); the only
remaining delta is the LICM hoist order `%308` (fi#10) before `%282`
(fi#11) in bb.5.  The `t2ADDri` is trivially rematerialisable, so
`IsProfitableToHoist` returns true before the register-pressure check; a
source-level way to make the k-loop add non-invariant was not found.

## 2. Generator: `volatile int rem` gets the right *bonus* slot but the wrong *rem* slot

The original block (`0x9770c..0x97722`) is:

```
adds  r5, r1, r6        ; bonus = rem + rawBonus
str   r1, [sp, #32]     ; rem -> sp+32
movs  r1, #50
mov   r0, r5
blx   __aeabi_idivmod
ldr   r0, [sp, #32]     ; rem
cmp   r1, #0
it    ne
subne r5, r6, r0        ; bonus = rawBonus - rem
str   r5, [sp, #16]     ; bonus -> sp+16
ldr   r5, [sp, #24]
```

`volatile int rem` in the faithful `bonus = rawBonus - rem` shape emits:

```
str   r1, [sp, #44]     ; rem -> sp+44 (not sp+32)
movs  r1, #50
ldr   r0, [sp, #44]     ; extra reload
adds  r5, r0, r6
mov   r0, r5
blx   __aeabi_idivmod
cbz   r1, ...           ; branch, not `cmp/it ne`
ldr   r0, [sp, #44]
subs  r0, r6, r0
str   r0, [sp, #16]     ; bonus -> sp+16  (matches original)
```

So the volatile local fixes the *bonus* stack slot but adds a reload and a
branch, and lands `rem` at sp+44.  Size 2000 B.  Not exact.

The w8118 result stands: the exact block is reachable only with a live
`agent` across the block (`if (agent == 0) bonus = 0;`), which costs 8 B.
The original keeps `agent` in r9 from `0x976e6` (`getRace`) through
`0x9776a` (`getImageParts`); our allocator splits it and reuses r9 for
`rem`.  Moving `String agentName = agent->getName();` after the bonus block
to force the use shrinks the function to **1924 B** (724 lines) — the whole
tail re-rolls.  Reverted.

## 3. SpaceLounge: pad/x are still a pure two-register rotation

Measured with the per-TU `.o` screen (pad load `ldr.w rX,[r6,#148]`,
function size/instruction count from `nm -S`):

| variant | pad | size | insns |
|---|---|---:|---:|
| HEAD (`short pad3`) | fp | 1948 | 728 |
| `const int pad` | fp | 1948 | 728 |
| `pad` declared after `centerX` | fp | 1948 | 728 |
| `int top = ... (pad * 2)` | fp | 1948 | 728 |
| `int boxW = pad*2+width` used in both calls, faithful clamp | **r9** | 1968 | 731 |
| `boxW` in both, HEAD `pad3` clamp | r5 | 1964 | 734 |
| `boxW` computed after `drawBox`, used in `DrawRectangle` | r4 | 1976 | 732 |
| `boxW` + `pad2` | fp | 1940 | 726 |

The `boxW` lever still reaches pad=r9 only at 1968 B: `pad*2` moves to
`fp`, `lineHeight` to `r8`, and `top` spills.  HEAD already has
`pad*2 = sl`, `lineHeight = fp`, `top = r8`; only `pad` itself is `fp`
instead of `r9` (and `x` is `r9` instead of `r5`).  No shape found that
flips only that one register pair.

## 4. ModStation / MGame

* `ModStation::ModStation`: faithful `far = pitch` is still **18** structural
  lines (baseline `far = yaw`, 6 lines).  The faithful shape swaps the two
  `Yaw`/`YawIPad` LDRLIT register assignments (`r3`/`r4`) and sinks the
  `Yaw` literal, so the "semantically correct" source is worse by the byte
  metric.  The 6-line residual stays a pre-RA NodeQueueId tie on the yaw
  load (`ldr.w r3` vs `ldr.w r4`) and the race spill.
* `MGame::OnUpdate`: the `Status *st = Globals::status;` local at line 2768
  is the only place the original's `ldr.w r0,[r9]` reload is replaced by a
  cached register.  Deleting it (and using `Globals::status->getPlayingTime()`)
  changes the GOT-pointer register from r9 to r5 and shrinks the function to
  **11392 B** (1920 lines).  The local is load-bearing; the `Globals::status`
  volatile lever (w7762 §4) remains untried at the full-gate level.

## 5. `.ARM.extab` Radar::Radar: the pad swap is not reachable from these shapes

Baseline (`playerGuns` / `enemyGuns` plain `if`): **7** differing bytes
(rows 1,3,11,13,15 outer pad 2464 vs 2462; rows 19,21 enemy pad 2462 vs
2464).  Tested shapes (full link + `extab_lsda_decode.py`):

| shape | diff bytes |
|---|---:|
| baseline | **7** |
| `if (Array<...> *playerGuns = getPlayerGuns())` init-statement | 7 |
| compound around playerGuns decl+if | 7 |
| `if (Array<...> *enemyGuns = ...)` init-statement | 13 |
| compound around enemyGuns decl+if | 13 |
| both init-statements | 30 |
| compound around both loops | 30 |
| `__builtin_expect(playerGuns != nullptr, 1)` | 29 |
| `__builtin_expect((*playerGuns)[i]->isRocketGun(), 1)` | 11 |
| `__builtin_expect((*enemyGuns)[i]->isRocketGun(), 0)` | 57 |

The tie is `MachineBlockPlacement::selectBestCandidateBlock` keeping the
last minimum in `EHPadWorkList`; the outer pad must be *later* in the
worklist than the enemy pad.  Source-order swaps (enemyGuns loop first)
change the `.text` body order (61 extab bytes), so they are not viable.
No body-preserving shape found.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8220
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                         # extab 1395 B / 7 B Radar
```

Scratch (not committed), `/dev/shm/w8220/`: `score.sh` (full link + verify
one symbol), `cc.sh` (exact per-TU compile), `sl_try*.py` (SpaceLounge
register screen), `gen_try.py` (Generator bonus-block screen),
`radar_try*.py` (extab shape screen), `*_full.txt` (disassembly dumps).

## 7. Files changed

`_work/reconstructions/long-tail-wave137-w8220.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
