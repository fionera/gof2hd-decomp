# PlayerEgo::calcCollision — wave 119 (w6926): the visible-arm shuffle is
# reproducible but costs 8 B; landed 95.50 % stays

**Status:** analysis only, no source change landed, 2026-11-06.
Worktree `wt/w6926`, branch `wave119-w6926`, base `dfe78cca9` (current
master, wave 119; includes w6905's real `i == 0` tail). Extends
`playerego-calccollision-wave119-w6905.md` (95.53 %), `-w6738.md`
(94.37 %) and `-w6732.md` (95.24 %).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* **No improvement found.** The landed body still measures
  **95.500000 %** (`347/346` normalised instructions, **968 B**).
  `check` = **RATCHET PASS**: byte_exact **4505**, linked_exact
  **4505**, size-exact 4524, data_exact 843/843, imports 207, lint
  **CLEAN**, drift 0.
* The main new result: the original's **visible-arm register shuffle is
  reproducible** by moving the redundant `objects->empty()` guard from
  *after* `projectCollisionOnSurface` to *before* it (anywhere between
  the steal arm and the `getPosition` call).  The resulting code is
  **instruction-for-instruction identical to the original in the whole
  visible arm**, including
  `mov sl,r5 / mov r5,r6 / add r6,sp,#96 / … / mov r6,r5 / mov r5,sl /
  mov r1,r6`.
* But that shape is **976 B**, not 968 B, and the 8 B growth shifts
  every branch target and both literal-pool offsets, dropping the score
  to **89.2 %**.  The 8 B are accounted for exactly (see §3); every
  attempt to pay them back failed.
* ~220 builds this wave.  Nothing at 968 B beats 95.50 %.

## 1. Current landed body (unchanged)

The w6905 body: faithful control flow + two redundant guards
(`objects->size() == i` in the wormhole arm, `objects->empty()` after
`projectCollisionOnSurface`) + the real `i == 0` tail storing
`object->field_0x71` (which drops the original's `movs r0,#1`, buying
the 2 B that keep the function at 968 B).

Remaining delta to the original (16 instructions):

1. A2 guard `ldr r0,[r4,#0]; cmp fp,r0; beq.w` (3 insns) and the six
   branch-target / pool-offset shifts it causes (`bne.n`/`beq.n` to
   `0x178`→`0x180`, `beq.n 0x22a`→`0x232`, `cbnz 0x1a4`→`0x1ac`,
   `cbz 0x1ae`→`0x1b6`, `add r1,pc,#436`→`#428`).
2. SZ guard `ldr r0,[r4,#0]; cbz r0` (2 insns).
3. Visible-arm register shuffle (8 insns net).
4. The dropped `movs r0,#1` in the tail (1 insn).

## 2. The shuffle is reproducible (new this wave)

Moving the SZ guard to just before the `projectCollisionOnSurface`
call (i.e. after `if (!object->isVisible()) continue;`) makes greedy RA
keep `&collision` in `r6` across the call and move `&surface` to `r5`,
exactly like the original:

```
9f300: ldr r1,[r5,#8]          ; this->geometry
9f302: mov r0,r6               ; sret = &surface (sp+32)
9f304: blx getPosition
9f308: ldr r0,[r8]             ; vtable
9f30c: mov sl,r5               ; sl = this          <-- shuffle
9f30e: mov r5,r6               ; r5 = &surface      <-- shuffle
9f310: add r6,sp,#96           ; r6 = &collision    <-- shuffle
9f312: mov r1,r8
9f314: mov r2,r5
9f316: ldr r3,[r0,#88]
9f318: mov r0,r6
9f31a: blx r3
9f31c: movs r0,#0
9f31e: strd r0,r0,[sp,#32]     ; zero temp reuses surface slot
9f322: str r0,[sp,#40]
9f324: mov r0,r6               ; &collision
9f326: mov r6,r5               ; r6 = &surface      <-- shuffle
9f328: mov r5,sl               ; r5 = this          <-- shuffle
9f32a: mov r1,r6
9f32c: blx ne
```

This matches the original byte-for-byte (the original's `mov r0,r6`
sits two slots earlier, before the `strd`; same code otherwise).
The guard itself is the only extra code in the arm.  It also requires
`__builtin_expect(isShrinking() || miningGame != 0, 1)` on the wormhole
continue to keep the un-rotated loop/register file (w6738's lever).

The guard can be placed anywhere from just after the steal arm to just
before the `getPosition` call; all positions produce the same code.

## 3. Why it cannot be landed at 968 B

The original's visible arm is **10 B** larger than the landed one
(shuffle = `mov sl,r5; mov r5,r6; add r6,sp,#96` = 8 B plus
`mov r6,r5; mov r5,sl; mov r1,r6` = 6 B, minus our two rematerialised
`add r0/add r1,sp,#96` = 4 B), and it additionally has the `movs r0,#1`
(2 B).  Total original extra = **12 B**, exactly the size of the landed
shape's two guards (8 B A2 + 4 B SZ).  That is why both compile to
932 B of code / 968 B total.

The shuffle shape needs, in addition:

| item | bytes |
|---|---|
| guard `ldr r0,[r4,#0]; cmp r0,#0; beq.n` (target `0x38c` is 134 B away, `cbz` cannot reach) | +6 |
| extra `str.w r9,[sp,#20]` in the wormhole arm (dock pointer is no longer spilled in the prologue; the RA sinks the spill to `9f1c4`) | +4 |
| missing `movs r0,#1` | −2 |
| **net** | **+8** |

Measured: 976 B (no A2 guard) / 980 B (A2 + moved SZ).  Both shift the
whole tail by 8 B, so every branch target and both pool offsets
mismatch: **89.2 %** (real compare order) / 88.4 % (landed compare
order), i.e. far below the landed 95.50 %.

To land the shuffle at 968 B one would need to remove the 6 B guard
(impossible — it *is* the block split that produces the shuffle) or
remove the 4 B extra store and shrink the guard to 2 B (a `cbz` whose
target is ≤126 B away; the visible arm is 134 B long, so no target
after it is in range).

## 4. Measurements (all 968 B unless stated; pct = verify.py)

```
landed (current)                                      95.500000  347/346
landed, compare `collision != Vector{}` (real order)  95.200000
landed, tail `= 1`                                    87.800000  972
landed, tail `= 1`, real compare                      87.500000  972
landed, tail `!= 0` / `!!f71` / `(bool)f71` / `?: 1:0` 87.8      972
landed, tail nested `if (i==0){if(f71)...}`           95.500000
landed, tail `f71 && i==0`                            95.500000
landed, top store `= f71` + tail `= 1`                94.700000
landed, top store `= f71` + tail `= f71`              87.700000
landed, `const Vector collision`                      95.500000
landed, `Vector &rc = collision` alias                95.500000
landed, `Vector collision(...)` ctor form             95.500000
landed, outerCollide(getPosition()) inline            95.500000
landed, A2 = `size()==0` / `empty()` / `size()<=i`    95.500000
landed, A2 merged into isShrinking `||`               95.500000
landed, A2 moved to loop top / after null             66.3      976
landed, A2 before surface block                       67.1      960
landed, A2 after surface block                        80.3      968
landed, A2 at end of wormhole arm                     66.3      976
landed, SZ at loop top / after null                   70.4      988
landed, SZ before isVisible (shuffle!)                88.1      980
landed, SZ before isVisible, no A2 (shuffle!)         59.6      984
landed, SZ before isVisible + expect(shr,1) (shuffle) 88.4      976
landed, same + real compare (shuffle)                 89.2      976
landed, same + A2 (shuffle)                           88.1      980
landed, SZ in steal arm / wormhole arm                55.8/62.4 1004/1000
landed, SZ after hitCamera                            95.500000
landed, visible arm nested in `if (isVisible())`      57.3      1048
landed, `else if (isVisible())`                       62.9      992
landed, `while(1){...break;}` / `do{}while(0)` /
        `switch(0)` / `if(true)` / bare block         70.4      988
landed, loop-scope `Vector surface` (reassigned)      59.7      1008
landed, function-scope `Vector surface`               59.9      1012
landed, loop-scope `Vector collision`                 63.1      1024
landed, function-scope `Vector collision`             63.0      1032
landed, named `Vector pos` in visible arm (shuffle,
        but wrong slot: collision at sp+28)           66.3      1004
landed, `Vector collision;` before guard              55.2      1028
landed, `goto` to hitCamera (beq.w, not cbz)          87.4      984
landed, `goto` to compare with trivial decl           49.7      1012
faithful (no guards)                                  59.7      992
faithful + expect(shr,1)                              63.2      984
faithful + expect(steal,1) + expect(shr,1)            71.4      964
faithful + expect(!isVisible,0)                       69.5      956
faithful + expect(cmp,1)                              68.3      952
faithful + expect(!isVisible,0)+expect(cmp,0)         60.5      968
faithful + expect(isVisible,1)+expect(null,0)         66.5      968
faithful + expect(steal,1)+expect(cmp,1)              58.1      968
```

No `__builtin_expect` combination on the faithful body produces the
shuffle; the block split must be a real branch.

## 5. Dead ends (don't redo)

* Any attempt to make the guard cheaper: `objects->empty()`,
  `objects->size() == 0`, `objects->size() == i`, `objects->size() <= i`
  all compile to the same 6 B (`ldr`+`cmp`+`beq.n`) at the pre-call
  position; `objects == nullptr` / `this == nullptr` / `object == nullptr`
  are folded away (0 B, no shuffle); `i == 0` is 6 B but no shuffle.
* Any `__builtin_expect` on the szpos shape: `steal`, `isVisible`,
  `null`, `i==0`, `dock`, `loop` hints either keep 976 B + shuffle or
  drop to 972 B and lose the shuffle.
* `goto` with a close target: the visible arm is 134 B, just past the
  `cbz` ±126 B range, so the compiler always emits `beq.w`.
* Named `pos`/`surface` in the visible arm: produces a shuffle but a
  different one (collision slot moves to sp+28), +36 B.
* Reordering / swapping the `i == 0` and `Vector surface` blocks,
  `if (isVisible())` nesting, `else if`, `while(1)/do-while/switch`
  block-split tricks, `const`/`volatile`/pointer aliases of `collision`,
  `Vector collision;` declared before the guard, `moveTarget` spelled as
  member / pointer / reference (all identical code).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6926
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch harness (`/dev/shm/w6926`): `score.py` (pct/size/insn count and
`diff`), `splice.py`, `run.sh`, `sweep.py`, `gen.py`, `sweep_*.py`
(guard position/type, hints, tail spellings, structural variants),
`body_*.cpp` (saved bodies).

## 7. Gate

`byte_exact 4505 / linked_exact 4505 / size-exact 4524 / data_exact
843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B, `.text` 1,259,464 B.
