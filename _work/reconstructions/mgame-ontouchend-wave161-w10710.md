# MGame::OnTouchEnd — wave 161 (w10710): drop the stale `isDockingToStream` hint (raw 4387 -> 4125)

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10710`, base `01c7f92d6` (wave 160 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10710`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 160) | this wave |
|---|---:|---:|
| size | 8080 | **8080** (exact) |
| raw same-address diff | 4387 | **4125** (-262) |
| aligned (`align_diff.py`) | 901 | **905** (+4) |
| `.text` differing bytes | 85,518 | **85,256** (-262) |
| `.rel.plt` differing bytes | 0 | **0** (byte-identical) |
| byte_exact | 4507/4524 | **4507/4524** |
| linked_exact | 4507/4524 | 4507/4524 |

## 1. The landing

Wave 149 added `__builtin_expect(this->player->isDockingToStream(), 1)` as the
companion that absorbed the +4 B of the then-current **sepif** CFG.  Wave 158
folded the `hudTouchFlags == 0` arm back into the `else if` chain, so the sepif
+4 no longer exists and the hint is now a pure re-layout liability.  Removing it:

```diff
-                    } else if (__builtin_expect(this->player->isDockingToStream(), 1)) { // lint: arm_builtin ...
+                    } else if (this->player->isDockingToStream()) {
```

is semantically identical and restores the natural 50:50 dock-chain layout:
raw **4387 -> 4125** (-262), aligned 901 -> 905 (+4), `.text` 85,518 -> 85,256,
size 8080, `.rel.plt` **0**, `check` green (byte 4507, linked 4507, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## 2. The `.rel.plt` slot-swap investigation (mission target)

The mission was to keep w10610's late join (join at `0x17afe2`, join block
byte-identical) without swapping the `dockToStream`/`useCloak` PLT slots.
Measured on the wave-160 base:

| variant | raw | aligned | size | `.rel.plt` | PLT slots |
|---|---:|---:|---:|---:|---|
| base (wave 160) | 4387 | 901 | 8080 | 0 | correct |
| **landed: no dock hint** | **4125** | **905** | **8080** | **0** | **correct** |
| arm800 + truthy + `expect(won,1)` (w10610) | 4201 | 646 | 8080 | 22 | **swapped** |
| arm800 + truthy (no expect) | 6081 | 1891 | 8060 | 4 | swapped |
| arm800 + truthy + `expect(won,1)` + starMap cold hint | 4502 | 823 | 8080 | 18 | **correct** |
| arm800 + truthy + `expect(won,1)` + starMap cold + swapdecl | 4429 | 827 | 8080 | 18 | correct |
| arm200 + truthy (`won` truthy, no expect) | 6069 | 1871 | **8060** | **0** | **correct** |
| arm200 + truthy + `expect(won,1)` | 4751 | 834 | 8092 | 18 | correct |
| arm200 + truthy + starMap cold | 6084 | 1921 | 8060 | 0 | correct |

### What the late join actually is

`arm200 + truthy` reproduces the original's **whole** weapon-chain body order
and the late join exactly:

```
0x200 body -> won block -> starMap exit -> 0x800 body (useCloak) ->
maneuver -> pause menu -> setAutoPilotToProgrammedStation -> StarMap dtor ->
0x400 body -> join (ldr flags; cbz -> cold) -> hot tail -> epilogue -> cold body
```

`dockToStream` (starMap) therefore precedes `useCloak`, and `.rel.plt` is
byte-identical.  But it is **20 B short (8060)**: the `won`-eval block
(`keepObjectives` / `mission = ...`) lands *after* the cold body instead of
between the StarMap dtor and the `0x400` body, and the branch-width/`nop`
ledger loses exactly 20 B (4 `beq.w`->`beq.n`, 2 `nop`, 1 `b.w`->`b.n`, plus a
`mov r0,r5; blx isCampaignMission; cbz` block that the original emits and we
tail-merge — the original has 23 `isCampaignMission` calls, we have 22).
`expect(won,1)` restores the 20 B only when the `0x800` arm is first (where it
also swaps the PLT slots); in `arm200` order it adds 32 B (8092).

The starMap cold hint (`__builtin_expect(!starMap->OnTouchEnd(x,y), 0)`) moves
the starMap block ahead of the `0x800` body and repairs the PLT slots in
`arm800` order, but re-lays the hot region (4502/823).  No 8080 B shape with
both the late join and `.rel.plt` 0 was found inside the budget; the 20 B
`won`-eval placement is the remaining lever.

## 3. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10710/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4125 / 8080
python3 /dev/shm/w10710/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 905
python3 /dev/shm/w10710/relplt.py                                 # diff 0
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10710/{fdiff.py,align_diff.py,relplt.py,secdiff.py,
gen.py,batch.sh,measure.sh,blockseq.py,calls.py}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
