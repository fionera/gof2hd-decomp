# w7632: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-127
# faithful-statement-order / call-site-argument / local-reuse sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7632`, based on current master.
Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; Ghidra address `0xbffd8`, linked `.text` address `0xaffd8`,
17172 B). Continues `level-createmission-wave121-w7112.md` …
`…-wave126-w7609.md`. `src/game/world/Level.cpp` is unchanged; this note is
the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

Ghidra was re-read from a copy of the project at `/dev/shm/w7632/gh`
(the live project is locked by the MCP server).  `createMission` is at
Ghidra `0xbffd8` (image base `0x10000` + `.text` vaddr `0xaffd8`).

## 1. What the original actually does (new, address-aligned evidence)

### 1.1 `case4Tail` hostile loop — the original re-reads `size()`

Original `0xb4134`-`0xb41d0`:

```text
b415c  ldr r0,[pc,#392]; add r0,pc; ldr.w sl,[r0]   ; sl = Globals::globals
b415e  mov.w fp,#1                                  ; hostile = true, hoisted
b4162  mov.w r9,#0                                  ; i = 0
b4168  ldr.w sl,[r0]                                ; (globals)
b416e  ldr r0,[sl]                                  ; globals->getRandomEnemyFighter
b417a  ldr r0,[sp,#120]; ldr r6,[r0]                ; rnd = *(&Globals::rnd)
...
b4198  strd r0,fp,[sp]                              ; (waypoint, hostile=1)
b419c  movs r0,#0; str r0,[sp,#8]                   ; group = false
...
b41c4  add.w r9,r9,#1
b41c8  ldr r0,[r8,#248]; ldr r0,[r0,#0]; cmp r9,r0  ; i < enemies->size()  RE-READ
b41d0  bcc.n b416e
```

So the original is the **faithful** `for (unsigned i = 0; i < this->enemies->size(); i++)`
(no cached `n`), with `hostile = true` hoisted into `fp` and `&Globals::rnd`
spilled to `sp+120` and reloaded every iteration.  Ours caches `n` in a
`volatile unsigned n` latch at `sp+312`, keeps `&Globals::rnd` in `fp`, and
materialises `1` per iteration (`movs r0,#1; str r0,[sp,#4]`).

The faithful re-read alone is **17164 B** (8 B short), because clang
rematerialises the GOT address `&Globals::rnd` instead of spilling it, so `fp`
is not freed for the `hostile=1` constant.  Every spelling that tries to force
the spill (volatile pointer, `AERandom **`, reference) either stays 17164/17168
or loses the size-exact latch.

### 1.2 Alien-turret loop — the whole register draw is one permutation

Original `0xb3286`-`0xb3394`:

```text
r4 = this (loop top, reloaded at b336e after &rotation clobbers r4)
r5 = &v        (sp+212, hoisted before the loop at b328a)
r6 = gun
r8 = t[k][1] temp
r9 = k*24
fp = i
sl = t base, then alienGuns (reloaded from sp+52 at the bottom)
```

Ours:

```text
r5 = this (reloaded at loop entry)
r4 = gun
r8 = &v        (rematerialised inside the loop at b32b8)
fp = k*24
r9 = i
r6 = t base, then alienGuns (reloaded from sp+52 at the bottom)
sl = &rotation / &v
```

The original clobbers `this` (r4) with `&rotation` and reloads it at the loop
bottom; ours keeps `this` in r5 for the whole loop and rematerialises `&v`.
The two draws are cyclic: `this` in r4 requires `gun` in r6, which requires
`alienGuns`/`t` in sl, which requires `&v` in r5.  No source spelling of the
loop body or declarations tried in waves 118-126 breaks the cycle.

## 2. Variants measured this session (~50 fresh builds)

Each row is a fresh `cmake --build cmake-build-match --target link_so -j6`
followed by the official verifier normalisation.  Baseline is
**92.326733 / 17172 / 6065**.

### 2.1 Faithful alien-loop statement order (original: `setMaxHitpoints, rot, setRotation, rot, setPosition, setScaling`)

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline (tuned: `setScaling` before `setRotation`, fresh `(*enemies)[i]`)** | **92.326733** | 17172 | 6065 |
| faithful order, `gun` receiver | 92.209936 | 17172 | 6063 |
| faithful order, fresh `(*enemies)[i]` receiver | 92.277228 | 17172 | 6065 |
| faithful order, plain `bigAlienWave` (no anchor) | 88.400892 | 17140 | 6058 |
| faithful order, statement `if (bigAlienWave)` (no anchor) | 88.400892 | 17140 | 6058 |
| `Level *self = this` + faithful order | 92.209936 | 17172 | 6063 |
| `Array<KIPlayer*> &enemies = *this->enemies` + faithful order | 88.628487 | 17172 | 6063 |
| `Array<KIPlayer*> *enemies = this->enemies` + faithful order | 88.628487 | 17172 | 6063 |
| `for (i = ..., k = 0; ...)` + faithful order | 92.209936 | 17172 | 6063 |
| `const Vector base` + faithful order | 92.209936 | 17172 | 6063 |
| `base` declared before `v` + faithful order | 92.044892 | 17172 | 6063 |
| `KIPlayer *gun;` declared before the loop, assigned in it | 92.209936 | 17172 | 6063 |
| `KIPlayer *const gun` (tuned body) | 92.326733 | 17172 | 6065 |
| all-fresh `(*enemies)[i]` body (faithful order) | 88.659794 | 17196 | 6070 |
| all-fresh `(*enemies)[i]` body (tuned order) | 88.591933 | 17204 | 6068 |
| `setScaling` before the table select (tuned body) | 92.310231 | 17172 | 6065 |
| `gun` receiver for `setScaling` (tuned body) | 90.955603 | 17164 | 6063 |
| `(bigAlienWave && i >= 0)` anchor | 92.161716 | 17172 | 6065 |
| `(bigAlienWave && (unsigned)i < 100000u)` anchor | 90.844606 | 17180 | 6069 |
| `(bigAlienWave + ((unsigned)i >> 31))` anchor | 92.326733 | 17172 | 6065 |
| `(bigAlienWave + (k < 0))` anchor | 88.413577 | 17140 | 6054 |

### 2.2 `raid` spill-slot swap (`sp+84` vs `sp+88`)

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `bool raid`** | **92.326733** | 17172 | 6065 |
| `int raid` / `char raid` / `unsigned raid` | 92.161716 | 17172 | 6065 |
| `bool raid = cond ? false : …` ternary | 92.326733 | 17172 | 6065 |
| inverted `if (!introSystem || hardCore)` | 92.326733 | 17172 | 6065 |
| `raid` declared after `RaidCoords3` | 71.968257 | 17304 | 6042 |
| `raid` declared after `raiders` | 71.968257 | 17304 | 6042 |
| `float raiders = 0; if (raid) raiders = …` | 92.326733 | 17172 | 6065 |
| `(security == 3) && raid` (instead of `&`) | 87.279327 | 17196 | 6067 |

### 2.3 `case4Tail` loop shape / local reuse

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n` latch** | **92.326733** | 17172 | 6065 |
| `bool hostile = true;` before the loop | 92.326733 | 17172 | 6065 |
| `int hostile = 1;` before the loop | 92.326733 | 17172 | 6065 |
| `Level *self = this` + `self->` | 92.326733 | 17172 | 6065 |
| `unsigned int i = 0; i != n` | 92.326733 | 17172 | 6065 |
| `const unsigned n` latch | 92.178218 | 17172 | 6065 |
| faithful `i < this->enemies->size()` re-read | 91.341312 | 17164 | 6060 |
| faithful re-read + `AERandom **rndp = &Globals::rnd` | 91.341312 | 17164 | 6060 |
| faithful re-read + `AERandom *&rnd = Globals::rnd` | 91.341312 | 17164 | 6060 |
| faithful re-read + `AERandom *volatile rnd` | 91.194190 | 17168 | 6062 |
| faithful re-read + `AERandom **volatile rndp` | 91.194190 | 17168 | 6062 |
| `AERandom **rndp` + latch | 92.326733 | 17172 | 6065 |
| `AERandom *volatile rnd` + latch | 92.161716 | 17172 | 6065 |
| `Array<KIPlayer*> *enemies = this->enemies` local | 91.032093 | 17172 | 6066 |
| `Route *route = this->playerRoute` local | 92.136315 | 17172 | 6064 |
| `AERandom *rnd = Globals::rnd` local | 91.113128 | 17168 | 6064 |
| `Globals *g = Globals::globals` local | 92.187113 | 17172 | 6066 |

## 3. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  The faithful statement order for the alien-turret loop is measurably
*worse* than the tuned wave-113 shape, so the tuned shape is retained.  The
residual is entirely register allocation / spill-slot colouring:

1. alien-turret cyclic draw (`this` r4-vs-r5, `gun` r6-vs-r4, `&v` r5-vs-r8,
   `k*24` r9-vs-fp, `i` fp-vs-r9, `t`/`alienGuns` sl-vs-r6) — ~2.7 pp;
2. `case4Tail` `fp=1` vs `fp=&Globals::rnd` — the original spills the GOT
   address to `sp+120`, ours rematerialises it — ~1 pp;
3. `raid`/`__stack_chk_guard` `sp+84`/`sp+88` swap (17 sites, ~0.28 pp);
4. `case4Else`/`case4Tail` `this` r8-vs-sl;
5. case-183 test order 2,1,0 + body order 0,1,2,default (MBP);
6. branchless `&&` at `+0x2e04`/`+0x2f52` (LICM hoists `!alienWave`);
7. pirate-fortress `ldmia`-vs-`ldrd` SLP pair.

## 4. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7632/score.py            # exact ratio, size, insns
python3 /dev/shm/w7632/analyze.py          # contiguous mismatch blocks
python3 /dev/shm/w7632/runall.py sp_*.py   # apply/build/score a variant
cmake --build cmake-build-match --target check -j6
```

Ghidra headless (project copy, because the live project is locked):

```text
cp -r ../_work/bins/android_2.0.16_libgof2hdaa.so_ghidra.{gpr,rep} /dev/shm/w7632/gh/
/tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless /dev/shm/w7632/gh \
  android_2.0.16_libgof2hdaa.so_ghidra -process android_2.0.16_libgof2hdaa.so \
  -scriptPath /dev/shm/w7632 -postScript Decomp.java 0xbffd8 -noanalysis
```

Scratch under `/dev/shm/w7632` (`Level.base.cpp` = pristine HEAD copy;
`score.py`, `analyze.py`, `batch.py`, `runall.py`, `gen*.py`, `sp_*.py`).

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
