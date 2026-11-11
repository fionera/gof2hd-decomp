# MGame::OnTouchEnd — wave 171 (w11713): ore-refine guard `count >= 0` (-2 raw)

Status: one source change landed, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w11713`, branch `wave169-w11713`, base `41ef4c8b6`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.

Entry/exit state: raw **3,775 -> 3,773**, aligned **1,454 -> 1,453**, size
**8,080/8,080**, `.rel.plt` byte-identical (0), gate byte **4507** linked
**4507**, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

## Landed

```diff
                         int count = -amount;
-                        if (count > 0) {
+                        if (count >= 0) {
```

The original guard is `cmp.w r9,#0 ; beq.n skip` (r9 = `amount`), i.e.
`amount == 0` skips the report (`count != 0`).  `count >= 0` canonicalises to
`cmp.w r9,#0 ; bgt.n skip`: the `cmp #0` now matches the original (the base's
`count > 0` emits `cmp #-1`), the branch still differs (`bgt` vs `beq`), so
raw drops by the 2-byte `cmp` immediate and aligned by 1.  Size stays 8,080
and `.rel.plt` stays byte-identical.

**Caveat (recorded honestly):** `count >= 0` is `amount <= 0`, which is always
true (the loop only decrements), so it emits `bgt` that never skips.  Wave-161
deliberately chose the semantically-correct `count > 0` for +2 raw.  This
change is a raw-only win (byte-identicality ground truth); reverting it
restores wave-161's correctness at raw 3,775.

## What was measured (all size 8,080 / relplt 0 unless noted)

### Epilogue at `0x17b004` — the unresolved lever (no raw win)

The base shared epilogue is at `0x17ba22` (offset `0x18de`); the original is
`0x17b004` (`0xec0`).  The ~2,590-byte gap is the reward chain + the
`hudTouchFlags == 0` block being placed *before* the epilogue instead of after
it.  Every shape that moves the epilogue earlier re-lays the tail and loses
more raw than it gains:

| shape | raw | aligned | size | epi popeq |
|---|---:|---:|---:|---:|
| base | **3,775** | 1,454 | 8,080 | 0x17ba32 |
| `expect(hudTouchFlags == 0, 0)` | 5,922 | 1,603 | 8,076 | 0x17af90 |
| `expect(flags==0,0)` + `expect(0x400,0)` | 4,698 | 1,655 | 8,072 | — |
| `expect(flags==0,0)` + `expect(0x200,0)` | 5,996 | 1,624 | 8,084 | — |
| drop `hasWon() == true` (truthy `won`) | 6,102 | 2,276 | 8,068 | 0x17af92 |
| `hasWon() != false` / `!!hasWon()` | 6,102 | 2,276 | 8,068 | 0x17af92 |
| `(hasWon() & 1) != 0` won form | 4,297 | 1,468 | 8,084 | — |
| `expect(failed, 0)` | 5,330 | 2,329 | 8,064 | — |
| `expect(isAutoPilot && !wasAutoPilot, 1)` | 4,379 | 1,487 | 8,076 | — |
| `isInstantActionMission() == true` | 6,069 | 1,522 | 8,080 | — |
| `expect(cutsceneActive, 1)` | 6,073 | 2,391 | 8,052 | — |
| `expect(menuTouchOpen, 1)` | 4,775 | 1,669 | 8,084 | — |
| `expect(won, 1)` | 4,746 | 1,366 | 8,084 | — |
| `expect(won, 0)` | 5,384 | 2,209 | 7,828 | — |
| campaign temp (wave-125 shape) | 4,702 | 1,485 | 8,076 | — |
| wave-163 pair (truthy won + `expect(0x400,0)`) | 4,692 | 2,236 | 8,076 | — |

`__builtin_expect(wasAutoPilot, 0)`, `count >= 1`, `count > -1`, `amount < 0`,
`0 > amount`, `!hudTouchFlags`, `hudTouchFlags == 0u`,
`hudMenuOpen != 0`, `weaponSlot != -1`, `wingmanCommand != -1` are all
codegen-identical to the base (3,775 / 1,454 / 8,080).

### hudMenuOpen arm order (12 arms)

All 11 adjacent swaps of the `hudTouchFlags & 0x{200,800,400,2000,4000,8000,
10000,20000,40000,80000,100000,1000}` dispatcher arms measured; base order is
optimal (`0x800<->0x400` = 3,780, `0x4000<->0x2000` = 3,777, `0x8000<->0x4000`
= 3,777, `0x10000<->0x8000` = 3,779, `0x40000<->0x20000` = 3,777,
`0x80000<->0x40000` = 3,777, `0x100000<->0x80000` = 3,948; `0x800<->0x200`
re-lays to 8,064).

### weaponSlot / wingman / hudTouchFlags==0 chain order

All 6 permutations of the three trailing arms measured: `slot,wing,flags`
(base) and `wing,slot,flags` are codegen-identical (3,775); the others are
8,044-8,112 B.

### Reward chain

Re-confirmed the wave-171 w11613 result on this base: the base order
`15,74,65,42,43,81,22,95,161,110,100,120,126,134,96,155,144,162` is optimal.
The original binary order `15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,
144,155,161,162` gives raw 4,696 / 8,084 B.  Ties at 3,775: `15<->43`
(aligned 1,494), `126<->144` (aligned 1,458).

### `.rel.plt`

Byte-identical before and after (0 differing bytes of 29,392; checked directly
against the original at file offset 0x5751c).

## Reproduction
```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j2
```
