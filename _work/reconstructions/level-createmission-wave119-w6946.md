# w6946: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-119
# Ghidra re-decompile + anchor/order sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6946`, branch `wave119-w6946`,
based on current master (`7a031742d`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
Continues `level-createmission-wave119-w6925.md` (stack-slot / tail / convoy /
turret sweep) and `…-w6918.md` (negated `bigShipsCount`, 92.326733).
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)`; one extra matching instruction is
`+0.0165 pp`, i.e. 92.3267 -> 92.3432.

## 1. New evidence: full Ghidra re-decompile

`rea decompile … --procedure _ZN5Level13createMissionEv --format json`
(`/dev/shm/w6946/rea.c`, 2391 lines) was re-read end to end against
`src/game/world/Level.cpp`.  The decompiled control flow matches the current
source block-for-block; the only source-visible differences are the ones
already catalogued in w6925.  Notable confirmations:

* `alienGuns = alienWave ? (bigAlienWave ? 8 : 7) : 0;` and
  `fortressGuns = pirateFortress ? 5 : 0;` (decompiler `iVar26`/`iVar27`).
* The alien-turret table select really is a plain `if (bVar4) t = A; else t = B;`
  **inside** the loop body (`cbz` at `0xb32c8`), not hoisted.
* The fortress loop walks a running `float *pfVar52` (`pfVar52 += 6`), i.e.
  `kBaseTurrets[k]`.
* `case 4` is a plain `if/else` (`nextInt(2)` -> `createRoute` vs
  `stationPosition` Route) followed by the shared hunter tail; the
  `case4Else:`/`case4Tail:` goto labels reproduce the original block placement.
* `uVar50 = iVar11 - 10U & 0x7fffffff;` — the `bic r1,r5,#0x80000000` in the
  case-183 convoy k-chain is present in **both** binaries, so it is not a
  difference.
* `hunterCount` is `unsigned` in the original (`uVar35`), and the tail loop
  re-reads `this->enemies->size()` on every back edge.

## 2. Variants measured this session (all 17172 B unless noted)

Baseline 92.326733 %.

| variant | % |
|---|---:|
| **baseline** | **92.326733** |
| `switch (k) { case 2/1/0 … }` (same as baseline) | 92.326733 |
| `switch (k) { case 0/1/2 … }` | 92.178218 |
| `switch (i) { case 12/11/10 … }` | 92.326733 |
| `if (i == 12) … else if (i == 11) … else if (i == 10)` | 91.807607 |
| `if (k == 3) default-first chain` | 91.395099 |
| `Level *self = this;` before both turret blocks (all `this->` -> `self->`) | 92.326733 |
| `Level *self = this;` before, alien block only | 92.326733 |
| `int baseIndex = friendCount + field_164;` hoisted | 73.448276 (17216) |
| `int first = guns + friendCount + field_164 + field_168;` in raid loop | 91.125041 (17180) |
| `int pirateFirst = …` in pirate loop | 90.319386 (17176) |
| both `first` hoists | 87.654729 (17168) |
| `unsigned guns` | 92.326733 |
| `if (alienGuns)` | 92.326733 |
| `KIPlayer *first = enemies[…]; Vector base = first->getPosition();` | 92.326733 |
| `((PlayerTurret *) gun)->setScaling(6.0f)` (drop fresh-load anchor) | 90.955603 (17164) |
| fortress `setScaling` after `setPosition` (fresh load) | 90.942089 (17180) |
| alien `setScaling` after `setRotation` | 92.326733 |
| alien `setScaling` after `setPosition` | 92.277228 |
| `cargo = nullptr` before `shipGroup` (both loops) | 92.211221 |
| same, with `gun` instead of fresh load | 92.143918 (17163) |
| alien only / fortress only | 92.260726 / 92.277228 |
| `bool hostile = true;` in tail | 92.326733 |
| `volatile int n` instead of `volatile unsigned n` | 92.326733 |
| `unsigned n` / `int n` (non-volatile) | 92.178218 |
| `unsigned hunterCount` | 92.310231 |
| `unsigned hunterCount` + faithful re-read | 91.324804 (17164) |
| `Objective(18, 0, enemies->size(), this)` + re-read | 89.720089 (17156) |
| `bool bigAlienWave` + `bool alienWave` + `bool pirateFortress` | 74.304581 (17304) |
| `volatile int bigAlienWave` (anchor / plain) | 87.756955 / 87.766221 (17140) |
| `volatile bool bigAlienWave` (anchor / plain) | 86.235654 / 86.360634 (17148) |
| anchor `(bigAlienWave && i >= 0)` / `(i >= 0 && bigAlienWave)` / `(bigAlienWave \|\| i < 0)` | 92.161716 |
| anchor `(bigAlienWave + (int)(i < 0))` / `+ !(i >= 0)` / `+ (i >> 31)` | 92.326733 |
| anchor `(bigAlienWave + (i & 0))` / `+ 1 - 1` | 88.413577 (17140) |
| `init t = B; if (bigAlienWave + (i<0)) t = A;` | 92.326733 |
| `if/else` with anchor | 92.326733 |
| `init t = B; if (bigAlienWave) t = A;` (plain) | 88.413577 (17140) |
| `unsigned k` / `size_t k` | 92.326733 |
| `unsigned i` in turret loops | 88.413577 (17140) |
| `unsigned pirateEscorts` | 92.310231 |
| `unsigned extraAliens` | 92.310231 |
| `unsigned alienPercent` | 91.848185 |
| `bool convoy` | 86.039416 (17176) |
| `int blueprint` (was `unsigned`) | 92.326733 |
| `unsigned count` in alien-orbit block | 92.293729 |
| swap the 68/33 campaign-mission checks | 92.293729 |
| `int raiders` instead of `float raiders` | 77.518104 (17212) |
| `if (raiders >= 1.0f)` / `if ((int) raiders > 0)` | 77.528182 (17220) / 92.326733 |
| `bool raid = false;` at top of isEmpty block | 91.485149 |
| guard-clause `if (!nameLeader) return;` | 92.326733 |
| `no-guns-var` (`alienGuns + fortressGuns` inline everywhere) | 88.855894 (17200) |

## 3. Where the residual lives (address-aligned cluster census)

266 clusters / 929 differing instruction slots, by 0x400 window:

```text
0xaffd8  10   0xb17d8  41   0xb2fd8 156   0xb3bd8  94
0xb03d8  16   0xb1fd8   4   0xb33d8 141   0xb3fd8  71
0xb07d8  20   0xb27d8  16   0xb37d8 212
0xb0bd8   4
0xb0fd8  34
```

150 of the 266 clusters are single-instruction replacements; 36 are 2:2.
They are dominated by:

1. `raid` (`[sp,#84]`) vs `Globals::status` (`[sp,#88]`) spill slots swapped
   (17 sites, `+0.28 pp` if isolated).  `Globals::status` is spilled at
   `0xb004c` (entry), `raid` at `0xb04e8`; both land in the 8-byte
   `fi#162`/`fi#163` slots.  No source spelling moves them (w6925 §2, re-checked
   here with `bool raid = false;` hoisted into the isEmpty block: 91.485149).
2. The `{sp+40, sp+44, sp+48}` 3-cycle (≈10 sites here plus the convoy
   `miningPlantIndex` sum at `0xb3f8e`-`0xb3fa4`).
3. Alien-turret register roles: orig `r4=this, r6=gun, r9=k, fp=i,
   sl=turret-table`; ours `r5=this, r4=gun, fp=k, r9=i, sl=&rotation`, and
   `this` is reloaded twice (`0xb3260`, `0xb3280`) instead of once in the
   preheader (`0xb3232`).  `Level *self = this;` before the block is neutral.
4. `case4Tail`: orig keeps `fp = 1` and `sl = Globals::globals`, spilling
   `&Globals::rnd` at `sp+120`; ours keeps `fp = &Globals::rnd` and
   rematerialises `movs r0,#1; str r0,[sp,#4]` instead of `strd r0,fp,[sp]`.
5. `&&` at `0xb2e04`/`0xb2f2a`: orig emits a branchless
   `orrs (i != bound), (flag ^ 1)`; ours short-circuits.  Clang 7's
   `EmitBranchOnBoolExpr` only takes the branchless `EvaluateExprAsBool` path
   when the `&&` is *not* the top-level `if` condition, and
   `ShouldEmitAsBranches` only merges two `icmp` against the *same null*
   constant, so `(i != bound) | (flag ^ 1)` is unreachable from any of
   `&&`, `||`, `!`, `&`, `|`, bool locals or ternary spellings tried
   (w6925 §8 and this note).
6. `case 183` k-chain: orig tests `[2,1,0]` with bodies laid out `[0,1,2]`;
   ours tests `[0,1,2]` with bodies `[2,0,1]`.  The MIR before
   MachineBlockPlacement already has the wrong body order for every source
   spelling tried, so this is MBP, not source.

## 4. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  83 new variants were measured this session (on top of the ~400 in the
preceding wave-119 notes); none beat the baseline.  The residual is compiler
register allocation, stack-slot colouring and if-conversion, not source shape.

## 5. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6946/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w6946/align.py            # address-aligned diff -> align.txt
python3 /dev/shm/w6946/batch.py bN.json    # apply/build/score/restore a batch
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w6946` (`Level.base.cpp` = pristine HEAD copy,
`vk*.json`, `vt*.json`, `vb*.json`, `vg*.json`, `vi.json`, `vn.json`,
`vr.json`, `vty.json`, `vf.json`, `vv.json`, `va.json`, `vc.json`,
`vz.json`, `vw.json` = variant
batches; `align.txt` = address-aligned diff; `rea.c` = Ghidra decompile;
`ifc.log` = `-mllvm -print-after-all` dump).
