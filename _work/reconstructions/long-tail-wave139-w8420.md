# Long-tail wave 139 (w8420): Gun / Generator / SpaceLounge / Radar extab —
# new branch-weight and slot-merge probes, no landed `src/` change

Status: **no landed `src/` change.** Worktree `wt/w8420`, base `bf123bd47`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave138-w8320.md`, `long-tail-wave137-w8220.md`,
`gun-calccharactercollision-wave133-w7739.md`,
`spacelounge-generator-wave133-w7740.md`,
`extab_lsda_scope_w7742.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.text` 85,476 differing bytes,
`.ARM.extab` 1,396 (unchanged). Raw same-address diff bytes (fdiff):
Gun **463**, Generator **445**, SpaceLounge **175**, Radar extab **7**.

## 0. Targets attempted

| target | result |
|---|---|
| `Gun::calcCharacterCollision` (1984 B) | **new negatives:** `expr` form 467 raw; source block swap (scatter before geometries) 463 raw (identical to HEAD); named k-loop `Vector kpos` at function/e-loop/scatter scope 1681-1791 raw |
| `Generator::createMission` (1996 B) | **new:** `__builtin_expect((type\|4)==12, 1)` reaches the *exact* shipped bonus block (`str r1,[sp,#32]`, `subne r5,r6,r0`, frame 96) at 1996 B, but the branch bias reshuffles the whole function (raw 1801, fuzzy 77.2). `volatile int bonus2` also reaches the block at 2000 B (raw 592) |
| `SpaceLounge::drawLounge` (1948 B) | **new:** `short pad` + `boxW` combinations reach 1948 B but with pad=sl/x=r6 (raw 1188); no shape with pad=r9 at 1948 |
| `.ARM.extab` `Radar::Radar` (7 B) | **new:** `eg_if_comp`, `pg_if_comp`, `empty_comp` are all body-exact ties at 7; `both_one_scope` 30/body 76; no flip |

## 1. Gun: the LICM hoist order is the shipped-vs-ours split

The w7739/w8320 conclusion is confirmed and sharpened with the pre-LICM MIR.

`-mllvm -print-after-all` pre-LICM (`Remove dead machine instructions`) for the
current `stmt` form, bb.18 (the scatter block):

```
%114 = t2ADDri %stack.10   ; pp getPosition sret   (fi#10)
...
%115 = t2ADDri %stack.11   ; delta operator- sret  (fi#11)
```

i.e. **pp's add precedes delta's in the source block**.  After `Early Machine
LICM` the i-loop preheader bb.5 contains:

```
%308 = t2ADDri %stack.10   ; fi#10  (hoisted first)
%282 = t2ADDri %stack.11   ; fi#11
```

Greedy RA walks in start order, so fi#10 -> r4, fi#11 -> r5.  In the `stmt`
form fi#10 = pp (sp+104), fi#11 = delta (sp+44): shipped registers (pp r4,
delta r5) but shipped-swapped slots.  In the `expr` form fi#10 = delta
(sp+104), fi#11 = pp (sp+44): shipped slots but swapped registers.

The reason the k-loop add wins the hoist race: `MachineLICM.cpp:1218`
(`IsProfitableToHoist`) returns **true unconditionally** for
`TII->isTriviallyReMaterializable(MI, AA)`, before the register-pressure
check, so the `t2ADDri %stack.10` in the k-loop body (bb.81) is hoisted to
bb.5 even though the MST add lives in the earlier block bb.80.  The shipped
build has **no** hoisted stack-address adds at all.

### New variants measured this wave (all 1984 B)

| variant | raw diff | notes |
|---|---:|---|
| HEAD (`stmt`) | **463** | pp r4/sp+104, delta r5/sp+44 |
| `expr` (`VectorLength(enemyPosition - getPosition())`) | 467 | shipped slots, swapped regs |
| source block swap: `if (scatter)` before `if (explosive)…else if (geometries)` | 463 | identical raw to HEAD; block order changed but hoist unchanged |
| `Vector diff = enemyPosition - playerPosition; VectorLength(diff)` | 463 | = HEAD |
| named `Vector kpos` in k-loop, function scope | 1790 | whole body re-rolls |
| named `Vector kpos` in e-loop scope | 1790 | same |
| named `Vector kpos` in scatter scope | 1681 | same |
| `expr` + function-scope `kpos` | 1791 | same |

The `kpos` experiments were meant to give the k-loop sret its own frame slot
(so it would not merge into fi#10 and would not be the first hoisted add), but
the named local changes the whole i-loop layout instead.  No shape found that
keeps 1984 B and swaps the merge/hoist.

## 2. Generator: the exact shipped block *is* reachable, but only by biasing
##    a branch that reshuffles the function

The shipped block (`0x9770c..0x97722`) is:

```
adds  r5, r1, r6        ; bonus = rem + rawBonus
str   r1, [sp, #32]     ; spill rem
movs  r1, #50
mov   r0, r5
blx   __aeabi_idivmod
ldr   r0, [sp, #32]     ; reload rem
cmp   r1, #0
it    ne
subne r5, r6, r0        ; bonus = rawBonus - rem
str   r5, [sp, #16]     ; bonus -> sp+16
ldr   r5, [sp, #24]
```

The faithful `bonus = rawBonus - rem` shape keeps `rem` in r9 (frame 104) —
the greedy allocator evicts `agent` (%659, weight 9.346e-04) from r9 for the
split range of `rem` (%718, weight 9.804e-04).  The eviction is accepted
because rem's weight is slightly higher.

**New positive (shape only):** biasing the `(type | 4) == 12` branch cold

```c++
if (__builtin_expect((type | 4) == 12, 1)) {
    bonus = 0;
} else {
    ... faithful bonus block ...
}
```

lowers the bonus block's frequency below `agent`'s weight, so the RA spills
`rem` to sp+32 and keeps `agent` in r9.  The emitted block is **byte-for-byte
the shipped block** (`str r1,[sp,#32]` / `subne r5,r6,r0` / `str r5,[sp,#16]`
/ `ldr r5,[sp,#24]`), frame 96, size **1996 B**.  But the `__builtin_expect`
also changes MachineBlockPlacement: the whole function is reshuffled from the
prologue (`mov r9,r1` -> `mov r6,r1`), giving raw diff **1801** and fuzzy
**77.2 %** (vs HEAD 97.2 %).  Not landable.

Other new shapes:

| variant | size | block | raw diff |
|---|---:|---|---:|
| HEAD (`bonus -= rem*2`) | 1996 | bonus r6, rem r5 | **445** |
| faithful `bonus = rawBonus - rem` | 2000 | rem r9 | — |
| `volatile int bonus2 = rawBonus + rem; bonus = bonus2;` + faithful | 2000 | exact block + 2 volatile moves | 592 |
| `__builtin_expect((type\|4)==12,1)` + faithful | **1996** | **exact block** | 1801 |
| `__builtin_expect(agent->getOffer()==5,0)` + faithful | 1968 | exact block | — |
| `__builtin_expect(type==8,1)` + faithful | 2012 | exact block | — |

So the target block is reachable at the right size; the remaining blocker is
that the only source-level weight lever (`__builtin_expect`) also perturbs
block placement.  A weight-only lever that does not affect layout was not
found.

## 3. SpaceLounge: `short pad` + `boxW` reaches 1948 but not pad=r9

w7740's `boxW` lever (pad=r9, x=r5) still costs 20 B because the whole
function's r8/r9/sl/fp packing rotates (frame 120 -> 128).  New combinations:

| variant | size | pad | x | raw diff |
|---|---:|---|---|---:|
| HEAD (`short pad3`) | **1948** | fp | r9 | **175** |
| `int boxW = pad*2+width` before drawBox, faithful clamp | 1968 | r9 | r5 | — |
| `short pad` + `int boxW`, faithful clamp | **1948** | sl | r6 | 1188 |
| `short pad` + `int boxW`, HEAD `pad3` clamp | 1948 | sl | r6 | — |
| `short pad` + `int boxW`, `(short)pad*2` clamp | 1948 | sl | r6 | — |
| `int boxW`, HEAD `pad3` clamp | 1964 | r5 | r2 | — |
| `int boxW`, `(short)pad*2` clamp | 1964 | r5 | r2 | — |

The 1948-B shapes all land pad in `sl` (r10) and x in r6, and the raw diff
explodes to 1188.  The target colouring (pad=r9, x=r5) is only reachable with
the 1968-B `int boxW` shape.

## 4. `.ARM.extab` Radar::Radar: the 7-byte pad swap is a hard tie

The body is byte-exact (`_ZN5RadarC2EP5Level` raw diff 0); only the two EH
pads are swapped (outer 2462 vs 2464, enemyGuns 2464 vs 2462).  New shapes
(all body-exact, `extab_lsda_decode.py` diffbytes):

| shape | extab diff |
|---|---:|
| baseline | **7** |
| `if (Array<...> *playerGuns = level->getPlayerGuns())` init-statement | 7 |
| compound around playerGuns decl+if | 7 |
| compound around the playerGuns `if` only | 7 |
| compound around the enemyGuns `if` only (`eg_if_comp`) | 7 |
| empty compound `{ }` after the enemyGuns block | 7 |
| single compound around both loops (`both_one_scope`) | 30 (body 76) |
| compound around enemyGuns decl+if (`eg_comp2`) | 13 (body 14) |
| enemyGuns init-statement | 13 (body 14) |
| both init-statements | 30 (body 76) |
| enemyGuns declared before playerGuns | 61 (body 1676) |
| enemyGuns declared early, assigned later | 25 (body 95) |

The tie is the `MachineBlockPlacement::selectBestCandidateBlock` last-minimum
rule documented in w7742; every body-exact shape leaves the outer pad later
in the worklist than the enemyGuns pad.  No body-exact shape flips it.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8420
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w8420/fdiff.py <mangled>                 # raw same-address diff
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                         # extab 1396 / Radar 7
```

Scratch (not committed), `/dev/shm/w8420/`: `fdiff.py` (worktree paths),
`show.sh`, `dis_orig.sh`, `cc_{sl,gen,gun,radar}.sh` (exact per-TU compile),
`sl_screen.sh`, `sl_batch*.py`, `gen_batch*.py`, `gun_batch*.py`,
`radar_batch*.py`, `radar_try.sh`, plus the variant `.cpp` files.
MIR evidence: `after_gun.txt` / `after_gun_expr.txt`
(`-mllvm -print-after-all`), `after_faithful.txt`, `ra_gen.txt`
(`RA_TRACE=1` instrumented clang `/tmp/llvm_android_702b/build2/bin/clang++`).

## 6. Files changed

`_work/reconstructions/long-tail-wave139-w8420.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
