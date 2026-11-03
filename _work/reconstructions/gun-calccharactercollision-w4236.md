# `Gun::calcCharacterCollision` pass 7: the shipped allocation is reachable — a PIC/non-PIC
# codegen lottery, not a source-shape difference (w4236)

Worker: w4236. Branch: `wave92-w4236`, base master `b379d00d`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w4236/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `b379d00d`) | **4498 / 4498** | **82.7%** | 1984 B |
| after (this pass) | **4498 / 4498** | **82.7%** | 1984 B |

No code change. This pass adds the decisive piece of evidence the earlier
notes were missing:

**The faithful body, compiled with the *same* LLVM 7.0.2 at `-O2` but with a
non-PIC relocation model, reproduces the shipped register allocation exactly**
(`this=r9`, `enemy=r8`, `i=r11/fp`, `i*3=r4`, `td` spilled). The PIC build of
the identical IR gives `this=r4`, `td=r9`. The two pre-RA machine-IR streams
differ only in the stack-guard load at `bb.0`. So the residual is a
greedy-allocator outcome that flips on a *single extra instruction at function
entry*, not a missing source construct.

## 1. Ground truth recap (unchanged, re-verified this pass)

Shipped prologue / stack map (`0x152f90`, frame 208):

```
152f9c  sub  sp, #208
152f9e  mov  r9, r0                 ; this = r9
152fa6  str  r0, [sp, #12]          ; canary ptr
152faa  str  r0, [sp, #204]         ; canary
152fd4  str  r1, [sp, #32]          ; scatter
152fe4  str  r2, [sp, #20]          ; explosive
153000  mov  r6, r9
153006  str.w r2, [r6], #216        ; lastHitKIPlayer = 0; r6 = this+0xd8
15300c  str  r1, [sp, #16]          ; explodeOnContact
15300e  str  r5, [sp, #36]          ; mine
153010  str  r6, [sp, #24]          ; *** td spill ***
153018  str  r2, [sp, #28]          ; e
153074  str.w fp, [sp, #40]         ; i
```

Shipped register map: `this=r9`, `enemy=r8` (outer and scatter `other` are the
same value), `i*3=r4`, `i=fp` (+spill `[sp,#40]`), `mine` spilled `[sp,#36]`,
`&enemy->radius` then `&position` in `sl`, `td` spilled `[sp,#24]` and reloaded
into `r6` at `15319c`, `153266`, `1533f6`.

Shipped scalar stack layout (all 4-byte, contiguous):

```
[sp,#12] canary ptr   [sp,#16] explodeOnContact   [sp,#20] explosive
[sp,#24] td           [sp,#28] e                  [sp,#32] scatter
[sp,#36] mine         [sp,#40] i                  [sp,#44..] vectors
```

## 2. The faithful body (no anchors) and its free `[sp,#24]`

The Ghidra-faithful body is the one in `body_faithful.txt` (local
`Vector *td = &this->targetDir;` right after `lastHitKIPlayer = 0`, no
`itemIndex` local, no volatile slot, no `hitFlag`, `enemy` reused by the
scatter loop). Compiled with the project command (clang 7.0.2 `-Oz`, PIC):

```
   0  push {r4,r5,r6,r7,lr}
   4  stmdb sp!, {r7,r8,r9,sl,fp}
   8  vpush {d8-d15}
   c  sub sp, #208
   e  mov r4, r0                 ; this = r4
  16  str r0, [sp, #12]          ; canary ptr
  1a  str r0, [sp, #204]         ; canary
  42  str r1, [sp, #36]          ; scatter
  52  str r2, [sp, #20]          ; explosive
  5e  mov r9, r4
  64  str.w r2, [r9], #216       ; lastHitKIPlayer = 0; r9 = this+0xd8
  7a  str r1, [sp, #16]          ; explodeOnContact
  7c  str r5, [sp, #40]          ; mine
  84  str r2, [sp, #24]          ; e
```

`this=r4`, `td=r9` (kept in a register), `enemy=r6`, `i=r8`, `i*3=r5`,
`radius=sl`, `mine` spilled `[sp,#40]`, `i*3`/`i` spilled `[sp,#28]/[sp,#32]`
around the ego branch. **Frame is 208 B**, and `[sp,#24]` holds `e` (the shipped
`td` spill slot); the shipped body has `e` one slot later at `[sp,#28]`.
Oracle 64.3% / 1920 B (HEAD 82.4 / 1984).

## 3. New evidence A — the same IR, non-PIC, gives the shipped allocation

`gun_strip.ll` is the clang `-Oz` IR of the faithful body (the `variables:`
field LLVM 7 cannot parse is stripped). Fed to the LLVM-7 `llc` from
`/home/debian/llvm7build/build/bin/llc`:

| llc invocation | frame | `this` | `td` | `enemy` | `i` | `i*3` |
|---|---:|---|---|---|---|---|
| `-O2` (default **static** reloc) | 200 | **r9** | **spilled `[sp,#8]`** | **r8** | **r11/fp** | **r4** |
| `-O2 -relocation-model=pic` | 208 | r4 | r9 | r6 | r8 | r5 |

Non-PIC prologue (target allocation):

```
	sub	sp, #200
	mov	r9, r0
	...
	mov	r6, r9
	str	r2, [r6], #216
	...
	str	r6, [sp, #8]            @ 4-byte Spill     <- td spilled
	...
	ldr.w	r8, [r0, r2, lsl #2]                     <- enemy = r8
	add.w	r4, r11, r11, lsl #1                     <- i*3 = r4, i = r11
```

PIC prologue (current allocation):

```
	sub	sp, #208
	mov	r4, r0
	...
	mov	r9, r4
	str.w	r2, [r9], #216
	...
	ldr.w	r6, [r0, r2, lsl #2]                     <- enemy = r6
	add.w	r5, r8, r8, lsl #1                       <- i*3 = r5, i = r8
```

The non-PIC frame is 200 B (one 4-byte scalar slot fewer than the PIC 208 B
because the PIC stack-guard load needs no extra local), so its `td` spill lands
at `[sp,#8]` rather than `[sp,#24]`; the *register* assignment is otherwise
exactly the shipped one. This is the first time the shipped allocation has been
reproduced from our source.

## 4. New evidence B — the only pre-RA IR difference is the stack-guard load

Dumping the pre-RA machine IR (`-print-after-all`, `# *** IR Dump After Live
Register Matrix ***`) for both builds and normalising away offsets, vreg
numbers, debug locations and `killed`:

```
 PIC (clang -Oz)                              non-PIC (llc -O2)
 %V:gprnopc = COPY $r0                        %V:gprnopc = COPY $r0
 %V:tgpr = tLDRLIT_ga_pcrel @__stack_chk_guard
 %V:gpr = t2LDRi12 %V:tgpr, 0                 %V:gpr = t2LDRpci %const.0
 %V:gpr = t2LDRi12 %V:gpr, 0                  %V:gpr = t2LDRi12 %V:gpr, 0
 t2STRi12 %V:gpr, %stack.0, 0                 t2STRi12 %V:gpr, %stack.0, 0
```

Everything after `bb.0` is structurally identical (the only other differences
are `%ir.N` label numbers, which are shifted by one, and the order of a couple
of `tLDRpci`/`VLDRS` constant loads inside `bb.8`). So the RA flip is caused by
**one extra instruction and one extra vreg at function entry**, not by any
source-level construct.

Independent confirmation with the real NDK clang: compiling the faithful body
with `-fno-stack-protector` (which removes the same two-instruction guard
sequence) also flips the allocation to `this=r9`, `td` spilled (`[sp,#20]`,
frame 200), oracle 60.5.

## 5. New evidence C — the exact greedy-allocator cascade (non-PIC trace)

`llc -O2 -debug-only=regalloc -filter-print-funcs=...` on the faithful IR.
Enqueue priorities are `(1<<29)+size`, `|=1<<31` for a global range, `|=1<<30`
for a copy hint; `this` (`%57`, size 16616) is first in both builds.

Non-PIC run — `this` is evicted and the cascade ends with `td` spilled:

```
selectOrSplit GPRnopc:%57 (this)
hints: $r0 $r0 $r0
assigning %57 to $r4: R4
...
selectOrSplit GPR:%455 [15992r,16096r)  weight 2.8e-01   <- remat of sp+stack.10 (a Vector temp)
hints: $r0 $r1
should evict: %57 (this)
evicting $r4 interference: Cascade 16
unassigning %57 from $r4: R4
assigning %455 to $r4: R4
...
selectOrSplit GPRnopc:%57 (this)          hints now $r0 $r0 $r0 $r9
should evict: %11 (td)
unassigning %11 from $r9: R9
assigning %57 to $r9: R9                  <- this = r9
...
selectOrSplit GPRnopc:%11 (td)            hints now $r0 $r0 $r0 $r9
evicting $r8 interference
assigning %11 to $r8: R8                  <- td = r8
...
selectOrSplit GPRnopc:%405 (enemy)
evicting $r8 interference
unassigning %11 from $r8: R8
assigning %405 to $r8: R8                 <- enemy = r8
...
selectOrSplit GPRnopc:%11 (td)            (no register left)
wait for second round
ENQUEUE ... prio=12174 stage=2
```

PIC run — `this` is **never evicted** (`grep -c "unassigning %57"` = 0):

```
selectOrSplit GPRnopc:%57 (this)
hints: $r0 $r0 $r0
assigning %57 to $r4: R4                  <- stays in r4
selectOrSplit GPRnopc:%11 (td)
assigning %11 to $r5: R5
...
unassigning %11 from $r5: R5
assigning %11 to $r9: R9                  <- td ends in r9
```

So the whole difference is the eviction of `this` from `r4` by the
rematerialised Vector stack address `%455`. Once `this` moves to `r9` it takes
`td`'s register, `td` takes `enemy`'s, and `enemy` takes `td`'s — the shipped
rotation.

## 6. New evidence D — iOS 1.1.4 corroborates the `td` spill

The iOS 1.1.4 Mach-O `Gun::calcCharacterCollision` is at `0xe8828`
(`llvm-objdump --macho --disassemble`):

```
   e8842:	sub	sp, #0x140
   e8844:	mov	r8, r0                 ; this = r8
   e88b2:	mov	r0, r8
   e88ba:	str	r2, [r0], #216         ; lastHitKIPlayer = 0; r0 = this+0xd8
   e88be:	str	r0, [sp, #0x28]        ; td spilled
```

i.e. the same source shape on a different compiler/ABI also ends with `td`
spilled and `this` in a callee-saved register. The spill is real in the
original source, not an artefact of the Android build.

## 7. Source-shape sweep (all measured with the object-level oracle)

`/dev/shm/w4236/sweep.py` splices a body into `Gun.cpp`, compiles the TU with
the exact project command, and reports `(pct, size, frame, this, td, td-spill
slot, enemy)`. HEAD oracle = 82.4 / 1984 / frame 216 / this=r9 / td=sl.

Faithful base = 64.3 / 1920 / frame 208 / this=r4 / td=r9.

| variant | pct | size | frame | this | td | td spill | enemy |
|---|---:|---:|---:|---|---|---|---|
| faithful base | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, drop `__builtin_expect` | 62.0 | 1920 | 208 | r4 | r9 | — | r6 |
| base + `const int itemIndex` (unused) | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base + `itemIndex` **used everywhere** | 65.8 | 1952 | 208 | **r9** | sl | — | r1 |
| base + `itemIndex` used in damage calls only | 62.6 | 1908 | 208 | r4 | sl | — | **r8** |
| base + `itemIndex` + `radius` declared first | 64.7 | 1952 | 208 | **r9** | sl | — | r1 |
| base, `int` instead of `bool` for mine/scatter/bomb | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, `int` loop counters (e/i/k) | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, `enemy` declared at function scope | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, `td` before the store / at function top / `Vector &` / direct member | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, `Gun *self = this` | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, `Vector position(...)` copy-ctor | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, `this->positions[i]` | 64.3 | 1920 | 208 | r4 | r9 | — | r6 |
| base, `radius` as `float` | 48.3 | 1920 | 200 | — | r9 | — | r6 |
| base, inner `while` | 58.4 | 1952 | 208 | r8 | r9 | — | r6 |
| base, outer `while` | 64.1 | 1932 | 208 | r4 | r9 | — | r6 |
| base, `volatile bool mine` | 48.5 | 1944 | 208 | r4 | sl | — | r6 |
| base, `volatile bool scatter` | 56.6 | 1932 | 208 | r4 | r9 | — | r6 |
| base, `volatile int explosive` | 53.0 | 1908 | 208 | r4 | r9 | — | r1 |
| base, `volatile int explodeOnContact` | 53.3 | 1912 | 208 | r4 | r9 | — | r2 |
| base, separate `if (bomb)` / `if (mine)` | 59.9 | 1916 | 208 | r4 | r9 | — | r3 |
| base, `explosive` computed after `explodeOnContact` | 60.8 | 1904 | 208 | r4 | r9 | — | r2 |
| base, `Vector`s declared **outside** the inner loop | 60.0 | 2004 | 208 | **r9** | r5 | **[sp,#20]** | r1 |
| base, `Vector`s at function top | 61.3 | 1996 | 208 | **r9** | r5 | [sp,#16] | r1 |
| base, `Vector`s in the outer loop | 61.4 | 2000 | 208 | **r9** | r5 | [sp,#16] | r1 |
| base, cached `positions.count` (inner) | 64.3 | 1936 | 208 | r4 | r9 | — | r6 |
| base, cached `enemies->size()` (outer) | 64.4 | 1924 | 208 | r4 | r9 | — | r6 |
| base, cached count + local `damage` | **69.8** | 1940 | 208 | **r9** | r8 | — | r6 |
| base, outer `while` + local `damage` | 69.6 | 1936 | 208 | **r9** | r8 | — | r6 |
| base, `itemIndex` in damage + local `damage` | 68.7 | 1904 | 208 | r4 | — | — | **r8** |
| base, `Vector`s outside + `volatile scatter` | 60.7 | 2020 | 208 | **r9** | r5 | [sp,#12] | r1 |

Also swept (all codegen-identical to the faithful base, 64.3 / 1920 / 208 /
this=r4): `td` declared at top / before the null check / inside the outer loop /
inside the scatter loop, `Vector **tdp`, `const`/`volatile` `td`, `Level *level`
local, `Array<Player*> *enemies` local, `KIPlayer *ki` hoisted, `e`/`i` declared
outside their `for`s, `do`/`while` inner loop, swapped damage branches, `int`
`explosive`/`explodeOnContact`, `radius` via an explicit `if`.

Compiler-flag probes on the faithful body (none produce the shipped allocation
with the PIC frame):

| flag | pct | size | frame | this | td |
|---|---:|---:|---:|---|---|
| `-mllvm -enable-misched=false` | 64.3 | 1920 | 208 | r4 | r9 |
| `-mllvm -enable-post-misched=false` | 64.3 | 1920 | 208 | r4 | r9 |
| `-mllvm -split-spill-mode=size` | 64.3 | 1920 | 208 | r4 | r9 |
| `-mllvm -split-spill-mode=default` | 63.2 | 1924 | 208 | r4 | r9 |
| `-mllvm -regalloc=basic` | 28.0 | 1896 | 200 | r6 | r8 |
| `-mllvm -regalloc=pbqp` | 28.9 | 1964 | 200 | r8 | sl |
| `-fno-stack-protector` | 60.5 | 1972 | 200 | **r9** | r5 `[sp,#20]` |

No variant beats HEAD's 82.4 oracle (82.7 official). The best `this = r9`,
frame 208 variants are 69.8 (`cached count + local damage`) and 65.8
(`itemIndex` used everywhere), both well below HEAD.

## 8. Conclusion / remaining work

The faithful source shape is confirmed from three independent directions:
Ghidra (`Vector *td = &this->targetDir`, `enemy` reused in the scatter loop, no
`itemIndex` local), the shipped stack layout (`td` at the one free `[sp,#24]`
of the 208-byte faithful frame, `e` shifted to `[sp,#28]`), and the iOS 1.1.4
build (same `td` spill). The shipped register allocation is also reachable from
this source: the identical IR compiled non-PIC gives `this=r9`, `enemy=r8`,
`i=fp`, `i*3=r4`, `td` spilled.

The residual is a **greedy-allocator lottery**: the PIC stack-guard load adds
one instruction and one vreg at `bb.0`, and that is enough to stop `%455` (a
rematerialised `sp+stack.10` Vector address) from evicting `this` from `r4`.
`this` then keeps `r4`, `td` keeps `r9`, and the shipped rotation never
happens. Because the flip is driven by vreg numbering / live-range tie-breaks
rather than by any source construct, none of the ~700 source shapes tried in
this pass (or the ~200 in w3805/w3829/w3909/w4010/w4110/w4211) reproduces it
while also beating HEAD.

The remaining lever, if anyone continues: find a source change that perturbs
the *early* IR (the `bb.0`/`bb.8` value numbering) enough to reproduce the
non-PIC eviction cascade without adding a stack slot — e.g. a different
constant-load order or a different first use of `this`. The `-fno-stack-protector`
and non-PIC experiments bound the search: any such change must move the RA into
the `this=r9` basin while leaving the 208-byte frame intact.

## 9. Reproduction

```
# baseline
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4498 linked 4498

# object-level oracle for one body
TMPDIR=/dev/shm/w4236 python3 /dev/shm/w4236/oracle.py \
  --src src/game/weapons/Gun.cpp --fn _ZN3Gun22calcCharacterCollisionEv
# signature
python3 /dev/shm/w4236/sweep.py /dev/shm/w4236/body_faithful.txt

# non-PIC llc replay (target allocation)
/home/debian/llvm7build/build/bin/llc -O2 -filetype=asm /dev/shm/w4236/gun_strip.ll -o /dev/shm/w4236/llc_O2.s
/home/debian/llvm7build/build/bin/llc -O2 -relocation-model=pic -filetype=asm \
  /dev/shm/w4236/gun_strip.ll -o /dev/shm/w4236/llc_pic.s
/home/debian/llvm7build/build/bin/llc -O2 -debug-only=regalloc \
  -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv /dev/shm/w4236/gun_strip.ll -o /dev/null
```

## 10. Files changed

None under `src/` (note only). Gate: byte 4498 / linked 4498, size-exact
4524/4524, data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26290`, lint CLEAN, drift 0.
