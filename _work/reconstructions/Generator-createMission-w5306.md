# `Generator::createMission` — wave 105 (w5306)

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(orig `0x971b8`, 1996 B, `.ARM.extab` LSDA record 168 B in the object).

## Result

| variant | fuzzy | body B | `.ARM.extab` (obj) | gate |
|---|---|---|---|---|
| HEAD `835748c18` (pad at top of body, plain `if (pick == 12)`) | 77.5 | 1996 | 0xa8 | green |
| pad moved to just before the reward computation (p15) | 80.8 | 1996 | 0xa8 | green |
| **landed**: p15 pad + `__builtin_expect(pick == 12, 0)` + `int itemId = 0;` at first use | **81.5** | 1996 | **0xa8** | **green** |
| p16 pad (before `if (type == 7)`) | 82.4 | 1996 | 0xb0 | **breaks** (`.rodata` shifts, byte_exact 4500→4040) |
| `int level = status->getLevel();` at first use | 82.6 | 1996 | 0xb0 | breaks |
| `if (pick == 12)` plain (no `__builtin_expect`) | 78.7 | 1996 | 0xa8 | green |
| `volatile int amount` → `int amount` | 70.5 | 1996 | 0xa8 | green |
| `__builtin_expect(pick == 10, 1)` removed | ~52 | 1988 | 0xa8 | green |

Full gate after landing: byte 4500, linked 4500, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `.ARM.extab` 0x26298, **RATCHET PASS**.

Landed source deltas vs HEAD (all inside `createMission`):

* the LSDA-scope pad `volatile double padA = 0; (void) padA;` moves from the top of
  the body to just before `float reward = (galaxy->distance(...` (after
  `fromSystemPtr`);
* `if (pick == 12)` → `if (__builtin_expect(pick == 12, 0))` (same-line lint waiver);
* `int itemId;` at the top of the body is removed and the first assignment becomes
  `int itemId = 0;`.

## Why the pad position matters (measured, not guessed)

The pad is a stack-slot / block-placement anchor, not logic. Its position controls
the LSDA call-site table:

| pad position | call-site entries | table bytes | object record | linked `.ARM.extab` |
|---|---|---|---|---|
| p15 (`float reward = ...`) | 25 | 147 | 0xa8 (168) | 0x26298 (matches orig) |
| p16 (`if (type == 7)`) | 26 | 152 | 0xb0 (176) | 0x262a0 (+8, gate fails) |
| original | 26 | 151 | 176 | 0x26298 |

The original's LSDA (parsed at `0x1a78c4`: LPStart `0xff`, TType `0xff`,
call-site encoding `0x01`, ULEB table length 151) has 26 entries.  The p15 record
has 25 entries and is 8 B smaller; the total section still lands on the original's
`0x26298`.  Any 26-entry record is ≥172 B and rounds to 176 B, so p16 can never be
gate-compatible.  A sweep of the pad at every top-level statement of the body found
80.8 % as the best 0xa8 result before the `itemId` change (p12/p13/p14/p15 all tie).

## Why `__builtin_expect(pick == 12, 0)`

The original emits

```
cmp  r6,#12
bne  <shared type=pick>      ; pick != 12 falls through to selected=true
...  Status::getSystem/getIndex, movs r2,#12, cmp r0,#25, mov fp,#1, it ne, movne r2,r6
b    <join>
```

i.e. the `pick == 12` body is the fall-through and the `pick != 12` path is the
branch.  With a plain `if (pick == 12)` clang -Oz emits the opposite polarity
(`beq` into the body).  Marking the type-12 arm cold flips the layout and is worth
+2.8 points (78.7 → 81.5) at zero byte cost.  `(pick == 12, 1)` instead shrinks the
body to 1988 B and scores 69.3.

`__builtin_expect(pick == 10, 1)` (already at HEAD) is still required: removing it
drops the body to 1988 B and the match to ~52 %.

## Why `int itemId = 0;` at first use

Hoisting the declaration to the first assignment (instead of `int itemId;` at the
top of the body) is worth +0.7 (80.8 → 81.5) at the same 1996 B / 0xa8.  It is
semantically identical — `itemId` is not read before that point and is still live
through `setProductionGoods(itemId, amount)` at the end.

## Remaining diff (not fixed)

* frame 112 vs original 96; `targetStation` at `[sp,24]` and `agent` at `[sp,28]`
  (original is the other way round), so the two `ldrd` sites read `r4,r9` instead
  of `r9,r4`; the `amount` volatile slot is `[sp,104]` instead of sharing the
  original's `[sp,28]` with `targetStation`;
* `getRoutes() == nullptr`: original is out of line (`cmp r0,#0; beq.w 0x3ce`),
  ours is inline (`cbz 0x240`); `__builtin_expect` on this condition makes it worse
  (61.6 %);
* `pick == 12` block polarity is still inverted relative to the original even with
  the cold hint (ours `beq` into the body, original `bne` over it);
* literal-pool placement and `tbb` case-block offsets (case order in the source is
  canonicalised away by clang; reordering the `switch` cases does not move the score).

## Commands

```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target verify -j6   # syncs report.json
```
