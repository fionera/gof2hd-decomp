# Radar::draw wave 191b (w13510) — fourth lens: station-blip / 24000 / tractor cells, no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13510`, branch `wave191b-w13510`,
base `master` `fc4e63303`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191-w13487.md` (base 6,055), `radar-draw-wave191b-w13489.md`
(station-blip pointer spelling), `radar-draw-wave189-w13319.md` (the `campaign`-in-`sl`
register map) and `radar-draw-wave125-w7512.md` (planet-loop encoding sweep).  Every number
below was measured in this worktree on the 6,055 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,055** |
| instruction-index alignment | 1,950 |
| instruction count | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. The three assigned lenses — what is already byte-identical

The byte-level equal-run map (`byte_runs.py`, maximal equal-byte runs) shows that the
**station-blip block is already byte-identical**: runs `12f8af` len 111 and `12f91f` len 383
cover the whole station blip (`12f8b8`–`12f95c`) and the asteroid prologue.  The w13489
`sf,dr,rp` / `A,B,A` spelling is confirmed optimal; no station-blip cell can help.

The **tractor 24000 chain** (`Radar.cpp:893`, `egoPos - targetPos`, lower-first) is also
byte-identical: `12f0f0`–`12f142` matches the original instruction for instruction, including
the `s18`/`s16` constants (`vcmpe s0,s18; bmi` then `vcmpe s0,s16; bgt`).  Do not touch it.

The **enemy 24000 chain** (`Radar.cpp:714`) is the one that differs.  The original's literal
pool at `12ec0c` is `[+24000, 60000, -24000, 100]` and it emits `vcmpe s0,s18; bgt` then
`vcmpe s0,s16; bmi`, i.e. the original source is **upper-first** (`> 24000` before `< -24000`),
the pre-w13487 spelling.  Reverting line 714 to upper-first restores the pool order and the
branch order but is **raw-neutral** (6,055) because clang then assigns `+24000` to `s16` and
`-24000` to `s18` (the original has `+24000` in `s18`), so each `vcmpe` still differs by one
byte.  The register tie-break, not the source order, is the residual.

## 2. Cells measured this pass (all size-exact 9,908 unless noted; base raw 6,055)

### 2.1 24000 chains

| cell | raw | aligned | note |
|---|---:|---:|---|
| enemy chain upper-first (`> 24000` first) | 6,055 | 1,963 | pool order matches, `s16`/`s18` swap keeps it neutral |
| gas chain already `x,z,y` upper-first | 6,055 | 1,950 | base is optimal (w13487 cell 1b) |
| tractor chain (line 893) | 6,055 | 1,950 | already byte-identical |

### 2.2 `otherLock` data flow

| cell | raw | note |
|---|---:|---|
| line 954 (`lockedStation \|\| (dockNavPtr && !lockedEnemy) \|\| field_0x1c \|\| field_0x14`) | 6,055 | base `ABCD` optimal |
| line 954 reordered to the original's `B,C,A,D` (dockNavPtr, field_0x1c, lockedStation, field_0x14) | 6,064 | instruction order matches, register allocation re-rolls |
| all 24 permutations of line 954 | 6,055–8,456 | base best |
| all 24 permutations of line 533 (`lockedAsteroid \|\| lockedGasCloud \|\| field_0x1c \|\| lockedStation`) | 6,055–6,059 | base best |
| all 24 permutations of line 1087 (`(hasDockNav && !lockedEnemy) \|\| field_0x1c \|\| lockedStation \|\| field_0x14`) | 6,055–7,671 | base best |
| tractor `tractorMode2 && wreck && isDead && 1c==nullptr` | 6,055 | base optimal (w13487) |

The original's line-954 order really is `B,C,A,D` (loads `[r4,#8]` dockNavPtr, `[sp,#128]`→
lockedEnemy, `[r4,#28]` field_0x1c, `[r4,#36]` lockedStation; the `field_0x14` fall-through at
`12f8a6` is reached only when all four are false), but reordering the source to match is a raw
loss because it re-colours the whole gas-loop prologue.

### 2.3 Gas `inSight` box — the original uses the `+` form

`Radar.cpp:969` is written `left > screenX - size*2` / `top > screenY - size*2`.  The original
computes `screenX < left + size*2` / `screenY < top + size*2`:

```
12f4da cmp r1,r2            ; screenX vs left
12f4de add.w r2,r2,r0,lsl#1 ; left + 2*size
12f4e2 cmp r1,r2
12f4f2 lsls r0,r0,#1
12f4f4 add r0,r1            ; top + 2*size
12f4f6 cmp r2,r0
```

Rewriting the source to the `+` form reproduces the original instruction *structure*
(aligned drops to 1,946 for the `float top` spelling, **1,925** for the `int top` spelling —
the best aligned value found for this function) but is a **raw loss**:

| cell | raw | aligned | note |
|---|---:|---:|---|
| base (`-` form) | **6,055** | 1,950 | |
| `screenX < left + size*2` / `screenY < (int)top + size*2` (`float top`) | 6,059 | 1,946 | structure matches; register/slot bytes cost more |
| same with `int top` | 6,071 | **1,925** | best aligned, worst raw |
| `x` half only | 6,056 | 1,946 | |
| `y` half only | 6,058 | 1,950 | |
| `+` form + enemy-blip `dr,rp,sf`/011 | 6,059 | 1,940 | |
| `+` form + planet-reorder | 6,060 | 1,939 | |

The `+` form is the *correct* original source shape but cannot be landed because it costs raw
bytes (the gas-loop register allocation re-rolls).  It is recorded here so a future pass with a
better allocation can pick it up.

### 2.4 Other single-knob cells (all neutral or worse)

- prologue zero spelling (`bool z=false`, `int z=0`, `unsigned char z=0`, `=false`, `=0u`,
  `=0x0`, chained `a=b=0`, swapped store order): all 6,055 except chained/swap 6,057.
- `campaign`: `const`, `unsigned`, `short`, `long`, `int32_t`, separate decl+assign, declared
  with the bools, declared before `missionBlocksPlanets`, `>= 2` → `> 1`/`2 <=`/`!(<2)`/`-2>=0`:
  all 6,055; `campaign` used in the landmark guard: 9,551 (size 9,904, fatal).
- `missionBlocksPlanets`: `short`/`char`/`unsigned char`/`uint8_t` all 6,055; `bool` 9,912.
- loop index type (`uint32_t`→`int`) on the planet / enemy / gas loops: all 6,055.
- `stations` `const` / `auto` / `(*stations)[i]` / declared before `planetTargets` guard: all
  6,055; declared *before* the `campaign>=2` guard: 9,082 (size 9,904, fatal).
- planet `currentStation`/`stationId` reorder (original order is onScreen→stationId→
  currentStation): 6,056 / aligned 1,943.
- gas `cloud->isDead() || cloud->isDying()` swap: 6,056.
- enemy-lock guard `enemyLockTimer > ((navWreck||field_0x24)?500:0) && dockNavPtr`: local
  threshold / no-paren spelling 6,055; remove expect 6,680; expect 0 8,541; reversed 6,585.
- enemy-blip `frames` computed from `int rawFrames` or `this->blipSprite`: 6,055; frames moved
  after `elapsed`: 6,646.
- `__builtin_expect` flips on `enabled`, `onScreen`, `enemyFlagsLo`, `cloud==locked`,
  `dockNavPtr!=current`: 6,089–9,513 (all worse).
- enemy-blip `dr,rp,sf`/011 reorder: 6,055 / aligned **1,944** (best raw-tie aligned).
- 20 random 3–5-cell combinations of the neutral cells: all 6,055.
- gas `const auto camZ` removed (use `this->cameraZ` inline): 6,174 — the original reloads
  `cameraZ` into `s6` at the `calcDistance` call (`12f6e6`) while ours hoists it into `s16`;
  removing the local does not reproduce the reload and re-rolls the gas block.
- enemy-blip `dr,rp,sf`/011 (`D`) measured against the whole `.so`: `.text` diff 71,478
  (unchanged), `.ARM.extab` 577 (unchanged), `.rel.plt` 0 — a pure aligned win
  (1,950→1,944, insn 3,541→3,542), no raw/extab movement.  Not landed (raw-neutral).

## 3. Root cause of the residual (confirmed)

The first divergence is `12e07a` `mov.w fp,#0` (orig) vs `mov.w sl,#0` (ours) — the shared
zero constant.  The original's zero lives in **`fp`**, which leaves **`sl`** free for
`campaign` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later for `stations`
(`12e3c6 mov sl,r0`, cold copy `12e3ce str.w sl,[sp,#116]`).  Ours puts the zero in `sl`, so
`campaign` spills to `[sp,#144]` and `stations` spills to `[sp,#132]`; the planet-loop prologue
is 4 B shorter (`12e3c6`–`12e3d2`), which shifts the entire planet/enemy/gas tail by 2–248 B
and is the bulk of the 6,055 raw diff.

The zero/`campaign`/`missionBlocksPlanets` colouring is a global allocator tie-break: the
callee-saved pair (`sl`,`fp`) is handed to the two long-lived prologue values
(`missionBlocksPlanets`, `campaign`).  The original gives `missionBlocksPlanets`→`fp` and
`campaign`→`sl`; ours gives `missionBlocksPlanets`→`sl` and spills `campaign`.  No source
spelling tried here (types, declaration order, separate assignment, `const`) changes it.  The
w13319 `bsor` body (`bool otherLock` + `int marked` + `bossSeen||`) is the only known body that
does put `campaign` in `sl`, but it is 771 B raw-worse.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6055 aligned= 1950 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13510/`: `sweep.py` (1 s compile+link+fn_diff
harness), `Radar_base.cpp` (6,055 base), `delta.py` (equal-run address map), `localdiff.py`
(per-region instruction diff), `blocks.py` / `byte_runs.py` (same-address byte-diff map),
`gen_*.py` + `*_variants.json` (all cell generators), `orig_full.txt` / `ours_full.txt`.
