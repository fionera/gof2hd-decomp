# SpaceLounge::drawLounge — wave-115 branch-weight source-shape probe (w6318)

Status: research note (2026-11-05). Worktree `wt/w6318`, branch
`wave115-w6318`, base `b847d5d46`. **No `src/` change landed**: the
committed HEAD is still the best measured shape. The tree is clean apart
from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `byte 4500 / linked 4500`, data 843/843, imports 207, lint CLEAN,
  drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave114-w6218.md` (eviction-weight
sweep), `-wave113-w6117.md` (exhaustive source-shape sweep),
`-wave112-w6015.md` (the 7:1 flag), `-wave110-w5818.md` (RA trace).
This note records the wave-115 probe of the coordinator hint
"find the source-level branch shape that reproduces the per-file 7:1
branch weight without the flag" and supersedes nothing.

## 1. Why the flag is there (recap)

`CMakeLists.txt` applies `-mllvm -likely-branch-weight=7 -mllvm
-unlikely-branch-weight=1` to `SpaceLounge.cpp`. The only
`__builtin_expect` in the TU is the width-add guard at line 1211:

```cpp
if (__builtin_expect(agent->isKnown(), 1)) {
    width += Globals::Canvas->GetTextWidth(..., role + String("  "));
}
```

`LowerExpectIntrinsic` writes `!prof` `7:1` on that branch. The weight
changes the EH landing-pad block frequencies and therefore the greedy
register allocator's spill weights. At the default `2000:1` the body is
still `pad -> fp`, but the tail landing pads are ordered differently
(raw 95.538778). At `1:1` (plain guard) the tail is right but the body
moves `pad -> r5` (raw 95.061728). `7:1` is the only tested weight that
gives both (`pad -> fp`, correct pad order), raw 95.676047.

## 2. The only source-level weight mechanism: a zero-compare `icmp`

`BranchProbabilityInfo` (LLVM 7.0.2, `lib/Analysis/BranchProbabilityInfo.cpp`)
is the only way a C++ shape can set a branch weight without
`__builtin_expect`:

| heuristic | weights | ratio |
|---|---|---|
| `calcZeroHeuristics` (`icmp eq/ne/slt/sgt` vs 0) | `ZH_TAKEN_WEIGHT=20`, `ZH_NONTAKEN_WEIGHT=12` | **20:12 = 5:3** |
| `calcPointerHeuristics` (`icmp eq/ne` on a pointer) | `PH_TAKEN_WEIGHT=20`, `PH_NONTAKEN_WEIGHT=12` | 20:12 |
| `calcFloatingPointHeuristics` | `FPH_TAKEN_WEIGHT=20`, `FPH_NONTAKEN_WEIGHT=12` | 20:12 |
| `calcLoopBranchHeuristics` | `124:4` | 31:1 |
| `calcColdCallHeuristics` | `4:64` | 1:16 |

Wave-112 §2 already showed that `5:3` is in the "good" ratio class
(95.676047), so a natural `20:12` would be a drop-in replacement for the
flag. The blocker is that `Agent::isKnown()` returns `bool` (confirmed by
the original body at `0x17756c`: `cmp r1,#0; it gt; movgt r0,#1`), so
clang 7 emits `br i1 %call` and `InstCombine` folds every comparison form
back to the raw `i1`:

```llvm
; all of these compile identically
if (agent->isKnown())
if (agent->isKnown() != 0)
if (agent->isKnown() == 1)
if ((int)agent->isKnown() != 0)
if (!!agent->isKnown())
```
=> `%1 = call zeroext i1 @isKnown(); br i1 %1` -> BPI's no-heuristic
`1:1`.

## 3. The one shape that survives: `int Agent::isKnown()`

Changing the declaration/definition to `int isKnown()` keeps the mangled
name and the ABI (r0), and makes the guard
`%1 = call i32 @isKnown(); %2 = icmp ne i32 %1, 0; br i1 %2`, which does
reach `calcZeroHeuristics` (`20:12 = 5:3`). Measured (full link + the
official `verify.py` internals, `autojunk=False`):

| variant | raw % | size | insns |
|---|---:|---:|---:|
| HEAD (bool, expect 7:1, `short pad3`) | **95.676047** | 1948 | 729/728 |
| `int isKnown` + plain guard + `short pad3` | 77.587389 | 1956 | 729/730 |
| `int isKnown` + plain guard + faithful `pad*2` | 77.587389 | 1956 | 729/730 |
| `int isKnown` + `__builtin_expect` (7:1) + `short pad3` | 81.400137 | 1952 | 729/728 |
| `int isKnown` + `__builtin_expect` (7:1) + faithful | 81.207133 | 1952 | 729/729 |

The `int` return is **not** a drop-in: the extra `icmp` survives to
codegen and the allocator spills the call result
(`blx isKnown; str r0,[sp,#96]; ldr r0,[sp,#96]; cbz r0`) — +8 bytes and
a whole-function RA shift (`x -> r9`, `pad -> r5`, `r5/r6` swap at the
top). The zero-compare weight is correct, but the IR is no longer the
original's. The original's four `blx isKnown` sites are all followed by a
bare `cbz/cbnz r0`, which is why the `bool` signature is kept.

## 4. Other source shapes measured this wave (all <= 95.676047)

Harness: `git checkout` the TU, substitute the whole `drawLounge` body,
`cmake --build cmake-build-match --target link_so -j6` (~1.2 s) and the
official `verify.py` internals (`/dev/shm/w6318/score.py`,
`autojunk=False`). The 7:1 override was active unless noted.

### 4.1 Guard forms (pad3 clamp)

```text
variant                                             raw         size
HEAD `__builtin_expect(isKnown(),1)`                95.676047   1948
`__builtin_expect(isKnown() != 0, 1)`               95.676047   1948
`__builtin_expect(isKnown() == 1, 1)`               95.676047   1948
`!__builtin_expect(!isKnown(), 0)`                  95.676047   1948
`width += __builtin_expect(isKnown(),1) ? ... : 0`  95.676047   1948
plain `if (isKnown())`                              95.061728   1948
plain ternary `width += isKnown() ? ... : 0`        95.061728   1948
switch (isKnown()) { case 1: ... }                  95.061728   1948
```

### 4.2 Clamp forms (expect 7:1 active)

```text
variant                                             raw         size
HEAD `short pad3 = pad; ... pad3 * 2`               95.676047   1948
inline `(short) pad * 2`                            95.676047   1948
inline `(int)(short) pad * 2`                       95.676047   1948
`(pad3 << 1)` / `2 * pad3` / `- width - pad3 * 2`   95.676047   1948
`- width - pad3 - pad3`                             93.964335   1948
faithful `-(width + pad * 2)`                       93.964335   1948
`int pad3 = pad` / `int pad2 = pad + pad`           93.964335   1948
plain guard + faithful clamp                        93.689986   1948
```

### 4.3 Clamp CFG shapes (expect/plain)

```text
variant                                             expect      plain
HEAD else-if                                        95.676047   95.061728
nested `else { if (...) ... }`                      95.676047   95.061728
negated first condition (`>=` then inner `if`)      95.676047   95.061728
cache `int right = Globals::w - inset`              95.676047   95.061728
```

### 4.4 Moving the `__builtin_expect` elsewhere (7:1 active)

```text
variant                                             raw         size
HEAD (width guard)                                  95.676047   1948
width + final `if (isKnown())`                      94.093407   1948
ternary name suffix `isKnown() ? "  " : ""`         91.643836   1952
final `if (isKnown())` only                         86.675824   1948
name selection `isKnown() || isStoryAgent()`        78.028747   1960
name + final                                        74.023304   1960
```

Adding an extra expect to `if (mode == 0)` or `if (hoverAgent < 0)`
changes the block frequencies too much: 50.2 / 76.5 (both with the
width-guard expect still present).

### 4.5 Faithful clamp ratio sweep

With the faithful `pad * 2` clamp the right arm is the original's
`itt gt; addgt.w r1,r6,sl; subgt r0,r1` (8 B), but `pad` is `fp` and `x`
is `r9` for every weight; `pad -> r9` never appears:

| N:M | raw % | pad/x | tail bytes vs orig |
|---|---:|---|---:|
| 1:1 | 93.689986 | r5/r9 | 11/148 |
| 2:1 .. 6:2, 8:3, 9:2, 20:12, 31:3, 2000:1 | 93.964335 | fp/r9 | 33/148 |
| **7:1** | 93.964335 | fp/r9 | **81/148** |
| 8:1, 10:1, 15:1, 20:1, 31:1, 100:1 | 86.890872 | fp/r9 | 25/148 |

So even the faithful clamp + 7:1 keeps `x` in `r9` and leaves an extra
duplicate `b.n` in the EH landing-pad chain (the original has six
selectors and one `b.n`; ours has six selectors and two). This is the
same wave-114 `shouldEvict` wall.

## 5. Conclusion

The 7:1 per-file flag cannot be replaced by a source-level branch shape
with the current `bool Agent::isKnown()`: clang 7.0.2's InstCombine folds
`icmp ne i1 %call, false` to `%call`, leaving BPI's `1:1` default. The
only surviving zero-compare (`int isKnown()`, `20:12`) adds an `icmp`
plus an 8-byte spill of the call result and shifts the whole-function
register allocation, dropping the score from 95.676047 to 77.587389.
The remaining `95.7 -> 100` gap is still the wave-110/113/114 greedy-RA
`shouldEvict` ordering (`pad` cannot evict `x` from `r9`), not a branch
shape.

## 6. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w6318/score.py
# branch-weight source of truth (LLVM 7.0.2)
sed -n '657,780p' /tmp/llvm_android_702b/lib/Analysis/BranchProbabilityInfo.cpp
```

## 7. Gate result (before -> after)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4500 / 4524 | **4500 / 4524** |
| linked_exact | 4500 / 4524 | **4500 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |
| sections | 27 exact | 27 exact |
| file size | 2,187,328 B | 2,187,328 B |
| `.text` / `.ARM.exidx` / `.ARM.extab` | size+base exact | size+base exact |
| `.rodata` | 0x41c27 (orig 0x41b67) | 0x41c27 (unchanged) |

No `src/` change: no ELF section, `.ARM.extab`, `.rodata`, `.dynsym` or
file-size delta was introduced.
