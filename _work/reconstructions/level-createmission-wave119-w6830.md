# w6830: `Level::createMission` (92.3432 -> 92.3432 %, 17172 B) — wave 119
# case4Tail volatile loop, turret call order, raid bound, `&&` and
# `*bigShipsCount > 0` lowering; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6830`, branch `wave119-w6830`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

This continues `level-createmission-wave119-w6804.md` and
`level-createmission-wave119-w6800.md`.  `src/game/world/Level.cpp` is
unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this wave)

```text
_ZN5Level13createMissionEv   official 92.3 %   raw 92.3432 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B.
     .text 0x1337c8 (exact), .ARM.extab 0x26294 (orig 0x26298, 4 B short,
     createShip only), .rodata 0x41c27 (trailing 0xc0 = mission-case table).
```

The fuzzy ratio is `2*M/(6055+6065)`; one extra matching instruction is
`+0.0165 pp`, i.e. 92.3432 -> 92.3597 (rounds to 92.4).  Nothing below
reached that.

## 1. `case4Tail` loop: the faithful form still loses on layout

`Level.cpp:2110` has the tuned `volatile unsigned n = this->enemies->size();
for (unsigned i = 0; i < n; i++)`.  The original re-reads `size()` on every
back edge (`b41c8: ldr.w r0,[r8,#248]; ldr r0,[r0,#0]; cmp r9,r0; bcc`).
Keeping the volatile store *and* re-reading was tried this wave:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile n` cached** | **92.3432** | 17172 | 6065 |
| faithful `for (i < this->enemies->size())` | 91.3578 | 17164 | 6060 |
| `volatile n` + re-read (n unused) | 90.9496 | 17176 | 6066 |
| `volatile n` + re-read + `(void)n` in body | 91.0006 | 17180 | 6068 |
| `volatile n` + re-read + `Objective(...,(int)n,...)` | 91.7898 | 17172 | 6064 |

The last one restores the exact 17172 B size but the tail still shifts: the
first post-loop cluster is at `0x41f4` and every landing-pad branch after it
differs.  The original also *reuses* `sp+124` for `hunterCount` (its `this`
live range is dead there), while ours keeps `this` at `sp+124` and puts
`hunterCount` at `sp+120`; the stack-slot colouring differs independently of
the loop form.

## 2. Turret loop: the original's call order is reachable but scores lower

The original issues `setScaling` **after** `setPosition`
(`b3308 setRotation ... b3320 MatrixRotateVector ... b3328 Vector::operator=
... b3364 setPosition ... b336a setScaling`); ours issues it between the two
`MatrixRotateVector` calls.  Moving it to the end reproduces the original call
sequence exactly (verified with the `blx` census: no call-order diffs remain)
but the fuzzy score drops:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline (setScaling between the rotates)** | **92.3432** | 17172 | 6065 |
| setScaling after setPosition, fresh `(*enemies)[i]` | 92.2937 | 17172 | 6065 |
| setScaling after setPosition via cached `gun` | 90.9721 | 17164 | 6063 |

Other turret-body/declaration permutations measured this wave (all
92.3432 / 17172 / 6065, i.e. byte-identical code): `Vector v;` default ctor,
`Vector v(0,0,0)`, `Vector v = Vector(0,0,0)`, `Vector v = {0,0,0}`,
`Matrix rotation = Matrix()`, `int k` before `Vector base`, `const float
(*const t)[6]`, `Vector pos` local for `setPosition`, `k` declared in the
`for` init, `unsigned k`, `KIPlayer *const gun`, `(bigAlienWave + (i<0) != 0)`
anchor, `this->field_164` -> `*jumperCount`, `this->field_168` ->
`*bigShipsCount` (each and both).  Worse: `((PlayerTurret*)gun)->setScaling`
90.9721 / 17164; `Vector r = MatrixRotateVector(...); v = r;` 90.4801 / 17188;
`gun->cargo` before `gun->shipGroup` 92.3267.

## 3. Raid loop bound: `guns` register vs spill

The original keeps `guns` in a callee-saved register across the raid loop
(`b396a: mov r8,r5; add r5,r0; add r5,r1; add r5,r2; add fp,r5,r3`); ours
reloads it from `sp+112` each iteration (`ldr r4,[sp,#112]; add r4,r0; ...`).
Source forms tried:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.3432** | 17172 | 6065 |
| `const int guns` / `unsigned guns` | 92.3432 | 17172 | 6065 |
| `int raidStart = ...; for (i = raidStart; i < raidStart + hostileCount; ...)` | 91.1415 | 17180 | 6069 |
| `i < hostileCount + guns + friendCount + ...` (term reorder) | 88.7734 | 17176 | 6068 |

## 4. `*bigShipsCount > 0` lowers to `cmp #1; blt` in every source spelling

Original (`b0ea8`): `ldr r0,[r0,#0]; cmp r0,#0; ble.n`.  Ours:
`cmp r0,#1; blt.n`.  The IR is `icmp sgt i32 %x, 0` on both sides; LLVM 7's
DAG combine canonicalises `sgt x,0` to `sge x,1`, which is what selects
`cmp #1; blt`.  All spellings tried give identical code (92.3432): `>= 1`,
`!(x <= 0)`, `0 < x`, `> 0L`, `(long)x > 0`, `x + 0 > 0`, `!localRace`,
`__builtin_expect(x > 0, 0/1)`.  `localRace != 1` is 92.3102; swapping the
`&&` operands (`localRace == 0 && *bigShipsCount > 0`) is 74.3395 / 17164.
Nothing produced `cmp #0; ble`.

## 5. `&&` at 1904 / 1924: branchless form is not reachable without a loss

The original materialises both conditions branchlessly
(`subs; it ne; movne #1; ldr; eor #1; orrs; bne`); ours short-circuits.
Measured this wave:

| variant | official % | size |
|---|---:|---:|
| **baseline** | **92.3432** | 17172 |
| `!((i != X) \|\| !alienWave)` (both) | 92.3432 | 17172 |
| `!((i != X) \|\| !alienWave)` (first only) | 92.3432 | 17172 |
| `!((i != X) \|\| !alienWave)` (second only) | 92.3432 | 17172 |
| `!((i != X) \| !alienWave)` (bitwise, both) | 83.4158 | 17176 |
| `(i == X) & (alienWave != 0)` (first only) | 92.3432 | 17172 |
| `(i == X) & (pirateFortress != 0)` (second only) | 63.2274 | 17312 |

LLVM canonicalises the logical `||`/`!` spelling straight back to the
short-circuit form.

## 6. Other measured negatives

| variant | official % | size | insns |
|---|---:|---:|---:|
| pirate-fortress block before alien-turret block | 88.0984 | 17164 | 6061 |
| `Level *const self = this;` before both blocks | 92.3432 | 17172 | 6065 |
| `Array<KIPlayer*> *const enemies = this->enemies;` before both blocks | 72.4692 | 17264 | 6135 |
| hoist `baseIndex = friendCount + field_164` | 75.5415 | 16968 | 5949 |
| hoist `enemyStart = friendCount + field_164 + field_168` | 75.5122 | 16976 | 5951 |
| hoist both | 75.5415 | 16968 | 5949 |
| `int guns` moved before the turret loop | 79.8217 | 17164 | 6057 |
| convoy `j` loop without the `enemies` cache | 90.1246 | 17180 | 6066 |
| ... without the cache, cargo/crateCaptured swapped | 90.1246 | 17180 | 6066 |
| convoy `start` = `pirateEscorts + stationPirates + guns` | 75.2656 | 17364 | 6086 |
| convoy `start` = `stationPirates + pirateEscorts + guns` | 75.2656 | 17364 | 6086 |
| convoy `start` = `guns + stationPirates + pirateEscorts` | 92.3432 | 17172 | 6065 |
| convoy `start` = `guns + pirateEscorts + stationPirates` | 92.3432 | 17172 | 6065 |
| convoy `start` = `pirateEscorts + guns + stationPirates` | 92.3432 | 17172 | 6065 |
| `int alienGuns` -> `unsigned` / `int fortressGuns` -> `unsigned` | 92.3432 | 17172 | 6065 |
| `alienGuns = bigAlienWave ? 8 : 7` (drop `== 1`) | 92.2937 | 17172 | 6065 |
| `alienGuns = bigAlienWave != 0 ? 8 : 7` | 92.2937 | 17172 | 6065 |

## 7. Residual map (unchanged)

* The suffix (from the alien-turret preheader, `0x322e`) is 78.89 % matched,
  the prefix 96.79 %.  The original hoists the `this` reload to the preheader
  (`b3232: ldr r4,[sp,#124]`, one reload for both turret and fortress blocks);
  ours rematerialises it separately in each block (`b3260`/`b3396`) and the
  allocator then gives `this` r5 and `gun` r4 instead of `this` r4 / `gun` r6.
  Every downstream register (`t` sl vs r6, `k*24` r9 vs fp, `i` fp vs r9,
  `&v` r5 vs r8) follows from that one choice.
* Guard/`raid` spill-slot swap (`sp+88`/`sp+84`) still costs ~19 instructions;
  `raid`'s spill weight stays above the guard's 1.19376 in every spelling.
* Convoy k-chain body order `[0][1][2][default]` is still unreachable from any
  source order (LLVM gives `[0][2][1][default]`).
* `&&` at 1904/1924 branchless OR-of-negations, `cmp #0; ble` at 1837,
  convoy literal-pool order, and the `.ARM.extab` 4-byte shortfall
  (createShip) all remain.

## 8. Conclusion

HEAD remains a local optimum at **92.3432 %** (17172 B, 6055/6065 insns).
The two genuinely-wrong source shapes re-confirmed this wave are the
`case4Tail` `volatile unsigned n` cache and the turret `setScaling` call
position; both are faithful-source wins that *lower* the fuzzy metric because
of the tail shift / block placement they cause.

## 9. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6830/score2.py          # 4-decimal ratio, size, insns
python3 /dev/shm/w6830/diff.py            # per-cluster diff
python3 /dev/shm/w6830/mapdiff.py         # clusters -> source lines
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w6830` (`Level.base.cpp` = pristine HEAD copy,
`score2.py` = unrounded score, `run.py`/`b21..b38.py` = variant batches,
`mirall.txt` = `-mllvm -print-after-all` dump).
