# LevelScript::process — wave 181 / w12505: switch-case / data-flow / literal-pool pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
original `0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12505`, branch `wave181-w12505`, base master
`1e270aa0e` (wave-180 harvest, raw **52,818**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12505`.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,818           52,816            (-2)
process aligned insns    20,109           20,109            (0)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
.text diffbytes          76,200           76,198            (-2)
.ARM.extab diffbytes     713              713               (unchanged)
.rel.plt                 md5 289b2a06…    md5 289b2a06…     (byte-identical)
```

## Landed change

One three-store rotation inside the mission-79 branch (source ~line 2811):

```diff
-                            m_bCinematicBreak = 0;
-                            m_nScriptTimerA = 0;
-                            m_nState = 14;
+                            m_nScriptTimerA = 0;
+                            m_nState = 14;
+                            m_bCinematicBreak = 0;
```

All three are independent stores to distinct fields; the rotation is
semantics-preserving.  It is the only non-adjacent triple rotation that wins
on this base (see below).

## Method

`scratch/w12505/harness.py` compiles one candidate `LevelScript.cpp` with the
exact `compile_commands.json` command and links it with the exact gold + strip
+ `reorder_shstrtab.py` pipeline into a private directory, then measures the
same-address raw diff and `.rel.plt` md5 (~1.5 s per candidate).  **Note:** the
link command in `build.ninja` names the LevelScript object as
`CMakeFiles/gof2.dir/./src/game/world/LevelScript.cpp.obj` (with `./`); a
replacement key without the `./` silently links the base object and makes every
candidate look identical.  The harness replaces the `./` form.

Sweeps run (all on the wave-180 base, all size-exact / `.rel.plt`-clean
candidates reported):

* **116 data-flow variants** — each `for (unsigned int …` ↔ `for (int …`
  occurrence, and simple `X == 0` → `!X`, `X != 0` → `X`, `!X` → `X == 0`.
  No wins; one loop flip changes size (57236).
* **43 Vector-construction variants** — `Vector zero{0,0,0}` → default ctor /
  int literals / three component stores, and the case-158 `Vector target`
  equivalent.  No wins; the 4th `zero` site is load-bearing (assignment form
  is +418 raw).
* **24 permutations of the four top-level local initialisers**
  (`messages/player/enemies/status`).  Base order is the unique best; the
  `status` alias is worth +1 raw if dropped (matches w12306).
* **134 `&&`/`||` operand swaps.**  One size-exact relplt-clean -1
  (`player->getRoute() != 0 && …->getWaypoint(0)->state`), but it reorders the
  null check after the dereference (semantic change) and the original
  disassembly at `0x1395b6` has the null check first, so it was not landed.
* **88 `== 0`/`!= 0`/`nullptr`/`!x` polarity rewrites.**  No wins.
* **21 receiver-alias removals** (`Level *lvl`, `TargetFollowCamera *cam…`,
  `KIPlayer *ship`, …).  No wins.
* **150 three-line rotations** of consecutive pure single-line assignment
  triples (all 6 permutations of each of the 30 triples).  Six size-exact
  relplt-clean cells: four -1 and one -2.  The -2 cell is landed.  The -1
  cells were `m_bCinematicBreak/m_pHud/m_pRadar` (case-158 intro), the
  `Status *st`/`Level *lvl`/`field_0x24` declaration block (mission 79), and the
  `camPos.x/camPos/camPos.y` float-accumulation order (case 70) — the last is
  not bit-exact in floating point and was not landed.

## Rejected / observations

* **Register-allocation delta is global.**  The original keeps `this` in `sl`
  and `&Globals::status` in `fp`; ours is the exact opposite (`this`=`fp`,
  `status`=`sl`).  This is the main reason raw stays ~52.8 KB.  Dropping the
  `Status *status` local, reordering the top-level initialisers, adding a
  `Level *lvl = m_pLevel` alias, and the 116 data-flow variants do not flip it.
  The original's prologue also loads `Status*` directly into `r0` where ours
  needs an extra `mov r0, r4`.
* **Block placement of cold tails differs.**  The original keeps the case-73
  state-7 tail inline at `0x139c30`; ours outlines it to `0x145a4c` (function
  end), which is why the final literal pool has different constants
  (orig: 100, -12000, 0.8, 3000, 6500, 25, 1.7; ours: 8000, 500, 1000, 10000,
  2400, 0.006, 4800) and why the case-70/case-16 body-size deltas are large.
  This is a block-placement effect, not a missing statement; expect cells are
  w12501's area.
* `case 144`'s `m_nState = 2` store is inline in ours but merged into the
  shared `0x144c0c` tail in the original; moving the store after `setTarget`
  is +624 raw.
* The mission-106 chain is compared `3,2,1` in the original and `1,2,3` in
  ours; reordering the source blocks to `3,2,1` is +50 raw (and `3,2,1` block
  orders change `.rel.plt`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12505
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52816 aligned= 20109 size= 57232/57232
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52816 size=57232/57232 text=76198 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2
```

Sweep harnesses live in `/home/debian/GalaxyOnFire2/scratch/w12505`
(`harness.py`, `sweep1.py` … `sweep5.py`); scratch, not tracked.
