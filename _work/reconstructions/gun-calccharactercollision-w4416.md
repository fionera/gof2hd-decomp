# `Gun::calcCharacterCollision` pass 10: a faithful IR replay, and the exact greedy-allocator
# cascade that keeps `F P H L K` at `this=r4` (w4416)

Worker: w4416. Branch: `wave94-w4416`, base master `f262bd4b`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch: `/dev/shm/w4416/`.
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`.

## 0. Result

No code change landed. The tree already carries the w4229 pass-7 body (`62313153`), which is
still the best measured shape. This pass adds two things the previous note did not have:

1. a **verified-faithful** llc replay of the function (the `gun_strip.ll` used by w4261 was
   stale — it produced `this=r4` for the *current* body, contradicting the real build), and
2. the **exact greedy-allocator trace** for both relocation models, which shows that the
   previous note's "non-PIC reproduces the shipped allocation" claim is only true for the
   `this` register, not for the frame or the stack slots.

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `f262bd4b`) | **4499 / 4499** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4499 / 4499** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0, `RATCHET PASS`.
The target is not byte-exact (`byte: false`, `linked: false`), so the hard counts are unchanged.

## 1. The measurement baseline

`tools/verify/verify.py`'s `score()` is `difflib.SequenceMatcher(autojunk=False).ratio()`.
For the current body that is **90.0 %** with **585 of 650** normalized instructions matched
(`650` vs `650` instructions, `1984` vs `1984` bytes). The 65 unmatched instructions are the
52 hunks catalogued in `gun-calccharactercollision-w4261.md` §2.

Faithful `F P H L K` (the shipped source order, proved by the `cbz` block boundary at
`0x153320` and corroborated by the iOS 1.1.4 `itt ne` P block):

| body | pct | size | matched |
|---|---:|---:|---:|
| `F H K L P` (HEAD) | **90.0** | **1984** | **585/650** |
| `F P H L K` | 64.8 | 1912 | 417/650 (638 insns) |

`F P H L K` emits the shipped statement order and the shipped `explosive`/`td` stack slots,
but `this` lands in `r4` instead of `r9`, which costs 72 bytes of `ldr.w`/`str.w` → `ldr`/`str`
narrowing. It therefore cannot be 1984 B.

## 2. A faithful IR replay (new)

`/dev/shm/w4416/strip.py` reduces the clang `-S -emit-llvm` module to just
`_ZN3Gun22calcCharacterCollisionEv` (all other `define`s become `declare`s, `alias`/`ifunc`
lines are dropped, `, variables: !N` is removed so llvm-7's `llc` can parse the NDK clang
7.0.2 debug metadata). `llc -O2 -relocation-model=pic` on that module now reproduces the
**real** clang `-Oz` allocation for both bodies:

```
F H K L P  PIC   frame 208  this=r9     (real build: this=r9, 1984 B)   <- matches
F H K L P  static frame 208 this=r9
F P H L K  PIC   frame 208  this=r4     (real build: this=r4, 1912 B)   <- matches
F P H L K  static frame 200 this=r9
```

The `gun_strip.ll` shipped with the previous note is stale: `llc -O2 -relocation-model=pic`
on it yields `frame=208 this=r4` for the *current* `F H K L P` body, which the real build does
not do. Every conclusion below is re-derived on the faithful module
(`/dev/shm/w4416/fphlk_strip.ll`, generated from `body_fphlk.txt`).

**Correction to pass 9 §4.** The static replay of the `F P H L K` IR does *not* reproduce the
shipped code: it gives `frame 200` (shipped `208`), `scatter` at `[sp,#24]` (shipped
`[sp,#32]`), `i` in `r8` (shipped `fp`). Only `this=r9` matches. So "the shipped RA is
reachable non-PIC" is true only for the `this` register.

## 3. The exact greedy-allocator cascade (new)

`%307` is the single SSA value `t2ADDri %stack.10` (the shared aggregate temp that the stack
slot colourer placed at `sp+104`):

```
1568B   %307:gprnopc = t2ADDri %stack.10, 0, 14, $noreg, $noreg
```

Its seven uses are exactly the three regions the previous note identified:

| use | source |
|---|---|
| `3696B`, `3792B` | `Gun.cpp:606:38`, `607:39` — `VectorLength(enemyPosition - getPlayer()->getPosition())` |
| `14688B`, `14880B`, `15056B` | `710:31`, `711:17`, `712:34` — `Matrix view`, `MatrixSetTranslation(view,…)`, `TransformSetLocal(…, view)` |
| `16064B`, `16160B` | `727:34`, `727:25` — `*td = enemy->getPosition()` |

### PIC (`-relocation-model=pic`) — `this` stays in `r4`

```
selectOrSplit GPRnopc:%57  … weight:4.079772e-02     ; this
assigning %57 to $r4
selectOrSplit GPRnopc:%11  … weight:2.274136e-02     ; i
assigning %11 to $r5
selectOrSplit GPRnopc:%307 … weight:8.641591e-03     ; &(sp+104)
assigning %307 to $r6
selectOrSplit GPRnopc:%415 … weight:4.111305e-02
evicting $r6 interference: Cascade 3
unassigning %307 from $r6
assigning %415 to $r6
selectOrSplit GPRnopc:%307 … weight:8.641591e-03
assigning %307 to $r11: R11                          ; <-- fp
… %307 is never evicted from r11 again
```

`%307` is one long interval (7 uses spread over the whole loop body). Once it takes `fp` it
pins `i` out of `fp`, and `this` is never evicted from `r4`. The PIC frame is 208, which is
what the shipped code has, but `this=r4` makes the code 1912 B.

### static — `this` is evicted from `r4`

```
selectOrSplit GPR:%454 [15976r,16080r:0) … weight:2.818017e-01   ; scatter-block copy of &(sp+104)
hints: $r0 $r1
missed hint $r0
$r11 is available at cost 1
Only trying the first 9 regs.
should evict: %57 … weight:4.098971e-02
evicting $r4 interference: Cascade 16
unassigning %57 from $r4
assigning %454 to $r4
… assign %57 to $r9 (evicting %11 from $r9) … assign %11 to $r8
```

The live-range splitter breaks `%451` (the static `%307`) into `%451`/`%452`/`%453`/`%454`;
the scatter copy `%454` has weight `2.8e-1` (33× the PIC whole-interval weight) and it prefers
`$r4`, so it evicts `this`. That is the entire difference.

So the residual is a genuine butterfly effect: *one* extra vreg at `bb.0` (the PIC
`tLDRLIT_ga_pcrel @__stack_chk_guard` + `t2LDRi12` pair, which the static model replaces with a
single `t2LDRpci`) changes the greedy allocator's interval splitting 15 000 slots later.

## 4. IR perturbation search, re-run on the faithful module (new)

All of the following keep `frame=208 this=r4` on `fphlk_strip.ll`:

| perturbation | result |
|---|---|
| extra `alloca i32` | 208 / r4 |
| extra `alloca [64 x i8]` | 272 / r4 |
| extra `alloca Matrix` | 264 / r4 |
| dead `add i32 0, 0` | 208 / r4 |
| dead `xor i32 0, 0` | 208 / r4 |
| `store volatile i32 0` to a fresh alloca at `bb.0` | 208 / r4 |
| `store i32 0` / `store i32 1` to a fresh alloca at `bb.0` | 208 / r4 |
| extra `llvm.dbg.value` at `bb.0` | 208 / r4 |

This confirms pass 9 §5 on a module that actually reproduces the real build.

## 5. New source-level experiments (all measured with the official verifier)

Baseline `F H K L P`: **90.0 / 1984 / 585 of 650**.

### 5.1 Drop the `int dmg` local (the shipped code does this)

The shipped damage block reloads `this->damage` at every site — `ldr.w r1,[r9,#96]` in the
plain path (`0x15330a`) and `vldr s0,[r9,#96]` in the two float paths (`0x1532b8`,
`0x1532d4`) — with no CSE. That is the signature of `enemy->damage(this->damage, …)` written
directly, not of a `dmg` local. Removing the local does fix residuals 4 and 5 (`sl` becomes
`&position`, and the plain path reloads damage), but:

| body | pct | size | matched |
|---|---:|---:|---:|
| `nodmg` + `F H K L P` | 84.0 | **1996** | 547/650 (652 insns) |
| `nodmg` + `F P H L K` | 64.1 | 1920 | 414/650 (641 insns) |
| `nodmg` + `F P H K L` | 64.1 | 1920 | 414/650 (641 insns) |

The `else if (enemy->egoFlag)` arm then hoists `ldr.w r1,[r9,#96]` above its `cbz` and adds a
`vmov s0,r1`, which costs more than items 4/5 save, and the size leaves 1984 (→ 1996), which
by itself collapses `byte_exact`.

### 5.2 Scatter-loop temp shapes

| shape | pct | size | matched |
|---|---:|---:|---:|
| `Vector ep = getPosition(); ep -= position; VectorLength(ep)` | 47.9 | 1876 | 307/650 |
| `Vector ep = getPosition(); *td = ep; *td -= position; VectorLength(*td)` | **90.0** | **1984** | **585/650** |
| `*td = getPosition() - position; VectorLength(*td)` | 63.4 | 1964 | 411/650 |

`local_td` is codegen-identical to the baseline in *both* statement orders, so the shared
`sp+104` interval is not reachable from that direction.

### 5.3 `td` shape

`Vector &td = this->targetDir;` (reference) and using `this->targetDir` directly (no pointer
at all) are both **codegen-identical** to the baseline — 90.0 / 1984 / 585.

### 5.4 Loop index types

`int i`, `int e`, `int k` are all codegen-identical (90.0 / 1984 / 585).
`for (int i = 0; i < (int) this->positions.count; ++i)` → 89.8 / 1984 / 584.

### 5.5 Type variants on the `F P H L K` body

`int mine`, `int scatter`, `bool explodeOnContact`, `bool explosive`, `unsigned weaponType`,
`int bomb` — **all** give the identical 64.8 / 1912 / 417 (638 insns). The allocation is
insensitive to every scalar type in the preamble.

### 5.6 `explosive` declaration placement

Declared at its assignment, after `const int weaponType`, or reordered with
`explodeOnContact` — all 90.0 / 1984 / 585 (identical codegen).

### 5.7 `Matrix view` placement (`F P H L K`)

| shape | pct | size | matched |
|---|---:|---:|---:|
| hoisted to function scope | 53.4 | 2008 | 352/650 |
| `Matrix view; view = …;` in place | 63.1 | 2040 | 417/650 |

### 5.8 Within-block reorderings (current body)

| reorder | pct | size | matched |
|---|---:|---:|---:|
| `hitPositions[i] = position` before `hitFlags[i] = 1` | 75.6 | 1964 | 489/650 |
| `impact->explode()` before `impact->isRocket()` | 84.1 | 1980 | 546/650 |
| animation states `1` then `3` | 89.7 | 1984 | 583/650 |

### 5.9 Cosmetic rewrites — all codegen-identical (90.0 / 1984 / 585)

`hitFlags[i] = true`, `field_54[0] = true`, `field_54[1] = true`,
`(int) enemy->getKIPlayer()` (no `intptr_t` hop), `float r = radius;`,
`AnimationMode(3)`, `nullptr` for the `emitManual` arg.

### 5.10 The `__builtin_expect` anchor in the mine block is still required

| mine-block shape | pct | size | matched |
|---|---:|---:|---:|
| `__builtin_expect(cond0, 1) && …` (HEAD) | **90.0** | **1984** | **585/650** |
| plain `cond0 && …` | 84.7 | 1984 | 550/650 (649 insns) |
| `bool hit = cond0 && …; if (hit)` | 84.7 | 1984 | 550/650 (649 insns) |
| `bool hit = __builtin_expect(cond0 && …, 1); if (hit)` | 73.7 | 1996 | 479/650 |

No faithful shape reproduces the shipped out-of-line fail block at `0x1535ea`. The anchor is
therefore still justified and is documented on the same line in `src/game/weapons/Gun.cpp`.

### 5.11 Random order × cached-field × expect × vector-order sweep

The 120-combination sweep that was still running at handoff finished: best **84.7**
(`F H K L P`, no cache, `expect(cond0,0)`, `P V E R`); nothing beat the baseline. The
previously-recorded best of the sweep family remains `F H K L P` with no cached field.

## 6. Conclusion / remaining work

The remaining 10 % is one coupled degree of freedom, now localised to a single value:

* The shipped **source** order is `F P H L K` (proved by the `cbz` at `0x153320`).
* In the PIC build that order makes clang 7's greedy allocator keep the shared `sp+104`
  address as **one long interval** and hand it `fp`; `this` is never evicted from `r4`
  (1912 B, 64.8 %).
* The tree's `F H K L P` order reproduces the shipped `this=r9`/1984 B allocation but emits
  the wrong statement order (90.0 %).
* The static replay of the *same* `F P H L K` IR evicts `this` from `r4` via a high-weight
  scatter-block copy of the same address — but its frame is 200 and its stack slots differ
  from the shipped ones, so it is not the shipped code either.

Remaining levers, in priority order:

1. A source construct that changes the *number of IR values before* the `%307` address
   computation without changing the emitted code. The faithful replay bounds this: the extra
   PIC stack-guard vreg alone is enough, but every dead/extra instruction tried at `bb.0`
   (allocas, stores, `add`/`xor`, `dbg.value`) leaves `this=r4`.
2. A different `Vector`/`Matrix` temp sharing pattern that changes the stack-slot numbering
   (`local`/`local_td`/`expr` all measured above; `local_td` is codegen-identical).
3. Keep 1984 B: any candidate that changes the size shifts every later `.text` symbol and
   fails the hard gate (`nodmg` → 1996 B is the clearest example).

## 7. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
cmake --build cmake-build-match --target lint  -j8        # CLEAN ✓
cmake --build cmake-build-match --target drift -j8        # 0 field(s)

# per-function official verifier
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False

# faithful IR replay (scratch, /dev/shm/w4416)
./mkll.sh body_fphlk.txt fphlk.ll           # clang -Oz -S -emit-llvm (exact build flags)
python3 strip.py fphlk.ll fphlk_strip.ll    # keep only the target define
python3 lab2.py fphlk_strip.ll
# -> pic: frame=208 this=r4 ; static: frame=200 this=r9

/home/debian/llvm7build/build/bin/llc -O2 -relocation-model=pic \
  -debug-only=regalloc -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  fphlk_strip.ll -o /dev/null 2> ra_pic2.txt
/home/debian/llvm7build/build/bin/llc -O2 \
  -debug-only=regalloc -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  fphlk_strip.ll -o /dev/null 2> ra_static2.txt
```

## 8. Files changed

None under `src/` (note only). Gate: byte 4499 / linked 4499, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
lint CLEAN, drift 0.
