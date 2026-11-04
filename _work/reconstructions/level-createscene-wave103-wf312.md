# `Level::createScene` — wave-103 wf312: the KiritoJPK mod-rebuild oracle confirms the
# shipped source shape; the residual is clang-7 `-Oz` greedy register allocation

**Status:** research note, 2026-11-04. Worktree `wt/wf312`, branch `wave103-wf312`,
base `1d2b4398e`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave101-wf111.md` (which followed
`level-createscene-wave100-w5012.md`, `…-wave99-w4911.md`, `…-wave98-w4814.md`,
`…-wave97-w4712.md`, `…-wave95-w4514.md`, `…-wave94-w4414.md`,
`…-wave92-w4249.md`, `…-wave92-w4213.md`, `…-wave91-w4113.md`,
`level-createscene-phi-use-w4013.md`, `level-createscene-licm-hoist-w3833.md`,
`Level-createScene-flag-free.md`, `level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`.
* Gate re-measured before and after this pass, unchanged and green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* **New oracle read:** the KiritoJPK mod-rebuild Ghidra decompilation
  (`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Level.c`,
  `Level::createScene @0x000c2910`, 2398 B) is **semantically identical** to the shipped
  source: same parts loop (`mesh < 0` / `continue`, `AEGeometry` → `setRotation` →
  `PlayerStatic` → child loop → `ArrayAdd`), same `ownHangar`/`capacity`/`shipCount`
  prelude order, same `spotUsed` allocation/zero loop, same retry loop, same
  `spots`/switch table selection, and same ship-loop statement order. §1.
* The oracle prints `AbyssEngine::AERandom::nextInt(Globals::rnd, …)` at **every** site,
  i.e. it has no local `AERandom *rnd`. That is a **clang 3.8 `-O2` artifact**: the
  original clang 7 `-Oz` binary also reloads `Globals::rnd` at every site (no value
  cache), but the shipped local-`rnd` source shape is the only one that reproduces the
  original's **2252 B** size and **88.3 %** score. The literal all-globals shape is
  **75.0 % / 2228 B / 787 insns** because the `__stack_chk_guard` address then takes
  `r5` and shifts the whole allocation. §2–§3.
* The 128-mask local-vs-`Globals::rnd` sweep was re-run on this base with the
  createScene-scoped replacement (the old global `str.replace` hit `createMission`'s
  identical `if (rnd->nextInt(100) <= 29)` line). The unique maximum is still the
  shipped `{top, pick, tries}` = mask `0x29` at **88.3 %**. §4.
* ~40 further source shapes were measured this pass (oracle-shaped `table = spots`
  initialization, switch case orders, `spots` if/switch, `plainHangar` negation,
  `int`/`char`/`unsigned` booleans, local `player`/`geometry`/`status`/`globals`/
  `enemies` temporaries, loop forms, `rnd` declaration positions, pointer/reference
  forms, `volatile`, `auto`, `decltype`). **None beats 88.3 %.** §5.
* Conclusion: the source shape is already the original shape; the remaining
  difference is clang 7.0.2 `-Oz` `RAGreedy` colouring of the same IR. No source-only
  lever was found.

## 1. Oracle comparison

Oracle file:
`/home/debian/GalaxyOnFire2/_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Level.c`
(lines 4564–4980, `Level::createScene @0x000c2910`, 2398 B). It is a Ghidra
decompilation of the KiritoJPK NDK-r14 / clang-3.8 `-O2` mod rebuild, not a byte
oracle. The game-source body is the same revision family as 2.0.16 (localization /
`DeviceInfo` are the only known differences).

The oracle's `missionPtr == 23` branch is the same source as ours:

```c
int sceneRace;
if (getStation()->getIndex() == 101) sceneRace = 8;
else if (getStation()->getIndex() == 100) sceneRace = 7;
else sceneRace = ((SolarSystem *)status->getSystem())->getRace();
this->enemies = new Array<KIPlayer *>();
ArraySetLength(1, *this->enemies);
int shipIndex = status->getShip()->getIndex();
(*this->enemies)[0] = createShip(status->getShip()->getRace(), 0, shipIndex, nullptr,
                                 this->missionPtr != 23, false);
(*this->enemies)[0]->setPosition(0.0f, (float) kShipHangarHeights[shipIndex], 0.0f);
((PlayerFighter *) (*this->enemies)[0])->removeTrail();
((PlayerFighter *) (*this->enemies)[0])->setExhaustVisible(false);
(*this->enemies)[0]->setToSleep();
(*this->enemies)[0]->player->setAlwaysFriend(true);
bool plainHangar = sceneRace == 1 || sceneRace > 3;
for (int k = 0; k < 4; k++) {
    int mesh = kHangarMeshes[sceneRace][k];
    if (mesh < 0) continue;
    AEGeometry *g = new AEGeometry(mesh, Globals::Canvas, false);
    g->setRotation(Vector{0.0f, 3.1415927f, 0.0f});
    PlayerStatic *hangarPart = new PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
    if (!plainHangar) {
        for (int j = 0; j < kBarChildCounts[sceneRace]; j++) {
            AEGeometry *child = new AEGeometry(kBarChildMeshes[sceneRace] + j,
                                               Globals::Canvas, false);
            g->addChild(child->transform);
            delete child;
        }
    }
    ArrayAdd((KIPlayer *) hangarPart, *this->enemies);
}
bool ownHangar = false;
if (Globals::status->getStation()->getIndex() == 108)
    ownHangar = Globals::status->field_114 == 3;
int capacity = kHangarCapacity[sceneRace];
int shipCount = Globals::rnd->nextInt(capacity + 1);
if (ownHangar) { … shipCount = min(capacity, ships->size()) … }
unsigned char *spotUsed = new unsigned char[capacity];
for (int i = 0; i < capacity; i++) spotUsed[i] = 0;
AbyssEngine::AERandom *rnd = Globals::rnd;          // shipped source; oracle prints
const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];
for (int i = 0; i < shipCount; i++) {
    int race = sceneRace;
    if (rnd->nextInt(100) <= 29) { … }
    int type = Globals::globals->getRandomEnemyFighter(race);
    if (Globals::status->getStation()->getIndex() == 100) { … }
    if (ownHangar) type = (*Globals::status->voidStation->getShips())[i]->getIndex();
    KIPlayer *ship = this->createShip(0, 0, type, nullptr, false, false);
    int spot = Globals::rnd->nextInt(capacity);
    for (int tries = 100; tries != 0 && spotUsed[spot] != 0; tries--)
        spot = rnd->nextInt(capacity);
    spotUsed[spot] = 1;
    const int *table;
    switch (sceneRace) {
        case 0: table = kHangarSpotsTerran[0]; break;
        case 1: table = kHangarSpotsVossk[0]; break;
        case 3: table = kHangarSpotsNivelian[0]; break;
        case 8: table = kHangarSpotsVoid[0]; break;
        default: table = spots; break;
    }
    ship->setPosition((float) table[spot * 3],
                      (float) (table[spot * 3 + 1] + kShipHangarHeights[type]),
                      (float) table[spot * 3 + 2]);
    ship->player->setAlwaysFriend(true);
    ship->setToSleep();
    ship->geometry->setRotation(0.0f,
        (float) Globals::rnd->nextInt(300) / 100.0f, 0.0f);
    ((PlayerFighter *) ship)->setExhaustVisible(false);
    ArrayAdd(ship, *this->enemies);
}
delete[] spotUsed;
return;
```

The oracle's decompiled `AbyssEngine::AERandom::nextInt(Globals::rnd, …)` is exactly
what clang 3.8 `-O2` produces after optimizing the local `rnd` away. It does **not**
prove the original source lacks the local; the original clang 7 `-Oz` disassembly
likewise has no cached value, but that is the register allocator's choice, not the
source's.

## 2. The clang-7 disassembly: the `r5` cache is the whole residue

`python3 tools/verify/verify.py --show _ZN5Level11createSceneEv`:
first 459 instructions byte-identical; first diff at instruction 460, the parts-loop
preheader. The instruction stream is identical apart from:

1. the parts loop's register permutation
   (`row`/`Canvas`/`zero`/`k`/`hangarPart`/`childCount`/`childMesh`);
2. the ship-loop `rnd` handling: ours caches the `AERandom*` value in `r5` and spills
   `&Globals::rnd` to `[sp,#44]`; the original keeps `&Globals::rnd` in `r8` and
   reloads the value at every site;
3. the `switch (sceneRace)` table-selection group (a `+2/-1` load-order difference).

Register map (unchanged from wave-98/99/100/101):

| value | original | shipped |
|---|---|---|
| parts `row` | `r9` | `r8` |
| parts `Globals::Canvas` value | `r8` | `r9` |
| parts zero | `sl` | `r4` |
| parts `k` | `r6` | `r6` (spilled `sp#44`) |
| parts `hangarPart` | `r4` (spilled) | `r6` |
| parts `g` | `r5` | `r5` |
| parts `j` | `sl` | `sl` |
| parts child count | `fp` | `r8` |
| parts child mesh | `r9` | `fp` |
| ship `&Globals::rnd` | `r8` | spilled `sp#44` |
| ship `rnd` value | never cached | `r5` |
| ship `capacity` | `r6` | `fp` |
| ship `race` | `r5` | `r6` |
| ship `type` | `fp` | `r8` |
| ship `i` | `r9` | `r9` |
| ship `&Globals::status` | `sl` | `sl` |
| ship `spotUsed` | `r4` | `r4` |

The `r5` value cache is the cause of the whole ship-loop rotation. It is also what
keeps the `__stack_chk_guard` address out of `r5`: in the shipped shape the guard
address is loaded into `r0` and spilled to `[sp,#36]`, so the prologue stays
byte-identical to the original. Removing the local `rnd` (the literal oracle shape)
frees `r5`; the greedy allocator then gives `r5` to the guard address, moves
`&Globals::status` to `r4`/spill, and shifts the first diff to instruction 8.

## 3. The literal oracle shape (all-globals) is worse

All sites use `Globals::rnd` directly and the local declaration is removed:

| shape | pct | size | insns |
|---|---|---|---|
| shipped local `rnd` at `{top, pick, tries}` | **88.3** | **2252** | **791** |
| literal oracle: `Globals::rnd` at all sites, no local | 75.0 | 2228 | 787 |
| all-local `rnd` at all sites | 83.7 | 2236 | 786 |
| local `rnd` at `{top, spot, tries}` | 88.1 | 2252 | 791 |
| local `rnd` at `{top, tries, rotation}` | 88.2 | 2252 | 791 |

The 24-byte / 4-instruction shortfall of the all-globals shape is the guard-address
allocation change, not a missing source statement.

## 4. 128-mask local-vs-`Globals::rnd` sweep (createScene-scoped)

Sites in source order:

```
0  if (rnd->nextInt(100) <= 29)                 [shipped local]
1  race = Globals::rnd->nextInt(4);             [global]
2  if (Globals::rnd->nextInt(100) < 30)         [global]
3  int pick = rnd->nextInt(3);                  [shipped local]
4  int spot = Globals::rnd->nextInt(capacity);  [global]
5  spot = rnd->nextInt(capacity);               [shipped local]
6  (float) Globals::rnd->nextInt(300)           [global]
```

All 128 subsets were built with the official `link_so` target and scored. Top:

| mask | sites local | pct | size | insns |
|---|---|---|---|---|
| **0x29** | top, pick, tries (shipped) | **88.3** | 2252 | 791 |
| 0x61 | top, tries, rotation | 88.2 | 2252 | 791 |
| 0x31 | top, spot, tries | 88.1 | 2252 | 791 |
| 0x70 | spot, tries, rotation | 87.7 | 2252 | 791 |
| 0x69 | top, pick, tries, rotation | 87.7 | 2252 | 791 |
| 0x68 | pick, tries, rotation | 87.7 | 2252 | 791 |
| 0x38 | pick, spot, tries | 87.5 | 2252 | 791 |
| 0x39 | top, pick, spot, tries | 87.4 | 2252 | 791 |
| 0x78 | pick, spot, tries, rotation | 86.9 | 2252 | 791 |
| 0x79 | top, pick, spot, tries, rotation | 86.7 | 2252 | 791 |
| 0x71 | top, spot, tries, rotation | 86.4 | 2252 | 791 |
| 0x00 | none (all-globals) | 75.0 | 2228 | 787 |

The shipped `{0,3,5}` is the unique maximum. This reproduces the wave-98/99/100/101
result exactly; the earlier global-replacement bug (hitting `createMission`'s identical
`if (rnd->nextInt(100) <= 29)` line) is avoided by scoping the replacement to the
`createScene` block.

## 5. Other source shapes measured this pass (all ≤ 88.3 %)

| variant | pct | size | insns |
|---|---|---|---|
| shipped (base) | **88.3** | 2252 | 791 |
| `const int *table = spots;` + switch without `default` | 88.3 | 2252 | 791 |
| switch case orders (9 permutations, `default` first/last) | 88.3 | 2252 | 791 |
| `plainHangar` as `int` / `char` / `unsigned` | 88.3 | 2252 | 791 |
| `ownHangar` as `int` / `char` | 88.3 | 2252 | 791 |
| local `Player *shipPlayer = ship->player;` | 88.3 | 2252 | 791 |
| local `AEGeometry *shipGeometry = ship->geometry;` | 88.3 | 2252 | 791 |
| both ship-player/geometry locals | 88.3 | 2252 | 791 |
| local `Status *status = Globals::status;` inside ship loop | 88.3 | 2252 | 791 |
| local `Globals *globals = Globals::globals;` inside ship loop | 88.3 | 2252 | 791 |
| `rnd` declared after `spots` | 88.3 | 2252 | 791 |
| `rnd` as `AERandom &rnd = *Globals::rnd` at shipped sites | 88.3 | 2252 | 791 |
| `rnd` as `auto` / `decltype` | 88.3 | 2252 | 791 |
| `spots` as if/else instead of ternary | 86.5 | 2252 | 791 |
| `spots` as switch | 86.5 | 2252 | 791 |
| `table` as nested ternary | 83.3 | 2268 | 800 |
| `table` as if-chain | 74.3 | 2260 | 800 |
| `table` as reverse if-chain | 75.3 | 2256 | 798 |
| hoisted `specialTable` + if | 72.8 | 2256 | 806 |
| hoisted `specialTable` + switch | 75.0 | 2260 | 800 |
| outer parts loop `do { … } while (k < 4)` | 87.8 | 2252 | 791 |
| inner child loop `do { … } while (j < count)` | 69.1 | 2256 | 792 |
| ship loop `do { … } while (i < shipCount)` | 85.5 | 2248 | 789 |
| `if (shipCount > 0) for (…)` | 75.5 | 2268 | 804 |
| `if (shipCount > 0) do { … } while` | 79.2 | 2256 | 797 |
| `rnd` volatile at shipped sites | 76.8 | 2232 | 796 |
| `rnd` pointer-to-pointer at all sites | 75.0 | 2228 | 787 |
| `rnd` pointer-to-pointer at shipped sites | 75.0 | 2228 | 787 |
| `rnd` declared before `spotUsed` | 76.0 | 2228 | 794 |
| `rnd` declared before `shipCount` + local at shipCount only | 75.0 | 2228 | 787 |
| `rnd` declared before parts loop + local at shipCount only | 74.5 | 2244 | 791 |
| `int race = sceneRace;` hoisted before parts loop | 66.4 | 2264 | 804 |
| local `Level *self` / `Status *status` in prologue | 59.9–83.9 | 2200–2252 | 786–793 |
| local `enemies` pointer | 63.8–70.3 | 2232–2244 | 788–793 |

## 6. What this pass proves / what remains

**Proved:**

1. The KiritoJPK oracle's `Level::createScene` is semantically identical to the
   shipped source in every statement, loop, call order and constant. The only
   difference is the decompiler's direct `Globals::rnd` spelling.
2. The literal oracle shape (all-globals) is 75.0 % / 2228 B, not the original's
   2252 B; the shipped local-`rnd` shape is the only one that reproduces the
   original's size and 88.3 %.
3. The 128-mask sweep on this base is exhaustive and the shipped `{0,3,5}` is the
   unique maximum.
4. The residual is clang 7.0.2 `-Oz` `RAGreedy` colouring: the local `rnd` value
   cache in `r5` forces the `__stack_chk_guard` address to `r0`/spill and preserves
   the original prologue, but it also rotates `capacity`/`race`/`type`/`&rnd`.
   Removing the cache frees `r5` for the guard address and shifts the first diff to
   instruction 8.
5. ~40 further source shapes (table initialization, switch/if forms, booleans,
   temporaries, loop forms, declaration positions, pointer/reference/volatile
   forms) do not beat 88.3 %.

**Remaining lever (unchanged):** a source shape that keeps the shipped prologue
(`__stack_chk_guard` address in `r0`/spilled, `&Globals::status` in `sl`, `this` in
`r6`) while making the ship loop use `&Globals::rnd` in `r8` with the value reloaded
at every site, `capacity` in `r6`, `race` in `r5`, `type` in `fp`. Equivalently: the
all-globals IR with the shipped shape's global-interval queue. No source spelling
tried across waves 91–103 produces it.

## 7. Reproduction

Scratch is `/dev/shm/wf312` only.

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level11createSceneEv
```

Scratch scripts:

* `/dev/shm/wf312/Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `/dev/shm/wf312/fscore.py` — official single-function score;
* `/dev/shm/wf312/run2.py` — createScene-scoped replacement + `link_so` + score;
* `/dev/shm/wf312/gen_allmasks2.py` / `spec_allmasks2.json` — the 128-mask sweep;
* `/dev/shm/wf312/allmasks2.log` — raw sweep results;
* `/dev/shm/wf312/show_base.txt`, `show_allg.txt`, `show_status.txt`,
  `show_rndparts.txt` — side-by-side disassemblies.

The oracle is read-only and was not modified.
