# Radar::draw wave 145 (w9009) — asteroid `hasCargo`-first dead-pair duplication

**Status:** source change landed, 2026-11-08. Worktree `wt/w9009`, based on `master`
(`eb13ada9b`, wave 144). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8,471 B**.
After this pass **8,373 B** (-98). `.text` differing bytes **85,994 -> 85,896** (-98);
`.ARM.extab` **1,370** (unchanged); `.rel.plt` **0** (byte-identical). All 27 section
sizes exact; file size exact. Gate: `RATCHET PASS`, byte **4507**, linked **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave144-w8907.md`, `radar-draw-wave143-w8807.md`,
`radar-draw-wave141-w8610.md`.

## 1. Mission hypothesis and answer

The mission asked for a source shape that forces the original's **6** asteroid-loop
`isDead`/`isDying` calls (ours had 4) at the same 9908 B. **Yes — the shape is a single
short-circuit `if` whose two `dead` expressions are textually separate:**

```cpp
if (!(asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying())) &&
    (asteroid->isDead() || asteroid->isDying())) {
    continue;
}
```

`!(A && B) && C` evaluates `A=hasCargo` first; if `A` is false it short-circuits to
`C`, and if `A` is true it evaluates `B` and then (on `B==false`) falls through into
`C`. clang cannot CSE the two `isDead`/`isDying` call pairs because the callees are
external, so it emits all four calls. The generated pre-update block is now
**instruction-for-instruction identical** to the original:

```
ours (12fabc..12faea)                 original (12faa6..12fad4)
ldrb.w r0,[r6,#76]                    ldrb.w r0,[r6,#76]
cbz    r0, <second pair>              cbz    r0, <second pair>
mov    r0,r6 / blx isDead             mov    r0,r6 / blx isDead
cbnz   r0, <update>                   cbnz   r0, <update>
mov    r0,r6 / blx isDying            mov    r0,r6 / blx isDying
cbnz   r0, <update>                   cbnz   r0, <update>
mov    r0,r6 / blx isDead             mov    r0,r6 / blx isDead
cmp    r0,#0 / bne.w <continue>       cmp    r0,#0 / bne.w <continue>
mov    r0,r6 / blx isDying            mov    r0,r6 / blx isDying
cmp    r0,#0 / bne.w <continue>       cmp    r0,#0 / bne.w <continue>
```

The `if (!hasCargo && dead)` and `bool dead = ...; if (dead && !hasCargo)` forms both
emit only 2 calls and are disproven.

## 2. Size budget: +20 B from the duplication

The faithful duplication adds **+20 B** (9908 -> 9928). The size-neutral companion is
the wave-144-w8907 enemy-loop reshape, which was already witnessed but needed a +4 B
companion:

* drop the unused enemy `bool dead = enemy->isDead() || enemy->isDying();` (-16 B;
  the original enemy loop has 4 `isDead`/`isDying` calls after `update`, the w8906
  pad had 6);
* revert the enemy `int wreck` (wave-143 padding) to `bool wreck` (-4 B). The original
  stores `wreck` with a word `str r0,[sp,#152]`, but the original has **no**
  `strb [sp]` anywhere in `Radar::draw`, so a word store is consistent with a `bool`
  spill at `-Oz`; `bool` is the semantically faithful type.
* revert `int enemyHit` (w8906 padding) to `bool enemyHit` (-4 B). w8906's "witnessed
  word-typed" evidence is `12f058 ldr r1,[sp,#152]`, but that slot is **`wreck`**
  (`mvns r1,r1; tst r1,r0` = `countsAsEnemyExcludeFlag && !wreck`), not `enemyHit`;
  the `int` was a layout pad. `bool` is the faithful type.

## 3. Variant matrix (raw same-address diff, size)

| variant | usize | raw |
|---|---:|---:|
| baseline (wave-144 landed) | 9908 | 8471 |
| asteroid duplication only (enemy `dead` still present, `int wreck`) | 9928 | 8701 |
| + drop enemy `dead` | 9912 | 8676 |
| + `bool enemy wreck` | 9908 | 8444 |
| + `bool enemyHit` (**landed**) | **9908** | **8373** |
| + `bool slotCount` (METRIC-ONLY, semantically wrong) | 9908 | 8266 |
| asteroid dup + drop enemy `dead` + `bool asteroid wreck` (int enemy wreck) | 9908 | 8708 |

`bool slotCount` is the lowest raw but wrong: `slotCount` is a 0..4 counter
(`slotCount++`, `slotCount <= 3`), so `bool` changes behaviour. It is recorded only
as a metric bound.

## 4. Remaining deltas / open questions

1. The asteroid loop is now instruction-identical but sits **+22 B** later than the
   original (`0x12faaa` vs `0x12fa94`). The pre-loop gas-cloud/asteroids block is
   ~22-28 B longer than the original's (the original `goto asteroids` at `0x12f9be`
   is `b.n 0x12fa7c`; ours has an extra `mov r8,r0` and a different block order).
   Fixing that block would align the whole asteroid loop and drop raw far more than
   the type toggles above.
2. The asteroid literal pool still has 4 words vs the original's 5 (the original has
   an extra PC-relative `crosshairPos` load); that is ~4 B of the remaining loop
   length gap.
3. The `wreck`/`enemyHit` bool reverts are type-faithful but their raw win is largely
   a shift effect; the coordinator may prefer to keep `int enemyHit` (w8906) and take
   only `bool enemy wreck` (raw 8444).

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,896** differing bytes (size/base exact
1,259,464), `.ARM.extab` **1,370** (size exact 156,312), `.rel.plt` **0**
(byte-identical, 29,392 B), `.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`,
`.got`, `.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
All 27 section sizes exact.
