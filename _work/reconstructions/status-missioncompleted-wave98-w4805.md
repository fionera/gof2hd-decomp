# `Status::missionCompleted` — dropping the `(short)` size anchor: 57.9% -> 60.4%
# at 1432 B (w4805, wave 98)

Status: **landed pass**. Worktree `wt/w4805`, branch `wave98-w4805`, base
master `fe4f5e7cf`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903` / LLVM `1d739ffb`, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN6Status16missionCompletedEbbx`
(`Status::missionCompleted(bool, bool, long long)`), original `0xabf38`,
size **1432 (0x598)**.

## 0. Gate

```
cmake --build cmake-build-match --target check -j8
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

The change is gate-neutral (byte 4500 -> 4500, linked 4500 -> 4500,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
`.text` still `0x1337c8`, `.rodata` `0x41b67`, `.ARM.exidx` `0x6f70`,
`.ARM.extab` `0x26294`). It improves the target's fuzzy score
**57.9 -> 60.4** (`report.json` row: `pct 60.4`, `osize 1432`,
`usize 1432`).

## 1. The landed change

One line in `src/game/mission/Status.cpp`:

```cpp
-        switch ((short) m->getType()) {
+        switch ((int) m->getType()) {
```

The case-166 body (the wave-97 ship null-check shape) is unchanged.

## 2. Why `(short)` was there, and why it is no longer needed

The original dispatch is a plain `int` switch:

```
abfd6  blx  Mission::getType
abfda  sub.w r1, r0, #150
abfde  cmp  r1, #39
abfe0  bhi.n ...
abfe2  tbh  [pc, r1, lsl #1]
```

There is **no `sxth`/`uxth`** anywhere in the original dispatch. The
`(short)` cast (added in w3472 as a size anchor, see
`Status-missionCompleted-4bytes-w3472.md`) inserts `sxth r0, r0` (2 bytes,
not in the binary) before the high-group `sub.w`, and changes the low-group
dispatch to signed-narrow lowering. It was only needed because the faithful
direct-`int` form with the *plain* case 166 was 1428 B (4 B short, which
shifts `.text` and breaks the Level `.rodata` block).

Wave 97 then landed the case-166 ship null check
(`status-missioncompleted-wave97-w4703.md`). That check is also a size
anchor: with it the direct-`int` form is **1432 B**, so the `(short)` cast is
now redundant. Removing it recovers the original dispatch (no `sxth`) and
raises the fuzzy score from 57.9 to 60.4 with every gate count unchanged.

## 3. Case 166 in the direct-`int` build

`cmake-build-match/libgof2hdaa.so` at `0xac318`:

```
ac318  mov  r0, sl
ac31a  blx  Mission::getProductionGoodIndex
ac31e  mov  r9, r0                 ; index -> r9
ac320  mov  r0, sl
ac322  blx  Mission::getProductionGoodAmount
ac326  mov  r5, r0                 ; amount -> r5
ac328  ldr.w r0, [r6, #400]        ; this->ship, this stays in r6
ac32c  cbz  r0, ac342              ; the null check (extra vs original)
ac32e  blx  Ship::getCargo
ac332  mov  r2, r0
ac334  mov  r0, r9
ac336  mov  r1, r5
ac338  blx  Item::isInList
ac33c  cmp  r0, #0
ac33e  bne.w ac498
```

The original (`0xac260`) is identical except that it has no `cbz` between
the ship load and `getCargo`:

```
ac26e  mov   r5, r0                ; amount -> r5
ac270  ldr.w r0, [r6, #400]        ; this->ship
ac274  blx   Ship::getCargo
ac278  mov   r2, r0
ac27a  mov   r0, r9
ac27c  mov   r1, r5
ac27e  blx   Item::isInList
```

So the direct-`int` build now has the original's case-166 register
allocation (`index -> r9`, `amount -> r5`, ship load `r0`/`[r6,#400]`) and
the original's `cmp r0,#0; bne.w` test. The only case-166 residual is the
single `cbz` from the null check.

The `cbz` is also the mechanism that produces the allocation: it splits the
case body into two MBBs, so `index` and `amount` become *global* intervals
(live across a block boundary) and the greedy allocator processes them by
spill weight instead of by start slot. Without the check the direct-`int`
build is 1428 B and allocates `index -> r5`, `amount -> r6` (evicting
`this`), which is the wave-96/97 "CB" shape.

## 4. Case-166 shape sweep (real NDK build, direct `int`)

All variants keep the rest of the function identical.

| variant | size | case-166 allocation | score |
|---|---:|---|---:|
| plain `ship->getCargo()` (no check) | 1428 | (r5, r6, r0/[r1]) | 59.5 |
| `Ship *s = ship; if (s == 0) return m;` (landed) | 1432 | (r9, r5, r0/[r6]) + cbz | **60.4** |
| `if (ship == 0) return m;` | 1432 | same | 60.4 |
| `if (ship == 0) return 0;` | 1432 | same | 60.4 |
| `if (s == 0) return m; else { ... }` | 1432 | same | 60.4 |
| `if (ship != 0 && isInList(...))` | 1428 | (r9, r5, r0/[r6]) + cbz | 60.4 |
| `if (missions != 0 && isInList(...))` | 1432 | (r9, r5, r0/[r6]) + cbz | 60.4 |
| `if (ship == 0) break;` | 1432 | (r9, r5, r0/[r6]) + cbz | 57.8 |
| `if (s) { ... }` / `if (!s) break;` | 1432 | (r9, r5, r0/[r6]) + cbz | 57.8 |
| `else if` hasEquipment | 1432 | (r5, r6, r0/[r1]) | 56.9 |
| `bool inList = ...; if (inList)` | 1432 | — | 56.9 |
| `goto`/label/`do{}while(0)`/`switch(0)`/`if(1)` | 1432 | (r5, r6) | 56.9 |
| `int index` declared before the `atStation` if | 1428 | (r9, r5, r0/[r6]) | 56.8 |

Every 1432 B variant scores 60.4; the score is saturated by the body-order
residual, not by the exact case-166 check shape. The `(short)` cast is the
only lever that moves the score in this pass.

## 5. What is still different

The original lays the 40-entry high-group TBH bodies out in **source order**
(`167/174 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163
164 165 168 169 171 172 ...`); clang 7's `MachineBlockPlacement` lays them
out in **ascending case value** (the jump-table successor/worklist order).
That is the dominant remaining difference and is a compiler-side MBP
behaviour, not a source-shape one: reordering the source case labels leaves
the layout ascending (measured), and the wave-96 exact oracle reproduces the
original order only with `MBP_SORT_SBCB` (sort the placement worklist by MBB
number). The wave-91 two-switch form reproduces the order exactly but costs
+76 B. See `status-missioncompleted-wave96-w4601.md`,
`status-missioncompleted-wave97-w4703.md` and
`status-missioncompleted-two-switch-order-w4103.md`.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4805
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8   # RATCHET PASS
cmake --build cmake-build-match --target verify -j8  # report.json: pct 60.4
```

Scratch (not committed): `/dev/shm/w4805/` — `cc.sh` (NDK clang++ -c with the
project flags), `dump.sh`, `classify2.py`, `score_fn.py` (imports
`tools/verify/verify.py` and scores one function), `batch_cast.py`,
`bodies*.py`, `tbh2.py` (TBH layout decoder), `base.cpp` (pre-change
`Status.cpp`).
