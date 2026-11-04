# SpaceLounge::drawLounge — wave-110 RA-ordering sweep (w5818)

Status: research note (2026-11-04). Worktree `wt/w5818`, branch
`wave110-w5818`, base `390f58052`. **No `src/` change landed**: no source
shape tested this wave beats the committed HEAD score. The tree is clean
apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* best measured: raw **95.538778%**, 1948 B, 728/729 insns (HEAD)
* faithful int `pad*2` clamp: raw **93.964335%**, 1948 B, 729/729 insns
* `width += cond ? ... : 0` width form: raw **95.061728%**, 1948 B, 729/729 insns

Prior work: `spacelounge-drawlounge-wave108-w5618.md` (newest),
`spacelounge-drawlounge-wave107-w5519.md`, `-w5016.md`, `-w4818.md`,
`-w4519.md`, `-w4246.md`, `-w4227.md`, `-w4118.md`, `-w4018.md`,
`-w3838.md`, `-w3818.md`, `SpaceLounge-OnRender3D.md`,
`SpaceLounge-layout.md`. This note records the wave-110 sweep and supersedes
nothing.

## 1. Baseline and residual

The committed source (HEAD) carries the wave-107 `short pad3` clamp form and
scores raw **95.538778** (728 instructions vs the original's 729, 1948/1948 B).
The complete residual is one contiguous region (pad load register, clamp right
arm, join, post-`drawBox` agent/top restore) plus the tail branch-target labels
and the literal-pool slotting that follows from the 2-byte size difference.

```text
value        original        ours (HEAD)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

Original right arm (3 insns, `itt gt` + 2):

```text
itt gt
addgt.w r1, r6, sl     ; width + pad*2   (sl = pad*2, kept from the `top` computation)
subgt   r5, r0, r1     ; x = (Globals::w - inset) - (width + pad*2)
```

HEAD right arm (4 insns, `ittt gt` + 3, the `short` sign-extend path):

```text
ittt gt
movgt.w r1, fp, lsl #16
addgt.w r1, r6, r1, asr #15
subgt.w r9, r0, r1
```

Original post-clamp block (the extra instruction HEAD lacks is the
`mov r3, r5` / `mov r9, r3` copy chain that lets `r5` be reused for
`top - pad` while `x` moves to `r9`, i.e. the original's `x` live interval is
split at the join):

```text
ldr r1, [pc, #476]
ldr.w fp, [r4, #48]      ; lineHeight
add r1, pc
add r0, sp, #56
movs r2, #0
blx String ctor
vmov r2, s16             ; top
mov r3, r5               ; x -> r3
add sl, r6               ; sl = pad2 + width
mov r1, r9               ; pad -> r1
subs r6, r3, r1          ; r6 = x - pad
str.w r8, [sp, #12]      ; spill agent
sub.w r5, r2, r9         ; r5 = top - pad   (pad dies here)
mov r9, r3               ; x -> r9          (reuses pad's r9)
mov r8, r2               ; top -> r8
strd sl, fp, [sp]
movs r1, #2
str r0, [sp, #8]
mov r0, r4
mov r2, r6
mov r3, r5
blx drawBox
```

## 2. RA mechanics (instrumented LLVM 7.0.2, same revision as NDK r18b)

Instrumented build at `/tmp/llvm_android_702b/build2/bin/clang++`
(`RA_TRACE=1`, `VRW`, `NOHINT`, `NOEVICT`, `PRIOVREG`, `FORCEVREG` knobs
compiled into `RegAllocGreedy.cpp` / `CalcSpillWeights.cpp`).  HEAD trace:

```text
select %608 x        size=4220 w=2.946139e-04 hint=$r3 -> $r5
select %607 width    size=1968 w=4.747330e-04 hint=$r0 -> $r6
select %47  boxW     size=768  w=6.080038e-04 hint=$r3 -> $r6
select %46  top-pad  size=736  w=6.251306e-04 hint=$r3 -> $r5   (evicts x from r5)
select %608 x        size=4220 w=2.946139e-04 hint=$r3 -> $r9   (r9 free -> whole)
select %45  x-pad    size=704  w=6.432504e-04 hint=$r2 -> $r6   (evicts boxW)
select %47  boxW     ...                          -> $r10
select %2   pad      size=6384 w=2.159248e-04 no hint -> spill (stage 2 -> fp)
```

The decisive step is `%46` (top-pad) at `tryAssign`: its hint `$r3` is
unavailable (kind=3, `%46` is live across the `String("")` ctor so it cannot
live in a caller-saved register), the first free register is `$r9` (cost 1,
high register), and `tryEvict` then picks the cheap low register `$r5` holding
`x` (weight 2.95e-4 < 6.25e-4) and evicts it.  `x` is re-enqueued with
priority `(1<<31)|(1<<30)|4220` and immediately re-selected into `$r9`, so
`pad` (unhinted priority `(1<<29)|6384`, always selected after every hinted
value) finds every allocatable register occupied and spills to `fp`/`r11`.

Weight formula (`normalizeSpillWeight`):
`weight = UseDefFreq / (Size + 25*InstrDist)`.
`x` has `UseDefFreq ~= 1.27`, `Size = 4220`; `%46` has `UseDefFreq ~= 0.52`,
`Size = 736`.  To make `%46` stop evicting `x`, `x`'s weight must exceed
6.25e-4 (i.e. `Size < ~1930`) or `%46`'s must drop below 2.95e-4 (i.e.
`Size > ~1650`).

Instrumented knob probes (scratch compiler only, not source-reachable):

| knob | result |
|---|---|
| `VRW=2147484256:2.5` (x weight 7.37e-4) | x stays r5, but `%47` (boxW) takes r9; pad still spills |
| `PRIOVREG=2147483650` (boost pad priority) | pad -> r5, x -> r6, everything reshuffles |
| `NOHINT=2147484255` (clear x hint) | x -> r10, pad -> r4 |
| `NOEVICT=2147484255` | pad -> r10, pad2 -> r9, x -> r5 (closest, pad != r9) |
| `FORCEVREG=2147483650:75 FORCEEVICT=1` | clang assert (register 75 = R9) |

So **pad->r9 is necessary but not sufficient**, and no source-level weight
change reaches it: the target needs pad selected before `%46`/`%47`/`%45`,
which requires a physical copy hint that a plain field load cannot have.

## 3. Source shapes measured this wave (all <= 95.538778)

Harness: write the variant over `src/game/menu/SpaceLounge.cpp`,
`cmake --build cmake-build-match --target link_so -j6`, then the official
`tools/verify/verify.py` score on the linked `.so`
(`/dev/shm/w5818/score.py`, `autojunk=False`).

```text
variant                                             raw        size  insns
base (HEAD, short pad3)                             95.538778  1948  728/729
faithful -(width + pad * 2)                         93.964335  1948  729/729
faithful + pad2 local (varA)                        93.964335  1948  729/729
flat -(width - pad - pad)                           95.473251  1948  729/729
width += cond ? ... : 0   (w_ternary)               95.061728  1948  729/729
width = width + ... (w_plus)                        95.538778  1948  728/729
clamp int16 / inline (short) pad*2                  95.538778  1948  728/729
DrawRectangle uses lineHeight                       87.895461  1948  725/729
volatile pad                                        66.075085  1980  736/729
short pad                                           75.427789  1960  732/729
unsigned x                                          95.401510  1948  728/729
float centerXf, cast at clamp                       87.577213  1948  728/729
centerX/ top moved after width                      95.538778  1948  728/729
lineHeight before clamp / after layout              91.643836  1952  731/729
layout before width computation                     79.041096  1968  731/729
no pad local, direct field access                   78.458362  1956  724/729
String() instead of String("")                      92.022008  1940  725/729
```

### 3a. The `w_ternary` width form (729/729 insns)

```cpp
int width = Globals::Canvas->GetTextWidth(...);
width += agent->isKnown() ? Globals::Canvas->GetTextWidth(..., role + String("  ")) : 0;
```

This is the only shape found that reproduces the original's **729**
instruction count and the original's post-clamp block *structure*
(`mov r1, <pad>`, `sub.w r6, <x>, r1`, `subs r5, r2, <pad>`, `mov r8, r2`,
`strd sl, fp, [sp]`, `mov r2, r6`, `mov r3, r5`).  It still scores lower
(95.061728) because the register pair is swapped: `pad -> r5`, `x -> r9`
(with an extra `mov r5, r9` before the `vmov r9, s18`).  Combining it with
the faithful int clamp or with the second-load trick
(`DrawRectangle(x - layout->field_0x94, ...)`) does not recover the target
pair *and* the schedule at once.

### 3b. Shapes that DO reach pad->r9 / x->r5 (but lose elsewhere)

| shape | raw | size | why it loses |
|---|---|---|---|
| `int boxY = top - pad;` + `pad2` local (varA+boxY) | 72.043746 | 1976 | boxY live from early; `top` spills to stack, +28 B |
| `DrawRectangle(x - layout->field_0x94, top - layout->field_0x94, ...)` | 91.546392 | 1948 | second pad load; x-pad/top-pad are sunk after the `String` ctor instead of shared across it, 726 insns |
| same, with `boxX`/`boxY` locals | 91.546392 | 1948 | identical MIR |
| `DrawRectangle(x - layout->field_0x94, top - pad, ...)` | 92.646048 | 1948 | pad -> r10, x -> r9 |

The second-load shape is interesting because it raises `x`'s weight
(`UseDefFreq` 1.27 -> 1.49, `Size` 4236, `w = 3.446576e-04`) above `%46`'s
(`Size` 1008, `w = 3.362430e-04`), so `%46` no longer evicts `x`; `x` stays
in `r5` and `pad` gets `r9`.  But because the DrawRectangle now reads a
*different* pad value, `x - pad` and `top - pad` are no longer shared between
`drawBox` and `DrawRectangle`, and the scheduler sinks them after the
`String("")` ctor (6 fewer instructions, but the wrong ones).

## 4. Hypothesis (not proven)

The original's `x` live interval is split at the join (the `mov r3, r5` /
`mov r9, r3` chain), so `x` occupies `r5` up to `top - pad`, `pad` occupies
`r9` until it dies at that same instruction, and `x` then takes over `r9`.
Reproducing it from source needs either (a) `x`'s pre-join piece to outrank
`%46` in `tryEvict` while `%46`/`%47`/`%45` all avoid `r9`, or (b) a
source-level construct that makes the register allocator split `x` at the
join instead of relocating the whole interval.  Neither was reachable from
any C++ shape tested in waves 106–110.

## 5. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w5818/score.py [path/to/libgof2hdaa.so]
# RA trace (instrumented LLVM 7, same revision as NDK r18b)
RA_TRACE=1 /tmp/llvm_android_702b/build2/bin/clang++ <TU flags> -S \
  -o /dev/null src/game/menu/SpaceLounge.cpp 2> ra.txt
```

## 6. Gate result (unchanged, no src/ change landed)

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN
drift:  0 field(s)
```

No ELF section, `.ARM.extab`, `.rodata`, `.dynsym` or file-size delta was
introduced (no `src/` change).
