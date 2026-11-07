# MGame::OnTouchEnd — wave 134 (w7758): campaign/hudMenu restructure vs BranchFolder survivor

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7758`,
base `ec0acea1e`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w7758`. **No source change landed**: the one shape that flips the
`initHudMenu` survivor (the 0x200/0x400 source swap) keeps 8080 B and `.rel.plt`
exact but reorders the dispatch tests and scores 81.9 (raw 81.85) vs the base
82.0 (raw 81.96). Every other shape either folds back to the base bytes or
re-lays-out the function.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

Baseline (verified this wave, full `check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
.text 85,143   .ARM.extab 1,416   .ARM.exidx 0   .rel.plt 0
.rodata/.data/.bss 0   size 8080/8080   raw fuzzy 81.9607 %
```

## 1. Ground truth (objdump, original)

The original cluster is

```
17ad4c  ldr.w r0,[fp,#248]; tst #512; bne 17adc0     ; 0x200 test first
17ad56  lsls #20; bmi 17ae8c                          ; 0x800
17ad5c  lsls #21; bmi 17afd8                          ; 0x400
...
17adc0  ldrd r0,r2,[fp,#116]; movs r1,#1; b 17afde    ; 0x200 body, forward branch
...
17afd8  ldrd r0,r2,[fp,#116]; movs r1,#2              ; 0x400 body
17afde  blx Hud::initHudMenu                          ; shared tail (0x400 falls through)
17afe2  ldr r0,[fp,#248]; cbz 17b01a                  ; join test: flags == 0
```

So the original has the **dispatch order 0x200, 0x800, 0x400** *and* the
**0x400 body as the shared-tail survivor** (it falls through into `blx`).

## 2. Why the base build picks the 0x200 body

`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5MGame10OnTouchEndEiiPv`
on the real NDK clang 7.0.2 gives the PEI and CFO MIR.  In the PEI dump the join
test is `bb.463`; its block-list predecessor is `bb.462` (the wingman-body tail),
which is **not** in `SameTails`.  `CreateCommonTailOnlyBlock` therefore falls to
the `EstimateRuntime` tie-break: both prefixes are `ldrd` (2) + `movs r1,#N` (1)
= 3, and the loop keeps the **last** (`SameTails[1]`, the lower-numbered body).
The bodies are `bb.426` (0x200) and `bb.430` (0x400), so `bb.426` wins.  The CFO
dump confirms it: `bb.404` (0x200 body) falls through into the new shared tail
`bb.405`; `bb.435` (0x400 body) branches to it.

The original must therefore have had the **0x400 body lower-numbered** (or as
`PredBB`) at BranchFolder time.  The 0x200-first dispatch order and the
0x400-survivor ownership are independent facts in the original.

## 3. Variants measured this wave (all reverted)

| variant | pct (raw) | size | `.text` | `.rel.plt` | notes |
|---|---:|---:|---:|---:|---|
| base | **82.0** (81.9607) | 8080 | 85,143 | 0 | 0x200 body owns `blx` |
| **swap 0x200/0x400 cases** | 81.9 (81.8526) | 8080 | 85,146 | 0 | **0x400 body owns `blx`**, dispatch 0x400/0x800/0x200 |
| swap + `missionWon` temp | 81.9 (81.8526) | 8080 | 85,146 | 0 | temp CSE'd, identical to swap |
| swap + 3rd `getCampaignMission` call | 80.8 | 8084 | 209,794 | 0 | +4 B call, re-layout |
| `flags == 0` moved after 0x400 case | 76.1 | 8084 | 209,823 | 0 | extra `cmp #0`; 0x400 body goes to final tail, not join test; merge moves to MBP's internal BranchFolder and still picks 0x200 |
| `goto hbMenu1` (0x200 body after chain, before weaponSlot test) | 82.0 | 8080 | 85,143 | 0 | **byte-identical to base** (clang folds the goto) |
| `goto hbMenu2` (0x400 body after chain, before weaponSlot test) | 82.0 | 8080 | 85,143 | 0 | **byte-identical to base** |
| `goto hbMenu1` after whole if/else (skip join test) | 78.6 | 8072 | 162,743 | 0 | no merge (4 calls), 0x200 body goes to final tail |
| separate `if (flags == 0)` | 79.1 | 8076 | 104,792 | 0 | original `ldr; cbz` polarity but re-layout |
| swap + separate `if` | 79.0 | 8076 | 104,795 | 0 | re-layout |
| campaign `if (!won) { reset } else { ... }` | 82.0 | 8080 | 85,143 | 0 | **byte-identical to base** |
| `hudMenuOpen = 1;` store in 0x200 body (EstimateRuntime lever) | 80.8 | 8084 | 209,769 | 0 | +2 B store flips the tie-break but re-lays-out |

## 4. The swap is the only shape that achieves the hypothesis

The swap (`if (flags & 0x400) ... else if (flags & 0x800) ... else if (flags & 0x200)`)
puts the 0x400 body first in the IR, so it is the lower-numbered body and
`CreateCommonTailOnlyBlock` picks it.  The emitted code is exactly the original's
ownership:

```
17adc4  ldrd r0,r2,[fp,#116]; movs r1,#2; blx initHudMenu; b test   ; 0x400 owns blx
17afaa  ldrd r0,r2,[fp,#116]; movs r1,#1; b 0x17adc4               ; 0x200 branches forward
```

but the dispatch tests become `tst #1024` (0x400), `lsls #20` (0x800),
`lsls #22` (0x200) instead of `tst #512` (0x200), `lsls #20`, `lsls #21`.
That test-order break costs ~0.11 raw points, so the swap is a near-tie, not a win.

## 5. Remaining attack

1. Find a source shape where the **0x200 test is emitted first** *and* the 0x400
   body is lower-numbered at BranchFolder time.  Every `goto`-based attempt
   (`hbMenu1`/`hbMenu2` before the weaponSlot test, or after the whole if/else)
   is folded back by clang's block ordering: the IR block order is identical to
   the base, so the tie-break still picks 0x200.
2. The `EstimateRuntime` lever works (a longer 0x200 prefix flips the tie-break
   to 0x400) but the extra store costs 2 B and re-lays-out the function.  A
   size-neutral way to make the 0x200 prefix longer than the 0x400 prefix (e.g.
   a different argument evaluation order that keeps the same instruction count)
   is the most promising untried lever.
3. The `flags == 0` join test's `PredBB` is the wingman tail.  If the wingman
   body could be moved out of the block-list slot immediately before the join
   test (without changing the CFG), `PredBB` would be a different block.  The
   `goto hbMenu*` shapes change the merge point instead of `PredBB`.

## 6. Reproduction

```
cp /dev/shm/w7758/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7758/fastscore.py        # 82.0 / 8080 / 2775:2774
python3 /dev/shm/w7758/secdiff.py          # .text 85,143 / .rel.plt 0
```

MIR dumps (real NDK clang, filtered to OnTouchEnd):

```
/dev/shm/w7758/pa.sh                       # -mllvm -print-after-all
/dev/shm/w7758/cfo.txt                     # base CFO dump (3 initHudMenu)
/dev/shm/w7758/mbp.txt                     # base MBP dump
```

The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; the
original 0x200/0x400 bodies are at `0x17adc0` (`ldrd; movs r1,#1; b 0x17afde`)
and `0x17afd8` (`ldrd; movs r1,#2; fallthrough 0x17afde`).
