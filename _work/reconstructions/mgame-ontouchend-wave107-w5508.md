# MGame::OnTouchEnd (8080 B) — w5508 (wave 107): the size/PLT lock, and why
# the tuned 80 % shapes cannot pass the gate

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144,
8080 B, `src/game/menu/MGame.cpp`).  Baseline at HEAD (`f857b5171`):

| shape | size | fuzzy | .rel.plt diff | byte_exact | gate |
|---|---:|---:|---:|---:|---|
| **HEAD** | **8080** | **78.9** (78.8829, 1338/2775) | 0 | **4500** | green |
| best tuned, gate-failing | 8080 | 80.0 | 12 | 4498 | — |
| best tuned overall | 8096 | 81.9 | 9 | 3102 | — |

Gate after HEAD: `byte_exact 4500 / linked_exact 4500`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

**No source change landed this pass.**  Every shape that scores above 78.9
either changes the length of `MGame::OnTouchEnd` or permutes `.rel.plt`, and
both of those are hard failures for `byte_exact` (see §1).  The note records
the lock, the full measurement table, and where the remaining 21 % lives.

## 1. The hard constraint: 8080 B *and* an unchanged `.rel.plt`

Two independent global effects make any local edit of this function pay a
1400-function or a 1–2-function penalty:

1. **`.text` total size.**  HEAD `.text` is `0x1337c8` bytes and `.rodata`
   starts at `0x1c9e10`.  Growing `MGame::OnTouchEnd` by 4 B (`nested`,
   8084 B) grows `.text` by **0x20** (section alignment), moves `.rodata` to
   `0x1c9e30`, and drops `byte_exact` **4500 → 3102** (every function with a
   link-time pool word pointing into `.rodata` changes).  The function must
   stay exactly 8080 B.
2. **`.rel.plt` first-reference order.**  gold emits PLT slots in the order
   the linker first sees each undefined symbol; `MGame::OnTouchEnd` is the
   first referencer for a contiguous 13-symbol window
   (`orig .rel.plt[3321..3333]`):

   ```
   3321 PlayerEgo::dockToStream(bool)          callers: OnTouchEnd 0x17ae16, dockEventEii 0x17fc16
   3322 MGame::useCloak()                      callers: OnTouchEnd 0x17aea6
   3323 MGame::maneuverTouchEnd(int,int,void*)  callers: OnTouchEnd 0x17aeb0
   3324 Mission::isInstantActionMission()      callers: OnTouchEnd 0x17af7e
   3325 LevelScript::resetStartSequenceOver()  callers: OnTouchEnd 0x17b0ae
   3326 Level::removeObjectives()              callers: OnTouchEnd x3, successCheck 0x18025a
   3327 PlayerEgo::goingToWaypoint()           callers: OnTouchEnd x2, successCheck 0x18016a
   3328 PlayerEgo::removeRoute()               callers: OnTouchEnd x2, successCheck 0x18017a
   3329 LevelScript::skipCutscene()            callers: OnTouchEnd 0x17b646
   3330 MenuTouchWindow::inCinematicMode()     callers: OnTouchEnd x2
   3331 MGame::setCinematicMode(bool)          callers: OnTouchEnd x2
   3332 Hud::resetAnalogStick()                 callers: OnTouchEnd 0x17b8ec
   3333 Radar::getLockedEnemy()                 callers: OnTouchEnd 0x17bf12
   ```

   Any permutation of this window renumbers the PLT slots between the old and
   new positions, so every `bl` to a shifted slot changes bytes.  Because
   `removeObjectives` / `goingToWaypoint` / `removeRoute` are also called by
   `MGame::successCheck` and `dockToStream` by `MGame::dockEventEii`, moving
   any of those four costs `byte_exact`; moving only symbols whose callers are
   all inside `OnTouchEnd` is free.  Measured: `cam0` (3 slots) → 4499,
   `disp`/`exphw1`-family (9–13 slots) → 4498–4499.

   HEAD's window is byte-identical to the original's, which is why HEAD is
   green.  The original's order is itself a fingerprint of the original block
   layout: it proves that in the original `resetStartSequenceOver`'s call site
   precedes `removeObjectives`'s, and `maneuverTouchEnd`'s precedes
   `isInstantActionMission`'s.

## 2. Where the 21 % lives

The function is **instruction-for-instruction identical to the original up to
offset 0xbb8** (deltas 0x0000–0x0bb8).  The call sequence is the same multiset
in both (458 calls each); the only differences are block order and the branch
targets that follow from it.  Cumulative address delta (ours − orig), HEAD:

```
0x0000–0x0bb8   0
0x0c58         +4      (orig `b.n loc_e9e` vs ours `b.w loc_17f0`: +2 B)
0x0c7c–0x0df8  +10 … +0x64
0x0ec2         −0x2a
0x0f28–0x11d6  −0x4e … −0x1a
0x1288         +0xa2
0x1310–0x13b0  +0xa6 … +0x11a   (campaign switch chain, max shift)
0x1502–0x1a52  +0xa8 … +0x48
0x1aec–0x1c2c  +0xc … +0x4
0x1c4c–0x1ebe   0               (weaponSlot/wingman re-align)
0x1ebe–0x1f90  epilogue/EH
```

Region loss (mismatching insn slots, HEAD): campaign chain **570**,
mission dispatch + tail **189**, weapon/wingman **168**, menus **29**,
EH **17**, mining **12**, prologue **5** (990 total).

The single structural cause is the placement of the `hudTouchFlags == 0`
cluster.  Original order (VMA):

```
0x17afe2  hudTouchFlags == 0 test        (ldr [fp,#248]; cbz 0x17b01a)
0x17afe8  isAutoPilot tail               (shared by camera 3/0, weaponSlot, hud0)
0x17b004  epilogue
0x17b016  __stack_chk_fail
0x17b01a  hud0 body (closeHudMenu first, unguarded)
0x17b07e  freelance mission failed
0x17b0aa  campaign switch chain
0x17b9ca  weaponSlot body                (ends b.w 0x17afe8 — skips the test)
0x17bf4a  wingman body                   (ends b.w 0x17afe2 — enters the test)
```

HEAD reproduces the same *internal* order `test → tail → epilogue → hud0`, but
at the end of the function (0x17f0/0x17f6/0x1824/0x1828) instead of at
0x0e9e.  In HEAD the `weaponSlot` body falls through into the test; in the
original the `weaponSlot` body jumps to the tail and the *`UseKhadorDrive`
block* falls through into the test.  This is an MBP trace choice, not a
content difference.

The `nested` source shape (faithful: `} else { if (wingmanCommand >= 0) {…}
if (hudTouchFlags == 0) {…} }`) does move the test early — to 0x0d6e, right
after the `UseKhadorDrive` block, i.e. the right *place* — but it lays out
`test → hud0 body` and leaves the tail at 0x182c.  Adding
`__builtin_expect(hudTouchFlags == 0, 0)` (`nested+exphf0`) restores the
original `test → tail` order at 0x0d68/0x0d72, but the epilogue then lands at
0x0e3a (original 0x0ec0) and the `b.w`/`b.n` at 0x06ae flips to `b.n`,
shifting the whole literal pool; fuzzy falls to 73.6.

## 3. Measurements (official `tools/verify` fuzzy; HEAD = 78.9 / 8080 B / 4500)

`plt` = number of differing positions in `.rel.plt` vs the original.

| variant | size | fuzzy | plt | byte |
|---|---:|---:|---:|---:|
| HEAD | 8080 | 78.9 | 0 | 4500 |
| `ap1` / `camdef` / `camif2` / `hudnot` / `hudrev` / `negwon` | 8080 | 78.9 | 0 | 4500 (code-identical) |
| `he` (hud0 as `else if` of the weaponSlot chain) | 8080 | 76.0 | 0 | 4500 |
| `cam0` (camera switch case 0 before 3) | 8080 | 78.6 | 3 | 4499 |
| `bool` (`bool refinedAny`, faithful) | 8076 | 76.4 | 0 | 4480 |
| `mf` (faithful mining `if (amount != 0)`) | 8072 | 70.9 | 0 | 4283 |
| `nested` (faithful wingman nesting) | 8084 | 76.6 | 0 | 3102 |
| `nested+bool` | 8080 | 74.7 | 0 | 4500 |
| `nested+exphf0` | 8084 | 73.6 | 0 | 3102 |
| `nested+exphf0+bc0` | 8080 | 74.6 | 0 | 4500 |
| `he+bool` | 8080 | 73.6 | 0 | 4500 |
| `disp` (campaign-first mission dispatch) | 8088 | 79.0 | 4 | 3102 |
| `exphw1` (`expect(hasWon…,1)`) | 8072 | 75.1 | 9 | 4283 |
| `hm1` (`expect(hudMenuOpen,1)`) | 8084 | 73.9 | 13 | 3102 |
| `disp+exphw1` | 8088 | 80.8 | 9 | 3102 |
| `disp+exphw1+bc1e+bool` | **8080** | **79.8** | 9 | **4499** |
| `disp+nested+hm1+exphf1` | 8080 | 79.9 | 13 | 4498 |
| `disp+nested+hm1+exphf1+cam0` | **8080** | **80.0** | 12 | **4498** |
| `disp+nested+bool+mf` | 8080 | 74.7 | 4 | 4499 |
| `disp+exphw1+nested` | 8096 | **81.9** | 9 | 3102 |
| `hud0up` (hud0 hoisted before weaponSlot) | 8048 | 72.9 | 11 | 3133 |

Exhaustive searches run this pass (all filtered to size 8080):
pairs over 17 transforms, triples over 12, and 500 random 2–4 transform
combinations over 42 transforms.  **Nothing at 8080 B with `plt == 0` beats
HEAD's 78.9**; every such combination is either code-identical to HEAD or
scores 70–77.

## 4. The mining block is 16 instructions from the original

The binary proves the mining loop's *shape*: `sub.w r9,r9,#1` (a down counter
in `r9`), `rsb r1,r9,#0` for `String(-amount)`, and a bit-0 test
(`lsls r0,r0,#31`) on `refinedAny` — i.e. `bool refinedAny`, `amount--`,
`String(-amount)`.  HEAD's tuned spelling (`int refinedAny`,
`int count = -amount; if (count > 0)`) reproduces the down counter and the
`rsb` exactly, and differs from the original in only 16 instructions of the
~90-instruction block:

```
orig  0x0486  cmp.w r9,#0        ours  cmp.w r9,#-1        (if (amount != 0) vs if (count > 0))
orig  0x048a  beq.n  loc_50e     ours  bgt.n  loc_50e
orig  0x04a4  add r0,sp,#116     ours  add.w r8,sp,#80     (scheduling of the String chain)
orig  0x0528  lsls r0,r0,#31     ours  cmp r0,#0           (bool refinedAny vs int)
```

The faithful spellings each cost size or layout and score worse:
`bool` alone 8076 B / 76.4, `mf` (`if (amount != 0)`) 8072 B / 70.9,
`nested+bool` (both faithful) 8080 B / **74.7**.  The reason `mf`/`bool` hurt
is that they make clang strength-reduce `amount--` + `-amount` into
`amount++` + `amount` and swap the `fee`/`amount` registers (`r8`/`r9`), which
perturbs the whole allocation; `swapfa` (declaration-order swap) does not
undo it.  HEAD's `count > 0` is the only spelling found that keeps `amount`
negative in `r9`, at the cost of `cmp #-1; bgt` instead of `cmp #0; beq`.

## 5. Proven vs hypothesis

**Proven from the binary**

* `.rel.plt[3321..3333]` order and the caller sets in §1 (objdump of the
  original; `readelf -rW`).
* The original block order in §2 (`test → tail → epilogue → hud0` at
  0x0e9e…0x0ed6; weaponSlot 0x1886 ends `b.w 0x0ea4`; wingman 0x1bf4a ends
  `b.w 0x0ea2`).
* The faithful mining shape and the `r8`/`r9` swap (§4).
* HEAD is instruction-identical to the original for the first 0x0bb8 bytes.

**Hypothesis / tuned**

* `int refinedAny` (HEAD) instead of `bool` is non-faithful but is the only
  size-neutral compensation found in wave 106; it is kept.
* The remaining mismatch is entirely MBP block placement.  Matching it needs
  the `hudTouchFlags == 0` cluster to be placed after the `UseKhadorDrive`
  block (0x0e9e) instead of after the `weaponSlot` body (0x17f0), without
  changing the function's length or the `.rel.plt` window.

## 6. Exact commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5508
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5508/score.py            # official fuzzy for this function
python3 /dev/shm/w5508/exact.py            # matched-insn ratio
python3 /dev/shm/w5508/probe3.py <combo>   # size + fuzzy + .rel.plt diff + byte_exact
python3 /dev/shm/w5508/bexact.py <so>      # true byte_exact count
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch (not tracked): `/dev/shm/w5508/` (`transforms.py` with the full
transform table incl. `he`, `hud0up`, `negwon`, `camdef`, `camif`; `probe3.py`,
`pltcmp.py`, `bexact.py`, `delta.py`, `idx.py`, `show2.py`, `pair2.py`,
`trip.py`, `rnd2.py`, `head.so`, `v80.so`, `full.s`).
