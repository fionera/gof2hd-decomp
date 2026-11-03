# SpaceLounge::drawLounge — greedy-RA register swap (w4118, wave 91)

Status: research note (2026-11-03). Worktree `wt/w4118`, branch `wave91-w4118`,
base `a4557300`. **No `src/` change landed**: every source shape tested keeps the
official score at 95.6 (byte_exact unchanged at 4498). The tree is clean.

Baseline (`cmake --build cmake-build-match --target check -j8`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.6`, `osize = usize = 1948`,
`byte = linked = false`. Object-level oracle (`_work/reconstructions/w3692_oracle.py`
copy in `/dev/shm/w4118/oracle.py`) reports `95.5`.

## 1. The remaining diff is one region

The normalised mnemonic streams are 704 instructions (original) and 704
(ours) with the same 1948-byte size. `difflib` on the normalised streams (with
`autojunk=False`) gives 81 differing lines, all in one contiguous region: the
clamp right arm, the join, and the downstream `agent`/`top` colouring. The
pre-existing side-by-side dump is `/dev/shm/w4118/base.diff`; the region is
instruction indices 535..589:

```text
index  original (0x1712dc..)                     ours (0x5a4..)
535    add.w    r1, r9, r5      ; pad + x         add.w    r1, fp, r9
536    add      r1, r6          ; + width          add      r1, r6
537    cmp      r1, r0                            cmp      r1, r0
538    itt      gt                                itttt    gt
539    addgt.w  r1, r6, sl      ; width+pad*2     movgt    r1, #0
540    subgt    r5, r0, r1      ; x = (w-inset)-…  subgt.w  r1, r1, fp, lsl #1
541    ldr      r1, [pc,#476]   ; String("")       subgt    r1, r1, r6
542    ldr.w    fp, [r4,#48]    ; lineHeight      addgt.w  r9, r1, r0
543    add      r1, pc                             str.w    r8, [sp,#12]
544    add      r0, sp, #56                        mov      r5, fp
545    movs     r2, #0                             ldr      r1, [pc,#464]
546    blx      String::String                      ldr.w    fp, [r4,#48]
547    vmov     r2, s16         ; top              add      r1, pc
548    mov      r3, r5          ; copy x           add      r0, sp, #56
549    add      sl, r6          ; pad*2+width      movs     r2, #0
550    mov      r1, r9          ; pad              blx      String::String
551    subs     r6, r3, r1      ; x - pad          vmov     r2, s16
552    str.w    r8, [sp,#12]    ; spill agent      mov      r1, r5
553    sub.w    r5, r2, r9      ; top - pad        add      sl, r6
554    mov      r9, r3          ; x -> r9          sub.w    r6, r9, r1
555    mov      r8, r2          ; top -> r8        subs     r5, r2, r5
556    strd     sl, fp, [sp]    ; args 4,5         mov      r8, r2
557    movs     r1, #2                             strd     sl, fp, [sp]
558    str      r0, [sp, #8]                       movs     r1, #2
559    mov      r0, r4                             str      r0, [sp, #8]
560    mov      r2, r6                             mov      r0, r4
561    mov      r3, r5                             mov      r2, r6
562    blx      drawBox                            mov      r3, r5
563    add      r0, sp, #56                        blx      drawBox
564    blx      String::~String                    add      r0, sp, #56
565    ldr      r0, [sp,#24]                       blx      String::~String
566    ldr      r1, [pc,#380]                      ldr      r0, [sp,#24]
567    ldr      r0, [r0,#0]                        ldr      r1, [pc,#376]
568    blx      SetColor                           ldr      r0, [r0,#0]
569    mov      fp, r8          ; top -> fp         blx      SetColor
570    ldr.w    r8, [sp,#12]    ; agent -> r8       ldr      r0, [sp,#20]
571    …                                           ldr      r1, [r0,#0]
572    …                                           ldr      r0, [sp,#24]
573    …                                           ldr      r1, [r1,#48]
574    …                                           ldr      r0, [r0,#0]
575    …                                           str      r1, [sp,#0]
576    …                                           mov      r1, r6
577    …                                           mov      r2, r5
578    …                                           mov      r3, sl
579    …                                           blx      DrawRectangle
580    …                                           ldr      r0, [sp,#24]
581    …                                           ldr      r0, [r0,#0]
582    …                                           mov.w    r1, #4294967295
583    …                                           blx      SetColor
584    …                                           ldr      r0, [sp,#16]
585    …                                           ldr      r6, [r0,#0]
586    …                                           ldr      r0, [sp,#24]
587    …                                           ldr      r4, [r0,#0]
588    …                                           ldr      r5, [sp,#12]
589    mov      r0, r8                             mov      r0, r5
```

The original spends two instructions on the clamp arm (`addgt.w`/`subgt`) and
two on the `x` split (`mov r3,r5` / `mov r9,r3`); ours spends four on the arm
and one on the pad copy (`mov r5,fp`). Both streams are exactly 1948 bytes.

## 2. Register maps

```text
value        original   ours
layout       r4         r4
x            r5 -> r9   r9
width        r6         r6
agent        r8         r8 (then r5 after drawBox)
top          r8 -> fp   r8
pad          r9         r11 (fp)
pad*2 (sl)   r10        r10
lineHeight   r11 (fp)   r11 (fp)
```

Only `x` and `pad` differ. Everything else, including all downstream uses, is
already in the original's registers. The `mov r5, fp` pad copy is a
*consequence* of pad living in `fp` while `lineHeight` also needs `fp`.

The original loads pad into `r9` at `0x170ebe` (`ldr.w r9,[r6,#148]`) and keeps
it there until `0x171304` (`mov r9,r3`, pad dies). Ours loads it into `fp` at the
same address (`ldr.w fp,[r6,#148]`), computes `sl=fp*2`, and copies pad to `r5`
just before the join.

## 3. Greedy-RA mechanism (instrumented LLVM 7)

`/tmp/llvm_ndk_build/bin/llc` is LLVM 7.0.0svn with `RA_DBG_PRIO`,
`RA_NOHINT`, `RA_FORCE_HINT`, `RA_NOXHINT`. It reproduces the official build's
allocation exactly on the extracted function IR (`/dev/shm/w4118/dl.ll`,
produced with the official compile command + `-S -emit-llvm` and
`llvm-extract -func=_ZN11SpaceLounge10drawLoungeEv`). Physical register numbers:
R0=66, R1=67, R2=68, R3=69, R4=70, R5=71, R6=72, R8=74, R9=75, R10=76, R11=77.

`RegAllocGreedy::enqueue` (LLVM 7.0.1 `lib/CodeGen/RegAllocGreedy.cpp:680`):
`Prio = (1u<<29) + Size`, then `|= 1u<<31` for global/local ranges, then
`|= 1u<<30` iff `VRM->hasKnownPreference(Reg)` (i.e. the interval has a copy
hint that is a physical register or an already-assigned vreg). The queue is a
max-heap; ties break on `~vReg` (lower vreg first).

Relevant intervals in the *faithful* IR (base is identical except the clamp
arm shape; sizes differ by a few slot indices):

```text
ENQ %2   (pad)            size=6304  prio=0xA0000000+6304  hint=0,0    haspref=0
ENQ %45  (x-pad)          size=704   prio=0xE0000000+704   hint=0,r2   haspref=1
ENQ %46  (top-pad)        size=736   prio=0xE0000000+736   hint=0,r3   haspref=1
ENQ %47  (width+pad*2)    size=768   prio=0xE0000000+768   hint=0,r3   haspref=1
ENQ %124 (x)              size=5856  prio=0xE0000000+5856  hint=0,r1   haspref=1
ENQ %1   (agent)          size=8656  prio=0xE0000000+8656  hint=0,r0   haspref=1
```

The eviction chain that produces our (wrong) allocation:

```text
x (%124)  -> r5   (first free cheap register)
%47       -> r6
%46       -> tries r5 (interferes with x), r9 (free but "expensive")
             shouldEvict(%46, IsHint=false, x, …) is weight-based:
             %46.weight (6.25e-4) > x.weight (2.96e-4)  -> evict x from r5
             assign %46 -> r5
x re-enqueued -> r9 (r5 now %46, r9 free)
%45       -> r6 (evicts %47)
pad (%2)  -> r11 (tryEvict; r9 is taken by x)
lineHeight-> r11 (pad copied to r5 first)
```

The target needs pad allocated to `r9` **before** `%46` evicts x, so that when x
is re-enqueued there is no free CSR and it is split (`r3` for the local use,
`r9` after pad dies). But pad has **no hint** (`hint=0,0`, `haspref=0`), so its
priority `0xA0000000+6304` is below every hinted interval (`0xE…`), and `%46`
always dequeues first.

`shouldEvict` (LLVM 7.0.1 `RegAllocGreedy.cpp:843`):

```cpp
bool shouldEvict(LiveInterval &A, bool IsHint, LiveInterval &B, bool BreaksHint) {
  bool CanSplit = getStage(B) < RS_Spill;
  if (CanSplit && IsHint && !BreaksHint) return true;   // hint always wins
  if (A.weight > B.weight) return true;
  return false;
}
```

`%46`'s hint comes from the `$r3 = COPY %46` that feeds the 4th `drawBox`
argument (r3). `%45`'s hint is r2 (3rd argument). `%47`'s is r3. These are
physical copy hints inserted by `CalcSpillWeights::calculateSpillWeightsAndHints`
(`copyHint` returns the physical operand of the COPY). When a copy hint is a
physical register, `hasKnownPreference` is true immediately, so the `1u<<30`
priority bit is always set. Pad is a plain `t2LDRi12` with no COPY, so it can
never get the bit from the current IR.

## 4. Variants measured this session

All measurements are the object-level oracle (`/dev/shm/w4118/score_variant.py`,
official compile command, normalised instruction diff). The base scores 95.5
(official 95.6). The tree was left unchanged.

### 4.1 Clamp right-arm forms (current tail)

| variant | pct | size |
|---|---:|---:|
| `+(-width - pad - pad)` (current, flat) | 95.5 | 1948 |
| `-(width + pad * 2)` (faithful) | 93.8 | 1948 |
| `-(pad * 2 + width)` | 93.8 | 1948 |
| `-(width + pad + pad)` | 95.3 | 1948 |
| `-width - (pad + pad)` | 93.8 | 1948 |
| `-width - pad * 2` | 93.8 | 1948 |
| `-(pad + pad + width)` | 93.8 | 1948 |
| `-width - 2 * pad` | 93.8 | 1948 |

The **faithful** form produces the original's two-instruction arm exactly
(`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1` in the flat-register assignment),
but because pad is in `fp` and lineHeight also needs `fp`, the compiler emits
`mov r5,fp` in *both* clamp arms (two copies instead of the original's zero),
so the score drops to 93.8.

### 4.2 Named temporaries / reuse

| variant | pct | size |
|---|---:|---:|
| `int xp=x-pad; int tp=top-pad;` before drawBox, used in both calls | 95.5 | 1948 |
| `int xp,tp,bw` before drawBox, used in both calls | 95.5 | 1948 |
| `int bw=pad*2+width;` before drawBox (flat) | 70.0 | 1968 |
| `int bw=pad*2+width;` before drawBox (faithful) | 71.6 | 1968 |
| `int topPad=top-pad;` before drawBox | 94.6 | 1948 |
| `int pad2=pad;` used in clamp / top / calls | 95.5 | 1948 (coalesced) |

### 4.3 Direct `Globals::layout->field_0x94` accesses

| variant | pct | size | notes |
|---|---:|---:|---|
| all three pad uses in `DrawRectangle` direct (faithful clamp) | 89.3 | 1944 | **x=r5, pad=r9**, no pad copy, but reloads field_0x94 after drawBox |
| same, flat clamp | 89.3 | 1944 | |
| direct in `DrawRectangle` only (flat) | 80.7 | 1952 | |
| direct in `drawBox` only | 68.9 | 1972 | |
| direct in `top` computation | 79.8 | 1972 | |
| direct in clamp | 91.0 | 1956 | |

The `faith_rect_all` variant is the only source shape found that produces the
target `x=r5, pad=r9` colouring *and* the faithful two-instruction arm. Its
instruction stream is wrong because the direct access forces a reload of
`field_0x94` after `drawBox`, which shortens pad's live range; the compiler then
materialises `x-pad`/`top-pad`/`boxW` directly in `r2`/`r3`/`r1` for `drawBox`
and recomputes them for `DrawRectangle` instead of keeping them in `r6`/`r5`/`sl`
as the original does. Score 89.3 / 1944 B.

### 4.4 Declaration order / statement order

| variant | pct | size |
|---|---:|---:|
| pad after SetColor | 92.8 | 1948 |
| pad after centerX | 92.8 | 1948 |
| pad before centerX | 92.8 | 1948 |
| pad before top | 92.8 | 1948 |
| pad before SetColor | 95.5 | 1948 |
| pad before agent | 95.5 | 1948 |
| `layout` declared right after pad | 66.9 | 1972 |
| `Globals::layout` used in clamp | 95.5 | 1948 |
| `lineHeight` before clamp | 95.0 | 1948 |
| `lineHeight` before clamp + faithful | 91.6 | 1952 |
| `lineHeight` passed as `layout->field_0x30_rowHeight` | 93.8 | 1948 |
| `top` as `(float)(pad * 2)` | 95.5 | 1948 |
| clamp on `centerX` directly (conditions use centerX) | 80.6 | 1964 |
| clamp as a ternary chain | 95.5 | 1948 |
| `name`/`role` as if/else instead of ternary | 79.3 / 78.1 / 74.9 | 1992 / 2028 / 2068 |

### 4.5 Tail expression permutations

135 combinations of
`boxW ∈ {pad*2+width, width+pad*2, 2*pad+width, pad+pad+width, (pad<<1)+width}`,
`xPad ∈ {x-pad, -pad+x, x+(-pad)}`,
`topPad ∈ {top-pad, -pad+top, top+(-pad)}`,
`height ∈ {lineHeight, layout->field_0x30_rowHeight, Globals::layout->...}`
all score 95.5 / 1948 (the compiler canonicalises every form to the same IR).

### 4.6 Branch-weight flags

Replacing `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12`
with every combination of `likely ∈ {8,12,16,20,24,32,64}` and
`unlikely ∈ {4,8,12,16}` never reaches 100 %; the best is 95.5 (the current
20/12 setting), several combinations drop to 88.1–95.3.

## 5. Instrumented-RA sweep (hint manipulation)

`RA_NOHINT` clears the `1u<<30` priority bit for a given vreg. Sweeping all 16
subsets of `{x, x-pad, top-pad, width+pad*2}` (vregs 575/45/46/47 in the
extracted IR) gives (pad and x read from the `@DEBUG_VALUE` comments of the
generated asm):

| cleared | x | pad | top | lineHeight |
|---|---|---|---|---|
| — (base) | r9 | r11 | r8 | r11 |
| x | r11 | r10 | r8 | r10 |
| x-pad (45) | r9 | r10 | r8 | r10 |
| top-pad (46) | r9 | r11 | r8 | r10 |
| width+pad*2 (47) | r9 | r10 | r8 | r10 |
| x-pad+top-pad (45,46) | r9 | r10 | r8 | r10 |
| x-pad+width (45,47) | r9 | r10 | r8 | r10 |
| top-pad+width (46,47) | r9 | r11 | r8 | r10 |
| all four | r11 | r10 | r8 | r10 |

No subset produces `x=r5, pad=r9`. `RA_FORCE_HINT="2:<n>"` (the only form that
affects pad) boosts pad's priority bit but the allocator then gives pad `r8`
(the first free cheap register at that point), not `r9`; x becomes `r10`.

## 6. Proof the target colouring is reachable

`llc -stress-regalloc=12` on the same IR produces the original's clamp/split
structure:

```text
vmov    r5, s0            ; x = r5
...
sub.w   r1, r5, r9        ; x - pad, pad = r9
...
add.w   r5, r0, r9        ; x = inset + pad
...
addgt.w r1, r6, r10       ; width + pad*2 (pad*2 = r10)
subgt   r5, r0, r1        ; x = (w-inset) - (width+pad*2)
...
mov     r1, r9            ; pad
mov     r3, r5            ; copy x
vmov    r2, s0            ; top
subs    r6, r3, r1        ; x - pad
sub.w   r5, r2, r9        ; top - pad
mov     r9, r3            ; x -> r9
strd    r10, r8, [sp]
```

Every register matches the original (`x=r5`, `pad=r9`, `top-pad=r5`,
`x-pad=r6`, `pad*2=r10`, split `r3`→`r9`) except `lineHeight`, which takes `r8`
because `top` was forced onto the stack. The assembled function is 1856 B /
54.4 % (the spills cost too much), but it proves the target colouring is a
natural greedy-RA outcome one notch of register pressure away.

## 7. Conclusion / remaining leads

The remaining 95.6 → 100 gap is purely greedy-RA colouring. The original's
allocation requires:

1. `x` allocated to `r5` first;
2. `pad` allocated to `r9` *before* `%46` (top-pad) evicts x;
3. `%46` then evicts x from `r5`; with `r9`/`r10` taken, x is **split** (`r3`
   for the local `x-pad`, `r9` after pad dies).

Step 2 is the blocker: pad is a plain load with no copy hint, so its priority
`0xA0000000+6304` is below `%46`'s `0xE0000000+736`. The only ways to move pad
ahead of `%46` are:

* give pad a copy hint (`0xE…`), which makes it dequeue *before* x (size 6304 >
  5856) and take `r5`/`r8`, not `r9`; or
* remove `%46`'s hint, which lets `%45` (x-pad) evict x instead; or
* shorten pad's live range so it is allocated while `r9` is still free — this is
  what the `field_0x94` reload does, but it also changes the reuse structure of
  `x-pad`/`top-pad`/`boxW` and costs the reload.

The faithful clamp form `x = Globals::w - layout->buttonInsetX - (width + pad * 2)`
is the correct source shape (it produces the original's two-instruction arm);
the only remaining difference is the pad register, which causes the two
`mov r5,fp` pad copies in the arms and the wrong join. A source shape that
keeps the faithful arm and the reuse of `x-pad`/`top-pad`/`boxW` but gives pad
`r9` would be byte-exact. No such shape was found in ~200 measured variants.

## 8. Harness (all in `/dev/shm/w4118`, not committed)

* `oracle.py` — copy of `_work/reconstructions/w3692_oracle.py` with `ROOT`,
  `ORIG` and output paths patched for this worktree.
* `score_variant.py` — compile one TU with the official command and score
  `_ZN11SpaceLounge10drawLoungeEv` by normalised instruction diff.
* `search.py` — block template (`HEAD`/`CLAMP`/`TAIL`) and `test(name, clamp,
  tail, head)` helper.
* `runllc.py` — run the instrumented `llc` on `dl.ll` with `RA_NOHINT` /
  `RA_FORCE_HINT` and dump the overview `drawBox` region.
* `classify.py` — read `@DEBUG_VALUE` comments to recover the `x`/`pad`/`top`/
  `lineHeight`/`layout`/`agent` registers.
* `dl.ll` — extracted `drawLounge` IR (official compile command + `-S
  -emit-llvm`, `llvm-extract`).
* `base.diff`, `orig.asm`, `ours.asm` — side-by-side dumps.
* `batch1.py` … `batch15.py` — the variant batches tabulated above.
* `bwtest.py` — branch-weight-flag sweep.
* `sweep_ra.py`, `force_results.txt` — hint sweeps.

## 9. Addendum: RA trace and the `faith_xy` near-miss (same session)

Two extra pieces of evidence were gathered after §7 was written. Neither
changes the conclusion, but they pin the mechanism more precisely.

### 9.1 Full greedy-RA trace (instrumented `llc`, `RA_TRACE=1`)

The base IR (`/dev/shm/w4118/dl.ll`, vreg numbers: pad `%2`, x `%575`,
top-pad `%46`, x-pad `%45`, boxW `%47`, lineHeight `%44`, pad*2 `%369`)
produces this exact order:

```text
ENQ  %2 pad       size=6304  prio=0xA0000000+6304  hint=0,0
select %575 x     size=4204  hint=0,$r3  -> $r5
select %47 boxW   size=768   hint=0,$r3  -> $r6
select %46 toppad size=736   hint=0,$r3  -> $r5   (evicts x: 6.25e-4 > 2.96e-4)
select %575 x     size=4204  hint=0,$r3  -> $r9   (r9 still free)
select %45 xpad   size=704   hint=0,$r2  -> $r6
select %47 boxW   size=768   hint=0,$r3  -> $r10
select %2  pad    size=6304  hint=0,0    -> $r11  (tryEvict)
select %369 pad*2 size=6064  hint=0,%370 -> $r10
select %44 lineH  size=368   hint=0,0    -> $r11  (evicts pad; 6.10e-4 > 2.18e-4)
ENQ  %2 pad       size=6304  prio=0xA0000000+6304  (stage 1)
select %2  pad    -> no free register (all interfere)
ENQ  %2 pad       size=6304  prio=6304            (stage 2, split)
```

So x is **not split at all** in our build: it is evicted whole from `$r5` by
`%46` and immediately re-assigned whole to the free `$r9`. The target's
`x = r5 -> r9` split requires `$r9`, `$r10` and `$r11` to be occupied when x is
re-enqueued (pad, pad*2 and lineHeight respectively), so that `tryAssign`
fails and the second-round `trySplit` runs. In our build pad/pad*2/lineHeight
are all allocated *after* x's re-enqueue.

### 9.2 Explicit physical hint on pad does not reproduce the target

The instrumented `llc` in `/tmp/llvm_android_702b/build2` has a `HINTVREG`
knob (`vreg:phys` pairs) that calls `MRI->setRegAllocationHint` at enqueue.
Setting `HINTVREG=2147483650:75` (pad -> `$r9`) gives:

```text
ENQ  %2 pad  size=6304 prio=0xE0000000+6304  hint=0,$r9
select %2  pad -> $r9  (tryAssign)          <- pad does reach r9
select %575 x  -> $r5
select %46     -> $r5  (evicts x)
select %575 x  -> $r10 (r10 still free)     <- x whole, not split
select %47     -> $r11
select %369    -> $r11
select %44     -> $r8  (tryEvict)
ENQ  %2 pad  (stage 1) ; select -> no free register ; ENQ stage 2 (split)
```

The final asm has `x=r10`, `pad=r8` (pad is evicted/split later). This corrects
the §7 bullet that said a pad hint would make it take `r5/r8`: with an explicit
`$r9` hint pad *does* take `r9` first, but x then takes the free `$r10` and pad
is later evicted by lineHeight. Hinting pad, pad*2 and lineHeight together
(`r9/r10/r11`) also fails because the hints perturb the earlier eviction order
(`x=r8`, `pad=r11`). So a single source-level copy hint on pad is not enough;
the target needs the *whole* r9/r10/r11 trio live at x's re-enqueue.

### 9.3 Closest join seen from source: `faith_xy` (89.6 %, 1940 B)

The §4.3 "all three direct" variant was re-run as all eight subsets of
`{x-pad, top-pad, boxW}` made direct in `DrawRectangle`. The best is
`faith_xy` — faithful clamp, `x - Globals::layout->field_0x94` and
`top - Globals::layout->field_0x94` direct, `pad * 2 + width` still using the
local:

```text
5a8: itt gt
5aa: addgt.w r1, r6, sl      ; width + pad*2      (sl = r10)
5ae: subgt   r5, r0, r1      ; x = (w-inset) - …
5b0: ldr r1, [pc,#464]
5b2: ldr.w r8, [r4,#48]      ; lineHeight = r8
...
5c0: vmov fp, s16            ; top
5c4: add  r6, sl             ; boxW
5c6: sub.w r2, r5, r9        ; x-pad      (x = r5, pad = r9)
5ca: sub.w r3, fp, r9        ; top-pad
5ce: strd r6, r8, [sp]
...
      DrawRectangle: ldr.w r3,[r1,#148]  ; reloads field_0x94
```

This is the first source shape that gives the original's two-instruction clamp
arm **and** `x=r5`, `pad=r9`, `pad*2=r10` simultaneously. It still scores only
89.6 because the direct `field_0x94` reload in `DrawRectangle` makes the
compiler recompute `x-pad`/`top-pad` straight into the call-argument registers
(`r2`/`r3`) instead of keeping them in `r6`/`r5` across `drawBox` as the
original does, and `lineHeight` lands in `r8` rather than `r11`. So the target
colouring is reachable from a source shape, but that shape also changes the
reuse structure; no shape found keeps both.

### 9.4 Net result

Re-ran the gate after all experiments: `byte 4498 linked 4498 stubs 0 missing 0
extra 0`, `data 843/843`, `sodiff 0/0`, imports 207, lint CLEAN, drift 0 —
unchanged. No `src/` change landed; this is a note-only pass.
