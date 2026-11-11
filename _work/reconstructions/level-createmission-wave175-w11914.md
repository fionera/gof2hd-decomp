# w11914 (wave 175): `Level::createMission` — faithful bool/order shape with
# `globals`/`rnd`/`enemies` loop hoists, three expect hints and the wave-173
# fortress-bound reorder: size-exact 17,172 B and raw 5,421 -> 5,285 (-136)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11914`, branch
`wave175-w11914`, base master `5f599a137` (wave 174).  Continues
`level-createmission-wave174-w11914.md` (faithful bool/order shape was
size-exact at 17,172 B but raw regressed 5,421 -> 7,147) and
`level-createmission-wave173-w11814.md` (landed 5,425 -> 5,421).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline and landed result

```text
                                     size            raw    aligned  insn
HEAD (base 5f599a137)                17172/17172     5421   940      6204/6197
landed (this pass)                   17172/17172     5285   958      6212/6197
```

`.text` differing bytes, measured in this worktree: **80,439 -> 80,303**
(-136; the task brief's baseline is 80,447, which would make the result
80,311).  Gate after the change: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size **0**, missing **0**,
wrong_type **0**, extra **0**, data_exact **843/843**, imports **207**, lint
**CLEAN ✓**, drift **0**, symbol parity **5370/5370**, `.rel.plt`
byte-identical.  `RATCHET PASS`.

## Landed source shape

The wave-174 note proved the original's flag block is `bool`-typed and its
guards are `bool`-first (`alienWave && i == first`), not `i == first &&
alienWave`.  This pass keeps that shape and adds only codegen-neutral
reorders/hints inside `case4Tail` and the three branch hints:

```cpp
    // flags
    bool bigAlienWave = false;
    bool alienWave = false;
    bool pirateFortress = false;
    ...
    if (alienWave && i == this->friendCount + this->field_164) { ... }
    else if (pirateFortress && i == this->friendCount + this->field_164) { ... }
    ...
    if (__builtin_expect(extraAliens >= 1, 1)) { ... }        // lint: arm_builtin
    ...
    if (__builtin_expect(blueprint, 1)) { ... }               // lint: arm_builtin
    ...
    if (__builtin_expect(Globals::status->getStation()->stationHasHiddenBlueprint(false), 1)) { // lint: arm_builtin
    ...
    // case4Tail hunter loop
    Globals *globals = Globals::globals;
    AERandom *rnd = Globals::rnd;
    Array<KIPlayer *> *enemies = this->enemies;
    for (unsigned i = 0; i < enemies->size(); i++) {
        KIPlayer *ship = this->createShip(
                8, 0, globals->getRandomEnemyFighter(8),
                this->playerRoute->getWaypoint(rnd->nextInt(this->playerRoute->length())), true, false);
        (*enemies)[i] = ship;
        ship->setToSleep();
    }
```

The wave-173 fortress-bound operand order is kept:

```cpp
for (int i = this->field_168 + this->field_164 + this->friendCount;
     i < this->friendCount + this->field_164 + this->field_168 + fortressGuns; i++) {
```

All changes are semantics-preserving for this function: the three hoisted
values are loop-invariant pointers (the called functions do not mutate
`Globals::globals`, `Globals::rnd` or `Level::enemies`), the `+` operands are
commutative `int`s, and `__builtin_expect` does not change the value.

## Mechanism

`-Oz` sizes are dominated by the shared literal pool and by the register
allocator's choice of which loop-invariant global address to keep live:

* The faithful bool/order shape alone (wave 174) was 17,172 B only with the
  `bool alienWave` + `bool pirateFortress` subset; the full three-`bool`
  shape is 17,176 B (+4).
* Hoisting `Globals::globals` alone (`g`) drops raw to 5,130 but is +4 B.
  Hoisting `this->enemies` alone (`e`) drops raw to 5,184 but is +4 B.  In
  both cases the compiler spills the other global address and needs one more
  GOT literal than the original's 6-entry pool.
* Adding the `AERandom *rnd = Globals::rnd;` hoist (`r`) changes the loop
  preheader so the extra GOT literal disappears; `re` is exactly 17,172 B
  (raw 5,288) and `gre` is exactly 17,172 B (raw 5,285).  The enemies hoist
  also removes the second `ldr [this,#248]` in the loop body, which is the
  4-byte compensation for the extra literal.
* The three `__builtin_expect(..., 1)` hints are required: without them
  `gre` is 17,176 B / raw 5,793; dropping only the `extraAliens` hint gives
  17,176 B / raw 5,805.
* The fortress-bound reorder is a pure `+` operand permutation; it leaves raw
  unchanged (5,285) and improves aligned 962 -> 958.

## Measured variants (all size-exact rows first)

| shape | size | raw | aligned | insn |
|---|---:|---:|---:|---:|
| HEAD (int flags, int-first guards) | 17172 | 5421 | 940 | 6204/6197 |
| `r` (rnd hoist) | 17172 | 5293 | 948 | 6209/6197 |
| `re` (rnd + enemies) | 17172 | 5288 | 955 | 6211/6197 |
| **`gre` (globals + rnd + enemies)** | **17172** | **5285** | **958** | **6212/6197** |
| `gre` without fortress-bound reorder | 17172 | 5285 | 962 | 6212/6197 |
| `gre` without any expect | 17176 | 5793 | 966 | 6214/6197 |
| `gre` without `extraAliens` expect only | 17176 | 5805 | 983 | 6214/6197 |
| `g` (globals only) | 17176 | 5130 | 955 | 6211/6197 |
| `e` (enemies only) | 17176 | 5184 | 969 | 6212/6197 |
| `ge` (globals + enemies) | 17176 | 5189 | 971 | 6212/6197 |

`g`/`e`/`ge` have lower raw but are +4 B; every attempt to find a second
4-byte compensation on top of them (fortress-bound/convoy/`cargo` operand
orders, `e_haswanted`, `r_guns`, `c_102/012/120/021/210`, `-S`/`-D` loop
body orders, `volatile`/pointer-to-pointer rnd forms) either stayed +4 or
lost the raw win.  `gre` is the best size-exact variant found.

## Commands

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w11914/`
(`gen48.py` loop-shape generator, `sweepQ.py`/`sweepR.py`/`sweepS.py`
sweeps, `Level.base.cpp`, saved `head2.so`/`g.so`/`e.so`/`gre.so`).
