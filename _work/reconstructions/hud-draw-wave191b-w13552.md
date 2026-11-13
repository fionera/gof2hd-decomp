# Hud::draw / MGame::OnUpdate — wave 191b (w13552): MIR frame analysis, no win landed

Worker `w13552`, branch `wave191b-w13552`, base master `ed90c5386`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13552`.

**No code change landed.** Tree left clean at HEAD shape.

| target | symbol | size | raw/aligned | insn |
|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9,332 | 19 / 6 | 3226/3226 |
| `MGame::OnUpdate` | `_ZN5MGame8OnUpdateEv` | 11,440 | 10 / 112 | 4003/4002 |

Gate after the session: `check` **RATCHET PASS** — byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## 1. Hud::draw — what the anchor actually does (MIR evidence)

New in this wave: a full `-mllvm -print-after-all` dump of `Hud.cpp`
(`scratch/w13552/mir_base.txt`, 224 MB) was read for the final machine
code of `Hud::draw`.  The relevant block is:

```
t2TSTri  $r1, 36
$r0 = t2MOVi 300, 14 (AL)                    ; mov r0,#300  (unpredicated)
t2IT     1, 24                               ; it ne
t2STRi12 $r8, $sp, 208, 1, ...               ; strne r8,[sp,#208]  <- anchor
t2STRi12 $r0, $r10, 1208, 14, ...            ; str r0,[sl,#1208]   (unpredicated)
```

The anchor's `mov r0,#300` is hoisted out of the IT block *because the
source's `field_0x4b8 = 300` store is unconditional* (the unbraced
`if (hitFlags & 0x24) Status **volatile p = ...;` only guards the
declaration).  That is a real behaviour bug in the shipped reconstruction
(the original stores conditionally), but it is what buys the 28-byte
block.  Braces around the anchor (`if (...) { ... } this->field_0x4b8 = 300;`)
give byte-identical output (raw 19) — the block structure is unchanged.

### The stack frame is the real missing piece

From the same MIR dump + objdump:

| build | frame | canary slot | `[sp,#...]` offsets used |
|---|---:|---:|---|
| original | `0xe0` | `0xdc` | `0,4,8,12,16,20,24,28,32,40,60..120,160,220` |
| HEAD (anchor) | `0xe0` | `0xdc` | same **plus `208`** (the anchor) |
| no anchor | `0xd8` | `0xd4` | `0,4,8,12,16,20,24,28,32,40,56,60..112,152,212` |

The original's frame is `0xe0` and its `0xd0..0xdb` region is **never
accessed** — i.e. the original has a 4-byte live-but-unaccessed stack
object at `[sp,#0xd0]` (plus alignment padding) that the anchor
reproduces.  Removing the anchor does not just drop 4 bytes: it changes
the whole-function allocation (frame `0xd8`, every spill offset shifts,
raw 7,340).  So the residual needs *both* (a) the shipped conditional
damage block and (b) an 8-byte-of-frame dead alloca that emits no code.

### Measured this wave (all 9,332 B / 3,226 insn unless noted)

| variant | raw |
|---|---:|
| HEAD (`Status **volatile p` unbraced, store unconditional) | **19** |
| braces around anchor, store outside | 19 |
| anchor after the if, then `field = 300`, then empty `if` | 19 |
| `long long`/`double`/`char[8]`/`Vector` dead local at block or at function end | 7,340 (9,328 B) |
| `volatile long long pad; (void)pad;` at function end | 7,020 (9,332 B) |
| `volatile int pad; (void)pad;` at block | 3,099 (9,332 B) |
| `volatile long long pad; (void)pad;` at block | 2,953 (9,332 B) |
| `volatile char pad = 0;` anchor in the if | 3,043 (9,332 B) |
| `volatile int pad = 300; this->field_0x4b8 = pad;` in the if | 2,914 (9,332 B) |
| `volatile int pad = 300; ... = 300; (void)pad;` | 2,918 (9,332 B) |
| `*(volatile int *)&this->field_0x4b8 = 300;` (volatile field store) | 7,340 (9,328 B) |
| volatile loads of `field_0x4ac/4b0/4b4/4b8`, volatile `hitFlags` | 7,340 / 7,367 (9,328 B) |
| unconditional anchor before/after the block | 7,135 / 7,125 (9,336 B) |
| `if (...) { anchor } if (...) field = 300;` (two ifs) | 7,092 (9,336 B) |
| `if (...) { anchor; field = 300; }` (braced, both conditional) | 7,092 (9,336 B) |
| `if (...) { anchor } else { anchor }` + store | 7,136 (9,328 B) |
| two volatile anchors in the if | 7,092 (9,336 B) |

A volatile store to a **stack** object is load-bearing for the
allocation; a volatile store/load of a *member* (`field_0x4b8`) is not
(it reproduces the no-anchor allocation).  `-mllvm -print-machineinstrs=1`
segfaults clang 7.0.2 on this TU; `-print-after-all` works (redirect to
disk, ~224 MB, one compile).

Dead end: no source spelling found that keeps the `0xe0` frame + the
HEAD allocation while emitting the shipped `itt ne / movne / strne`
block.  The anchor store (4 B) exactly pays for the second `movne` (4 B),
so any solution must make the anchor free (a dead alloca), and clang 7
`-Oz` removes every dead alloca tried.

## 2. MGame::OnUpdate — two independent regalloc sites (raw 10)

`fndiff` (same-address byte runs) gives exactly two sites:

**Site 1** `0x17e2b9..0x17e2c4` (m23 idle-timer 64-bit compare):

```
orig  17e2b6 ldr.w r2,[r9]          ; Globals::status -> r2
      17e2ba ldr   r4,[pc,#316]     ; 3600000 -> r4
      17e2bc ldrd  r2,r3,[r2,#256]
ours  17e2b6 ldr.w r4,[r9]          ; Globals::status -> r4
      17e2ba ldr   r5,[pc,#316]     ; 3600000 -> r5
      17e2bc ldrd  r2,r3,[r4,#256]
```

Pure `RegAllocGreedy` tie (status pointer r2 vs r4, constant r4 vs r5);
the rest of the block is identical.  `3600000LL`, `>= 3600001`,
`3600000 < ...` and an unsigned compare all leave it unchanged.

**Site 2** `0x17f190..0x17f1aa` (m122–124 radio block):

```
orig  mov r4,r0 ; ldr.w r0,[r9] ; blx getCurrentCampaignMission
ours  mov r5,r0 ; mov r0,r4 ; str r4,[sp,#120] ; blx ...
```

The original reloads `Globals::status` after `nextInt` and keeps
`radioIndex` in `r4`; ours keeps `st` in `r4` across `nextInt` and
spills it (the `Status *volatile dummy = st;` anchor).  Both spellings
are 6 bytes, so size is exact either way.

Measured this wave (all 11,440 B unless noted):

| variant | raw | aligned | insn |
|---|---:|---:|---:|
| HEAD (`st` + `volatile dummy = st` after nextInt, `st->...`) | **10** | 112 | 4003 |
| `st` + dummy, mission check via `Globals::status` (m1) | 348 | 18 | 4002 |
| dummy **before** nextInt, mission check via `Globals::status` (n1) | 361 | 21 | 4002 |
| dummy before nextInt, mission check via `st` (n2) | 15 | 112 | 4003 |
| dummy after `radioBase` (m3) | 915 | 110 | 4001 (11,436 B) |
| dummy after `createRadioMessage` | 898 | 127 | 4003 (11,444 B) |
| no `st` at all, `Globals::status` direct (m2) | 5,196 | 597 | 3974 (11,392 B) |
| `volatile Status *dummy` | 5,196 | 597 | 3974 (11,392 B) |
| `Status **volatile dummy = &st` | 945 | 133 | 4004 (11,444 B) |
| `(void)*(Status *volatile *)&st` | 938 | 131 | 4004 (11,444 B) |
| two-statement `dummy; dummy = st;` | 348 | 18 | 4002 |

m1/n1 reach the *shipped* site-2 instruction pair (`mov r4,r0 ;
ldr.w r0,[r9]`) but the dummy's 2-byte `str` moves the epilogue and
shifts ~180 bytes of code (raw 348/361).  Removing the dummy drops the
function to 11,392 B (m2), i.e. the anchor is also load-bearing for the
whole-function allocation.  Dead end: no placement found that both
reproduces site 2 and keeps 11,440 B.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj _ZN5MGame8OnUpdateEv
# raw=19 / raw=10
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch harness: `scratch/w13552/` — `Hud.cpp.base`, `MGame.cpp.base`,
`fndiff.py` (same-address byte-run diff with disasm context), `swA..G.py`
(Hud sweeps), `swM*.py` (MGame sweeps), `mir_base.txt` / `mir_paa.txt`
(`-print-after-all` dumps).

## 4. Suggested next angles

* Hud: find a construct that keeps a 4-byte alloca alive with zero code
  (the original's `[sp,#0xd0]` is a pure hole).  A `-g` `dbg.declare`-only
  alloca was not reachable from source at `-Oz`.
* MGame: the dummy's 2-byte `str` is the only thing preventing the m122
  block from matching; a source form that forces the `st` spill without
  emitting a store (e.g. a volatile read of an already-stored slot) has
  not been found.
