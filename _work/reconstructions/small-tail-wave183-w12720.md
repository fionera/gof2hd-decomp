# Small tails — wave 183 / w12720: Generator 40 and MGame 15 are RA plateaus

Status: **research note, no `src/` change landed.**  Worktree `wt/w12720`,
branch `wave183-w12720`, base master `9555474d8` (wave-182 harvest 5).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

Baselines re-confirmed with `tools/fn_diff.py` on the base link:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996/1996 |
| `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11440/11440 |
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948/1948 |
| `_ZN3Gun22calcCharacterCollisionEv` | 458 | 63 | 1984/1984 |
| `_ZN6Status16missionCompletedEbbx` | 721 | 327 | 1432/1432 |

Every candidate below was screened with the committed object-level oracle
`_work/reconstructions/w12120_oracle.py` (path-adjusted copy in scratch) and
the size-exact finalists were re-linked and measured with `tools/fn_diff.py`.
The tree is left clean at the base.

## 1. `Generator::createMission` — 40 raw, frame 104 vs 96

The 40 bytes are 12 same-size substitution runs (no size shift):

```
0x971c4   1   sub sp,#104          vs sub sp,#96
0x971d2   1   str r0,[sp,#100]     vs str r0,[sp,#92]      (canary)
0x9735a   1   bhi target 0x973fa   vs 0x97388              (type>=15 fold)
0x9755e   1   ldr r0,[pc,#952]     vs [pc,#964]
0x976d8   1   str r0,[sp,#96]      vs str r0,[sp,#16]      (bonus=0)
0x9770e   1   str r1,[sp,#44]      vs str r1,[sp,#32]      (rem spill)
0x97718   9   cbz/ldr/subs/str/str vs ldr/cmp/it/subne/str (predicated subne)
0x9783a   1   ldr r1,[sp,#96]      vs ldr r1,[sp,#16]      (setBonus)
0x978b0   1   ldr r0,[sp,#100]     vs ldr r0,[sp,#92]      (canary)
0x978bc   1   addeq sp,#104       vs addeq sp,#96
0x97916   1   b.n 0x97920          vs b.n 0x9791c
0x97918  21   String-D1 cleanup block / embedded literal pool order
```

The current shape (`volatile int bonus`, `volatile int bv`, `volatile int rem`)
is the only one found that reproduces the original's `rawBonus=r6`, `b=r5`
assignment and the `str rem` / reload / predicated `subne` order.  Its price is
the frame: the two volatile locals occupy `sp+92`/`sp+96` and push the canary
to `sp+100`, so `sub sp,#104` instead of the original `#96`.

New shapes measured this wave (all function size 1996 unless noted):

| shape | oracle diff | frame | note |
|---|---:|---:|---|
| current (`volatile bonus` + `volatile bv` + `volatile rem`) | **25** | 104 | base |
| `volatile bonus` at top, no `bv` | 123 | 104 | RA reverts |
| `volatile bonus` at `Galaxy *galaxy` / `level` / `reward` / before-if | 29 | 104 | slot still top-of-frame |
| `int bonus` at top / before-if, plain `rem`, `bonus -= rem*2` | 30 | 96 | **raw 627**: zero arm emits `movs r6,#0`, 2-byte shift |
| same with `int bonus[1]` or `struct { int v; } bonus` | 30 | 96 | still promoted to r6 |
| same with `(void)&bonus` / `int *bp=&bonus` | 30 | 96 | address-taken but SSA-promoted |
| `volatile int *bonusPtr = &bonus`, `*bonusPtr` accesses | 73 | 96 | raw 171 |
| `int bonus` + `*(volatile int*)&bonus = 0` + `bv` | 25 | 104 | bonus slot moves to sp+96 |
| `volatile int bonus` + `volatile int &bv = bonus` | 99 | 104 | one slot, but RA wrong |
| `volatile short/char bv` | 45 | 104 | size 2000 |
| `volatile long long bv` | 117 | 112 | size 2000 |
| `bv = rawBonus` after `rawBonus` / `remVal` / `b` / `rem` | 57-58 | 104 | size 2004 |
| `bv = rawBonus` before `if` | 58 | 104 | size 2004 |
| `volatile int rem` replaced by `volatile const`, assignment, pointer | 25 | 104 | identical code |
| `int remVal`/`int b` operand/negation spellings | 25 | 104 | identical code |
| `int bonus` + `volatile int bv` (no volatile bonus) | 151 | 104 | size 2000 |

The two remaining structural levers are independent compiler decisions:

1. **Frame 96** requires no volatile local.  But without a volatile local the
   allocator keeps `bonus` in `r6` and the `bonus = 0` arm collapses to
   `movs r6,#0` (4 B instead of 8 B), shifting every following byte by 2 and
   exploding raw to 627.  Forcing `bonus` to memory without `volatile`
   (address-taken array/struct/pointer) is SSA-promoted away.
2. **`r6`/`r5` assignment + rem spill** requires the volatile `bv`/`rem`
   anchors.  Removing either reverts the allocator (oracle diff 123).

No shape found gives both.  The `bhi` fold at `0x9735a` and the tail block
placement are downstream of the same `-Oz` block-placement choice; the
`__builtin_expect(race < 4 && type == 13, 1)` anchor is required for size
1996 (every non-expect spelling shrinks to 1956/1992), and every nonzero
expect weight gives identical code.

## 2. `MGame::OnUpdate` — 15 raw, two independent ties

The 15 bytes are 4 runs:

```
0x17e2bf   2   ldrd r2,r3,[r2,#256]   vs ldrd r3,r5,[r2,#256]
0x17e2c2   1   sbcs r1,r3             vs sbcs r1,r5
0x17e2c9   2   mov.w r2,#0 / sbcs.w   vs mov.w r3,#0 / sbcs.w
0x17f18c  10   ldr.w r0,[r9] + blx     vs str r2,[sp,#120] + blx + ldr r0,[sp,#120]
```

The first three are the m23 64-bit idle-time compare (`getPlayingTime() -
field_0x100 > 3600000`).  The last is the m122 radio body: the
`Status *volatile st` anchor keeps `&Globals::status` in `r9` (as the
original) but spills `st` to `sp+120`, replacing the original's single
`ldr.w r0,[r9]`.

Shapes measured this wave (all size 11440 unless noted):

| shape | oracle diff | note |
|---|---:|---|
| current `Status *volatile st` + `Level *level` before ccm | **7** | raw 15 |
| same, `this->level` direct (no level local) | 7 | raw 15, identical |
| wave-163 `Level *level` before ccm, direct `Globals::status` | 6 | raw **16**; m23 exact, level load hoisted before ccm |
| `Level *level` after ccm / reference-to-field / `Level **` | 503 | size 11392 |
| `volatile Level *level` after ccm | 500 | size 11396 |
| `Status *st` plain (no volatile) | 24 | size 11444 |
| `Status *&status` / `Status **statusp` | 503 | size 11392 |
| `volatile int stv = (int)Globals::status` | 22 | size 11440 |
| `volatile char/short/long long` dummy + `Status *st` | 29-32 | size 11444 |
| `reinterpret_cast<int64_t&>` / `*(long long*)` cast spellings | 7 | identical |
| `3600000LL` / `(long long)3600000` | 7-8 | identical |
| m23 `&&` reorder (`time_first`, `time_second`, `time_last`) | 96-234 | size 11436/11440/11432 |
| m23 `swapjump` / `swapmission` | 11 / 96 | size 11440 |

The m23 tie is a side effect of the volatile `st` frame slot: the
wave-163 shape (no volatile local) makes m23 byte-exact but hoists
`this->level` before the `getCurrentCampaignMission` call (16 raw).  The
volatile shape keeps the level load after the call but spills `st` (15 raw).
No shape found gives both; `15` is the raw minimum.

## 3. `SpaceLounge::drawLounge` — 175/41/1948, re-confirmed

The residual is the right-edge clamp: original keeps `pad` in `r9` and `x` in
`r5` and emits `addgt.w r1,r6,sl; subgt r5,r0,r1`; ours keeps `pad` in `fp`
and `x` in `r9` and emits the 4-instruction
`movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6; addgt.w r9,r1,r0`.
The faithful 2-instruction arm measures raw 445.  Re-swept the clamp
association, condition sum order, declaration order, local types and
`__builtin_expect` values; no size-1948 shape moves `pad` to `r9` (the
`long long pad` shapes that do are 1980/2008 B).  Base 175 remains best.

## 4. `Gun::calcCharacterCollision` — 458/63/1984, re-confirmed

The residual is the `i*3` index (`r4` original vs `r5` ours) and the
`enemyPosition - playerPosition` scratch Vector stack slot (`sp+104` vs
`sp+44`), plus the scatter-loop `subs`/`vmov` schedule.  The wave-176
`int diff = this->magnitude - distance` fold (463 -> 458) is still the best
size-exact shape; `diff >= 1`, `diff != 0`, inline forms, loop-var types,
pointer locals and `Matrix` copy variants all re-measure at 458 or worse.

## 5. `Status::missionCompleted` — 721/327/1432, re-confirmed

The known PGO plateau.  The plain `switch (m->getType())` is required for
`.rodata` exactness (the former 192 B rank table was removed in wave 121).
The if/else-chain fold produces the target weight vector but the post-MBP
`-Oz` size passes canonicalise the block order back to numeric, so every
source chain/nested/expect shape is `cmp`-identical to the base.  The
profile-only 558/47 plateau is not source-expressible.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12720
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN5MGame8OnUpdateEv _ZN11SpaceLounge10drawLoungeEv \
  _ZN3Gun22calcCharacterCollisionEv _ZN6Status16missionCompletedEbbx
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=40/15/175/458/721`, `aligned=21/111/41/63/327`,
sizes `1996/11440/1948/1984/1432`, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12720/`
(`oracle.py`, `sdiff.py`, `sweep_gen*.py`, `sweep_ou*.py`, `sweep_m23*.py`,
`sweep_cond.py`, `sweep_exp.py`, `sweep_anchor.py`, `sweep_rem.py`,
`sweep_first.py`, `sweep_vst.py`, `Generator.base.cpp`, `MGame.base.cpp`).
