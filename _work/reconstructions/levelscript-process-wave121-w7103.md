# LevelScript::process — wave 121 / w7103 (block-order probe)

Status: **no source win**. Branch `wave121-w7103`, base master `bd061a489`.
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions.

## Baseline

```
raw ratio (difflib)      59.959208039
orig / ours insns        20169 / 20035
orig / ours size         57232 / 57232        (size-exact)
byte_exact               4505
linked_exact             4505
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
RATCHET                  PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`.
Helper `/dev/shm/w7103/regions2.py` (same definition as the wave-119 note, plus
orig/ours addresses per opcode).

## Main finding: the residual is block *ordering*, not only register allocation

The wave-119 note says the 133-instruction deficit is "all from register allocation,
not missing code". That is incomplete. The difflib opcode map with addresses shows
several large **block reorderings** that move whole case bodies hundreds of
instructions relative to the original:

| region | original | ours | opcode |
|---|---|---|---|
| case 125 state 0 tail | `0x139ff6` `strd; b loc_3800` | `0x139d8a` `strd` then fallthrough | `insert orig[3096:3096] ours[2877:3191] n=0/314` |
| case 154 state 10 | `0x14354c` | `0x142f78` (~666 insns earlier) | `replace orig[16663:16943] ours[16573:16578] n=280/5` |
| case 131 state 2 | `0x13c8fc` | `0x13c3cc` | `replace orig[6808:6811] ours[6272:6600] n=3/328` |
| case 125 state 2 | `0x13b564` | `0x13b374` | `replace orig[5106:5337] ours[4867:4871] n=231/4` |

Two concrete structural differences behind those:

1. **Case 125** (`mission == 125`, source line 3892). The original is a chain of
   separate `if (m_nState == N)` checks whose bodies are laid out far apart:
   state-0 body at `0x139f9c` ends with `strd; b.w 0x13b518` (the state-2 check),
   the state-1 check is at `0x13b43e`, the state-2 check at `0x13b518`. Ours
   places the state-1 check immediately after the state-0 body (`0x139d8e`), so
   the 314-instruction state-body block sits *before* the dispatch instead of
   after it. Converting the source's `if (m_nState == 1)` to `else if` (the
   faithful `if`-chain shape) reproduces the layout difference but scores worse
   (see table).
2. **Mission-0 state switch.** The original emits `subs r0,r1,#6; cmp r0,#9;
   tbh` at `0x139b16` (switch over states 6..15) and keeps the state-4 `if`
   separate. Ours emits `subs r0,r1,#4; cmp r0,#11; tbh` at `0x13902e`
   (states 4..15): clang merged the `else if (m_nState == 4)` arm into the
   switch. Breaking the chain before state 4 or state 5 changes the merge but
   drops the ratio to ~51%.

## Content mismatches found (real, but fixing them alone hurts the global score)

Case 154 (`mission == 154`, source line 4448):

* **state 7** (`0x14349c`): original is
  `if ((*enemies)[0]->isDocked()) { m_nScriptTimerA = 0; m_nState++; }`
  (`strd r1,r1,[this+144]; adds r0,#1; str r0,[this+28]`). Source has
  `{ m_pLevel->objectivesB = nullptr; m_nState = 8; }`.
* **state 10** (`0x14354c`): original first `if` is
  `m_nScriptTime = 0; m_nTimeLimit = 91000; objectivesB = new Objective(3,91000,m_pLevel);`
  (`strd r0,r0,[this+8]`, **no delete**). The second `if` is
  `m_nState = 11; delete objectivesB; objectivesB = nullptr;` (**no**
  `m_nScriptTimerA = 0`; `str r0,[this+28]` stores 11, not 12). Source has the
  `delete` in the first `if`, `m_nScriptTimerA = 0` in the second, and
  `m_nState = 11; m_nState++;`.

Case 78 (`mission == 0x4e`, source line 1043): the original `tbh` at `0x138384`
has 12 entries (`0x138388`..`0x13839e`, states 1..12), matching the source's
12-arm switch; the table offsets differ from ours because the bodies are
placed differently.

## Variants tried (all reverted; official ratio / insns)

| variant | ratio | insns |
|---|---:|---:|
| baseline | **59.959208** | 20035 |
| prologue `messages,player,enemies` | 58.753236 | 19999 |
| prologue separate decl + call `m,p,e` | 58.753236 | 19999 |
| prologue call `m,e,p` | 58.736837 | 20000 |
| prologue call `e,m,p` | 58.731858 | 20000 |
| prologue call `p,m,e` | 58.736837 | 20000 |
| prologue call `p,e,m` | 59.957717 | 20036 |
| case 125 `if (state==1)` -> `else if` | 57.729138 | 20036 |
| case 125 break chain before state 5 | 51.649636 | 20052 |
| case 125 break chain before state 4 | 51.002637 | 20025 |
| case 154 state-7 fix | 59.199005 | 20031 |
| case 154 state-10 fix | 59.210624 | 20040 |
| case 154 state-7 + state-10 fix | 58.544957 | 20036 |
| case 154 state-10 add `m_nScriptTime=0` only | 59.956225 | 20037 |
| case 154 state-10 drop `m_nState++` | 59.959208 (tie) | 20035 |
| case 154 state-7 `m_nState=8` -> `++` | 59.959208 (tie) | 20035 |
| case 154 state-10 drop `m_nScriptTimerA=0` | 59.259259 | 20034 |
| mission-0 swap state-4/state-5 blocks | 56.899896 | 20049 |
| mission-0 break chain before state 4 | 51.002637 | 20025 |
| mission-0 break chain before state 5 | 51.649636 | 20052 |
| swap mission 5 / mission 26 arms | 59.879614 | 20035 |
| reorder mission-switch cases to original address order | 59.144840 | 20034 |
| reorder mission-switch cases reverse address order | 58.836695 | 20044 |
| reorder cases sorted with 158 first | 59.206010 | 20033 |
| move `default:` to end of mission switch | 59.959208 (tie) | 20035 |
| multi-line `&&` swap @5220 | 59.160324 | 20037 |
| multi-line `&&` swap @1460 (first two) | 46.601410 | 19980 |
| multi-line `&&` swap @1460 (last two) | 59.949259 | 20035 |
| multi-line `&&` swap @1547 | 59.949259 | 20035 |
| multi-line `&&` swap @2796 | 59.178191 | 20035 |
| multi-line `&&` swap @3747 | 59.944284 | 20035 |
| multi-line `&&` swap @4015 | 59.949259 | 20035 |

The prologue reorder is instructive: `messages,player,enemies` makes the first
32 instructions match the original *exactly* (call order, `r5=messages`,
`r8=enemies`), but it swaps the `[sp,#72]/[sp,#76]` stack slots for
`messages`/`delta` (`strd r6,r5` instead of `strd r5,r6`) and loses 36
instructions further down, so the global ratio drops by 1.2%. The baseline's
declaration order (`enemies,player,messages`) reproduces the original's stack
layout but not its call order.

## Why the fixes fail

The official score is a global difflib alignment. Every content fix that changes
a block's size or position shifts the alignment of everything after it; the
local gain (a few matching instructions) is smaller than the global loss. The
baseline is a strong local optimum: of ~30 structural variants measured, none
beat 59.959208, and the only ties are changes that clang folds to identical
codegen.

## Remaining root cause

1. **Block ordering (MachineBlockPlacement).** Whole case bodies are placed
   hundreds of instructions away from the original. The source's statement order
   and the shape of each `if`/`else` chain drive this; matching it requires
   reconstructing the original's exact block structure, not local edits.
2. **Register allocation.** The original spills `this` to `[sp,#88]` (328
   reloads) and uses `sl` as scratch; ours keeps `this` in `sl` (195 reloads).
   The stack slots themselves already match the original.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w7103/regions2.py                      # 59.959208039
```

`/dev/shm/w7103` helpers (`regions2.py`, `opmap.py`, `dump_addr.py`,
`regdiff.py`, `bad1.py`) are scratch only. No source change is committed; the
worktree is clean.
