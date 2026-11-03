# MGame::OnUpdate — `Status*` r5/r9 cache residual (wave 97, w4717)

Status: research pass, 2026-11-03. Worktree `wt/w4717`, branch `wave97-w4717`,
based on master `9b58a39d5`. No source change landed: the function is already at
its best measured shape and every alternative tried regressed the byte count.

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8. Baseline residual is one
41-byte run at `+0x17bb..+0x17e3` (absolute 0x17e093..0x17e0bb), the mission-23
idle-contact condition (`src/game/menu/MGame.cpp:2770-2776`):

```
original                                      ours (current source)
17e08c ldr.w r9,[r0]   ; r9 = &Globals::status
17e090 ldr.w r0,[r9]                         17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission         17e094 mov   r0,r5
17e098 cmp   r0,#23                          17e096 blx   getCurrentCampaignMission
17e09a bne   17e176                          17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                         17e09c bne   17e176
17e0a0 blx   isMining                        17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                           17e0a2 blx   isMining
17e0a6 bne   17e176                          17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]                         17e0a8 bne   17e176
17e0ac blx   getMission                      17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                         17e0ae blx   getMission
17e0b4 cmp   r0,#0                           17e0b2 blx   isEmpty
17e0b6 beq   17e176                          17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]                         17e0b8 beq   17e176
17e0bc blx   getPlayingTime                   17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]                         17e0bc blx   getPlayingTime
                                             17e0c0 ldr.w r2,[r9]
```

The original has **four fresh `ldr [r9]` loads and no cached `Status*`**; the
current reconstruction carries `Status *st = Globals::status;` used at
`getPlayingTime()`. Size is neutral (4+4 = 4+2+2 bytes), so the run is a pure
byte permutation, not a size error.

## 1. Why the `st` anchor exists

Dropping `st` (all four accesses via `Globals::status->`) gives 11392 B /
5148 diff / 212 runs: `&Globals::status` then lands in **r5** instead of r9,
every `ldr.w rX,[r9]` in the whole idle block becomes a 2-byte `ldr rX,[r5]`,
and the function re-colours (48 B smaller). The `st` local is what pins the GOT
base to r9; it is a *size anchor*, not a source witness.

## 2. New evidence: a faithful `-Oz` machine-IR oracle

Earlier waves traced register allocation with `llc -O2` on the emitted `.ll`.
That is **not** faithful to this build: `llc -O2` puts the GOT base in r9 even
for the no-`st` source, while the real clang `-Oz` object puts it in r5. The
backend pipelines differ (stack frame 288 vs 296, different spill set).

Two faithful recipes (both reproduce the real object byte-for-byte in the
mission block):

```bash
# 1. fast codegen replay (NDK clang, ~0.8 s)
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb \
  -mfpu=neon -mfloat-abi=softfp -fPIC -Oz -S -o X.s X.ll

# 2. machine-IR dump after each backend pass (~45 s, 4.2 M lines)
clang++ <same flags> -mllvm -print-after-all -c X.ll -o /dev/null > pa_all.txt
```

`-mllvm -debug-only=regalloc` is **not** available in the NDK clang (release
build); `-print-after-all` is. The final `NoVRegs` machine dump for
`_ZN5MGame8OnUpdateEv` shows the mission-23 block (`bb.382`, `%ir-block.1271`):

```
$r0 = tLDRpci %const.220
$r0 = tPICADD killed $r0, 40
renamable $r5 = tLDRi killed renamable $r0, 0   ; r5 = &Globals::status
renamable $r0 = tLDRi renamable $r5, 0
tBL getCurrentCampaignMission
tCMPi8 $r0, 23
tBcc %bb.392, 1
```

The block has exactly one live-in GPR (`$r11` = `this`; `$r10` = `&this->player`
is the `sl` value); **r5 is genuinely free at 0x17e08c in the original too**
(the last r5 write before the block is `strb r0,[r5]` at 0x17e044, the next is
`ldr r5,[r0]` at 0x17e12a). So the original's r9 choice is *not* caused by a
source-level value occupying r5 — it is the greedy allocator's split/rematerial-
isation heuristic choosing "keep the GOT base whole in r9 (CostPerUse 1)" over
"rematerialise it into r5 (CostPerUse 0) at every branch".

With `st` present, `st` is processed first (weight 1.64e-06 vs the GOT base's
1.27e-07) and takes r5, so the GOT base must take r9 and stays unsplit — which
is exactly the original's r9 colour, at the cost of the two `mov r0,r5`.

## 3. IR detail: the ccm load is already separate

The `-Oz` IR for the current source is:

```
%1091 = load @_ZN7Globals6statusE            ; ccm load (block 1090)
%1092 = call getCurrentCampaignMission(%1091)
...
%1098 = load @_ZN7Globals6statusE            ; st load (block 1097)
call llvm.dbg.value(%1098, !"st")
%1099 = call getMission(%1098)
%1100 = call isEmpty(%1099)
%1102 = call getPlayingTime(%1098)
%1103 = load @_ZN7Globals6statusE            ; field load
```

`st` is already attached to the *getMission* load, not the ccm load; machine CSE
then merges `%1091` and `%1098` (identical loads of the same global, `isMining`
is known not to write it) into one value in r5, which is why the ccm emits
`mov r0,r5`. The no-`st` IR keeps four separate loads (`%1091`, `%1098`,
`%1102`, `%1104`) — i.e. **the original's IR is the no-`st` shape**.

## 4. Variants measured this pass

All measured with the official verifier (`link_so` + `tools/verify` byte diff).

| variant | size | diff | runs |
|---|---:|---:|---:|
| baseline `st` at getPlayingTime | **11440** | **41** | **1** |
| no `st` (reference) | 11392 | 5148 | 212 |
| `st` assigned after ccm via comma `(st = Globals::status, ...)` | 11396 | 5129 | 240 |
| same, field via `st` | 11392 | 5145 | 214 |
| `Status *&st = Globals::status` at pt | 11392 | 5148 | 212 |
| `Status **st = &Globals::status` at pt (`(*st)->`) | 11392 | 5148 | 212 |
| `st` at pt + `(void) st;` after the if | 11440 | 41 | 1 |
| `auto *st` at pt | 11440 | 41 | 1 |
| `(*st).getPlayingTime()` | 11440 | 41 | 1 |
| `st` at pt + `Status *&r = Globals::status` for ccm | 11440 | 41 | 1 |
| `Status *volatile st` | 11340 | 6076 | 317 |
| `Status *st = *(Status *volatile *)&Globals::status` | 11440 | 41 | 1 |
| volatile ccm read `(*(Status *volatile *)&Globals::status)->` | 11440 | 853 | 36 |
| volatile ccm + `Status **st` at pt | 11392 | 5148 | 212 |
| volatile ccm + no `st` | 11392 | 5148 | 212 |
| volatile ccm + `st` at pt + field via `st` | 11436 | 4990 | 303 |
| `Status *const st` | 11440 | 41 | 1 |
| `register Status *st` | 11440 | 41 | 1 |
| cast ccm `((Status *)Globals::status)->` | 11440 | 41 | 1 |
| `st` at pt + touch `hints[0]` before the if | 11440 | 41 | 1 |
| `st` at pt + unused `PlayerEgo *pl` local | 11440 | 41 | 1 |
| `st` at pt + unused `int dummy` local | 11440 | 41 | 1 |
| self-store `Globals::status = st;` after decl | 11440 | 41 | 1 |
| volatile self-store after decl | 11444 | 5000 | 314 |
| two locals `st`, `st2` (mission via `st2`) | 11436 | 4982 | 303 |
| unused `st` (declared, never used) | 11392 | 5148 | 212 |
| `if (st == Globals::status && ...)` folded check | 11392 | 5148 | 212 |
| nested `if (ccm == 23) { Status *st = ...; if (...) }` | 11400 | 4990 | 240 |
| `st` at m24 `getPlayingTime` (re-measured) | 11440 | 270 | 9 |

The volatile-ccm variant (853 diff) is the closest non-baseline shape: it keeps
the GOT base in r9 and emits the four fresh loads, but adds `ldr r5,[r9]` and
`mov r0,r5` for `st`, and the extra 2 bytes shift every later branch offset
(36 runs). Its mission block is:

```
17e090 ldr.w r5,[r9]   ; st
17e094 ldr.w r0,[r9]   ; volatile ccm
17e098 blx   getCurrentCampaignMission
...
17e0bc mov   r0,r5     ; getPlayingTime
17e0c2 ldr.w r2,[r9]
```

## 5. Conclusion

Proved this pass:

1. The original has **no cached `Status*`** in the idle-contact block: all four
   `Globals::status` dereferences are fresh `ldr.w rX,[r9]` loads, and there is
   no `mov r0,rN` before any `Status::` call in the whole function.
2. The original's IR is therefore the **no-`st` shape** (`%1091/%1098/%1102/
   %1104` distinct loads).
3. `r5` is free at 0x17e08c in the original as well, so the r9 colour is a
   whole-function greedy-RA split decision, not a source-level r5 occupant.
4. The `st` anchor is the only measured source shape that reproduces the r9
   colour; every shape that reproduces the four fresh loads re-colours the GOT
   base to r5 and loses 48 B / 200+ runs.
5. The faithful oracle is clang `-Oz` on the emitted `.ll`, **not** `llc -O2`
   (the two backend pipelines disagree on this function).

Remaining: a source shape (or a code-neutral IR difference) that makes the
greedy allocator keep the GOT base whole in r9 while leaving the four loads
uncached. Guessing one is not honest C++; it needs a source witness.

## 6. Gate (unchanged, note-only pass)

`check` green: byte_exact **4500/4524**, linked_exact **4500/4524**,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.
Before -> after: byte 4500 -> 4500, linked 4500 -> 4500.

Scratch drivers live in `/dev/shm/w4717` (`h.py`, `fdiff.py`, `mkll.sh`,
`batch6.py`..`batch9.py`, `pa_all.txt`, `u_*.txt`).
