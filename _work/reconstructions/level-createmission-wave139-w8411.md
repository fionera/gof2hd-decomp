# w8411 (wave-139): `Level::createMission` — case-183 switch block order,
# `alienWave` is a bool, and the tail's `objectivesA` tail-merge

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8411`, based on master
`bf123bd47` (wave 138). Continues `level-createmission-wave138-w8311.md`,
`…-wave137-w8213.md`, `…-wave136-w8111.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` =
17,172 B). Ghidra MCP unavailable; the NDK `objdump` disassembly of the original
is the authority and is what `tools/verify/verify.py` scores. Raw same-address
byte diff is the primary metric (the fuzzy pct is nondeterministic).

## Baseline (pristine HEAD, re-measured this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5766
.text diff 85,476   .ARM.extab 1,396   .rel.plt 0   .note.gnu.build-id 20
.rodata/.data/.bss/.ARM.exidx exact
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

(The prompt's `.text` 85,476 / `.ARM.extab` 1,396 match this worktree.)

## 1. The case-183 switch: source order 0,1,2 beats default-first on raw diff

Wave 138 landed a `default:`-first switch. Re-measuring all 24 switch
permutations and the valid if/else permutations against the **raw diff** (not
the fuzzy pct) shows the three source orders that start with `case 0` are
strictly better:

| source order | emitted compare chain | raw diff | size |
|---|---|---:|---:|
| **case 0,1,2 then default (landed)** | `cmp #2`, `cmp #1`, `cbnz` | **5763** | 17172 |
| default first (wave-138 HEAD) | `cmp #0`, `cmp #1`, `cmp #2` | 5766 | 17172 |
| case 2 first | `cmp #0`, `cmp #1`, `cmp #2` | 5766 | 17172 |

The original's compare chain is `cmp #2`, `cmp #1`, `cbnz` (descending), which
is exactly the chain produced by a source starting with `case 0`. The
wave-138 note chose default-first because the *fuzzy* ratio was 0.15 pp
higher, but the raw byte diff is 3 bytes lower with case0-first (and the
`.ARM.extab` diff also drops 1, 1396 -> 1395). The block layout is still
`case0, case2, case1, join, default` vs the original's
`case0, case1, case2, default, join` — the residual +2-byte join branch
(`b.n` over the default block) and the literal-pool reorder are the remaining
difference, and are the same MachineBlockPlacement tie-break documented in
wave 138 (unreachable from a plain switch/if-chain).

Landed source:

```cpp
switch (k) {
case 0: offset[0] = 20000.0f; offset[1] = -3000.0f; offset[2] = -50000.0f; break;
case 1: offset[0] = 10000.0f; offset[1] = -7000.0f; offset[2] = -30000.0f; break;
case 2: offset[0] = 25000.0f; offset[1] = 0.0f;     offset[2] = -36000.0f; break;
default: offset[1] = 6000.0f; offset[2] = -45000.0f; offset[0] = 12000.0f; break;
}
```

## 2. NEW evidence: `alienWave` (and `pirateFortress`) are `bool` in the original

The alien-wave loop's two guards in the original are compiled as a *combined*
test, not two short-circuit branches:

```text
b2e04 subs.w r0, fp, r3        ; i - loopStart
b2e08 mov    r1, r0
b2e0a it     ne
b2e0c movne  r1, #1            ; r1 = (i != loopStart)
b2e0e ldr    r2, [sp, #60]     ; alienWave
b2e10 eor.w  r2, r2, #1        ; !alienWave
b2e14 orrs   r1, r2
b2e16 bne.w  b2f2a             ; if !(i==start && alienWave) goto else-if
```

The `eor r2, r2, #1` is only valid when the value is known 0/1, i.e. a
`bool`. The `pirateFortress` guard at `b2f2a` is identical (`eor #1`). Our
current source declares all three as `int`, and clang emits two sequential
short-circuit branches instead (`ldr alienWave; cmp #0; beq else-if; cmp
fp,r3; bne else-if`), 8 bytes shorter.

Changing `alienWave` to `bool` reproduces the original 18-byte combined test
exactly (modulo r0/r1 vs r1/r2), but it makes the whole function **16 bytes
shorter** (17,156 B): with a `bool`, clang drops the redundant
re-initialisation of `alienWave`/`bigAlienWave` that the original keeps after
the `nextInt(100)` call:

```text
original b0f16..b0f26 (after nextInt):   ours (bool) b0f14..b0f18:
  mov.w r0,#0 ; str r0,[sp,#60]            (only)
  mov.w r1,#0 ; str r1,[sp,#56]          movs r1,#0
  mov.w r0,#0 ; str r0,[sp,#64]          str  r1,[sp,#56]
```

That is exactly the 16-byte deficit. `bool` for `bigAlienWave`/`pirateFortress`
alone, explicit re-inits, `(bool) alienWave`, `!!alienWave`, a `bool aw =
alienWave;` local, and `bool isAlien = (i==start) && alienWave;` were all
measured; none both keep 17,172 B and reproduce the `eor` form:

| variant | size | raw diff |
|---|---:|---:|
| int alienWave (HEAD) | 17172 | 5766 |
| bool alienWave | 17156 | 12381 (shift) |
| bool alienWave + faithful tail | 17148 | 12392 |
| bool alienWave, explicit re-init | 17156 | 12381 |
| bool bigAlienWave+alienWave | 17164 | 12365 |
| bool all three | 17304 | 16536 |

So the `bool` hypothesis is strongly supported by the `eor` but needs the
missing 16 bytes of redundant init (or 16 bytes elsewhere) to land; the
current `volatile unsigned n` tail hack supplies only 8 of them.

## 3. Tail: `objectivesA` store is tail-merged in ours but not in the original

The case-1 `objectivesA = new Objective(7, attackerCount, this)` store is
emitted inline by the original:

```text
original b20f2 blx Objective ; b20f6 str r5,[r4,#40] ; b20f8 b.w b02ca
ours     b20f2 blx Objective ; b20f6 b.w b41e4       ; b20fa nop
```

Ours tail-merges it with the case-4-tail store at `b41e4`
(`str r5,[r4,#40]`), because both use `this` in `r4`. The original's tail
store is `str.w r5,[r8,#40]` (`this` in `r8`), so the two stores are not
identical and are not merged. This is the same `this`-in-`r8` register-map
issue wave 137 documented: the faithful tail (`i < this->enemies->size()`,
re-reading the size) keeps `this` live in a register through the loop, but
clang assigns `this` to `sl` and `Globals::globals` to `r8` (swapped vs the
original); the `volatile unsigned n` hack keeps `this` in `r8` but leaves
`&Globals::rnd` in `fp` instead of `true`. No source spelling found that
simultaneously re-reads the size, keeps `this` in `r8`, `globals` in `sl`,
`&rnd` spilled, and `true` in `fp`.

## 4. Other residuals (unchanged)

* The raid block (`if (raid)`) hoists `guns + friendCount + field_164 +
  field_168` before the raid guard in ours; the original computes it after the
  raid-specific spawn logic. Register allocation/scheduling, not statement
  order.
* Stack-slot swap `[sp,#0x54]`/`[sp,#0x58]`: original has `raid` at 0x54 and
  `Globals::globals` at 0x58; ours the reverse. This is a spill-slot
  allocation-order tie-break (PEI), not source declaration order.

## Landed change

`case 0,1,2` then `default:` (source order) for the case-183 convoy offset
switch. Function stays exactly 17,172 B; raw diff 5766 -> 5763; `.text`
85,476 -> 85,473; `.ARM.extab` 1,396 -> 1,395.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85,476 -> 85,473
.ARM.extab      1,396  -> 1,395
.rel.plt        0      -> 0
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw diff 5766 -> 5763
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8411/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8411/regdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8411/align.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w8411/{Level.base.cpp,Level.V1.cpp,fdiff.py,regdiff.py,
align.py,deltas.py,sweep*.py,swfull.py,combo.py,tailsweep.py,tailobj.py,
awtest.py,orig_full.asm,ours_full.asm,ours_V1.asm,ours_B.asm}`.
