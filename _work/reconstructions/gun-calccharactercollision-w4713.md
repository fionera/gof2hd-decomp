# `Gun::calcCharacterCollision` pass 12: the clang-7.0.2 greedy-allocator trace, the exact
# non-PIC eviction that the PIC build misses, and why no tail order reaches 1984 B (w4713)

Worker: w4713. Branch: `wave97-w4713`, base master `9b58a39d5`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base `0x10000`).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 instructions.
Scratch: `/dev/shm/w4713/`.

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail, see §2) is still the best measured
shape: **90.0 %**, 1984/1984 B, 585 of 650 normalized instructions matched (official verifier).

This pass adds the thing every earlier pass lacked: an **actual clang-7.0.2 greedy-register-
allocator trace** for this function, obtained with the LLVM 7 build that already exists on this
machine (`/home/debian/llvm7build/build/bin/llc`, "LLVM version 7.0.0, Optimized build with
assertions", `-debug-only=regalloc`). Earlier notes (w4261) had to use `llc` from a *different*
LLVM, so their `%454`/`stack.10` story could not be checked against the real allocator. It now
can, and it holds — with one important correction (§4).

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `9b58a39d5`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0, `RATCHET PASS`.

## 1. The two facts that pin the residual

Re-measured this pass (object-level oracle `score.py`, which reproduces the official 90.0 to the
digit):

| body | tail source order | pct | size | `this` | `enemy` | `i` | `i*3` | `td` slot | `explosive` slot |
|---|---|---:|---:|---|---|---|---|---|---|
| tree (HEAD) | `F H K L P` | **90.0** | **1984** | r9 | r8 | fp | r4 | [sp,#20] | [sp,#24] |
| shipped order | `F P H L K` | 64.8 | 1912 | r4 | r6 | r8 | — | r9 (unspilled) | **[sp,#20]** |
| shipped binary | `F P H L K` | — | 1984 | **r9** | **r8** | **fp** | **r4** | **[sp,#24]** | **[sp,#20]** |

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`, `H` = `setHitVector`,
`L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

So the shipped code is **the tree's `F H K L P` allocation with the shipped `F P H L K` tail
order**. The whole residual is: *make the `F P H L K` source order allocate like `F H K L P`.*

## 2. Exhaustive tail-order search (re-run, now with `dmg` on/off)

All 120 permutations of `{F,H,K,L,P}` × `{int dmg` local, no local`}` = 240 bodies, object
oracle. Full table in `/dev/shm/w4713/perm120b.json`.

| rank | tail | `dmg` local | pct | size | `this`/`enemy` |
|---|---|---:|---:|---:|---|
| 1 | `F H K L P` (**tree**) | yes | **90.0** | 1984 | r9 / r8 |
| 2 | `F K H L P` / `K F H L P` | yes | 89.8 | 1984 | r9 / r8 |
| 4 | `F H P L K` | yes | 85.3 | 1976 | r9 / r8 |
| 5 | `P H K F L` | yes | 85.3 | 1980 | r9 / r8 |
| — | `F H P L K` | no | 84.7 | 1992 | r9 / r8 |
| — | `F H K L P` | no | 84.0 | 1996 | r9 / r8 |
| — | `F P H L K` (**shipped**) | yes | **64.8** | **1912** | **r4 / r6** |

There are exactly three allocation families:

* `this=r9, enemy=r8, i=fp, i3=r4` — only when `L` and `P` are the **last two statements, in
  that order**, with `F` before `H`/`K` and `H`,`K` in the two slots before them
  (`F H K L P`, `F K H L P`, `K F H L P`). 1984 B, 84.8–90.0 %.
* `this=r9, enemy=r6, i` unassigned — 1964–1968 B, 75–76 %.
* `this=r4, enemy=r6, i` unassigned — 1912–1924 B, 64.x %.

`F P H L K` is in the third family. **No permutation of the five tail statements produces the
shipped `this=r9` + `td=[sp,#24]` + `explosive=[sp,#20]` combination.**

## 3. The real clang-7.0.2 allocator trace (new)

Reproduce the exact front-end IR and feed it to the LLVM 7 `llc` (the NDK's `clang -S -emit-llvm`
already emits **text** IR here):

```
# in /dev/shm/w4713
clang++ <exact compile_commands.json command, -S -emit-llvm> -o x.ll   # text IR
sed -E 's/, variables: ![0-9]+//g' x.ll > x_strip.ll
/home/debian/llvm7build/build/bin/llc -O2 -relocation-model=pic -filetype=asm x_strip.ll -o x.s
/home/debian/llvm7build/build/bin/llc -O2 -relocation-model=pic -filetype=asm x_strip.ll \
        -o /dev/null -debug-only=regalloc 2> ra7_pic.txt
```

`llc -O2` and `llc -Oz` agree; `-relocation-model=pic` reproduces the clang object exactly
(`mov r4, r0`, 1912 B), and the default (static) model reproduces the **shipped** allocation
(`mov r9, r0`). So the replay is faithful.

### 3.1 PIC build of `F P H L K`: `this` is never evicted

```
selectOrSplit GPRnopc:%57 [16r,8016r:0)[8368B,9008r:0)...  weight:4.079772e-02
hints: $r0 $r0 $r0
missed hint $r0
assigning %57 to $r4: R4 ...
```

`%57` is `this` (the `COPY $r0` in `bb.0`). It keeps `$r4` for the whole function — there is no
`unassigning %57` anywhere in this function's trace. That is the 1912 B body.

### 3.2 Non-PIC build of `F P H L K`: `%454` evicts `this`

```
selectOrSplit GPR:%454 [15976r,16080r:0)  0@15976r weight:2.818017e-01 w=2.818017e-01
hints: $r0 $r1
missed hint $r0
$r11 is available at cost 1
should evict: %57 [16r,7984r:0)...  weight:4.098971e-02
evicting $r4 interference: Cascade 16
unassigning %57 from $r4: R4
assigning %454 to $r4: R4 [15976r,16080r:0)  0@15976r
...
selectOrSplit GPRnopc:%57 ...
hints: $r0 $r0 $r0 $r9
RS_Assign Cascade 16
should evict: %11 ...                       # %11 = td
evicting $r9 interference: Cascade 16
unassigning %11 from $r9: R9
assigning %57 to $r9: R9 ...
...
assigning %11 to $r8: R8 ...               # td -> r8, then spilled
```

`%454` is a **short, high-weight copy of the `stack.10` address** created for the scatter block
(`Gun_exp.cpp:727`, `*td = enemy->getPosition();`). Its weight `2.818e-01` is ~7× `this`'s
`4.1e-02`, so the greedy allocator evicts `this` from `$r4`, and the whole shipped cascade
(`this→r9`, `td→r8→spill`, `explosive`/`td` slot swap) follows.

### 3.3 Why PIC has no `%454`

Both builds split the `stack.10` address (`%307` in PIC, `%305` in non-PIC; they differ by only
**2** vregs — the PIC GOT-load temporaries). The split itself happens in both:

```
selectOrSplit GPRnopc:%307 ...   RS_Split Cascade 9
Split for $r11 in 24 bundles, intv 1.
```

but the **partition** differs. In PIC the scatter block is absorbed into the main interval:

```
%bb.92 [16000B;16480B) intf invalid-invalid, live-through 1 -> 1, straight through.
    selectIntv 1 -> 1
```

so no short high-weight scatter copy is ever created; the `stack.10` address stays in `$r11`
(`fp`) and `i` is pushed out instead. In non-PIC the same block becomes its own interval
(`useIntv [15976r;16080r):3`) because the region-split cost model ranks `$r11` differently —
the decisive line is the bundle weight:

```
PIC     $r11  static = 21.32312512, v=69, total =  98.31180978 with bundles EB#1 ... EB#46.
non-PIC $r11  static = 21.32312512, v=62, total = 108.5182329  with bundles EB#1 ... EB#46.
```

PIC has 7 extra `$r11` bundles (the GOT loads in `bb.0`/`bb.8`), which lowers the split cost
enough that the allocator keeps the scatter use in the main interval.

**Correction to w4261 §4:** w4261 said "the PIC build keeps `stack.10` as *one long interval*".
That is not what happens — PIC *does* split it (into 2 intervals, not 4); what PIC loses is the
*scatter-block copy*, i.e. the high-weight interval that does the eviction.

## 4. Everything tried this pass that does **not** flip the PIC allocation

All measured with the object oracle; "identical" means byte-identical codegen to HEAD.

### 4.1 Coalesceable extra copies (the w4261 "one extra vreg" lever)

Inserting a redundant local initialised from another local, for every value in the function
(`enemy`, `ki`, `td`, `this`, `level`, `weaponType`, `position`, `radius`, `explosive`, `e`, `i`,
`velocity`, `mine`, `scatter`) is **coalesced away**: all 15 variants are codegen-identical
(1912 / r4 for `F P H L K`). So an extra *copy* does not shift the vreg numbering seen by RA.

### 4.2 Type / spelling knobs (all codegen-identical on both orders)

`bool explosive`, `bool explodeOnContact`, `int mine`, `int scatter`, `int bomb`, `int weaponType`,
`int e/i/k`, `float r = radius`, `explosive = (...) ? 1 : 0`, `explodeOnContact = bomb || mine`,
`radius` via if instead of `?:`, `bool outer = ki && ...`, `hitFlags[i] = true`,
`field_54[0] = true`, `field_54[1] = true`, `if (impact)`, `if (geometries)`,
`if (playerGun != 0)`, `if (explosive != 0)`, `if (mine != 0)`, `if (scatter != 0)`,
`if (isAsteroid() != 0)`, `if (explodeOnContact != 0)`, `if (empDamage > 0)`,
`if (getHitpoints() <= 0)`, `if (!isActive())`.

### 4.3 Positional knobs

* `td` declaration moved to 9 different anchors (top, after `explodeOnContact`, after the
  `enemies == nullptr` guard, after `weaponType`, after `bomb`, after `mine`, after `scatter`,
  after the `explosive =` assignment, after `if (bomb||mine)`). On `F P H L K`: 1912/r4 for the
  first six anchors, 1916/r4 for the last three. On `F H K L P`: 1964/65.3 for the first four
  anchors, 1968/66.2 for the next three, and 1984 only for the last two (after `explosive =` /
  after `if (bomb||mine)`), i.e. only the tree's own position keeps the shipped allocation.
* `explosive = ...` moved after the `td` declaration: `F H K L P` → 1968/70.2 (worse).
* Block order `{X(explosive/geometries), I(impact), S(scatter)}`: all 6 permutations measured on
  both orders. `X I S` is best; on `F P H L K` every permutation stays ≤ 64.8.
* Scatter-loop shapes (`*td = getPosition(); *td -= position;` vs `Vector ep = ...` vs
  `*td = getPosition() - position` vs named `dir`): `F P H L K` ≤ 64.8 (and the `ep`/`dir` forms
  change the size), `F H K L P` ≤ 90.0. The `getpos_td` spelling
  (`Vector ep = enemy->getPosition(); *td = ep; *td -= position;`) is codegen-identical.

### 4.4 A 160-sample randomised multi-knob search

Random combinations of 1–3 edits from a 30-entry pool × 6 tail orders: best is still
**90.0 / 1984 / `F H K L P`**. Nothing beats HEAD.

### 4.5 The `dmg` local is an anchor, and removing it is not the answer

The shipped binary reloads `this->damage` at all three damage sites
(`0x1532b8`, `0x1532d4`, `0x153308`), so the original source has **no** `int dmg` local. The tree
has one (it is what keeps `sl = &position`, matching the shipped `add.w sl, sp, #192`). Removing
it on `F H K L P` gives 1996/84.0 (a duplicated plain-damage block, +12 B); on `F P H L K` it
gives 1920/64.1. Either way it is worse, so the anchor stays (documented in `Gun.cpp:682`).

## 5. Conclusion and the remaining lever

The residual is a **PIC-vs-non-PIC register-allocation lottery**, not a statement-order problem:

* the shipped source order is `F P H L K` (proved by the `movs r1,#1` shared between `F` and `P`
  before the `H`/`K` calls, and by the iOS 1.1.4 build);
* compiling that exact source order **non-PIC** reproduces the shipped allocation
  (`mov r9, r0`, `enemy=r8`, `i=fp`, `i3=r4`, `td=[sp,#24]`, `explosive=[sp,#20]`) — measured
  here with `-fno-pic`: 1948 B, 68.0 %, correct register map, only the frame size (200 vs 208)
  and the GOT prologue differ;
* compiling it **PIC** (which the shipped `.so` is) keeps `this` in `r4` because the region
  splitter folds the scatter block's `stack.10` use into the main interval, so the high-weight
  `%454` copy that evicts `this` is never materialised.

The remaining lever is therefore exactly w4261's item 1, now with a precise target: **a source
construct that changes the PIC region-split cost model around `bb.92`** (the scatter block) so
that the scatter use of the shared `Vector` return temp becomes its own interval, **without
changing the emitted instruction count**. Every source-level knob tried in this pass and in
w4261/w4416/w4516 either leaves the codegen identical or changes the size away from 1984.

Note that the *shipped* `explosive`/`td` slot swap is a **downstream consequence** of the same
event: once `%454` evicts `this`, `td` is spilled after `explosive`, which is what puts `td` at
`[sp,#24]`. Fixing the slots on `F H K L P` in isolation would be worth only +0.9 pt and would
still not be byte-exact.

## 6. Reproduction

```
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

Scratch harness (`/dev/shm/w4713/`):

| file | purpose |
|---|---|
| `score.py` | object-level oracle: disassemble `.text.<sym>` from a `.o`, normalise external `bl/blx` to `call #X`, `SequenceMatcher` ratio vs the original. Reproduces the official 90.0 exactly. |
| `mkvar.py` | splice the five tail statements in any order into `Gun_exp.cpp`, compile with the exact `compile_commands.json` line to `/dev/shm/w4713/Gun_exp.o`. |
| `sweep.py`, `sweep2.py` | tail order + textual replacements; `sweep2` also reorders the `X`/`I`/`S` blocks. |
| `v2.py` | tail order × `dmg` local on/off × mine-`expect` on/off. |
| `perm120.py`, `perm120b.py` | exhaustive 120-permutation tail sweeps (with/without `dmg`). |
| `knobs.py`, `knobs2.py`, `tdpos.py`, `explpos.py`, `scat.py`, `extra.py`, `rand.py` | the knob families in §4. |
| `sig.py` | extract `{size, frame, this, enemy, i, i3, td_slot, explosive_slot}` from a `.o`. |
| `ratrace.py`, `irpert.py` | clang-7 IR generation and `llc` replay / IR perturbation. |
| `ra7_fphlk_pic.txt`, `ra7_fphlk_static.txt` | the real clang-7.0.2 greedy-allocator traces quoted in §3. |

## 7. Files changed

None under `src/` (note only). Gate: byte 4500 / linked 4500, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
lint CLEAN, drift 0.
