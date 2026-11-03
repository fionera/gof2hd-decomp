# MGame::OnUpdate — the 41-byte `Status*` cache: byte-level classification and the
# rematerialisation dead end (wave 96, w4606)

Worktree `/home/debian/GalaxyOnFire2/wt/w4606`, branch `wave96-w4606`, base master
`3bcaeb3a`.  Every number below was measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 and the official `tools/verify/verify.py`.  Scratch drivers live in
`/dev/shm/w4606` (`rdiff.py`, `bdiff.py`, `b3..b5.py`, `gotscan.py`, `cmpobj.py`,
`cmpasm.py`, `ir.sh`).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8, **99.9 %**, **41 differing bytes in
2 runs**.  No source change was landed: every shape that removes the two `mov`s also
moves `&Globals::status` out of r9 and re-colours the whole function (≥ 4480 diff bytes).

## 0. Gate (unchanged before and after this pass)

| metric | value |
|---|---:|
| byte_exact / linked_exact | 4500 / 4524 |
| size-exact | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |
| `.text` / `.rodata` / `.ARM.exidx` | `0x1337c8` / `0x41b67` / `0x6f70` |
| `.ARM.extab` | `0x26290` (-8) |

`cmake --build cmake-build-match --target check -j8` ends with `RATCHET PASS`.

## 1. The 41 bytes, classified

The whole 11440-byte body differs in exactly two runs, both inside the mission-23
idle-contact condition (`src/game/menu/MGame.cpp:2767-2774`):

```
_ZN5MGame8OnUpdateEv size 11440 diffbytes 41 runs 2
  17e092..17e093 (1)     ; low byte of the ldr.w target register: 00 (r0) vs 50 (r5)
  17e094..17e0bc (40)    ; everything is shifted +2 because ours has one extra mov
```

Instruction-level (original = `O`, ours = `U`):

```
O 17e088 ldr r0,[pc,#872]              U 17e088 ldr r0,[pc,#872]
O 17e08a add r0,pc                     U 17e08a add r0,pc
O 17e08c ldr.w r9,[r0]   ; &Globals::status   U 17e08c ldr.w r9,[r0]
O 17e090 ldr.w r0,[r9]   ; Status*      U 17e090 ldr.w r5,[r9]   ; st = Globals::status
O 17e094 blx getCurrentCampaignMission U 17e094 mov r0,r5
                                       U 17e096 blx getCurrentCampaignMission
O 17e098 cmp r0,#23                    U 17e09a cmp r0,#23
O 17e09a bne.n 17e176                  U 17e09c bne.n 17e176
O 17e09c ldr.w r0,[sl]  ; player       U 17e09e ldr.w r0,[sl]
O 17e0a0 blx isMining                  U 17e0a2 blx isMining
O 17e0a4 cmp r0,#0                     U 17e0a6 cmp r0,#0
O 17e0a6 bne.n 17e176                  U 17e0a8 bne.n 17e176
O 17e0a8 ldr.w r0,[r9]  ; Status*      U 17e0aa ldr.w r0,[r9]
O 17e0ac blx getMission                U 17e0ae blx getMission
O 17e0b0 blx isEmpty                   U 17e0b2 blx isEmpty
O 17e0b4 cmp r0,#0                     U 17e0b6 cmp r0,#0
O 17e0b6 beq.n 17e176                  U 17e0b8 beq.n 17e176
O 17e0b8 ldr.w r0,[r9]  ; Status*      U 17e0ba mov r0,r5
O 17e0bc blx getPlayingTime            U 17e0bc blx getPlayingTime
O 17e0c0 ldr.w r2,[r9]  ; Status*      U 17e0be ldr.w r2,[r9]
```

So there are exactly **three** real instruction differences, and the rest of the run is
the 2-byte shift they cause:

1. `17e090`: the first `Status*` load lands in **r0** (original) vs **r5** (ours).
2. `17e094`: ours inserts `mov r0,r5` (2 B) before the `getCurrentCampaignMission` call.
3. `17e0b8`: the original re-loads `[r9]`; ours reuses r5 (`mov r0,r5`).

The byte count is exactly neutral: ours `ldr r5,[r9]` + `mov` + `mov` = 4+2+2 = 8 B;
original `ldr r0,[r9]` + `ldr r0,[r9]` = 4+4 = 8 B.  `&Globals::status` is in **r9 in
both** builds, and everything outside `17e088..17e0c4` is byte-identical (`outside = 0`).

## 2. Why the two `mov`s exist: the IR has one shared `st` load

`clang -emit-llvm -S` on the TU (see `ir.sh`) gives, in the mission-23 block:

```llvm
%1091 = load %class.Status*, %class.Status** @_ZN7Globals6statusE   ; st
call void @llvm.dbg.value(metadata %class.Status* %1091, metadata !"st", ...)
%1092 = call i32 @_ZN6Status25getCurrentCampaignMissionEv(%class.Status* %1091)
...
%1098 = load %class.Status*, %class.Status** @_ZN7Globals6statusE   ; fresh (getMission)
%1099 = call %class.Mission* @_ZN6Status10getMissionEv(%class.Status* %1098)
...
%1102 = call i64 @_ZN6Status14getPlayingTimeEv(%class.Status* %1091) ; st again
%1103 = load %class.Status*, %class.Status** @_ZN7Globals6statusE   ; fresh (field)
```

`%1091` is live across the `isMining` and `getMission` calls, so the register allocator
must keep it in a callee-saved register (r5).  The first `Globals::status` load is
adjacent to the `st` load with no call in between, so GVN folds the
`getCurrentCampaignMission` receiver into `%1091`; that is why the first call also uses
r5.  The original has four independent `ldr [r9]` loads, i.e. its IR has **four distinct
loads** and no live `st`.

MIR confirmation (after instruction selection, `-print-machineinstrs`):

```
bb.238: %1052 = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
        %1053 = t2LDRi12 %1052, 0        ; &Globals::status  (72 uses in the function)
        %128  = t2LDRi12 %1053, 0        ; st
        DBG_VALUE %128, !"st"
        $r0 = COPY %128
        tBL getCurrentCampaignMission
bb.240: %1059 = t2LDRi12 %1053, 0        ; fresh (getMission)
bb.241: $r0 = COPY killed %128           ; st again
bb.241: %1066 = t2LDRi12 %1053, 0        ; fresh (field_0x100)
```

and after the virtual-register rewriter (baseline `onupd_vrr2.txt`):

```
bb.238  liveins: $r6, $r10, $r11
        renamable $r9 = t2LDRi12 killed renamable $r0, 0   ; base
        renamable $r5 = t2LDRi12 renamable $r9, 0          ; st
        $r0 = COPY renamable $r5
bb.239  liveins: $r5, $r6, $r9, $r10, $r11
bb.241  $r0 = COPY killed renamable $r5
```

## 3. Why simply deleting `st` is not the answer

Deleting `st` (four `Globals::status->` accesses) gives the right four loads but the
allocator then picks **r5** for `&Globals::status` and splits its live range
(`mov r9,r5` at 0x17e0d2, 11392 B, 5196 diff bytes / 57 in-region / 5139 outside).  The
whole function re-colours because every `ldr.w rX,[r9]` becomes a 2-byte `ldr rX,[r5]`.

The `st` local is therefore a **size anchor that buys r9** at the cost of the two `mov`s;
the original reaches r9 without it.

## 4. Variant sweep (official verifier / raw byte differ)

`rdiff.py` prints `size orig/ours`, total diff bytes, diff bytes inside
`0x17e088..0x17e0c4`, and outside.  `outside == 0` means the whole rest of the function
(including `&Globals::status` in r9) is untouched.

| variant | size | total | region | outside |
|---|---:|---:|---:|---:|
| **baseline** (`st` used at `getPlayingTime` only) | 11440 | **41** | 41 | **0** |
| `st` also at `getCurrentCampaignMission` (same IR, GVN) | 11440 | 41 | 41 | 0 |
| `Status *const st` | 11440 | 41 | 41 | 0 |
| `Status &st = *Globals::status` + `st.getPlayingTime()` | 11440 | 41 | 41 | 0 |
| `st` declared after the `elapsedTime > 5000` block (same IR) | 11440 | 41 | 41 | 0 |
| volatile read at `getMission` (load already post-call) | 11440 | 41 | 41 | 0 |
| volatile read at `field_0x100` | 11440 | 41 | 41 | 0 |
| **no `st` at all** | 11392 | 5196 | 57 | 5139 |
| `st` also at `getMission` | 11436 | 4986 | 39 | 4947 |
| `st` also at `field_0x100` | 11436 | 4985 | 46 | 4939 |
| `st` also at mission-24 `getPlayingTime` | 11436 | 4785 | 42 | 4743 |
| `st` also at mission-24 `getMission` | 11436 | 4800 | 42 | 4758 |
| `st` also at >=122 `getPlayingTime` | 11436 | 4525 | 42 | 4483 |
| `st` also at >=122 `getMission` | 11436 | 4538 | 42 | 4496 |
| `st` also at >=122 second `getCurrentCampaignMission` | 11436 | 4558 | 42 | 4516 |
| `st` also at >=122 `radioBase` (`== 122`) | 11436 | 954 | 41 | 913 |
| `st` also at alien `getPlayingTime` | 11444 | 4480 | 42 | 4438 |
| `st` also at alien `inAlienOrbit` | 11436 | 4474 | 42 | 4432 |
| `st` also at alien `field_0x17c` read | 11440 | 50 | 41 | **9** |
| `st` also at alien `field_0x17c = 0` store | 11452 | 4893 | 48 | 4845 |
| `st` also at wingmen-loop `getCurrentCampaignMission` | 11456 | 4750 | 48 | 4702 |
| `st` also at `hints[23]` `getShip()->hasBooster()` | 11464 | 5020 | 55 | 4965 |
| `st` also at `hints[23]` `getCurrentCampaignMission() >= 2` | 11464 | 5020 | 55 | 4965 |
| `st` also at `hints[33]` / `hints[34]` `getShip()` | 11464 | 5029 | 55 | 4974 |
| `st` also at `hints[32]` `>= 10` | 11468 | 5048 | 55 | 4993 |
| `st` also at `hints[21]` `wingmen` | 11468 | 5057 | 55 | 5002 |
| `st` also at `hints[18]` `>= 4` | 11488 | 5110 | 55 | 5055 |
| `st` also at `hints[55]` `== 2` | 11488 | 5113 | 55 | 5058 |
| `st` also at `hints[20]` `>= 7` | 11488 | 5116 | 55 | 5061 |
| `st` also at `hints[37]` `getFirstEquipmentOfSort(13)` | 11488 | 5103 | 55 | 5048 |
| `st` also at `hints[46]` `== 91` | 11472 | 6197 | 53 | 6144 |
| `st` also in the mission-23 body (`getPlayingTime`) | 11524 | 6231 | 59 | 6172 |
| `st` also in the mission-23 body (`field_0x100`) | 11524 | 6228 | 59 | 6169 |
| `st` also in the mission-23 body (both) | 11520 | 6233 | 59 | 6174 |
| `(st = Globals::status)->getPlayingTime()` in the condition | 11392 | 5196 | 57 | 5139 |
| `st` declared at function top (after `hints`) | 11372 | 10890 | 58 | 10832 |
| `*(Status *volatile *)&Globals::status` at the first access | 11440 | 853 | 46 | 807 |
| remove the `uint8_t *hints` local (use `Globals::hints[...]`) | 11440 | 52 | 41 | **11** |

Key facts from the table:

* **Only the baseline IR gives `outside == 0`.**  Every shape that changes the
  `Globals::status` load pattern also moves `&Globals::status` out of r9, and because
  every r9 use is a 4-byte `ldr.w` vs a 2-byte `ldr rX,[r5]`, the whole function shifts
  (≥ 4432 diff bytes).
* The `hints`-local removal is the only *other* shape with `outside == 0`; it leaves the
  base in r9 and adds 11 bytes of scheduling/`ldr`-offset difference at
  `0x17f02c..0x17f03a` (the hint block), i.e. 52 total.  It is not an improvement.
* `st` also at the alien `field_0x17c` read keeps the base in r9 (outside 9) but is still
  worse than the baseline (50 vs 41).

## 5. Why the original cannot be "one `st` load that got rematerialised"

The natural way for the original to have four `ldr [r9]` loads *and* keep
`&Globals::status` in r9 would be: same IR as ours, but the allocator rematerialises
`%1091` at its two uses instead of keeping it in r5.  That is impossible with this
compiler:

* `TargetInstrInfo::isReallyTriviallyReMaterializableGeneric` (LLVM 7.0.2,
  `lib/CodeGen/TargetInstrInfo.cpp:885`) rejects any load that is not
  `MachineInstr::isDereferenceableInvariantLoad(AA)`.
* `MachineInstr::isDereferenceableInvariantLoad` (`lib/CodeGen/MachineInstr.cpp:1124`)
  accepts a load only if the memoperand is `!invariant.load`+dereferenceable, or a
  constant PSV, or `AA->pointsToConstantMemory(...)`.
* `@_ZN7Globals6statusE` is `static Status *status;` (`src/game/core/Globals.h:161`,
  `Globals.cpp:197`) — a non-const `.bss` global, so `pointsToConstantMemory` is false.
  The MIR memoperand is `(dereferenceable load 4 from @_ZN7Globals6statusE, !tbaa !7940)`
  with no `!invariant.load`.
* The ARM target does not override `isReallyTriviallyReMaterializable`
  (`lib/Target/ARM/ARMBaseInstrInfo.{h,cpp}` contain no such override), so the generic
  check applies and `t2LDRi12 %r9, 0` is **not** rematerialisable.

Consequently the original's four fresh loads must be four distinct IR loads, i.e. the
original source has **no `st` local** — and the r9 colouring of `&Globals::status` must
come from something else in the function.

## 6. What the r9 colouring actually depends on

Register-allocation facts extracted from the MIR:

* The `&Globals::status` vreg (`%1053`) has **72 uses** and one live range spanning
  `0x17e08c..0x17e846` (spilled with `str.w r9,[sp,#36]` at `0x17e846`, reloaded from the
  stack at `0x17e86c`/`0x17e8ce`/`0x17e8d8`).  It is the same single range in the
  original, in the baseline and in the no-`st` build.
* Live-ins at `bb.238` (the mission-23 block) are `$r6, $r10, $r11` in the baseline
  (`$r6` = `fp+72` = `&this->elapsedTime`, `$r10` = `sl` = `&this->player`, `$r11` = `fp`
  = `this`).  r5 is free until `st` is defined at `0x17e090`; r9 is free.
* In the no-`st` build the same block has live-ins `$r5, $r6, $r10, $r11`: the base
  itself took r5, and the allocator later split the range with `mov r9,r5` at
  `0x17e0d2`.  The original has no such split (r9 from `0x17e08c` on).
* LLVM 7 spill weight is `UseDefFreq / (Size + 25*InstrDist)`
  (`include/llvm/CodeGen/CalcSpillWeights.h:35`).  The base is a long, low-density range;
  a short value live across the two calls outranks it and takes r5, which is exactly what
  `st` does in the baseline.

So the original must contain **a value that is live in r5 somewhere inside
`0x17e08c..0x17e846` and outranks the base**, while the four `Status*` loads stay
independent.  That value is not visible in the emitted code: r5's previous value dies at
`0x17e044` and its next write is `0x17e12a` (`ldr r5,[r0,#0]`, the `gameText` pointer),
so r5 is free in `0x17e044..0x17e12a` in the original.  Every `st` use we add inside/after that window changes the emitted code
(the table above), so the value must be one that the reconstruction is missing, not a
different spelling of `Globals::status`.

## 7. Reproduction

```
cmake --build cmake-build-match --target link_so -j8     # ~1.5 s
python3 /dev/shm/w4606/rdiff.py                          # size 11440/11440 total 41 region 41 outside 0
python3 /dev/shm/w4606/bdiff.py                          # the two runs above
cmake --build cmake-build-match --target check -j8       # RATCHET PASS, byte 4500
```

`FN=... verify-fn` is broken in worktrees; call `tools/verify/verify.py --show
_ZN5MGame8OnUpdateEv` with `--original/--ours/--objdump/--build-dir` directly.
