# SpaceLounge::drawLounge — wave-114 eviction-weight sweep (w6218)

Status: research note (2026-11-05). Worktree `wt/w6218`, branch `wave114-w6218`,
base `d8557f7e2`. **No `src/` change landed**: every source shape measured this
wave scores <= the committed HEAD. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `byte 4500 / linked 4500`, data 843/843, imports 207, lint CLEAN,
  drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave113-w6117.md` (exhaustive source-shape
sweep), `-wave112-w6015.md` (7:1 branch weight), `-wave110-w5818.md` (RA trace),
`-wave108-w5618.md` (shape table), `spacelounge-drawlounge-ra-split-w3838.md`.
This note records the wave-114 sweep and supersedes nothing.

## 1. Residual (unchanged)

The whole function is identical except one contiguous region plus the tail EH
landing-pad selectors:

```text
value        original        ours (HEAD)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11) -> r5 (split)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)

orig right arm:  itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1              (8 B)
ours right arm:  ittt gt; movgt.w r1,fp,lsl#16; addgt.w r1,r6,r1,asr#15;
                 subgt.w r9,r0,r1                                      (10 B)
```

## 2. The exact RA gate: `shouldEvict` compares spill weights

Wave-113 framed the blocker as "pad is unhinted and selected last". This wave
pinned the *decisive comparison* down to one line of LLVM 7.0.2
(`lib/CodeGen/RegAllocGreedy.cpp`):

```cpp
bool RAGreedy::shouldEvict(LiveInterval &A, bool IsHint, LiveInterval &B, bool BreaksHint) {
  bool CanSplit = getStage(B) < RS_Spill;
  if (CanSplit && IsHint && !BreaksHint) return true;
  if (A.weight > B.weight) return true;      // <-- the gate
  return false;
}
```

`tryEvict` is entered for `%46`/pad only when the first free register has
`getCostPerUse() == 1` (r8–r11). It then walks the allocation order and keeps
the *cheapest* eviction (`EvictionCost`: broken hints first, then max weight).

Measured weights in `drawLounge` (instrumented clang 7.0.2, `RA_TRACE=1`):

```text
vreg  what                 size   weight        prio
%608  x (COPY %386:spr)    4220   2.946140e-04  0xE000107C  (hint $r3)
%46   top-pad              736    6.251309e-04  0xE00002E0  (hint $r3)
%2    pad                  6384   2.159249e-04  0xA00018F0  (no hint)
%365  GOT[Globals::Canvas]  -     2.145276e-04
%1    agents data          8784   2.263368e-04
```

HEAD selection order (unchanged from wave-113):

```text
select %608 x  -> $r5
select %46     -> tryEvict(x from $r5), $r5   (6.25e-4 > 2.95e-4)
select %608 x  -> $r9 (free)
...
select %2 pad  -> all regs taken; tryEvict picks $r11 (%365 w=2.145e-04
                  < pad 2.159e-04); assign %2 -> $r11
```

So pad can only reach `$r9` by **evicting x from r9**, and that requires
`pad.weight > x.weight` (2.159e-04 > 2.946e-04 — false today). The hint on x is
*not* the blocker: `BreaksHint` uses `hasPreferredPhys()` (is x currently in
its hint r3?) which is false, and `IsHint` is false inside `tryEvict`.

Verifying with the instrumented compiler's `XWFACTOR` knob (scales the weight
of the `COPY %spr` interval, i.e. x; not source-reachable):

| XWFACTOR | x weight | pad lands in |
|---|---:|---|
| 1.0 (HEAD) | 2.946e-04 | r11 (evicts %365) |
| 0.8 | 2.357e-04 | r11 |
| 0.75 | 2.210e-04 | r11 |
| 0.73 | 2.151e-04 | r11 |
| 0.7 | 2.062e-04 | **r8** (x moved to r8, %1 took r9) |
| 0.6 | 1.768e-04 | r8 |
| 0.5 | 1.473e-04 | r8 |
| 0.3 | 8.838e-05 | r11 (whole allocation reshuffles) |

Even when pad can evict x, x is no longer in r9: lowering x's weight makes
x take r8 and pushes `%1` (2.263e-04) into r9, which is heavier than `%365` on
r11, so r11 still wins. This is the same "pad->r9 is necessary but not
sufficient" wall wave-110/113 hit, now stated as a weight-ordering condition.

## 3. Branch-weight sweep (pad register only)

`-mllvm -likely-branch-weight=N -mllvm -unlikely-branch-weight=M` changes the
block frequencies and therefore the spill weights. Sweeping N:M (object-only):

| ratio | pad register |
|---|---|
| 1:1, 1:2, 1:3, 1:4, 1:5, 1:6, 1:8, 1:10, 1:16, 1:32, 2:3, 2:5, 3:4, 3:5, 4:5 | **r5** |
| 2:1, 3:1, 4:1, 5:1, 6:1, 7:1, 8:1, 10:1, 15:1, 20:1, 31:1, 50:1, 100:1, 2000:1, 5:4, 4:3, 5:3, 7:3, 8:3, 10:3, 16:3, 32:3, 64:3, 128:3, 256:3, 512:3, 1024:3 | r11 |

**No ratio produces r9.** The 7:1 override (already committed) is in the r11
class; the r5 class costs ~0.6 points of body score (wave-112 §4.2).

## 4. Source shapes measured this wave (all <= 95.676047)

Harness: `git checkout` the TU, substitute the whole `drawLounge` body, then
`cmake --build cmake-build-match --target link_so -j6` (~1.2 s) and the
official `verify.py` internals (`/dev/shm/w6218/score.py`, `autojunk=False`).
The 7:1 `-likely-branch-weight` override was active throughout.

### 4.1 Clamp-expression forms (short-pad3 family)

```text
variant                                             raw         size  insns
HEAD (short pad3, `-(width + pad3 * 2)`)            95.676047   1948  728/729
`(width + (pad3 << 1))`                             95.676047   1948  728/729
`(width - -(pad3 * 2))`                             95.676047   1948  728/729
`(width - (0 - pad3 * 2))`                          95.676047   1948  728/729
`(width + 2 * pad3)`                                95.676047   1948  728/729
`short pad3 = (short) pad;`                         95.676047   1948  728/729
`short pad3 = (short) (pad & 0xffff);`              95.676047   1948  728/729
`signed char pad3 = pad;`                           95.676047   1948  728/729
`(width + pad3 + pad3)`                             92.4        1952  730/729
`(width + (short) (pad * 2))`                       93.8        1948  729/729
`unsigned short pad3`                               91.7        1956  732/729
faithful `-(width + pad * 2)`                       93.964335   1948  729/729
faithful `-(width + pad + pad)`                     95.473251   1948  729/729
`int pad3 = pad`                                    93.964335   1948  729/729
`int pad2 = pad + pad` (also used for top/drawBox)  93.964335   1948  729/729
`int pad2 = pad * 2`                                93.964335   1948  729/729
```

### 4.2 Declaration order / placement (all identical code)

```text
`int x` before `int pad`                            95.676047
`int x` after `int top`                             95.676047
`short pad3` after `int top`                        95.676047
`int lineHeight` after the drawBox call             95.676047
`int top` before `int centerX`                      95.676047
`int pad2 = pad*2` local used only for `top`        95.676047
`int boxW = pad*2+width` local before the clamp     95.676047
`int boxX/boxY/boxW` locals                         95.676047
same locals reused by DrawRectangle                 95.676047
`int inset = layout->buttonInsetX` local            95.676047
`int origX = centerX; int x = origX;`               95.676047
`const int pad`                                     95.676047
`long x`                                            95.676047
`unsigned centerX`                                  95.676047
`Array<Vector*>* rects = this->agentRects` local    95.676047
`Array<Agent*>* agents = this->agents` local        95.676047
`width + pad*2` vs `pad*2 + width` (all 3 sites)    95.676047
```

### 4.3 Shapes that move pad off fp (but not to r9)

```text
variant                                             raw         pad   size
plain `if (agent->isKnown())` + width ternary       95.061728   r5    1948
`if (agent->isKnown()) { width += ...; }`           95.061728   r5    1948
faithful clamp + lineHeight before the clamp        95.198903   r5    1948
`boxY`/`boxX` locals after the clamp                93.1        r5    1948
`int boxW` before the clamp + drawBox uses it       71.1        r5    1964
`int pad` read after `SetColor`                     93.1        ?     1948
`int pad` read after `int centerX`                  93.1        ?     1948
DrawRectangle uses `lineHeight`                     87.895461   r5    1948
both DrawStrings use `centerX` instead of x         80.300958   **r9** 1956
```

The last row is the only shape measured **this wave or in waves 106–113** that
puts pad in r9, and it does so by keeping `centerX` live past the join (extra
stack slot, 1956 B, 80.3 %). It confirms the mechanism (pad gets r9 when x is
not in r9 at pad's selection) but is not the original source.

### 4.4 Other dead ends

```text
variant                                             raw         size
`Vector *topLeft` / `Vector *bottomRight` pointers  91.4        1948
`Vector *topLeft` only                              93.6        1948
`unsigned pad`                                      95.401510   1948
`unsigned x`                                        95.538778   1948
`unsigned pad` + `unsigned x`                       95.401510   1948
`(unsigned)(x-pad) < (unsigned)inset`               95.538778   1948
`String name`/`role` as if/else instead of ?:       79.6/76.2/73.5  1988/2028/2068
`String()` instead of `String("")`                  82.9        1900
`int top = (int)bottomRight.y - pad - pad`          81.6        1960
second `Globals::layout->field_0x94` load in top-pad 81.2       1976
`short pad`                                         75.4        1960
`drawBox(..., pad + pad + width, ...)`              72.0        1964
DrawRectangle `pad3*2+width`                        71.8        1980
DrawRectangle `pad+pad+width`                       72.0        1964
`int top` moved after the width computation         83.0        1960
`int centerX` moved after the strings               70.2        1972
inline `int x = (int)(...)` (no centerX)            70.2        1972
```

`__builtin_expect_with_probability` is **not available** in NDK r18b clang
7.0.2 (`use of undeclared identifier`), so the 7:1 weight cannot be expressed
in-source that way.

## 5. Conclusion

The residual is not a wrong constant, field offset, callee, block order,
literal-pool slot or clamp source shape. It is the greedy allocator's
`shouldEvict` weight comparison: pad (2.159e-04) cannot evict x (2.946e-04)
from r9, and every source-reachable change that lowers x's weight also moves
x out of r9 (to r8) before pad is selected. The original's `x` live-interval
split (`mov r3,r5` / `mov r9,r3`) needs the PHI copy `%43 = COPY %608` to
survive coalescing, which no C++ shape tested in waves 106–114 achieves.

## 6. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w6218/score.py
# RA trace (instrumented LLVM 7, same revision as NDK r18b)
RA_TRACE=1 /tmp/llvm_android_702b/build2/bin/clang++ <TU flags> -S -o /dev/null \
  src/game/menu/SpaceLounge.cpp 2> ra.txt
XWFACTOR=0.7 RA_TRACE=1 ...   # weight-scaling probe (not source-reachable)
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
