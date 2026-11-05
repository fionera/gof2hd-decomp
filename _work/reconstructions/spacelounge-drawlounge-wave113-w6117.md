# SpaceLounge::drawLounge — wave-113 exhaustive source-shape sweep (w6117)

Status: research note (2026-11-05). Worktree `wt/w6117`, branch `wave113-w6117`,
base `a979ace18`. **No `src/` change landed**: every source shape measured this
wave scores <= the committed HEAD. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `byte 4500 / linked 4500`, data 843/843, imports 207, lint CLEAN,
  drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave112-w6015.md` (7:1 branch weight),
`-wave110-w5818.md` (RA trace), `-wave108-w5618.md` (shape table),
`spacelounge-drawlounge-ra-split-w3838.md` (split/eviction analysis). This note
records the wave-113 sweep and supersedes nothing.

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
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)

orig right arm:  itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1              (8 B)
ours right arm:  ittt gt; movgt.w r1,fp,lsl#16; addgt.w r1,r6,r1,asr#15;
                 subgt.w r9,r0,r1                                      (10 B)
```

Original post-clamp block (the `mov r3,r5` / `mov r9,r3` pair is the `x` live
interval being split at the join so `r5` can be reused for `top - pad` and
`r9` (pad's register) can take over once pad dies):

```text
1712f4: vmov r2, s16        ; top
1712f8: mov  r3, r5         ; x -> r3      (split copy)
1712fa: add  sl, r6         ; pad2 + width
1712fc: mov  r1, r9         ; pad -> r1
1712fe: subs r6, r3, r1     ; x - pad
171300: str.w r8, [sp,#12]  ; agent spill
171304: sub.w r5, r2, r9    ; top - pad   (pad dies)
171308: mov  r9, r3         ; x -> r9      (split copy)
```

Ours instead copies pad out of `fp` once (`mov r5, fp` at 1712ee, before the
`String("")` ctor) and has no split copy.

## 2. Root cause, re-confirmed this wave

Instrumented LLVM 7.0.2 (same revision as NDK r18b,
`/tmp/llvm_android_702b/build2/bin/clang++`, `RA_TRACE=1`) on HEAD:

```text
ENQ  %2   pad      size=6384 prio=0xA00018F0 (no hint)
ENQ  %608 x        size=4220 prio=0xE000107C (hint $r3)
select %608 x      -> $r5
select %47  boxW   -> $r6
select %46  top-pad weight 6.25e-4 -> evicts x from $r5, takes $r5
select %608 x      -> $r9   (r9 free)
select %45  x-pad  -> $r6
select %2   pad    -> no free callee-saved reg -> spill (fp)
```

`x`'s hint `$r3` comes from `$r3 = COPY %608` for the `DrawString` 4th
argument (`COPYHINTDBG reg=2147484256 hint=69 mi=$r3 = COPY %608`), so `x`
has a physical hint and bit 30 of its priority; `pad` is a plain
`t2LDRi12 [layout,#148]` with **no COPY anywhere**, so it is unhinted and is
selected after every hinted value.

Two (and only two) ways to reach the original's allocation were identified:

1. **Give `pad` a physical hint** so it is selected before `%46`/`%608`.
   Diagnostic: replacing `SetColor(0xffffffffu)` with
   `SetColor((unsigned) pad)` makes the very first instruction
   `ldr.w r9, [r6, #148]` (pad -> r9). But the rest of the allocation shifts
   (x -> r10, width -> r4, lineHeight -> r8) and the function grows to 1968 B
   (78.6 %). There is no code-neutral call in the source that takes `pad` as
   an argument (drawBox/DrawRectangle/DrawString all take `x - pad`,
   `top - pad`, `pad*2 + width`, never `pad`).
2. **Split `x` at the join** so its post-join piece does not occupy `r9`
   before `pad` is selected. This needs the PHI copy
   `%43 = COPY %608` (bb.43) not to be coalesced with `%608`. In our MIR the
   coalescer always joins them (`mov r3,r5`/`mov r9,r3` disappear); every
   source-level copy of `x` tried (`textX`, `x2`, nested scope, `const`,
   `x+0`) is either sunk past the last use of `x` or coalesced away.

Knob probes this wave (scratch compiler only, not source-reachable):

| knob | result |
|---|---|
| `FORCEVREG=pad:r9` (no evict) | no effect (r9 held by x) |
| `FORCEEVICT=1 FORCEVREG=pad:r9` | pad r9, x r5, but lineHeight -> r8 and top spilled: 81.2 %, 1952 B |
| `FORCEEVICT=1 FORCEVREG=x:r5,pad:r9` | 80.9 %, 1952 B |
| `... + agent:r8,lineHeight:r11` | 90.2 %, 1944 B |
| `NOHINT=x` | pad -> r10 (not r9), 69.1 %, 1964 B |
| `VRW=x*1.5` | 95.7 % (identical code) |
| `VRW=x*2.5..16` | x stays r5 but pad2 -> r6, width -> r9: 81.4 %, 1968 B |
| `PRIOVREG=pad` + `HINTVREG=pad:r9` | pad -> r6, everything reshuffles: 76.7 %, 1980 B |

So `pad -> r9` is necessary but not sufficient: the whole 7-value packing
(`r4 layout, r5 x, r6 width, r8 agent, r9 pad, r10 pad2, r11 lineHeight`)
must fall out at once, and the greedy allocator only does that for one
selection order.

## 3. Source shapes measured this wave (all <= 95.676047)

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6` (~1.1 s incremental), then the
official `verify.py` internals (`/dev/shm/w6117/score.py`, `autojunk=False`).
The 7:1 `-likely-branch-weight` override was active throughout.

### 3.1 Clamp / declaration shapes

```text
variant                                             raw        size  insns
HEAD (short pad3, `-(width + pad3*2)`)              95.676047  1948  728/729
faithful `-(width + pad * 2)`                       93.964335  1948  729/729
faithful `-width - pad*2`                           93.964335  1948  729/729
`int pad2 = pad + pad` / `pad << 1` in clamp        93.964335  1948  729/729
`int pad3 = pad` (int, not short)                   93.964335  1948  729/729
`x = (w-inset) - width - pad - pad`                 92.518874  1944  728/729
`int left = x - pad` used in both conditions        93.9       1948  728/729
conditions written `inset > x-pad` / `w-inset < ...` 95.3      1948  728/729
right clamp checked first (cond_swap)               90.9       1952  730/729
`pad3` used for `x - pad` / `pad + x` too           90.9       1952  730/729
`short pad3` after `int x` / before `layout`        95.7       1948  same
`int x` before `Layout *layout`                     95.7       1948  same
`inset` / `right` locals                            95.7       1948  same
`Layout *const layout`, `const short pad3`          95.7       1948  same
```

### 3.2 `x`-copy / PHI-split attempts (all coalesce or sink)

```text
variant                                             raw        size
`int textX = x` after clamp, used in DrawStrings    95.7       1948
`int x2 = x` after clamp, first DrawString uses x2  95.7       1948
`int x2 = x`, both DrawStrings use x2               95.7       1948
`int x2 = x + 0`                                    95.7       1948
nested scope `{ int cx = ...; x = cx; }`            95.7       1948
`int cx = centerX; ...; int x = cx;` / `const int x` 95.7      1948
`x` declared uninit + 3-way `else x = centerX`      95.7       1948
conditions use `centerX` instead of `x`             95.7       1948
`int x = centerX` moved before `layout`/`pad3`      95.7       1948
```

### 3.3 Box locals / layout caching

```text
variant                                             raw        size
`boxX`/`boxY`/`boxW` locals after clamp             95.7       1948
`boxX`/`boxY` used in drawBox+DrawRectangle         95.7       1948
`boxY = top - pad` after clamp (unused)             95.7       1948
`boxW = pad*2 + width` local                        71.1       1964
single `Layout *layout` for pad + clamp + drawBox   68.2       1972
`layout` cached before `pad`                        68.2       1972
```

### 3.4 Brute-force cross product

180 combinations of
{`short pad3`, `(short)pad`, none, `int pad2=pad+pad`, `int pad2=pad<<1`,
`int pad3=pad`, `signed char pad3`} x
{faithful, faithful-flat, short, pad2} x
{no extra local, `boxY`, `boxX`} x
{`pad*2+width`, `width+pad*2`, `pad+pad+width`} x
{`lineHeight` before/after drawBox}: **best 95.7**, 164 valid builds.

## 4. Conclusion

The 95.7 -> 100 gap is not a wrong constant, field offset, callee, block
order, literal pool, or clamp source shape. It is the greedy allocator's
selection order: `pad` is unhinted and selected last, `x` is hinted and
evicted from `r5` by `top - pad`, so `x` takes `r9` and `pad` spills to `fp`.
The original keeps `x` in `r5` and splits it at the join, which requires the
PHI copy `%43 = COPY %608` to survive coalescing (or `pad` to acquire a
physical copy hint). Neither is reachable from any of the ~230 C++ shapes
measured here or in waves 106-112.

## 5. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w6117/score.py
# RA trace (instrumented LLVM 7, same revision as NDK r18b)
RA_TRACE=1 /tmp/llvm_android_702b/build2/bin/clang++ <TU flags> -S -o /dev/null \
  src/game/menu/SpaceLounge.cpp 2> ra.txt
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
| `.rodata` | 0x41c27 (orig 0x41b67) | 0x41c27 (unchanged) |

No `src/` change: no ELF section, `.ARM.extab`, `.rodata`, `.dynsym` or
file-size delta was introduced.
