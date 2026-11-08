# Radar::draw wave 147 (w9309) — extra `play` and missing `strd` are one register-allocation cause; branch weights are the lever

**Status:** source change landed, 2026-11-08. Worktree `wt/w9309`, based on `master`
(`9c9744ca5`, wave 146). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8,733 B** (wave-146 landed
state), instruction-index alignment **2,712**. After this pass raw **8,731** (-2),
alignment **2,744** (+32), size still 9908. `.text` differing bytes **85,928 -> 85,926**
(-2); `.ARM.extab` **1,393** (unchanged, size exact 156,312); `.rel.plt` **4** (unchanged);
`.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical. All 27 section sizes exact; file size
exact. Gate: `RATCHET PASS`, byte **4507**, linked **4507**, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

Read with `radar-draw-wave146-w9206.md`, `radar-draw-wave145-w9009.md`,
`radar-draw-wave143-w8808.md`.

## 1. The two mission questions, decoded

### (a) The 8th `FModSound::play` is the 146/147 tail

Original has **7** `blx play` sites; ours has **8**. Mapping (same order, ours shifted):

| # | original | ours (base) | source |
|---|---|---|---|
| 1 | `12f88e` | `12f848` | landmark station-lock `play(26)` |
| 2 | `12ff82` | `12ff78` | planet-lock `play(26)` |
| 3 | `1300a2` | `13008c` | gas-lock `play(26)` |
| 4 | `1302d6` | `13035e` | **music tail 1** (136/140/141/142/145/149/151/143) |
| 5 | `1303ea` | `13019e` | planet-lock `play(26)` (music-adjacent block) |
| 6 | `13046c` | `130460` | asteroid-lock `play(26)` |
| 7 | `13069c` | `1305ee` | **music tail 3 — EXTRA** (146/147) |
| 8 | — | `13069c` | **music tail 2** (2241/2242/148/152/race) |

The original's 146/147 branches (`1305d8`, `130608`) both `b.n 130698` — i.e. they share
the *same* tail as the supernova/deep-science/148/race branches. Ours gives 146/147 their
own tail at `1305ea`:

```
ours 1305ea:  movs r2,#0 ; movs r3,#0 ; blx play ; ldr r5,[sp,#124] ; b.n 13039a
ours 130698:  movs r2,#0 ; movs r3,#0 ; blx play ; ldr r5,[sp,#124] ; ldr r4,[sp,#152] ; b.n 13039a
orig 130698:  movs r2,#0 ; movs r3,#0 ; blx play ; ldr r5,[sp,#120] ; b.n 130326
```

The tails differ **only** by the redundant `ldr r4,[sp,#152]` (`this` reload). The tail
merger therefore cannot merge them, so the 146/147 branch keeps its own `blx play`. This is
not a source-shape difference in the music block: it is a symptom of `this` (r4) being
spilled in ours and never spilled in the original.

### (b) The `str r6,[sp,#0]; movs r6,#68; str r6,[sp,#4]` tails

The `DrawImage2D(image, x, y, 17, 68)` overload is `Ejiihh` = `(unsigned int, int, int,
unsigned char, unsigned char)`; with the implicit `this` the two flags are the 5th/6th
arguments and land at `[sp,#0]`/`[sp,#4]`. The original keeps the constant **68 in `sl`**
for the whole enemy loop:

```
orig 12ecd0:  mov.w sl,#68          ; enemy-loop preheader
orig 12ef44:  strd  r6,sl,[sp]      ; 17 in r6, 68 in sl
orig 12f154:  strd  r5,sl,[sp]
orig 12f39c:  strd  r5,sl,[sp]
orig 12f12e:  str  r6,[sp,#0] ; str.w sl,[sp,#4]   ; r6/r5 not pairable here
```

Ours keeps **`wreck` in `sl`** instead (`12ed1c/12ed22/12ed2e` write the wreck value to
`sl`), so at every site it rematerialises 68 into r6:

```
ours 12eef0:  str r6,[sp,#0] ; movs r6,#68 ; str r6,[sp,#4]
```

The original spills `wreck` to `[sp,#152]` (`12ed82 str r0,[sp,#152]`,
`12edb8 ldr r1,[sp,#152]`), freeing `sl` for the constant. Ours spills `this` to
`[sp,#152]` instead and reloads it six times (`12e642, 12e6a6, 12e6ae, 12f6de, 12f7f8,
130690`), which is also why the music tail needs the r4 reload. **Both mission symptoms are
the same register-allocation divergence: ours spills `this`, the original spills `wreck`.**

## 2. The lever: branch weights on the enemy-loop guard

The wave-146 `!prof` finding suggested branch weights. Applying `__builtin_expect` to the
enemy-loop guard is decisive:

| variant | usize | raw | align | plays | `movs #68` |
|---|---:|---:|---:|---:|---:|
| baseline (wave-146 landed) | 9908 | 8733 | 2712 | 8 | 5 |
| `expect(scannerAvailable && enemies != nullptr, 1)` (E4) | 9768 | 9362 | **2325** | **7** | 3 |
| `expect(scannerAvailable, 1) && enemies != nullptr` (E1) | 9744 | 9188 | 2457 | 8 | 1 |
| **`scannerAvailable && expect(enemies != nullptr, 1)` (E2)** | 9800 | 9192 | **1963** | 8 | 1 |
| E2 + `int asteroidHit` | 9788 | 9205 | **1888** | 8 | 1 |
| `expect(scannerAvailable && enemies != nullptr, 0)` | 10152 | 9331 | 3165 | 7 | 1 |
| `expect(gasClouds != nullptr, 1)` (G1) | 9896 | 8747 | 2711 | 8 | 5 |
| `expect(inSupernovaSystem(), 0)` (**landed**) | **9908** | **8731** | 2744 | 8 | 5 |

E2/E4 **fix both symptoms**: with the branch weight the allocator stops spilling `this`,
keeps 68 in `r9` (`strd r6,r9` / `strd r5,r9`) and the 146/147 tail merges (E4 plays=7).
The alignment collapses from 2712 to **1963** (E2) / **1888** (E2+`int asteroidHit`) — by
far the largest structural gain seen on this function. The catch is that the function also
shrinks (9800 / 9788 B): the better allocation removes ~108-120 B of spill/reload code. The
original is 9908 B, so the original must contain ~108 B of code we are still missing (the
wave-146 landmark `DrawString` un-merge is only ~20-24 B of it). **This is the next lever:
find the missing ~108 B of faithful body, then the `expect(enemies != nullptr, 1)` shape
should land at 9908 with alignment ~1900 and both symptoms fixed.**

## 3. Landed change and its trade-off

The only variant found that keeps 9908 B **and** changes a number is
`__builtin_expect(Globals::status->inSupernovaSystem(), 0)` on the music branch. It is a
plausible branch-weight hint (the original was built with branch weights, and the supernova
path is cold), and it drops the raw same-address diff by 2 B. It does **not** fix the extra
play or the `strd` sites, and the alignment metric rises 2712 -> 2744, so it is a
**METRIC-ONLY / marginal** change; the coordinator may prefer the wave-146 baseline or wait
for the E2 padding work.

`bool enemyHit` (the witnessed type) is **9884 B** on this base, i.e. it is the wave-146
`+24 B` companion question again; it improves alignment 2712 -> 2682 but changes the section
size, so it was not landed.

## 4. Variant matrix (raw / align / usize)

| variant | usize | raw | align |
|---|---:|---:|---:|
| baseline | 9908 | 8733 | 2712 |
| `bool enemyHit` | 9884 | 8797 | 2682 |
| `bool enemyHit` + `int bossSeen` | 9848 | 8777 | 2683 |
| `bool enemyHit` + `int asteroidHit` | 9896 | 8803 | 2808 |
| `race` moved into final `else` (getRace after getStation, **not faithful**) | 9900 | 8754 | 2726 |
| `race` moved + `int asteroidHit` | 9892 | 8772 | 2631 |
| `musicId` single-`play` variable | 9852 | 8740 | 2744 |
| `expect(inSupernovaSystem(), 0)` (**landed**) | 9908 | 8731 | 2744 |
| `expect(inSupernovaSystem(), 0)` + `bool alwaysEnemySeen` | 9908 | 8690 | 2760 |
| `expect(inDeepScienceOrbit(), 0)` | 9904 | 8742 | 2734 |
| `expect(route != nullptr, 1)` | 9908 | 8733 | 2712 (no-op) |
| `expect(planetTargets != nullptr, 1)` | 9908 | 8733 | 2712 (no-op) |

`race` moved into the final `else` gives **plays=7** (the 146/147 branch no longer clobbers
r5) but contradicts the original's witnessed order (`1305b8 getRace` before `1305c2
getStation`), so it is METRIC-ONLY.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,926** differing bytes (size/base exact
1,259,464), `.ARM.extab` **1,393** (size exact 156,312), `.rel.plt` **4** (29,392 B),
`.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`, `.rel.dyn`,
`.dynsym`, `.dynstr`, `.hash` are byte-identical. All 27 section sizes exact.
