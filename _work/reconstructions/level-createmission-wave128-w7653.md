# w7653: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-128
# Ghidra-decompile-driven call-site / local-reuse / mission-store-order sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7653`, based on current master.
Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; Ghidra address `0xbffd8`, linked `.text` address `0xaffd8`,
17172 B). Continues `level-createmission-wave121-w7112.md` …
`…-wave127-w7632.md`. `src/game/world/Level.cpp` is unchanged; this note is
the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`.  One matching
instruction is `+0.0165 pp`.

## 1. New evidence: full Ghidra decompile, not just disassembly

This wave decompiled the whole function with Ghidra headless (project copy in
`/dev/shm/w7653/gh`, script `/dev/shm/w7653/Decomp.java`; 113,439 chars of C
written to `/dev/shm/w7653/gh_decomp.c`).  The decompile confirms the current
source's loop shapes and store order in most places, and isolates the remaining
residual to register allocation.  The important new observations are:

### 1.1 Alien-turret loop (original source shape)

```c
iVar27 = 0;
iVar44 = *(int *)(this + 0x160);   // friendCount
iVar52 = *(int *)(this + 0x164);   // field_164
iVar53 = *(int *)(this + 0x168);   // field_168
for (iVar66 = iVar52 + iVar44 + iVar53;
     iVar66 < iVar44 + iVar34 + iVar52 + iVar53; iVar66++) {
    uVar16 = createStaticObject(this, 0, 0x1a74, true);
    enemies[iVar66] = uVar16;
    piVar49 = enemies[iVar66];
    Player::setMaxHitpoints(piVar49[1], 1000);
    if (bVar4) t = kAlienStationTurrets; else t = kAlienOutpostTurrets;
    ...
    AEGeometry::setRotation(piVar49[2]);
    ...
    setPosition(piVar49, &local_11c);
    PlayerTurret::setScaling(...);
    piVar49[0x14] = 0;      // cargo = nullptr
    iVar27 += 0x18;         // k += 1
    piVar49[10] = iVar19;   // shipGroup = localRace
    iVar44 = *(int *)(this + 0x160);
    iVar52 = *(int *)(this + 0x164);
    iVar53 = *(int *)(this + 0x168);
}
```

* The initial index is `field_164 + friendCount + field_168` (already in the
  source; the earlier wave-127 note's concern about this order is resolved).
* The original body order is `setScaling` **after** `setPosition`, then
  `cargo = nullptr`, then `shipGroup` (with `k += 1` scheduled between the two
  stores).  The current source uses the tuned `setScaling` before
  `setRotation`, `shipGroup` then `cargo`.
* The original table select is a real `cbz` diamond; ours is if-converted to
  `cmp.w r1, r9, asr #31; it ne; movne`.  This is the source of the alien-turret
  register permutation (`this` r4-vs-r5, `gun` r6-vs-r4, `&v` r5-vs-r8,
  `k*24` r9-vs-fp, `i` fp-vs-r9, table sl-vs-r6).

### 1.2 `case4Tail` loop (original source shape)

```c
this->enemies = new Array<KIPlayer *>();
ArraySetLength(hunterCount, *this->enemies);
for (uVar61 = 0; uVar61 < this->enemies->size(); uVar61++) {
    iVar15 = getRandomEnemyFighter(8);
    pRVar28 = this->playerRoute;
    iVar19 = Route::length(pRVar28);
    iVar19 = nextInt(rnd, iVar19);
    pWVar38 = Route::getWaypoint(pRVar28, iVar19);
    uVar16 = createShip(this, 8, 0, iVar15, pWVar38, true, false);
    enemies[uVar61] = uVar16;
    KIPlayer::setToSleep(enemies[uVar61]);
}
Objective(0x12, 0, hunterCount, this);
```

The original re-reads `this->enemies->size()` every iteration and spills
`&Globals::rnd` to `sp+120` (set early at `+0x29e` and reloaded at `+0x41a2`).
The current `volatile unsigned n` latch is the only size-exact shape; the
faithful re-read is 8 B short (17164) because clang rematerialises
`&Globals::rnd` instead of spilling it.

### 1.3 Mission-object store order in the case-183 block

The decompile shows, for `enemies[8]`:

```c
createStaticObject(0, 0x495d, false);
setMoving(false); setDockingType(2); setName(getText(3208));
player->setAlwaysFriend(true);
geometry->rotate({0, -2.3561945, 0});
*(int *)(enemies[8] + 0x28) = 10;   // shipGroup
*(char *)(enemies[8] + 0x74) = 1;   // field_0x74
delete cargo; cargo = nullptr;
```

i.e. `shipGroup` before `field_0x74`, the opposite of the current source.  The
same swap appears for `enemies[9]`.

## 2. Variants measured this session (~40 fresh builds)

Each row is a fresh `cmake --build cmake-build-match --target link_so -j6`
followed by the official verifier's normalized stream.  Baseline is
**92.326733 / 17172 / 6065**.

### 2.1 Alien-turret body / store order

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline (setScaling before setRotation, shipGroup then cargo, fresh setScaling receiver)** | **92.326733** | 17172 | 6065 |
| faithful order, `gun` receiver for setScaling | 92.193431 | 17172 | 6063 |
| faithful order, fresh `(*enemies)[i]` receiver | 92.260726 | 17172 | 6065 |
| setScaling after setPosition, shipGroup then cargo | 92.277228 | 17172 | 6065 |
| cargo before shipGroup (setScaling before setRotation) | 92.310231 | 17172 | 6065 |
| initial index `field_164 + friendCount + field_168` (both loops) | 92.310231 | 17172 | 6065 |

### 2.2 Alien-turret table select / anchor

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `(bigAlienWave + (i < 0))` ternary** | **92.326733** | 17172 | 6065 |
| `(bigAlienWave + (i / 0x80000000))` | 92.326733 | 17172 | 6065 |
| `(bigAlienWave + (i & 0x80000000))` | 92.277228 | 17172 | 6065 |
| `(bigAlienWave + (i % 0x80000000))` | 92.277228 | 17172 | 6065 |
| statement-`if` without anchor | 88.413577 | 17140 | 6054 |
| `switch` without anchor | 88.413577 | 17140 | 6054 |
| `(bigAlienWave != 0)` without anchor | 88.413577 | 17140 | 6054 |
| `volatile int bigAlienWave` | 87.756955 | 17140 | 6058 |
| declaration permutations of `Matrix rotation` / `Vector v` / `Vector base` | 91.947195–92.161716 | 17172 | 6065 |

### 2.3 `alienGuns` spill / loop-local reuse

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.326733** | 17172 | 6065 |
| `int ag = alienGuns;` used in the loop condition | 92.326733 | 17172 | 6065 |
| `volatile int ag = alienGuns;` used in the loop condition | 91.551155 | 17172 | 6065 |
| `const volatile int ag = alienGuns;` | 91.551155 | 17172 | 6065 |
| `int type; Route *route; int idx; Waypoint *wp;` reused outside the case-1 loop | 92.326733 | 17172 | 6065 |
| same locals reused outside the case4Tail loop | 92.326733 | 17172 | 6065 |
| `struct ShipArgs { bool hostile; bool group; }` in the tail loop | 92.326733 | 17172 | 6065 |
| same struct in the raid loop | 92.326733 | 17172 | 6065 |
| `Status *status` + `AERandom **rndp` aliases in the tail | 92.326733 | 17172 | 6065 |
| global `Level *self = this` alias (whole function) | 92.326733 | 17172 | 6065 |
| global `Status *status = Globals::status` alias | 60.683972 | 17252 | 6080 |
| global `Globals *globals = Globals::globals` alias | 54.628325 | 16988 | 6012 |
| `AERandom **rndp = &Globals::rnd` at function top, all uses | 92.326733 | 17172 | 6065 |
| same + faithful tail re-read | 91.341312 | 17164 | 6060 |
| `AERandom **volatile rndp` at function top | 63.708087 | 17244 | 6113 |

### 2.4 Mission-object store order / setup shape

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.326733** | 17172 | 6065 |
| case-183 `enemies[8]` shipGroup before field_0x74 | 92.326733 | 17172 | 6065 |
| case-183 `enemies[9]` shipGroup before field_0x74 | 92.310231 | 17172 | 6065 |
| both case-183 swaps | 92.310231 | 17172 | 6065 |
| pirate-fortress cargo before shipGroup | 92.277228 | 17172 | 6065 |
| pirate-fortress `const float (*t)[6]` + cargo before shipGroup | 92.251836 | 17172 | 6064 |
| pirate-fortress `const float (*t)[6]` (current store order) | 92.284842 | 17172 | 6064 |
| `friendCount` ternary -> if/else | 92.326733 | 17172 | 6065 |
| stationIndex==78 branch `extraFriend = 0` first | 92.326733 | 17172 | 6065 |
| `i == 0 && hasWanted` (swap `&&` operands) | 80.614013 | 17164 | 6062 |

## 3. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  The decompile confirms that the current source's mission-store order,
loop-bound recomputation, and call-site argument evaluation order are already
faithful in the places that matter.  The residual is entirely backend:

1. alien-turret table select: original `cbz` diamond vs our if-converted
   `cmp.w r1, r9, asr #31; it ne; movne`, producing the whole cyclic register
   permutation (`this` r4-vs-r5, `gun` r6-vs-r4, `&v` r5-vs-r8, `k*24`
   r9-vs-fp, `i` fp-vs-r9, table sl-vs-r6) — ~2.7 pp;
2. `case4Tail`: `fp = 1` vs `fp = &Globals::rnd`; the faithful re-read is 8 B
   short because clang rematerialises the GOT address instead of spilling it —
   ~1 pp;
3. `raid`/`__stack_chk_guard` `sp+84`/`sp+88` swap (17 sites, ~0.28 pp);
4. `case4Else`/`case4Tail` `this` r8-vs-sl;
5. case-183 test order 2,1,0 + body order 0,1,2,default (MBP);
6. branchless `&&` at `+0x2e04`/`+0x2f52` (LICM hoists `!alienWave`);
7. pirate-fortress `ldmia`-vs-`ldrd` SLP pair.

No source spelling tried this wave moves the `cbz`/if-conversion or the
`fp=1`/`fp=&Globals::rnd` tie-break without losing the size-exact frame.

## 4. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7653/score.py            # exact ratio, size, insns
python3 /dev/shm/w7653/analyze2.py         # mismatch regions with addresses
cmake --build cmake-build-match --target check -j6
```

Ghidra headless (project copy, because the live project is locked):

```text
cp -r ../_work/bins/android_2.0.16_libgof2hdaa.so_ghidra.{gpr,rep} /dev/shm/w7653/gh/
/tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless /dev/shm/w7653/gh \
  android_2.0.16_libgof2hdaa.so_ghidra -process android_2.0.16_libgof2hdaa.so \
  -scriptPath /dev/shm/w7653 -postScript Decomp.java 0xbffd8 -noanalysis
```

Scratch under `/dev/shm/w7653` (`Level.base.cpp` = pristine HEAD copy;
`score.py`, `analyze2.py`, `variant.py`, `v*.py` = the ~40 variant specs;
`gh_decomp.c` = full Ghidra decompile).

## 5. Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
stub_zero_size     0 -> 0
missing            0 -> 0
wrong_type         0 -> 0
extra              0 -> 0
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
```
