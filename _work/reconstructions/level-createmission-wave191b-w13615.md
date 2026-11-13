# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13615

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13615`, base master `8c8c1826d`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13589.md` (two-mechanism split,
post-RA MachineLICM diagnosis), `...w13567.md` (tail `&Globals::rnd`
live-range) and `...w13545.md` (case-1 hoist).

```text
                                     size            raw    aligned  insn
base 8c8c1826d                       17172/17172     2626    845     6206/6197
this pass (no new win)               17172/17172     2626    845     6206/6197
```

No size-exact raw win was found; the tree is left at `8c8c1826d`
(`git status --porcelain` empty).

## 1. Baseline region table (unchanged)

`python3 scratch/w13615/regiondiff.py` on the pristine tree:

| region | diff bytes | mechanism |
|---|---:|---|
| `0x0..0x1500` | 2 | branch-target indices |
| `0x1500..0x178e` (case-1 `enemyRace` reload) | 300 | **post-RA MachineLICM hoist** |
| `0x178e..0x1930` (case-2 prologue) | 39 | scheduling of the same stores (`strd`/`str`) |
| `0x1930..0x1980` (case-2 loot) | 17 | source order (CSGR best, w13545 §6) |
| `0x1980..0x322e` | 66 | `r1` vs `r6` regalloc + a tail-merged `Objective` epilogue |
| `0x322e..0x4314` (alienGuns/fortress/raid tail) | 2202 | **greedy-RA live range** |
| total | 2626 | |

The `-2` from the case-1 hoist is *not* a simple `+2` elsewhere: the case-2
prologue re-schedules the same store sequence
(`strd r0,r0,[sp,#272]` + `str r0,[sp,#280]` + `movs r0,#0` in ours vs
`str r0,[sp,#100]` … in the original) and re-syncs exactly at `0xb178e`.
Any local fix on one side therefore changes the compensation on the other;
a real win needs **both** mechanisms fixed (or an independently matching
`+-2` pair).

## 2. Case-1: new shapes tried (all neutral)

Loop body `for (int i = 0; i < attackerCount; i++) { … }` (source lines
2265–2269).  Every variant below measured **2626 / 845 / 17172**, i.e. the
`renamable $r5 = t2LDRi12 %stack.75` load is still hoisted into the
preheader (`bb.371`) by post-RA MachineLICM:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `int erace = enemyRace;` before loop, use `erace` | 2,626 | 845 | 17,172 |
| `int &erace = enemyRace;` before loop | 2,626 | 845 | 17,172 |
| `const int &erace = enemyRace;` before loop | 2,626 | 845 | 17,172 |
| `int *erace = &enemyRace;` before loop, use `*erace` | 2,626 | 845 | 17,172 |
| `const int enemyRace` | 2,626 | 845 | 17,172 |
| `int i = 0;` outside, `for (; i < attackerCount; i++)` | 2,626 | 845 | 17,172 |
| `if (i >= attackerCount) break;` appended to body | 2,626 | 845 | 17,172 |
| `Array<KIPlayer*> &enemies = *this->enemies;` | 15,080 | 1,041 | **17,180** |
| `Array<KIPlayer*> *enemies = this->enemies;` | 15,080 | 1,041 | **17,180** |
| `*(volatile int*)erace` (volatile read only) | 16,406 | 2,369 | **17,232** |
| `enemyRace = enemyRace + 0;` in loop + dead use after | 15,669 | 1,526 | **17,132** |

The two `enemies`-local forms and the volatile-read form *do* change the
prologue/loop enough to move the load, but at a size cost.  The volatile
read keeps `ldr.w` (4 B) instead of the original's 2-byte `ldr`, so it can
never be size-exact without an independent `-2`.

### Why the hoist is hard to block (MIR evidence)

From `scratch/w13615/mir_all.txt` (`-mllvm -print-after-all`,
`-filter-print-funcs=_ZN5Level13createMissionEv`):

* pre-RA (`# *** IR Dump After Live Register Matrix ***`, line 1247999):
  `bb.226 (%ir-block.1751)` — the `if (alienGuns != 0)` test — is just
  `t2CMPri %3015:rgpr, 0` + `t2Bcc`/`t2B`.  **No** `%stack.*` load.
* post-RA (`# *** IR Dump After Machine Copy Propagation Pass ***`,
  line 1296346): the same block now starts with
  `renamable $r8 = t2LDRi12 %stack.73` (this) and
  `renamable $r4 = t2LDRi12 %stack.79` (`&Globals::rnd`).

So the tail's extra `ldr r4,[sp,#100]` at `0xb3234` is **not** a source
expression and not a MachineLICM hoist — it is a *greedy-RA live-range
split* of the `&Globals::rnd` value, inserted by `VirtRegRewriter` at the
`bb.226` block start.  The value is live from its last use before the
alienGuns block (`0xb31b0`) through the alienGuns/pirateFortress blocks to
the raid block (`0xb3788`), and the allocator picks `bb.226` as the split
point.  The original reloads it only at `0xb3788` (and again at
`0xb388a`, `0xb39e2`, `0xb3cde`).

For case 1, the load is in `bb.376` (loop body) and the preheader `bb.371`
terminator is `t2B` (no registers), so all four `HoistRegionPostRA`
conditions still pass; `StoredFIs` is empty because no loop block stores to
an FI (`bb.376`'s `$sp` stores are physreg, not FI).  Blocking the hoist
needs either a front-end stack object (not a spill slot) or a store to the
FI inside the loop — both cost bytes in every cheap shape tried.

## 3. Tail: new shapes tried

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,626** | **845** | 17,172 |
| `Globals *&globals` (non-volatile) | 2,627 | 844 | 17,172 |
| `AERandom *&rnd` (non-volatile) | 2,627 | 845 | 17,172 |
| `raid` block uses `Globals::rnd` directly (drop volatile ref) | 2,627 | 845 | 17,172 |
| direct `Globals::rnd` + drop `globals` volatile | 2,628 | 844 | 17,172 |
| keep `rnd` volatile, drop `globals` volatile | 2,627 | 844 | 17,172 |
| `int guns` first in the raid block | 2,626 | 845 | 17,172 |
| `bool hard` first in the raid block | 4,201 | 848 | **17,160** |
| `(void) &Globals::rnd;` before the alienGuns block | 2,626 | 845 | 17,172 |
| `AERandom *volatile &rnd` declared before alienGuns | 2,626 | 845 | 17,172 |
| raid condition uses `Globals::rnd` (ref kept for loop) | 2,626 | 845 | 17,172 |
| `(void) Globals::rnd;` inside alienGuns block | 2,626 | 845 | 17,172 |
| `Vector v;` (no init) in alienGuns | 2,626 | 845 | 17,172 |
| `Vector v;` (no init) in pirateFortress | 2,626 | 845 | 17,172 |
| both `Vector v;` | 2,626 | 845 | 17,172 |
| `v = MatrixRotateVector(...)` directly (drop `Vector p`) | 16,656 | 2,244 | **17,316** |
| pirateFortress: setPosition before setRotation | 16,650 | 2,268 | **17,316** |

None is a raw win.

### 3.1 A semantic finding (not a raw win)

The current alienGuns source is **semantically inconsistent with the
binary**:

```cpp
v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
gun->geometry->setRotation(v);
```

The binary (`0xb32f6` rotate `{t[3],t[4],t[5]}` → `0xb3300` `Vector::op=` →
`0xb3308` `AEGeometry::setRotation`) uses the **first** (t[3..5]) rotation
for `setRotation` and the second (t[0..2]) for `setPosition`
(`0xb3320`/`0xb335e`).  So the faithful source is

```cpp
v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
gun->geometry->setRotation(v);
v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
gun->setPosition(...);
```

Measured variants:

| alienGuns statement order | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base (rot1; rot2; setRotation; cargo; setPosition; setScaling; group) | **2,626** | 845 | 17,172 | 6206 |
| `rot1; rot2; setRotation` but **swapped** (`rot2; rot1; setRotation; …`) | 2,626 | **830** | 17,172 | 6208 |
| faithful (`rot1; setRotation; rot2; setPosition; setScaling; cargo; group`) | 2,642 | 1,022 | 17,172 | 6206 |
| faithful with `cargo` before `setScaling` | 2,638 | 1,022 | 17,172 | 6206 |
| `rot1; setRotation; rot2; cargo; setPosition; setScaling; group` | 2,636 | 841 | 17,172 | 6206 |
| base + `cargo` after `setScaling` | 2,632 | 844 | 17,172 | 6206 |
| base + `cargo` after `shipGroup` | 2,634 | 841 | 17,172 | 6206 |
| base + `cargo` between the two rotations | 2,632 | 846 | 17,172 | 6206 |

The **swapped** variant is the only one that improves `aligned`
(845 → 830) at unchanged `raw`/`size`, but it is *also* semantically wrong
(it makes `setPosition` use the t[3..5] rotation).  It is recorded here as
a possible aligned-only trade, not landed (raw is the primary metric and
the gate counts exact functions, which this does not change).

The binary's `cargo = nullptr` is at `0xb3372`, i.e. **after**
`setScaling` (`0xb336a`); the source has it before `setPosition`.  Moving
it costs raw bytes (rows above), so it is left alone.

## 4. Scratch / reproduce

`/home/debian/GalaxyOnFire2/scratch/w13615/`:
`measure.sh` (incremental `link_so -j2` + `fn_diff` line),
`Level.base2.cpp` (pristine TU), `variant.py`, `regiondiff.py`,
`aligndiff.py`, `dumpmir.sh`, `mir_all.txt` (131 MB full
`-print-after-all`; greedy RA at line 1259734, MachineCopyPropagation at
1296346, MachineLICM at 1308432), `MachineLICM.cpp`, `orig.dis`, `ours.dis`,
`sweep11.py`..`sweep16.py` (this pass).

```sh
cd /home/debian/GalaxyOnFire2/wt/w13615
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=845 size=17172/17172 insn=6206/6197`.

## 5. Next steps

1. **Case-1 (~300 raw).** The only remaining lever is making the
   `enemyRace` load a non-spill-slot (`isSpillSlotObjectIndex == false`) or
   making `fi#75` store-bearing inside the loop *without* adding bytes.  A
   front-end object requires `volatile`/address-escape, both of which cost
   bytes; the original's `ldr r5,[sp,#116]` is 2 B, so any replacement must
   also be 2 B.
2. **Tail (~2202 raw).** It is a greedy-RA live-range split of
   `&Globals::rnd` at `bb.226`; the pre-RA MIR has no reload there, so the
   split point is chosen by the allocator from global register pressure.
   A source edit that changes the pressure *before* the alienGuns block
   (e.g. the value's last use at `0xb31b0`) is the plausible way to move
   the split point to `0xb3788`.
3. Both mechanisms must be fixed together (or an independent matching
   `+-2` pair found) for the size to stay 17,172.
