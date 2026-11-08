# w10812 (wave 162): `Level::createMission` — stack-slot permutation
# investigation; no size-exact source lever found (raw 5,454 / aligned 786 /
# 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10812`, based on master
`29248c1d0` (wave 161).  Continues
`level-createmission-wave161-w10713.md`,
`level-createmission-wave160-w10612.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B).
Raw same-address diff: `/dev/shm/w10812/fdiff.py`; instruction-index
alignment: `/dev/shm/w10812/align_diff.py` (worktree-local copies with
`OURS` pointed at this worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD 29248c1d0, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5454   align 786
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,374   .ARM.extab 725   .rel.plt 0 (byte-identical)
```

## 1. Frame map (final `-mllvm -print-machineinstrs` frame objects)

Frame size 496 B.  `[SP-N]` is relative to the frame top; `sp+N` is relative
to the post-`sub sp,#400` stack pointer.

```text
fi#0   4  sp+396   __stack_chk_guard canary value
fi#1   12 sp+260   Vector playerPos
fi#3   12 sp+248   RaidCoords3 raidCoords (a/b/c at sp+272..280 in the body)
fi#5   12 sp+384   (align 8)
fi#6   12 sp+372
fi#7   60 sp+312   (align 8)
fi#8..fi#11, fi#19, fi#45, fi#48, fi#69, fi#70  12-byte locals
fi#71  36 sp+272   (align 8)
fi#73..fi#100  4-byte spill slots, laid out sp+124 down to sp+16
   fi#73 = sp+124, fi#74 = sp+120, ... fi#82 = sp+88, fi#83 = sp+84, ...
   fi#100 = sp+16
fi#148..fi#163  fixed callee-saved register save areas (sp+400..496)
```

The two swapped slots are `fi#82`/`fi#83`:

```text
              ours                         original
guard addr    fi#83 = sp+84   (str.w fp)   fi#82 = sp+88
raid bool     fi#82 = sp+88   (str r0)     fi#83 = sp+84
```

The only differing instructions in the whole function are the three guard/raid
stores/loads:

```text
ours:     b0024 str.w fp,[sp,#84]   b04c0 str r0,[sp,#88]   b04d8 str r1,[sp,#88]
original: b0024 str.w fp,[sp,#88]   b04c0 str r0,[sp,#84]   b04d8 str r1,[sp,#84]
          b02ca ldr.w fp,[sp,#84]   (original: [sp,#88])
```

## 2. Root cause: StackSlotColoring weight order

`-mllvm -print-machineinstrs` (`/dev/shm/w10812/pmi.txt`) shows:

* RA spill slot 73 = `__stack_chk_guard` address (2 refs: prologue store,
  block 520 reload before the shared epilogue).
* RA spill slot 85 = `raid` (17 refs across the body).
* StackSlotColoring (`SSIntervals` sorted by `li.weight` descending, stable on
  slot index) maps slot 73 -> colour 83 and slot 85 -> colour 82.
* Colours are the frame indices in increasing order; `fi#73` is sp+124 and
  `fi#100` is sp+16, so a *lower* colour is a *higher* address.

Computed slot weights from the machine CFG and the MBP branch weights
(`/dev/shm/w10812/color_sim.py`):

```text
slot 73 (guard) weight 1.1811   -> colour 82 (simulation; real 83)
slot 85 (raid)  weight 1.3078   -> colour 81 (simulation; real 82)
```

Raid is processed first because its weight is 0.13 higher, so it takes the
lower colour (sp+88).  The original must have guard weight >= raid weight
(PGO/sample-profile branch weights), which is why the original gets guard at
sp+88 and raid at sp+84.  The gap is small; a size-exact *source* lever that
changes only block frequencies was not found.

## 3. New levers measured this wave (all size-exact unless noted)

| candidate | raw | align | size | flip? | notes |
|---|---:|---:|---:|---|---|
| HEAD (baseline) | 5,454 | 786 | 17,172 | no | guard sp+84 / raid sp+88 |
| `__builtin_expect(mission == nullptr, 0)` | 15,912 | 1,159 | **17,192** | **yes** | fatal +20; RA changes (guard `r6`, not `fp`) |
| `!__builtin_expect(mission != nullptr, 1)` | 15,912 | 1,159 | **17,192** | yes | same |
| `bool noMission = ...; expect(noMission,0)` | 15,912 | 1,159 | **17,192** | yes | same |
| `volatile char buf[4]; buf[0]=0;` early | 16,143 | 1,622 | **17,188** | **yes** | fatal +16 |
| `char buf[4]; buf[0]=0;` early | 5,454 | 786 | 17,172 | no | dead store removed |
| `int raid` / `char raid` / `unsigned char raid` | 5,681 | 798 | 17,172 | no | |
| `long long raid` | 16,156 | 1,700 | **17,296** | no | fatal +124 |
| `volatile bool raid` | 15,233 | 1,357 | **17,200** | no | fatal +28 |
| `double raiders` | 5,850 | 816 | 17,172 | no | |
| `double alienChance` | 13,545 | 908 | **17,180** | no | fatal +8 |
| `long long extraFriend` | 5,454 | 786 | 17,172 | no | identical codegen |
| `long long stationIndex` | 5,454 | 786 | 17,172 | no | identical codegen |
| `long long security` | 16,569 | 2,223 | **17,360** | no | fatal +188 |
| `long long raidRoll` | 16,373 | 1,723 | **17,208** | no | fatal +36 |
| `unsigned raidRoll` | 5,455 | 788 | 17,172 | no | +1 raw |
| `unsigned count` (alien loop) | 5,456 | 788 | 17,172 | no | |
| `unsigned i` (alien loop) | 5,456 | 788 | 17,172 | no | |
| `i != count` (alien loop) | 5,456 | 788 | 17,172 | no | |
| `while (i < count)` rewrite | 5,454 | 786 | 17,172 | no | identical codegen |
| `struct alignas(8) RaidCoords3` | 5,454 | 786 | 17,172 | no | object already 8-aligned |
| `alignas(8) bool raid` | 5,454 | 786 | 17,172 | no | bool promoted to 4-byte slot |
| `alignas(8) int coords[3]` (sieged) | 5,485 | 829 | 17,172 | no | worse |
| `char raidCoords[12]` (cast stores) | 5,454 | 786 | 17,172 | no | identical codegen |
| `volatile int raidCoords[3]` | 5,454 | 786 | 17,172 | no | identical codegen |
| `__builtin_expect(inAlienOrbit(),1)` | 6,007 | 824 | 17,172* | no | `.text` +32 (another fn grew) |
| `__builtin_expect(inAlienOrbit(),0)` | 6,041 | 827 | 17,172* | no | `.text` +32 |
| `__builtin_expect((int)raiders>=1,0)` | 16,240 | 2,401 | **17,240** | no | fatal +68 |
| `__builtin_expect(mission->isEmpty(),1)` | 16,383 | 7,391 | **17,356** | no | fatal +184 |
| `__builtin_expect(hardCoreMode(),0)` (raiders) | 14,698 | 957 | **17,184** | no | fatal +12 |
| `bool raid;` moved above `raidRoll` | 6,914 | 855 | 17,172 | no | worse |

`*` = createMission size exact but a preceding function in the TU grew, so the
symbol address moves (`.text` 0x1337c8 -> 0x1337e8); gate-fatal.

## 4. Conclusion / open questions

1. The 4-byte permutation is a `StackSlotColoring` weight-order artefact:
   `raid` slot weight 1.31 > `__stack_chk_guard` slot weight 1.18, so raid
   gets colour 82 (sp+88).  The original's PGO branch weights must put the
   guard at or above raid (guard 82 / raid 83).
2. Every source lever that flips the colours (`__builtin_expect` on the
   mission-null branch, a `volatile char buf[4]`) also perturbs the register
   allocator and grows the function by 16-20 B.  No size-exact flip found.
3. The closest size-exact no-flip variants are `unsigned raidRoll` (5,455),
   `unsigned count` / `i != count` (5,456), and `double raiders` (5,850);
   none improves the raw metric, so HEAD (5,454 / 786) is kept.
4. A future wave could try a *post-RA* weight lever: e.g. a source shape that
   adds a third reference to the guard slot (another canary check) without
   changing the code size, or a branch weight that changes block 520's
   frequency without changing the virtual-register spill weights.

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5454 -> 5454   (unchanged)
                           align  786 -> 786    (unchanged)
.text diff             85,374 -> 85,374
.ARM.extab diff           725 -> 725
.rel.plt                    0 -> 0   (byte-identical)
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
```

`check` is green.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10812/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10812/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10812/{fdiff.py,align_diff.py,quick.sh,measure.sh,
color_sim.py,freq.py,pmi.txt,Level.base.cpp,ours.asm,orig.asm}`.
