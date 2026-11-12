# Radar::draw — wave 190 (w13476): landmark station-store order

Target: `_ZN5Radar4drawEP6PlayerP3Hudi` (`src/game/weapons/Radar.cpp`, 9,908 B).

## Rejected: the raw -1 swap is not semantically equivalent

Base: master `91ba24a04` (raw 6,064 / aligned 2,043 / size 9,908).

The candidate swapped the guarded timer reset and the unconditional
`field_0x28` store:

```cpp
// original / current master
if (this->field_0x28 != this->landmarks->data()[i]) {
    this->stationLockTimer = 0;
}
this->field_0x28 = this->landmarks->data()[i];
landmarkHit = true;

// candidate
if (this->field_0x28 != this->landmarks->data()[i]) {
    this->field_0x28 = this->landmarks->data()[i];
}
this->stationLockTimer = 0;
landmarkHit = true;
```

The candidate emits one byte fewer (raw **6,064 -> 6,063**), but it is not
semantically equivalent: it clears the timer even when the landmark did not
change, and only writes `field_0x28` when it did change.

The original disassembly at `0x12e8c0` proves the current master source is
the correct shape:

```
cmp   r1, r0
itt   ne
movne r1, #0
strne.w r1, [r4, #420]   ; stationLockTimer = 0 only on change
str   r0, [r4, #40]      ; field_0x28 = landmark unconditionally
```

The candidate emits `it ne; strne r0,[r4,#40]; movs r0,#0; str.w
r0,[r4,#420]`, i.e. the opposite conditionals. It is therefore a false
raw win and was not landed. The current master remains raw **6,064**.

## Measured negative cells this pass (all reverted)

- Adjacent-swap sweep, 385 source pairs: only the above win; everything else
  >= 6,065.
- Rotation sweep, same-indent statement pairs at distance 11–25 (538 pairs):
  no win (min 6,065).
- Multi-line adjacent statement swap (88 pairs): no win.
- `&&`/`||` operand reorders (planet-lock `campaign` guard; music
  `isAttackedByAliens||campaign16||inAlienOrbit`; `A||expect(B)||C` ->
  `A||C||B`): >= 6,065 (best neutral; last 6,088).
- `campaign` type/decl variants (`unsigned`, `int32_t`, `long`, `const`,
  `register`, `> 1`, `!=0&&>=2`, `(unsigned)`): neutral 6,065 or 6,066.
  Hoisting `campaign` above `turretMode`/`alienOrbit` is size-fatal.
- `currentStation` hoist undo: 6,066 (landed w13465 hoist still good).
- Gas-cloud block: original draw-after-box order 6,154; draw after
  `targetPos` only 8,219 (size 9,912).
- Enemy-lock blip call reorders: 6,066–6,069.
- Tractor guard variants: 6,067 / 7,633 / 6,472 / size-fatal.
- Extra `expect` hints (asteroid/gas lock bounds, timer > 500): 6,803 /
  9,245 / 7,828 / 7,019 / 8,043 / neutral.
- Outer music `expect(field_0x1bc>=1,1)` removal / invert-and-swap / goto:
  neutral or 6,069 (confirms w13346).
- Per-TU `-mllvm -regalloc-csr-first-time-cost=0/500/1000/2000/3000/4000`:
  exactly neutral (6,065).

## Still-open structural blockers (re-confirmed)

- `campaign` spilled to `sp+0x90` in ours vs `sl` in the original; shared
  zero in `sl` vs `fp`; planet-loop counter `i` in `sl` vs `fp`. Drives the
  `12e3d0`–`12ef50` shift cascade.
- Music tail: original has the `field_0x1bc < 1` else block inline and the
  `inAlienOrbit, isAttackedByAliens, campaign16` test order; ours is
  out-of-line and reordered. Reordering alone costs raw.
- Wreck x/y: ours CSE-hoists `screenX` (12 B shorter); forcing the original
  shape is size-fatal. w13346 suggests reconstructing wreck x/y and the
  asteroid-lock block jointly (+12 / -12).
- Tractor guard: original 26 B `ldrb; cbz; ldr; cbnz; eors; itttt eq`; ours
  32 B `eor/cmp/it/orrs`. No source spelling found yet.
