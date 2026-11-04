# MGame::OnUpdate — idle-contact `Status*` r5/r9 residual (wave 105, w5312)

Status: research pass, 2026-11-04. Worktree `wt/w5312`, branch `wave105-w5312`,
base `835748c18`. No source change landed: the function is already at its best
measured shape (41 diff bytes, size exact, 99.9 %). This note re-derives and
extends the wave-97 analysis (`mgame-onupdate-r5-cache-wave97-w4717.md`) with
the greedy-allocator cascade and the body-entry split.

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8 (offset 0 = 0x17c8d8).

## 1. Exact remaining diff

`python3 fdiff.py _ZN5MGame8OnUpdateEv` on the current source:

```
size 11440 11440
diffbytes 41 runs 2
0x17ba 0x17ba 1
0x17bc 0x17e3 40
```

The whole residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2770-2776`). Original (`0x17e08c`) vs ours:

```
original                                    ours (current source)
17e08c ldr.w r9,[r0]   ; r9 = &Globals::status
17e090 ldr.w r0,[r9]                       17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission       17e094 mov   r0,r5
17e098 cmp   r0,#23                        17e096 blx   getCurrentCampaignMission
17e09a bne   17e176                        17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne   17e176
17e0a0 blx   isMining                      17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                         17e0a2 blx   isMining
17e0a6 bne   17e176                        17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]                       17e0a8 bne   17e176
17e0ac blx   getMission                    17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                        17e0ae blx   getMission
17e0b4 cmp   r0,#0                         17e0b2 blx   isEmpty
17e0b6 beq   17e176                        17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]                       17e0b8 beq   17e176
17e0bc blx   getPlayingTime                  17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]                       17e0bc blx   getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

The original has **four fresh `ldr.w rX,[r9]` loads and no cached `Status*`**.
Size is neutral (4+4 = 4+2+2), so the run is a pure byte permutation.

## 2. Source witness: the original is the no-`st` shape

The Unity-remake decompilation of the same routine
(`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/MGame.c:4906-4990`)
renders the whole idle block with direct globals:

```c
iVar4 = Status::getCurrentCampaignMission(Globals::status);
if ((iVar4 == 0x17) && (iVar4 = PlayerEgo::isMining(...), iVar4 == 0)) {
  pMVar13 = (Mission *)Status::getMission(Globals::status);
  iVar4 = Mission::isEmpty(pMVar13);
  ...
  lVar39 = Status::getPlayingTime(Globals::status);
  ... *(longlong *)(Globals::status + 0x100) ...
```

so the original C++ is `Globals::status->...` four times, i.e. the IR is the
no-`st` shape (`%1091/%1098/%1102/%1104` distinct loads). The `st` local in our
tree is a **size/RA anchor, not a source witness**.

## 3. Why the anchor is needed: greedy-RA cascade

Dropping `st` (all four accesses through `Globals::status->`) gives
**11392 B / 5148 diff / 208 runs**. The GOT base `&Globals::status` moves from
r9 to r5, every `ldr.w rX,[r9]` in the idle block becomes a 2-byte `ldr rX,[r5]`,
and the function re-colours (48 B smaller).

Faithful `-Oz` machine-IR (`clang -Oz -mllvm -print-after-all`, or the
`llc -O2` replay used here, which is byte-identical for this function) shows the
mission-23 block in both builds:

```
baseline (st)                          no-st
42016 %GOT = t2LDRi12 %lit, 0          42016 %GOT = t2LDRi12 %lit, 0
42032 %128 = t2LDRi12 %GOT, 0  ; st    42032 %1053 = t2LDRi12 %GOT, 0 ; ccm
42064 $r0 = COPY %128                  42064 $r0 = COPY %1053
42080 tBL getCurrentCampaignMission    42080 tBL getCurrentCampaignMission
```

Greedy RA (`-debug-only=regalloc` on the emitted IR):

* baseline: `%128` (st, weight 1.6e-06) is processed first and takes **r5**;
  the GOT base `%1053` (weight 2.48e-07) then cascades `r6` -> `r10` -> **r9**
  (evicting `%131/%138/%1266`, then `%712/%764`), and stays whole in r9.
* no-st: the GOT base `%1052` (weight 2.51e-07, `hints: $r9`) finds r9 occupied
  by `%2939` and takes **r5** directly; at the body entry (slot 42872) the
  allocator emits `%2931 = COPY %2932` -> a real `mov r9,r5` instruction, and
  the body uses r9. r5 is genuinely free at the block in the original too
  (last r5 write before it is `strb r0,[r5]` at 0x17e044; next is
  `ldr r5,[r0]` at 0x17e12a), so the r9 colour is a whole-function greedy-RA
  split/rematerialisation decision, not a source-level r5 occupant.

The constant 1 (`%1155`, slots [41488,42176)) is in r5 in both builds but is
rematerialised at its uses (`movs r0,#1`), so it does not pin r5.

## 4. Variants measured this pass

All measured against the linked original with `link_so` + `fdiff.py`
(size, diff bytes, runs). Every shape that reproduces the four fresh loads
re-colours the GOT base to r5 and loses ~48 B / 200+ runs.

| variant | size | diff | runs |
|---|---:|---:|---:|
| baseline `st` at getPlayingTime (current) | **11440** | **41** | **2** |
| `st` at ccm + getPlayingTime | 11440 | 41 | 2 |
| `st` at ccm only | 11392 | 5040 | 208 |
| `st` at getMission only / ccm+getMission | 11396 | 5029 | 208 |
| `st` at ccm+getMission+getPlayingTime | 11436 | 4906 | 208 |
| `st` at getPlayingTime+field | 11436 | 4867 | 208 |
| no `st` (reference) | 11392 | 5148 | 208 |
| `Status *&st = Globals::status` (local or namespace-scope alias) | 11392 | 5040 | 208 |
| `Status *volatile *st = &Globals::status` (all four) | 11392 | 5040 | 208 |
| `*(Status *volatile *)&Globals::status` (all four) | 11392 | 5040 | 208 |
| volatile ccm read + `st` at pt | 11440 | 853 | 36 |
| `Status *volatile st` | 11340 | 6076 | 317 |
| `long long &last = ...field_0x100` + `st` at pt | 11440 | 55 | 2 |
| `st` declared before outer/inner `if` | 11392 | 8623/8626 | 240 |
| `st` at function top | 11372 | 10647 | 240 |
| `st` + `llvm.assume(st != null)` (IR-level) | 11392 | 5040 | 208 |
| `st` + volatile IR load (IR-level) | 11392 | 5040 | 208 |
| discarded volatile read of `Globals::status` + no `st` | 11392 | 5148 | 208 |
| `Status *st = *(Status *volatile *)&Globals::status` at pt | 11440 | 41 | 2 |

The 16-way sweep over which of the four calls use `st` confirms that only
`{ccm, getPlayingTime}` (and its CSE-equivalent spellings `Status *const`,
`auto *`, `(Status *)`, `&*`, `*(&)`, `+0`, `0+`, `(*st).`, `(&*st)`) yields 41.

### 4.1 The `volatile` lever is a dead end

Three further experiments (this pass) show why:

* A **discarded** volatile read, `(void)*(Status *volatile *)&Globals::status;`,
  is deleted by clang 7 at `-Oz` (discarded-value expression): the result is
  byte-identical to the plain no-`st` build (11392 / 5148 / 208), so it cannot
  be used to park a value in r5.
* A volatile read whose value *is* used (`Status *st = *(Status *volatile
  *)&Globals::status;` with `st` at `getPlayingTime`) is byte-identical to the
  baseline (11440 / 41 / 2): clang legally reuses the volatile-loaded value for
  the later non-volatile `Globals::status` load at the `ccm` call, so the
  `volatile` qualifier buys nothing.
* Two volatile reads (one for `st`, one at `ccm`) do produce two loads
  (11440 B, 855 diff bytes vs the C++ reference object) but still re-colour the
  block, so they are not the target either.

So `volatile` can only add or re-order loads; it cannot make the allocator keep
the GOT base whole in r9 while leaving the four status loads uncached.

## 5. Next hypothesis

The residual is not a source-shape problem: the original IR is the no-`st`
shape and the only missing piece is a **code-neutral IR difference** that makes
the greedy allocator prefer "keep the GOT base whole in r9 (CostPerUse 1)" over
"rematerialise it into r5 (CostPerUse 0) at every branch". Candidate levers, in
order of promise:

1. Make the GOT load non-rematerialisable (e.g. a declaration/attribute change
   that removes `local_unnamed_addr` from `@_ZN7Globals6statusE`, or a
   `volatile` GOT load) so the allocator must keep it in one register. This
   requires touching `Globals.h`, not `MGame::OnUpdate`.
2. A source witness for a value that pins r5 during slots 42032-42608 without
   emitting code (a dead-but-live value). `llvm.assume` and a volatile IR load
   were tried and are removed before RA.
3. A code-neutral IR difference elsewhere in OnUpdate that changes the greedy
   priority order (e.g. the `%2939` interval that blocks the r9 hint in the
   no-`st` build). This is a chaotic search; it needs an IR oracle, not C++.

## 6. Gate (note-only pass)

`check` green: byte_exact **4500/4524**, linked_exact **4500/4524**,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.
Before -> after: byte 4500 -> 4500, linked 4500 -> 4500.

Scratch drivers in `/dev/shm/w5312` (`ctry.py`, `fdiff.py`, `brute.py`,
`ref_greedy.txt`, `nost_greedy.txt`, `ref_ra.txt`, `nost_ra2.txt`).
