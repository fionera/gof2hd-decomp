# MGame::OnTouchEnd — wave 161 (w10711): mining-refine `count > 0`; `won`/`keepObjectives` materialisation sweep

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10711`, base `01c7f92d6` (wave 160 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10711`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 160) | this wave |
|---|---:|---:|
| size | 8080 | **8080** (exact) |
| raw same-address diff | 4387 | **4389** (+2) |
| aligned (`align_diff.py`) | 901 | **902** (+1) |
| `MGame::dockEvent` byte diff | 0 | **0** (1300 B) |
| `.rel.plt` diff | 0 | **0** (29,392 B byte-identical) |

Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS.  `.text` diff 85,518 -> **85,520** (+2), `.ARM.extab` 1,258
(unchanged), `.rodata`/`.data`/`.bss`/`.ARM.exidx`/`.rel.plt` byte-identical.

## 1. Landed: `count > 0` (the only size-exact correct mining-refine spelling)

The original skips the report when `amount == 0` (`cmp r9,#0; beq` at
`0x17a5ca`); HEAD's `count >= 0` is `amount <= 0` and emits `cmp r9,#0; bgt`
(i.e. it never skips).  `count > 0` is the semantically correct spelling and
is size-exact:

```diff
-                        if (count >= 0) {
+                        if (count > 0) {
```

It emits `cmp.w r9,#-1; bgt.n` (the backend canonicalises `count > 0` to
`amount < 0`), i.e. the branch is still `bgt`, not the original `beq`, and the
immediate is `-1` instead of `0` -> **+2 raw**.  This is the only correct
spelling that stays at 8080 B and keeps `.rel.plt` byte-identical.

## 2. Why no correct `!= 0` spelling is size-exact

The size is governed by the branch weight, not by LSR.  `count != 0` /
`amount != 0` canonicalise to an `icmp eq X, 0`, which
`BranchProbabilityInfo::calcZeroHeuristics` weights 20:12 (unlikely); the
block layout then re-lays the whole function to **8060 B** (raw 6590,
aligned 1835, `.rel.plt` 0).  `-mllvm -disable-lsr` still gives 8060 with
`cmp #0; beq`, confirming the weight is the cause.  The only static 50:50
escape is the AND exception, which needs a boolean operand, so it cannot
apply to the integer `count`.

Measured mining spellings (all on the wave-160 base):

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| `count >= 0` (base, wrong) | 4387 | 901 | 8080 | 0 |
| **`count > 0` / `count >= 1` (landed)** | **4389** | **902** | **8080** | **0** |
| `count != 0`, `amount != 0`, `~amount+1 != 0` | 6590 | 1835 | 8060 | 0 |
| `count >= 0 && count != 0` (and 9 other `&&` forms) | 4389 | 902 | 8080 | 0 |
| `count >= 0 && amount >= 0` / `count <= 0` | 6528 | 2060 | 8076 | 0 |
| separate `count++` loop counter | 6590 | 1835 | 8060 | 0 |
| `count != 0 && refinedAny` | 7488 | 2381 | 8008 | 0 |

## 3. `won` / `keepObjectives` materialisation

`keepObjectives` is already exact: every spelling (`!= false`, truthy,
`== true`, `!= 0`, named `bool campaignWon`) gives the identical
`cmp #0; beq` + `eor sl,r0,#1` + `mov sl,#0` and leaves raw at 4387/901,
8080 B, relplt 0.

The `won` guard is the residual.  Original (`0x17adda`): `cmp r0,#0; beq.w`
(body fall-through, else far).  HEAD (`== true`): `cmp r0,#1; bne.w`
(50:50, body fall-through, 8080 B, relplt 0).  The truthy `A || B` form gives
the original `cmp #0; beq` but the 20:12 weight re-lays the function to
8060 B (raw 6069, aligned 1871).  The `(B & 1) != 0` AND form recovers 50:50
and 8080 B (`lsls r0,#31; bne.n body`, else fall-through) but the inverted
block order puts `LevelScript::resetStartSequenceOver`'s `bl` before
`MGame::useCloak`'s, rotating four `.rel.plt` slots.

Measured `won` spellings:

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| `B == true` (base) | 4387 | 901 | 8080 | **0** |
| `(B & 1) != 0` (AND) | **4367** | 902 | 8080 | 9 |
| AND + starMap cold hint | 4375 | 956 | 8080 | 7 |
| AND + star + `expect(hudTouchFlags==0,1)` | 4375 | 956 | 8080 | 7 |
| truthy `A \|\| B` / `!= 0` / named bool / ternary | 6069 | 1871 | 8060 | 0 |
| `(B+1)==1`, `(B-1)==-1`, `(B<<1)==0`, `B*2==0`, `B&0xff==0` | 4774 | 1226 | 8092 | 4 |
| `(B<<8)&0x100==0`, `B\|1==1`, `B\|1!=1` | 4387 | 901 | 8080 | 0 |
| `expect(won,1)` on AND | 4752 | 835 | 8092 | 18 |
| `expect(won,0)` on AND | 5146 | 2162 | 7832 | 24 |
| `(A\|\|B)&1` and all `(A\|\|B)` spellings | 6069 | 1871 | 8060 | 0 |

The `&1` form is a **-20 raw** win but cannot be landed under the
`.rel.plt`-byte-identical invariant: the four rotated slots are
`useCloak` / `maneuverTouchEnd` / `isInstantActionMission` /
`resetStartSequenceOver` (our order puts `resetStartSequenceOver` first,
`0x683a0`).  The starMap cold hint repairs only `dockToStream` (7 bytes
remain).  `-20 raw` is the ceiling if the invariant is relaxed.

### Merge combinations (for the coordinator)

| combination | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| `count > 0` only (landed) | 4389 | 902 | 8080 | 0 |
| AND `won` + `count > 0` | 4369 | 903 | 8080 | 9 |
| AND + star + `count > 0` | 4377 | 957 | 8080 | 7 |
| arm-824 + truthy + `expect(won,1)` (w10610) | 4201 | 646 | 8080 | 22 |
| arm-824 + AND + `expect(won,1)` + star | 4502 | 823 | 8080 | 18 |

## 4. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10711/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4389 / 8080
python3 /dev/shm/w10711/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 902
python3 /dev/shm/w10711/relplt.py                                # diff 0
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10711/{fdiff.py,align_diff.py,measure.py,measure2.py,
measure3.py,relplt.py,relcmp.py,sectdiff.py,batch.py,MGame.base.cpp}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
