# `Gun::calcCharacterCollision` wave 191b (w13674): the r4/r5 swap is
# reachable from source (several `expect`/scope shapes flip `i*3` to r4) but
# every flip is size-inexact; no landed win

Worker: w13674. Branch `wave191b-w13674`, base master `2064dfd40`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13674`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:

| build | raw | aligned | size | insns | `i*3` |
|---|---:|---:|---:|---:|---|
| committed (HEAD `2064dfd40`) | **455** | **93** | 1984/1984 | 660/660 | r5 |
| faithful C-D-E `expect(mag>dist,0)` | 463 | **57** | 1984/1984 | 660/660 | r5 |

`check` is green on the untouched tree: byte 4508, linked 4508, size 4524,
stub 0, missing 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`.

`git status --porcelain` is empty apart from this note.

## 1. New evidence (a): the LICM/CSE merge is the whole residual, and it is
## *use-count* driven

Re-reading the `-print-after-all` MIR of the faithful build pins the exact
mechanism (all line numbers from `mir_faith.txt`):

* Pre-LICM (`Remove dead machine instructions`, 823755) the radius-block
  `playerPosition` address is `%114 = t2ADDri %stack.10` in **bb.18**, used
  **twice** (`$r0 = COPY` for `PlayerEgo::getPosition`'s sret, then `$r2 =
  COPY` for `AEMath::operator-`).
* `Early Machine Loop Invariant Code Motion` (825343) hoists `%114` to
  **bb.5** (the e-loop preheader) and renames it `%308`.
* `Machine Common Subexpression Elimination` (826915) then folds the view
  block's `t2ADDri %stack.10` and the k-loop sret's `t2ADDri %stack.10` into
  that same `%308`.
* Final `Live Interval Analysis` (846003): `%308` def **1568B**, last use
  **16176B** (the k-loop `getPosition` sret) → **14608B**; `%99` (`i*3`,
  `t2ADDrs %16,%16,10`) def **2880B**, last use **15712B** → **12832B**.
  `RAGreedy` enqueues size-descending, so `%308` takes **r4** and `i*3` falls
  to **r5** — exactly the w13576/w13588/w13602/w13628 anchor.

**The use count is the lever.** Two `t2ADDri %stack.10` in the mine branch
(`%198` at 10528B, `%208` at 10912B) are used **once** each and are *not*
hoisted (they stay in bb.52). Every frame address used ≥2× is hoisted to
bb.5. This is why the original can keep `&view` in r5 across the view block
(one sunk, single-block vreg) while ours rematerialises it: ours is merged
with the radius and k-loop addresses, so `Machine code sinking` cannot sink
it (multi-block uses).

The original's own view block confirms it: `add r5,sp,#104` once at
`0x1533c8`, then `mov r0,r5` / `mov r1,r5` / `mov r2,r5`; ours emits
`add r0,sp,#104` / `add r1,sp,#104` / `add r4,sp,#104` (base) at the same
addresses.

## 2. New evidence (b): the r4/r5 swap *is* reachable from source

Several source shapes do flip `i*3` to **r4**. All are size-inexact, but the
flip itself is no longer a mystery:

| variant | raw | aligned | size | insns | `i*3` |
|---|---:|---:|---:|---:|---|
| faithful + `expect(geometries != nullptr, 1)` | 1609 | 230 | **1956** | 658 | **r4** |
| faithful + `expect(mine, 1)` | 1833 | 262 | **1996** | 666 | **r4** |
| faithful + `expect(enemy->isAsteroid(), 1)` | 1789 | 524 | **1968** | 653 | **r4** |
| faithful + `expect(!enemy->egoFlag && …, 1)` | 1795 | 357 | **1928** | 652 | **r4** |
| faithful + `expect(explosive, 1)` | 1827 | 466 | **1948** | 646 | **r4** |
| faithful + function-scope `Vector kpos` k-loop sret | 1826 | 292 | **1972** | 664 | **r4** |
| faithful + function-scope `playerPosition` + `kpos` | 1746 | 298 | **1988** | 669 | **r4** |

Every flip also re-runs `MachineBlockPlacement`, so the whole stream moves;
raw is 1.6k–1.8k, not the hoped-for ~190. The size never lands on 1984.

## 3. Angle (a): compensating the faithful body's +8 raw

The faithful C-D-E body is raw 463 (base 455) and aligned 57 (base 93). The
+8 is *not* one structural item: comparing per-byte matches against the
original, the faithful body is worse than the base in 14 bytes (all inside
`0x153481..0x1534eb`, the k-loop body) and better in 6 (`0x153499`,
`0x15349b`, `0x1534b6`, `0x1534ef`×3). It is the header-first k-loop shift
(see §4), not the body order.

Body-order × guard matrix (all size-exact 1984 / 660 insns):

| order / guard | raw | aligned |
|---|---:|---:|
| E-C-D `expect(diff>0,0)` (committed) | **455** | **93** |
| E-C-D `expect(mag-dist>0,0)` | **455** | **93** |
| E-C-D `expect(mag>dist,0)` | 460 | 89 |
| E-C-D `expect(dist<mag,0)` | 460 | 89 |
| C-E-D `expect(diff>0,0)` | 457 | 84 |
| C-E-D `expect(mag>dist,0)` | 462 | 78 |
| C-D-E `expect(diff>0,0)` | 458 | 63 |
| C-D-E `expect(mag-dist>0,0)` | 458 | 63 |
| C-D-E `expect(mag>dist,0)` (faithful) | 463 | **57** |
| C-D-E `expect(dist<mag,0)` | 463 | **57** |
| D-C-E `expect(diff>0,0)` | 463 | 83 |
| D-C-E `expect(mag>dist,0)` | 466 | 80 |
| D-E-C / E-D-C (all four guards) | 1788–1803 | 157–171 (**1940 B**) |

Best faithful-body raw is **458** (`diff>0` / `mag-dist>0` guard), 3 B short
of the base. No compensating shape found that recovers those 3 B.

Other faithful-body shapes (size-exact unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| faithful + `Vector diff = enemyPosition - getPlayer()->getPosition()` (diff-first slot swap) | 467 | 67 | 1984 |
| faithful + swap the two `SetAnimationState` calls | 465 | 59 | 1984 |
| faithful + `TransformSetLocal` before `MatrixSetTranslation` | 472 | 70 | 1984 |
| faithful + `expect(this->impact != nullptr, 1)` | 463 | 57 | 1984 |
| faithful + `expect(this->playerGun, 1)` | 463 | 57 | 1984 |
| faithful + `expect(this->empDamage != kNoAttribute, 1)` | 465 | 60 | 1984 |
| faithful + `Matrix *vp = &view` / `Matrix &vp = view` | 463 | 57 | 1984 |
| faithful + `Matrix *volatile vp = &view` | 1121 | 138 | **1988** |
| faithful + `(char*)&view + (i >> 31)` | 1100 | 105 | **1992** |
| faithful + `(char*)&view + (enemy == nullptr)` | 463 | 57 | 1984 (folded) |
| faithful + `(char*)&view + (i == 0xFFFFFFFFu)` | 463 | 57 | 1984 (folded) |
| faithful + `hitPositions[i]` moved after the k-loop | 1808 | 270 | **1912** |
| faithful + `Vector kpos;` in the k-loop | 1668 | 271 | **2004** |
| faithful + function-scope `playerPosition` | 1795 | 246 | **1960** |
| faithful + `Matrix view; view = …` | 1889 | 243 | **2056** |

Loop / decl forms on the committed body that are codegen-identical to the
base (455/93/1984): `i != count`, `i <= count-1`, `e != size`, `i = i + 1`,
`k = k + 1`, `unsigned long i/k`, `int e`. Adding `expect` to the e-loop or
i-loop condition is also codegen-identical.

## 4. Angle (c): the collision-test operand order is already matched

The main collision test `enemyPosition.{x,y,z} - position.{x,y,z} +
velocity.{x,y,z}` (`0x1531b8..0x153254`) and the mine test
(`0x15351c..0x1535aa`) are **byte-identical** between the committed build and
the original (instruction indices 200–245 and 461–539 in the aligned diff).
The compiler reassociates `enemyPosition.x - position.x + velocity.x` into
`(position.x - enemyPosition.x) + velocity.x` and compares against `radius`
and `-radius`, which is exactly the original's `vsub/vadd/vcmpe` sequence.
No operand-order change is available.

## 5. `.ARM.extab` / `.ARM.exidx`

Per-function section sizes from the object (`objdump -h`):

| build | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|
| committed base | 1984 | 16 | 8 |
| faithful C-D-E | 1984 | 16 | 8 |

The faithful body is extab-neutral, so its only cost is the 8 raw bytes.

## 6. Conclusion

The residual remains the single greedy-RA decision documented in
w13576/w13588/w13602/w13609/w13619/w13628: the LICM-hoisted, CSE-merged
`&stack.10` (`%308`, 14608 B) takes r4 and pushes `i*3` (12832 B) to r5.
This pass adds:

1. The merge is **use-count driven**: single-use frame addresses (mine
   branch `%198`/`%208`) are not hoisted; ≥2-use ones are hoisted to bb.5
   and CSE-merged. The original keeps `&view` as a short single-block vreg
   (r5) because it is not merged across blocks.
2. The r4/r5 swap **is** reachable from source (`expect(...)` hints,
   function-scope temps) — but every flip is size-inexact (1928–2004 B) and
   re-runs block placement, so raw is 1.6k–1.8k.
3. The faithful body's +8 raw is spread over 14 single bytes in the k-loop
   body, not a structural item; best faithful raw is 458.
4. The collision-test operand order is already byte-identical.

The committed body stays (raw 455 / aligned 93 / 1984 B).

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13674
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

MIR probe (scratch, 12 GB ulimit, output to disk):

```sh
clang++ <Gun.cpp compile line from compile_commands.json> \
  -mllvm -print-after-all > /home/debian/GalaxyOnFire2/scratch/w13674/mir.txt
# 823755 pre-LICM:  %114 = t2ADDri %stack.10 in bb.18 (2 uses)
# 825343 post-LICM: %308 = t2ADDri %stack.10 in bb.5
# 826915 post-CSE:  view + k-loop ADDri folded into %308
# 846003 LIA:       %308 1568..16176 (14608); %99 2880..15712 (12832)
```

## 8. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13674.md` only.
