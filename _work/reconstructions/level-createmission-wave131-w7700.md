# w7700 (wave-131): `Level::createMission` — call-site / loop-local reuse + spill-slot/type sweep (no source win)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7700`, based on master `7c6933c2d`
(the wave-130b extab note).  Continues
`level-createmission-wave130b-w7677.md`, `…-wave130-w7677.md`,
`…-wave128-w7653.md`, `…-wave127-w7632.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; Ghidra `0xbffd8`, linked `.text` `0xaffd8`, 17172 B).
Build/flags unchanged (NDK r18b clang 7.0.2, `-Oz`).  `src/game/world/Level.cpp`
is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.376238 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
.text diff_bytes 85379  (whole section)
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`; one matching instruction
is `+0.0165 pp`.  As in the wave-130b note, the metric the ratchet actually
locks in for this (non-`byte_exact`) function is the **`.text` differing-byte
count** (85379), not the fuzzy ratio.

## New evidence this wave

### 1. The alien-turret select is a `select` *instruction* at IR level

`Level.ll` (emitted from the checked-in source) shows, at the alien-turret
loop (`%1733` = `i`, `%1734` = `k`):

```llvm
%1753 = ashr i32 %1733, 31
%1754 = icmp eq i32 %1112, %1753          ; bigAlienWave == (i < 0)
%1755 = select i1 %1754,
          [6 x float]* getelementptr @_ZL20kAlienOutpostTurrets,
          [6 x float]* getelementptr @_ZL20kAlienStationTurrets
```

So the `(i < 0)` anchor makes the condition loop-variant, which keeps the
`select` in-loop, but the select is still an IR `select`; `LowerSELECT_CC`
lowers it to `cmp.w r1, r9, asr #31; it ne; movne`.  The original's
`0xb32c6` is a real `cbz` **branch** (`ldr r0,[sp,#64]; cbz; ldr;add;b;ldr;add`),
so the original's IR must contain a `br` + `phi`, not a `select` (consistent
with wave-112/113: `LowerSELECT_CC` always returns `getCMOV` for i32 and `-Oz`
disables `CodeGenOptimize::optimizeSelectInst`).  Every source spelling that
avoids the anchor lets `SimplifyCFG`/`InstCombine` fold the diamond to a
`select` and then `LICM` hoists it out of the loop (function shrinks to 17140).

### 2. `Globals::status` is `fp`, `raid` is a 4-byte stack slot

`0xafff0 ldr.w fp,[r0]` loads `Globals::status`; `0xb0024 str.w fp,[sp,#88]`
(orig) / `[sp,#84]` (ours) spills it.  `raid` is stored 4-byte
(`0xb04e8 str r0,[sp,#84]` orig / `[sp,#88]` ours), i.e. clang promotes the
`bool` to a 4-byte slot.  The swap is a `RegAllocGreedy` slot-colouring
tie-break, not a source-order property.

## Variants measured this session (fresh `link_so` + verifier normalisation)

Baseline row is **92.376238 % / 17172 B / 6065 insns / `.text` 85379**.
A `.text` jump to ~9e5 means the function changed size (all following code
shifts); those rows are not size-exact.

### 2.1 `raid` declaration / assignment / type

| variant | % | `.text` |
|---|---:|---:|
| **baseline `bool raid;` + if/else before `raidCoords`** | **92.376238** | **85379** |
| `raid` block moved after `raidCoords` (decl + assign) | 75.903116 | 924344 (17164 B) |
| `bool raid;` kept, assignment moved after `raidCoords` | 75.903116 | 924344 (17164 B) |
| `bool raid = !(intro && !hard) && raidRoll < (…)` | 92.376238 | 85379 |
| `bool raid = cond ? false : raidRoll < (…)` | 92.376238 | 85379 |
| `char` / `signed char` / `short` / `long` / `unsigned char` / `int8_t` | 92.211221 | 85606 |

`bool` is already optimal; the `raid`/`status` slot swap survives every
type/position change tried (also wave-130b: `int`, `uint8_t`, top-of-function
declaration).

### 2.2 `nameLeader` / `case4Else` / `case4Tail` skeleton

| variant | % | `.text` |
|---|---:|---:|
| **baseline `if (nameLeader) {…} else { return; }`** | **92.376238** | **85379** |
| `if (!nameLeader) return;` then the name store | 92.376238 | 85379 |
| `if (nameLeader) {…} else return;` (no braces) | 92.376238 | 85379 |
| `return;` moved inside the `case4Tail` block | 92.376238 | 85379 |
| extra `{}` before the `else` removed | 92.376238 | 85379 |

Removing the *outer* scope brace after `if (mission->isEmpty()) {` does not
compile: the `goto case4Tail` from case 4 jumps over `coords`/`localRace`/
`raidCoords` initialisations ("jump bypasses variable initialization").  The
brace is load-bearing and must stay.

### 2.3 Alien-turret table select (the 32 B residual)

| variant | % | `.text` |
|---|---:|---:|
| **baseline `(bigAlienWave + (i < 0)) ? station : outpost`** | **92.376238** | **85379** |
| `!bigAlienWave ? outpost : station` (anchor inverted) | 92.376238 | 85379 |
| plain ternary / `if`/`else` / `switch` (0/default, 1/default, one-case) | 88.463127 | 901022 (17140 B) |
| `t = outpost; if (bigAlienWave == 1 / != 0) t = station;` | 88.463127 | 901022 (17140 B) |
| `switch (bigAlienWave) { default: t = station; }` | 88.485049 | 905460 (17140 B) |
| `*(volatile int *)&bigAlienWave ? …` | 87.914809 | 907376 |
| `volatile int bigAlienWave` (plain ternary) | 87.914809 | 907379 |
| `volatile int bigAlienWave` + anchor | 87.905556 | 907383 |
| `const float (*volatile t)[6]; if (…) …` | 69.082561 | 920027 (+87 insns) |
| `const float (*volatile t)[6] = cond ? …` | 87.044668 | 898333 (+25 insns) |
| `(void)&t;` after a plain if/else | 88.463127 | 901022 (17140 B) |

**No source spelling produced the original's in-loop `cbz` branch.**  The
`volatile` attempts either still hoisted (function shrank) or added
load/store traffic.  The anchor remains the only size-exact shape.

### 2.4 `RaidCoords3` / fortress-loop / `case4Tail` locals

| variant | % | `.text` |
|---|---:|---:|
| **baseline** | **92.376238** | **85379** |
| `RaidCoords3` → `int raidCoords[3]` | 92.376238 | 85379 |
| `RaidCoords3` field-by-field assignment | 92.376238 | 85379 |
| fortress `Vector vv = {t[3],t[4],t[5]};` then rotate | 90.856577 | 832949 (17164 B) |
| fortress `*(const Vector *)&t[3]` struct load | 88.600908 | 832950 (17164 B) |
| case4Tail `AERandom *&rnd = Globals::rnd;` | 92.376238 | 85379 |
| case4Tail `bool hostile = true;` passed to `createShip` | 92.376238 | 85379 |
| cache `Globals::status` in a local `Status *status` (whole function) | 60.832303 | 921920 (17180 B) |

The fortress `Vector`-local form is interesting: it changes the load shape
(`ldmia`-vs-`ldrd` cluster) and the function size (17164), but the overall
score drops to 90.86, so it is not a win.

## Conclusion

No source change in this pass lowered the `.text` differing-byte count or the
function count.  The checked-in baseline is kept.  The residual is unchanged
and backend-only:

1. alien-turret table select: original in-loop `cbz` **branch** vs our in-loop
   if-converted `select` (`cmp/it/movne`); the anchor is the only size-exact
   spelling, and no `if`/`switch`/`volatile` spelling yields a `br` in the IR
   (the diamond is always folded to a `select`, then LICM-hoisted without the
   anchor);
2. `case4Tail`: original `fp = 1` (the `true` argument) + `&Globals::rnd`
   reloaded from `sp+120`; ours keeps `&rnd` in `fp` and rematerialises `1`;
3. `raid`/`Globals::status` spill-slot swap `sp+84`/`sp+88` (17 sites) —
   invariant under every type/position change tried;
4. `stationPirates`/`extraAliens`/`fortressGuns` slot permutation
   (`sp+48/44/40` vs `sp+44/40/48`); the `ArraySetLength` addition order is
   already the original's (`pirateEscorts + stationPirates + blueprint +
   extraAliens + pirates + convoy + fortressGuns + alienGuns + friendCount +
   field_164 + field_168 + hostileCount`, verified at `0xb2a50`–`0xb2a8c`);
5. case-183 test/body order (2,1,0) — proven backend-only in wave-131/w7677;
6. branchless `&&` at `+0x2e04`/`+0x2f52`;
7. pirate-fortress `ldmia`-vs-`ldrd` SLP pair.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7700/score2.py        # 92.376238, 6055/6065
python3 /dev/shm/w7700/textdiff.py      # 85379
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target lint -j6    # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch: `/dev/shm/w7700/{runv.py,score2.py,textdiff.py,diff2.py,Level.base.cpp,
Level.ll,gh_decomp.c,orig.asm,ours.asm}`.

## Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
```
