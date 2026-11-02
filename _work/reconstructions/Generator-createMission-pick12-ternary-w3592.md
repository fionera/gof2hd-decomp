# Generator::createMission — the `pick == 12` ternary (w3592)

Status: research note (2026-11-02). Every number below was produced by a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or by a controlled rebuild of this tree. No source
or flag change was landed: none of the experiments is a strict improvement over the current gate.

This note continues `Generator-createMission-real-4-bytes-w3591.md`. That note proved the current
`0.5000001f` literal is a +4-byte size anchor and that the real 4-byte delta is in the `pick == 12`
block. This note isolates the block further and records the source-shape search.

## 1. Baseline (HEAD `352273ca`, clean tree)

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`readelf -S cmake-build-match/libgof2hdaa.so`:

```
.text   0x1337c8
.rodata 0x41b67
```

`cmake-build-match/verify/report.json` row:

```
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
  osize 1996  usize 1996  pct 73.6
```

## 2. Original block and the surviving select

Original (`../_work/bins/android_2.0.16_libgof2hdaa.so`, `0x972b0..0x972d4`):

```
972b0: 2e0c        cmp   r6, #12
972b2: d10c        bne.n 972ce
972b4: 9808        ldr   r0, [sp, #32]
972b6: 6800        ldr   r0, [r0]
972b8: f7ca ebaa   blx   Status::getSystem()
972bc: f7ca ebde   blx   SolarSystem::getIndex()
972c0: 220c        movs  r2, #12            ; type = 12
972c2: 2819        cmp   r0, #25
972c4: f04f 0b01   mov.w fp, #1             ; selected = true
972c8: bf18        it    ne
972ca: 4632        movne r2, r6             ; type = pick
972cc: e002        b.n   972d4
972ce: f04f 0b01   mov.w fp, #1
972d2: 4632        mov   r2, r6
```

The select **survives** to instruction selection: `select (getIndex() != 25), pick, 12` is
lowered as `movs r2,#12; cmp r0,#25; it ne; movne r2,r6`. `type` stays in `r2`; `pick` stays in
`r6`. The surrounding loop body is 140 bytes (`0x97248..0x972d4`).

Current build (`cmake-build-match/libgof2hdaa.so`, `0x972b4..0x972d4`):

```
972b4: f5b1 2f40   cmp.w r1, #786432        ; (short)pick << 16 == 12 << 16
972b8: d109        bne.n 972ce
972ba: 9807        ldr   r0, [sp, #28]
972bc: 6800        ldr   r0, [r0]
972be: f7ca eba8   blx   Status::getSystem()
972c2: f7ca ebdc   blx   SolarSystem::getIndex()
972c6: f04f 0b01   mov.w fp, #1
972ca: 260c        movs  r6, #12             ; select constant-folded
972cc: e001        b.n   972d2
972ce: f04f 0b01   mov.w fp, #1
972d2: 3d01        subs  r5, #1
```

Our clang 7.0.2 folds the select into the loop phi (`%84 = phi ... [12, %78] ...` in the
optimized IR) and coalesces `type` and `pick` into `r6`. The original does neither. The loop body
is 134 bytes (`0x9724e..0x972d4`).

Reproduce:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x971b8 --stop-address=0x97320 ../_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x971b8 --stop-address=0x97320 cmake-build-match/libgof2hdaa.so
```

Ghidra confirms the same structure (project `../_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`,
function at Ghidra `0xa71b8` = real `0x971b8`):

```
if (uVar10 == 0xc) {
    pSVar3 = Status::getSystem(...);
    iVar8 = SolarSystem::getIndex(pSVar3);
    bVar1 = true;
    uVar11 = 0xc;
    if (iVar8 != 0x19) {
        uVar11 = uVar10;
    }
}
```

i.e. `type = 12; if (getIndex() != 25) type = pick;` — a branchy select that the original compiler
did **not** fold.

## 3. Why the fold matters

`type` is initialized to 0 (`movs r2,#0` at `0x97234` in the original, `movs r6,#0` in ours) and is
a loop-carried phi. In the original, the true block contributes a *select result* to the phi, so the
phi must live in `r2`; `pick` stays in `r6`. In our build, the true block contributes the constant
`12`, so clang can coalesce the phi with `pick` in `r6`. That single difference also explains the
other loop differences:

* original `pick == 8` path is 4 bytes (`cmp r0,#8; beq 972d2`) because it shares the
  `mov r2,r6` at `972d2` with the non-12 path; ours is 8 bytes (`cmp r0,#8; bne; movs r6,#8; b`);
* original `pick == 10` path checks `pick` first and folds `type = pick` to `movs r2,#10`
  (12 bytes); ours checks `race` first and folds to `movs r6,#10` (14 bytes);
* original non-12 path is 6 bytes (`mov fp,#1; mov r2,r6`); ours is 4 bytes (`mov r9,#1`);
* original decrements `guard` immediately after `nextInt` (`sub r9,#1`, 4 bytes); ours decrements
  at the loop latch (`subs r5,#1`, 2 bytes).

The loop-size delta is +6 bytes in the original (140 vs 134). The `pick == 12` block alone is
+4 bytes (36 vs 32); the remaining +2 comes from the `pick == 8`/`pick == 10` path shapes. The
function-size delta is +4 bytes once the `0.5f` hack is removed (1996 vs 1992), because the hack
adds 4 bytes to our build.

## 4. Controlled full-gate experiments

All runs: edit `src/game/mission/Generator.cpp`, `cmake --build cmake-build-match --target link_so
-j8`, then `cmake --build cmake-build-match --target check -j8`. `report.json` row is
`Generator::createMission`. Baseline is `byte 4496 / linked 4496`, `usize 1996`, `pct 73.6`,
`.text 0x1337c8`.

| variant | usize | pct | gate byte/linked | .text | verdict |
|---|---:|---:|---|---:|---|
| HEAD (`short pick`, `0.5000001f`) | 1996 | 73.6 | 4496 / 4496 | 0x1337c8 | baseline, PASS |
| `int pick` (hack kept) | 1960 | 48.9 | 2045 / 4496 | 0x1337a8 | FAIL; −32 B |
| `while (!selected)` + `--guard` after `nextInt`, `int pick` (hack kept) | 1976 | 45.4 | 2615 / 4496 | 0x1337c8 | FAIL; −20 B |
| same as above + true `0.5f` | 1972 | 45.6 | 2615 / 4496 | 0x1337c8 | FAIL; −24 B |
| `int pick` + explicit `type = pick` in `pick == 10` branch (hack kept) | 2024 | 50.8 | 2045 / 4496 | 0x1337e8 | FAIL; +28 B |
| `int pick` + `short p = pick; if (p == 12) ...` (hack kept) | 1960 | 48.9 | 2045 / 4496 | 0x1337a8 | FAIL; select survives but `uxth` + inverted form |
| true `0.5f` alone (`short pick`) | 1992 | 73.8 | 2615 / 4496 | 0x1337c8 | FAIL; −4 B (w3591) |

The `int pick` row is the closest to the original's loop *shape* (no `sxth`, direct `cmp r6,#12`),
but it is 32 bytes shorter and the fuzzy match drops from 73.6 to 48.9. The `while` variant moves
the `guard` decrement to the original's position but is still 20 bytes short. The explicit
`type = pick` in the `pick == 10` branch makes the function 28 bytes too long and does not unfold
the select.

## 5. Source-shape search for the surviving select

Scratch harness (outside the tree):

```sh
# /tmp/w3592/compile.sh <src.cpp> <out.o>  -- exact Generator.cpp flags from build.ninja
# then: objdump -d <out.o> | sed -n '/<.*createMission.*>:/,/^$/p'
```

Variants tested in the real function (all compiled with the exact `Generator.cpp` flags):

| shape | result |
|---|---|
| `int pick` | folds to `movs r6,#12` |
| `short`, `unsigned short`, `char`, `unsigned char`, `long`, `long long`, `unsigned int`, `volatile int` | fold (volatile gives a branchy select with a stack reload) |
| `if (type == 12)` with `type = pick` at top | folds (`cmp r6,#12`) |
| `switch (pick) { case 12: ... }` (with/without extra cases) | folds |
| `bool is12 = pick == 12; if (is12) ...` | folds |
| `if/else` instead of `?:` | folds |
| `type = 12; if (getIndex() != 25) type = pick;` | folds |
| `switch (getIndex()) { case 25: type = 12; default: type = pick; }` | folds |
| `(getIndex() ^ 25) != 0`, `(getIndex() - 25) != 0` | folds |
| `int p = (short)pick; if (p == 12) type = ... ? pick : 12;` | **select survives**, but `uxth r0,r6; cmp r0,#12` and `it eq; moveq r6,#12` (inverted, destination `r6`) |
| `short p = pick; ...` | same as above |
| `volatile int p = pick; ...` | branchy select with `ldr` reloads |
| remove `type = pick` at top, assign `type` only in else/continue paths, select true arm `type` | select survives, but old `type` is reloaded from the stack (`ldr r0,[sp,#36]`) |
| loop-carried `prev` phi as the true arm | select survives, but `prev` is reloaded from the stack |
| `do { ... } while`, `while (done)`, `goto` loop | folds |
| `-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz` on `Generator.cpp` | fold at every level (O0 uses a branch, not the `it` form) |
| `-fwrapv`, `-fno-strict-overflow`, `-fno-strict-aliasing`, `-fno-builtin`, `-fno-inline-functions`, `-fno-vectorize`, `-fno-slp-vectorize`, `-fno-threadsafe-statics`, `-fno-use-cxa-atexit`, `-fno-exceptions` | fold |
| `-mllvm -disable-branch-fold`, `-disable-early-ifcvt`, `-disable-cgp-select2branch`, `-simplifycfg-*` | fold |

The only shapes that keep a select alive use a value the compiler cannot prove is `12`
(a `short` truncation, a volatile/reloaded copy, or a loop-carried phi). None of them reproduces the
original's exact form: the original's true arm is the *untruncated* `pick` in `r6`, the compare is
the plain `cmp r6,#12`, and the destination is `r2` with the false value `12` materialized first.

## 6. Conclusion

Proven:

* The original's `pick == 12` block contains a surviving select
  `type = (getIndex() != 25) ? pick : 12`, lowered as
  `movs r2,#12; cmp r0,#25; it ne; movne r2,r6`.
* Our clang 7.0.2 folds that select into the loop phi and coalesces `type`/`pick` into `r6`.
  This is the root of the 4-byte `pick == 12` block delta and of the `r2`/`r6` register split.
* `int pick` alone is not a fix: it removes the `sxth`/shifted compare but shrinks the function by
  32 bytes and drops the fuzzy match to 48.9%.
* Moving `--guard` to the original's position (immediately after `nextInt`) is a real structural
  match but is not sufficient: the select still folds and the function is 20 bytes short.
* No tested source shape, flag, or optimization level produces a strict gate improvement. The
  current `0.5000001f` hack is still the only variant that passes `check`.

Still hypothesis:

* The exact source spelling that keeps the select alive while leaving `pick` untruncated in `r6`.
  The evidence points at a value the compiler cannot prove is `12` but which the register allocator
  can coalesce with `pick`; a loop-carried phi or a memory copy that is not spilled are the most
  plausible candidates, but neither reproduced the exact `r2`/`r6` form in this tree.
* The original `pick` type is `int` (no `sxth`/`uxtb` anywhere in the function, direct
  `cmp r6,#12`), but the rest of the function is also ~32 bytes larger than our `int pick` build,
  so other source-shape work is needed before the `int pick` switch can be landed.

No source or flag change is committed with this note. The worktree is clean and `check` passes at
`byte 4496 / linked 4496`.

## 7. Reproduction commands

```sh
# baseline
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8

# int pick
sed -i 's/        short pick = Globals::rnd->nextInt(15);/        int pick = Globals::rnd->nextInt(15);/' \
  src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8

# while loop with guard-- after nextInt (int pick)
python3 - <<'PY'
p='src/game/mission/Generator.cpp'
s=open(p).read()
s=s.replace('''    for (; !selected; --guard) {
        selected = guard < 1;
        short pick = Globals::rnd->nextInt(15);
        type = pick;''','''    while (!selected) {
        selected = guard < 1;
        int pick = Globals::rnd->nextInt(15);
        --guard;
        type = pick;''')
open(p,'w').write(s)
PY
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8

# original block / our block
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x971b8 --stop-address=0x97320 ../_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x971b8 --stop-address=0x97320 cmake-build-match/libgof2hdaa.so
```
