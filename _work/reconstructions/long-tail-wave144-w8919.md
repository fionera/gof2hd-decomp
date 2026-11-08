# Long-tail wave 144 (w8919): Gun / Generator / SpaceLounge —
# spill-weight ordering, boxW live-across-call, faithful-bonus frame objects

Status: **no landed `src/` change.** Worktree `wt/w8919`, base `f5093474f`
(wave 143). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave143-w8817.md`, `long-tail-wave142-w8718.md`,
`spacelounge-generator-wave133-w7740.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. Raw same-address diff bytes (fdiff):
Gun **463**, Generator **445**, SpaceLounge **175** (unchanged).

## 0. Targets attempted

| target | result |
|---|---|
| `SpaceLounge::drawLounge` (1948 B) | **new mechanism:** `pad` vs `centerX` is decided by greedy spill-weight ordering. `pad` w=1.915485e-4 (interval 6088) loses to `centerX/x` w=2.946140e-4 (interval 4220), so x takes r9 and pad falls to r11(fp). Shipped has pad=r9, x=r5. `boxW` frame-128 is because boxW is live across the `drawBox` call and gets a stack slot. |
| `Generator::createMission` (1996 B) | **new mechanism:** the faithful `bonus = rawBonus - rem` form's frame 104 is *two* extra live 4-byte frame objects (fi#5-12 live vs shipped's 6). `imageParts` hoist proves the agent spill is one of them (frame 96, 1996 B, 1174 raw). |
| `Gun::calcCharacterCollision` (1984 B) | confirmed the crossed slot pairing with frame-object evidence; new negatives only. |

## 1. SpaceLounge: pad vs centerX is a greedy spill-weight ordering

Instrumented-clang trace (`RA_TRACE=1`, `/tmp/llvm_android_702b/build2`) of the
committed source:

```
select %646 size=6088 w=1.915485e-04   ; pad   (ldr [layout,#148])
select %608 size=4220 w=2.946140e-04   ; centerX/x (COPY from s18)
FINAL  %646 -> $r11                    ; pad -> fp
FINAL  %608 -> $r9                     ; x   -> r9
```

Greedy processes the higher-weight interval first.  In the shipped function
pad=r9 and x=r5, i.e. the ordering is inverted.  r9 is free when pad is
allocated in our build, so the only lever is the priority (weight) order; the
register *number* is not the issue.  This explains why every previous source
spelling that touched only the clamp or the temp names failed.

`boxW` (`int boxW = pad*2 + width;` used in both drawBox and DrawRectangle) is
live across the `layout->drawBox(...)` call.  All callee-saved registers are
already allocated (r4 this, r5/r6 layout, r8 agent, r10 pad*2, r11 pad), so
boxW is forced to a stack slot: frame **120 -> 128** and linked **1948 -> 1964**
(1824 raw).  The original does not need a local: it computes `sl = pad*2` for
`top`, reuses `sl` in the clamp, then `add sl, r6` immediately before drawBox;
`sl` is callee-saved and survives the call, so DrawRectangle reuses it.

New negatives (all 1948 B unless noted):

| variant | raw | size |
|---|---:|---:|
| HEAD (`short pad3` clamp) | **175** | 1948 |
| plain clamp (`width + pad*2`) | 445 | 1948 |
| `int pad2 = pad+pad` reused top/clamp/box | 445 | 1948 |
| pad load moved before `centerX` / before `top` | 254 | 1948 |
| `unsigned pad` | 177 | 1948 |
| `const pad` / `const centerX` / `x` written directly | 175 | 1948 |
| `pad + pad + width` in drawBox/DrawRect | 175 | 1948 |
| `boxW` local (both call sites) | 1824 | **1964** |

Conclusion: to reach pad=r9 the source must raise pad's spill weight above
centerX's (or shorten centerX's interval) *without* moving the pad load or
introducing a cross-call local.  Nothing tried does both.

## 2. Generator: the faithful bonus form's +8 is two extra frame objects

Faithful `bonus = rawBonus - rem` (shipped `subne r5,r6,r0`) is 304 raw /
**2000 B**, frame **104**.  `-print-after-all` frame objects after
StackSlotColoring:

```
fi#0 size=4 ; fi#1..4 size=12 ; fi#5..12 size=4 (8 live) ; fi#13..15 dead
```

i.e. 4 + 48 + 32 = 84 bytes of live objects (frame 104 with alignment/pushes).
The shipped 96-byte frame implies only 6 live 4-byte objects: **two extra
4-byte slots** in ours, not one.  Hoisting `int *imageParts =
agent->getImageParts();` before the bonus block makes `agent` dead across the
`__aeabi_idivmod` and reaches **frame 96 / 1996 B** (1174 raw), confirming the
agent spill is one of the two.

Register story at the bonus block (shipped `0x9770c..0x97720`):

```
adds r5,r1,r6        ; bonus = rem + rawBonus
str  r1,[sp,#32]     ; spill rem into the (dead) status slot
...
ldr  r0,[sp,#32]
subne r5,r6,r0       ; bonus = rawBonus - rem
```

Ours evicts agent and keeps rem in r9 (`mov r9,r1` / `subne.w r6,fp,r9`), then
reloads agent.  Same crossed-priority pattern as SpaceLounge: the short-lived
`rem` outranks the long-lived `agent`.

New negatives:

| variant | raw | size | frame |
|---|---:|---:|---:|
| HEAD (`bonus -= rem*2`) | **445** | 1996 | 96 |
| faithful `bonus = rawBonus - rem` | **304** | **2000** | 104 |
| faithful + `Status *status = Globals::status` local | 1754 | **1980** | 96 |
| faithful + `int agentRace = agent->getRace()` cache | 694 | 2000 | 104 |
| faithful + `int bonus;` hoisted (no change) | 304 | 2000 | 104 |
| faithful + `imageParts` hoist before bonus | 1174 | **1996** | 96 |

## 3. Gun: crossed 12-byte/60-byte slot pairing, frame-object evidence

First real divergence at `0x15306c`:

```
orig: add.w r4, fp, fp, lsl #1     ; r4 = i*3
ours: add.w r5, fp, fp, lsl #1     ; r5 = i*3
orig: add r4, sp, #44              ; playerPosition sret
ours: add r4, sp, #104
orig: add r5, sp, #104             ; operator- sret
ours: add r5, sp, #44
```

Frame objects: `fi#1,2,3` are 12 B, `fi#10,11` are 60 B; StackSlotColoring
leaves all of them live in the frame-object list, so the 12-byte scatter temps
are coloured into the 60-byte slots.  Shipped pairs
`playerPosition -> 12B slot (sp+44)` / `operator- temp -> 60B slot (sp+104)`;
ours is crossed.  The r4/r5 swap is the same value rotation.

New negatives (all 1984 B unless noted):

| variant | raw | size |
|---|---:|---:|
| HEAD | **463** | 1984 |
| `velocity` declared before `position` | 478 | 1984 |
| `enemyPosition` before `position`/`velocity` | 546 | 1984 |
| `enemyPosition` first, then position/velocity | 546 | 1984 |
| `radius` copied after the scatter block | 463 | 1984 |
| `playerPosition` hoisted before `enemyPosition` | 1559 | **1964** |

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8919
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w8919/fdiff.py <mangled>                 # raw same-address diff
```

Scratch (not committed), `/dev/shm/w8919/`: `fdiff.py`, `regions.py`,
`cc_sl.sh` (exact per-TU compile), `sweep_sl{,2,3}.py`, `sweep_gen.py`,
`sweep_gun.py`, `SL.head.cpp` / `Gen.head.cpp` / `Gun.head.cpp`,
`greedy_sl.txt` / `vrr_sl.txt` / `gen_paa.txt` / `gun_paa.txt` (MIR dumps),
`trace_sl.txt` (RA_TRACE).

## 5. Files changed

`_work/reconstructions/long-tail-wave144-w8919.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
