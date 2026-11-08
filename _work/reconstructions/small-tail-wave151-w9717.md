# Small tail wave 151 (w9717): Gun / Generator / MGame::OnUpdate —
# three new hypothesis families, no source win (note only)

Status: **note only, no `src/` change**. Worktree `wt/w9717`, base master
`db9658a51` (wave 150). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave150-w9617.md`, `small-tail-wave149-w9516.md`,
`gun-calccharactercollision-wave133-w7739.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
RATCHET PASS, avg 99.93.  All three targets stay size-exact.

## 0. Residuals (raw same-address bytes / aligned instructions)

| target | size | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 463 | 57 | 57 |
| `Generator::createMission` | 1996 B | 445 | 445 | 59 | 59 |
| `MGame::OnUpdate` | 11440 B | 318 | 318 | 19 | 19 |

Mangled names:
`_ZN3Gun22calcCharacterCollisionEv`,
`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`,
`_ZN5MGame8OnUpdateEv`.

## 1. MGame::OnUpdate — NEW: the original caches `&Globals::status`, not the value

The 19 aligned residual is **one real instruction** (`mov r0,r5` at the
mission-122 store) plus its 2-byte shift.  Everything else in the 19 is a
branch/literal-pool offset that only differs because of that shift:

* `lsrs r2,r5,#29` vs `lsrs r0,r5,#29` etc. at function offset `0x2c94`
  are **literal-pool words** (`0x00090ff0` vs `0x00090fee`), not code;
* the `nop` at `0x2a1a` is literal-pool alignment;
* all `b.n`/`beq.w`/`cbz` diffs are the same 2-byte shift.

New finding from the disassembly: the original's `r9` is
**`&Globals::status`** (address of the global pointer variable), not the
`Status*` value.  At `0x17e088`:

```
ldr r0,[pc,#872]; add r0,pc; ldr.w r9,[r0]   ; r9 = &Globals::status
ldr.w r0,[r9]; blx Status::getCurrentCampaignMission
```

and the shared store tail `0x17e26c` is
`ldr.w r0,[r9]; blx getPlayingTime; ldr.w r2,[r9]; strd r0,r1,[r2,#256]`.
So the original's mission-122 block is **fully direct**: ccm direct and
store direct, with `&Globals::status` CSE'd into `r9` across the whole
mission-23/24/122 chain (and spilled to `[sp,#36]` at `0x17e31c`/`0x17e846`
when `r9` is needed for the wingmen table).

The current HEAD anchor is a **value cache** `Status *st = Globals::status;`
which forces `r5 = Globals::status`; the store then needs `mov r0,r5` and
jumps to `0x17e270` (one instruction past the shared tail).

New hypotheses measured this wave (all 11440 B unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (value anchor at pt store) | **318** | **19** | 11440 |
| `Status *&st = Globals::status` at pt store only | 5148 | 597 | 11392 |
| `Status *&st` at ccm + store | 5148 | 597 | 11392 |
| `Status **sp = &Globals::status` at ccm + store | 5148 | 597 | 11392 |
| function-scope `Status *&st`, all 120 uses | 5165 | 633 | 11384 |
| function-scope `Status *st`, all 120 uses | 10565 | 1682 | 11100 |
| chain-scope `Status *&st`, chain uses only | 5165 | 633 | 11384 |
| chain-scope `Status *st`, chain uses only | 5151 | 815 | 11384 |
| chain-scope `Status **stp = &Globals::status`, chain uses | **318** | **19** | 11440 |
| chain-scope `stp` + direct ccm/store (no local anchor) | 5148 | 597 | 11392 |
| anchor at ccm, store direct (V3, re-measured) | 914 | 109 | 11436 |
| anchor at store target, call direct (V4) | 5253 | 761 | 11492 |
| `if (st != nullptr) { Globals::status->... = ...; }` | 894 | 127 | 11444 |
| block-local `Status **stp` at ccm + store | 5148 | 597 | 11392 |
| block-local `Status **stp` at store only | 5148 | 597 | 11392 |

Key result: `Status **stp = &Globals::status; ... (*stp)->...` reproduces
the original's chain-start **exactly** (`ldr.w r9,[r0]; ldr.w r0,[r9]`) and
keeps 11440 B / 318 / 19 — but as soon as the local anchor is removed the
compiler rematerialises `&Globals::status` and switches to `r5`, shrinking
the function to 11392 B (direct form).  The value cache is therefore
required for the frame/size, and it is exactly what produces `mov r0,r5`.

Open question: a source spelling that keeps `&Globals::status` in `r9`
(so the store is direct) *without* a live `Status*` value in `r5`.  The
`stp` form proves the address-only cache is expressible; the blocker is
that removing the value cache lets clang rematerialise the GOT-relative
address into `r5` and drop 48 B.

## 2. Gun::calcCharacterCollision — NEW: the hoist replaces `ldr fp` in two blocks

Re-confirmed HEAD = **1984 B, raw 463, aligned 57**.  The `i*3` register
(`r4` original vs `r5` ours) is decided by the k-loop sret-address hoist:
ours emits `add r4, sp, #104` at function offsets `0x208` and `0x2d0`
(addresses `0x153198`, `0x153260`) where the original has `ldr.w fp,[sp,#40]`.
`r4` is then live across the i-loop body, so `add.w r4,fp,fp,lsl #1`
(`i*3`) is forced into `r5`.  The original recomputes
`add r4, sp, #104` inside the k-loop body (`0x153442`).

New hypotheses measured this wave (all 1984 B unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **463** | **57** | 1984 |
| i-loop `__builtin_expect(i < count, 1/0)` | 463 | 57 | 1984 |
| k-loop `while` instead of `for` | 463 | 57 | 1984 |
| `this->targetDir = enemy->getPosition()` | 463 | 57 | 1984 |
| `Vector &ep = *td; ep = ...; ep -= ...` | 463 | 57 | 1984 |
| `Vector kpos; kpos = ...; *td = kpos;` (default+assign) | 1759 | 324 | 1972 |
| `Vector kpos[1]; kpos[0] = ...` | 1767 | 351 | 1972 |
| `Vector kpos[2]; kpos[k&1] = ...` | 1757 | 378 | 1976 |
| i-loop-scope `Vector kpos; kpos = ...; *td = kpos;` | 1664 | 273 | 2008 |
| i-loop-scope `*td = kpos = ...` | 1663 | 277 | 1996 |
| `volatile Vector kpos = ...` | build fail (volatile copy ctor) | | |

The `kpos` forms all lengthen/shorten the function and re-colour the whole
i-loop.  No source spelling prevented the `add r4, sp, #104` hoist while
keeping 1984 B.

## 3. Generator::createMission — NEW: faithful arithmetic is right, spill is not

Re-confirmed HEAD algebraic `bonus -= rem * 2` = **1996 B, raw 445,
aligned 59**.  The faithful `bonus = rawBonus - rem` is **2000 B, raw 304,
aligned 112**: ours emits `mov r9,r1` (rem in `r9`) and evicts `agent`
(agent then reloads from a new stack slot, frame 104); the original emits
`str r1,[sp,#32]` / `ldr r0,[sp,#32]` and keeps `agent` in `r9` (frame 96).

The base algebraic form already has frame 96 (rem in `r5`), so it is the
best aligned/size variant even though its arithmetic differs.

New hypotheses measured this wave (all 2000 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD algebraic | **445** | **59** |
| faithful `bonus = rawBonus - rem` | 304 | 112 |
| `bonus = rawBonus + rawBonus % 50; ... rawBonus - rawBonus % 50` (CSE) | 304 | 112 |
| `int rem2 = rem;` used twice | 304 | 112 |
| `int check = bonus % 50; if (check != 0)` | 304 | 112 |
| `if ((rawBonus + rem) % 50 != 0)` | 304 | 112 |
| if/else form (`bonus = rawBonus - rem` in then, `+ rem` in else) | 304 | 112 |
| `unsigned rem` / `long long rem` | 304/304 | 112/112 |
| `volatile int rem` | 702 | 91 |
| `int *remp = &rem;` then `*remp` | 304 | 112 |

The allocator consistently prefers `rem` (short, hot) over `agent` (huge
live range) for `r9`; the original's spill of `rem` to the dead
`[sp,#32]` is not reproducible by any tested spelling.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9717
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9717/fdiff.py <mangled>
python3 /dev/shm/w9717/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w9717/`: `fdiff.py`, `align_diff.py`,
`hunks.py`, `try_mgame.py`, `try_gun.py`, `gun_variants.py`, `gun_v4.py`,
`try_gen.py`, `gen_variants2.py`, `gen_variants3.py`, `*.base.cpp`,
`gun_ir.txt` (full `-print-after-all` dump).

## 5. Files changed

* `_work/reconstructions/small-tail-wave151-w9717.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
