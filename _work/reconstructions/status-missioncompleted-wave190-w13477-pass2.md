# `Status::missionCompleted` — wave 190 (w13477 pass 2): restore the case-208
# `Globals::status` chain and the plain case-20 compare

Worktree `wt/w13477`, branch `wave190-w13477`, base master `7db176c4d`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13477`.

**Status: landed.**  Raw **135 -> 97** (-38), aligned **173 -> 54**,
size **1,432/1,432** exact.  `.text` differing bytes **71,946 -> 71,908**
(-38); `.ARM.extab` **578** unchanged (size/base exact at `0x1a3b78`,
156,312 B); build-id 20 unchanged.  Total (text+extab+build-id)
**72,544 -> 72,506** (-38).  `.rodata` unchanged (size 269,644, +0x1e5;
original 269,159 B content exact), `.rel.plt` byte-identical, 27/27
section headers, file size 2,187,328 exact.  Gate: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207,
stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370
(`RATCHET PASS`).

## 1. What changed

Two source changes, both restoring the original's own expressions:

```cpp
// case 208
- if (m->isCampaignMission() && currentCampaignMission == 0x8f && !atStation &&
-     __builtin_expect(ship != 0, 0)) {                       // lint: arm_builtin
+ if (m->isCampaignMission() && Globals::status->currentCampaignMission == 0x8f && !atStation) {

// case 20
- if (Globals::status->field_178 >= 0 &&
-     Globals::status->field_178 >= (unsigned short) m->getStatusValue()) {
+ if (Globals::status->field_178 >= m->getStatusValue()) {
```

Both are the faithful original expressions: the original's case-208 body
loads `Globals::status` through the GOT chain and its case-20 body is a
plain signed compare.  The previous devices (this-member read + `ship`
expect guard; `>= 0` + `unsigned short` cast) were compensating for the
case-10 body's 6-byte deficit; once pass 1 moved the case-10 dead
re-query after the `isInList` test, the two devices could be dropped
together and the function stays exactly 1,432 B.

## 2. Why this is a win

The case-208 body is now byte-identical to the original
(`ac060..ac07f`, including the `Globals::status` GOT chain at `ac068`),
and the case-20 body is byte-identical (`ac3de..ac3fb`, including the
`nop` at `ac3f6` and the case-208 `Globals` literal at `ac3f8`).  The
first literal island is back to the original's 4 B `Globals` literal at
`ac3f8` (the rank-table literal is still at `ac300`), and the case-14
body lands on the original `ac32a`.

The 38-byte drop is almost entirely the two restored bodies:

* case 208: **18 B** of diff removed (the `ldr.w [r6,#488]` /
  `ldr.w [r6,#400]` direct reads and the `expect` guard become the
  original's four-instruction GOT chain and `beq.w`);
* case 20: **18 B** of diff removed (the `cmp r5,#0; blt` and `uxth`
  devices become the original's plain `cmp r5,r0`), and the `nop` +
  `Globals` literal at `ac3f6`/`ac3f8` match;
* the remaining 2 B is the tail realignment.

## 3. Measured combinations (case-10 = pass-1 `I,C,A,T,V,H`)

| case 208 | case 20 | raw | aligned | size |
|---|---|---:|---:|---:|
| this + `expect(ship!=0,0)` | `>=0` + `(unsigned short)` (pass 1) | 135 | 173 | 1432 |
| **Globals, no expect** | **plain `>=`** (landed) | **97** | **54** | **1432** |
| Globals, no expect | `>=0` + `(unsigned short)` | 411 | 156 | 1440 |
| Globals, no expect | `(unsigned short)` only | 105 | 56 | 1432 |
| Globals, no expect | `>=0` only | 406 | 155 | 1436 |
| this + `expect` | plain | 400 | 182 | 1428 |

The `(unsigned short)`-only row is also size-exact but 8 raw bytes worse
(the `uxth` does not match the original's `cmp r5,r0`).

## 4. Residual (97)

* Dispatch 4 B: `blx helper` vs the original's `sub.w r1,r0,#150`
  (and `cmp r0` vs `cmp r1`).
* First TBH 36 B: rank-indexed vs the original's type-indexed table.
* Second dispatch 2 B: `sub #208` vs the original's `sub #8`.
* Second-dispatch default 1 B: `cmp r0,#200` vs `cmp r0,#0`.
* Case-6 tail merge 6 B: `mov r8,r4; lsls; ldr r6` vs the original's
  `ldr r6; mov r8,r4; lsls` (documented in w13423).
* Case-10 body 43 B: index/cargo/amount register order (`r5/r6/r1` vs
  the original's `r9/r5/r2`) and the `hasEquipment` tail
  (`cmp; b ac1d4` vs `b ac458`).
* End pool 1 B: the case-10 `Globals` literal PC-relative offset.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13477
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=97 aligned=54 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 6. Files changed

* `src/game/mission/Status.cpp` — case 208 uses
  `Globals::status->currentCampaignMission` (no expect guard); case 20
  is the plain signed compare.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-190 numbers.
* this note.
