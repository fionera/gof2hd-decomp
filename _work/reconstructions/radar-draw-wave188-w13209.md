# Radar::draw wave 188 (w13209) — branch/landing-pad re-sweep on the 6,309 body: no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13209`, branch
`wave188-w13209`, base `master` `ce16f45f1` (wave-187 harvest 5). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave186-w13009.md` (the same branch/landing-pad pass on the
6,311 body), `radar-draw-planetlock-store-order-wave187-w13109.md` (the landed
6,311 -> 6,309 store-order/`||` cell), `radar-draw-wave187-w13110.md` (the
`campaign`/`otherLock` register-allocation lever) and
`radar-draw-wave187-w13111.md` (expect/alias re-sweep). This note is
self-contained: every number below was measured in this worktree on the 6,309
base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,309** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| `.text` differing bytes (whole `.so`) | 74,223 |
| `.ARM.extab` differing bytes | 639 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.ARM.exidx` / `.rel.plt` / `.rodata` / `.data` / `.bss` | 0 / 0 / 0 / 0 / 0 |
| `.note.gnu.build-id` differing bytes | 20 |
| total differing bytes | 74,882 |
| file size | 2,187,328 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this
pass.

## 1. What the 6,309 base looks like at the assigned sites

The wave-187 landing left the planet-lock body as `dock; store; sound` (DTS in
this note's notation; `D`=dock guard, `T`=`field_0x14 = field_0x18` store,
`S`=sound guard) with a plain dock guard and `expect(!lockConfirmed, 0)` on the
sound guard. The disassembly of the cold block is

```
ours 1303ce: ldrb r0,[r4,#424]  ; field_0x1a8
     1303d2: cbz  r0, 1303de
     1303d4: getPlayer; dockToPlanet
     1303de: ldrb r0,[r4,#536]  ; lockConfirmed
     1303e2: cmp  r0,#0
     1303e4: beq.w 1305d8      ; sound block, out of line
     1303e8: ldr.w r0,[r4,#416] ; planetLockTimer
     1303ec: ldr  r1,[r4,#24]   ; field_0x18
     1303ee: str  r1,[r4,#20]   ; field_0x14
```

whereas the original lays the same statements out as sound (inline), store,
dock:

```
orig 130454: ldrb r0,[r4,#536]
     130458: cbnz r0, 130470
     13045a: ... play(26)
     130470: ldrb r0,[r4,#424]
     130474: ldr  r1,[r4,#24]
     130476: cmp  r0,#0
     130478: str  r1,[r4,#20]
     13047a: beq.n 130486
     13047c: getPlayer; dockToPlanet
```

The shipGroup/wreck skip branch is structurally identical to the original
(`isActive`, `isDying`, `hasCargo`, `field_0x74`, then the rare
`shipGroup == 10 && isActive && !isDead && !isDying` guard); the only local
difference is `bossSeen` materialisation: the original emits
`cmp r0,#10; it eq; moveq r6,#1`, ours emits `subs r0,#10; clz r0,r0;
lsrs r5,r0,#5` (same 6 B). The `wreck` computation and the off-screen wreck
image chain are also instruction-for-instruction identical modulo register
choice.

## 2. Planet-lock block: 108-cell cross product

6 statement orders of `{D,T,S}` x dock guard `{plain, expect(0), expect(1)}` x
sound guard `{plain, expect(0), expect(1)}` x `{normal, inverted}` mission guard
x `||` timer, plus 60 timer-guard variants. Best size-exact cells:

| cell | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base `D T S`, plain dock, `expect(sound,0)`, `\|\|` | **6,309** | 1,858 | 3,541 | 9,908 |
| `D T S`, plain dock, `expect(sound,0)`, `\|\|` (same) | 6,309 | **1,852** | 3,541 | 9,908 |
| `D S T` (landed wave-187) | 6,309 | 1,858 | 3,541 | 9,908 |
| `T S D` | 6,310 | 1,853 | 3,541 | 9,908 |
| `S D T` | 6,311 | 1,854 | 3,541 | 9,908 |
| `S T D` (sound, store, dock = original layout) | 6,426 | 1,835 | 3,541 | 9,908 |
| `D S T` + plain sound | 6,405 | 1,839 | 3,539 | **9,904** |
| `D T S` + plain sound | 6,431 | 1,848 | 3,541 | 9,908 |
| `S T D` + plain sound | 6,426 | 1,835 | 3,541 | 9,908 |
| `D S T` + `\|` timer | 6,425 | 1,908 | 3,544 | 9,908 |
| `D T S` + `\|` timer | 6,424 | 1,904 | 3,544 | 9,908 |

`D T S` is raw-equal to the landed `D S T` but six fewer mismatched
instructions (aligned 1,852 vs 1,858); it is a stepping stone, not a raw win,
so it is not landed.

Mission-guard arm inversion (`if (!missionBlocksPlanets) { core } else { hud;
goto }`), `else`-only form, `if (missionBlocksPlanets) ... else ...`, and
moving the guard after the core are all byte-identical to the base except the
after-core form (6,959). Arm inversions of the dock guard and of the sound
guard are also byte-identical. 24 permutations of the four-term outer planet
guard: base `campaign>=2, planetTargets, !alienOrbit, !isDockingToPlanet` is
the unique best; the next size-exact permutation is 8,571.

Timer-guard spellings on the landed body, all size-exact: `||` (base),
`missionBlocksPlanets != 0 || planetLockTimer < 1` (6,557), `<= 0` (identical),
two independent `if`s in either order (identical / 6,557), `!(planetLockTimer
>= 1 && missionBlocksPlanets == 0)` (identical), bitwise `|` (6,424).

## 3. `field_0x1a8` dock guard / +8 B site

The +8 B is the pair of expects (dock +4, sound +4); the current base already
has plain dock + `expect(sound,0)`. Removing the sound expect gives 9,904
(raw 6,405). A full 60-cell sweep of every other expect removal/flip on top of
that plain-sound body found only six size-9908 combinations; the best is
`plain` + remove `expect(alwaysEnemySeen, 1)` at 6,384 (75 raw worse). The
sound-inline `D T S`/`S T D` shapes are size-exact at 9,908 but 6,426-6,431
raw. No compensation recovers the ~100 raw the plain-sound allocation loses.

## 4. `shipGroup` / wreck branches

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base | **6,309** | 1,858 | 3,541 | 9,908 |
| skip guard flattened to one `\|\|` chain | 6,309 | 1,858 | 3,541 | 9,908 |
| skip guard `!hasCargo && isDying()` first | 6,309 | 1,861 | 3,541 | 9,908 |
| skip guard De Morgan | 6,309 | 1,858 | 3,541 | 9,908 |
| skip guard reversed (`dying/field_0x74` first) | 6,313 | 1,873 | 3,541 | 9,908 |
| rare guard `isActive` first | 6,309 | 1,863 | 3,541 | 9,908 |
| rare guard `!isDying` before `!isDead` | 6,311 | 1,872 | 3,541 | 9,908 |
| rare guard split into nested `if` | 6,309 | 1,858 | 3,541 | 9,908 |
| `bossSeen = (bool)/(!!)/(!=0)/(x-10==0)/(10==x)` | 6,309 | 1,858 | 3,541 | 9,908 |
| `bossSeen = (x==10) ? true : false` | 6,309 | 1,858 | 3,541 | 9,908 |
| `bossSeen = (x==10) ? 1 : 0`, `&1`, `*1`, `\|0`, `^0`, `+0`, temp int/bool | 6,309 | 1,858 | 3,541 | 9,908 |
| `if (x==10) bossSeen = true; else bossSeen = false;` | 6,309 | 1,858 | 3,541 | 9,908 |
| `bossSeen = true;` | 8,539 | 1,940 | 3,539 | **9,900** |
| `bossSeen \|= (x==10)` | 9,208 | 2,166 | 3,544 | **9,976** |
| `if (x==10) bossSeen = true;` | 9,208 | 2,164 | 3,554 | **9,988** |
| `wreck = hasCargo && (isDying \|\| isDead)` | 6,309 | 1,858 | 3,541 | 9,908 |
| `wreck = hasCargo ? ... : false` | 6,309 | 1,858 | 3,541 | 9,908 |
| `wreck = (isDead \|\| isDying) && hasCargo` | 9,802 | 3,510 | 3,640 | **10,196** |
| `(isDying \|\| isDead) && !hasCargo` continue | 6,309 | 1,858 | 3,541 | 9,908 |
| `!hasCargo && (isDead \|\| isDying)` continue | 8,573 | 1,871 | 3,543 | **9,916** |
| steal guard De Morgan / flattened | 6,309 | 1,858 | 3,541 | 9,908 |
| steal guard `expect` first | 6,311 | 1,868 | 3,541 | 9,908 |
| `if (locked == enemy)` (3 sites) | 6,309 | 1,858-1,859 | 3,541 | 9,908 |
| `if (wreck == true)` / `!= false`, `if (onScreen != 0)`, `if (isDead() == false)` | 6,309 | 1,858 | 3,541 | 9,908 |

The `bossSeen` `clz` form is robust: every source spelling that keeps the
function size at 9,908 produces the same `subs/clz/lsrs` bytes. Only
`bossSeen = true` (9,900) and the `|=`/`if` forms (9,976/9,988) force a
different materialisation, and all are size-fatal.

## 5. Other cells re-confirmed neutral on 6,309

Nested-if forms of the five-term enemy-tail guard and of the outer
`isDockingToAsteroid && !(...)` guard are byte-identical. `field_0x14`
truthiness/`!= nullptr`, `field_0x18 != nullptr`, swapping the `field_0x18`
terms, `planetLockTimer > field_0x1b8` inversions (`field_0x1b8 < ...`,
`!(... <= ...)`, `>= field_0x1b8 + 1`) are all byte-identical. The
`!turretMode && !landmarkHit` swap and `!(turretMode || landmarkHit)` are
byte-identical. `wreck && hasTractor13` arm swap is size-fatal (9,904); the
five-term tail-guard term reorders are size-fatal (9,900-9,912).

## 6. Conclusion

The 6,309 body is a strong local optimum for every branch/landing-pad cell in
the brief. The dominant residual is still the `campaign` allocation cascade
(wave-187 w13110): the original keeps `campaign` in `sl` and `missionBlocksPlanets`
in `fp`; ours spills `campaign` to `sp+0x90` and uses `sl` for the shared zero.
That cascade is what makes the original's inline sound/store/dock cold block
and `it eq; moveq` `bossSeen` unreachable at the same function size. No
size-exact raw win exists in the assigned cells on this base.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13209/`: `Radar.base.cpp` (6,309
base), `Radar.dts.cpp` (the raw-equal / alignment-better `D T S` stepping
stone), `harness.py` (build+measure), `measure.sh`, `gen_pl.py`/`gen_pl2.py`
(the 108+60 planet-lock cells), `gen_batch.py`..`gen_batch4.py` (the
shipGroup/wreck/guard cells), `expect_sweep.py`, `plain_expect.py`-style
results in `*_results.json`, `aldiff.py`/`structdiff.py`/`sync.py`/
`diffruns.py` (stream alignment and per-region diff census), `seccmp.py`
(per-section diff census), `orig.dis`/`ours.dis`.
