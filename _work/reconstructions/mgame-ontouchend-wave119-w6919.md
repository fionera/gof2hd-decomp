# MGame::OnTouchEnd (8080 B) — w6919 (wave 119): menuTouchOpen-block predicate tuning

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Branch `wave119-w6919`, base master
`8b63afe8a` (wave-119 head; OnTouchEnd source = w6838 best2).

## Result

* Before (HEAD): **8080 B / 81.2827 %**, `RATCHET PASS`
  (byte_exact 4505, linked_exact 4505, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0).
* After: **8080 B / 81.6429 %**, `RATCHET PASS`
  (byte_exact **4505**, linked_exact **4505**, same size/data/imports).
* No section, `.ARM.extab` or file-size change: 27 sections,
  file 2,187,328 B, `.text` 0x1337c8 @0x69440, `.ARM.extab`
  0x26290 @0x1a3b78, `.rodata` 0x41c27 — all identical to HEAD.

## The change (three predicate spellings, all behaviour-neutral)

```cpp
if (this->starMap->exitRequested) { ... }                  // was: if (this->starMap->exitRequested == true)
if (this->menuTouchOpen) { ... }                           // was: if (this->menuTouchOpen == true)
if (this->menuWindow->OnTouchEnd(x, y, touchId) == true) { ... }  // was: if (this->menuWindow->OnTouchEnd(x, y, touchId))
```

All three are pure boolean reads (`bool` / 0-1 flag / `bool` return), so the
rewrite is behaviour-neutral.  As in w6818/w6838, the point is that
`== true` / bare makes LLVM 7's branch-probability analysis treat the branch
as 50/50 instead of the `isZero` 37.5/62.5 heuristic, which changes
`MachineBlockPlacement`'s candidate choices and therefore the cold-block
layout of the whole 8080-byte function.  The instruction stream stays 8080 B,
so `.rel.plt`, `.text` offsets and every other function's bytes are untouched.

`.rel.plt` first-reference order is preserved: `resetStartSequenceOver`
0x17b160 < first `removeObjectives` 0x17b2ee, so `MGame::successCheck`
stays byte-exact and `byte_exact` stays 4505.

Two of the three edits are *towards* the original source shape
(`exitRequested` and `menuTouchOpen` are bare `if`s in the original
disassembly: `cmp #0; beq`, not `cmp #1; bne`), the third (`OnTouchEnd(...)
== true`) is a probability hint.

## Measurement table (all on the HEAD base; NDK r18b link + verify)

`OK` = `.rel.plt` first-reference order matches the original (reset before
removeObjectives); `ROT` = rotated, which drops `byte_exact` 4505 -> 4504 and
fails the ratchet.

| variant | size | pct | plt |
|---|---:|---:|---|
| HEAD (w6838 best2) | 8080 | 81.2827 | OK |
| MT (`menuTouchOpen` bare) | 8084 | 78.8545 | OK |
| MO (`OnTouchEnd == true`) | 8080 | 79.1569 | OK |
| EX (`exitRequested` bare) | 8076 | 80.0000 | OK |
| MT+MO | 8080 | 81.3547 | OK |
| **MT+MO+EX** | **8080** | **81.6429** | **OK** |
| MT+MO+EX+TB / TN | 8088 | 82.1686 | ROT |
| MT+MO+EX+II | 8092 | 82.1030 | ROT |
| MT+MO+EX+T2 | 8080 | 81.6069 | OK |
| MT+MO+EX+CS | 8080 | 81.6069 | OK |
| MT+MO+EX+{TQ,SB,KB,HF,PA,HM,JA,SC,HK,RC,AT,DK,AP,PA2,ONTE,DLG,FCM,FCM2,BC8_2} | 8080 | 81.6429 | OK (code-identical) |
| MT+EX+CS | 8080 | 80.8503 | OK |
| MT+EX+GO | 8080 | 80.3604 | OK |
| MT+MO+KE | 8084 | 80.1729 | OK |
| MT+MO+GO | 8076 | 80.1946 | OK |
| MT+MO+EX+MA | 8076 | 79.6974 | OK |
| ORD1 (faithful `campaign`/`keepObjectives` before `mission`) | 8080 | 81.4268 | OK |
| ORD2 (`campaign` first) | 8084 | 79.8487 | OK |
| ORD3 (`campaign`, `keepObjectives`, `mission`) | 8072 | 80.1946 | OK |

Ops: MT = `menuTouchOpen == true` -> bare; MO = `OnTouchEnd(...)` ->
`... == true`; EX = `exitRequested == true` -> bare; TB = top `hasWon() ==
true` -> bare; TN = top `hasWon() == true` -> `!= false`; II =
`isInstantActionMission()` -> `== true`; T2 = second `touchesStream` ->
`== true`; CS = `cinematicSteerActive` -> `== true`; MA =
`modalActive || transitionActive` -> `== true` on both; KE =
`campaign->hasWon() != false` -> `== true`; GO = `goingToWaypoint() == true`
-> bare.

A full single sweep of 40 predicate spellings, a full pair sweep of the 19
layout-affecting spellings (171 pairs), and a 4-deep sweep around
MT+MO+EX found nothing above 81.6429 % that keeps 8080 B and PLT OK.

## Findings worth keeping (from this pass)

1. **The PLT constraint is only `resetStartSequenceOver` vs the first
   `removeObjectives`** in OnTouchEnd's emitted code.  `resetStartSequenceOver`
   is referenced by no other function; `removeObjectives` is also referenced by
   `MGame::OnUpdate` and `MGame::successCheck`, both later in link order.  So
   OnTouchEnd's own first-reference order sets `.rel.plt` indices 3324/3325,
   and any variant that emits the reset block after the first
   `removeObjectives` rotates them and makes `successCheck` non-byte-exact
   (byte_exact 4505 -> 4504 -> RATCHET FAIL).
2. **The faithful top-level won test is `if (getMission()->hasWon())`**
   (`cbnz r0, ...` in the original at 0x17c84), i.e. the bare spelling (TB/TN).
   It scores **81.5709 %** at 8080 B but always rotates `.rel.plt`
   (reset moves to 0x135e, after removeObjectives at 0x119c), so it cannot be
   landed.  Every TB/TN pair tried (39) is ROT.
3. **The faithful `isInstantActionMission()`-first dispatch**
   (`if (instant) ... else if (campaign) ... else ...`, per REA) costs size:
   8092 B / 76.3689 % (8084 / 76.4547 with TB).  A 25-op sweep on top of it
   found only MT (8080 / 72.99) and BC8 (8080 / 73.00) keeping 8080 B, both far
   below HEAD.
4. **The original `.text` is only 8 B longer than ours** (orig ends 0x17c0d4,
   ours 0x17c0cc); the first real instruction diff is at offset 0x486:
   original `cmp.w r9,#0 / beq` (mining `amount != 0`) vs ours
   `cmp.w r9,#-1 / bgt` (`count > 0`).  The faithful mining spelling
   (`amount != 0` / `count != 0` / `String(-amount)`) is 8092 B / 75.3514 % in
   every spelling tried: the 4-byte-shorter `b.n` at the end of the block moves
   the second inline constant-pool group from 0x6b4 to 0x6b0, which cascades
   into a different pool-group split (10 groups instead of 9) and a much worse
   layout.  `amount < 0` is code-identical to HEAD.
5. **Inline literal-pool groups** (original / ours, word counts):
   `0x3a4(12) 0x6b4(7) 0xac4(5) 0xe7c(6) 0x1278(9) 0x166c(17) 0x1a28(16)
   0x1e20(7) 0x1f60(12)` / `0x3a4(12) 0x6b0(7) 0xac0(5) 0xe84(5) 0x12b0(10)
   0x1660(16) 0x1a40(17) 0x1e20(7) 0x1f60(12)` — same 91 words, different
   split between adjacent groups.  The `ldr rX,[pc,#N]` immediates are *not*
   normalised by verify.py, so every pool-group shift costs several
   instructions on its own.
6. **The `switch (getCurrentCampaignMission())` case order is not
   source-order controllable**: writing the cases as an `if/else` chain
   (either order) produces byte-identical code, and swapping `case 0`/`case 1`
   in the source is a no-op.  The original emits `cmp r0,#1; beq case1;
   cmp r0,#0; bne default` (case 0 fall-through); ours emits `cmp r0,#0;
   beq case0; cmp r0,#1; bne default` (case 1 fall-through).

## Gate

`cmake --build cmake-build-match --target check -j6` on the final tree:
`verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`,
`data: exact 843 of 843`, `sodiff: extra-beyond-allow 0 missing 0
allowed 0 imports 207`, `RATCHET PASS`.
