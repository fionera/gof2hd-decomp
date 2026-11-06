# LevelScript::process — wave 124 / w7404 (case-125 cold-block probe + state2 `m_nState++`)

Status: **no source win**, note-only. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `6f8ee2320` (wave-123 final). Source:
`src/game/world/LevelScript.cpp` (unchanged).

## Baseline / gate

```
raw ratio (difflib)      60.256665340
official verify pct      60.3
orig / ours insns        20169 / 20039
orig / ours size         57232 / 57232        (size-exact)
byte_exact               4506
linked_exact             4506
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN ✓
drift                    0
RATCHET                  PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
over `tools/verify/verify.py`'s reconciled instruction streams of the linked
`.so` (helper `/dev/shm/w7404/score.py`, same definition as wave 119/121/122/123).

## What this pass established

### 1. The original's case-125 state machine, block by block

The original splits case 125 across four widely separated regions:

| state | original block | ours (baseline) |
|---|---|---|
| 0 (`route` setup) | `0x139f9c` (hot, body ends `b.w 0x3800`) | `0x139d42` |
| 1 (`setName` loop) | `0x13b43e` (cold, after the supernova block) | `0x139d96` (inline) |
| 2 (pirate ambush + teardown) | `0x13b518` check, body `0x13b522` | `0x139e6a` (inline) |
| 3 (`moveForward` + timer) | `0x13dcb2` | `0x139e6a` check, body `0x13d76c` |
| 4..6 (hacking) | `0x13de44` | `0x13d942` |

The original's state-0 `if` is an **`else if` chain** with state 1:

```
139f9c  ldr r0,[sp,#88] ; this
139f9e  ldr r1,[r0,#28] ; m_nState
139fa0  cmp r1,#0
139fa2  bne.w 0x3726    ; -> state-1 check at 0x13b43e
...
139ffa  b.w   0x3800    ; state-0 body -> state-2 check at 0x13b518
```

and state 1 falls through into the state-2 check:

```
13b43e  cmp r1,#1
13b442  bne.n 0x3800
...
13b516  bls.n 0x376e    ; loop back
13b518  ldr r0,[sp,#88] ; state-2 check (fallthrough)
```

So the original source is `if (state==0 && msg) {...} else if (state==1) {...}
if (state==2) {...} else if (state==3) ...`. Our source has state 1 as a
**separate `if`**, which is why MBP keeps it inline after the state-0 body.

### 2. The faithful `else if` shape reproduces the original CFG but not its size

`else if` alone (no hint) makes clang treat the state-1 arm as the hot
fallthrough and moves the *state-0 body* cold (`cmp r1,#0; beq body`), the
opposite of the original. Adding a branch hint on the state-0 condition
reproduces the original's block structure exactly:

```cpp
if (__builtin_expect(m_nState == 0 && messages[1]->isTriggered(), 1)) {
    ...
} else if (m_nState == 1) {
    ...
}
if (m_nState == 2) { ... } else if (m_nState == 3) { ... } ...
```

Measured layout of that variant (`expect2`):

```
139d42  state-0 body ... 139d9e  b.w 0x450a   ; -> state-2 check
...
13c150  state-1 check
13c156  state-1 body ... 13c220
13c222  state-2 check (fallthrough)
```

This is the original's shape (state 0 hot, state 1 + state 2 cold, state-0 body
jumps over state 1 to the state-2 check). But it is **+8 bytes** (57240) and
scores **58.106394091** because it perturbs the global MBP layout of the later
cases: a 966-instruction run at orig `0x13cf62`-`0x13da0e` is displaced, and a
590-instruction run is inserted at `0x13df5c`. The case-125 gain (~330
instructions) is smaller than the global loss.

### 3. Content finding: the original's state-2 body ends with `m_nState++`

The source currently ends the case-125 state-2 body with `m_nState = 4;`
(line 3961). The original ends it with an increment:

```
13b7e0  movs r1,#0
13b7e2  ldr  r0,[sl,#28]        ; m_nState
13b7e6  str  r1,[sl,#144]       ; m_nScriptTimerA = 0 (lo)
13b7ea  str  r1,[sl,#148]       ; m_nScriptTimerA = 0 (hi)
13b7ee  b.w  loc_ccf4           ; -> 0x144a0c
...
144a0c  adds r0,#1
144a0e  str.w r0,[sl,#28]       ; m_nState++
144a12  b.n  loc_d11e           ; post-switch tail
```

`loc_ccf4` is reached from this one site only. So the faithful source is
`m_nState++;` (state 2 -> 3, then the state-3 block runs next frame), not
`m_nState = 4;`. Changing it is a real fidelity fix, but it is **-4 bytes**
(57228) and scores **60.115405661**, so it cannot be landed alone without a
+4-byte compensation; size-exactness is mandatory (a -4 B change shifts every
later `.text` symbol and drops `byte_exact` from 4506 to ~3690).

## Variants measured (all reverted; 57232 B unless noted)

| variant | raw % | ours size |
|---|---:|---:|
| **baseline** | **60.256665340** | 57232 |
| case-125 state1 `else if` (plain) | 58.006913875 | 57248 |
| nested `if` + `else if` | 58.117787505 | 57248 |
| `else if` + `__builtin_expect(cond, 1)` | 58.084413162 | 57236 |
| `else if` + `__builtin_expect(state==0, 1)` | 58.106394091 | 57240 |
| `else if` + `__builtin_expect(state==1, 0)` | 57.955732405 | 57244 |
| `else if` + expect(cond,1) + expect(state1,0) | 58.066602005 | 57240 |
| nested + expect(state0,1) + `else if` | 58.129181486 | 57244 |
| `else { state1; state2; ... }` wrap + expect(cond,1) | 41.309585911 | 57340 |
| `else` wrap + expect(state0,1) | 48.404479873 | 57616 |
| separate `if` + expect(cond,1) | 60.246717071 | 57232 |
| separate `if` + expect(state0,1) | 60.246717071 | 57232 |
| separate `if` + expect(state1,0) | 25.529166253 | 57544 |
| state2/3/4-6 chain order `324` | 59.018676482 | 57240 |
| state2/3/4-6 chain order `243` | 52.317798403 | 57396 |
| state2/3/4-6 chain order `342` | 48.001986837 | 57628 |
| state2/3/4-6 chain order `423` | 59.027725973 | 57260 |
| state2/3/4-6 chain order `432` | 56.893464637 | 57252 |
| state-2 tail faithful order (up before right, timer after setPosition) | 60.256665340 | 57232 (codegen-identical) |
| state-2 tail `m_nState++` | 60.115405661 | 57228 |
| state-2 tail faithful order + `m_nState++` | 60.110431279 | 57228 |
| `expect` + `m_nState++` | 60.115405661 | 57228 |

The `else if` + `__builtin_expect(state==0,1)` variant is the only one that
reproduces the original's *case-125 block structure*; every size-exact variant
(the baseline and the two `separate if + expect` variants) leaves state 1
inline.

### `getPlayer()` sweep (all single-site `m_pLevel->getPlayer()` -> `player`)

The wave-123 landed fix already removed the one size-exact win at line 790.
Re-swept every remaining occurrence against the current baseline; the best
size-exact results are still below baseline:

| source line | raw % | size |
|---|---:|---:|
| 3995/3996 (`setAutoPilot`/`isDockedToDockingPoint`) | 60.250193 | 57232 |
| 4000/4001 (`resetGunDelay`/`dockToDockingPoint`) | 60.240245 | 57232 |
| 5188 (`resetGunDelay`) | 60.225323 | 57232 |
| all others | 59.35–59.52 | 57224/57228 |

So the remaining `getPlayer` count delta (orig 23 / ours 27) is not a
size-exact source lever; the original genuinely re-calls `getPlayer()` at most
of these sites.

## Remaining root cause

The residual is a **global MachineBlockPlacement permutation**, not a local
case-125 problem. The original interleaves case 125's cold state-1/state-2
blocks (at `0x13b43e`/`0x13b518`) with case 92's hot body; our baseline keeps
case 125's sub-blocks contiguous. Reproducing the original's case-125 layout
from source is possible (`else if` + `__builtin_expect(state==0,1)`), but the
same MBP decision re-permutes the later cases and the net difflib score drops
by ~2.1 points. The baseline is a strong local optimum.

The one real content bug found is the case-125 state-2 `m_nState = 4` which the
binary shows as `m_nState++`; it is -4 bytes and needs a +4-byte companion fix
before it can be landed under the size-exact gate.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7404/score.py                         # 60.256665340
python3 /dev/shm/w7404/dump3.py orig 0x139f9c 0x139ffc  # state-0 hot body
python3 /dev/shm/w7404/dump3.py orig 0x13b43e 0x13b51a  # cold state-1
python3 /dev/shm/w7404/dump3.py orig 0x13b7e0 0x13b7f0  # m_nState++ tail
```

Scratch helpers (`score.py`, `dump3.py`, `pbuild.py`, `opcodes.py`,
`sweep_player.py`, variant `.cpp` files) live in `/dev/shm/w7404/` and are not
part of the tree. No source change was committed; the worktree is clean.
