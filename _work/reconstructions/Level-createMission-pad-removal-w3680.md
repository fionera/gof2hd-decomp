# w3680: removing the `Level::createMission` pad anchor

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w3680`, branch `wave87-w3680`,
HEAD `fa2d054f`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(real address = Ghidra address − 0x10000). No source change is committed: every
pad-removal variant below is a regression, so this note is the deliverable.

## Baseline (HEAD `fa2d054f`, pad present)

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

Measured:

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN ✓ (no new soft violations; hard invariants 0)
drift:  0 field(s) across 0 class(es)
```

ELF / symbol facts:

```text
.text       0x1337c8 @ 0x69440
.ARM.exidx  0x006f70
.ARM.extab  0x026290          (original: 0x026298, already −8)
.rodata     0x041b67 @ 0x1c9e10   (base exact)
createMission 0xaffd8 size 17172
```

`createMission`'s `.ARM.extab` record is **956 bytes / 148 call sites**,
identical to the original (see EH section below).

## Controlled A/B experiments

All variants start from the baseline source and differ only in the
`case4Tail` block of `src/game/world/Level.cpp`. `check` was run for variants
A, C, D2 and E; B and G were measured with `link_so` only (size/section
effect is deterministic and no gate number can improve when the function is
already smaller than the original).

| variant | source change | createMission | `.text` | `.ARM.extab` | `.rodata` | byte_exact | linked_exact |
|---|---|---:|---:|---:|---:|---:|---:|
| baseline | 8 volatile pad stores + `true, false` | 17172 | 0x1337c8 | 0x26290 | 0x1c9e10 | **4496** | **4496** |
| A | delete pad, keep `true, false` | 17140 | 0x1337a8 | 0x26290 | 0x1c9df0 | **2501** | 4496 |
| B | delete pad + `int hostile = 1;` | 17140 | 0x1337a8 | 0x26290 | 0x1c9df0 | (not checked) | — |
| C | delete pad + try/catch around `new Objective` | 17164 | 0x1337c8 | 0x262a0 | 0x1c9e20 | **2814** | 4496 |
| D2 | C + `volatile int hostile = 1;` | 17172 | 0x1337c8 | 0x262a0 | 0x1c9e20 | **3419** | 4496 |
| E | delete pad + `volatile int hostile = 1;` (no try/catch) | 17148 | 0x1337c8 | 0x26290 | 0x1c9e10 | **3086** | 4496 |
| G | delete pad + source-level `int *p164/p168` aliases | 17360 | 0x133888 | 0x26290 | 0x1c9ed0 | (not checked) | — |

Exact commands:

```sh
# A
#   delete the eight `pad = N;` lines, keep `true, false`
cmake --build cmake-build-match --target check -j8
# B
#   delete the pad, add `int hostile = 1;` before the loop and pass `hostile`
cmake --build cmake-build-match --target link_so -j8
# C
#   delete the pad, wrap `new Objective(18, 0, hunterCount, this)` in try/catch
cmake --build cmake-build-match --target check -j8
# D2
#   C + `volatile int hostile = 1;` / pass `hostile`
cmake --build cmake-build-match --target check -j8
# E
#   delete the pad + `volatile int hostile = 1;` / pass `hostile`
cmake --build cmake-build-match --target check -j8
# G
#   delete the pad, add `int *p168 = &this->field_168; int *p164 = &this->field_164;`
#   after the `stationIndex == 78` if/else, and use them at the six sites where
#   Ghidra shows pLVar31/pLVar32
cmake --build cmake-build-match --target link_so -j8
```

Per-function rows for the checked variants:

```text
A : _ZN5Level13createMissionEv  pct 85.8  osize 17172  usize 17140  byte False
C : _ZN5Level13createMissionEv  pct 85.7  osize 17172  usize 17164  byte False
D2: _ZN5Level13createMissionEv  pct 86.4  osize 17172  usize 17172  byte False
E : _ZN5Level13createMissionEv  pct 85.6  osize 17172  usize 17148  byte False
```

### What each experiment proves

* **A** — the pad is exactly 32 bytes (`0x20`) of code. Removing it alone
  shrinks `createMission` by exactly 32 bytes and shifts `.text` and
  `.rodata` by `-0x20`. `linked_exact` stays 4496 because the function is
  still linked at a resolvable symbol; `byte_exact` collapses to 2501.
* **B** — `int hostile = 1;` is constant-folded at `-Oz`; the function stays
  17140 bytes. It is not the missing code.
* **C** — the old try/catch restores 24 of the 32 bytes (17140 → 17164) and
  makes `.text` exact, but it adds an EH call-site and a type-table entry:
  `.ARM.extab` grows by 8 (`0x26290 → 0x262a0`) and `.rodata` moves by
  `+0x10` (`0x1c9e10 → 0x1c9e20`). `byte_exact` falls to 2814. The try/catch
  is therefore *not* the original construct.
* **D2** — adding `volatile int hostile = 1;` on top of C restores the
  function size to 17172, but the EH/rodata shift remains, so `byte_exact`
  is only 3419.
* **E** — a volatile `hostile` without the try/catch keeps `.text`,
  `.ARM.extab` and `.rodata` at their baseline sizes, but `createMission`
  is 17148 (24 bytes short) and `byte_exact` is 3086. The volatile load
  changes the tail shape but does not recover the missing 32 bytes.
* **G** — forcing source-level field pointers (the shape Ghidra shows as
  `pLVar31 = this + 0x168; pLVar32 = this + 0x164;`) over-generates badly:
  17360 bytes (+188), because the pointers stay live across the whole
  `isEmpty` branch.

## Disassembly evidence: the pad is not the missing code

Original tail after `ArraySetLength` (`0xaffd8 + 0x4184 = 0xb415c`):

```asm
b415c: ldr   r0, [pc, #392]     ; Globals::globals
b415e: mov.w fp, #1             ; hostile = 1 hoisted into fp
b4162: mov.w r9, #0             ; loop index
b4166: add   r0, pc
b4168: ldr.w sl, [r0]           ; sl = Globals::globals
b416c: b.n   b41c8
...
b4198: strd  r0, fp, [sp]       ; waypoint, hostile(=1)
b419c: movs  r0, #0
b419e: str   r0, [sp, #8]       ; group = false
```

Our pad-free build (`variant A`, `0xb4130`):

```asm
b4130: ldr   r0, [pc, #400]     ; Globals::globals
b4132: mov.w r9, #0
b4136: add   r0, pc
b4138: ldr.w r8, [r0]           ; r8 = Globals::globals
b413c: ldr   r0, [pc, #392]     ; Globals::rnd
b413e: add   r0, pc
b4140: ldr.w fp, [r0]           ; fp = Globals::rnd
b4144: b.n   b41a2
...
b4170: str   r0, [sp, #0]
b4172: movs  r0, #1
b4174: str   r0, [sp, #4]       ; hostile materialized every iteration
b4176: movs  r0, #0
b4178: str   r0, [sp, #8]
```

The original keeps `rnd` in stack slot `[sp,#120]` and uses `fp` for the
constant 1; ours keeps `rnd` in `fp` and materializes 1 at each call. The
original tail is therefore **6 bytes smaller** (20-byte setup vs 26) and its
literal pool is **4 bytes smaller** (6 words vs 7). Matching the original
tail would shrink our function further, not add the missing 32 bytes.

The missing 32 bytes are distributed across the rest of the function. A
call-anchored comparison of the 810 `bl`/`blx` sites shows 63 inter-call
regions with non-zero net length (largest: −11 B at `b3284`, −10 B at
`b3f5e`, +8 B at `b32c2`, +7 B at `b3308`, +7 B at `b3f3c`, +6 B at
`b0640`). These are mostly register-allocation/scheduling differences; no
single missing statement was found. The +6 B at `b0640` is the original's
`add.w r0, sl, #360 / str r0, [sp,#72] / add.w r0, sl, #356 /
str r0, [sp,#76]` address-taking for `field_168`/`field_164`; the
source-level pointer rewrite (variant G) over-generates and is not a fix.

## EH evidence: the original has no try/catch

Parsing the `.ARM.exidx`/`.ARM.extab` record for `createMission`
(`fn = 0xaffd8`) with `_work/reconstructions/exidx_table.py`:

| build | record length | call sites | LSDA |
|---|---:|---:|---|
| original | 956 | 148 | `lps_enc=0xff, ttype_enc=0xff, cs_enc=0x01` |
| baseline (pad) | 956 | 148 | same shape |
| variant C (try/catch) | 972 | 149 | extra type/action entry; `ttype_enc=0x00` |

The original's call-site table has exactly 148 entries and its record is
956 bytes — the same shape as the pad build, not the try/catch build. The
old try/catch therefore cannot be the real source construct: it adds a
149th call site and 16 bytes of EH data.

## Conclusion

**Proven:** the 8-store volatile pad is load-bearing. It is exactly the
32 bytes that the current source fails to generate. Removing it alone gives
`byte_exact 2501`; the try/catch variant gives 2814; the volatile-hostile
variants give 3086/3419. None is a strict improvement, so no source change
is committed.

**Still hypothesis:** the 32 bytes correspond to real source somewhere in
`createMission`, but not to the try/catch, not to a `hostile` local, and not
to a simple `field_164/field_168` pointer alias. The call-anchored diff shows
the difference is spread over many basic blocks (mostly codegen/register
allocation), so the next pass should target the largest inter-call deltas
(`b3284`, `b3f5e`, `b32c2`, `b3308`, `b3f3c`, `b0640`) rather than adding
another anchor.

**Recommendation:** keep the pad until a real 32-byte reconstruction lands.
Any replacement must preserve all of:

```text
createMission size 17172
.text             0x1337c8
.ARM.extab        0x026298 (or at least not grow the createMission record)
.rodata base      0x1c9e10
byte_exact        4496
linked_exact      4496
```

## w3706 follow-up: cheaper 32-byte pad (86.84 -> 86.90%)

The pad was still 8 `movs r0,#N; str r0,[sp,#312]` pairs = 16 instructions / 32 bytes.
The original tail has only 3 instructions in that region, so the pad was costing 13
unmatched instructions.  Storing a *runtime* value that lives in a callee-saved high
register makes clang emit a 4-byte `str.w rX,[sp,#N]` per element instead of a
movs/str pair:

```cpp
volatile unsigned pad[8];
unsigned padValue = (unsigned) (intptr_t) this;
pad[0] = padValue; ... pad[7] = padValue;
```

This is still exactly 32 bytes of dead stores (8 x `str.w sl,[sp,#N]`, the compiler
scatters the never-read elements over free slots 188..384), so createMission stays
17172 B and every section size/base is unchanged.  It halves the pad's instruction
count (16 -> 8), which lifts the fuzzy match 86.84 -> 86.90 (raw 86.838199 ->
86.895527) with the gate unchanged: byte 4496 / linked 4496, .text 0x1337c8,
.rodata 0x1c9e10, lint CLEAN, drift 0.

Measured alternatives (all from the same base, same 17172 B unless noted):

| pad form | instrs | fuzzy |
|---|---:|---:|
| `volatile int pad; pad = 1..8;` (baseline) | 16 | 86.84 |
| `volatile unsigned pad[8]` + `padValue = (unsigned)(intptr_t)this` | 8 | **86.90** |
| same, scalar `volatile unsigned pad` | 8 | 86.90 |
| `volatile int pad[8]` + `pad[i] = 1..8` | 16 | 86.84 |
| `volatile int pad[8] = {1..8}` | 0 (17140 B) | 85.85 |
| `volatile unsigned long long pad[4]` | 6 (17164 B) | 85.00 |
| `volatile double pad[4]` | 5 (17164 B) | 85.00 |

Also re-measured the siegedStation turret-index fix (the original loads
`Vector{t[4],t[5],t[6]}` / `Vector{t[1],t[2],t[3]}` from `kStationTurrets[race][k]`,
i.e. the table layout is `{unused,x,y,z,rx,ry,rz}`, not `{x,y,z,rx,ry,rz}`): it is
semantically correct but scores 86.79 alone and 86.85 combined with the new pad, so
it is not committed here.
