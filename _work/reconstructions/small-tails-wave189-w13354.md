# Small tails — wave 189 / w13354: MGame m122 block isolated to a 2-byte anchor, no net win

Status: **research note, no `src/` change landed.** Worktree `wt/w13354`, branch
`wave189-w13354`, base master `6f5e6a957`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13354`.

This pass re-attacked the five smallest size-exact residuals with a
register-live-range lens.  The main new result is that `MGame::OnUpdate`'s m122
block can be reproduced **instruction-for-instruction** (including the
`movw r2,#3166 / ldr.w r1,[fp,#120] / add r2,r4 / mov r0,r1` argument setup)
by forcing `&Globals::status` into `r9` with a `Status *volatile` dummy placed
before `nextInt`.  The dummy costs one 2-byte `str`, which shifts the whole
remaining function by 2 and loses more than it gains.  Every other candidate
either changes the function size or re-colours the body.  Tree left clean.

## 0. Baseline (all five at base source)

| symbol | VMA | size | raw | aligned |
|---|---|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **10** | 112 |

Gate on entry: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.text` diff **73,939**, `.ARM.extab` 614, build-id 20,
total 74,573, file size 2,187,328 exact.

## 1. `MGame::OnUpdate()` — the m122 block is now understood exactly

The original m122 block (`0x17f190`, 40 B) is:

```
mov   r4, r0                 ; radioIndex
ldr.w r0, [r9]               ; reload Globals::status
blx   Status::getCurrentCampaignMission
movw  r2, #3166
ldr.w r1, [fp, #120]         ; this->level
cmp   r0, #122
it    eq
movweq r2, #3164
add   r2, r4
mov   r0, r1
movs  r1, #28
blx   Level::createRadioMessage
b.w   0x17e26c               ; shared getPlayingTime tail
```

`r9` holds `&Globals::status` (the *address* of the global slot, loaded once at
`0x17e08c` and live across the m23/m24/m122 blocks).  The landed base source
(`Status *st = Globals::status; ... Status *volatile dummy = st; ...`) keeps
`st` in `r4` across `nextInt`, which forces `&Globals::status` into `r9` (good)
but swaps the reload for `mov r0,r4; str r4,[sp,#120]` and puts `radioIndex`
in `r5` (6-byte m122 diff).  The m23 64-bit compare then uses `r4/r5` instead
of the original `r2/r4` (4-byte diff).  Total raw 10.

New cells measured this pass (all size 11440 unless noted):

| shape | raw | aligned | m23 | m122 | note |
|---|---:|---:|---:|---:|---|
| base (landed) | **10** | 112 | 4 | 6 | dummy store in m122 |
| `level_after_nextint` (`Level*` before `radioBase`, no `st`) | 16 | **6** | **0** | 16 | level hoisted to `r5`, base in `r1`, `adds r2,r1,r4` |
| `level_first_use` (`Level*` before `nextInt`) | 33 | 8 | **0** | 30 | level in `r4`, `radioIndex` in `r5` |
| `status_first_noload` (`Status *volatile dummy = Globals::status;` before `nextInt`, natural body) | 361 | 21 | 5 | **0**\* | m122 exact except one extra `str r2,[sp,#120]` (2 B shift) |
| `st_pt_after` (`st` used for the tail's `getPlayingTime`) | 318 | 19 | 2 | **0**\* | m122 exact; tail emits `mov r0,r5; b.w 17e270` (29 B) |
| `dummy_st_reload_after` (`st` load, `nextInt`, `ccm`, dummy, level) | 19 | **7** | 2 | 17 | constant in `r1`, level in `r0` |
| `dummy_st_reload` (`st`, `nextInt`, dummy, `ccm` reload) | 348 | 18 | 2 | 59 | extra store before `ccm` |
| `wingmen` dummy (`Status *volatile` in the wingmen block) | 361 | 21 | 5 | 0\* | compiler hoists the store to `0x17f18c` |
| `m23` dummy | 792 | 60 | — | — | store inside m23 |
| `hints` dummy | 6167 | 1037 | — | — | size 11340 (r5) |

\* m122 block instruction-identical from `mov r4,r0` through `b.w` (same
registers/operands, including `movw r2,#3166 / ldr.w r1,[fp,#120] /
add r2,r4 / mov r0,r1`); the residual is the 2-byte anchor store, which shifts
the block and every following PC-relative `blx`/`b.w`/pool reference by 2.

Key mechanism: the `&Globals::status` live range ends at the m122 block's
`ldr.w r0,[r9]`.  A `Status *volatile` store anywhere before that point forces
the address into `r9` (because the stored pointer occupies `r5`), but the store
itself is an extra 2-byte instruction in the m122 path.  A store after the
block leaves the address in `r5` and the function shrinks to 11392 B.  No
source form was found that occupies `r5` across the block without emitting a
store; the original's `r9` therefore comes from a construct we have not
recovered (possibly a different m24-block local that our reconstruction
already matches byte-for-byte, so it cannot be changed).

`int volatile` / `void *volatile` / `Status *volatile` after `nextInt` all
leave the address in `r5` (size 11396).  `__builtin_expect` on the m23 /
m122 conditions re-colours the body (raw 355–5121).  `local_elapsed`,
`local_limit`, `local_pt`, `cast_pt`, `sub_ll_first`, `const_left`, `ll_const`,
`ge_3600001`, `hour_expr` all leave the m23 diff at 4 bytes (raw 10).

## 2. `Hud::draw()` — the damage anchor is exactly size-neutral

The landed damage block is

```cpp
if (hitFlags & 0x24)
    Status **volatile p = &Globals::status;
    this->field_0x4b8 = 300;
if (hitFlags & 0x18)
    this->field_0x4b4 = 300;
```

and emits one unconditional `mov.w r0,#300`, a conditional anchor store
`strne.w r8,[sp,#208]`, an unconditional `str.w r0,[sl,#1208]`, then reuses
`r0=300` in the second block.  The original is two clean
`itt ne / movne / strne` blocks.  Bracing the first `if` makes the damage
block exact but the whole body re-colours to 9328 B (4 short); adding any
4-byte anchor back (`volatile int`, `Status *volatile`, tail store) re-colours
to raw 2929–7145.  New cells: `second_braced_first_anchor` 32/21/9332 (same
as base), `faithful_anchor_after` 7111/823/9336, `faithful_anchor_after2`
2929/889/9332.  The 13 one-byte dock diffs remain the `Canvas`/`top` `r5`/`r6`
colour tie.

## 3. `ModStation::ModStation()` — farKey operand colour is stable

New cells (all 1008 B / 374 insn): `local_yawfar` (third argument hoisted to a
`float` local) 11/5, `local_yawfar_ptr` 11/5, `arg_order_swap` 13/7,
`local_yawfar_bits` (volatile bit round-trip) 29/7.  The landed 11-byte shape
(load `farKey` x into `r4` + `mov r3,r4`) is still the best; the residual is
the greedy choice of `r4` vs `r3` for the `yawTable[race]` load.

## 4. `Generator::createMission()` — the `bhi` fold resists nesting

Nesting the mask test as `if (type < 15) { if (mask) { ... } }` (and `else {}`
variants) does not stop clang folding `type > 14 ⇒ type != 13`: raw stays 40,
size 1996, 744 insn.  The original's `bhi 0x97388` still enters the
`race < 4 && type == 13` test; ours jumps straight to `0x973fa`.  Preventing
the fold requires a memory round-trip for `type` (`volatile` costs 8 B / 2004 B
per the w13339 note; the faithful body is 2000 B).

## 5. `MenuTouchWindow::OnTouchBegin()` — unchanged

No new lever found beyond the w13339 table: the case-15 block is a
fresh-vs-cached `Globals::layout` colour tie (best landed 28/16/1896) and the
case-3/8 `ip` vs `r4` pair at `0x401/0x404` is a pure greedy-RA choice.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN10ModStationC1Ev _ZN10ModStationC2Ev \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv _ZN5MGame8OnUpdateEv
# raw=32/40/11/11/28/10, sizes 9332/1996/1008/1008/1896/11440
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13354/`
(`meas.sh`, `bytediff.py`, `disasm_fn.py`, `mg_sweep{10..22}.py`,
`hud_sweep6.py`, `mod_sweep6.py`, `gen_sweep2.py`, `*.base.cpp`).
