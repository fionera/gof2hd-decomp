# SpaceLounge::drawLounge — wave-119 x-split / eviction-cost study (w6927)

Status: research note (2026-11-06). Worktree `wt/w6927`, branch
`wave119-w6927`, base `dfe78cca9`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.67604667124228 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4505 / linked 4505, data 843/843, imports 207,
  lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave119-w6744.md` (eviction order),
`-wave118-w6616.md` (scratch knobs, exact clamp at 93.617 %),
`-wave117-w6517.md` (292-variant sweep), `-wave119-w6714.md` (literal pool
already correct). The wave-119 w6906 note (clamp/`boxW` sweep) is not present
in this worktree; its conclusions are re-derived below.

## 1. What actually differs (exact opcode diff)

`verify.py` reports **176 positional mismatches**; the aligned diff has only
~31 unmatched instructions per side. Everything is in the clamp and the join,
plus the register names that follow from it:

```
orig[142] ldr.w r9,[r6,#148]        ours ldr.w fp,[r6,#148]      ; pad
orig[147] mov.w sl,r9,lsl #1        ours mov.w sl,fp,lsl #1      ; pad2
orig[518] vmov  r5,s18              ours vmov  r9,s18            ; x = centerX
orig[522] sub.w r1,r5,r9            ours sub.w r1,r9,fp          ; x - pad
orig[525] add.w r5,r0,r9            ours add.w r9,r0,fp          ; x = inset+pad
orig[532] add.w r1,r9,r5            ours add.w r1,fp,r9          ; pad + x
orig[535] itt gt                    ours ittt gt                 ; 8 B vs 12 B arm
          addgt.w r1,r6,sl                   movgt.w r1,fp,lsl #16
          subgt   r5,r0,r1                   addgt.w r1,r6,r1,asr #15
                                             subgt.w r9,r0,r1
orig[545] mov r3,r5                 ours mov r1,r5               ; x copy
orig[547] mov r1,r9                 ours (merged)
          subs r6,r3,r1                       sub.w r6,r9,r1
          str.w r8,[sp,#12]                   (earlier)
          sub.w r5,r2,r9                      subs r5,r2,r5
          mov r9,r3                           (absent)
orig[566] mov fp,r8                 ours (absent)                ; top -> fp
          ldr.w r8,[sp,#12]                   (absent)             ; agent -> r8
orig[586] mov r0,r8                 ours ldr r5,[sp,#12]; mov r0,r5
orig[608] add r0,fp                 ours add r0,r8               ; top+0x2c0
orig[647] add r1,fp                 ours add r1,r8               ; top+0x2c0
```

So the whole residual is one fact: the original has **pad -> r9, x -> r5**
during the clamp; HEAD has **pad -> fp (r11), x -> r9**. Everything else
(including the `top -> fp` / `agent -> r8` moves after the drawBox call, and
the two `add ...,fp` uses of `top`) is a consequence.

## 2. New finding: the original's `x` is split into THREE live ranges

The original's join is not a simple two-register join:

```
1712f4  vmov r2, s16        ; top
1712f8  mov  r3, r5         ; x -> r3      (copy #1)
1712fa  add  sl, r6         ; boxW
1712fc  mov  r1, r9         ; pad
1712fe  subs r6, r3, r1     ; x - pad
171300  str.w r8, [sp,#12]  ; spill agent
171304  sub.w r5, r2, r9    ; top - pad   (r5 reused)
171308  mov  r9, r3         ; x -> r9      (copy #2)
17130a  mov  r8, r2         ; top
17130c  strd sl, fp, [sp]   ; boxW, lineHeight
```

`mov r3,r5` is the MIR `%43:rgpr = COPY %608:rgpr` at slot 7280 (bb.43): in
the original it is **not coalesced**. `mov r9,r3` is a split copy of `%43`
placed before the `drawBox` call (r3 is call-clobbered and is reused as the
`$r3 = top-pad` argument at 171318). So the original's `x` lives in
`r5` (clamp) -> `r3` (join) -> `r9` (both `DrawString` calls).

In HEAD (and in every source shape measured) `%43` coalesces with `%608`, so
`x` is one interval `[6784,11072]` and lands wholly in `r9`.

## 3. New finding: eviction cost is (BrokenHints, MaxWeight), not priority

`EvictionCost::operator<` compares `(BrokenHints, MaxWeight)`, and
`tryEvict` picks the cheapest register. With `pad` already in `r9` and `x` in
`r5`, `%46` (`top - pad`, weight 6.25e-4) chooses between

| reg | occupant | weight | evictable |
|---|---|---|---|
| r5 | `%608` x | 2.946e-4 | yes |
| r8 | `%1` agent | 2.267e-4 | yes |
| r9 | `%2` pad | 2.078e-4 | yes |

and evicts **pad** (cheapest). Trace (`-w6927-padprio`):

```
selectOrSplit GPRnopc:%46 ...
should evict: %607 (x, r5) weight 2.946140e-04
should evict: %1   (agent, r8) weight 2.267318e-04
should evict: %2   (pad, r9)   weight 2.077779e-04
evicting $r9 interference: Cascade 10
```

So even if `pad` reaches `r9` before `%46` pops, `%46` immediately takes it
back. The original must therefore have `%46` pop **after** pad is in `r9` but
with `pad` *not* the cheapest evictee — or `%46` must not need `r9` at all.
Neither is reachable by priority tweaks: `%46`'s hint is `$r3 $r2`, both
`CANNOTEVICT`, and r4/r6/r10/r11 are occupied by non-evictable interference.

## 4. The priority-queue window is empty

For the original allocation the queue must do

1. `x -> r5` (hinted, size 4220),
2. `pad -> r9` (r9 still free),
3. `%45`/`%46` evict `x` from `r5`; `x` re-selects and splits.

Step 2 before step 3 needs `pad` to pop between `x` and `%45`/`%46`. With the
stock `getPriority` (`(1<<31)|(1<<29)|size`, plus `1<<30` for any interval
with a copy hint to a physical register — `CalcSpillWeights` adds copy hints
to `RegAllocHints`, and `VirtRegMap::hasKnownPreference` accepts physical
hints), the order is

| value | vreg | size | priority | hint |
|---|---|---:|---:|---|
| agent | `%1` | 8784 | `0xE0000000+8784` | `$r0` |
| x | `%608` | 4220 | `0xE0000000+4220` | `$r3` |
| width | `%607` | 1968 | `0xE0000000+1968` | `$r0` |
| `pad2+width` | `%47` | 768 | `0xE0000000+768` | `$r3` |
| `top-pad` | `%46` | 736 | `0xE0000000+736` | `$r3 $r2` |
| `x-pad` | `%45` | 704 | `0xE0000000+704` | `$r2 $r1` |
| pad | `%2` | 6384 | `0xA0000000+6384` | none |
| pad2 | `%381` | 6160 | `0xA0000000+6160` | none |

Every hinted interval outranks every unhinted one, so `pad` (unhinted) can
never pop between `x` and `%45`/`%46`. Hinting `pad` makes it pop *before*
`x` (size 6384 > 4220) and it takes `r5` (free) or `r8`, never `r9`.

## 5. Measurements this wave (real NDK r18b build)

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then the official `verify.py`
internals (`/dev/shm/w6927/score.py`). `pad`/`x` are the registers of
`ldr.w X,[r6,#148]` and `vmov X,s18`.

All values are `<= 95.67604667124228`; `mismatched-pos` is the positional
count (the score is the SequenceMatcher ratio).

| variant | raw % | size | insns | mism |
|---|---:|---:|---|---:|
| **HEAD (`short pad3`)** | **95.67604667124228** | **1948** | 729/728 | 176 |
| `const int pad` | 95.67604667124228 | 1948 | 729/728 | 176 |
| `long pad` | 95.67604667124228 | 1948 | 729/728 | 176 |
| `unsigned pad` | 95.40150995195607 | 1948 | 729/728 | 178 |
| `top = ... (pad*2)` / `(pad<<1)` | 95.67604667124228 | 1948 | 729/728 | 176 |
| `top = ... (float)pad - (float)pad` | 81.75582990397805 | 1968 | 729/729 | 501 |
| `pad3 << 1` in clamp | 95.67604667124228 | 1948 | 729/728 | 176 |
| `width - pad3 * -2` | 95.67604667124228 | 1948 | 729/728 | 176 |
| `width + pad3 + pad` | 93.82716049382715 | 1948 | 729/729 | 171 |
| `width + pad + pad3` | 93.82716049382715 | 1948 | 729/729 | 171 |
| `width + pad3 + pad3` | 92.39204934886909 | 1952 | 729/730 | 194 |
| clamp as nested ternary | 95.67604667124228 | 1948 | 729/728 | 176 |
| clamp as explicit 3-way `else` | 95.67604667124228 | 1948 | 729/728 | 176 |
| `int drawX = x;` (4 placements) | 95.67604667124228 | 1948 | 729/728 | 176 |
| `int x2 = x;` / `int x3 = x2;` | 95.67604667124228 | 1948 | 729/728 | 176 |
| `lineHeight` before clamp | 91.64383561643835 | 1952 | 729/731 | 227 |
| `int boxW` after `lineHeight` | 71.08680792891319 | 1964 | 729/734 | 607 |
| `int boxW = width + pad*2` (both uses) | 72.60273972602739 | 1968 | 729/731 | 594 |
| `boxW` in `drawBox` only | 72.60273972602739 | 1968 | 729/731 | 594 |
| `boxW` in `DrawRectangle` only | 78.16563997262149 | 1976 | 729/732 | 590 |
| `boxW` from `pad3*2` | 81.58795345653662 | 1976 | 729/732 | 595 |
| `int boxX/boxY` locals (both uses) | 93.96433470507544 | 1948 | 729/729 | 178 |
| `int boxY` only | 92.30769230769231 | 1944 | 729/727 | 240 |
| **faithful `width + pad*2`** | 93.96433470507544 | 1948 | 729/729 | 178 |
| `int pad2 = pad*2` + faithful | 93.96433470507544 | 1948 | 729/729 | 178 |

Key negative results:

* The faithful clamp (`width + pad*2`, no `pad3`) reproduces the original's
  **8-byte** right arm exactly, but scores **93.964 %** — the join is then 4 B
  short (`mov r3,r5` / `mov r9,r3` missing), and the shorter join costs more
  than the wrong arm. The committed `short pad3` hack is a 1.7-point
  workaround: it emits the 12-byte arm and a `mov r5,fp` that together keep
  the instruction count/alignment closer to the original.
* Every `boxW` local (the only family that reaches `pad -> r9`, `x -> r5`)
  changes the whole-function allocation: `sub sp,#128`, pad/pad2 split in the
  second RA round, `top` spilled. 1968 B, 72.6 %.
* `lineHeight` must stay *after* the clamp; moving it before costs 4 B and
  4 points.

## 6. Scratch-compiler runs (patched LLVM 7.0.2, HEAD source)

Knobs live in `/tmp/llvm_android2` (`RegAllocGreedy.cpp`), not in the build.
They are diagnostic only.

| flags | raw % | size | pad | x | lineHeight |
|---|---:|---:|---|---|---|
| (none) | 95.67604667124228 | 1948 | fp | r9 | fp |
| `-w6616-hintpad` | 78.63013698630137 | 1968 | r8 | sl | r8 |
| `-w6616-boostx` | 80.79289131920710 | 1972 | fp | r5 | fp |
| `-w6616-boostx -w6616-hintpad` | 92.04389574759945 | 1948 | fp | r6 | r8 |
| `-w6616-boostx -w6616-nohint46` | 81.53740562800274 | 1960 | fp | r5 | sl |
| `-w6616-boostx -w6616-nohint46 -w6744-nohint45` | 81.34430727023320 | 1964 | sl | r5 | sl |
| `-w6616-forcepad9` | 83.32189430336308 | 1960 | r5 | sl | r9 |
| `-w6616-nohint46` | 81.26286890871654 | 1964 | fp | r9 | sl |
| `-w6744-nohint45` | 80.05483207676491 | 1968 | r5 | r8 | sl |
| `-w6616-nohintx` | 66.94101508916323 | 1964 | r4 | sl | fp |
| `-w6927-padprio` (pad prio `0xE0000000+1000`) | 68.35616438356165 | 1956 | r9 | r2 | sl |
| `-w6616-boostx -w6616-forcepad9 -w6616-nohint46` | 83.65384615384616 | 1948 | r9 | r5 | r8 |
| same on the faithful source | 93.61702127659575 | 1944 | r9 | r5 | fp |

`-w6927-padprio` is the only knob that reaches `pad -> r9` while `x` keeps
`r5` for the clamp, but `%46` then evicts pad from `r9` (section 3) and the
whole allocation rotates (`x -> r2`, pad2 spilled to `[sp,#16]`).

## 7. Conclusion

The clamp and join are fully explained. The residual is a single greedy-RA
constraint with two coupled parts:

1. `pad` must occupy `r9` before `%45`/`%46` evict `x` from `r5` — but `pad`
   is unhinted and therefore pops after every hinted interval, including
   `%45`/`%46`, and no source shape gives `pad` a copy hint.
2. `%43 = COPY %608` (the join `x`) must **not coalesce** in the original
   (it becomes `mov r3,r5`), while every source shape measured coalesces it.

Both are properties of the MIR that the source cannot express with stock
clang-7: `pad` has no COPY in the IR (only `t2SUBrr`/`t2ADDrr`/`t2LSLri`), so
it can never acquire a hint, and `%43`/`%608` are trivially coalescible.
The committed HEAD (`short pad3`, **95.67604667124228 %**, 1948 B) remains
the best measured shape out of ~150 variants this wave (~1700 across
waves 110-119).

## 8. Structural state (unchanged, re-verified)

`check` = RATCHET PASS: byte 4505 / linked 4505, data 843/843, imports 207,
lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact. No
section/extab change from this pass.
