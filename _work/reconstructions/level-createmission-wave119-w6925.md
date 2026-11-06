# w6925: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# stack-slot / tail-register / convoy-switch / turret-anchor sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6925`, branch `wave119-w6925`,
based on current master (`dfe78cca9`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
Continues `level-createmission-wave119-w6834.md`, `…-w6830.md`, `…-w6824.md`.
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this wave)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

The fuzzy ratio is `2*M/(6055+6065)`; one extra matching instruction is
`+0.0165 pp`, i.e. 92.2937 -> 92.3102.  Nothing below reached that.

## 1. Remaining diff clusters (address-aligned, `align.py`)

The whole function is address-aligned except for the case-183 convoy k-chain
(2 B) and the post-chain landing pads.  The residual clusters are:

| cluster | what | size |
|---|---|---:|
| `0x4c` + 16 sites | `raid` / `&Globals::rnd` spill slots swapped (`#84` vs `#88`) | 17 insns |
| `0x2d4` | jump-table halfwords 1-2 too high (downstream placement) | 3 |
| `0xed4` | `cmp #0; ble` (orig) vs `cmp #1; blt` (`*bigShipsCount > 0`) | 2 |
| `0x1434` | case-183 k-chain: tests `[2,1,0]` + body order `[0][1][2]` vs ours `[0,1,2]` + `[0][2][1]` | ~10 |
| `0x1958` | case-2 convoy: orig stores `cargo` then `hasCargo`, reloads `this->enemies`; ours caches `enemies` in r4 and stores `hasCargo` first | 5 |
| `0x2e04`/`0x2f2a` | orig branchless `(i != X) \| !alienWave` / `\| !pirateFortress`; ours short-circuits | 2+2 |
| `0x32ee` | alien-turret table select: orig real `cbz`; ours `cmp/it/movne` cmov | many |
| `0x3e02`+ | case4Tail stack-slot colours (`sp+48/92/20/44/32` vs `sp+44/112/20/40/32`) | many |
| `0x417a`+ | case4Tail: orig keeps `fp=1` (hostile arg) and `sl=&Globals::globals`; ours keeps `fp=Globals::globals` and materialises 1 in r0 | 4 |

## 2. Stack-slot swap: `raid` vs `&Globals::rnd`

The original spills `&Globals::rnd` (`fp`) at `sp+88` and the `raid` bool at
`sp+84`; ours does the reverse.  The swap alone costs 17 instruction matches
(`+0.28 pp` if isolated).

Source forms tried this wave (all keep the 17172 B size unless noted):

| variant | official % |
|---|---:|
| **baseline `bool raid;`** | **92.293729** |
| `int raid` | 92.128713 |
| `char raid` / `unsigned char raid` | 92.128713 |
| `bool raid = false;` at the declaration | 92.293729 |
| `bool raid;` before `security` / before `introSystem` / after `raidRoll` | 92.293729 / 91.452145 / 91.452145 |
| `bool raid;` at the top of the function (empty / `= false`) | 91.452145 |
| `raid = 0;` instead of `raid = false;` | 92.293729 |
| `raid != 0` / `raid == 1` in the `(security == 3) & raid` test | 92.293729 |
| `raid != 0` in the final `if (raid)` | 92.293729 |
| ternary `raid = cond ? true : false` | 92.293729 |
| assign-first-then-override (`raid = roll < …; if (intro) raid = false;`) | 84.884967 (17228 B) |
| `volatile bool raid` (prior wave) | 82.483077 |
| `Globals *globals = Globals::globals;` at fn top / `AERandom *rnd = Globals::rnd;` at fn top | 92.293729 |
| `AERandom *&rnd = Globals::rnd;` at fn top | 92.293729 |

No spelling moves the slot without also re-placing the whole function.

## 3. `case4Tail` loop: faithful re-read vs `volatile` latch

The original re-reads `this->enemies->size()` on every back edge
(`b41c8: ldr.w r0,[r8,#248]; ldr r0,[r0,#0]; cmp r9,r0; bcc`).  The current
`volatile unsigned n` latch is a tuned form that scores higher.

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n` latch** | **92.293729** | 17172 | 6065 |
| faithful `for (i < this->enemies->size())` | 91.308296 | 17164 | 6060 |
| faithful + `(unsigned)` / `int` cast | 91.308296 | 17164 | 6060 |
| `volatile n` + re-read + `(void)n` | 90.918090 | 17180 | 6068 |
| `volatile n` + re-read + `Objective(...,(int)n,...)` | 91.707237 | 17172 | 6064 |
| `unsigned n` (non-volatile) | 92.145215 | 17172 | 6065 |
| `volatile unsigned n = (unsigned) hunterCount;` | 91.228649 | 17168 | 6064 |
| `volatile int hunterCount` | 91.831683 | 17172 | 6065 |
| `volatile n` after the loop + `(void)n` | 91.179140 | 17172 | 6064 |

The faithful form is exactly 8 B short: the original keeps the constant `1`
(`hostile` arg) in `fp` and uses `strd r0,fp,[sp]`, while ours keeps
`Globals::globals` in `fp` and emits `str r0,[sp]; movs r0,#1; str r0,[sp,#4]`.
The 8 B are precisely `mov.w fp,#1` + `strd` vs the 3-instruction sequence.

## 4. `case4Tail` `fp=1` / globals hoist

Tried to free `fp` for the constant `1`:

| variant | official % | size |
|---|---:|---:|
| **baseline** | **92.293729** | 17172 |
| `Globals *globals = Globals::globals;` before the tail loop | 92.104612 | 17172 |
| `Globals *&globals = Globals::globals;` before the tail loop | 92.293729 | 17172 |
| `AERandom *rnd = Globals::rnd;` before the tail loop | 90.949592 | 17172 |
| `AERandom *&rnd = Globals::rnd;` before the tail loop | 92.293729 | 17172 |
| `Globals **gp = &Globals::globals;` + `(*gp)->…` | 92.293729 | 17172 |
| `Globals **gp` + `AERandom **rp` | 92.293729 | 17172 |
| `bool hostile = true;` / `int hostile = 1;` before the loop | 92.293729 | 17172 |
| `bool hostile = mission != nullptr;` | 92.293729 | 17172 |
| `bool hostile = Globals::status != nullptr;` | 90.921085 | 17188 |
| `bool hostile = this->enemies != nullptr;` | 90.967582 | 17180 |
| `bool hostile = hunterCount != 0;` | 91.009568 | 17180 |
| `Globals *g = Globals::globals;` inside the loop | 92.293729 | 17172 |
| `Globals *volatile g = Globals::globals;` inside the loop | 91.156575 | 17176 |
| split `int model = …; Waypoint *wp = …;` before `createShip` | 92.293729 | 17172 |
| `Waypoint *wp` first | 90.940594 | 17172 |

## 5. `case 2` convoy loop (`createShip(…, 44, …)`)

Original reloads `this->enemies` per statement and stores `cargo` (+80) before
`hasCargo` (+76); ours caches `enemies` in r4 and stores `hasCargo` first.

| variant | official % | size |
|---|---:|---:|
| **baseline `enemies` cache, `cargo` then `crateCaptured`** | **92.293729** | 17172 |
| `crateCaptured` then `cargo` | 92.293729 | 17172 |
| `cargo = 0` / `crateCaptured() = false` | 92.293729 | 17172 |
| comma-joined assignments | 92.293729 | 17172 |
| no `enemies` cache (use `this->enemies` everywhere) | 89.976075 | 17180 |
| no cache + `convoyShip` for all uses | 83.542216 | 17140 |
| `enemies` cache declared inside the loop | 84.385053 | 17204 |
| `convoyShip` from `this->enemies`, cache after | 92.293729 | 17172 |
| `hasCargo = 0; cargo = nullptr;` (direct field) | 84.697920 | 17188 |

## 6. Case-183 convoy switch

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline descending chain `2,1,0`** | **92.293729** | 17172 | 6065 |
| ascending chain `0,1,2` (right literal pool) | 92.145215 | 17172 | 6065 |
| nested `2 { 1 { 0 } }` | 92.293729 | 17172 | 6065 |
| `switch`, `default` first | 92.293729 | 17172 | 6065 |
| `if (k > 1) { k==2 … else default } else if (k==1) … else …` | 89.701271 | 17164 | 6063 |
| ascending + `__builtin_expect(k==1, 1)` | 89.809390 | 17172 | 6064 |
| ascending + `__builtin_expect(k==1, 2)` | 89.809390 | 17172 | 6064 |
| ascending + `__builtin_expect(k==0, 1)` | 89.792887 | 17172 | 6064 |
| ascending + `__builtin_expect(k==2, 1)` | 89.875402 | 17172 | 6064 |

## 7. Turret table select / anchor

| variant | official % | size |
|---|---:|---:|
| **baseline `(bigAlienWave + (i < 0)) ? … : …`** | **92.293729** | 17172 |
| `!= 0` / `(bool)` cast / `^` / `-` anchor | 92.293729 | 17172 |
| `== 1` / `|` anchor | 92.244224 | 17172 |
| `(i >= 0)` anchor | 92.178218 | 17172 |
| plain ternary `bigAlienWave ? … : …` | 88.380543 | 17140 |
| plain if/else | 88.380543 | 17140 |
| `(i == 0)` anchor | 90.901592 | 17180 |

## 8. `&&` / `cmp` / type sweeps

| variant | official % | size |
|---|---:|---:|
| **baseline** | **92.293729** | 17172 |
| `alienWave != 0` / `pirateFortress != 0` in the `&&` tests | 92.293729 | 17172 |
| `!((i != X) || !alienWave)` (both) | 92.293729 | 17172 |
| `bool alienWave` / `bool pirateFortress` / `bool bigAlienWave` | 89.615321 / 65.918283 / 65.037159 | 17156/17308/17288 |
| `(unsigned) *bigShipsCount > 0` / `!= 0` / `> 0u` | 89.666557 | 17164 |
| `!( *bigShipsCount <= 0)` | 92.293729 | 17172 |
| `(unsigned) *bigShipsCount >= 1` (second test) | 92.260726 | 17172 |
| `unsigned security` | 92.169321 | 17172 |
| `unsigned raidRoll` | 92.260726 | 17172 |
| `unsigned pirates` / `unsigned localRace` / `unsigned stationIndex` | 92.293729 | 17172 |
| `int introSystem` | 80.227948 | 17164 |
| `volatile int security` / `raidRoll` / `pirates` / `localRace` / `stationIndex` | 67.227029 / 66.962671 / 71.241453 / 53.044766 / 67.994732 | 17440/17364/17404/17428/17328 |
| `volatile int extraFriend` / `volatile int *bigShipsCount` | 79.570957 / 75.584180 | 17172 |

## 9. Conclusion

HEAD remains a strict local optimum at **92.293729 %** (17172 B, 6055/6065
insns).  ~110 new variants were measured this wave on top of the ~300 from the
preceding wave-119 notes; none beat the baseline.  The residual is compiler
register allocation / stack-slot colouring and if-conversion, not source shape:

1. `raid` / `&Globals::rnd` spill slots (`#84`/`#88`) are swapped; no source
   spelling flips them without re-placing the whole function.
2. The case-183 k-chain needs the original's `[0][1][2]` body order *and*
   descending tests; the MIR before MachineBlockPlacement is right for an
   ascending source, but MBP swaps k1/k2.
3. The `case4Tail` faithful re-read is 8 B short because the original keeps
   `fp=1` and `sl=&Globals::globals`; ours keeps `fp=Globals::globals`.
4. The `&&` at 0x2e04/0x2f2a stays short-circuit in every spelling; the
   `*bigShipsCount > 0` test stays `cmp #1; blt`.

## 10. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6925/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w6925/align.py            # address-aligned diff -> align.txt
python3 /dev/shm/w6925/batch.py bN.json    # apply/build/score/restore a batch
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w6925` (`Level.base.cpp` = pristine HEAD copy,
`b1.json`…`b32.json` = variant batches, `align.txt` = address-aligned diff,
`mirall.txt` = `-mllvm -print-after-all` dump).
