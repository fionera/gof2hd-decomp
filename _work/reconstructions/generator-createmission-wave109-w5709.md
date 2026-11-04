# `Generator::createMission` — wave 109 (w5709)

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record 168 B).

Status: **research note, no source change landed.** The landed wave-108 shape
(`volatile double padA` before the reward computation,
`__builtin_expect(pick == 10, 1)`, `__builtin_expect(pick == 12, 0)`,
shared `goto assignType`) remains the best measured variant at
**82.1 % / 1996 B**.  This pass ran an additional source-shape sweep over the
pick/type arm block layout, branch hints, loop forms, `amount` scheduling,
stack-slot/declaration order and reward/bonus spellings; nothing beat 82.1 %.

Gate at the start and end of this pass (full `check`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

`git status --short` is empty after the note commit.

## 1. What is already exact

The prologue, the whole pick loop (`0x97248..0x972d8`) except the
`pick == 12` arm placement, the campaign-mission switch, the `type == 8`
item loop, the `switch (type)` jump table, the distance/reward block from
`0x9760a` on, the `Mission` ctor, the costs rounding, the target-system loop
and the landing pads are instruction-for-instruction identical modulo register
names and stack offsets.  The remaining residual is:

* frame 112 vs original 96 (`padA` at `[sp,#80]`, `amount` at `[sp,#104]`,
  `agentName` at `[sp,#92]`; original `agentName` at `[sp,#80]`, canary at
  `[sp,#92]`, `amount` reusing `targetStation`'s `[sp,#28]`);
* `agent`/`targetStation` stack slots swapped (`ldrd r4,r9` vs `ldrd r9,r4`);
* `difficulty` in `r5` instead of the original's `r6`;
* `galaxy`/`fromSystemPtr` swapped (`galaxy` in `r6`, `fromSystemPtr` at
  `[sp,#16]`; original keeps `fromSystemPtr` in `r6` and spills `galaxy`);
* `pick == 12` arm is out of line (`cmp r6,#12; beq` into it) instead of the
  original's fall-through (`cmp r6,#12; bne` over it);
* `getRoutes() == nullptr` block is inline (`cbz`) instead of the original's
  out-of-line cold block at `0x97586` (`cmp; beq.w`);
* the `amount` stores are sunk to one shared join store (`[sp,#104]`) instead
  of the original's per-case stores (`[sp,#28]` in each arm).

## 2. The p12/loop-cond coupling (MBP)

The original loop is bottom-tested:

```
972b0  cmp  r6,#12
972b2  bne  972ce        ; selected=true
972b4  ... p12 body ...
972cc  b   972d4         ; loop cond
972ce  mov fp,#1         ; selected=true
972d2  mov r2,r6         ; type=pick
972d4  movs.w r0,fp,lsl#31
972d8  beq  97248        ; loop body
```

Our landed shape has the same bottom test but swaps the two successors of the
`cmp r6,#12` block:

```
972b0  cmp  r6,#12
972b2  beq  972bc        ; p12 body
972b4  mov fp,#1         ; selected=true
972b8  mov r2,r6         ; type=pick
972ba  b    972d4
972bc  ... p12 body ...
972d4  movs.w r0,fp,lsl#31
972d8  beq  97248
```

The p12 branch weight is the switch.  With `__builtin_expect(pick == 12, 0)`
the p12 body is cold, `selected=true` is laid out first, and the loop cond is
correctly at the bottom.  With p12 plain/hot the p12 body is laid out first
(matching the original), but MBP no longer rotates the loop: the cond moves to
the top and the score collapses to 79.3 %/69.4 %.

The MBP trace shows the mechanism: changing p12 from cold to plain changes
`FREQ %bb.18` (the hot `selected=true` block) from `500966735` to
`250608609`; that flips `hasBetterLayoutPredecessor` for `%bb.19`.  Selecting
`%bb.19` first makes the loop chain start with the cond block, so `rotateLoop`
bails and the cond stays at the top.  The `assignType` shared label is still
required; any form that duplicates `type = pick` in both arms is CVP-folded to
1988 B / ~50 %.

`__builtin_expect` in clang 7 only emits 1:2000 / 2000:1 weights, so there is
no intermediate weight available from source.  `__builtin_expect_with_probability`
does not exist in this clang (tested: undeclared identifier).  Non-constant
expected values (`guard`, `pick`, `race`, `pick == 12`) behave like plain.

## 3. Stack-slot weights (base, from `-print-machineinstrs` + `-print-machine-bfi`)

```
stack.10 rnd       80.2319
stack.11 race      34.9840
stack.13 used      15.9206
stack.12 status     6.7134
stack.9             5.0406
stack.8             5.0000
stack.14            4.4428
stack.15 mission    3.1218
stack.0/7 canary    2.6938
stack.16            2.6015
stack.2 agentName   2.5203
stack.17 fromSysPtr 2.0812
stack.1             1.5823
stack.4             1.5609
stack.3             1.0406
stack.6             0.5039
stack.5             0.3902
```

Original SSC order: `rnd > race > status > targetStation > agent >
&__stack_chk_guard > galaxy`; ours has `used` on the stack (original keeps it
in `sl`) and `galaxy` in a register (original spills it).  Original stack map:
`[sp,16] galaxy/bonus`, `[sp,20] &stack_chk_guard`, `[sp,24]
agent/difficulty`, `[sp,28] targetStation/amount`, `[sp,32]
&Globals::status/rem/mission`, `[sp,36] race/type`, `[sp,40] &Globals::rnd`;
`agentName [sp,80]`, canary `[sp,92]`; frame 96.  Base DBG map:
`targetStation [sp,24]`, `race [sp,36]`, `agentName [sp,92]`, `type [sp,36]`,
`difficulty [sp,24]`, `fromSystemPtr [sp,16]`, `bonus [sp,16]`,
`mission [sp,32]`.

Original `amount` stores: `0x974a8` (type8), `0x974b4` (initial 0),
`0x9753e` (case15), `0x975ca` (case0), `0x97604` (cases2/11), all `[sp,#28]`.
Base `amount` stores: `0x974c4`, `0x97500`, `0x9754c`, `0x97600` at
`[sp,#104]` (4 stores; type8 sunk to the shared join).

Original type8 schedule:

```
0x97498 mov r6,r0
0x9749a ldr r0,[r5,#4]
0x9749c ldr r0,[r0,sl,lsl#2]
0x974a0 blx Item::getTecLevel
0x974a4 mov r1,r0
0x974a6 adds r0,r6,#5
0x974a8 str r0,[sp,#28]
0x974aa mov r6,r1
0x974ac b 0x9760a
```

Base:

```
blx getTecLevel
mov r5,r0
adds r0,r6,#5
b <shared amount store>
```

`amount = count + 5;` before `getTecLevel()` gives the type8 store in the arm
and 5 stores total, but `difficulty` stays in `r5` and the score is 81.8 %.

## 4. Measured variants

All rows use the official scorer, one `link_so` per row, starting from the
landed wave-108 shape unless noted.  `pct / usize`.

### Pick-loop branch weights

| variant | pct | usize |
|---|---:|---:|
| base (`p10 exp1`, `p12 exp0`, `p8 plain`) | **82.1** | **1996** |
| p12 plain | 79.3 | 1996 |
| p12 `ne1` / `not_exp0` / `exp0_not` / `eq12 exp1` | 69.4 | 1992 |
| p12 `ne0` / `not_exp1` / `exp0_neq0` | 82.1 | 1996 |
| p8 plain + p12 plain | 79.3 | 1996 |
| p8 plain + p12 exp0 | 82.1 | 1996 |
| p8 exp1 + p12 plain/exp0 | 52.3 | 2020 |
| p8 exp0 + p12 plain/exp0 | 57.1 / 56.2 | 1952 / 1960 |
| p10 plain | ~52 | 1988 |

Loop-condition expects:
`plain p12 + __builtin_expect(!selected,1)` 42.9/2032;
`plain p12 + __builtin_expect(!selected,0)` 54.2/1968;
`plain p12 + for(;!selected;)` 79.3/1996;
`exp0 p12 + __builtin_expect(!selected,1)` 43.8/2020;
`exp0 p12 + __builtin_expect(!selected,0)` 55.2/1964.

### p12 arm rewrites (all canonicalize)

| variant | pct | usize |
|---|---:|---:|
| base | 82.1 | 1996 |
| `sel_before_if` | 82.1 | 1996 |
| `fallthrough` | 82.1 | 1996 |
| `and_form` (`pick==12 && getIndex()==25`) | 82.1 | 1996 |
| `nested_if` | 82.1 | 1996 |
| `no_continue` | 52.0 | 1996 |
| `break_form` | 52.2 | 1988 |
| `sel_after` | 75.7 | 1992 |
| `type_cond` | 50.7 | 1988 |
| `and_form2` (getIndex first) | 68.1 | 1984 |

Additional forms tested this pass: `if (pick != 12) { selected=true; } else
{...}` (82.1), `if (pick == 12) {...} else { selected=true; }` (82.1),
`switch (pick)` with p8/p10/p12 cases (50.3), `switch (p8/p10)` (52.2),
`switch (p12)` (79.3), `switch (__builtin_expect(p12,0))` (82.1), explicit
`goto` forms (82.1), `int/bool isP12` temporaries (79.3/82.1), volatile
`isP12` (74.3), `sys` local / pointer / `!=25` / `type` before/after
(82.1), and `if/else` duplicating `type=pick` (58.7).  None changes the
landed layout.

### `amount` / type-8 schedule

| variant | pct | usize |
|---|---:|---:|
| base | **82.1** | **1996** |
| `amount = count+5;` before `getTecLevel()` | 81.8 | 1996 |
| `int tec = getTecLevel(); amount=count+5; difficulty=tec;` | 79.8 | 2000 |
| `int amt = count+5; difficulty=getTecLevel(); amount=amt;` | 79.8 | 2000 |
| `volatile int amt` temp | 75.9 | 2004 |
| pointer/ref/array store to `amount` | 82.1 | 1996 |
| `amount = count; amount += 5;` | 81.0 | 1996 |
| `switch(type)` with case 8 | 74.2 | 1992 |
| `default: amount=0` in switch | 79.5 | 1996 |
| `amount=0` before `if(type==8)` | 81.5 | 1996 |
| swap `type==8` arms | 73.1 | 1988 |

### Pad / frame

| variant | pct | usize |
|---|---:|---:|
| base `volatile double padA = 0; (void) padA;` | **82.1** | **1996** |
| `volatile double padA[1] = {0}` | 82.1 | 1996 |
| no `(void)` | 78.3 | 1992 |
| no init | 78.6 | 1988 |
| no init/no void | 71.5 | 1984 |
| `padA=0` assignment form | 78.3 | 1992 |
| `volatile long long padA` | 77.9 | 1996 |
| two `volatile int` pads | 70.8 | 1996 |
| `volatile float[2]` | 70.8 | 1996 |
| `volatile int[2]` | 71.3 | 1992 |
| remove pad entirely (volatile amount) | 71.5 | 1984, frame 104 |

### Declaration/order

| variant | pct |
|---|---:|
| base | 82.1 |
| `gal_after_from` / `gal_after_ptr` / `gal_after_pad` | 81.9 |
| `name_after_diff` (agentName after difficulty) | 67.9 |
| `name_after_clamp` | 52.6 / 1948 |
| `name_before_item` | 82.1 |
| `type_int` / `level_int32` / `item_uint` | 82.1 |
| `race_int` | 81.9 |
| `diff_uint` | 81.4 |
| `costs_uint` | 81.8 |
| `guard_uint` | 55.9 / 1984 |
| `diffTmp` / `tsTmp` Mission args | 82.1 |
| `rewardInt` temp | 78.0 / 1988 |
| `imageParts` hoist | 76.2 / 2004 |
| `raceTmp` | 50.0 / 1992 |
| `nameTmp` | 71.2 / 2024 |
| `targetStation` declaration split / `race` split | 82.1 |
| `agentAlias` | 82.1 (folds away) |

### Bonus / reward rewrites

All 82.1: `if_else`, `rem2`, `rem3`, `rem_unsigned`, `rem_hoist`,
`neg_first`, `ternary`, `rem_neg`, `rem_plus`, pointer/ref/array `rem`.
`minus2rem` 79.0.  `volatile int rem` 78.1/1992.  `short/unsigned/long/int8/
uint32 rem` all 82.1.

### getRoutes / null route

| variant | pct | usize |
|---|---:|---:|
| base inline `cbz` | **82.1** | **1996** |
| expect null 0 (`cmp; beq.w` original block) | 61.9 | 2000 |
| expect null 1 | 47.7 | 2040 |
| inverted `!= nullptr` if | 82.1 | 1996 |

### Other loop expects (with p12 plain)

`used != nullptr` exp0/exp1 55.5/53.6 (2016/1992);
`used->data[pick]` exp0/exp1 57.2/79.3 (1996/1996);
`sum==14` exp0/exp1 57.8/69.4 (1976/1992);
p8 exp0/exp1 57.1/52.3 (1952/2020).

### Additional wave-109 sweeps

* Exhaustive p8/p12/p10/used/data/sum/inner/race branch-hint combinations
  (p12 hot, p8 cold or plain; all 32 combinations of used/data/sum/inner/race):
  best 69.4 %, no 82.1+.
* p10 source shapes (`if/else`, `type` after, `&&`, race expects, plain,
  expect 0) crossed with p12 exp0/exp1/plain: only `p12 exp0 + p10 base`
  reaches 82.1.
* Loop forms (`while`, `while(selected==false)`, `while(selected!=true)`,
  `for(;!selected;)`, `for(;;)`, `do/while`, `while(true)` with `break`,
  `goto` loop) with p12 exp0/exp1/plain: no improvement.
* `volatile bool/int selected`, `volatile guard`, `volatile type/pick/race`:
  all below 74.3 %.
* `__builtin_expect` expected values `0/1/2/-1/guard/pick/race/isP12`:
  nonzero constant behaves like 1, non-constant behaves like plain; no
  intermediate weight.
* `__builtin_expect_with_probability`: unsupported by clang 7.0.2.

## 5. Proven vs hypothesis

Proven:

* `volatile int amount` + the 8-byte pad is needed for 1996 B; removing the
  pad drops to 1984 B / 71.5 %.
* `p12 exp0` + `p10 exp1` is required for 82.1; plain p12 gives the correct
  p12 `bne` but the wrong loop cond and 79.3 %.
* p8 must stay plain; any p8 expect collapses.
* All p12 arm rewrites canonicalize; no source spelling flips the p12
  fall-through while keeping the loop cond at the bottom.
* `getRoutes()==nullptr` expect 0 produces the original out-of-line block but
  the global register allocation drops to 61.9 %.
* `agentName` must remain before `difficulty`.
* `amount-first` gives 5 stores but 81.8 %; no better.
* MBP after placement explains the p12/loop-cond coupling exactly (see §2).
* clang 7 has no intermediate `__builtin_expect` weight.

Hypothesis (not closed):

* A source shape that makes the p12 body the layout successor of the
  `cmp r6,#12` block while keeping the loop cond at the bottom would close the
  p12 arm placement.  No such spelling was found.
* The original type8 `mov r1,r0; adds r0,r6,#5; str r0,[sp,#28]; mov r6,r1`
  schedule is not reproduced; it is coupled to frame/register allocation and
  the `amount` store sink.
* The original frame-96 slot map likely follows from getting `amount` to reuse
  `targetStation`'s `[sp,#28]`; our `amount` is allocated at `[sp,#104]` and
  `targetStation`/`difficulty` share `[sp,#24]`.

## 6. Commands

```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Per-variant scoring used one `link_so` plus:

```
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --top 0
```

Side-by-side disassembly:

```
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```

MIR / MBP dumps:

```
CMD=$(python3 -c "import json; d=json.load(open('cmake-build-match/compile_commands.json')); \
  print(next(e['command'] for e in d if e['file'].endswith('Generator.cpp')))")
eval "${CMD/-o CMakeFiles\/gof2.dir\/src\/game\/mission\/Generator.cpp.obj/-o /dev/shm/w5709/out.o -mllvm -print-machineinstrs -mllvm -print-machine-bfi}" \
  > /dev/shm/w5709/mi.txt 2>&1
```

Scratch files from this pass live under `/dev/shm/w5709/` (`Generator.cpp.base`,
`orig.asm`, `base.asm`, `mi.txt`, `all.txt`, `batch*.py`, `search_big.py`,
`weights.py`).  They are not part of the repo.
