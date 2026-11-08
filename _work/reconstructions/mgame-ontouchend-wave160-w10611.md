# MGame::OnTouchEnd — wave 160 (w10611): touch-flag decode is byte-identical; AND-form `won` guard lands raw 4375 at 8080 B

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10611`, base `08af4633b` (wave 159 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10611`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 159) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4387 | **4375** |
| aligned (`align_diff.py`) | 901 | **956** |
| `MGame::dockEvent` byte diff | 0 | **0** (1300 B) |
| `dockToStream` PLT slot | `0x683a0` | **`0x683a0`** |

Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS.

## 1. The touch-flag decode is byte-identical (mission hypothesis falsified)

The whole `hudTouchFlags` decode was diffed instruction-for-instruction against
the original. Every `and`/`tst`/`lsls`/`ubfx`/`ldrb` sequence matches exactly,
in the same order, with the same masks:

| source site | original | ours |
|---|---|---|
| `flags & 1` (pause) | `lsls r0,r0,#31` | same |
| `flags & 2` (thrust) | `lsls #30` | same |
| `flags & 4` (quick menu) | `ldrb [fp,#248]; lsls #29` | same |
| `flags & 8` (fire) | `lsls #28` | same |
| `flags & 0x40` (dock) | `tst.w r0,#64` | same |
| `flags & 0x80` (camera) | `ldrb [fp,#248]; lsls #24` | same |
| `flags & 0x200` (hack LCW) | `tst.w r0,#512` | same |
| `flags & 0x400` (hack RCW) | `lsls r0,r0,#21` | same |
| `flags & 0x200000` | `lsls r1,r0,#10` | same |
| `flags & 0x400000` | `lsls r1,r0,#9` | same |
| `flags & 0x800000` | `lsls r1,r0,#8` | same |
| `flags & 0x1000000` | `lsls r1,r0,#7` | same |
| `flags & 0x2000000` | `lsls r0,r0,#6` | same |
| `0x4000000 << i` loop | `lsl.w r1,r8,r4` | same |
| `flags & 0x20000000` | `ldrb [fp,#251]; lsls #26` | same |
| arm chain 0x200/0x800/0x400/0x2000/0x4000/0x8000/0x10000/0x20000/0x40000/0x80000/0x100000/0x1000 | `tst #512`, `lsls #20`, `#21`, `#18`, `#17`, `#16`, `#15`, `#14`, `#13`, `#12`, `#11`, `#19` | same order, same immediates |
| `flags == 0` | `ldr [fp,#248]; cbz` | same (`ldr; cmp #0`) |

The instruction histogram of every `tst`/`lsls`/`ands`/`ubfx` in the function is
identical between original and ours (same count, same immediates). The
`this->field_0xNN` offsets used by the decode were re-checked against
`MGame.h`'s `static_assert`s (`hudTouchFlags @ 248`, `hudMenuOpen @ 214`,
`pauseOpen @ 93`, `_bc8 @ 200`, `menuTouchOpen @ 201`, `freeCamMode @ 350`,
`field_0x1a4 @ 420`, `pauseSnapshot @ 422`, `_b1b8 @ 440`): all correct.

**Conclusion:** there is no wrong mask or wrong bit order. The decode residual is
100% block placement (the original is PGO-weighted), not decode logic.

## 2. The landing: AND-form `won` guard + cold starMap return

The only gate-green raw win found is the wave-159 `won`-guard shape, now landed
with a same-line lint waiver:

```diff
-                               ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() == true;
+                               ((((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() & 1) != 0);
...
-                if (!this->starMap->OnTouchEnd(x, y)) {
+                if (__builtin_expect(!this->starMap->OnTouchEnd(x, y), 0)) { // lint: arm_builtin ...
```

The `& 1` keeps LLVM's `calcZeroHeuristics` AND exception (50:50 weight) so the
`won` zero-compare keeps the original `lsls #31; bne` shape instead of
re-laying the function to 8060 B. The `starMap` cold hint repairs the
`dockToStream` PLT slot (the AND form alone puts `resetStartSequenceOver`'s `bl`
first, moving `dockToStream` to `0x683ac` and dropping `MGame::dockEvent` by 1
byte -> byte_exact 4506).

Result: raw **4387 -> 4375** (-12), size 8080, `dockEvent` 0, gate green.
Aligned regresses **901 -> 956**: the cold hint moves the starMap block, which
difflib's greedy alignment penalises. This is a raw-metric win with an
aligned-metric trade-off; the coordinator should weigh it.

## 3. New hypotheses tried (all measured on the wave-159 base, 8080 B unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 4387 | 901 | 8080 |
| **AND + starMap cold hint (landed)** | **4375** | **956** | **8080** |
| AND alone (PLT broken, gate FAIL) | 4367 | 902 | 8080 |
| `if (count > 0)` (mining refine) | 4389 | 902 | 8080 |
| `if (count != 0)` / `if (amount != 0)` | 6590 | 1835 | 8060 |
| `expect(hudTouchFlags == 0, 0)` on the last else-if | 5777 | 1035 | 8068 |
| separate `if (flags == 0)` after the chain | 4440 | 921 | 8080 |
| separate `if (expect(flags == 0, 0))` | 4776 | 941 | 8092 |
| `expect(won,1)` alone | 4752 | 835 | 8092 |
| `expect(won,1)` + `expect(flags==0,0)` | 6032 | 940 | 8080 |
| reorder arms 0x200,0x400,0x800 | 4392 | 1020 | 8080 |
| reorder + store + `expect(won,1)` (wave-158 shape) | 4022 | 762 | 8092 |
| reorder + store (no expect) | 5536 | 1138 | 8072 |
| `expect(isDockingToStream(), 0)` | 4481 | 930 | 8080 |
| `expect(starMapOpen, 1)` | 7440 | 2944 | 7996 |
| `expect(cutsceneActive, 0)` | 5336 | 2374 | 8020 |
| `expect(starMap->exitRequested, 0)` | 5202 | 1292 | 8080 |
| `expect(starMap->exitRequested, 1)` | 4786 | 3198 | 8076 |
| `expect(touchesStream, 1)` on the dock branch | 4605 | 932 | 8084 |
| AND + `count > 0` + star hint | 4377 | 957 | 8080 |
| AND + `keepObjectives` idiom removed + star hint | 4375 | 956 | 8080 |
| AND + `expect(won,1)` | 4752 | 835 | 8092 |
| AND + `expect(won,1)` + `if (failed)` | 4770 | 1980 | 8080 |

The `count >= 0` mining-refine condition is semantically wrong (original skips
the report when `amount == 0`; ours always enters), but every spelling that
produces the original `cmp r9,#0; beq` canonicalises the loop counter to a
positive induction variable and shrinks the function to 8060 B (wave-121's
known hard case). `if (count > 0)` is the only 8080 B spelling and is +2 raw.

## 4. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10611/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4375 / 8080
python3 /dev/shm/w10611/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 956
python3 /dev/shm/w10611/fdiff.py _ZN5MGame9dockEventEii           # 0 / 1300
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10611/{fdiff.py,align_diff.py,realdiff.py,diffdetail.py,
firstsz.py,firstdiv.py,patch.py,apply.py,andbase.sh,try.sh,MGame.base.cpp,orig.asm}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
