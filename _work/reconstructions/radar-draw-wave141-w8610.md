# Radar::draw wave 141 (w8610) — status-reload sink + `stations` in `sl`: raw 8279 -> 8106 at 9908 B

**Status:** source change landed, 2026-11-07. Worktree `wt/w8610`, based on `master`
(`e18e0c2a4`). Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8279 B** (the wave-140
w8509 base: `int alwaysEnemy`, `int wreck`, the `Vector delta` `inSight` shape). After
this pass **8106 B** (-173). `.text` differing bytes **86,472 -> 86,299** (-173);
`.ARM.extab` **1,411 -> 1,395** (-16). All section sizes exact.

Read with `radar-draw-wave140-w8510.md`, `radar-draw-wave137-w8212.md`,
`radar-draw-wave136-w8110.md`.

## 1. Hypothesis and answer

Wave 140 (w8510) found that `expect(A,1) && B && expect(C,0)` makes the hot
`else { otherLock = 1; }` a 4 B `mov.w r9,#1` and sinks the status GOT reload out of
it, but the function was 9912 B; the campaign `stations->data()[i]` reload restored
9908 B on the w8409 base but was raw +443 on the w8509 base (reverted in `d365057e2`).

This wave re-ran the guard matrix **on the current w8509 base** and found a better
fixed point that keeps the 4 B hot else *and* puts `stations` in `sl`:

```cpp
int otherLock;
if (__builtin_expect(this->lockedAsteroid == nullptr, 0) && // lint: arm_builtin anchor: Radar::draw
    __builtin_expect(this->lockedGasCloud == nullptr, 1) && // lint: arm_builtin anchor: Radar::draw
    __builtin_expect(this->field_0x1c == nullptr, 1)) { // lint: arm_builtin anchor: Radar::draw
    otherLock = this->field_0x14 != 0;
} else {
    otherLock = 1;
}
```

and the campaign `isTarget` compares with the **operands swapped**:

```cpp
bool isTarget = campaignMission->getTargetStation() == stations->data()[i];
```

The swap is the companion the mission asked for. With the left operand
`stations->data()[i]`, clang spills `stations` to `[sp,#144]` and re-registers
`stationId`/`currentStation` badly (raw 8723 at 9900 B). With the right operand
`stations->data()[i]`, clang keeps `stations` in `sl` and loads the data word into a
scratch (`mov r1,sl; ldr.w r1,[sl,#4]; ldr.w r1,[r1,fp,lsl#2]; cmp r0,r1`), which is
the original's `sl`-based data load. That alone is raw 8123 at 9908 B.

The last 4 B of size came from the asteroid-loop `wreck` local: `int wreck` ->
`bool wreck` (wave 140 had made it `int` to pad the alwaysEnemy select). With the
`A0B1C1` guard the 4 B is no longer needed there, and the bool form is the original's
type. Final: 9908 B, raw 8106.

## 2. Landed guard (best variant)

```
12e310 mov   r1, r4
12e312 ldr.w r0, [r1, #12]!
12e316 str   r1, [sp, #124]
12e318 cmp   r0, #0
12e31a beq.w 1305aa          ; A==0 -> cold B/C checks
12e31e mov.w r9, #1          ; hot else: 4 B, falls through
12e322 ldr   r5, [sp, #156]  ; status GOT reload, sunk after the else
12e324 ldr.w r0, [r4, #312]  ; join: landmarks
12e328 cbz   r0, 12e362
12e32a ldr   r0, [r5, #0]    ; *status
```

`expect(A,0)` makes the A==0 path (the only path that can reach the then block)
cold; `expect(B,1)`/`expect(C,1)` keep the B/C checks hot in the cold block. The
original is the same placement with its `cbnz` chain and `movs r5,#1`; ours is
`cmp`+`beq.w` and `mov.w r9,#1` because the literal pool sits at `12e2f0` (the
hot block is 20 B shorter than the original's, so the pool is 0x50 earlier). That
pool shift is the remaining guard-region delta.

## 3. Variant matrix (all on the w8509 base)

### 3.1 Guard matrix, `int otherLock` (raw / ours size)

| guard | raw | usize |
|---|---:|---:|
| baseline `expect(A && expect(B,1) && C, 0)` | 8279 | 9908 |
| `expect(A && B && C, 0)` | 8336 | 9908 |
| `A && B && C` | 8575 | 9912 |
| `expect(A,0) && B && C` (**A0**) | **8137** | 9908 |
| `expect(A,0) && expect(B,1) && C` | 8145 | 9912 |
| `expect(A,0) && expect(B,1) && expect(C,0)` (**A0B1C0**) | **8131** | 9908 |
| `expect(A,0) && expect(B,1) && expect(C,1)` (**A0B1C1**) | **8097** | 9912 |
| `expect(A,1) && B && expect(C,0)` (w8510 g) | 8806 | 9912 |
| `expect(A,0) && expect(B,0) && expect(C,0)` | 8677 | 9928 |

`A0` and `A0B1C0` are the only 9908-B int shapes below the baseline; `A0B1C1`
is 4 B over but lower raw.

### 3.2 Companion matrix on `A0` / `A0B1C0`

| companion | raw | usize |
|---|---:|---:|
| none (A0) | 8137 | 9908 |
| campaign `stations->data()[i] == target` | 8723 | 9900 |
| campaign `target == stations->data()[i]` (swap) | **8123** | 9908 |
| campaign inline in the `if` | 8664 | 9904 |
| `stations->data()[i]` in the `stationId != currentStation` check | 9130 | 9952 |
| `int *stationData = stations->data()` | 8944 | 9856 |
| `Array<int> *const stations` | 8137 | 9908 |
| `planetHit` before `stations` | 8137 | 9908 |

The operand swap is the only planet-loop edit that both keeps 9908 B and lowers
raw. `A0B1C1` + swap = 8117; adding `bool wreck` = **8106** at 9908 B.

### 3.3 Type controls

| variant | raw | usize |
|---|---:|---:|
| `A0B1C1` + swap, `int wreck` | 8117 | 9912 |
| `A0B1C1` + swap, `bool wreck` | **8106** | **9908** |
| `A0B1C0` + swap, `bool wreck` | 8131 | 9904 |
| `A0B1C0` + swap, `bool alwaysEnemy` | 8403 | 9912 |
| `bool otherLock` + swap + bool wreck | 8805 | 9788 |

`bool otherLock` still reproduces the original guard but is 120 B short; the
int shapes are the only ones that reach 9908 B.

## 4. Root cause / open questions

The winning combination is:

1. `expect(A,0)`: the only path to the then block is A==0, so the then block and
   the B/C checks are cold; the hot else is a 4 B fall-through.
2. `expect(B,1)` + `expect(C,1)`: keeps the B/C checks in the cold block and the
   status GOT reload out of the hot else.
3. Campaign `getTargetStation() == stations->data()[i]` (operand swap): keeps
   `stations` in `sl` and loads the data word through `sl`, matching the original's
   `ldr.w r0,[sl,#4]; ldr.w sl,[r0,fp,lsl#2]` shape without forcing the bad
   `[sp,#144]` spill.
4. `bool wreck`: restores the original's type and the last 4 B of size.

Remaining deltas: the literal pool at `12e2f0` vs the original `12e340` (the hot
block is 20 B shorter), `mov.w r9,#1` vs `movs r5,#1`, `cmp`+`beq.w` vs `cbnz`,
`stations` in `sl` but not spilled to `[sp,#116]` (the original overwrites `sl`
with the data word and reloads), and the enemy `inSight`/tail blocks. The next
guard target is a shape that keeps the 4 B hot else *and* lengthens the hot block
by 20 B so the pool lands at `12e340` — i.e. the original's `cbnz` chain with the
int `otherLock` register pressure.

## 5. Gate / section deltas (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **86,299** differing bytes (size/base
exact 1,259,464), `.ARM.extab` **1,395** (size exact 156,312), `.rel.plt` **0**
(byte-identical, size exact 29,392), `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are
byte-identical. All 27 section sizes exact.

Official fuzzy on this base is 65.7 (baseline) vs 64.2 (this variant) in the same
process; raw same-address bytes are the primary metric here.
