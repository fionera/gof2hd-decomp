# ModStation constructor — wave 191b (w13586): ISel LDRLIT-order root cause

Worker `w13586`, branch `wave191b-w13586`, base master `2a2fcd43e`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13586`.
Predecessor notes: `modstation-ctor-wave191b-w13550.md` (11-byte plateau) and
`modstation-ctor-wave191b-w13575.md` (LDRLIT-order sweep).  Read both first.

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no raw win found.**  Tree clean,
`check` green at entry counts (byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.rel.plt` byte-identical, `.ARM.extab` 577).

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

HEAD form (unchanged):

```cpp
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
float yaw   = *(const float *) &pitchBits;   // yaw   == pitchBits
float pitch = *(const float *) &yawBits;     // pitch == yawBits
MatrixSetRotation(nearKey, yaw, pitch, 0.0f, (RotationOrder) 2);
...
MatrixSetRotation(farKey, pitch, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                  (RotationOrder) 2);
```

`fn_diff --json` classifies the 11 bytes as exactly five categories:
`opcode:ldr.w/str.w` (1), `extra-ours:ldr.w` (1), `extra-orig:str.w` (1),
`extra-ours:mov` (1), `extra-orig:mov` (1) — i.e. one 3-instruction schedule
rotation plus the `mov r3,r4` / `mov r4,r2` substitution.  The instruction
multiset (374) and size (1008) are identical.

## Root cause is the **ISel** LDRLIT order, not the pre-RA scheduler

This pass pinned down *where* the pool order is decided.  Dumping
`-mllvm -print-machineinstrs` for the faithful form (`near(pitch,yaw)`,
`far(pitch,...)`) and for the landed form shows the order is already fixed at
`# After Instruction Selection:` and is *preserved unchanged* by
`# After Machine Instruction Scheduler:` (pre-RA, `list-hybrid`).

Faithful, at ISel (`mir_all.log`, function `_ZN10ModStationC2Ev`):

```
%115 = tLDRLIT_ga_pcrel @_ZL15gHangarCamPitch
%117 = tLDRLIT_ga_pcrel @_ZL13gHangarCamYaw
%118 = tLDRLIT_ga_pcrel @_ZL17gHangarCamYawIPad
%121 = t2MOVCCr %118:tgpr, killed %117:tgpr, 0, $cpsr   ; base=YawIPad, cond=Yaw
```

Landed, at ISel (`mir_landed.log`):

```
%116 = tLDRLIT_ga_pcrel @_ZL15gHangarCamPitch
%117 = tLDRLIT_ga_pcrel @_ZL17gHangarCamYawIPad
%118 = tLDRLIT_ga_pcrel @_ZL13gHangarCamYaw
%121 = t2MOVCCr %117:tgpr, killed %118:tgpr, 0, $cpsr   ; base=YawIPad, cond=Yaw
```

The `MOVCCr` operand order is **identical** in both (`base=YawIPad`,
`cond=Yaw`, so the register roles are `r3=YawIPad`, `r4=Yaw` in both, which is
what the original has).  Only the *creation/emission order of the two
`LDRLIT`s* differs, and that order is what sets the literal-pool order
(`Pitch, YawIPad, Yaw` vs `Pitch, Yaw, YawIPad`).

The only IR difference between the two forms is the far call's first float
operand (`%108` yawBits in landed vs `%107` pitchBits in faithful).  That one
use extends `pitchBits`' live range across the near call and changes the ISel
scheduler's tie-break for the two independent `LDRLIT`s.  So the pool order
and the live-out register (`r4`) are coupled through the same operand: there is
no source shape that keeps `pitchBits` live-out *and* emits the `YawIPad`
`LDRLIT` first, because both are decided by the same DAG.

## New source shapes measured this pass (all size-exact 1008 B)

`measure.sh` = `link_so -j2` under `ulimit -v 12000000` + `tools/fn_diff.py`.

| shape | raw / aligned | note |
|---|---:|---|
| HEAD landed form | **11 / 5** | floor |
| faithful `far(pitch)` (all spellings) | 43 / 12 | pool flips |
| `iPad ? Yaw : YawIPad` (semantically swapped select) | 43 / 12 | see below |
| `(iPad ^ 1) ? Yaw : YawIPad` | 44 / 13 | `cmp #1` |
| inline `gHangarCamPitch[race]` tables (both) | 698 / 52 (1000 B) | size-fatal |
| inline pitch table only | 46 / 13 | |
| yaw load first | 752 / 40 (1000 B) | size-fatal |
| `const int` bits / `const float` / `const float&` / `float&` | 43 / 12 | same IR |
| `decl_then_assign` (`float pitch, yaw; pitch = ...; yaw = ...`) | 43 / 12 | same IR |
| `ROTATION_ORDER_YXZ` enum / `static_cast` / `0.0` | 43 / 12 | same IR |
| `Vector rot(pitch,yaw,0)` temp, then `rot.x`,`rot.y` | 43 / 12 | same IR |
| far arg1 = fresh `*(float*)&pitchTable[race]` reload | 43 / 12 | CSE'd back |
| far arg1 = `*(float*)&pitchBits` inline | 43 / 12 | same IR |
| far arg1 = `farPitch` local copy after near call | 43 / 12 | same IR |
| near args inline bitcasts, far `pitch` | 43 / 12 | same IR |
| `union { int i; float f; }` bits | 43 / 12 | same IR |
| `__builtin_memcpy` into int / into float | 667 / 59 | `vldr`/`vmov` shape |
| plain `float pitch = gHangarCamPitch[race]` | 667 / 59 | `vldr s0`+`vmov` |
| far arg1 = `yaw` local (landed, far=yawBits) | 43 / 12 | = faithful liveness |
| far arg2 = `yaw` local / `yawTable[race]` local | 604 / 71 (984 B) | size-fatal |
| `if (yaw != yaw) pitch = yaw;` after near | 710 / 107 (1032 B) | size-fatal |

### `sel_swapwrong` — correct pool *and* `mov r4,r2`, but roles flip

`yawTable = Globals::iPad ? gHangarCamYaw : gHangarCamYawIPad` (semantically
swapped) produces IR `select(eq, YawIPad, Yaw)` (verified with a minimal
`-emit-llvm` probe: `iPad ? Yaw : YawIPad` is the only spelling that yields
this operand order).  It gives the original's pool order
(`Pitch, YawIPad, Yaw`) and `mov r4,r2`, but the register roles swap
(`r4=YawIPad`, `r3=Yaw`) and the `add`/`ldrb`/`mov r0,r3` order changes, so it
still measures **43 / 12**.  `(iPad ^ 1) ? Yaw : YawIPad` is semantically
correct and yields `select(icmp eq 1, YawIPad, Yaw)` — same roles, but
`cmp r1,#1` instead of `cmp r1,#0`, measuring **44 / 13** (the eq1 variant).

## Sweep of `-pre-RA-sched` (manual compile + `linkcmd.sh`)

`-pre-RA-sched` also drives the ISel scheduler, so it changes the whole
function.  With the faithful source:

| scheduler | raw / aligned | size |
|---|---:|---:|
| `list-hybrid` (= default) | 43 / 12 | 1008 |
| `list-ilp` | 836 / 304 | 1008 |
| `list-burr` | 880 / 261 | 984 |
| `source` | 901 / 313 | 984 |

With the landed source: `list-hybrid` 11, `list-ilp` 833, `list-burr` 886,
`source` 901.  No flag combination reaches 0, and any non-default scheduler
regresses the rest of the function.

## Conclusion

The 11-byte residual is a single ISel/DAG tie: the original keeps
`pitchBits` live across the near call (giving `mov r4,r2`) *and* emits the
`YawIPad` LDRLIT before the `Yaw` LDRLIT (giving pool `Pitch, YawIPad, Yaw`).
Both facts are decided by the same far-call operand in clang 7.0.2, so every
source shape that fixes one breaks the other.  The landed 11 form trades the
semantics of the far-key pitch (it passes `yawBits`, not `pitchBits`) for the
correct pool order and is the measured floor.  No source change landed; the
tree is clean and `check` is green.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
cmake --build cmake-build-match --target check -j2
```
