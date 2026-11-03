# w3835: `Level::createMission` (90.7 %, 17172 B) — slot-swap correction + merged-condition evidence

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w3835`, branch `wave89-w3835`,
base `5710f6d0`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
No source change is committed: every variant below is neutral or a regression.

## Baseline (unchanged, gate green)

```text
_ZN5Level13createMissionEv  fuzzy 90.7 %  size 17172 B  @0xaffd8
normalized instruction stream: 6055 (orig) vs 6070 (ours)
  mask [sp,#N] stack offsets            -> 91.8 %
  mask stack offsets + loc_XXXX targets -> 93.9 %
gate: byte 4498 / linked 4498, .text 0x1337c8,
      .rodata/.data/.data.rel.ro/.plt/.got exact, lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after
this pass. `report.json` is not committed.

Diff shape with stack offsets and branch targets masked: **254 blocks,
365 orig / 380 ours mismatched instructions**. Of the 113 one-for-one
replacements, 34 are `ldr [pc,#N]` literal-pool address shifts (pure cascade),
1 is an `add r0,pc,#N`, and 1 is a `.short` branch offset. The rest are
register-allocation / block-placement differences.

## CORRECTION to the w3815 note: the swapped slot is the stack-protector guard address, not `&Globals::rnd`

The w3815 note says "orig `&Globals::rnd` = `[sp,#88]` (written once at
`b0024`, reloaded at `b02ca`), `raid` = `[sp,#84]`". The `raid` half is right;
the `&Globals::rnd` half is wrong.

`b0024` stores **`fp`**, and `fp` is loaded at `b000a`-region from the literal
pool word at `0xb036c`. That word is `0x0015fd14`; `add r0,pc` from `0xaffec`
gives GOT entry `0x20fd04`, and `Elf32.got_map()` resolves `0x20fd04` to
**`__stack_chk_guard`**. `fp = &__stack_chk_guard`; `ldr r1,[fp]` at `b000c`
feeds the canary store `str r1,[sp,#396]`, and `b02ca` reloads `fp` for the
canary check. So:

| slot | original | ours |
|---|---|---|
| `raid` (bool, 17 refs) | `sp+84` (fi#84) | `sp+88` (fi#83) |
| `&__stack_chk_guard` (2 refs: entry def, exit use) | `sp+88` (fi#83) | `sp+84` (fi#84) |

`&Globals::rnd` is actually at **`sp+120`** (stored at `b003e` from the GOT
word at `0xb0374` = `_ZN7Globals3rndE`), and `&Globals::status` at `sp+108`.
The whole frame is otherwise identical (`sub sp,#400`, canary value at
`sp+396`, `this` at `sp+124`).

Slot-access histogram (orig vs ours), confirming only these two slots move:

```text
sp+40  4/6   sp+44  6/6   sp+48  6/5   sp+52  6/5
sp+84 17/2   sp+88  2/17  sp+92 13/11  sp+104 18/22
sp+112 30/31 sp+120 28/27 sp+380 8/9
```

So the ~1.1 pp lost to stack offsets is the frame-index ordering of the
`raid` spill slot relative to the stack-protector guard-address spill slot.
Both are plain 4-byte RA spill slots (final frame objects `fi#74..fi#101`,
`offset = 124 - 4*(fi-74)`), so this is register-allocator spill order, not
source declaration order. The guard address is spilled in the entry block
(`b0024`) and `raid` first at `b04c0`, yet ours assigns `raid` the lower frame
index. Moving `bool raid;` to the top of the function regresses to
88.2 % / 17160 B (the frame shrinks 12 B), so it is not a declaration-order
fix.

## Secondary residual: the `b2e04` / `b2f2a` materialised merged condition

Original `b2e04` (`if (i == friendCount+field_164 && alienWave)`):

```asm
subs.w r0, fp, r3      ; r0 = i - X
mov    r1, r0
it     ne
movne  r1, #1          ; r1 = (i != X)
ldr    r2, [sp, #60]   ; alienWave
eor.w  r2, r2, #1      ; !alienWave
orrs   r1, r2
bne.w  b2f2a           ; -> else-if
```

i.e. the whole `(i != X) | !alienWave` is **materialised as an i32** and
branched on `!= 0`. The same shape appears at `b2f2a` for `pirateFortress`
(reusing `r0 = i - X` from the first test). Ours instead short-circuits:

```asm
ldr r0,[sp,#64] ; cmp #0 ; beq.w b2f20
cmp fp,r3       ; bne.w b2f20
ldr r0,[sp,#56] ; cmp #0 ; beq.w b306c
```

Mechanism (LLVM 7 `SelectionDAGBuilder::visitBr` / `FindMergedConditions`):
the merged-branch path is only taken when the branch condition is an
`and`/`or` with `hasOneUse()` **and** both operands satisfy
`InBlock(operand, CurBB)` (i.e. are defined in the branch block). Otherwise
`EmitBranchForMergedCondition` materialises the condition as a value and
branches on `!= 0`.

Our final IR is:

```llvm
; <label>:1123
  %1124 = icmp ne i32 %1116, 0        ; alienWave != 0   (block 1123)
...
; <label>:1410
  %1411 = icmp eq i32 %1403, %1404    ; i == X
  %1412 = and i1 %1124, %1411
  br i1 %1412, label %1413, label %1529
```

`%1124` is defined in block 1123, not 1410, so `InBlock` is false and the
materialised form would be expected — yet ours short-circuits. The ISel path
is therefore not fully explained by `hasOneUse`/`InBlock` alone (the value is
likely exported/rematerialised). The other compound site in this function,
`if (hasWanted && i == 0)` at `b2aa2`, **does** materialise in both binaries
(`mov r0,r1; cmp r1,#0; str r1,[sp,#104]; it ne; movne r0,#1; eor #1; orrs;
bne`), so the materialisation is reachable from this source shape.

### Variants tried for the compound condition (all from the same baseline)

| variant | fuzzy | size |
|---|---:|---:|
| **baseline** | **90.7** | **17172** |
| `bool isAlien = ...; if (isAlien) … else if (!isAlien && …)` (2 uses) | 90.7 | 17172 |
| `bool isAlien; if (isAlien \|\| (i==X && pirateFortress)) { if (isAlien) … }` | 86.3 | 17188 |
| `int isAlien = (i==X) && alienWave; if (isAlien)` | 90.7 | 17172 |
| `(i==X) & (alienWave != 0)` | 90.7 | 17172 |
| `alienWave == 1` | 90.7 | 17172 |
| nested `if (!(i==X && alienWave)) { if (!(i==X && pirateFortress)) … }` | 90.7 | 17172 |

`!isAlien` in the else branch is folded by instcombine (the value is known
false there), so the 2-use trick does not survive to ISel.

## Secondary residual: convoy `k` offset if-chain (`b1408`)

`for (int k = 0; k < 4; k++) { … if (k == 2) … else if (k == 1) … else if
(k == 0) … else … }` (source order `2,1,0,else`).

* original lowers **descending**: `cmp #2; beq case2; cmp #1; beq case1;
  cbnz default; case0` — i.e. it preserves the source order.
* ours lowers **ascending**: `cmp #0; beq case0; cmp #1; beq case1;
  cmp #2; bne default; case2`.

The case bodies (literal-pool constants 25000/0/-36000, 10000/-7000/-30000,
20000/-3000/-50000, 12000/6000/-45000) are identical. Tried `switch (k)`
(90.7, still ascending), explicit `else if (k == 3)` (90.7), and source
orders `0,1,2` (90.6), `1,2,0` (90.6), `2,0,1` (90.7), `0,2,1` (90.6). None
reproduces the descending order; it is a switch-lowering artifact.

## Secondary residual: alienGuns loop (`b32ba`)

Ours differs in three ways:

1. extra `Vector turretPos` default-ctor zero-init
   (`movs r0,#0; strd r0,r0,[sp,#212]; str r0,[sp,#220]`);
2. `turretPos` gets its own 12-byte slot (`sp+188`) where the original reuses
   the `turretDir` slot (`sp+200..208`);
3. ours calls `Vector::operator=` through the GOT
   (`ldr r0,[pc,#696]; add r0,pc; ldr r6,[r0]; blx r6`) where the original
   uses a direct PLT `blx sym:_ZN11AbyssEngine6AEMath6VectoraSERKS1_`.

Removing the default ctor shrinks the frame: `Vector turretPos;` removed ->
88.4 % / 17144 B; a single reused `Vector turretPos = {t[3],t[4],t[5]};
… turretPos = Vector{t[0],t[1],t[2]}` -> 88.2 % / 17160 B. So the original
keeps **two** Vector locals but StackSlotColoring merges their slots; ours
does not merge.

## Other confirmed (not source-controllable)

* `b0eac` `if (*bigShipsCount > 0 && localRace == 0)`: orig keeps
  `cmp #0; ble` (SETGT), ours canonicalises to `cmp #1; blt` (SETGE). The IR
  is already `icmp sgt i32 %x, 0`; rewriting `>= 1`, `!= 0`, `!(x <= 0)` or
  swapping the `&&` operands does not produce the SETGT form.
* EH landing pads (`b41f0`–`b42d0`): one extra cleanup variant and more
  `b.n` filler in the original; `.ARM.extab` is identical (956 B / 148 call
  sites), so this is block order, not a different try/catch structure.
* Literal-pool ordering differs for the case-183 diamond floats
  (`25000.0f`/`-36000.0f`/`-45000.0f`) and the final
  `pic:got:_ZN7Globals3rndE` word.

## Conclusion / next step

The single biggest remaining cause is the **frame-index swap between `raid`
and the stack-protector guard-address spill slot** (~67 instructions,
1.1 pp). It is a register-allocator spill-order artifact. Next step: find
what makes the RA create the guard-address slot before the `raid` slot in the
original (both are 4-byte slots; guard spilled at `b0024`, `raid` first
spilled at `b04c0`). The second biggest is the `b2e04`/`b2f2a` materialised
merged condition (12 instructions, ~0.2 pp), then the convoy switch order
(~7 instructions) and the alienGuns `Vector` slot merge (~20 instructions).
