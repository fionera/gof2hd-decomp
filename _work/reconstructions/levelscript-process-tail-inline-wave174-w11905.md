# LevelScript::process — wave 174 / w11905: mission-78 tail, the missing `return`, and the inlined-vs-called split

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11905`, branch
`wave174-w11905`, base master `be9dd429b` (wave 173).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11905/`.

Follow-up to `levelscript-process-tail-inline-wave173-w11805.md` (w11805) and
`levelscript-process-blockorder-wave173-w11806.md` (w11806).

## Result (no source change landed; note only)

```
                         base (HEAD)      this wave
process raw diffbytes    53,711           53,711     (no change)
process aligned insns    18,228           18,228     (no change)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20830 / 20728    20830 / 20728
.text diffbytes          80,897           80,897
.rel.plt                 byte-identical   byte-identical (3675 entries)
gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN
      drift 0 / symbol parity 5370/5370 / RATCHET PASS
```

No live-tail source shape beat base raw 53,711 while staying size-exact **and**
keeping `.rel.plt` byte-identical.  Every size-exact live-tail shape rotates
`.rel.plt`; the one shape that keeps `.rel.plt` identical (tail moved to the
end of the function) is +236 B.  Base kept; this note is the deliverable.

## 1. Baseline layout (re-measured)

```
original                              ours (base)
137d18 prologue                       137d18 prologue
137f7c second chain (m_bStartSequence) 137f76   (-6 B)
13837e mission-78 dispatch             138320   (-0x5e)
138c80 mission-106 check               138ca6   (+0x26)
13c830 mission-78 case-10 fade         13cdce   (+0x5a0)
13c866 mission-78 case-8               13ce0a   (+0x5a4)
13c88c mission-78 tail                 (absent)
13c890   ldr r0,[sl,#28] / subs #5
13c8ba   ldrb r5,[sl,#17] / b.w 144f5a
144f5a shared bool epilogue            144f5a
```

The dead tail is `src/game/world/LevelScript.cpp` lines 1246-1248, inside the
`switch` braces after `case 11`'s `break;`.  Every case ends in `break;`, so
the tail is unreachable and clang drops it; the dispatch `bhi` goes to the
second-chain entry `0x137f76` instead of `0x13c88c`.

The original's tail block (`0x13c88c`..`0x13c8c2`, 54 B including the `nop`
pad before the literal pool at `0x13c8c4`) is:

```
13c88c  ldr  r0,[sl,#28]      ; m_nState
13c890  subs r0,#5
13c892  cmp  r0,#3
13c894  bhi  13c8ba
13c896  ldr  r0,[sp,#76]      ; delta
13c898  movs r2,#0
13c89a  vldr s2,[pc,#92]      ; 0.8f
13c89e  movs r3,#0
13c8a0  negs r0,r0
13c8a2  vmov s0,r0
13c8a6  vcvt.f32.s32 s0,s0
13c8aa  ldr  r0,[sl,#20]      ; m_pCamera
13c8ae  vmul.f32 s0,s0,s2
13c8b2  vmov r1,s0
13c8b6  blx  TargetFollowCamera::translate
13c8ba  ldrb r5,[sl,#17]      ; m_bCinematicBreak
13c8be  b.w  144f5a
```

20 branch sites target `0x13c88c` (the dispatch `bhi` at `0x138380`, every
mission-78 case `break`/early-exit, and case 8's fallthrough).  All 20 are
inside the mission-78 region `0x138380`..`0x13be92`; none is outside it.

## 2. Key new finding: the mission-78 branch must `return`

The base's mission-78 branch *falls through* to the second chain
(`if (m_bStartSequence != 0) ... else if (mission == 5) ...`), which is at
`0x137f76`.  In the original the mission-78 branch **returns**: the tail ends
with `ldrb r5,[sl,#17]; b.w 144f5a`, and `0x144f5a` is the stack-canary-checked
function epilogue (`cmp r5,#0 / it ne / movne r5,#1 / ... popeq {r4,r5,r6,r7,pc}`).
Every one of the original's 20 mission-78 case-body branches targets
`0x13c88c`; none targets the second-chain entry `0x137f7c`.  In the base the
same case bodies (and the dispatch `bhi`) target `0x137f76` instead, so the
base's mission-78 branch falls through into the second chain.

So the base is semantically *not* faithful here (mission 78 in the base keeps
running the second chain).  Adding the missing `return` is a real correctness
fix, but it is expensive:

| shape | raw | aligned | size | `.rel.plt` |
|---|---:|---:|---:|---|
| base (dead tail, falls through) | **53,711** | 18,228 | 57,232 | identical |
| `return` after the switch (tail dead) | 55,355 | 23,752 | 57,272 (+40) | DIFF |
| `return` + both PLT anchors removed | 55,305 | 23,736 | **57,232 (exact)** | DIFF |
| `return` + one PLT anchor removed | 55,287 | 23,745 | 57,256 (+24) | DIFF |

`return` + both anchors is the only shape found that is **size-exact and
semantically faithful**, but raw is +1,594 and `.rel.plt` rotates, so it is not
landable under the current gate.

## 3. Live-tail shape sweep (all full link, size and `.rel.plt` checked)

`A` = the tail moved out of the `switch` braces to just after the closing
brace (all `break;` then target it).  `goto` = every mission-78 `break;`
replaced with `goto mission78_tail;` and the tail emitted at a label.

| shape | raw | aligned | size | `.rel.plt` |
|---|---:|---:|---:|---|
| **base (tail dead)** | **53,711** | **18,228** | **57,232** | **identical** |
| A: tail after switch | 55,473 | 20,945 | 57,268 (+36) | DIFF |
| A + `return m_bCinematicBreak != 0` | 55,504 | 23,731 | 57,360 (+128) | DIFF |
| A + `-0.8f * (float) delta` (operand order) | 55,497 | 20,942 | 57,260 (+28) | DIFF |
| A + `TargetFollowCamera *tailcam` alias | 55,473 | 20,945 | 57,268 (+36) | DIFF |
| A + `m_nState < 9` / `(unsigned)(m_nState-5) <= 3` / `!((unsigned)(m_nState-5) > 3)` / cached local / `float f` / inverted `if` | 55,473 | 20,945 | 57,268 (+36) | DIFF |
| A + case-12 store-before-return | 55,481 | 20,944 | 57,268 (+36) | DIFF |
| **goto (label after case 11)** | **54,927** | 20,711 | **57,232 (exact)** | DIFF |
| goto + `expect(cond,0)` | 55,380 | 20,191 | 57,232 (exact) | DIFF |
| goto + `expect(cond,1)` | 55,261 | 20,308 | 57,224 (-8) | DIFF |
| goto + case-12 store-before-return | 54,757 | 20,714 | 57,240 (+8) | DIFF |
| goto label after case 8 (case 8 falls through) | 55,225 | 23,673 | 57,088 (-144) | DIFF |
| goto label after case 10 (case 10 falls through) | 55,495 | 23,425 | 57,136 (-96) | DIFF |
| tail moved to the end of the function (before the final `return`) | 55,647 | 23,429 | 57,468 (+236) | **identical** |
| A + `return` + both PLT anchors removed | 55,463 | 23,716 | 57,320 (+88) | DIFF |
| A + `return` + one anchor removed | 55,439 | 23,726 | 57,344 (+112) | DIFF |
| swap mission-78 and mission-89 branches in source | 55,494 | 22,952 | 57,348 (+116) | DIFF |

### Why every live tail is worse

Adding a live tail is a **+36 B** code-size change in the only shape the
compiler produces (`A`).  The tail's `translate(x,0,0)` suffix is tail-merged
into the mission-1 `translate(-delta*0.1,0,0)` block at `0x137e7e`
(`vmov r1,s0 / ldr r0,[fp,#20] / movs r2,#0 / movs r3,#0 / blx translate`), so
ours is 36 B where the original's self-contained tail is 54 B.  The +36 B shift
re-colours every block after the insertion point, so raw rises by ~1,760 even
though the tail itself is only 36 B.  The original avoids the merge because its
mission-1 translate block schedules `movs r2,#0; movs r3,#0` *before*
`vcvt.f64`, leaving only a 2-instruction shared suffix (`vmov r1,s0; blx`),
below the tail-merge threshold.

The `goto` shape is the only **size-exact** live tail (57,232), because the
`goto` label placement happens to make the compiler drop 36 B elsewhere, but it
still loses raw (54,927 vs 53,711) and rotates `.rel.plt`.

## 4. `.rel.plt` rotation of the size-exact `goto` shape

`.rel.plt` keeps the same 3,675-entry **set**; only the first-reference order
moves (positions are 0-based within `.rel.plt`):

```
- pos 2806  RadioMessage::isTriggered            -> pos 2812
- pos 2826  StarSystem::scaleSunDuringSupernovaIntro -> pos 2813
- pos 2827  StarSystem::updateSupernova          -> pos 2814
- pos 2832  StarSystem::switchPlanetForIntro     -> pos 2815
- pos 2833  Level::switchSkyboxForIntro          -> pos 2816
- pos 2834  ParticleSystemManager::reset         -> pos 2817
- pos 2835  PlayerEgo::startSmokeEmission        -> pos 2818
```

All six are mission-0 state-9/10/11 and mission-89 calls: the live tail moves
the mission-0 state-9 block (`switchPlanetForIntro` / `switchSkyboxForIntro` /
`ParticleSystemManager::reset`, source lines 824-840) *before* the mission-89
block (`scaleSunDuringSupernovaIntro` / `updateSupernova`, source lines 1330-1331)
and pushes the first `RadioMessage::isTriggered` later.  Restoring the order
requires the mission-0 state-9 block to be placed after mission-89 again, which
is MachineBlockPlacement output, not source order (see w11806 §3: the physical
block order is not source-controllable).

The tail-at-end-of-function shape keeps `.rel.plt` identical (the mission-0
state-9 block does not move), which is why it is the only `.rel.plt`-safe live
tail — but it is +236 B.

## 5. Inlined-vs-called split — re-confirmed, nothing new to land

* `LevelScript::resetCamera` is a PLT call **51x** on both sides — never
  inlined (verified by `grep -c blx ...LevelScript11resetCamera`).
* `ArrayAdd<Player*>` is a PLT call **2x** on both sides.
* The 245-name PLT callee **set** is identical.  Direct-call **counts** differ
  by 5 net: ours has +1 each of `AEGeometry::setPosition(Vector const&)`,
  `Level::getStarSystem`, `Level::getPlayer`, `PlayerEgo::getPosition`,
  `Status::getStanding`, `Standing::setStanding`, `PlayerEgo::setAutoTurret`,
  `PlayerEgo::autoTurretIsEnabled`, and -1 `LODManager::forceUpdate`.
* The `getStanding`/`setStanding` and `setAutoTurret`/`autoTurretIsEnabled`
  pairs are the two artificial PLT-order anchors (source lines 1831 and 2037).
  Removing both is -24 B and rotates `.rel.plt` by 18 B / 26 B respectively.
  Removing both *and* adding the missing mission-78 `return` is exactly
  size-neutral (57,232) but still rotates `.rel.plt` (see §2).
* The net -1 `LODManager::forceUpdate` is inside the mission-106 state-3 block
  (original `0x13bfae`, `cmp r1,#1 / bne 144e36 / ... forceUpdate`); ours
  places that block elsewhere and the call is absorbed into a reordered run.
  It is a block-placement artefact, not a missing source call.

## 6. Conclusion

The mission-78 tail is genuinely dead in the base and genuinely live in the
original, and the base is also missing the mission-78 branch's `return`.
Fixing either is size-changing (+36 for the tail, +40 for the return, +128 for
both) and every fix rotates `.rel.plt`.  The only size-exact live tail (`goto`)
is raw 54,927 (worse than base 53,711) and `.rel.plt` DIFF.  The only
`.rel.plt`-safe live tail (end of function) is +236 B.  No shape satisfies
{size-exact, `.rel.plt`-identical, raw < 53,711} simultaneously, so the base
(dead-tail approximation) is kept.

The remaining lever is the same one identified in w11806: get
`&Globals::status` into `r4` and the canary into `fp` *without* changing the
frame size, so the prologue loses its extra `mov r0,r4` and the whole post-tail
stream shifts onto the original's addresses.  Every shape measured so far that
reaches `&status` -> `r4` either changes the size or re-colours the prologue.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53711 aligned=18228 size= 57232/57232 insn= 20728/20830
readelf -rW cmake-build-match/libgof2hdaa.so | awk '/\.rel\.plt/{f=1;next} f&&/^$/{exit} f{print $1, $NF}'
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch harnesses (all under `/home/debian/GalaxyOnFire2/scratch/w11905/`):
`LevelScript.base.cpp` (pristine base), `rebuild.sh`, `batch.sh`, `asmvar.sh`,
`runvar.sh`, `relpltdiff.sh`, `gen3.py`, `callalign.py`, `icount.py`,
`diffruns.py`, `range.py`, and the `g_*.cpp` / `v_*.cpp` variant sources.
