# `Level::createShip` — wave 168 (w11315): RA-map re-sweep; the map is
# reachable in part at 2,764 B but it is not the raw residual

Worktree `wt/w11315`, base `master 631c26f0b` (wave 167).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`).  Evidence: objdump of the
original and of our linked `.so`, plus `-mllvm -print-machineinstrs`
scratch `/dev/shm/w11315/`.

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0).

**No source change landed.**  The committed body remains the raw optimum
(1,560 / 455 / 2,764 B).  This note records the sweep and the measured
counter-example to the "RA shift is the dominant residual" hypothesis.

## 1. Entry map (original vs committed)

| value | original | ours |
|---|---|---|
| `race` | `[sp,#60]` (`str r1`) | `[sp,#56]` (`strd`) |
| `shipClass` | `r5 -> sl -> r6` | `[sp,#60]` (`strd`) |
| `type` | `fp` (r11) | `sl` (r10) |
| `this` | `r9` (spill `[56]`) | `r9` (spill `[52]`) |
| `jitterX/Y/Z` | `[36]/[32]/[28]` | `fp,[28],[24]` |

## 2. Sweep (full link, size checked)

Generator knobs: coordinate type (`mixed` = `float z,y; int x` as
committed, `int x,y,z`, `float x,y,z`), coordinate declaration order,
coordinate position (`post` = after `new Player`, `pre` = before it,
`emp` = after `setEmpData`), status-null-check present/absent, jitter
order, hitpoints case order, mission guard (`split` if/else as committed,
`bit` = single `(mission>=49 && mission<=52) || mission==56`).

### 2.1 Only the committed shape reaches 2,764 B at raw 1,560

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **committed** (`mixed`, `status=1`, `post`, jitter `Y,X,Z`, split) | **1,560** | **455** | **2,764** |
| committed + jitter `X,Y,Z` (faithful) | 1,560 | 460 | 2,764 |
| committed + mission `bit` (faithful) | 1,560 | 530 | 2,764 |
| `mixed`, `status=0`, `post` (no spurious null check) | 2,051-2,053 | 491-509 | 2,764 |
| `int x,y,z`, `status=1`, `post` | 2,530 | 886 | 2,788 |
| `int x,y,z`, `status=0`, `post` | 2,412 | 470 | 2,792 |
| `int x,y,z`, `status=0`, `pre` | 2,342 | 536 | 2,772 |
| `mixed`, `status=1`, `pre` | 2,495 | 511 | 2,780 |
| `float x,y,z`, `status=1`, `post` | 2,499 | 585 | 2,800 |

All 6 case orders and 3 jitter orders were re-measured on the committed
shape: raw 1,560-1,563, size 2,764 in every case.  `empTime` ternary,
`int cls = shipClass` copies (before/after `mission`), `int rc = race`,
`bool capital = shipClass==1`, two separate `if (shipClass==0)` /
`if (shipClass==1)` instead of `else if`, `switch (shipClass)`, moving
`armor`/`empPoints`/`hitpoints`/`level`/`mission`/the `mission==0x9a`
block around, and `__builtin_expect` on the status/waypoint/`mission==0x9a`/
`mission==56`/`shipClass==1` tests were all measured; none beats
1,560/455/2,764.

## 3. The map *is* partly reachable at 2,764 B — but raw gets worse

Moving `mission`, `level` and `hitpoints` **after** the three jitter
calls (`int mission = ...` etc. re-ordered below the `nextInt` block;
semantically identical, `level`/`hitpoints` do not feed the jitters)
reproduces 3 of the 4 original map features at exactly 2,764 B:

```
mov r9, r0          ; this      -> r9      (original: r9)
ldr r0, [r7, #8]    ; waypoint
mov r6, r3          ; type      -> r6      (original: fp)
mov sl, r2          ; shipClass -> sl      (original: r5->sl->r6)
cmp r0, #0
str r1, [sp, #60]   ; race      -> [60]    (original: [60])
```

Measured: raw **1,606** (+46), aligned **570** (+115), size 2,764,
1,121 insns (vs 1,117).  Jitter `X,Y,Z` or the mission bitmask on top
give 1,605-1,606.  So recovering the map *by itself* does not reduce
raw; the residual is dominated by the instruction/branch layout
(the switch compare chain, the coord block length, the mission guard),
not by the register colouring.

## 4. Why the allocator spills `shipClass` (pre-RA evidence)

`-mllvm -print-machineinstrs` on the committed body gives the
`After Machine Instruction Scheduler` IR with `DBG_VALUE` names:

| value | vreg | def slot | uses | live span |
|---|---|---:|---:|---:|
| `this` | %127 | 0 | 6 | 26,464 |
| `race` | %128 | 48 | 10 | 25,984 |
| `type` | %130 | 16 | 16 | 26,176 |
| `shipClass` | %129 | 32 | 3 (380, 4768, 6992) | 6,960 |
| `jitterZ` | %151 | 944 | 1 | 3,632 |
| `jitterY` | %147 | 720 | 1 | 3,808 |
| `jitterX` | %149 | 832 | 1 | 3,856 |

A segment-sum spill-weight estimate (`sum 2*freq/segment_len`, block
freqs propagated from the printed successor probabilities) ranks
`this` 0.396 > `race` 0.377 > `type` 0.374 > **`shipClass` 0.355** >
`mission` 0.268 > `jitterZ` 0.249 > `jitterY` 0.246 > **`jitterX` 0.238**.
`shipClass` outranks `jitterX`, yet greedy spills `shipClass` and keeps
`jitterX` in `fp`.  The spill is therefore decided by the
call-crossing/segment structure (the original keeps `shipClass` alive
across five calls with `mov sl,r5` / `mov r6,sl` splits), not by the
simple uses/length weight; without the original's PGO block weights the
committed shape cannot be nudged by source ordering alone.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11315
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11315/`: `gen.py` (body generator: coordmode/order/
position, status, jitter, case, mission knobs), `try.py` (splice + full
link + fn_diff + entry map), `sweep.py`, `sweep6.json`/`sweep6_out.json`
(64-variant grid), `weights.py` (pre-RA vreg interval/weight parser),
`mi.txt` (machineinstrs dump), `orig.txt`/`ours.txt`, `body_*.txt`.
