# MGame::OnUpdate — compare-spelling sweep, the `st_cond` family, and the post-RA
# scheduler's store-before-copy order, wave 191b / w13654

Status: **no landed `src/` change** (negative sweep).  Worktree `wt/w13654`,
branch `wave191b-w13654`, base master `2f38a4bfd`.  Target
`_ZN5MGame8OnUpdateEv` stays at **raw 8 / aligned 108 / 11,440 B,
insn 4003:4002**.  `.text` content delta **71,196 B**, `.ARM.extab` content
delta **577 B**, `.ARM.exidx` 0, `.rodata` 0, all section sizes exact.
`check` re-run at the end: **RATCHET PASS** (byte 4508, linked 4508, size 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).  No
baseline touched.

Follows `mgame-onupdate-wave191b7-w13623.md` (the read-back anchor that
took raw 10 -> 8), `…b6-w13608.md`, `…b5-w13592.md`, `…b4-w13565.md`.

## 1. Residual (unchanged, re-confirmed)

* m23 `0x17e2b6..bd` (2 B): orig `ldr.w r2,[r9]; ldrd r2,r3,[r2,#256]`;
  ours `ldr.w r5,[r9]; ldrd r2,r3,[r5,#256]`.  The anchor value is live
  across the `ldrd` (it is the m23 `Globals::status` load `%142`), so RA
  must keep it in r5; the original lets r2 die in the `ldrd`.
* m122 `0x17f190..196` (6 B): orig `mov r4,r0; ldr.w r0,[r9]`;
  ours `str r5,[sp,#120]; mov r4,r0; ldr r0,[sp,#120]`.  The store+read-back
  exactly replaces the 4-byte `ldr.w r0,[r9]` (size-neutral), but the
  post-RA scheduler emits the store before the `mov r4,r0`.

## 2. New evidence: the order is the post-RA scheduler, not the pre-RA scheduler

`-mllvm -print-machineinstrs` on `MGame.cpp` (142 MB dump, scratch only)
shows the block order through every pass:

| pass | order |
|---|---|
| instruction selection / machine-scheduler / greedy / virtregrewriter / prologepilog / if-converter / t2-reduce | `mov r4,r0` then `str r5,[sp,#120]` then `ldr r0,[sp,#120]` |
| **post-RA top-down list latency scheduler** | `str r5,[sp,#120]` then `mov r4,r0` then `ldr r0,[sp,#120]` |

So the copy is first in the MIR all the way to `post-RA-sched`; the
top-down latency scheduler hoists the store because its operand (r5) is
ready while the copy depends on the call result (r0).  Disabling both
machine schedulers (`-enable-misched=false -enable-post-misched=false`)
does **not** undo it — the reorder is the legacy `post-RA-sched` pass.
This is why every source-level dependency trick that folds away
(`& 0`, `- radioIndex`, comma, ternary, `* 0`) leaves the order unchanged:
the fake dependency is gone before ISel.

## 3. Angle (a)/(b): compare/constant spelling is dead

Changing only the m23 compare expression (same anchor block) gives
byte-identical code for every non-structural spelling:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`> 3600000`) | **8** | 108 | 11,440 |
| `> 3600000LL` | 8 | 108 | 11,440 |
| `> 3600000L` | 8 | 108 | 11,440 |
| `> (long long)3600000` | 8 | 108 | 11,440 |
| `>= 3600001` | 8 | 108 | 11,440 |
| `3600000 < …` (swap) | 8 | 108 | 11,440 |
| `reinterpret_cast<int64_t &>` | 8 | 108 | 11,440 |
| `*(long long *)&…` | 8 | 108 | 11,440 |
| `(unsigned long long)(…) > 3600000` | 9 | 109 | 11,440 |
| `getPlayingTime() > field + 3600000` | 4,471 | 385 | 11,436 |

The register of the m23 load is decided by the anchor's liveness across
the `ldrd`, not by the compare spelling.  Only the `+ 3600000` rewrite
changes the 64-bit sequence, and it loses size.

## 4. Angle (c): store/load order

The store is always scheduled first.  Every shape that makes the store
depend on `radioIndex` (so the copy must precede it) pays an instruction:

| shape | raw | aligned | size | note |
|---|---:|---:|---:|---|
| HEAD | **8** | 108 | 11,440 | |
| `Status *volatile dummy[2]; dummy[radioIndex>>1] = st; …` | 1,170 | 417 | 11,464 | +24 B |
| `struct {Status*p; int i;} volatile pair; pair={st,radioIndex}` (`strd`) | 352 | 119 | 11,440 | `.ARM.extab` 586 (+9) |
| `volatile int radioIndex` | 995 | 141 | 11,456 | |
| `uintptr_t volatile dummy = (uintptr_t)st` | 989 | 141 | 11,456 | |
| `volatile Status *dummy = st` | 932 | 146 | 11,452 | |
| dead store + `st2 = st` (ccm via `st`) | 990 | 143 | 11,456 | tail hoists |
| dead store + `st2 = Globals::status` | 977 | 139 | 11,456 | tail hoists |

The `strd` pair is the only size-exact dependency shape, but its
store+load is 6 B vs the original 4 B `ldr.w`, so raw jumps to 352 and
`.ARM.extab` grows by 9 B — a net loss.

## 5. New family: a `Status *` condition local (`st_cond`)

Instead of the body anchor, pin r9 with a `Status *` local live across
the m122 condition's calls.  This makes the m23 and the body byte-exact
(the condition local replaces the body anchor), but the condition's
receivers become `mov r0,r5` instead of `ldr.w r0,[r9]`:

| shape | raw | aligned | size | note |
|---|---:|---:|---:|---|
| `st_cond` for all four condition receivers | 4,508 | 380 | 11,436 | -4 B |
| `st_cond` for clauses 1,2,5 (getMission fresh) | 4,496 | **79** | 11,436 | -4 B |
| `st_cond` for clauses 1,2 + field read | 4,486 | **61** | 11,436 | -4 B |
| `st_cond` for 1,2,5 + volatile clause-2 load | **49** | 334 | **11,440** | size-exact! |
| `st_cond` declared before the if-chain | 5,038 | 106 | 11,432 | changes m24 |
| `st_cond` in a comma at clause 4 | 5,180 | 598 | 11,396 | no pin |
| `Status **got = &Globals::status` | 5,196 | 597 | 11,392 | rematerialised |

The best new shape is the **volatile-clause-2** variant: raw 49, size
11,440 exact, m23 and body byte-exact.  Its entire residual is the +2 B
`mov r0,r5` at clause 1 (`ldr.w r5,[r9]; mov r0,r5` vs `ldr.w r0,[r9]`),
which shifts the first `blx/cmp/blt` by 2 and is realigned by the
volatile clause-2 load.  That is ~49 raw bytes over the 8-byte shift
window.  It does not beat HEAD's 8.

## 6. Why the body anchor cannot be dropped

Any shape that uses `st` directly at the m122 `getCurrentCampaignMission`
receiver (instead of the volatile read-back) lets the compiler reorder
the tail `getPlayingTime()` before `createRadioMessage`, adding a
`mov r5,r0` + inline tail (+16 B, size 11,452-11,456):

| shape | raw | size |
|---|---:|---:|
| `Status *st` at ccm (`n1`) | 932 | 11,452 |
| dead store, ccm via `st` (`d1`) | 990 | 11,456 |
| `Status **got` at ccm (`g1`) | 981 | 11,456 |

The volatile read-back (`st2 = dummy`) is what keeps the tail shared
with the m24 branch; the compiler cannot prove `dummy == Globals::status`.

## 7. Conclusion

HEAD's **raw 8 / aligned 108 / 11,440 B** remains the local optimum.
The theoretical floor for the current anchor scheme is raw 4 (m23 2 B +
body 2 B) if the post-RA scheduler ever keeps `mov r4,r0` before the
store; the theoretical floor for a byte-exact shape is 0, but no source
spelling found here gets the store out of the body without a size shift.
No change landed; `src/` is clean.

## 8. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13654/`
(`harness.py`, `h2.py`, `h3.py`, `batch*.py`, `dd.py`, `mir_all.txt`).

## 9. Additional sweeps (this session)

The following shapes were measured after the table in §3-§5.  None beats
HEAD's raw 8; they are recorded so the next pass does not repeat them.
`raw`/`aligned`/`size` are from `tools/fn_diff.py`; `.ARM.extab` is the
section byte-diff count against the original.

| shape | raw | aligned | size | `.text` | `.ARM.extab` | note |
|---|---:|---:|---:|---:|---:|---|
| **HEAD** | **8** | **108** | 11,440 | 71,196 | 577 | |
| `store_before_nt_readback` | 15 | 111 | 11,440 | 71,203 | 577 | store before `nextInt`, read back after; setup shifts |
| `ll_unsigned` (`(unsigned long long)(...) > 3600000ULL`) | 9 | 109 | 11,440 | 71,197 | 577 | changes `bge` to `bcs` only |
| `st_cond_pt_vread` | 8 | 108 | 11,440 | 71,196 | 577 | reproduces HEAD |
| `direct_st_tail_pt_st` | 37 | 109 | 11,440 | 71,225 | 577 | ccm via `st`, tail `getPlayingTime` via `st`; tail shared |
| `ccm_first_anchor` | 47 | 120 | 11,440 | 71,235 | 577 | `getCurrentCampaignMission` before `nextInt` |
| `assign_field_vread` | 329 | 132 | 11,440 | 71,517 | 583 | field load `r2` exact; pays `mov r0,r5` before `getPlayingTime` |
| `vafter_direct2` | 362 | **20** | 11,440 | 71,550 | 586 | volatile anchor after the 64-bit compare; m122 condition byte-exact, body exact except a 4-byte `ldr.w r4,[r9]` at `0x17f186` |
| `addr_store` | 348 | 18 | 11,440 | 71,536 | 586 | `Status **addr = &Globals::status`; store before `nextInt` |
| `rnd_store` | 930 | 128 | 11,444 | 71,536 | 1,355 | `Globals::rnd` anchor; m122 condition exact, but `mov r0,r4` + store cost 4 B |
| `ntcond_plain` | 4,043 | 406 | 11,440 | 75,231 | 671 | `nextInt` moved into the m122 condition |
| `m23_tail st_both` | 4,913 | 612 | 11,432 | — | 754 | caches status in the m23 tail; GOT base copied to `r5` |
| `vol_before_nt_direct2` | 5,205 | 600 | 11,396 | — | 1,464 | volatile anchor before `nextInt` without the read-back |

Two useful near-misses:

* `assign_field_vread` proves the m122 field load can be kept in `r2`
  while the body anchor stays in `r5`; the cost is the `mov r0,r5`
  before `getPlayingTime` (the anchor is CSE'd into the call receiver).
* `vafter_direct2` proves the whole m122 condition can be byte-exact and
  the body can use the anchor directly (tail shared with m23).  Its only
  body insertion is the 4-byte anchor load at `0x17f186`, but that shifts
  the `nextInt` setup and yields raw 362 despite aligned 20.

Also re-confirmed: `Status **`/reference/`const`/`auto` spellings of the
anchor and all `3600000` literal spellings (`L`, `LL`, `U`, `ULL`, hex,
casts) are byte-identical to HEAD or one byte worse (`ULL`/unsigned).
