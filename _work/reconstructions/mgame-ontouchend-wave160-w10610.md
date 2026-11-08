# MGame::OnTouchEnd — wave 160 (w10610): arm order `0x800,0x200,0x400` + truthy `expect(won,1)` lands the exact join at 8080 B

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10610`,
base `08af4633b` (wave 159 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10610`. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 159) | this wave |
|---|---:|---:|
| size | 8080 | **8080** (exact) |
| raw same-address diff | 4387 | **4201** (-186) |
| aligned (align_diff) | 901 | **646** (-255) |
| join address | 0x17aeb2 | **0x17afe2** (orig 0x17afe2) |
| epilogue | 0x17ba18 | **0x17b004** (orig 0x17b004) |
| byte_exact | 4507 | **4505** (PLT side effect, see §4) |
| linked_exact | 4507 | 4507 |

## 1. The winning source edit

Two edits on the wave-159 base:

1. **Arm order `0x800, 0x200, 0x400`** in the `if (this->hudMenuOpen)` weapon/utility
   chain (the `0x800` useCloak arm moved to the front).
2. **Truthy `won` + `__builtin_expect(won, 1)`**:
   ```cpp
   bool won = Globals::status->getMission()->hasWon() ||
              ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon();
   if (__builtin_expect(won, 1)) { // lint: arm_builtin anchor: MGame::OnTouchEnd; branch-weight hint
   ```

The truthy spelling makes the guard `cmp r0,#0; beq.w` (the original's form); the
`expect(won,1)` hint is what moves the hot tail inline after the join.

## 2. The join block is now byte-identical

Original and ours at `0x17afe2` (only the PC-relative literal offset differs, because
our literal pool sits 12 B earlier):

```
17afe2: f8db 00f8   ldr.w  r0, [fp, #248]      ; hudTouchFlags
17afe6: b1c0        cbz    r0, 17b01a          ; flags==0 -> cold body (after epilogue)
17afe8: f8da 0000   ldr.w  r0, [sl]
17afec: f6eb ec16   blx    PlayerEgo::isAutoPilot
17aff0: f080 0001   eor.w  r0, r0, #1
17aff4: ea50 0008   orrs.w r0, r0, r8
17aff8: bf02        ittt   eq
17affa: 48ef/48ec   ldreq  r0, [pc, #956/#944]
17affc: f04f 517e   moveq.w r1, #0x3f800000
17b000: e9cb 106c   strdeq r1, r0, [fp, #432]
17b004: 9832        ldr    r0, [sp, #200]     ; epilogue
```

The `0x400` arm (`ldrd r0,r2,[fp,#116]; movs r1,#2; blx initHudMenu`) is placed
immediately before the join, exactly as in the original; the cold body follows the
epilogue (`0x17b01a`), also exactly as in the original. The `0x800` arm is at
`0x17adc0` (original `0x17ae8c`) and the `0x200` arm at `0x17aea4` (original
`0x17adc0`): the two arm *bodies* are swapped relative to the original, but the
tail is identical.

## 3. Measured sweep (all on the wave-159 base)

`raw` = `fdiff.py`; `aligned` = `align_diff.py`; `size` = ours.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (`== true`, arm 284) | 4387 | 901 | 8080 |
| **arm 824 + truthy + expect(won,1)** | **4201** | **646** | **8080** |
| arm 824 + `== true` + expect(won,1) | 4202 | 647 | 8080 |
| arm 824 + truthy + expect(won,1) + ehot=1 | 4333 | 681 | 8076 |
| arm 824 + truthy + expect(won,1) + ecold=0 | 4379 | 819 | 8084 |
| arm 284 + truthy + expect(won,1) | 4751 | 834 | 8092 |
| arm 284 + truthy + expect(won,1) + ecold=0 | 6031 | 939 | 8080 |
| arm 248 + truthy + expect(won,1) | 4759 | 857 | 8092 |
| arm 482 + truthy + expect(won,1) | 4755 | 839 | 8092 |
| arm 842 + truthy + expect(won,1) | 4204 | 650 | 8080 |
| arm 824 + store `hudMenuOpen=0` after initHudMenu(2) | 4022 | 762 | 8092 |

Only arm orders beginning with `0x800` (824/842) are size-exact; they are the ones
that place the hot tail inline. The w10511 `store` hack is not needed.

## 4. The one open issue: `dockToStream`/`useCloak` PLT slot swap

Putting the `0x800` arm first moves the first `useCloak` call to `0x17adda`, which is
*before* the first `dockToStream` call at `0x17ae2e` (the starMap-exit branch). Gold
therefore gives `useCloak` the earlier `.plt` slot:

| | original | ours (arm 824) |
|---|---|---|
| `dockToStream` PLT | 0x683a0 | **0x683ac** |
| `useCloak` PLT | 0x683ac | **0x683a0** |

This is a pure link-order side effect. It costs exactly two byte_exact functions:
`MGame::successCheck` (3 bytes) and `MGame::dockEvent` (1 byte), both of which call
`dockToStream` via the swapped slot. `linked_exact` is unaffected (4507). The ratchet
therefore reports `byte_exact 4507 -> 4505`.

The original keeps `dockToStream` first because its `0x800` arm body sits at
`0x17ae8c`, *after* the starMap-exit branch (`0x17ae08..0x17ae8a`); its first arm
body is the small `0x200` arm at `0x17adc0`. Reproducing that body order while
keeping the late join is the remaining lever. Tested and rejected: arm 284/248/428/482
(join moves back to `0x17aeb2`, +12 B), `dock` reorder (`cab`/`cba`, still 8076/8080
but `useCloak` still first), moving `useCloak()` after the chain (`cloakout`, 8092),
`__builtin_expect` on the `0x200`/`0x400`/`0x800` arm tests (8044/8032/8084).

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10610/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4201 / 8080
python3 /dev/shm/w10610/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 646
cmake --build cmake-build-match --target check -j6                # byte 4505 (PLT swap)
```

Scratch: `/dev/shm/w10610/{fdiff.py,align_diff.py,showdiff.py,netdiff.py,gen.py,
batch.sh,run.sh,arm824.net}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
