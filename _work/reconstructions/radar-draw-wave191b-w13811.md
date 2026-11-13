# Radar::draw wave 191b (w13811) — campaign planet rect: original screenX-first order

**Status:** complete, 2026-11-13. Worktree `wt/w13811`, branch `wave191b-w13811`,
base `master` `dcb04a00d`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13797.md` (previous pass) and the wave-191b
`docs/IDENTICALITY.md` update section.

**Landed:** the campaign planet-hit rect test now uses the original's
screenX-first operand order:

```cpp
if (this->screenX > left && this->screenX < left + size &&
    this->screenY > top && top + size > this->screenY) {
```

Size-neutral (9,908 B exact) and a win on aligned; raw and `.text` each grow by 1.

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 6,994 | 1,314 | 9,908 | 75,904 | 594 |
| 1 (w13811) | planet rect screenX-first | **6,995** | **1,303** | **9,908** | **75,905** | **594** |

`.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical. Full `check` gate green:
byte_exact 4508, linked_exact 4508, size-exact 4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, parity 5370/5370.

## 1. Evidence

Original `12e536..12e55a` is branchy, not predicated:

```
cmp r2, r3 ; ble ...      ; screenX <= left  -> skip
add r3, r1 ; cmp r2, r3
bge ...                   ; screenX >= left+size -> skip
...
```

i.e. the source tests `screenX` against `left`/`left+size` first, then the
`screenY`/`top` pair. The old source order (`screenY > top && top + size >
screenY && screenX < left + size && screenX > left`) made clang 7 `-Oz` emit an
`itt gt` predicated pair at `12e50a`. Reordering to screenX-first restores the
branch shape and drops the aligned delta from 1,314 to 1,303.

## 2. Rect-shape sweep (each row size-checked)

All rows start from the w13797 base (`6,994 / 1,314 / 9,908`). `—` marks a
size change (fatal under the gate).

| condition order | raw | aligned | size | `.text` |
|---|---:|---:|---:|---:|
| base: `SY>top && top+size>SY && SX<left+size && SX>left` | 6,994 | 1,314 | 9,908 | 75,904 |
| **C: `SX>left && SX<left+size && SY>top && top+size>SY`** | **6,995** | **1,303** | **9,908** | **75,905** |
| D: `SX<left+size && SX>left && SY>top && top+size>SY` | 6,995 | 1,303 | 9,908 | 75,905 |
| E: `SX>left && SX<left+size && top+size>SY && SY>top` | 6,995 | 1,306 | 9,908 | 75,905 |
| I: `SX>left && left+size>SX && SY>top && top+size>SY` | 6,997 | 1,307 | 9,908 | 75,907 |
| F: `SY>top && SY<top+size && SX>left && SX<left+size` | 8,375 | 1,310 | 9,904 | — |
| G: `SX>left && SY>top && SX<left+size && SY<top+size` | 8,542 | 1,484 | 9,920 | — |
| H: `left<SX && SX<left+size && top<SY && SY<top+size` | 8,375 | 1,303 | 9,904 | — |
| reorder to `SX>left && SX<left+size && SY>top && SY<top+size` | 8,375 | 1,303 | 9,904 | — |

C and D are codegen-identical; C is the more natural source order (matches the
original's `12e53c` screenX test first). H matches C's aligned count but is
size-fatal (9,904).

## 3. Campaign block root cause (not yet fixable)

The original keeps `campaign` in `sl` (`mov sl,r0` at `12e0f8`,
`cmp.w sl,#2` at `12e38a`). Our build spills it to `[sp,#144]` and reloads
(`str r0,[sp,#148]; ldr r0,[sp,#144]; cmp r0,#2`), which cascades: `stations`
lands in `[sp,#132]`/`0x84` instead of `sl`, and the loop index moves from
`fp` to `sl`. The frame is already `sub sp,#296` in both, so the older
"304 vs 296" note is stale for this base. Dropping or inverting the campaign
`__builtin_expect` hint is size-fatal (9,980 / 9,956), so the spill has to be
attacked through the `MatrixGetInverse` 64-byte temporary / local lifetime
(`add r5,sp,#240` vs `#232`), not through the hint.

## 4. EMP block — exhausted

Only register-allocation differences remain (`r5`/`r6` vs `fp`/`lr`), plus our
hoisted `ldr.w r6,[fp]` before `getEmpDamageRate`. Every source shape that
changes the `empCanvas` local's lifetime/declaration order is neutral or
size-fatal (see `radar-draw-wave191b-w13797.md` §3). Not re-run here.
