# Long-tail wave 134 (w7762): Radar extab / ModStation / SpaceLounge — new
# BFI evidence, no source win

Status: **no landed `src/` change.** Worktree `wt/w7762`, base `ec0acea1e`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Extends `extab_lsda_scope_w7742.md`, `near-miss-wave133-w7741.md`,
`spacelounge-generator-wave133-w7740.md`.

Gate before/after: byte_exact **4506 -> 4506**, linked_exact **4506 -> 4506**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.
`.ARM.extab` differing bytes stay **1416 / 12 records**.

## 0. Targets attempted this wave

| target | result |
|---|---|
| `Radar::Radar` `.ARM.extab` (7 B, pure pad swap) | **new BFI data; no shape flips the two 519 pads** |
| `ModStation::ModStation` (5 insns) | faithful `far=pitch` re-measured 18 lines (baseline 6); select-canonicalisation sweep all 18 |
| `SpaceLounge::drawLounge` (95.7 %, 1948 B) | faithful clamp is *worse* raw (94.0 %); `boxW` variants 1968 B / 1940 B |
| `MGame::OnUpdate` `Globals::status` lever | `volatile` pointer cascades a type error through `Item.cpp`/`MenuTouchWindow.cpp`; reverted |
| `Gun`, `Generator`, `MenuTouchWindow` | not reached (time-boxed); prior notes stand |

## 1. Radar::Radar extab — the two 519 pads are frequency-tied, and the
##    tie is not branch-weight-sensitive

Instrumented the exact TU with
`-mllvm -print-machine-bfi -mllvm -print-machineinstrs` (NDK clang 7.0.2).
The two pads that swap are the blocks with `float ~= 1.156e-5`:

```
BB60  float = 1.1563e-05  int = 519   <- "outer" pad (rows 1,3,11,13,15)
BB87  float = 1.1568e-05  int = 519   <- "enemyGuns"/image-group pad (rows 19,21)
Min   BB195 float = 1.7815e-07 int = 8   (scale = 8/Min = 44,907,140)
```

`MachineBlockPlacement::selectBestCandidateBlock` keeps the **last** minimum,
so with equal ints the later worklist block (BB87) wins and is placed at the
lower offset (2462). The original has BB60 at 2462. To flip we need
`int(BB60) > int(BB87)`, i.e. the outer float sum must rise by ~0.14 %
(519.3 -> 520) or the inner sum fall by ~0.09 %.

**New negative result:** the pad frequencies are essentially invariant to every
branch-weight / scope spelling tried. Measured with the BFI screen
(`/dev/shm/w7762/try_radar.py`, `try_radar2.py`; exact per-TU flags):

| variant | pads (float, int) |
|---|---|
| HEAD | 60:(1.1563e-5,519) 62:(8.868e-6,398) 73:(1.4113e-5,633) 87:(1.1568e-5,519) |
| `if ((int)enemyGuns != 0)` | identical |
| `if (enemyGuns)` | identical |
| `if (enemyGuns != 0)` | identical |
| `reinterpret_cast<uintptr_t>(enemyGuns) != 0` | identical |
| playerGuns/enemyGuns `i != n`, `n > i`, `i += 1`, `while` | identical |
| `if (Array<...> *playerGuns = getPlayerGuns())` (init-statement) | identical |
| `if (Array<...> *enemyGuns = getEnemyGuns())` (init-statement) | pad 87 -> 77 (1.2071e-5, 542) |
| compound block around decl+if+image group | pad 87 -> 77 |

The reason is structural: the image group after the `if` is **unconditional**,
so its invoke blocks are frequency 1.0 regardless of the null-check weight. The
0.005 difference between the two sums is not reachable through a branch weight.

Full-link check of the hinted "init-statement / compound-statement" shapes
(`/dev/shm/w7762/try_radar_full.py`, decode via
`_work/reconstructions/extab_lsda_decode.py`):

| shape | extab row diffs | body |
|---|---:|---|
| compound around image group (HEAD-equivalent) | 7 | byte-exact |
| `if (Array<...> *enemyGuns = ...)` init-statement | 11 | 14 B diff |
| init-statement + compound closing after image group | 11 | 14 B diff |

So the init-statement shape *does* change the pad structure, but in the wrong
direction and it costs the byte-exact body. The compound shape is already the
committed one (7 rows).

Conclusion: the remaining lever is the **worklist/IR block order** of the two
`b %cleanup` trampolines, not their frequency. Both are `b.n 12de28` and
byte-identical, so only the extab rows can change. No body-preserving source
shape found this wave.

## 2. ModStation::ModStation — faithful `far=pitch` is still 18 lines

Re-measured the committed baseline (`far = yaw`, `short pad3`): **6 structural
diff lines**. Faithful `far = pitch` (`MatrixSetRotation(farKey, pitch, ...)`):
**18 lines**, 1008 B. The residual is the near `yaw` load / race spill schedule
plus the `Pitch, Yaw, YawIPad` vs `Pitch, YawIPad, Yaw` literal-pool order.

New variants (all 1008 B, all 18 lines unless noted):

* select spelled `!iPad ? Yaw : YawIPad`, `iPad == 0 ? ...`, `iPad != 0 ? ...`,
  `(iPad == 0) ? ...` — identical
* `if (iPad) yawTable = YawIPad; else yawTable = Yaw;` — identical
* `&gHangarCamYawIPad[0]`, `gHangarCamYawIPad + 0` — identical
* `yaw`/`pitch` declaration order swapped, `pitch` loaded before `yaw` — identical
* `far` y-arg reused from the local `yawTable` — 271 lines, 984 B (wrong)
* `volatile float yaw` — 248 lines

The emitted IR select is **identical** in the baseline and the faithful shape
(`select i1 %99, [10 x float]* @gHangarCamYaw, [10 x float]* @gHangarCamYawIPad`),
so the pool-order difference is a DAG `NodeQueueId` tie in the pre-RA
scheduler, not a select-operand-order fact. The committed baseline (6 lines)
remains the best shape.

## 3. SpaceLounge::drawLounge — faithful clamp is worse by the official metric

The committed source uses `short pad3 = pad` and `width + pad3 * 2`, which emits
the 3-instruction sign-extend clamp
(`movgt.w r1,pad,lsl#16; addgt.w r1,width,r1,asr#15; subgt.w x,w-inset,r1`).
The **original** actually uses the 2-instruction int form
(`itt gt; addgt.w r1, width, sl; subgt x, w-inset, r1`, `sl = pad*2`), so the
source comment claiming a 16-bit sign-extend is wrong.

Measured (`/dev/shm/w7762/try_sl.sh`, full link + verify):

| variant | size | structural lines | raw |
|---|---:|---:|---:|
| HEAD (`short pad3`) | 1948 | 189 | **95.676 %** |
| faithful clamp (`width + pad * 2`) | 1948 | 179 | 94.0 % |
| `int boxW = pad*2+width;` before `drawBox` | **1968** | 619 | — |
| `int boxW = width + pad*2;` before `x` | **1940** | 233 | — |
| `const int boxW`, `width + (pad<<1)` before `x` | **1940** | 233 | — |
| `int pad2 = pad*2;` used in both places | 1948 | 179 | — |

So the faithful clamp is *semantically* right and has fewer structural lines,
but the official byte percentage drops (94.0 vs 95.7) because the register swap
(`fp` vs `r9` for `pad`) cascades through the whole function. The `boxW` shapes
that reach `pad = r9` are not size-exact (1968 / 1940). Committed source
unchanged.

## 4. MGame::OnUpdate — `Globals::status` volatile attempt

Tried `static Status *volatile status;` in `src/game/core/Globals.h` (the
wave-129 candidate #1). It cascades: `Globals.cpp` redefinition, `Item.cpp`'s
`static Status *&status`, and `MenuTouchWindow.cpp`'s `Status *const *`
initialisers all need `volatile` qualifiers, and the change would touch all 331
`Globals::status` sites. Reverted before a build; no gate impact.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7762
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                       # extab 1416 B / 12 records
```

Scratch (not committed), `/dev/shm/w7762/`:
`cc_radar.sh` (exact per-TU flags), `try_radar.py` / `try_radar2.py`
(BFI pad screen), `try_radar_full.py` (full-link extab row count),
`bfi_pads.py`, `bodycmp.py`, `try_mod.sh` + `gen_mod*.py` (ModStation),
`try_sl.sh` + `gen_sl*.py` (SpaceLounge).

## 6. Files changed

`_work/reconstructions/long-tail-wave134-w7762.md` only.
