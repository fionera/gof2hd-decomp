# Radar::draw wave 190 (w13435) — drop the `field_0x1a9` and `cargo == nullptr` hints
# raw 6,289 -> 6,272 (-17), size-exact, `.rel.plt` identical

**Status:** landed, 2026-11-12. Worktree `wt/w13435`, branch `wave190-w13435`, base
`master` `be282d6e9`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13422.md` (camZ CSE), `-w13412.md` (planet-lock y form),
`-w13404.md` (spark `int top`) and `radar-draw-wave189-w13364.md` (camZ hoist).

## 0. The change

Two reconstruction-only `__builtin_expect` hints in the enemy-lock block have no
counterpart in the binary. Removing them **as a pair** (each alone is neutral or worse)
changes the cold-block layout of the music tail and wins 17 raw bytes:

```cpp
-                        if (__builtin_expect(this->field_0x1a9, 1)) {  // lint: arm_builtin
+                        if (this->field_0x1a9) {
...
-                                if (__builtin_expect(cargo == nullptr, 0)) {  // lint: arm_builtin
+                                if (cargo == nullptr) {
```

Semantics unchanged (pure branch-weight hint removal).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,289 | **6,272 (-17)** |
| instruction-index alignment (lower better) | 1,912 | 1,941 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,543 / 3,532 | 3,543 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,884 | **72,867 (-17)** |
| `.ARM.extab` differing bytes | 601 | 601 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 73,505 | **73,488 (-17)** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. Where the win is

Per-0x100 raw diff (base -> after):

| region | base | after | delta |
|---|---:|---:|---:|
| `130150` | 2 | 3 | +1 |
| `130450` | 245 | 243 | -2 |
| `130550` | 184 | 168 | -16 |
| everything else | — | — | 0 |

So the gain is entirely in the music tail (`1304xx`-`1306xx`): the two hint removals
re-pin the tail's cold-block placement. The rest of the function is byte-for-byte
unchanged. `.ARM.extab` does not grow.

## 2. How it was found

A pair search over 16 candidate hint removals / spellings (136 combinations, full
incremental `link_so -j2` + `tools/fn_diff.py`). Only `rmcargo + rm1a9` beat the base:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,289 | 1,912 | 9,908 |
| **`rmcargo + rm1a9` (landed)** | **6,272** | 1,941 | **9,908** |
| `rmcargo` alone | worse | — | — |
| `rm1a9` alone | worse | — | — |
| `rmcargo + rm1a9 + comma` | 6,272 | 1,941 | 9,908 |
| `rmcargo + rm1a9 + rmf18` | 6,272 | 1,941 | 9,908 |
| `rmcargo + rm1a9 + rmdock` | 6,275 | 1,941 | 9,908 |
| `rmcargo + rm1a9 + camz-in-body` | 6,279 | 1,924 | 9,908 |
| `rmcargo + rm1a9 + rmlock` | 6,421 | 1,918 | 9,904 |
| `rmcargo + rm1a9 + rm1bc` | 6,460 | 1,818 | 9,916 |

No third edit improves on the pair. The pair is a genuine content win (the raw drop is
in the tail block placement, not a whole-function shift: the other 0x100 regions are
unchanged).

## 3. Negative results measured this pass (all reverted)

Full incremental `link_so -j2` + `tools/fn_diff.py`; scratch
`/home/debian/GalaxyOnFire2/scratch/w13435/`.

* `campaign` register-rotation attempts: `campaign` as `const`, `unsigned`, cast,
  `+0`, `2 <= campaign`, early `bool campaignOk`, `!mission`/`mission == nullptr` as the
  `missionBlocksPlanets` initialiser (this last one *does* put campaign in `sl` but
  costs +8 B / +16 B and raw 7,795-8,255), `0L`/`'\0'`/`(short)0`/`0x0` initialisers,
  `zeroFlag`/`falseFlag` locals — all neutral or worse.
* `plasmaInRange`/`lockConfirmed` store orderings (`SetColor` between / before,
  chained assignment, comma, `unsigned char` casts) — neutral or worse.
* Planet-block multi-operand permutations (`!otherLock`/`!planetHit`/`!turretMode`,
  `stationId != currentStation && isTarget`, warp-gate order) — neutral or worse.
* Asteroid-block permutations (`hasScanner` invert, `hasTractor13` invert,
  `field_0x10` invert, `frame` reverse, `asteroidLockTimer > 500` expect, swap the two
  `asteroidLockTimer` blocks, `slotCount` unsigned, `wreck` bool, `inCenter` bool) —
  neutral or worse.
* `camZ` inside the gas for-body: 6,296 / 1,895 (aligned win, raw loss); combining it
  with the spark `int top` forms is 6,312.
* Wreck `screenX` volatile reload (reproduces the original 82-byte x/y block) is
  size-fatal (+12 B, 9,920) and raw 6,928; compensating hint removals land at
  6,513 / 1,844 / 9,908, still raw-worse.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6272 aligned= 1941 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 5. Second pass (same worker): exhaustive `__builtin_expect` sweeps — no further win

All cells below are full incremental `link_so -j2` + `tools/fn_diff.py` on the **landed
6,272 base** (`Radar.base.cpp` == the committed `Radar.cpp`). 30 `__builtin_expect`
wrappers remain in the function.

### 5.1 All-pairs removal sweep (465 cells)

Every single removal (30) and every pair of removals (30 choose 2 = 435). **No cell beats
6,272.** The best ties are all exactly `raw 6,272 / aligned 1,941 / size 9,908`:

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,272 | 1,941 | 9,908 |
| rm #4 (`screenX > centerX - field_0x128`, line 433) | 6,272 | 1,941 | 9,908 |
| rm #10 (`field_0x18 != 0`, line 650) | 6,272 | 1,941 | 9,908 |
| rm #12 (inner `isDockingToPlanet`, line 655) | 6,272 | 1,941 | 9,908 |
| rm #15 (enemy `enemyFlagsLo`, line 787) | 6,272 | 1,941 | 9,908 |
| any pair of {4,10,12,15} | 6,272 | 1,941 | 9,908 |
| rm #11+#12 (whole `isDockingToPlanet` wrapper, line 655) | 6,272 | 1,941 | 9,908 |

`#11+#12` was the one cell the sweep reported as `BAD` (a transient gold-link failure
under fleet load); it was re-run by hand and is neutral, as are the two outer/inner
spellings on their own. So the landed `rmcargo + rm1a9` pair is still the only removal
combination that moves the number.

### 5.2 Weight-flip sweep (30 cells) and `flip #17` combinations

Flipping each hint's weight `1<->0` singly: no raw win. The interesting cell is

| cell | raw | aligned | size |
|---|---:|---:|---:|
| flip #17 (`carriesFriendCargoFlag`, line 826, `0 -> 1`) | 6,272 | **1,875** | 9,908 |

i.e. a free 66-slot alignment improvement at identical raw. Combining `flip #17` with
every other flip (29 cells) and every other removal (29 cells) yields **no raw win**:
the best is `6,272 / 1,875` for `f17+f12`, `f17+f15`, `f17+r4`, `f17+r10`, `f17+r12`,
`f17+r15`; everything else is 6,273-6,415. The aligned win is real but raw is the gate
metric, so it is not landed.

### 5.3 Removal triples (286 cells)

All triples from {4,10,12,15,17,22,23,24,25,26,27,28,29} (the neutral/sensitive cells
plus the whole music tail): no cell beats 6,272. Best ties `6,272 / 1,941`.

### 5.4 Music-tail semantic-preserving rewrites (13 cells, all byte-identical)

Every one of these compiles to the **same bytes** as the base, i.e. the tail is
insensitive to source spelling and only reacts to branch weights:

`if (current == 143) return;` one-liner; `(unsigned)(current-136) > 15u`;
`(0xe071 & (1 << (current-136))) == 0`; `field_0x54 = true`; `16 ==`/`1 ==`/`145 !=`/
`105 <`/`143 ==`/`108 ==`/`101 ==` operand swaps; `bool ambient` -> `int ambient`;
`if (ambient == 0)`.

### 5.5 Other cells measured this pass (all reverted)

| cell | raw | aligned | size |
|---|---:|---:|---:|
| `const int campaign` | 6,272 | 1,941 | 9,908 |
| wreck `int x; int y;` -> `int y; int x;` | 6,272 | 1,941 | 9,908 |
| wreck `11e&&11f` branch: swap the two stores | 6,923 | 1,947 | **9,920** |
| wreck `11f` branch: swap the two stores | 6,963 | 1,960 | **9,920** |
| both wreck branches swapped | 6,974 | 1,959 | **9,916** |
| `int missionBlocksPlanets = mission == nullptr` | 7,859 | 1,994 | **9,916** (insn 3,532/3,532 exact) |
| drop the `auto hasScanner = this->hasScanner19` local | 6,392 | 1,924 | 9,908 |
| `plasmaInRange = (uint8_t)(player == nullptr)` | 9,474 | 2,014 | **9,888** |
| `lockConfirmed = (uint8_t)(player == nullptr)` | 9,475 | 2,014 | **9,888** |
| both of the above | 9,119 | 2,005 | **9,920** |

The `mission == nullptr` initialiser is notable: it is the only variant measured that
reaches the original's **exact instruction count 3,532/3,532**, but it pays +8 B of
32-bit branch encodings and raw 7,859.

## 6. Literal-pool placement: the original has a mid-function pool island

The task brief's "original end pool at `0xd2854` with `0.325f, 0.65f, 100.0f, 50.0f`"
is Layout's pool, not `Radar::draw`'s. The real pool difference is this:

* The original keeps a **5-word constant-pool island at `0x1302dc`** (`0x1302dc`-
  `0x1302ec`), referenced by `ldr r0,[pc]` at `12ff5c`, `130048`, `130090`, `13022e`
  and `130264`. It sits immediately after the unconditional `b.n 130326` at `1302da`
  (the `play(136)` arm branches to the epilogue instead of falling through).
* Ours has no mid-function pool: those same five PC-relative `Globals::` offsets are
  emitted in the **end-of-function pool at `0x1306f4`**, because our `play(136)` block
  falls straight through into the epilogue (`12e2e2`-equivalent) — no branch, so no
  pool island.

The pool *contents* are PC-relative offsets (`target - (pool_pc+4)`), so moving the
pool changes the words; both pools are 4/5 words of `Globals::` slot offsets and both
resolve to the same globals. Reproducing the mid-function island requires reproducing
the original's block order (epilogue at `0x130326`, after the ambient switch), which is
gated by the outer `field_0x1bc >= 1` hint (see `radar-draw-wave189-w13346.md`). That
hint removal is size-fatal (+8 B) on its own.

## 7. Why the `campaign` rotation resists

`campaign` (`12e0f8 mov sl,r0`, single use `12e38a cmp.w sl,#2`) loses `sl` to the
shared zero. The zero vreg is live `12e07a` (two `strb` of `lockConfirmed`/
`plasmaInRange`) through `12e0d8` (remat for `missionBlocksPlanets`) to `12e3ba`
(`str.w fp,[sp,#112]`). The original has the *same* two `mov.w fp,#0` remats and the
*same* `12e3ba` store, so the original's zero vreg must be shorter than `campaign`'s —
i.e. the original does **not** CSE the two `strb` zero with the `missionBlocksPlanets`
zero. Every source spelling of the constant (`0`, `0L`, `'\0'`, `(short)0`, `0x0`,
`false?1:0`) is the same IR `i32/i8 0` and re-merges. The only measured way to split
them (`mission == nullptr`) changes the branch structure and is size-fatal. Breaking
the zero with a real load (`player == nullptr`) is far worse (9,474).

## 8. Reproduction (second pass)

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6272 aligned= 1941 size=9908/9908 insn=3543/3532
```
Scratch scripts: `sweep_pairs_all.py` (5.1), `sweep_flip1.py` (5.2), `sweep_f17.py`
(5.2), `sweep_tri.py` (5.3), `t3.py`/`t4.py`/`t5.py`/`t6.py`/`t7.py` (5.4/5.5).
