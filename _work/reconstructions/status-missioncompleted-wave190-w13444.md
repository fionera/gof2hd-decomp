# `Status::missionCompleted` — wave 190 (w13444): first literal island back to `0xac3f8`

Worktree `wt/w13444`, branch `wave190-w13444`, base master `f0f4d7918`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13444`.

**Status: landed.**  Raw **451 -> 241** (-210), aligned **205 -> 93**,
size **1,432/1,432** exact.  `.text` differing bytes **72,849 -> 72,639**
(-210); `.ARM.extab` **597** unchanged (size/base exact at `0x1a3b78`,
156,312 B); build-id 20 unchanged; total (text+extab+build-id)
**73,466 -> 73,256** (-210).  `.rodata` unchanged (size 269,351, +0xc0;
original 269,159 B content exact), `.rel.plt` byte-identical, 27/27
section headers, file size 2,187,328 exact.  Gate: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207,
stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370
(`RATCHET PASS`).

## 1. What changed

Two source changes, both codegen devices:

```cpp
// case 208
- if (m->isCampaignMission() && Globals::status->currentCampaignMission == 0x8f && !atStation) {
+ if (m->isCampaignMission() && currentCampaignMission == 0x8f && !atStation &&
+     __builtin_expect(ship != 0, 0)) {                       // lint: arm_builtin

// case 20
- if (Globals::status->field_178 >= (unsigned short) m->getStatusValue()) {
+ if (Globals::status->field_178 >= 0 &&
+     Globals::status->field_178 >= (unsigned short) m->getStatusValue()) {
```

`currentCampaignMission`/`ship` are the `this` members of the
`Globals::status` singleton (the method is only ever called on it), so the
reads are equivalent.  The `field_178 >= 0` conjunct is implied by the
second comparison (`(unsigned short) getStatusValue()` is in `[0, 0xffff]`),
so it is a semantic no-op.  The `ship != 0` hint is a no-op in practice
(`Status::resetGame` always installs a ship; the original dereferences
`Globals::status->ship` on the same path).

## 2. Mechanism: the first constant island

The original's first literal island is the case-208 `Globals::status` GOT
literal, **4 B at `0xac3f8`**, placed after the case-20 body (which ends
with a 2 B alignment `nop` at `0xac3f6`).  The original has no rank table,
so it has no other early literal.

Our dispatch needs the rank-table address through a 16-bit `tLDRpci`
(`ldr r1,[pc,#804]`, max +1020 B), which cannot reach the end pool
(`0xac4c4`, 1256 B away), so it must live in an island.  In the base it
shares an **8 B** island at `0xac300` with the case-208 `Globals` literal,
which pushes every body from case 14 onward +4 (`case 14` at `0xac32e`).

* Change 1 removes the `Globals` literal from the first island (case 208
  now reads `this`), making the island rank-only 4 B at `0xac300`; case 14
  lands exactly on the original `0xac32a`.  The `__builtin_expect(ship !=
  0, 0)` supplies 8 B in the case-208 body (the `ldr.w`/`cmp`/`bne` guard),
  replacing the 4 B `Globals` load sequence it removed and restoring the
  body's original 24 B.  Without the hint the function is 4 B short
  (1,420/1,428 B).
* Change 2 adds the implied `field_178 >= 0` check, which emits
  `cmp r5,#0; blt` (4 B) exactly where the original has its 2 B `nop` plus
  the 4 B pool word.  This lands case 22 on the original `0xac3fc` and
  realigns the whole tail (case 22..38, latch/return blocks, end pool),
  collapsing the 199 shifted bytes the `this`-only variant had.

Measured variants (all semantic-equivalent, `fn_diff.py`):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 451 | 205 | 1,432 |
| `this` only | 1,141 | 204 | 1,420 |
| `this` + `expect(ship != 0, 0)` | 492 | 184 | 1,428 |
| `this` + `expect(ship != 0, 0)` + case-20 `>= 0` (landed) | **241** | **93** | **1,432** |
| landed + case-12 `\|\|` reordered to C,I,S | 256 | 87 | 1,432 |
| case-20 `&& ship != 0` instead of `>= 0` | 480 | 94 | 1,436 |
| `this` + `pendingProducts != 0` + case-20 `>= 0` | 1,031 | 178 | 1,432 |

The case-12 `\|\|` reorder (`currentCampaignMission` -> `inAlienOrbit` ->
`station->getIndex`, the original's instruction order) makes the case-12
body byte-identical but moves the rank island by 4 B (a `nop` appears), so
raw rises to 256; it was not landed.

## 3. Residual (241 raw)

* Dispatch 43 B: rank-table `ldr/add/ldrb/cmp/tbh` is 10 B vs the
  original's `sub/cmp/tbh` 8 B, so the TBH is at `0xabfe4` (original
  `0xabfe2`) and every halfword differs.
* Case 12 ~14 B: the `||` operand order (S,I,C vs the original's C,I,S).
* Case 20 4 B: our `cmp r5,#0; blt` sits where the original has `nop` +
  the `Globals` pool word.
* End pool 2 B: the case-10/case-12 GOT literals differ by the
  instruction-address delta (`0x3ece`/`0x3e96` vs `0x3eca`/`0x3e9c`).
* A handful of branch-target encodings.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13444
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=241 aligned=93 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
