# MenuTouchWindow C1/C2 wave 107 (w5518): defer the cinematic gameText load
# past `operator new` (95.451807% -> 95.481928%, 9600 B)

Date: 2026-11-04. Worktree `wt/w5518`, branch `wave107-w5518`, base
`f857b5171`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before:** 9600 B, 3320 instructions, exact difflib ratio
  **0.9545180722891567** (95.451807%), 2094 raw-diff lines.
* **After:** 9600 B, 3320 instructions, exact difflib ratio
  **0.9548192771084337** (95.481928%), **2091** raw-diff lines.
* The `GameText *cinematicText = Globals::gameText;` declaration in the
  iPad cinematic block was hoisting the `Globals::gameText` load *before*
  `operator new`.  The original loads it *after* `new` (0x123aea:
  `ldr r0,[sp,#96]; ldr r0,[r0,#0]` follow the `blx _Znwj`).  Folding the
  assignment into the first ctor argument
  (`*(cinematicText = Globals::gameText)->getText(497)`) removes the
  hoist and recovers the original order without changing the function
  size or instruction count.
* Gate before and after: `byte_exact 4500 / linked_exact 4500`,
  `size 4524/4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS**. No baseline update. No section/extab change.

## 1. The change

`src/game/ui/MenuTouchWindow.cpp`, iPad cinematic block:

```cpp
        GameText *cinematicText;                       // was: GameText *cinematicText = Globals::gameText;
        this->cinematicBtnB = new TouchButton(
            *(cinematicText = Globals::gameText)->getText(497), 0, Globals::w >> 1,
            ...
        this->cinematicBtnA = new TouchButton(
            *cinematicText->getText(496), 0, Globals::w >> 1,
            ...
```

Only the **first** use is rewritten.  Rewriting the 496 use too (or
dropping the local entirely) shrinks the function to 9592 B / 3317
instructions and drops the score to ~93.3% (the local is load-bearing
for the *loop* register allocation; see §3).

## 2. Why this works (mechanism, proven)

* With the initialised declaration clang emits
  `ldr r0,[sp,#96]; ldr.w r9,[r0]` **before** `movs r0,#200; blx _Znwj`,
  then `mov r0,r9` after `new`.  The original instead emits
  `movs r0,#200; blx _Znwj; ldr r0,[sp,#96]; ldr r0,[r0,#0]`
  (0x123ae2-0x123aec).  Folding the assignment into the argument makes
  clang evaluate `Globals::gameText` after the `new` call, matching the
  original's order.
* The change is byte-neutral in size (the hoisted `ldr.w r9` + `mov r0,r9`
  become `ldr r0` after `new`), so all branch targets and literal-pool
  slots stay put; only the 3 raw-diff lines in that block disappear.

## 3. Measured table (official verifier, full TU)

`orig` is always 9600 B / 3320 instructions.  `fz` is the exact difflib
ratio from `tools/verify/verify.py`'s normalised stream.  Scratch base =
`/dev/shm/w5518/base.cpp` (pristine HEAD).

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| **HEAD (hoisted `cinematicText`)** | **9600** | **3320** | **0.95451807** | **2094** |
| **assignment folded into 497 arg** | **9600** | **3320** | **0.95481928** | **2091** |
| `cinematicText = 0;` + folded assign | 9600 | 3320 | 0.95481928 | 2091 |
| folded assign + redundant `= Globals::gameText` | 9600 | 3320 | 0.95481928 | 2091 |
| `(cinematicText = Globals::gameText, cinematicText)` | 9600 | 3320 | 0.95481928 | 2091 |
| 497 direct, 496 via local | 9600 | 3320 | 0.95451807 | 2094 |
| 496 direct too ("both direct") | 9592 | 3317 | 0.93325298 | 2939 |
| 495 via local | 9608 | 3321 | 0.91311549 | 2942 |
| `GameText *volatile cinematicText` | 9596 | 3319 | 0.92935683 | 2386 |
| declare local at function top / before `if` | 9600 | 3320 | 0.95481928 | 2091 |

Other levers re-measured this wave (all neutral or worse, exact numbers
in the session scratch `/dev/shm/w5518/`): loop shape (`while`,
`do/while`, `++i`, `i += 1`), `special` spellings (`i > 7`, `i >= 8`,
`(i & ~1) == 8`, `i / 8 == 1`, `i >> 3 == 1`, `i == 8 || i == 9`),
loop variable types (`bool`/`int`/`char`), `fontValue`/`langValue`/
`ctorFont` signedness, `specialFontPtr` placement, and the
`sliders[2]` y-expression regrouping.

## 4. Remaining delta (proven)

The remaining 2091 raw-diff lines are dominated by four RA/scheduling
fixed points, all re-verified this wave:

1. **Mission block `adds r4,r5,#2`** (0x122da6 in the original): the
   default `ScheduleDAGRRList` always sinks the `row + 2` ADD to just
   before the `addButton` call, while the original hoists it above the
   `getText(395)` call.  `-mllvm -pre-RA-sched=source` reproduces the
   original shape but changes the frame from 0x174 to 0x184 and drops the
   whole TU to 46.7%, so the original was *not* built with source
   scheduling.  Every source spelling tried (`int id = row + 2;`
   before/after the call, before the `if`, `row += 2`, `volatile`,
   `__builtin_assume`, `String` local, pointer round-trip) is moved back
   by the scheduler; this is a 2-byte shift that re-labels every later
   branch target and literal-pool slot.
2. **Language loop `lang`/`i` r8<->r9 swap** (original `mov r9,r8` at
   0x123474 is missing in ours).  `i != 10` is required (`i < 10` folds
   the value-select PHI and costs ~23 points); no source lever found.
3. **Cinematic block `mov sl,r4`** (ours) vs `ldr.w r9,[sp,#92]`
   (original): `sl` is reused for `&Globals::w` because `r9` holds the
   `cinematicText` pointer.  Dropping the local fixes this block but
   shifts the loop allocation and shrinks the function by 8 B.
4. **iPad slider y-tree association** (`sliders[0..3]` setPosition):
   the original groups the sum as
   `0x2fc + ((0x20+0xc) + 0x154 + 3*0x2c) + 0x164 + 2*(0x15c+0x1c+0x2c)`;
   ours groups it as `0x154 + 2*(0x15c+0x1c) + 0x20 + 0xc + 5*0x2c +
   0x164 + 0x2fc`.  Rewriting the source with the original grouping
   reproduces the tree but changes the surrounding register allocation
   and costs ~0.36 points.

## 5. Proven vs hypothesis

**Proven (measured):**
* The original loads `Globals::gameText` after `operator new` in the
  cinematic block; the folded-assignment spelling reproduces that order
  with identical size and instruction count.
* Only the first (497) use may be rewritten; the 496 use must keep the
  local or the function shrinks to 9592 B and the loop allocation moves.
* The default scheduler (not `source`) is the original's scheduler:
  `-pre-RA-sched=source` gives 46.7% for the TU.
* Gate is green: byte 4500 / linked 4500 / size 4524 / data 843 /
  imports 207 / lint CLEAN / drift 0 / RATCHET PASS.

**Hypothesis (not proven):**
* The original source probably wrote `*Globals::gameText->getText(497)`
  directly and the compiler rematerialised the load for the 496 use;
  no spelling reproduces both the cinematic block and the loop
  allocation simultaneously (the local is needed for the latter).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5518
TMPDIR=/dev/shm/w5518/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5518/tmp cmake --build cmake-build-match --target check -j6
# per-function score:
python3 /dev/shm/w5518/score2.py _ZN15MenuTouchWindowC1Ei

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x123ae2 --stop-address=0x123b00 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x123ae2 --stop-address=0x123b00 \
  cmake-build-match/libgof2hdaa.so
```

REA evidence id: `ev_5c2683c3de0b3e59e495e94516493b3b5106b7379d148749a66a6fb3791db9ab`.

Scratch (not tracked): `/dev/shm/w5518/` (`base.cpp`, `cinater.cpp`,
`score2.py`, `run2.sh`, `vrun2.py`, `vrun3.py`, `probe.py`,
`v*.json`, `orig_full.asm`, `on.txt`, `un.txt`).
