# Radar::draw wave 140 (w8510) — status-reload sink + `stations` spill: 4 B hot else at 9908 B

**Status:** source change landed, 2026-11-07. Worktree `wt/w8510`, based on `master`
(`d4857f174`), with the wave-139 w8409 `Vector delta` `inSight` change restored (the
`fb6fba7dd` LevelScript commit had accidentally reverted it; the main worktree still
carries it uncommitted). Target `src/game/weapons/Radar.cpp`.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B; ours 9908 B. Baseline raw same-address diff **8346 B** (the wave-139
w8409 delta base); after this pass **8318 B** (-28). Official fuzzy is 75.4-ish and
nondeterministic; raw same-address bytes are the metric here.

Gate before and after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, symbol parity
5370/5370. Section deltas: `.text` **85,273 -> 85,245**; `.ARM.extab`
**1,404 -> 1,392**; `.rel.plt` **0** (byte-identical); `.note.gnu.build-id` 20;
`.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical. All 27 section sizes exact.

Read with `radar-draw-wave139-w8410.md`, `radar-draw-wave138-w8310.md`,
`radar-draw-wave137-w8212.md`, `radar-draw-wave136-w8110.md`.

## 1. The new hypothesis and the answer

Wave 139 (w8410) found `la_exp1` — a first-term `expect(...,1)` — makes the hot
`else { otherLock = 1; }` block exactly 6 B (`mov.w r9,#1` + `b.n`) with
`otherLock` in `r9` and the status GOT reload sunk out of it, but the function was
9900 B (8 B short). This wave tested whether the missing bytes are the `stations`
r6/spill difference, and found a **better shape than the 6 B target**:

```cpp
int otherLock;
if (__builtin_expect(this->lockedAsteroid == nullptr, 1) &&
    this->lockedGasCloud == nullptr &&
    __builtin_expect(this->field_0x1c == nullptr, 0)) {
    otherLock = this->field_0x14 != 0;
} else {
    otherLock = 1;
}
```

The hot else block is now **4 B** — `mov.w r9,#1` with a fall-through to the join
(no `b.n`), even closer to the original's 2 B `movs r5,#1` — and the function is
exactly **9908 B**. The status GOT reload sits in the check-B predecessor
(`ldr r5,[sp,#156]` at `0x12e320`) and in the cold then block, not in the hot else.

The second half is a real source-level match for the original's `stations` spill:
the campaign `isTarget` comparison must reload `stations->data()[i]`, not reuse the
`stationId` local:

```cpp
bool isTarget = stations->data()[i] == campaignMission->getTargetStation();
```

The original does exactly this (`ldr.w r0,[sl,#4]; ldr.w sl,[r0,fp,lsl#2]` at
`0x12e474..0x12e478`, then `ldr.w sl,[sp,#116]`), which is why `stations` is
spilled. With the reload in the source, clang spills `stations` here too
(`str r0,[sp,#144]` / `ldr r0,[sp,#144]`), closing the register-pressure gap.

## 2. Landed guard shape

```
12e318 cmp   r0, #0
12e31a bne.w 1305b6        ; A != 0 -> cold else (otherLock = 1)
12e31e ldr   r0, [r4, #56] ; B
12e320 ldr   r5, [sp, #156]; status GOT reload (check-B predecessor)
12e322 cmp   r0, #0
12e324 itt   eq
12e326 ldreq r0, [r4, #28]; C
12e328 cmpeq r0, #0
12e32a beq.w 1305f0        ; B && C -> cold then (otherLock = field_0x14 != 0)
12e32e mov.w r9, #1        ; hot else: 4 B, r9, falls through
12e332 ldr.w r0, [r4, #312] ; join: landmarks
12e336 cbz   r0, 12e370
12e338 ldr   r0, [r5, #0]  ; *status
```

The `expect(A,1)` flips the guard weights so the then path is cold; the
`expect(C,0)` (not `expect(C,1)`) keeps the else hot and lets it fall through.
The original's `cbnz` chain is a different block layout (its then is cold at
`0x12ebc2` and its else is `movs r5,#1`); ours is the same placement with `r9`.

## 3. Variant matrix (all on the w8409 delta base)

| variant | guard | usize | raw diff | hot else |
|---|---|---:|---:|---|
| **baseline** | outer `expect(...,0)`, inner gas `expect(...,1)` | **9908** | **8346** | 10 B |
| `la_exp1` (w8410) | `expect(A,1) && expect(B,1) && C` | 9900 | 9027 | 6 B |
| `g2` | `expect(A,1) && expect(B,1) && expect(C,0)` | 9912 | 8704 | 10 B |
| `g3` | `expect(A,1) && B && expect(C,0)` | 9912 | 8702 | 10 B |
| **`g3 + campaign reload`** | `g3` + `stations->data()[i]` isTarget | **9908** | **8318** | **4 B** |
| `g2 + campaign reload` | `g2` + campaign reload | 9908 | 8323 | 4 B |
| `g3 + campaign reload + ph_int` | + `int planetHit` | 9928 | 8808 | 4 B |
| `g3 + campaign reload + freelance` | + freelance `stations->data()[i]` check | 9928 | 8756 | 4 B |

The campaign `stations->data()[i]` reload is the only planet-loop edit that both
lands on 9908 B and keeps the 4 B hot else. Every other station/planet spelling
(`campdraw`, `ph_int`, `warp`, `freelance`, `i_int`, `camp+scope_outer`, `pt_local`)
moves the size off 9908 or re-rolls the guard.

## 4. Root cause / open questions

The wave-139 conclusion (the reload block and `r9` occupancy are one
`SpillPlacement`/Greedy-RA fixed point) still holds, but the fixed point has more
than one solution. The winning combination is:

1. `expect(A,1)` + no outer expect: flips the then/else weights so the hot else
   can fall through (4 B) while `otherLock` stays in `r9`.
2. `expect(C,0)`: makes the then block cold, so the else is the fall-through and
   the status GOT reload is hoisted only into the check-B predecessor and the cold
   then.
3. `stations->data()[i]` in the campaign `isTarget`: the original's source, forces
   the `stations` spill and supplies the missing register pressure/bytes.

Remaining deltas in the guard: the original uses `cbnz` (2 B) where ours uses
`cmp`+`bne.w` (6 B), and `movs r5,#1` (2 B) where ours uses `mov.w r9,#1` (4 B).
The `stations` spill slot is `[sp,#144]` here vs the original's `[sp,#116]`
(StackColoring), and `stationId` is `r9` vs the original's `r8` (the original's
`otherLock` is `r5`, so `r9` is free there). These are the next guard/planet
targets.

## 5. Gate / section deltas (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,245** differing bytes (size/base
exact), `.ARM.extab` **1,392** (size exact 156,312), `.rel.plt` **0**
(byte-identical), `.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`, `.got`,
`.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
