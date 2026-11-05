# SpaceLounge::drawLounge — wave-112 branch-weight pass (w6015)

Status: code change landed (build flag), 2026-11-05. Worktree `wt/w6015`, branch
`wave112-w6015`, base `5d98bd391`. Target: `_ZN11SpaceLounge10drawLoungeEv`
(1948/1948 B, `byte = linked = false`).

* before: raw **95.538778 %**, official **95.5 %**, 1948 B, 728/729 insns
* after:  raw **95.676047 %**, official **95.7 %**, 1948 B, 728/729 insns
* gate: `byte 4500 / linked 4500`, data 843/843, imports 207, lint CLEAN,
  drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

The change is a per-file `-mllvm -likely-branch-weight=7 -mllvm
-unlikely-branch-weight=1` override for `src/game/menu/SpaceLounge.cpp` in
`CMakeLists.txt`, next to the existing StarMap 7:1 override. **No `src/` change
landed**: every source shape tested this wave is a dead end (measurements in
§4). The override is a matching knob, not an original build flag; it is
gate-neutral but *not* score-neutral.

## 1. What the flag changes

`SpaceLounge.cpp` has exactly one `__builtin_expect` in the whole TU, at
`SpaceLounge::drawLounge` line 1211:

```cpp
if (__builtin_expect(agent->isKnown(), 1)) {
    width += Globals::Canvas->GetTextWidth(..., role + String("  "));
}
```

`-likely-branch-weight`/`-unlikely-branch-weight` only feed
`LowerExpectIntrinsic`, so the override changes the `!prof` weight of that one
branch from `2000:1` to `7:1` and nothing else. Diffing every exported function
between the base and 7:1 links shows **exactly one changed function**
(`drawLounge`, 1948 B both ways); `.text`, `.ARM.exidx`, `.ARM.extab`,
`.dynsym`, `.hash`, `.rel.plt` sizes and bases are unchanged, `.rodata` stays at
its pre-existing `0x41c27` (orig `0x41b67`, +0xC0, also present at base).
`.ARM.extab` keeps its size (`0x26298`) and base, but 4 bytes of `drawLounge`'s
LSDA call-site landing-pad offsets shift by 2 (`0x1c718d`, `0x1c7193`,
`0x1c7199`, `0x1c71d5`: `a8/a6/a4/a2` -> `a6/a4/a2/a8`), because the tail
cleanup blocks move. `.ARM.exidx` is byte-identical to base.

The only byte delta is 4 bytes in the tail EH landing-pad selector chain:

```text
offset  original   base(2000:1)  7:1
0x720   19         1a            1a
0x726   16         05            17
0x728   16 e0      16 e0         ff e7
```

i.e. the six `b.n` selectors at `0x171456..0x171460` branch to different
duplicate cleanup blocks. The block-frequency dump (`-mllvm
-print-machine-bfi`) shows the only BFI change in the whole TU is
`drawLounge`'s `bb.63`/`bb.67` successors:

```text
default: successors %bb.63(0x7fef9fcb), %bb.67(0x00106035)  ; 99.95 / 0.05
7:1:     successors %bb.63(0x70000000), %bb.67(0x10000000)  ; 87.50 / 12.50
```

`bb.63` is the width-add block; its frequency scales the landing pad for
`ir-block.142`, which reorders the four tail landing pads
(`ir-block.289/150/146/142`). `-print-machineinstrs=block-placement` shows the
7:1 order is `150, 146, 142, 289`; the default is `289, 150, 146, 142`.

## 2. Weight sweep

Full link + official score for `-likely-branch-weight=N
-unlikely-branch-weight=M` (only the ratio matters; the score is quantized):

| N:M | raw % | tail pattern (0x71e..) |
|---|---:|---|
| 7:1, 3:1, 3:2, 4:3, 5:2, 5:3, 6:2, 7:2, 8:3, 9:2, 9:3, 10:1, 11:2, 11:3, 13:3, 14:1, 14:2, 15:2, 20:2, 31:3 | **95.676047** | `09e01ae002e006e017e0ffe7` |
| 2000:1 (default), 15:1, 31:1, 31:2 | 95.538778 | `09e01ae002e001e005e016e004` |
| 1:1 (plain / 2:2) | 95.061728 | `09e01ae002e006e017e0ffe7` |
| 2:3 | 94.924554 | `04e008e019e006e017e0ffe7` |
| most other ratios | 88.598901 / 88.461538 | — |

No ratio reproduces the original tail exactly. Sweeping all `N,M` in 1..24
(object-only, `b[0x71e:0x72a]`) gives 329 ratios with the 7:1 pattern and
**zero** with the original `09e019e002e006e016e0ffe7` pattern. The original's
2-byte-smaller branch displacements are a consequence of the clamp/join being
2 bytes shorter (see §3), not of the branch weight.

`N=3,7,10,14` with `M=1` all give the identical best body; `7:1` was chosen
because it is the value the StarMap override already uses and the
`__builtin_expect` hint is "likely".

## 3. Why the residual cannot be closed by the weight

Original vs 7:1 `drawLounge` still differ in 175 bytes / 25 groups. The body
residual is exactly the wave-107..110 one:

```text
value        original        ours
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)

orig right arm:  itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1          (8 B)
ours right arm:  ittt gt; movgt.w r1,fp,lsl#16; addgt.w r1,r6,r1,asr#15;
                 subgt.w r9,r0,r1                                  (10 B)
```

The committed source keeps `short pad3 = pad;` to force the sign-extend
(`mov.w r1, fp, lsl #16; add.w r1, r6, r1, asr #15`) instead of reusing the
`sl = pad*2` that the `top` computation already materialised. The 2-byte-longer
clamp shifts every later block (including the cleanup landing pad at
`0x17148e` -> `0x171490`), which is the last 2 bytes of the tail diff. The
missing `mov r3, r5` / `mov r9, r3` pair in our post-clamp block is the same
root cause: the original splits `x`'s live interval at the join so `pad` can
live in `r9`, ours keeps `x` in `r9` for the whole function and spills `pad` to
`fp`.

Prior waves proved this allocation is not source-reachable with clang 7.0.2
(`_work/reconstructions/spacelounge-drawlounge-wave110-w5818.md` §2: the
`tryEvict` weight ordering needs `pad` selected before `%45/%46/%47`, which
needs a physical copy hint a plain field load cannot have). This wave did not
find a counterexample.

## 4. Source shapes measured this wave (all dead ends)

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6` (~1.2 s incremental), then the official
`verify.py` internals (`/dev/shm/w6015/score.py`, `autojunk=False`). The 7:1
override was active for the expect variants; plain variants have no
`__builtin_expect`, so the override does not touch them.

### 4.1 Width-branch guard forms (expect present, 7:1 active)

All of these compile to the identical body/tail and score **95.676047**:

```text
__builtin_expect(agent->isKnown(), 1)                  (committed)
__builtin_expect(agent->isKnown() != 0, 1)
__builtin_expect(agent->isKnown() == 1, 1)
__builtin_expect(agent->isKnown() > 0, 1)
__builtin_expect(agent->isKnown(), 1) && true
__builtin_expect(agent->isKnown(), 1) || false
__builtin_expect(agent->isKnown() ? 1 : 0, 1)
!__builtin_expect(!agent->isKnown(), 0)
if (__builtin_expect(!agent->isKnown(), 0)) {} else { ... }
```

`__builtin_expect(agent->isKnown(), 0)` (inverted hint) gives 95.538778;
`if (__builtin_expect(agent->isKnown(),1)) {} else { ... }` with the add in the
else grows the body to 1980 B and scores 78.932969.

### 4.2 Plain guards (no `__builtin_expect`)

Every comparison form compiles identically — the `icmp` is folded away and the
branch gets LLVM's no-heuristic weight **1:1** (`-print-machineinstrs` shows
`bb.61` successors `%bb.63(0x40000000), %bb.67(0x40000000)`):

```text
if (agent->isKnown())             95.061728  1948 B  729/729 insns
if (agent->isKnown() != 0)        95.061728
if (agent->isKnown() == 1)        95.061728
if (agent->isKnown() > 0)         95.061728
if ((int)agent->isKnown() != 0)   95.061728
if (agent->isKnown() != false)    95.061728
if (agent->isKnown() & 1)         95.061728
if (!!agent->isKnown())           95.061728
if (true == agent->isKnown())     95.061728
```

They *do* reproduce the 7:1 tail pattern (`09e01ae002e006e017e0ffe7`) but lose
0.6 points in the body. This is the closest thing to the "source-level branch
shape" the wave-111 hint asked for: the tail move is reproducible without a
flag, but it is not an improvement because the body regresses.

The natural zero-compare heuristic (20:12 = 5:3) would be in the good weight
set, but clang 7 InstCombine folds every `bool`-returning `isKnown()`
comparison back to `br i1 %call`, so the `icmp` never reaches
`BranchProbabilityInfo`. No source form tried keeps it.

### 4.3 Clamp forms (faithful `pad*2`)

```text
variant                                        raw        size  insns
committed `short pad3 = pad; ... pad3 * 2`     95.676047  1948  728/729
faithful `-(width + pad * 2)`                  93.964335  1948  729/729
faithful `-(width + (pad << 1))`               93.964335  1948  729/729
faithful `... - width - pad * 2`               93.964335  1948  729/729
faithful `... - width - pad - pad`             92.518874  1944  728/729
`int pad2 = pad * 2;` local                    93.964335  1948  729/729
```

The faithful forms get 729/729 instructions but reorder the whole post-clamp
block (the `sl` reuse moves `x`/`pad` around) and lose ~1.7 points.

## 5. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w6015/score.py cmake-build-match/libgof2hdaa.so
# block frequencies / placement (NDK clang 7.0.2)
<mangled-clang++> <TU flags> -mllvm -print-machine-bfi -c src/game/menu/SpaceLounge.cpp
<mangled-clang++> <TU flags> -mllvm -print-machineinstrs=block-placement \
  -c src/game/menu/SpaceLounge.cpp
```

## 6. Gate result (before -> after)

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
| `.ARM.extab` content | 4-byte LSDA landing-pad shift | 4-byte LSDA landing-pad shift |
| `.rodata` | 0x41c27 (orig 0x41b67) | 0x41c27 (unchanged) |

`docs/IDENTICALITY.md`'s current-state list is updated 95.5 -> 95.7 in the same
commit. The historical wave-109 section is left as-is.
