# `Level::createScene` — wave-102 wf212: capacity-first × `rnd`-mask sweep,
# real post-RA MIR extraction, and the `k`-vs-`hangarPart` priority inversion

**Status:** research note, 2026-11-04. Worktree `wt/wf212`, branch `wave102-wf212`,
base `53d9ee4d5`. **No source change landed** — no source shape found that raises
`byte_exact` or the fuzzy score above the shipped 88.3 % while keeping the
2252-byte size. This commit contains only this note.

Follow-up to `level-createscene-wave101-wf111.md` (which followed
`…-wave100-w5012.md`, `…-wave99-w4911.md`, `…-wave98-w4814.md`,
`…-wave97-w4712.md`, `…-wave95-w4514.md`, `…-wave94-w4414.md`,
`…-wave92-w4249.md`, `…-wave92-w4213.md`, `…-wave91-w4113.md`,
`level-createscene-phi-use-w4013.md`, `level-createscene-licm-hoist-w3833.md`,
`Level-createScene-flag-free.md`, `level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`, first byte diff at
  **0x547** (the parts-loop preheader `add.w r9/r8, r0, r2, lsl #4`).
* Gate re-measured before and after this pass, unchanged and green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* **New: `capacity`-before-`ownHangar` × the full 128-value `rnd`-use mask is
  swept.** On the shipped (local-`rnd`) shape the best is **82.7 % / 2268 B**
  (masks `0x27 0x2b 0x2d 0x2f 0x33 0x35 0x36 0x3a 0x3c 0x63 0x65 0x66 0x6a
  0x6c 0x6e 0x72 0x74`), strictly worse than 88.3. §3.
* `all-globals` + `capacity`-first reproduces the **original parts loop
  exactly** (`row r9, Canvas r8, zero sl, k r6, hangarPart spilled,
  j sl, childCount fp, childMesh r9, child r4, g r5`) but rotates the
  prologue and is **2264 B** (12-byte size regression). §4.
* **New tool: the real post-RA MIR is extractable from `_work`-style
  `-print-machineinstrs` dumps.** For `Level.cpp` the section
  `# After Virtual Register Rewriter` at `mi.txt` lines 1157996–1159843 is
  the physical-register MIR of `createScene` (the `# After Greedy` section at
  1156154 is still virtual). This is the authoritative RA record; the
  wave-101 llc replay is **not** faithful. §5.
* A hand liveness pass over the extracted pre-greedy MIR gives the shipped
  parts-loop interval sizes (live instruction slots): `row` 108, `pi` 107,
  `vecaddr` 106, **`k` 105**, `g` 89, **`hangarPart` 65**, `childCount` 41,
  `child` 29, `j` 26, `mesh` 15. `k` is larger than `hangarPart`, yet the
  allocator spills `k`; the decision is made by the greedy eviction/split
  order, not raw size. §6.
* New source variants measured this pass (all ≤ 88.3): `self_local` 88.3,
  `partcount` 88.3, `hangar_out_init` 88.3, `g_outside` 88.3,
  `ownhangar_int` 88.3, `shipcount_uint` 88.3, `capacity_const` 88.3,
  `spot_int` 88.3, `zero_var`/`zero_var_inside` 88.3,
  `j_unsigned`/`i_unsigned`/`spot_unsigned` 88.3, `k_outside` 88.1,
  `vec_out_assign` 85.3, `flat_hangarmeshes` 84.4, `vec_out_ctor` 83.9,
  `enemies_local` 76.6, `vec_static_local` 72.6. §7.

## 1. Baseline

```sh
cd /home/debian/GalaxyOnFire2/wt/wf212
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level11createSceneEv
cmake --build cmake-build-match --target check -j8
```

`_ZN5Level11createSceneEv: pct=88.3 orig=2252B ours=2252B insns=783/791
prefix=0x547`. `check` → `RATCHET PASS` (numbers above).

The first diff is the parts-loop preheader: original
`add.w r9, r0, r2, lsl #4` (`row` in `r9`), ours `add.w r8, …` (`row` in
`r8`). Everything before 0x547 is byte-identical. The remaining diff is
entirely register allocation plus the spill/reload instructions it implies.

## 2. Register maps (unchanged from wave 101)

Parts loop:

| value | original | shipped |
|---|---|---|
| `row` | `r9` | `r8` |
| `&Globals::Canvas` | `r8` | `r9` |
| zero (Vector temp) | `sl` | `r4` |
| `k` | `r6` | `r6`, spilled to `sp#44` in the body |
| `hangarPart` | `sp#44` (spilled) | `r6` |
| `j` / child count | `sl` / `fp` | `sl` / `r8` |
| child mesh | `r9` | `fp` |
| `child` / `g` | `r4` / `r5` | `r4` / `r5` |

Ship loop:

| value | original | shipped |
|---|---|---|
| `&Globals::rnd` | `r8` | `sp#44` |
| `rnd` value | reloaded at every site (`ldr r0,[r8]`) | cached in `r5` |
| `capacity` | `r6` | `fp` |
| `race` | `r5` | `r6` |
| `type` | `fp` | `r8` |
| `i` | `r9` | `r9` |
| `&Globals::status` | `sl` | `sl` |
| `spotUsed` | `r4` | `r4` |

The parts-loop difference is exactly the `k`↔`hangarPart` swap; the
ship-loop difference is exactly the `&rnd`-address-vs-value swap.

## 3. New: `capacity`-first × 128 `rnd`-use masks

Variant: move `int capacity = kHangarCapacity[sceneRace];` before
`bool ownHangar = false;`. `rnd` is declared before `spots` (position P0).
Mask bits (1 = use the local `rnd`, 0 = use `Globals::rnd`):

```
0 top nextInt(100)<=29   1 race=nextInt(4)   2 inner nextInt(100)<30
3 pick nextInt(3)        4 int spot=nextInt(capacity)
5 spot=nextInt(capacity) 6 rotation nextInt(300)
```

Shipped mask is `0x29` (bits 0,3,5). Full sweep (128 `link_so` builds,
`/dev/shm/wf212/sweep_capf.log`):

| mask | pct | size | insns | prefix |
|---|---|---|---|---|
| 0x27 / 0x2b / 0x2d / 0x2f / 0x33 / 0x35 / 0x36 / 0x3a / 0x3c / 0x63 / 0x65 / 0x66 / 0x6a / 0x6c / 0x6e / 0x72 / 0x74 | **82.7** | 2268 | 799 | 0x4c |
| 0x29 (shipped mask) | 79.8 | 2276 | 800 | 0x4c |
| 0x7f (all local) | 81.0 | 2264 | 797 | 0x16 |
| 0x00 (all globals) | 77.2 | 2264 | 806 | 0x10 |
| 0x20 | 62.2 | 2264 | 808 | 0x0c |

For comparison, the same mask sweep **without** capacity-first has its unique
optimum at `0x29` = **88.3 / 2252** (wave-101 §2). `capacity`-first is
therefore a strict regression on the shipped shape; it only helps the
all-globals shape (§4).

## 4. `all-globals` + `capacity`-first reproduces the original parts loop

`all-globals` = remove `AbyssEngine::AERandom *rnd = Globals::rnd;` and use
`Globals::rnd` at all three local sites.

| variant | pct | size | insns | prefix | parts loop |
|---|---|---|---|---|---|
| shipped | **88.3** | 2252 | 791 | 0x547 | shipped |
| all-globals | 75.0 | 2228 | 787 | 0x10 | shipped-ish (`k r6`, `hangarPart` spill, but `row r8`, `Canvas sl`, `zero r9`, `j fp`) |
| all-globals + capacity-first | 77.2 | 2264 | 806 | 0x10 | **original** (`row r9`, `Canvas r8`, `zero sl`, `k r6`, `hangarPart` spill, `j sl`, `childCount fp`, `childMesh r9`, `child r4`, `g r5`) |

The all-globals+capacity-first prologue is rotated: `this` → `sp#48`,
canary address → `sp#40` (kept in `r5` until 0xb432e), `&Globals::status` →
`sp#44` (kept in `r4`), whereas the original has `this` in `r6`, the canary
address spilled to `sp#36` immediately, and `&Globals::status` in `sl`
(spilled to `sp#40` only for the parts loop). The 12-byte size regression
(2264 vs 2252) also makes this variant unattractive under the size-exact
metric.

## 5. New tool: extract the real post-RA MIR

`_work`-style `-print-machineinstrs` dumps contain the whole pipeline. For
`Level.cpp` (`/dev/shm/wf212/mi.txt`):

* `# Machine code for function _ZN5Level11createSceneEv` at line 1154295 —
  the pre-greedy MIR (what wave-101 extracted to `pre_greedy.txt`);
* `# After Greedy Register Allocator` at 1156153 — still **virtual**
  (`%528:tgpr` etc.);
* **`# After Virtual Register Rewriter` at 1157996** — **physical registers**
  (`renamable $r8 = t2ADDrs … kHangarMeshes`, `renamable $r6 = t2MOVi 0`
  for `k`, `t2STRi12 killed renamable $r6, %stack.22` for the `k` spill,
  `renamable $r6 = COPY $r0` for `hangarPart`). This is the authoritative
  record of the shipped allocation and should be used instead of the llc
  replay for future RA work.

The wave-101 llc replay recipe is **not faithful**:

```sh
python3 build_mir.py pre_greedy.txt fresh.mir          # strips slot indices, adds CustomRegMask
# insert `define void @_ZN5Level11createSceneEv() { entry: ret void }` into the module
# (otherwise llc does not run the pass pipeline)
/dev/shm/wf110/llvm702build/bin/llc -mtriple=armv7-none-linux-androideabi \
  -run-pass=greedy,virtregrewriter -print-machineinstrs pre8.mir -o - >/dev/null 2>replay9.txt
```

The replay gives `row r7, k r9, g r5, Canvas r8`, not the shipped
`row r8, k r6, g r5, Canvas r9`. Two independent causes:

1. the extracted MIR does not print virtual-register `liveins` (only physreg
   `liveins` are printed), so the MIR parser reconstructs different liveness;
2. `pre8.mir` needs the wave-101 workaround
   `$r0 = t2MOVi 0` / `$r1 = t2MOVi 0` in the `ArrayAdd` tail block in place
   of `$r0 = COPY %111:gpr` / `$r1 = COPY %168:gpr`; without it llc aborts in
   the machine verifier (`%35`/`%57` defs do not dominate uses), and with it
   the liveness of `%111`/`%168` changes.

Use the replay only for *hypothetical* MIR edits, never as ground truth.

## 6. Hand liveness pass over the pre-greedy MIR

A simple backward liveness pass over `pre_greedy.txt` (slot indices as
printed, upward-exposed uses, block-level fixpoint) gives the shipped
parts-loop live-interval sizes in live instruction slots:

| vreg | value | live slots | span |
|---|---|---|---|
| `%529` | `row` | 108 | 3520–6592 |
| `%539` | `pi` constant | 107 | 3552–6592 |
| `%541` | Vector-temp address | 106 | 3568–6592 |
| `%6` | `k` | 105 | 3616–6592 |
| `%10` | `g` | 89 | 4336–6240 |
| `%11` | `hangarPart` | 65 | 4800–6464 |
| `%12` | child count | 41 | 5136–6240 |
| `%18` | `child` | 29 | 5728–6320 |
| `%574` | `j` | 26 | 5152–6240 |
| `%9` | `mesh` | 15 | 4192–4448 |

`k` (105) is larger than `hangarPart` (65), so under the documented
`Prio = (1<<29) + Size` rule `k` should be assigned first and
`hangarPart` should spill — which is exactly the **original** allocation.
The shipped build does the opposite. The likely explanation is that
`pi`/`vecaddr` are rematerializable (they are `tLDRpci`/`t2ADDri` and are
rematerialized in the final code), so the effective queue is
`row` → `k` → `g` → `hangarPart`; the greedy allocator's eviction/split
choice then decides the `k`↔`hangarPart` outcome. Reproducing the original
needs a source shape that changes that queue, not the parts-loop body.

The ship loop has the analogous inversion: `&Globals::rnd` (`%417`, the GOT
address) is live only 3 slots in the shipped IR because the *value*
(`%420`) is the long-lived vreg kept in `r5`; the original keeps the address
in `r8` and rematerializes the value load at every `nextInt` site.

## 7. New source variants measured this pass

All via the official `link_so` + `tools/verify` score. Base = 88.3 / 2252 /
791 / 0x547.

| variant | pct | size | insns | prefix |
|---|---|---|---|---|
| `self_local` (`Level *self = this;`, use for the enemies release) | 88.3 | 2252 | 791 | 0x547 |
| `partcount` (`const int partCount = 4;`) | 88.3 | 2252 | 791 | 0x547 |
| `hangar_out_init` (`hangarPart = nullptr` outside the loop) | 88.3 | 2252 | 791 | 0x547 |
| `g_outside` (`AEGeometry *g = nullptr` outside the loop) | 88.3 | 2252 | 791 | 0x547 |
| `ownhangar_int` (`int ownHangar = 0`) | 88.3 | 2252 | 791 | 0x547 |
| `shipcount_uint` (`unsigned int shipCount`) | 88.3 | 2252 | 791 | 0x547 |
| `capacity_const` (`const int capacity`) | 88.3 | 2252 | 791 | 0x547 |
| `spot_int` (named `int zero = 0` for the `spotUsed` fill) | 88.3 | 2252 | 791 | 0x547 |
| `zero_var` / `zero_var_inside` (`float zero = 0.0f`) | 88.3 | 2252 | 791 | 0x547 |
| `j_unsigned` / `i_unsigned` / `spot_unsigned` | 88.3 | 2252 | 791 | 0x547 |
| `k_outside` (`int k; for (k = 0; …)`) | 88.1 | 2252 | 791 | 0x547 |
| `vec_out_assign` (`Vector rot;` outside, member assigns inside) | 85.3 | 2264 | 793 | 0x4c |
| `flat_hangarmeshes` (`int[40]` + `[sceneRace*4+k]`) | 84.4 | 2256 | 792 | 0x4c |
| `vec_out_ctor` (`const Vector rot(...)` outside) | 83.9 | 2260 | 791 | 0x4c |
| `enemies_local` (`Array<KIPlayer*> *enemies = this->enemies` cache) | 76.6 | 2248 | 797 | 0xe |
| `vec_static_local` (`static const Vector rot(...)`) | 72.6 | 2228 | 778 | 0xa |
| `mask 0x7f` (all `rnd` sites local, no capacity-first) | 83.7 | 2236 | 786 | 0x16 |

## 8. What this pass proves / what remains

**Proved:**

1. `capacity`-first × all 128 `rnd` masks on the shipped shape is a dead end
   (max 82.7, shipped 88.3).
2. `all-globals` + `capacity`-first is the only shape found that reproduces
   the original parts loop exactly, but it rotates the prologue and costs
   12 bytes.
3. The real post-RA MIR is available at `mi.txt` 1157996–1159843; the llc
   replay is unfaithful (missing vreg liveins + the `%111`/`%168` workaround).
4. In the shipped IR `k`'s live interval is larger than `hangarPart`'s, so
   the `k`↔`hangarPart` swap is a greedy eviction/split artifact, not a
   raw-size ordering.

**Remaining lever (unchanged in substance):** a source shape that keeps the
shipped prologue/early allocation (`this` in `r6`, canary address at
`sp#36`, `&Globals::status` in `sl`) while making the ship loop keep
`&Globals::rnd` in `r8` (value rematerialized) and the parts loop keep `k`
in `r6` with `hangarPart` spilled. Equivalently: the original's *global
interval queue* with the shipped prologue. ≈1800 source variants have now
been measured across waves 91–102 without finding it.

## 9. Reproduction

Scratch is `/dev/shm/wf212`:

* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `fscore.py` — official single-function score (imports `tools/verify`);
* `vfun.py tX.json` — apply `[name, [[old,new],…]]` replacements inside
  `createScene`, build `link_so`, score (must be run from `/dev/shm/wf212`);
* `sweep_capf.py <masks…>` — capacity-first × mask sweep;
* `sweep_capf.log` — the 128-mask table from §3;
* `liveint2.py` — the hand liveness/interval-size pass from §6;
* `real_after_vrr.txt` — `mi.txt` lines 1157996–1159843 (real post-RA MIR);
* `pre_greedy.txt`, `build_mir.py`, `pre8.mir`, `replay9.txt` — the
  (unfaithful) llc replay from wave 101.

Build/verify recipe:

```sh
cd /home/debian/GalaxyOnFire2/wt/wf212
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level11createSceneEv
```
