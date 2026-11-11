# LevelScript::process — wave 178 / w12206 (dispatch order, pools, data flow)

Target `_ZN11LevelScript7processEi` (57,232 B, original `0x137d18`). Base master
`1b48f990f` (wave-177 merge). Baseline raw **53,507**, aligned **18,941**, size
**57,232 exact**. Gate at the start and end of this pass: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. No code change landed; `src/` is at the base.

## 1. What the residual actually is

The function is size-exact but almost every byte differs because of a **global
callee-saved register swap** in the prologue:

```
original:  mov sl, r0            ; this  -> sl
           ldr.w fp, [r0]        ; guard -> fp
ours:      mov fp, r0            ; this  -> fp
           ldr.w sl, [r0]        ; guard -> sl
```

Everything downstream inherits the swap: `[sl,#24]` vs `[fp,#24]`, the
exception landing pads (`mov sl,r0` in the original, `mov fp,r0` in ours), and
the choice of scratch registers. The original also reuses `fp` as a general
scratch register later (e.g. `ldr.w fp,[sp,#72]` at `0x138140`), which our build
cannot do because `fp` holds `this`.

A second, independent cause is **literal-pool placement**. The first pool is at
`0x1380a4` in the original and `0x1380c0` in ours (28 B later); the pool used by
the mission-89 camera block is at `0x138e1c` vs `0x138d70` (172 B later). Every
`ldr rX,[pc,#N]` / `vldr` that targets those pools therefore differs, even when
the instruction shape is identical. The longest same-byte run in the function is
93 B at `0x138b8f..0x138beb`; it is bounded on both sides by pool-relative
loads.

## 2. Mission-dispatch order is wrong

The original's pre-switch mission chain is (comparison order):

```
5, 81, 26, 78, 50, 89, 106, 95/99/109/119/126/133/160/161, 51
```

Our source order is:

```
81, 78, 89, 106, 95..., 5, 26, 50, 51
```

The original also runs the shared "active" tail (`0x137f7c`) **before** the
mission-5 chain; our source places the tail after the whole chain. The original
mission-5 block is at `0x13812e`, immediately after the tail; ours is at
`0x137f82` when moved before mission 81 without moving the tail.

A full restructure (tail before the chain, chain reordered to
`5,81,26,78,50,89,106,95,51`) does put mission 5 at the original `0x13812e`,
but the function grows to 57,476 B (+244) because the switch guard is laid out
immediately after mission 5 and the 106/95/51 blocks become cold tail blocks.
Size is sacred, so this was not landed.

## 3. Experiments (all size-exact unless noted)

| change | raw | size | verdict |
|---|---:|---:|---|
| base | **53,507** | 57,232 | — |
| drop `Status *status` local, call `Globals::status->...` twice | 53,508 | 57,232 | +1, rejected |
| `Level *level = m_pLevel` for the three getters | 53,554 | 57,232 | +47 |
| `LevelScript *self = this` for the three getters | 53,507 | 57,232 | neutral |
| `bool fading` -> `int` | 53,507 | 57,232 | neutral |
| `bool docked` -> `int` (both sites) | 53,507 | 57,232 | neutral |
| `for (unsigned int i...)` -> `int i` (one loop) | 53,507 | 57,232 | neutral |
| all 47 `for (unsigned int` -> `for (int` | 53,901 | 57,236 | +4 B |
| `if (!(depth < 1.0f))` -> `if (depth >= 1.0f)` | 54,850 | 57,224 | -8 B, `vmin` instead of `vcmpe` |
| same + `rumble < 0` -> `!(rumble >= 0)` | 54,286 | 57,232 | still worse |
| `m_bStartSequence != 0` -> `m_bStartSequence` | 53,507 | 57,232 | neutral |
| `reinterpret_cast<float&>` -> `*(float*)` | 53,507 | 57,232 | neutral |
| `lvl->getPlayer()->setCollide` -> `player->setCollide` | 53,525 | 57,232 | +18 |
| `lvl->getPlayer()->player->setVulnerable` -> `player->...` | 53,525 | 57,232 | +18 |
| swap `m_bStartSequence` / `m_bStartSequenceOver` stores | 53,507 | 57,232 | neutral |
| `-mllvm -disable-postra-machine-sink` removed | 53,509 | 57,232 | +2 |
| `-mllvm -regalloc-csr-first-time-cost=0/1/2` | 53,507 | 57,232 | neutral |

`vmin` is 6 B shorter than the original `vcmpe/vmrs/it/vmovmi` clamp, so the
`depth >=` cell is not a code-shape match; its raw movement is pure layout.

## 4. Recommendations for the next pass

1. **Attack the `sl`/`fp` swap first.** It is worth far more than any local
   cell. Try source shapes that change the live range of `this` or of the
   stack-guard pointer (e.g. a local `Status *` that is *not* live across the
   whole function, a `Level *level` alias, or a different first-use order).
   A single flip would align the prologue, the mission-1 block and the landing
   pads at once.
2. **Then align the first pool.** The mission-1 block is instruction-for-
   instruction identical except for the pool distance (28 B). If the
   pre-pool code shrinks by 28 B (or the post-pool code grows by 28 B), the
   mission-1 `vldr`/`ldr` offsets match and the block becomes a large same-byte
   run.
3. **Do not move the mission chain without moving the tail.** The original tail
   is at `0x137f7c`, before mission 5. Moving blocks inside the current
   `else if` chain only makes the function larger.
4. **106/95/51 are chain blocks, not switch cases.** The branch chain reaches
   them in the order `106, 95..., 51`; the switch guard (`0x13879e`) is laid out
   between the mission-51 check and the mission-106 body. Any restructure must
   preserve that layout, or the switch guard moves and the size changes.

## 5. w12206 follow-up: instruction-shape and flag sweep (no code win)

State at the end of this pass: base `src/` unchanged, `git status` clean, gate
re-measured green — byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`tools/fn_diff.py` for the target: **raw 53,507, aligned 18,941, size
57,232/57,232, insn 20,732 ours / 20,830 original** (ours is 98 instructions
shorter at the same byte size).

### 5.1 Where the 98 missing instructions are

Instruction-for-instruction comparison (register names ignored) shows the two
bodies are the same code almost everywhere; the deficit is concentrated in a
few loop bodies whose *block placement* differs:

* **Mission-0 "face the three ships" loop** (`0x137f34` original). The
  original reloads `(*enemies)[i]` for the second call
  (`ldr.w r0,[r9,#4]` / `ldr.w r0,[r0,r5,lsl #2]` / `ldr r0,[r0,#8]`, 12
  instructions per iteration). Ours keeps `KIPlayer *e = (*enemies)[i]` in
  `r4` and reuses it (10 instructions). Rewriting the loop without the `e`
  local reproduces the original body exactly, but the register allocation
  shifts and the function grows to **57,392 B (+160)**, raw 55,637, aligned
  19,625 — so the `e` local is load-bearing for the size. `KIPlayer *const e`
  and hoisting `e` outside the loop are neutral; forcing a reload only for the
  second call is also +160.
* **First wingman loop** (`0x137fb0` original). The original keeps the
  `field_0x12d = 1` store inline (`cbz` over one `strb`). Ours places the store
  out of line after the loop (`cbnz` forward + `b` back), costing one extra
  branch. Removing `__builtin_expect(..., 0)` or changing it to `1` keeps the
  size exact but *raises* the raw diff (53,574); `!= 0` is neutral; a
  `?:` assignment is much worse (54,307). The expect must stay.
* The **second wingman loop** body is instruction-identical to the original;
  only its position relative to the first literal pool differs (the original
  puts the pool at `0x1380a4`, before the loop body; ours puts it at
  `0x1380c0`, after it). That is the source of the +28 B first-pool offset
  already noted in section 1.

### 5.2 Prologue / data-flow cells (all size-exact unless noted)

| change | raw | size | verdict |
|---|---:|---:|---|
| base | **53,507** | 57,232 | — |
| getter order messages, player, enemies (base) | 53,507 | 57,232 | best |
| getter order messages, enemies, player | 53,519 | 57,232 | +12 |
| getter order player, messages, enemies | 53,519 | 57,232 | +12 |
| getter order enemies, messages, player | 53,521 | 57,232 | +14 |
| `Level *lvl = m_pLevel` for the three getters | 53,554 | 57,232 | +47 |
| `(void)status;` after the `Status *status` local | 53,507 | 57,232 | neutral |
| `Status *status` -> reference | 53,507 | 57,232 | neutral |
| `KIPlayer *const e` in mission-0 loop | 53,507 | 57,232 | neutral |
| `KIPlayer *e` hoisted outside the loop | 53,507 | 57,232 | neutral |
| `if (depth < 1.0f) {} else depth = 1.0f;` | 53,507 | 57,232 | neutral |
| `enemyZ * -0.0002f` instead of `/ -5000.0f` | 53,508 | 57,232 | +1 |
| `(*enemies)[0]->getPosition().z` inline | 53,507 | 57,232 | neutral |
| `((RadioMessage *) ((*messages)[2]))->isOver()` inline | 53,507 | 57,232 | neutral |
| `angle = ... * (1.52587890625e-05f * 6.2831855f)` | 54,832 | 57,224 | -8 B, rejected |
| `if (depth >= 1.0f)` | 54,850 | 57,224 | -8 B, rejected |

The two `-8 B` cells are the already-known `vmin`/`vcmpe` shape change; they
are not size-exact.

### 5.3 Block reorders (all grow the function)

| reorder | size | raw |
|---|---:|---:|
| mission-5 block moved before mission 81 (`0,1,5,81,...,tail,26,50,51`) | 57,400 | 55,665 |
| whole "active" tail moved before mission 81 (`0,1,tail,5,26,50,51,81,...`) | 57,488 | 55,837 |

This confirms section 4.3: the current source order is a size local optimum;
the original's `tail,5,81,26,78,50,89,106,95,51` layout cannot be reproduced
without the +244 B growth the earlier pass measured.

### 5.4 Per-TU clang flag sweep (properly applied)

The section-3 flag cells were re-run with the flag actually reaching the
compiler. **Method note:** `cmake --build` re-runs the compile because ninja
records the output mtime in `.ninja_log`; a `.o` produced outside ninja is
treated as dirty. The working recipe is: compile `LevelScript.cpp` manually
with the extra `-mllvm` flags to its `.o`, then run the last
`ninja -t commands link_so` line (gold link + strip + `reorder_shstrtab.py`)
directly. `-mllvm -regalloc=basic` under this recipe gives size 59,516 /
raw 57,833, proving the flags apply.

Size-exact and raw-neutral: `-enable-post-misched=false`,
`-misched-postra=false`, `-enable-earlycse-memssa=false`,
`-enable-shrink-wrap=false`, `-disable-cgp-branch-opts`,
`-regalloc-csr-first-time-cost=0/1/2/5/10`, `-align-all-nofallthru-blocks=0`,
`-disable-early-ifcvt` (and `=false`), `-enable-if-conversion`,
`-min-jump-table-entries=4`, `-max-jump-table-size=0/100000`,
`-pre-RA-sched=list-hybrid`, `-unroll-threshold=0`,
`-vectorize-loops=false`, `-vectorize-slp=false`, `-inline-threshold=0`,
`-enable-post-misched=false/true`, `-post-RA-scheduler`,
`-enable-aa-sched-mi=false`, `-enable-misched=false`,
`-split-spill-mode=speed`, `-condsider-local-interval-cost=false`,
`-break-anti-dependencies=none`, `-disable-early-taildup=false`.

Size-exact but worse raw: `-disable-post-ra` / `-post-RA-scheduler=false`
(54,904). Size-changing: `-disable-postra-machine-licm` (+4, 54,237),
`-likely-branch-weight=7 -unlikely-branch-weight=1` (+28, 54,729), `=1/1`
(+52, 54,897), `-tail-dup-size=0` (+20, 54,938), `-enable-gvn-hoist` (-32,
55,208), `-disable-block-placement` (-60, 55,562), `-tail-merge-threshold=0`
(58,080), `-arm-promote-constant` (57,380), `-min-jump-table-entries=1/2`
(57,620/57,516), `-pre-RA-sched=source` (57,276), `-disable-machine-sink`
(57,256), `-inline-threshold=100000` (61,148),
`-split-spill-mode=default/size` (57,392/57,408),
`-enable-deferred-spilling` (57,108, raw 55,440),
`sink-insts-to-avoid-spills` (57,320), `-condsider-local-interval-cost`
(57,264), `-break-anti-dependencies=all` (57,236), `-optimize-regalloc=false`
(126,164), `-disable-machine-licm` (57,540), `-disable-machine-cse` (58,064),
`-disable-machine-dce` (57,352), `-enable-pre=false` (57,256),
`-enable-load-pre=false` (57,204), `-disable-early-taildup` (57,252).

No cell is both size-exact and below 53,507. The residual is therefore not a
flag knob; it is the whole-function register assignment (section 1) plus the
literal-pool placement (section 1), both of which follow the block layout.

### 5.5 What to try next

The only untried lever with real upside is the **switch-body block order**.
`fn_diff` reports 18,941 aligned mismatches even after ignoring registers, and
the switch is ~0x13879e-0x144e36 (the bulk of the function). The linear compare
chain at `0x1387e2` is `14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,114,125,154,157,158`
with range prongs `131..145` and `64..73`; our source switch already lists the
cases in that order, but the *bodies* are laid out by `MachineBlockPlacement`
from branch probabilities, not by source order. A pass that reproduces the
original body order (and the original's `b.n` fall-throughs) would move many
`b.w` offsets and literal pools at once. It needs a full-function block map
from Ghidra, not a local cell.
