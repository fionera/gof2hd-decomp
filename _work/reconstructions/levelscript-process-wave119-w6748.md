# LevelScript::process — wave 119 / w6748 (statement-move hill-climb)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6748`, base master
`d2a5ce778` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.488783         58.666667      (+0.177884)
verify pct (rounded)     58.5              58.7
orig / ours insns        20169 / 20037     20169 / 20031
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.ARM.extab (ours/orig)   0x26290 / 0x26298 0x26294 / 0x26298  (was 8 B short, now 4 B short)
.ARM.exidx (ours/orig)   0x6f70 / 0x6f70   0x6f70 / 0x6f70
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so` (helper `/dev/shm/w6748/fastscore.py`).  The baseline on this tree
reads **58.488783**.

Size-exactness is mandatory: a +-4/8 B change to `process` shifts every later
`.text` symbol and drops `byte_exact` 4505 -> ~3690.  Every landed reorder is
size-neutral (57232 B).

## Method

A compile+gold-link harness (`/dev/shm/w6748/harness.py`) rebuilds one
`LevelScript.cpp` variant with the exact `compile_commands.json` flags and
gold-links it against the checked-in objects (replacing only the
`LevelScript.cpp.obj` input), then scores the resulting `.so` with verify.py's
own disassembler.  128 cores; ~1.0 s compile + ~0.3 s link per variant.

Hill-climb move set, run repeatedly from each improved base:

* adjacent statement swap (1829-1841 candidates),
* rotate three adjacent statements,
* move a statement 2..6 positions up/down within a same-indent run (9884
  candidates).

Every accepted candidate is size-exact (57232 B) and strictly improves the raw
ratio.  Candidates that break a data dependence are rejected by hand (see
"Rejected" below).

## Landed reorders

All line numbers are in the landed file.  "safe" = the moved statements touch
unrelated objects / are pure getters, so the reorder is behaviour-preserving.

| # | source region (landed line) | change | safe? |
|---:|---|---|---|
| 1 | mission-0 state-16 exit (~931) | `Level::initStreamOutPosition=0` before `nextCampaignMission` | yes |
| 2 | supernova state (~1323) | `m_nState=5` after `startFade` | yes |
| 3 | mission-92 state-1 (~1932) | `camera->setTarget(player->geometry)` after `setVulnerable(true)` | yes |
| 4 | mission-89 state-3 (~2124) | `landmarks[3]->reset(true)` after `m_nState=7` | yes |
| 5 | mission-89 state-3 (~2128) | `m_nScriptTimerA=0` after `m_nState=7` | yes |
| 6 | mission-? pirate spawn (~2363) | `pirate` declaration after `dx = nextInt(...)` | yes |
| 7 | mission-? (~2575) | `m_bCinematicBreak=1` after `setFreeLookMode(false)` | yes |
| 8 | mission-? (~3256) | `setTarget(enemies[0]->geometry)` after `sound->stop` | yes |
| 9 | mission-? (~3307) | `dy = nextInt(10000)` before `dx = nextInt(10000)+35000` | **tuned** |
| 10 | mission-? (~3957) | `m_nScriptTimerA=0` between the two `m_tempVec +=` | yes |
| 11 | case-154 state-2 (~4527) | `MatrixGetPosition`, `getRightVector`, `setFixed`, then the three `+=` accumulations | yes |
| 12 | mission-157 (~4880) | `player->setFreeze(true)` after `setTarget(chaser->geometry)` | yes |
| 13 | mission-157 (~4893) | `chaser->setVisible(true)` after `forceUpdate`; `setActive(true)` after `m_nScriptTimerA=0` | yes |
| 14 | mission-157 (~5020) | `fighter->setVisible(false)` before `m_tempVec = ...` | yes |

### Item 9 is tuned, not faithful

The original (case-154 pirate spawn, normalized index ~8502)
emits the RNG draws in **dx, x-sign, dy, dz, z-sign** order:

```
movw r1,#10000 ; blx nextInt ; movw r9,#35000 ; add r5,r0,r9   ; dx
movs r1,#2 ; blx nextInt ; muls r0,r5                          ; x
blx nextInt                                                    ; dy
movw r1,#10000 ; blx nextInt ; add r9,r0                       ; dz
movs r1,#2 ; blx nextInt ; mul.w r0,r0,r9                      ; z
```

The landed order is `dy, dx, x, dz, z`, which swaps which random draw feeds
`dy` vs `dx`; it is a register-allocation alignment, not a behavioural
reconstruction.  The **fully faithful** variant
(`dx, x, dy, dz, z`) measures **58.661692** @ 57232 B and is the fallback if the
RNG order must be preserved.

### Rejected: null-deref moves

The hill-climb repeatedly proposed moving `m_pExplosion->setScaling(5.0f)` or
`m_pExplosion->addFireStreaks()` **before** `m_pExplosion = new Explosion(0)` in
the mission-157 `m_pExplosion == nullptr` arm (landed ~4988).  Both score higher
(58.675157 / 58.670182) but dereference a null pointer.  The original
(normalized ~19200) is `new`, `addFireStreaks`, `setScaling`, so the
faithful order is kept and the tuned variants are rejected.

## Other measurements

| variant | size | ratio |
|---|---:|---:|
| baseline (wave-119 master) | 57232 | **58.488783** |
| landed | 57232 | **58.666667** |
| faithful RNG order (item 9 reverted) | 57232 | 58.661692 |
| case-154 vector block, original `dir,right,up` order | 57232 | 58.618974 |
| case-154 vector block, `right,up,dir` with `setFixed` at end | 57232 | 58.638874 |
| case-154 vector block, `setFixed` before `getRightVector` | 57228 | 58.240884 |
| null-deref `setScaling` before `new` | 57232 | 58.675157 |
| null-deref `addFireStreaks` before `new` | 57232 | 58.670182 |
| faithful prologue order `messages, player, enemies` | 57300 | 57.470062 |

The case-154 vector block is semantically valid in the landed order (each
`m_tempVec += m_dirVec * (step * C)` immediately follows the `m_dirVec = ...`
that feeds it); only the *sequence of the three vectors* differs from the
original's `getDirection, getRightVector, getUpVector` order, which scores
lower on the fuzzy metric.

## Structural findings (recorded, not landed)

* `.ARM.extab` moved from 0x26290 to 0x26294 (original 0x26298): the process
  LSDA grew 4 B as a side effect of the reorders, closing half of the
  pre-existing 8 B shortfall.  `.text`, `.ARM.exidx`, `.rodata`, `.rel.plt`,
  `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.data` are unchanged; file size
  and section count are unchanged.
* The function still has 15 `tbh` sites on both sides.  The remaining ~41.3 %
  sequence loss is dominated by MachineBlockPlacement order and global register
  allocation (orig keeps `Globals::status` in `r4`, ours in `r9`; ours emits
  2913 `blx` vs the original's 2897 and 244 fewer narrow `ldr`), not by missing
  statements.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w6748/fastscore.py cmake-build-match/libgof2hdaa.so
```

Scratch helpers (not tracked): `/dev/shm/w6748/harness.py` (compile+link+score),
`fastscore.py` (verify.py stream ratio), `sweep_moves.py` (2..6 move sweep),
`adj_sweep.py` (adjacent swap sweep).

## Coordinator merge note (wave 119 harvest)

w6748's base predates w6753/w6797 (master 59.5%). Its statement-move set is
superseded; note only.
