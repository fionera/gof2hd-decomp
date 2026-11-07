# w7733 (wave-133): `Level::createMission` — tail-loop `size()` re-read + register-allocation study (no source win)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7733`, based on master `144fc16b0`.
Continues `level-createmission-wave132-w7710.md`, `…-wave131-w7700.md`,
`…-wave130b-w7677.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; linked `.text` `0xaffd8`, 17172 B). Build/flags unchanged
(NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.2 %   size 17172/17172 B
.text diff_bytes 85244  (whole section)
.ARM.extab diff 1416  .ARM.exidx diff 0  .rodata/.data/.bss exact
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

Note: the w7710 note quotes `.text` 85376; the actual current master measures
**85244**.  Use 85244 as the baseline.

## Method: exact byte-delta map instead of guessing regions

`/dev/shm/w7733/opdelta2.py` parses both objdumps, aligns the instruction
streams with `difflib`, and prints every replace/insert/delete block with the
**byte-size delta** (orig minus ours).  This isolates where the 17172 B are
spent.  The function's differences are not one contiguous region: they are ~90
small blocks, almost all register-allocation/scheduling permutations.

### The tail block (`case4Else` … `Objective`) is already size-neutral

`regionsum.py` sums instruction bytes from the `case4Else` entry to the
`Objective` constructor:

```text
orig b40b6..b41ec : 310 B / 98 insns
ours b40a8..b41da : 308 B / 99 insns   (baseline)
```

The per-block deltas inside the tail sum to **0**.  The residual differences
are all register/stack-slot choices:

| item | original | ours (baseline) |
|---|---|---|
| `this` in the tail | r8 (reloaded at `case4Else`, `ldr.w r8,[sp,#124]`) | sl (no reload; `this` stays in sl) |
| `Globals::globals` | sl | r8 |
| `Globals::rnd` | re-read from `[sp,#120]` each iteration | hoisted into fp |
| `true` argument | `mov.w fp,#1` hoisted + `strd r0,fp,[sp]` | `movs r0,#1; str r0,[sp,#4]` |
| `hunterCount` slot | `[sp,#124]` (same slot `this` occupied at entry) | `[sp,#120]` |
| loop condition | `ldr [r8,#248]; ldr [r0]; cmp; bcc` (`this->enemies->size()` re-read) | `ldr [sp,#312]; cmp; bcc` (`volatile n`) |

The `hunterCount` slot follows directly from the reload: the original frees
`[sp,#124]` by moving `this` to r8 at `case4Else`; ours keeps `this` live in
sl until after the loop, so `hunterCount` goes to `[sp,#120]`.

## Hypothesis tested: drop `volatile unsigned n`, use `this->enemies->size()`

This reproduces the original loop condition exactly, but the function shrinks
**17172 -> 17164 B** (the 8 bytes are the missing `case4Else` `this` reload,
the `fp=1` hoist, and the extra branch-path reload, partly offset by `strd`).
An 8-byte shrink shifts every following function, so the section diff explodes:

```text
no-n tail loop:  .text 832423  size 17164  extab 1443  exidx 1645
```

(The handoff note called 832423 a "huge improvement"; it is 10x *worse* than
85244 — it is the address-shift artefact, not a code win.)

### Padding sweep to restore 17172 (all measured with fresh `link_so`)

| variant | `.text` | size | extab | exidx |
|---|---:|---:|---:|---:|
| **baseline `volatile unsigned n`** | **85244** | **17172** | **1416** | **0** |
| no-n + `volatile unsigned n = 0; (void)n;` before loop | 85274 | 17172 | 1416 | 0 |
| no-n + `volatile unsigned n = 0;` after loop | 85283 | 17172 | 1416 | 0 |
| no-n + `volatile unsigned n = hunterCount; (void)n;` | 85275 | 17172 | — | — |
| no-n + `volatile int n = hunterCount; (void)n;` | 85275 | 17172 | — | — |
| no-n + `volatile unsigned n = 0;` before `ArraySetLength` | 85274 | 17172 | — | — |
| `unsigned n` (non-volatile) + `i < n` | 85250 | 17172 | — | — |
| `volatile unsigned n` + `(void)n` + `i < n` | 85254 | 17172 | — | — |
| no-n + `Level *volatile self = this; (void)self;` in `case4Else` | 85256 | 17172 | **1386** | 0 |
| no-n + `Level *volatile self = this;` used for `stationPosition` | 875735 | 17180 | 2075 | 3553 |

Every padding variant that restores 17172 is **worse** in `.text` than the
baseline.  The `volatile unsigned n` is already the cheapest way to hold the
size at 17172.

## Other hypotheses tested this session (all neutral or worse)

- **case-183 `offset[3]` chain** (w7710 landed ascending order): re-tested
  descending and `switch` — neutral/worse (85247 / 85244).  Confirmed optimal.
- **`case 4` branch restructures** with the no-n loop: `Route *r = …;
  this->playerRoute = r;`, `if (==0) goto else; else {…}`, `Level *self = this;`
  — all still 17164 B, i.e. the compiler still hoists the `this` reload into sl
  before the branch.  The reload placement is not source-order controllable.
- **`case4Else` `Level *self = this;` / `Level &self = *this;`** — neutral
  (85244, 17172).  Coalesced away.
- **branchless `&&`** (`(i==X) & alienWave`, `& (alienWave != 0)`,
  `alienWave && i==X`, `alienWave != 0`): 92418 / 85244 / 85244 / 85244.
  The original's `subs/it/movne/eor/orrs` if-conversion is a backend choice.
- **`pirateBase` declaration**: `Vector pirateBase;` (no zero init) — neutral.
- **`int first` / `int escortsEnd` / `int start` reorders and inlining**:
  `first = stationPirates + guns` neutral; `first` before `pirateBase` neutral;
  `escortsEnd = first + pirateEscorts + pirates` neutral;
  `escortsEnd = first + pirates + pirateEscorts` worse (87373/17168);
  inlining `start` worse (87262/17168).
- **`Vector pirateBase` register study** (line 2050): original keeps
  `pirateBase` in s16/s18/s24 (loaded once per iteration) while ours reloads
  s2/s4/s6 from `[sp,#312]` later in the body.  Same instruction count and
  bytes; pure scheduling/register-name difference, not source order.

## Conclusion

No source win this wave.  The baseline `volatile unsigned n` tail loop is the
best size-exact formulation; the remaining tail residual is a single
register-allocation decision (move `this` from sl to r8 at the `case4Else`
block boundary), after which `Globals::globals` moves to sl, `Globals::rnd`
falls back to the stack, `fp` becomes the literal 1, and `hunterCount` reuses
`[sp,#124]`.  No source spelling tried (goto polarity, local aliases, volatile
anchors, branchless `&&`, declaration placement) makes clang 7.0.2 take that
decision.  The global opdelta map (≈90 small blocks) is the right tool for the
next wave: attack the largest blocks first (b2e04 branchless `&&`, b3c6c
`pirateBase` register choice, b3dda `start` arithmetic), but each is a backend
permutation rather than a source-order bug.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7733/score.py                 # section + function-size diff
python3 /dev/shm/w7733/opdelta2.py /dev/shm/w7733_ours.asm   # per-block byte deltas
python3 /dev/shm/w7733/regionsum.py /dev/shm/w7733_ours.asm  # tail-region byte sum
cmake --build cmake-build-match --target check -j6            # RATCHET PASS
```

Scratch: `/dev/shm/w7733/{Level.base.cpp,Level.non.cpp,score.py,build_score.sh,
opdiff.py,opdelta2.py,regionsum.py,batch*.py,orig.asm,ours.asm,ours_non.asm,
ours_v2.asm,unstripped.so}`.

## Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85244 -> 85244
```
