# Radar::draw wave 191b (w13507) — no size-exact raw win; base 6,055 re-confirmed as a
# strong local optimum

**Status:** analysis complete, 2026-11-12. **No `src/` change committed** (worktree left
clean). Worktree `wt/w13507`, branch `wave191b-w13507`, base `master` `fc4e63303`
(HEAD `6159e4ee7`). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191-w13487.md` (the 6,055 base), `radar-draw-wave191b-w13489.md`
(station-blip pointer spelling), `radar-draw-wave190-w13474.md` (statement/regalloc
sweeps), `radar-draw-wave190-w13456*.md` (asteroid timer swaps) and
`radar-draw-wave189-w13319.md` (register map / `campaign`-in-`sl`).

## 0. Baseline (verified in this worktree)

| metric | value |
|---|---:|
| raw same-address diff | **6,055** |
| instruction-index alignment | 1,950 |
| function size | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 |
| frame | `sub sp,#296` (`0x128`) |
| `mode` argument slot | `sp+0x68` (original `sp+0x64`) |
| `campaign` | spilled `sp+0x90` at definition (original: `sl`) |
| shared zero | `sl` (original: `fp`) |
| planet-loop counter | `sl` (original: `fp`) |
| planet-loop `stations` | spilled `sp+0x84` (original: `sl`) |
| `marked` | spilled `sp+0x90` (original `sp+0x98`) |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base.

## 1. Residual shape (why 6,055 is hard to move)

The function is byte-identical in `12e050..12e3cc` except for the `campaign`/zero-register
colouring, in `12f710..12fc50` (gas/asteroid lock region) except for a handful of stack
slot immediates, and in `12fd90..12fe13`. The bulk of the 6,055 raw bytes is the
**shift cascade**: the extra `ldr r0,[sp,#144]`/`cmp r0,#2` pair at `12e38a` (campaign
reload) plus the `stations` spill re-lays out `12e3d0..12f6d0` (~5 KB) at a
non-constant offset (shift profile: `-4` at `12e3d0`, `-52` at `12e4d0`, `-8` at
`12eb50`, `-56` at `12ec10`, `0` from `12ef50`).  Fixing the `campaign`-in-`sl`
allocation is therefore the only large lever; every source construct tried so far
(see §3) either changes the function size or introduces a worse early spill.

The original keeps `campaign` in `sl` from `12e0f8` to the spill at `12e7ba` (the
`enemies:` precompute, reached only from the landmark path); `sl` is then reused for the
planet-loop `stations`/counter.  Ours spills `campaign` at definition (`t2STRi12 $r0,
%stack.45` in the Greedy-RA MIR) and reloads it at `12e38c`.

## 2. New cells measured this pass (all size-exact 9,908 unless noted; base 6,055)

| cell | raw | notes |
|---|---:|---|
| `campaign` used in the landmark guard (`campaign > 93` replacing the re-call) | 9,551 / **9,904** | size-fatal |
| `(campaign > 93 \|\| getCurrentCampaignMission() > 93)` at the landmark guard | 8,573 / **9,916** | size-fatal |
| `(campaign > 93 && getCurrentCampaignMission() > 93)` | 9,115 / **9,944** | size-fatal |
| remove the `campaign` local, re-call at the planet guard | 9,244 / **9,904** | size-fatal |
| planet-loop `currentStation` moved back **after** the `onScreen` guard (original order) | 6,056 | +1; structure now matches original |
| planet box restored to x-pair-first (`screenX > left && screenX < left+size && …`) | 6,055 | neutral |
| asteroid timer *semantic* fix: `= 0` in the `best` block + `+= mode` in the else, no `lockedAsteroid = nullptr` | 6,173 | +118 (matches the original `12fd82..12fd8c` but re-shifts the tail) |
| same with `lockedAsteroid = nullptr` kept in the else | 6,913 / **9,912** | size-fatal |
| music tail mask-before-enemy (`range>15 \|\| mask==0 \|\| !(current==151 && seen)`) | 6,079 | +24 |
| music tail same with `expect` on the enemy term | 6,222 / **9,912** | size-fatal |
| wreck x/y block: 4-arm form (merge `11e&&11f` into `11f`) | 6,055 | neutral |
| wreck x/y block: `int x = screenX; int y = screenY;` at top, arms adjust | 7,642 / **9,948** | size-fatal |
| wreck x/y block: `int x = screenX;` top only | 6,173 | |
| wreck x/y block: duplicate y per arm (ternary) | 6,911 / **9,912** | size-fatal |
| wreck x/y block: `11e` arm first / `11f` arm first / `11f`-first + ternary y | 6,912 / **9,912**, 6,922 / **9,896**, 6,916 / **9,904** | size-fatal |
| wreck x/y block: `x = 0; y = 0;` init | 6,055 | zero init folded away |
| wreck x/y block: `Globals::Canvas->DrawImage2D` instead of `canvas->` | 7,563 / **9,896** | size-fatal |
| wreck x/y block: inline `this->image_0xac` (drop the `image` local) | 6,924 / **9,912** | size-fatal |
| `image`/`canvas` declaration swap | 6,061 | |
| asteroid `slotCount`/`asteroidHit` declaration swap | 6,057 | |
| asteroid `best`/`bestDistance` declaration swap | 6,055 | neutral |
| `int asteroidHit` | 7,579 / **9,924** | size-fatal |
| station blip `field_0x28 != 0 && … != lockedStation` swap | 6,055 | neutral |
| planet blip `field_0x18 != 0 && … != field_0x14` swap | 6,055 | neutral |
| gas blip `candidate == nullptr \|\| candidate == lockedGas` swap | 7,754 / **9,912** | size-fatal |
| `missionBlocksPlanets` as `bool` | 6,214 / **9,912** | size-fatal |
| `missionBlocksPlanets = 0` (spelling) | 6,055 | neutral |
| `turretMode` as `int` | 9,221 / **9,860** | size-fatal |
| `alienOrbit` as `bool` | 6,064 | |

## 3. Exhaustive sweeps re-run on the 6,055 base (all reverted)

* **All 200 `&&`/`||` operand swaps** in the body: best neutral 6,055.  (Re-confirms
  w13484's "every `&&`/`||` swap" on the newer base; the w13484 `isDead`/`isDying`
  alignment artifact is already in the base.)
* **All 142 `==`/`!=`/`<=`/`>=` operand swaps**: best neutral 6,055.
* **All 29 single `__builtin_expect` weight flips**: best neutral 6,055 (sites on
  lines 655, 787, 826); everything else 6,069–9,558 or size-fatal.
* **All 1,485 same-brace-depth simple-statement swaps at distance ≤ 40 lines**
  (includes the ≤15-line set that found w13456's `asteroidLockTimer` swap): best
  neutral 6,055.  No swap beats the base.

## 4. Conclusion

The 6,055 base is a strong local optimum for every local/operand/statement lever tried
here.  The only remaining large win is the global `campaign`-in-`sl` colouring (with the
`stations`/`marked`/`mode` stack-slot knock-ons), which no source spelling tried so far
reproduces without a size change or a worse early spill.  `src/` is left at the base.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6055 aligned= 1950 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13507/`: `fast.py` (1 s compile+link
harness), `sbs.py`, `sweep_swap.py` (statement swaps), `sweep_andor.py` (`&&`/`||`),
`sweep_cmp.py` (comparison operands), `tryblk.py`, `c_*.cpp` cell sources.
