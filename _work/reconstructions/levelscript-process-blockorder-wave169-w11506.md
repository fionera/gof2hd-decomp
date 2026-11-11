# LevelScript::process — wave 169 / w11506: block-order census, mission-0
# dispatch and the shared wingman-loop body

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact. Worktree `wt/w11506`, branch `wave169-w11506`, base master
`00c403d5a` (wave 169). Scratch `/dev/shm/w11506/`.

Follow-up to `levelscript-process-blockorder-wave169-w11406.md` (outer
mission-dispatch census, no win).  This pass takes the next step: it decodes
the mission-0 state dispatch, measures the source reorderings the census
suggested, and identifies the one *shared-tail* difference in the function.

## Baseline (HEAD, tree left clean)

```
process raw diffbytes    53,970
process aligned insns    17,818
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20722
.rel.plt                 0        (BYTE-IDENTICAL)
```

## 1. Mission-0 dispatch: original is a 10-entry tbh, ours a 13-entry tbh

Original `0x139b16`:

```
139b16 ldr r1,[sl,#28]      ; m_nState
139b1a subs r0,r1,#6
139b1c cmp  r0,#9
139b1e bhi  13f906          ; state 16 / default
139b22 tbh  [pc,r0,lsl #1]  ; states 6..15
```

Ours `0x13904e` (HEAD): `subs r0,r1,#3; cmp r0,#12; tbh` — states **3..15**.
Clang merged the `else if (m_nState == 4) / == 5 / == 3` chain into the
switch.  Reordering the pre-switch chain to **4, 3, 5** (the original's
compare order: `cmp #4; beq`, `cmp #3; bne`, `cmp #5; bne`) restores the
10-entry `tbh` at `0x13b1b8` (verified) but is **+16 B** on this base, as
w11403 measured.

Decoded original table (base `tbh+4 = 0x139b26`, target = base + 2*entry):

```
state  6 -> 139b70   state  7 -> 13d022   state  8 -> 13d13c
state  9 -> 13d1de   state 10 -> 13cf76   state 11 -> 13d2d4
state 12 -> 13d3b8   state 13 -> 13d440   state 14 -> 13d4f4
state 15 -> 13d4c6
```

Physical order: **6, 10, 7, 8, 9, 11, 12, 13, 15, 14** (matches w11406).

## 2. Reordering the mission-0 `case` labels is a codegen no-op

Moving the ten `case 6..15` bodies into the original physical order
`6,10,7,8,9,11,12,13,15,14` produces **byte-identical code**:
raw 53,970 / aligned 17,818 / 57,232 B, unchanged.  `SwitchInst` stores
cases sorted by value, so MBP's successor order is value order, not source
order.  This confirms w11406's mission-78 result for mission 0 too.

## 3. `4,3,5` reorder + case-11 `enableFillScreen` removal is size-exact but raw-worse

The `4,3,5` reorder (+16) combined with removing the spurious mission-0
case-11 `Globals::layout->enableFillScreen(true)` (-16, w11403) is
size-exact:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **53,970** | **17,818** | **57,232** |
| `4,3,5` + case-11 fill-screen removed | 54,286 | 18,489 | 57,232 |

The 10-entry `tbh` is restored (at `0x13b1b8`) but the whole mission-0 body
region shifts by ~0x1696, so raw/aligned regress.  Not landed.

## 4. The shared wingman-loop body (the real shared-tail difference)

The shared "active" tail has two identical wingman loops:

```
if (Globals::status->getWingmen() != 0) {
    for (...) { if (isWingMan()) field_0x12d = 1; }
}
... passedTime block ...
if (Globals::status->getWingmen() != 0) {
    for (...) { if (isWingMan()) field_0x12d = 0; }
}
```

Original: the **second loop shares the first loop's body block** (tail
merge).  First loop setup `0x137fa8`, body `0x137fb0..0x137fc2`, condition
`0x137fc4`; second loop setup `0x13809a` (`r8=0`), condition `0x138124`
`bcc 137fb0` — i.e. it branches back into the *first* body.  Only the
conditions are duplicated (different fallthroughs).

Ours (HEAD): both bodies are emitted.  The first loop uses
`__builtin_expect(isWingMan(), 0)` and lowers to `cbnz <store>` with an
out-of-line store block; the second (plain `if`) lowers to the original's
inline `cbz <increment>; strb r8`.  Removing the first loop's expect (or
making it `expect(...,1)`) makes the first body instruction-for-instruction
identical to the original's inline body, but the two bodies are **still not
merged**:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **53,970** | **17,818** | **57,232** |
| first loop expect removed / expect(1) | 53,985 | 17,814 | 57,232 |
| second loop also expect(0) | 55,136 | 17,815 | 57,240 (+8) |

So the first body is now structurally faithful, but raw is +15 (aligned
-4); not landed.  A minimal clang-7 `-Oz` reproduction (two identical
`isWingMan()` loops separated by a counter update) also does **not** tail
merge: the register allocator picks different physical registers for the
two loop values (`r9`/`r6` vs `r8`/`r5`), so BranchFolder sees different
instructions.  The original's allocator reuses `r8`/`r5`/`r6`/`r4` for both
loops.  Forcing that register identity (a single value variable live across
the intervening block) is the remaining lever; if the bodies merge the
function loses ~26 B, so a +26 B companion is required.

## 5. Outer dispatch: the tail fallthrough

Original `0x137f7c` tail body ends with the second wingman-loop condition
at `0x138124..0x138128` and **falls through to mission 5** (`0x13812a`);
the `beq.w 13812a` at `0x137f82` skips the body.  Ours ends the tail with
`b.n 138786` (switch guard) because mission 5 is placed at `0x13824c` and
mission 81 at `0x138124`.  Physical body order:

```
orig  5, 81, 26, 78, 50, 89, 51      (switch guard 0x13879e)
ours  81, 5, 78, 26, 50, 51, 89      (switch guard 0x138786)
```

i.e. three pairwise swaps (5/81, 26/78, 51/89), not a wholesale reorder.
The census matcher (`/dev/shm/w11506/blocks.py`, unique-signature pairing,
78 unique pairs) reports 115 pairwise inversions; the largest are the
mission-0 case bodies (orig `0x13da88..0x13de00` 329 insns, `0x13e70c`
250 insns, `0x13b568` 232 insns) and the case-154 chain.  All are
MachineBlockPlacement outcomes; source case order does not move them.

## 6. Conclusion / open levers

* Mission-0 `case` order: no-op. `4,3,5` restores the original 10-entry
  `tbh` but needs a -16 B companion and is raw-worse at 54,286.
* The wingman-loop tail merge is the clearest *structural* difference:
  the original shares one body; ours emits two.  The blocker is the
  physical-register identity of the two loop values.
* The outer dispatch differs by three pairwise body swaps; the original's
  tail falls through to mission 5, ours branches to the switch guard.

No size-exact raw win was found; tree left clean at the entry baseline.

## Files changed

* `_work/reconstructions/levelscript-process-blockorder-wave169-w11506.md`
  — this note.
* No `src/`, `CMakeLists.txt`, baseline or allowlist change.
