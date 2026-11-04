# PlayerEgo::calcCollision — pass 11 (w4813): the wrap form is the original's
# structure; only the `dock`/`i` register pair is wrong

**Status:** note only, 2026-11-04, wave 98. Worktree `wt/w4813`, branch
`wave98-w4813`, base master `fe4f5e7cf`.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B,
347 instructions).  The landed source is unchanged (pass-9 two-anchor
shape, **87.6 % / 968 B / 349 insns**).  No strict score improvement was
found this pass; the value of the pass is a much sharper picture of *what
is actually left*.

| | score | size | insns | mnemonic¹ | this | dock | i | surface | canary |
|---|---:|---:|---:|---:|---|---|---|---|---|
| original | 100 | 968 | 347 | — | r5 | r9 | fp | r6 | `[sp,#24]` |
| committed (pass 9) | **87.6** | 968 | 349 | 96.8 % | r5 | r9 | fp | r6 | `[sp,#24]` |
| **wrap after `outerCollide`** | 87.3 | **968** | **345** | **98.0 %** | r5 | **fp** | **r9** | r6 | `[sp,#24]` |
| faithful (no anchors, no guard) | 59.8 | 988 | 349 | — | **r9** | r9 | fp | **r5** | `[sp,#20]` (`strd`) |
| faithful + `collision != Vector` | 60.1 | 988 | 349 | — | r9 | r9 | fp | r5 | `[sp,#20]` |
| faithful + real `i == 0` guard | 59.8 | 996 | 352 | — | r9 | r9 | fp | r5 | `[sp,#20]` |

¹ mnemonic ratio = `difflib.SequenceMatcher` over the instruction stream
with every operand stripped (mnemonic only, `.w`/`.n` suffixes removed).
It measures instruction *selection and order* only.

Gate (unchanged, verified with `check`):
`byte_exact 4500/4524`, `linked_exact 4500/4524`, size-exact 4524/4524,
`data_exact 843/843`, imports 207, lint CLEAN, drift 0, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26294 (−4)`.

---

## 1. New this pass: the wrap body's diff is *only* the `dock`/`i` swap

Pass 10 found the wrap form (`if (i < objects->size()) { … }` after
`outerCollide`) reproduces the original's un-rotated block layout at 968 B.
This pass diffed it instruction-by-instruction against the original.  The
**complete** non-matching set is:

```
--- replace 26 27 26 27      ORIG add.w  r9, r5, #328      OURS add.w  fp, r5, #328   (dock)
--- replace 31 32 31 32      ORIG mov.w  fp, #0            OURS mov.w  r9, #0         (i)
--- delete  35 36 35 35      ORIG str.w  r9, [sp, #20]                                 (preheader spill)
--- replace 40 41 39 40      ORIG cmp    fp, r0             OURS cmp    r9, r0
--- replace 43 44 42 43      ORIG ldr.w  r8, [r0, fp, lsl #2]  OURS ... r9
--- replace 46 48 45 47      ORIG cmp.w fp,#0; bne.n loc_ae  OURS cmp.w r9,#0; bne.n loc_aa
--- replace 51 53 50 52      ORIG cbnz r0,loc_ae; mov r0,r9  OURS ... mov r0,fp
--- replace 57/59            (branch targets only, −4)
--- insert  72 72 71 74      OURS ldr r0,[r4,#0]; cmp r9,r0; bcs.w loc_2c0   (the wrap)
--- replace 76/80/137/147/149  (branch targets only, ±8)
--- insert  90 90 92 93      OURS str.w fp, [sp, #20]                               (extra spill)
--- replace 93/95/100/103    ORIG ldr.w r9,[sp,#12]; mov r0,r9 …  OURS fp
--- replace 111/114          ORIG sub.w r9,r1,sl; cmp.w r9,#1  OURS fp
--- replace 178              ORIG add r1,pc,#436    OURS add r1,pc,#428   (pool offset)
--- replace 205 208 208 209  ORIG mov sl,r5; mov r5,r6; add r6,sp,#96
                             OURS add.w sl,sp,#96                  (collision shuffle)
--- replace 209/211/214/217  ORIG mov r2,r5; mov r0,r6; … mov r6,r5; mov r5,sl; mov r1,r6
                             OURS mov r2,r6; mov r0,sl; mov r1,sl
--- replace 225              (pool offset)
--- replace 233/238/240      ORIG mov r9,r0 …  OURS str.w fp,[sp,#20]; mov fp,r0 …
--- delete  246 248 246 246  ORIG cmp.w fp,#0; bne.n loc_2ba            (real `i == 0` guard)
--- replace 254/287/305/307/311/322   (dock/i swap, again)
```

Two things fall out of this:

1. **Every `loc_aa` vs `loc_ae` branch-target mismatch is exactly −4 B** —
   the missing preheader `str.w r9,[sp,#20]`.  The wrap body's *block
   order* is already the original's; only its instruction offsets are
   shifted by the absent 4-byte preheader spill.
2. The rest is the `dock`/`i` swap (≈ 25 instructions), the wrap block
   (3), the extra in-loop spill (1), the missing real guard (2) and the
   collision-block `mov` shuffle (4).  There is no other structural
   difference.  **The wrap body is the original's source shape.**

So the whole remaining problem is: get `dock` into `r9` and `i` into `fp`
in the un-rotated layout.

## 2. Why the allocator flips the pair (correcting pass 9/10)

Pass 8/9 blamed the anchor's hot use of `i`; pass 10 showed a wrap whose
condition does not mention `i` flips just the same.  This pass looked at
the actual live intervals (MIR `Live Interval Analysis`, NDK clang 7.0.2
`-mllvm -print-after-all`):

| body | vreg | role | interval (slot idx) | size |
|---|---|---|---:|---:|
| wrap | `%20` | `&dockOffsetVec` | 384 – 464 | 80 |
| wrap | `%0` | `dock` copy (guard) | 464 – 1184 | 720 |
| wrap | `%175` | loop counter `i` | 720 – 8352 | 7632 |
| committed | `%20` | `&dockOffsetVec` | 384 – 464 | 80 |
| committed | `%0` | `dock` copy | 464 – 1184 | 720 |
| committed | `%177` | loop counter `i` | 720 – 8448 | 7728 |

The two bodies have **the same vregs and essentially the same interval
sizes**, yet `RAGreedy` gives `r9` to `i` in the wrap body and to `dock`
in the committed body.  So this is not "the bigger interval wins" and not
"`i` is hotter because of an anchor": `i`'s interval is *larger* in the
committed (winning) body, 7728 vs 7632.

The mechanism is `calculateSpillWeightsAndHints` (`CalcSpillWeights.cpp`):
`weight = (Σ blockFreq over uses) / intervalSize * numInstr`, and
`shouldEvict` uses `A.weight > B.weight`.  `SlotIndex` numbering follows
the **machine block layout**, so changing the layout changes the segment
structure and therefore the weights.  The committed body's rotated layout
(where the latch `bb.39` is emitted right after the anchor `bb.12`) is
what lets `dock` evict `i` from `r9`.

Empirical confirmations that it is *not* the `dock` weight:

| variant on the wrap body | score | size | dock/i |
|---|---:|---:|---|
| baseline wrap | 87.3 | 968 | fp/r9 |
| + a second `VectorLength(this->dockOffsetVec)` use in the guard | 78.8 | 988 | **fp/r9** |
| + the real `i == 0 &&` guard in the collision block | 79.4 | 976 | **fp/r9** |
| `i == 0` guard moved to the end of the loop body | 67.4 | 980 | **r0/r9** |

Adding `dock` uses — even in the hot loop block — does not move it to
`r9`; moving the guard to the end moves it to `r0`, not `r9`.

## 3. Variant tables run this pass (all official `verify.score`)

### 3.1 Anchor form matrix on the committed body (`gt` = landed)

| anchor1 | anchor2 | `collision != Vector` | pct | size | insns |
|---|---|---|---:|---:|---:|
| `i > size-1` | `size == i` | no | **87.6** | 968 | 349 |
| `i > size-1` | `size == i` | yes | 87.4 | 968 | 349 |
| `i > size-1` | — | no | 79.5 | 964 | 347 |
| `i > size-1` | — | yes | 79.3 | 964 | 347 |
| `size <= i` | `size == i` | no | 85.5 | 976 | 348 |
| `size <= i` | — | no | 87.3 | 968 | 345 |
| `size <= i` | — | yes | 87.0 | 968 | 345 |
| `size == i` | `size == i` | no | 87.4 | 968 | 349 |
| `size == i` | — | no | 79.5 | 960 | 345 |
| `i >= size` | `size == i` | no | 85.5 | 976 | 348 |
| `i >= size` | — | no | 87.3 | 968 | 345 |

### 3.2 Anchor *forms* that are byte-identical to the landed body

| anchor1 text | pct | size | insns | note |
|---|---:|---:|---:|---|
| `i > objects->size() - 1` | 87.6 | 968 | 349 | landed |
| `objects->size() - 1 < i` | 87.6 | 968 | 349 | `cmp r0,fp; bcs.n` instead of `cmp fp,r0; bls.n` |
| `!(i <= objects->size() - 1)` | 87.6 | 968 | 349 | **byte-identical** to landed |
| `objects->size() - i < 1` | 87.4 | 968 | 349 | |

### 3.3 Anchor2 forms (with the landed anchor1)

| anchor2 | pct | size | insns |
|---|---:|---:|---:|
| `objects->size() == i` | **87.6** | 968 | 349 |
| `objects->size() <= i` / `i >= objects->size()` | 87.6 | 968 | 349 |
| `i == 0xFFFFFFFFu` | 79.0 | 964 | 347 |
| `i == 0` | 79.0 | 964 | 347 |
| `this->miningGame == 2` | 79.5 | 964 | 347 |
| `object->field_0x71 == 2` | 84.9 | 972 | 350 |
| `object->isVisible() && i == 0xFFFFFFFFu` | 84.3 | 980 | 353 |

### 3.4 Loop / declaration shape variants (committed body, all 87.6)

`++i`, `i += 1`, `int i` with `(unsigned int)` cast, `uint32_t i`,
`unsigned int i;` declared before the `for`, `KIPlayer *object;` declared
before the `for`, nested `if (i == 0) { if (…) … }`, split
`if (i == 0 && …) { if (field_0x71) … }`, `objects->size() > i` loop
condition.  Only `objects->size() > i` differs (87.1); everything else is
87.6.  The same list on the *wrap* body is uniformly 87.3.

### 3.5 Wrap-condition / wrap-position variants

`i < objects->size()` (landed wrap) 87.3; `objects->size() > i` 87.0;
`object->field_0x71 != 2`, `object->stealFlagByte != 2`,
`this->dockedFlag != 2` etc. 79.7 / 964 (pass 10); `i != objects->size()`
79.5 / 960; `i <= objects->size() - 1` 79.5 / 964.  Moving the wrap to
the top of the loop body, into the collision block, or merging it with
`isVisible()` all score ≤ 86.2 (pass 10).

### 3.6 Faithful body (no anchors) — the prologue is the blocker

The faithful body is 988 B (996 with the real guard) and gets
`this = sl`, `surface = r5`, canary `[sp,#20]` with a merged
`strd r6, r9, [sp, #20]`.  The cause is visible in the preheader MIR:
the canary *address* is kept in `r6` and merged with the `dock` store into
one `strd`, which pins `r6` and pushes `this` to `sl`.  The original
stores the canary address immediately (`str r0,[sp,#24]`) and the dock
address 0x24 bytes later (`str.w r9,[sp,#20]`), so no merge happens and
`this` keeps `r5`.  `collision != Vector{0,0,0}` (the provably original
argument order, see pass 8 §2) is worth +0.3 on the faithful body
(59.8 → 60.1) but −0.2 on the committed body (87.6 → 87.4).

## 4. Negative results (this pass)

* `collision != Vector{0.0f, 0.0f, 0.0f}` on the committed body: 87.4
  (re-measured; the pass-8 note's 87.0 was on an older shape).  It is the
  faithful order but a 0.2-point score regression, so it was not landed.
* `collision != Vector` on the wrap body: 87.0 (was 87.3).
* Real final guard `if (i == 0 && object->field_0x71)` on the committed
  body: 85.0 / 976; on the wrap body: 79.4 / 976.
* Anchor1 moved to the end of the loop body (after `hitCamera`), all three
  forms (`>`, `<=`, `==`): 70.8 / 984.
* `if (i == 0)`-style anchors (`i == 0`, `i == 0xFFFFFFFF`) do not un-flip
  `dock` and cost the 968-byte size.
* Adding a second `dockOffsetVec` use, or the real guard, does not move
  `dock` to `r9` in the wrap body (§2).
* Faithful body + nested `if (i == 0)`: 59.8 / 996 (no change).

## 5. Conclusion / next lead

The wrap body is **the** original source shape: 98.0 % mnemonic-identical,
the original's un-rotated block order, the original canary spill, and a
diff that is 90 % the `dock`/`i` pair.  The landed two-anchor body is
96.8 % mnemonic-identical and wins only because its rotated layout happens
to give `dock` `r9`.

The remaining work is therefore exactly:

> find a source shape that keeps the wrap's un-rotated layout **and**
> produces the committed body's spill weights, so `dock` evicts `i` from
> `r9` without the 8-byte second anchor.

Since the weights are layout-derived (`SlotIndex` numbering follows block
order), and the wrap's block order is already the original's, the likely
answer is *not* another anchor but a source detail that changes the
**preheader**: if `dock`'s address were spilled in the preheader (as in
the original) instead of at its first in-loop clobber, the whole function
would shift +4 B and every `loc_aa`-vs-`loc_ae` branch-target mismatch in
§1 would disappear.  Candidates not yet tried: forcing the canary and
dock stack slots apart (a real local that shifts `LocalStackSlotAllocation`),
or a `dockOffsetVec` access form that makes the address die at the
preheader store.

## 6. Reproduction

```sh
cmake --build cmake-build-match --target check -j8       # RATCHET PASS
python3 /dev/shm/w4813/fastscore.py                      # pct / size / insns / sig
python3 /dev/shm/w4813/normdiff.py                       # normalised instruction diff
python3 /dev/shm/w4813/mnem.py                           # mnemonic-only ratio
```

Scratch harnesses (throw-away, `/dev/shm/w4813`): `splice.py` splices a
body file into `src/game/ship/PlayerEgo.cpp`; `run.sh` / `batch.sh`
splice + `link_so` + score; `fastscore.py` is the in-process official
scorer; `gen.py`, `gen2.py`, `gen3.py` generated the variant tables in
§3; `extract2.py` pulls one function's MIR out of a
`-mllvm -print-after-all` dump (`mir.txt`, `mir_wrap.txt`,
`mir_committed.txt`); `csw.cpp` is LLVM 7.0.1 `CalcSpillWeights.cpp`,
`rag.cpp` is `RegAllocGreedy.cpp`, `mbp.cpp` is
`MachineBlockPlacement.cpp`.
