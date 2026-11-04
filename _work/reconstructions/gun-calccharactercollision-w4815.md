# `Gun::calcCharacterCollision` pass 13: tail/block sweep, the `__builtin_expect` grid, the
# corrected stack.10 vreg identification, and the `Matrix view` scope experiment (w4815)

Worker: w4815. Branch: `wave98-w4815`, base master `fe4f5e7cf`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base `0x10000`).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 instructions.
Scratch: `/dev/shm/w4815/`.

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail, see §2) is still the best measured
shape: **90.0 %**, 1984/1984 B, 585 of 650 normalized instructions matched (official verifier).

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `fe4f5e7cf`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0, `RATCHET PASS`.

This pass adds four things w4713 did not have:

1. A re-run of the **full 120 tail permutations × 24 block permutations** sweep (2880 bodies),
   confirming HEAD is a strict local optimum.
2. A **`__builtin_expect` grid** over the whole function: the radius-block
   `else if (scatter)` branch is the *only* single knob that restores `this=r9` on the shipped
   `F P H L K` order.
3. A **correction of the w4713 vreg identification**: the `stack.10` temp is `%307` in the
   `F P H L K` trace but `%16`/`%255` in the `F H K L P` trace; the `%466` evictor quoted by
   w4713 is the *scatter-block* copy in the `F H K L P` trace only (in `F P H L K` the same
   value is `%509` and is assigned `$r11`, not `$r4`).
4. A new source-shape probe: hoisting `Matrix view` to function scope reproduces the **shipped
   register map and slot order exactly** (`this=r9`, `enemy=r8`, `explosive=[sp,#20]`,
   `td=[sp,#24]`) but costs +48 B because the tree's `Matrix()` default ctor is non-trivial.

## 1. The two facts that pin the residual (re-verified)

| body | tail source order | pct | size | `this` | `enemy` | `i` | `i*3` | `td` slot | `explosive` slot |
|---|---|---:|---:|---|---|---|---|---|---|
| tree (HEAD) | `F H K L P` | **90.0** | **1984** | r9 | r8 | fp | r4 | [sp,#20] | [sp,#24] |
| shipped order | `F P H L K` | 64.8 | 1912 | r4 | r6 | — | — | [sp,#28] | **[sp,#20]** |
| shipped binary | `F P H L K` | — | 1984 | **r9** | **r8** | **fp** | **r4** | **[sp,#24]** | **[sp,#20]** |

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`, `H` = `setHitVector`,
`L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

The shipped instruction order is `F P H L K` (the `movs r1,#1` at `0x153316` is shared by `F`
and `P`, and `P`'s store at `0x153326` precedes the `setHitVector` call at `0x15333c`). The
shipped *register* map is the tree's `F H K L P` map (`this=r9`, `enemy=r8`, `i=fp`, `i*3=r4`),
with the `td`/`explosive` spill slots swapped. So the whole residual is: *make the `F P H L K`
source order allocate like `F H K L P`.*

## 2. Exhaustive tail × block sweep (re-run)

All 120 permutations of `{F,H,K,L,P}` × all 24 permutations of the four post-damage blocks
`{T(tail), X(explosive/geometries), I(impact), S(scatter)}` = 2880 bodies, object oracle.
The oracle reproduces the official 90.0 to the digit.

| rank | tail | block order | pct | size | `this`/`enemy` |
|---|---|---|---:|---:|---|
| 1 | `F H K L P` (**tree**) | `T X I S` | **90.0** | 1984 | r9 / r8 |
| 2 | `F K H L P` / `K F H L P` | `T X I S` | 89.8 | 1984 | r9 / r8 |
| 4 | `F H P L K` | `I T X S` | 88.7 | 1984 | r9 / r8 |
| 5 | `F K P H L` | `I T X S` | 88.5 | 1984 | r9 / r8 |
| — | `F P H L K` (**shipped**) | any | ≤ 64.8 | 1912 | r4 / r6 |

There are exactly three allocation families:

* `this=r9, enemy=r8, i=fp, i3=r4` — only when `L` and `P` are the **last two statements, in
  that order**, with `F` before `H`/`K` and `H`,`K` in the two slots before them
  (`F H K L P`, `F K H L P`, `K F H L P`). 1984 B, 84.8–90.0 %.
* `this=r9, enemy=r6, i` unassigned — 1964–1968 B, 75–76 %.
* `this=r4, enemy=r6, i` unassigned — 1912–1924 B, 64.x %.

**No permutation of the five tail statements produces the shipped `this=r9` + `td=[sp,#24]` +
`explosive=[sp,#20]` combination.**

### 2.1 Why `F H K L P` is cheap and `F P H L K` is not

In `F H K L P` the constant `1` used by both `F` and `P` is live across the `H` and `K` calls,
so it takes a callee-saved register (`fp`), pushing `i` out of `fp`. In `F P H L K` the constant
is only live across the `P` branch, so it takes `r1` and `fp` stays free for `i`. That is the
`movs r1,#1` / `mov.w fp,#1` difference visible in the two builds. It is also why the shipped
`F P H L K` body is 12 instructions shorter than the tree's `F H K L P` body (638 vs 650).

## 3. The `__builtin_expect` grid (new)

The tree already carries one `__builtin_expect` anchor in the mine block
(`Gun.cpp:659`, `// lint: arm_builtin anchor`). Grid over every branch in the function with
`{none, 0, 1}`, on the shipped `F P H L K` order:

| expect site | value | pct | size | `this` | `enemy` | slots |
|---|---:|---:|---:|---|---|---|
| radius `else if (scatter)` | 1 | **79.0** | 1976 | **r9** | r6 | [(r2,20),(r2,24)] |
| radius `else if (scatter)` + `emp` | 0 | 79.4 | 1980 | r9 | r6 | [(r2,20),(r2,24)] |
| radius `else if (scatter)` + `impact` | 0 | 79.0 | **1984** | r9 | r6 | [(r2,20),(r2,24)] |
| radius `else if (scatter)` + `pg` | 0 | 79.0 | **1984** | r9 | r6 | [(r2,20),(r2,24)] |
| radius `else if (scatter)` + `geometries` | 0 | 76.2 | 1988 | r9 | **r8** | [(r2,24),(r1,20)] |
| radius `else if (scatter)` + tail `explosive` | 1 | 75.5 | 1992 | r9 | **r8** | [(r2,24),(r1,20)] |
| radius `else if (scatter)` + tail `scatter` | 1 | 59.9 | 1888 | r4 | r6 | [(r2,20),(r2,24)] |
| radius `else if (scatter)` + `explodeOnContact` | 1 | 58.1 | 2036 | r9 | — | [(r0,24),(r1,20)] |

`__builtin_expect(scatter, 1)` on the **radius-block** `else if (scatter)` is the only single
knob that restores `this=r9` on `F P H L K`. It gives the shipped's `explosive=[sp,#20]` slot
but leaves `td` in `r8` (the shipped has `td=r6`) and is 8 B short.

On the tree's `F H K L P` order the same knob is a regression: `82.6 %`, 1988 B.

A 300-sample randomised search of 1–3 edits (40-knob pool: types, spellings, `dmg` on/off,
`Array::operator[]` vs `.data()`, scatter-loop forms, falloff clamp forms, …) on the
`expect(scatter,1)` base tops out at **79.8 % / 1976 B** — nothing beats HEAD.

## 4. The LLVM-7 allocator trace, corrected

The replay is faithful: `llc -O2 -relocation-model=pic` on the clang-7 text IR reproduces the
clang object exactly (1912 B, `mov r4, r0` for `F P H L K`); the default static model reproduces
the shipped allocation (1948 B, `mov r9, r0`). Traces:
`/dev/shm/w4815/ra_a_pic.txt` (`F H K L P`), `/dev/shm/w4815/ra_b_pic.txt` (`F P H L K`).

### 4.1 The `stack.10` vreg is `%307` in B and `%16`/`%255` in A

w4713 quoted `%307` for both traces. That is wrong: `%307` in the `F P H L K` trace is the
`stack.10` address (live `[1568r,7040B)[9360B,9424B)[11136B,11200B)[11296B,17536B)[17568B,17680B)`),
but in the `F H K L P` trace the same shape is `%16` (`[2672r,2736B:1)[2736B,7040B)…`) and
`%255` (`[1520r,7040B)…`). The two traces are shifted by 2 in the high vreg numbers because the
tail swap changes the IR value ordering.

The `%466` evictor quoted by w4713 is real, but it is the **scatter-block** copy in the
`F H K L P` trace only:

```
A (F H K L P):  %466 -> $r4   defined 16056B  (Gun_var.cpp:727:34, scatter getPosition temp)
B (F P H L K):  %509 -> $r11  defined 6824B/9384B/14984B/16064B (one long rematerialised vreg)
```

In A, `%466` is a short high-weight copy that evicts `this` from `$r4`, so `this` cascades to
`$r9` (the shipped map). In B, the `stack.10` address stays in `$r11` across all its uses, so no
evictor is created and `this` keeps `$r4`.

### 4.2 The split itself

Both builds split `stack.10`; the partition differs:

```
B:  Best split candidate of vreg %307  may not cause bad eviction chain
    Split for $r11 in 24 bundles, intv 1.
    splitAroundRegion with 2 globals.
    %bb.92 [16000B;16480B) intf invalid-invalid, live-through 1 -> 1, straight through.
```

The `$r11` candidate cost is `static = 21.32312512, v=69, total = 98.31180978` in B and
`total = 110.4083111` in A. The difference is the **edge-bundle set**: A carries
`EB#26 EB#27 EB#39 EB#40 EB#46`, B carries `EB#34–#38` instead. Those edge bundles are a
function of the CFG block layout, i.e. of the `P` block position — not of statement spelling.

### 4.3 The shipped has the `F H K L P` split, not the `F P H L K` split

The shipped binary has exactly **5** `add rX, sp, #104` sites (`0x1530dc`, `0x1533c8`,
`0x153444`, `0x1535f8`, `0x153626`), all 16-bit `add`, never `add.w r11` — i.e. the
`stack.10` address is rematerialised per region, exactly as in the tree's `F H K L P` build
(also 5 sites, all 16-bit). The `F P H L K` build has 8 sites, including three
`add.w fp, sp, #104` (the address kept in `fp` across regions).

So the shipped is `F P H L K` **statement order** with `F H K L P` **region splitting**.

## 5. New source-shape probes

### 5.1 Hoisting `Matrix view` reproduces the shipped map — at +48 B

Declaring `Matrix view;` at function scope (instead of inside the `else if (geometries)` block)
and assigning it there:

```
    Vector *td = &this->targetDir;
    Matrix view;
    ...
        } else if (this->geometries != nullptr) {
            view = *(Matrix *) Globals::Canvas->CameraGetLocal(...);
```

gives **`this=r9`, `enemy=r8`, `explosive=[sp,#20]`, `td=[sp,#24]`** — the shipped register map
*and* the shipped slot order — but **2032 B / 64.2 %**. The +48 B is the tree's `Matrix()`
default ctor (identity matrix + `scaleVector()` call), which the shipped prologue does not
contain. The stack frame also grows 208 → 224 because the hoisted `Matrix` no longer shares the
`stack.10` slot with the `getPosition` temps.

This is the strongest evidence that the residual is the **`stack.10` live range**, not any
individual statement.

### 5.2 The radius-block `expect(scatter,1)` variant (`sc2`)

`F P H L K` + `__builtin_expect(scatter, 1)` on the radius block: `this=r9`, `td=r8`,
`enemy=r6`, `explosive=[sp,#20]` (correct), `td=[sp,#28]`. It is the shipped map with
`td`/`enemy` swapped and the `td`/`e` slots swapped. 1976 B / 79.0 %.

### 5.3 Other probes (all ≤ HEAD)

| probe | pct | size | `this`/`enemy` |
|---|---:|---:|---|
| `td` direct member (`this->targetDir` in scatter loop) | 64.8 | 1912 | r4 / r6 |
| `Vector &td = this->targetDir` reference | 64.8 | 1912 | r4 / r6 |
| `td` local copy + write-back | 65.9 | 1984 | — / r8 |
| `Matrix view;` decl-then-assign inside block | 63.1 | 2040 | r9 / r6 |
| `Matrix view;` at top of `else if (geometries)` | 62.4 | 2040 | r9 / r6 |
| hoist `Vector enemyPosition` | 52.4 | 1948 | — |
| hoist `Vector position` | 63.3 | 1908 | r4 / r6 |
| hoist `Vector velocity` | 64.2 | 1956 | r9 / — |
| hoist `KIPlayer *ki` | 64.8 | 1912 | r4 / r6 |
| hoist `int radius` | 64.8 | 1912 | r4 / r6 |
| `int explosive;` moved after `explodeOnContact` | 90.0 | 1984 | r9 / r8 |
| `int explosive = 0;` | 90.0 | 1984 | r9 / r8 |
| `td` declared before `lastHitKIPlayer = 0` | 90.0 | 1984 | r9 / r8 |
| `int dmg;` at function scope | 84.0 | 1996 | r9 / r8 |

The three 90.0 rows are codegen-identical to HEAD (same bytes, same slots).

### 5.4 Non-PIC confirmation

`F P H L K` with `-fno-pic`: **1948 B / 68.0 %**, `this=r9`, `enemy=r8` — the shipped register
map, but the frame is 200 (not 208) and the GOT prologue is missing. So the shipped is
"non-PIC allocation + PIC addressing", which is exactly the combination the PIC allocator fails
to reproduce.

## 6. Conclusion and the remaining lever

The residual is a **PIC-vs-non-PIC register-allocation lottery**, not a statement-order problem:

* the shipped source order is `F P H L K` (proved by the shared `movs r1,#1` and by the iOS
  1.1.4 build);
* compiling that exact order **non-PIC** reproduces the shipped allocation;
* compiling it **PIC** keeps `this` in `r4` because the region splitter folds the scatter
  block's `stack.10` use into the main interval, so the high-weight evictor that moves `this`
  to `r9` is never materialised.

The remaining lever is a source construct that changes the **PIC region-split cost model around
`stack.10`** so that its scatter use becomes its own interval, **without changing the emitted
instruction count**. The `Matrix view` scope experiment (§5.1) shows the lever exists and that
it is the `stack.10` live range; the blocker is that every shape which flips the split either
adds the `Matrix()` ctor (+48 B) or changes the frame size.

## 7. Reproduction

```
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

Scratch harness (`/dev/shm/w4815/`):

| file | purpose |
|---|---|
| `mkvar.py` | splice a body into `Gun_base.cpp`, compile with the exact `compile_commands.json` clang line to `Gun_var.o` |
| `score2.py` | object-level oracle: disassemble `.text.<sym>` from a `.o`, normalise external `bl/blx` to `call #X`, `SequenceMatcher` ratio vs the original; reproduces the official 90.0 exactly |
| `sig2.py` / `sig_s.py` | extract `{size, this, enemy, slots, n}` from a `.o` / `.s` |
| `tail.py` | splice any permutation of the five tail statements (`FHKLP` default = HEAD) |
| `blocks.py` | permute the four post-damage blocks `T/X/I/S` |
| `pool.py` | ~35 named single-edit knobs |
| `diffs.py` | opcode-level diff vs the original (normalises branch targets and external calls) |
| `sweep_exp.py` | tail × block × `expect(scatter,1)` sweep |
| `grid2.py` | `expect` grid on the shipped order |
| `rand2.py` | 300-sample randomised multi-knob search |
| `variants1..5.py` | `td`/`Matrix view`/loop-variable scope probes |
| `ra_a_pic.txt` / `ra_b_pic.txt` | full `-debug-only=regalloc` traces for `F H K L P` / `F P H L K` |
| `bp_a.txt` / `bp_b.txt` | block-placement traces |

## 8. Files changed

None under `src/` (note only). Gate: byte 4500 / linked 4500, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
lint CLEAN, drift 0.
