# SpaceLounge::drawLounge — wave-190 raw-lever sweep (w13464)

Status: research note (2026-11-12). Worktree `wt/w13464`, branch `wave190-w13464`,
base `83d52fe52`. **No `src/` change landed**: every size-exact shape tested keeps
the committed raw/aligned score. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, 741/741 insns).

* baseline: raw **175**, aligned **41**, size **1948/1948**, insn **741/741**
* after: unchanged (note only)
* gate re-run at the end: `RATCHET PASS` — byte 4507 / linked 4507, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN

Prior work: `spacelounge-drawlounge-wave190-w13397.md` and the notes it cites.
This note adds the current-base RA trace (the wave-190 note's `ra.log` was from a
variant: it shows `x=%607`, `pad size=6368`; the pristine HEAD is `x=%609`,
`pad size=6400`), the forced-assignment oracle results, the full source-shape
sweep, and the LSDA/cleanup landing-pad mapping.

## 1. Residual is still pure RA

`tools/fn_diff.py` gives 175/41/1948/741. Byte runs (25, 175 B total):

| offset run | len | what |
|---|---:|---|
| 0x189, 0x19c | 1+1 | `ldr.w pad` / `mov.w sl, pad, lsl #1` register field |
| 0x57d–0x5e1 | ~73 | clamp condition + right arm |
| 0x5e2–0x627 | ~64 | join arg shuffle (`mov r5,fp` vs `mov r3,r5`/`mov r9,r3`) |
| 0x65c, 0x67a | 1+1 | `add r0, top-reg` / `mov r0, agent-reg` in DrawString |
| 0x720–0x76c | ~37 | cleanup landing pads (extra `b.n`) |
| 0x78c | 1 | literal-pool branch offset |

Everything before 0x57d is byte-identical except the two pad-register fields, so
the whole residual is the same root cause: **pad is in `fp` (r11) in ours and in
`r9` in the original**, which rotates `x`, `top`, `agent`, and the join copies.

```
value        original        HEAD
pad          r9              fp (r11) + copy in r5
x            r5 -> r9        r9 (whole)
top-pad      r5              r5
x-pad        r6              r6
boxW         sl (r10)        sl (r10)
lineHeight   fp (r11)        fp (r11) after the join
top          r8 -> fp        r8
agent        r8              [sp,#12] -> r5
```

## 2. Current-base RA trace (pristine HEAD)

Instrumented clang `/tmp/llvm_android_702b/build2/bin/clang++`; register enum
offset 66 (`r0=66 … r11=77`). Raw vreg numbers are `0x80000000 + n`.

```
ENQ %2  (pad)  size=6400 prio=0xA0001900 stage=1   (bit31|bit29, no hint)
ENQ %609 (x)   size=4220 prio=0xE000107C stage=1   (bit31|bit30|bit29, hint r3)
select %609 -> $r5 (tryAssign)
select %47  (boxW)    -> $r6
select %46  (top-pad) TRYASSIGN2 phys=$r9 cheap=$r5 cost=1 -> $r5   # evicts x
ENQ %609 (re-enqueue)
select %609 TRYASSIGN2 phys=$r9 cheap=$noreg cost=1 -> $r9          # whole, no split
select %45  (x-pad)   -> $r6
select %47  again     -> $r10
select %2  (pad)  all r4..r11 interfere; r11 holds %365(w=2.14e-4)
                  TRYASSIGN1 -> $noreg; tryEvict -> $r11; re-enqueue; FINAL $noreg
```

The decisive step is `%46`'s `TRYASSIGN2 phys=$r9 cheap=$r5 cost=1`: `r9` is
free but has `CostPerUse=1`; `tryEvict` finds the cost-0 `r5` (x, weight
2.95e-4 < %46's 6.25e-4) and evicts it. `evictInterference` unassigns the
*whole* x interval and re-enqueues it; re-selection finds `r9` free and takes
it whole. There is no `trySplit` on this path, so x never keeps `r5` for the
clamp.

The original needs `pad=r9` **and** `x` split (`r5` in the clamp, `r9` after
the join). That is only possible if pad is selected before x (pad size 6400 >
x size 4220, so pad needs bit30) *and* pad's r9 interval is dead when x's
post-join interval is placed. In the pre-RA MIR the phi copy `%43 = COPY %609`
sits at the top of the join block, i.e. **before** the `%489 = top - pad` /
`%490 = x - pad` uses, so x_post overlaps pad and cannot share r9.

## 3. Forced-assignment oracle (instrumented clang)

`FORCEVREG`/`FORCEEVICT` (raw vreg numbers; r5=71, r9=75):

| forced | raw | aligned | size |
|---|---:|---:|---:|
| pad→r9, x→r5 (whole x) | 559 | 170 | 1952 |
| pad→r9, x→r5, top-pad→r5 | 559 | 170 | 1952 |

Forcing the whole x to r5 is gate-fatal (1952 B) because the post-join x must
be r9; the split cannot be expressed by a single vreg force.

## 4. Source-shape sweep (all size-exact unless noted)

Every variant below was compiled with the official command and measured with
`tools/fn_diff.py`. HEAD is 175/41/1948.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `int x` declared before width / after centerX / after top / before name | 175 | 41 | 1948 |
| clamp moved before the width block | 175 | 41 | 1948 |
| nested `if` / no braces / explicit `else x = centerX` | 175 | 41 | 1948 |
| `x0 = centerX; x = x0;` cond uses x0 / x / mixed | 175 | 41 | 1948 |
| `int x2 = x;` then use x2 everywhere | 175 | 41 | 1948 |
| ternary phi (nested `?:`) | 175 | 41 | 1948 |
| per-arm `int nx = ...; x = nx;` | 175 | 41 | 1948 |
| `int pad2 = pad;` (clamp / all uses / after load / reverse copy) | 175 | 41 | 1948 |
| `const int pad` / `const int centerX` / `const int x` | 175 | 41 | 1948 |
| `long x` / `unsigned width` / `long width` | 175 | 41 | 1948 |
| `x - pad` and `top - pad` locals after the clamp (both orders) | 175 | 41 | 1948 |
| `int boxW = pad*2 + width;` after the clamp | 1853 | 258 | 1968 |
| `int ypad = top - pad;` after the clamp | 265 | 50 | 1948 |
| `top` via `(float)(pad*2)` / `(float)(pad<<1)` | 175 | 41 | 1948 |
| `top` via `(float)pad - (float)pad` / int form | 1845/1847 | 163/168 | 1968/1956 |
| `x = w - inset - (width + pad + pad)` | 176 | 42 | 1948 |
| `x = w - inset - (width + pad*2)` (faithful) | 445 | 54 | 1948 |
| `x = w - inset - width - pad - pad` (left assoc) | 468 | 64 | 1944 (fatal) |
| `x = w - inset + (-width - pad - pad)` (HEAD) | **175** | **41** | **1948** |
| `cond1` as `inset > x - pad` | 177 | 43 | 1948 |
| `cond2` as `w - inset < pad + x + width` | 176 | 42 | 1948 |
| `cond2` as `pad + width + x` / `x + width + pad` | 175 | 41 | 1948 |
| `role + String("  ")` reversed / `name + ...` reversed | 177 | 43 | 1948 |
| `String("  ", false)` / `String("", false)` | 175 | 41 | 1948 |
| `plain if` (no `__builtin_expect`) | 550 | 76 | 1952 |
| `__builtin_expect(isKnown() != 0, 1)` / `!!isKnown()` | 175 | 41 | 1948 |
| `int x` volatile | 1874 | 445 | 1972 |
| `int pad` unsigned / `int x` unsigned | 177/176 | 43/42 | 1948 |
| `Layout *layout` hoisted before pad load / after SetColor | 1860/1859 | 276/287 | 1972 |
| `lineHeight` hoisted before width / before clamp | 1867/588 | 169/79 | 1968/1952 |
| `String empty("")` before the clamp | 1922 | 347 | 2020 |
| `pad` fully inlined as `Globals::layout->field_0x94` | 599 | 86 | 1960 |

Instrumented-compiler knobs (same base source):

| knob | raw | aligned | size |
|---|---:|---:|---:|
| `HINTVREG=pad:r9` | 1883 | 209 | 1980 |
| `HINTVREG=pad:r9,pad2:r10` | 188 | 57 | 1948 |
| `HINTVREG=pad:r9,top-pad:r5,boxW:r10` | 188 | 57 | 1948 |
| `HINTVREG=pad:r9,top-pad:r5,boxW:r10` + trace | pad→r9, x→r5, boxW→r10, top-pad→r5, **x re-enq→r11** | | |
| `PRIOVREG=pad` | 1884 | 225 | 1980 |
| `NOEVICT=x` | 1912 | 257 | 1996 |
| `XWFACTOR=1.5/2.0` | 203 | 62 | 1948 |
| `-regalloc-csr-first-time-cost=1..26` | 175 | 41 | 1948 |
| `-condsider-local-interval-cost=true` | 175 | 41 | 1948 |
| `-enable-deferred-spilling=true` | 1837 | 123 | 1960 |
| `-join-liveintervals=false` | 1867 | 605 | 2016 (fatal) |
| `-join-globalcopies=false` / `-terminal-rule` | 175 | 41 | 1948 |

With `pad→r9` hinted, pad is selected first and gets r9; x gets r5; `%46`
still evicts x from r5; x is re-enqueued and takes the first free callee-saved
(r11, then r10 in other knob combos) because r9 is still live with pad. That is
the 188/57 family — worse than HEAD.

## 5. LSDA / cleanup landing-pad mapping (new)

LSDA at 0x1c713c, call-site table at 0x1c714d, len 0x91; offsets are relative
to the function start (0x170d38).

| entry | call site | original LP | ours LP |
|---|---|---|---|
| 1 | 0x51c (String("  ") ctor for role) | 0x752 → 17148a (name only) | 0x754 → 17148c (name only) |
| 2 | 0x532 (GetTextWidth(name)) | 0x756 → 17148e (role+name) | 0x74c → 171484 (**bare `b.n`** → role+name) |
| 3 | 0x5b4/0x5c0 (String("") ctor) | 0x756 → 17148e (role+name) | 0x758 → 171490 (role+name) |
| 4 | 0x5f2/0x5f6 (String("") dtor) | 0x756 → 17148e (role+name) | 0x758 → 171490 (role+name) |
| 5 | 0x626/0x624 (isKnown / "  " ctor) | 0x74a → 171482 (bare → role+name) | 0x74a → 171482 (bare → role+name) |

The original shares the role+name landing-pad body between entries 2/3/4; ours
gives entry 2 its own bare-branch pad (171484), i.e. one extra `b.n` (2 B) plus
the shifted branch targets. This is downstream of the 2-byte clamp shift, not
an independent lever: the cleanup body offsets move with the clamp size, and
every source change that touched the String temporaries changed `.text` size.

## 6. Conclusion

No size-exact raw win was found. The committed flat clamp remains optimal:
**raw 175 / aligned 41 / 1948 B / 741 insns**. The residual is the greedy-RA
fact that `x` (hinted, prio `0xE0000000+4220`) is selected before `pad`
(`0xA0000000+6400`), `%46` evicts x from r5 via the cost-0 eviction path, and
the re-enqueued x takes the free r9 whole. To match the original, pad must be
in r9 *and* x must be split at the join; every source shape found that forces
pad into r9 either changes the value graph (faithful right arm, 445) or leaves
x in r11/r10 instead of the r5→r9 split (188–243), and the r9/x_post overlap
in the pre-RA MIR blocks the split without a non-coalesced phi.

## 7. Reproduction

```bash
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13464` (`tryvar.py`,
`tryforce.py`, `tryflag.py`, `batch8.py`..`batch21.py`, `compile_inst2.sh`,
`compile_flag.sh`, `linkcmd.sh`, `base.cpp`, `ra_base.log`, `hintpad.log`,
`h3.log`, `coaldbg.txt`, `preall.mir`). The instrumented clang is
`/tmp/llvm_android_702b/build2/bin/clang++`; its `HINTVREG`/`PRIOVREG`/
`NOEVICT`/`XWFACTOR`/`FORCEVREG` knobs and the `RA_TRACE` env are documented
above (register numbers offset by 66).

## 8. Addendum (same session): MIR phi/coalescing detail and extra cells

The final pre-RA MIR (`-mllvm -print-after=...`, function body at the last
`# Machine code for function _ZN11SpaceLounge10drawLoungeEv` block) shows the
join block as:

```
%43:rgpr = PHI %3:rgpr, %bb.41, %42:gpr, %bb.42, %40:gpr, %bb.40
%489 = t2SUBrr %389:rgpr, %2:rgpr     ; top - pad
%490 = t2SUBrr %43:rgpr, %2:rgpr      ; x - pad   <-- uses the PHI, not %388
%47 = COPY %488 ; boxW
%46 = COPY %489 ; top-pad
%45 = COPY %490 ; x-pad
```

So the pad uses consume the join PHI `%43`, and the coalescer merges that PHI
with the `centerX` vreg (`%388`, the `COPY %386:spr` that carries the `$r3`
allocation hint): the RA trace interval is
`[6784r,6896B:0)[6912r,6976B:2)[6976B,7168B:0)[7232r,7280B:1)[7280B,11088r:3]`
— one vreg live through the join to 11088r.  `%46` starts at 7568r, inside
that last segment, hence the cost-0 eviction of x from r5.  The original needs
`%388`=r5 (ends before the join) and the PHI in r5 then split to r9 after the
`drawBox` call; the merged vreg makes that split impossible.  The faithful
right arm (`-(width + pad*2)`) does emit the original's
`addgt.w r1, r6, sl ; subgt r1, r0, r1`, but its pad interval is 6368 (one
fewer pad use) and pad is fully spilled/split (`FINAL %2 -> $noreg`, then
`%644/%645/%646`), giving 445.

Extra source cells (all measured with the official command; HEAD is
175/41/1948):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `x = w - inset + (-pad - pad - width)` (reordered neg) | 175 | 41 | 1948 |
| `x = w - inset - width + (-pad - pad)` | 175 | 41 | 1948 |
| `x = w - inset - (width + pad) - pad` | 469 | 65 | 1944 (fatal) |
| `x = w - (inset + width + pad + pad)` | 477 | 77 | 1952 (fatal) |
| `x = w - inset + (-width - (pad + pad))` | 445 | 54 | 1948 |
| `if (x < inset + pad)` / `if (x < pad + inset)` | 492 | 73 | 1944 (fatal) |
| `if (pad > x - inset)` | 178 | 43 | 1948 |
| `else if (width + x + pad > w - inset)` | 175 | 41 | 1948 |
| `else if ((pad + x) + width > ...)` / `pad + (x + width)` | 175 | 41 | 1948 |
| `else if (pad + x + width + inset > w)` | 524 | 100 | 1940 (fatal) |
| `boxW = width + pad * 2` | 175 | 41 | 1948 |
| `boxW = pad + pad + width` / `width + pad + pad` | 1851 | 249/250 | 1964 (fatal) |
| `top = bottomRight.y - (float)pad - (float)pad` | 1845 | 163 | 1968 (fatal) |
| `centerX = (int)((topLeft.x + bottomRight.x) * 0.5f)` | 182 | 46 | 1948 |
| `int left = x - pad; int above = top - pad;` locals | 175 | 41 | 1948 |
| same, locals also used in DrawRectangle | 175 | 41 | 1948 |
| drawBox before DrawRectangle / reordered | 1884 | 268 | 1980 (fatal) |
| `SetColor(0xffffffff)` moved after centerX | 1506 | 201 | 1944 (fatal) |

The `left`/`above` locals do not change the MIR order (`%489` top-pad is still
before `%490` x-pad), so the eviction is unchanged.
