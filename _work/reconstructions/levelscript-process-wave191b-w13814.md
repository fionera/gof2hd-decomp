# LevelScript::process — wave 191b / w13814: case-158 up-vector + shared-tail route order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13814`, branch
`wave191b-w13814`, base master `342cf7a86`.

Session start: raw **53,007**, aligned **19,694**, `.text` diffbytes 75,919,
`.ARM.extab` 594, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`.
Session end: raw **53,006**, aligned **19,684**, `.text` diffbytes 75,918,
`.ARM.extab` 594, `.rel.plt` md5 unchanged, size 57,232/57,232 exact.

## Landed

### 1. case 158 (mission 0x9e) state 1 — lazy `up` vector (orig 0x13891c..0x13892c)

Original constructs the `up` vector `{0,1,0}` *after* `getPosition` / `VectorNormalize`
and passes it straight to `setDirection`.  Our source materialised a named
`Vector up{0.0f, 1.0f, 0.0f};` before the call.  Inlining the temporary:

```c++
- Vector up{0.0f, 1.0f, 0.0f};
- marker->setDirection(AbyssEngine::AEMath::VectorNormalize(target - pos), up);
+ marker->setDirection(AbyssEngine::AEMath::VectorNormalize(target - pos), Vector{0.0f, 1.0f, 0.0f});
```

This alone is raw **53,007 -> 53,005**, aligned **19,694 -> 19,688**,
`.text` **75,919 -> 75,917**.

### 2. shared tail (orig 0x144f36..0x144f4c) — route teardown order

The original fail branch emits

```
createRadioMessage(15)
PlayerEgo::setAutoPilot(nullptr)      ; r0 = player (sl)
PlayerEgo::setRoute(nullptr)          ; r0 = player (sl)
Level::setPlayerRoute(nullptr)        ; r0 = m_pLevel
m_nState = 2
```

Our source had `m_pLevel->setPlayerRoute` before `player->setRoute`.  Swapping them
to the original order gives raw **53,005 -> 53,006**, aligned **19,688 -> 19,684**,
`.text` **75,917 -> 75,918** (raw +1, aligned -4).  Landed because the aligned
instruction match is the better signal in the shifted tail region and the prior
w13810 pass established the precedent of trading a small raw regression for a
larger aligned win.

## Measured variants (all size 57,232 exact unless noted)

| variant | raw | aligned | .text | .ARM.extab |
|---|---:|---:|---:|---:|
| session start (HEAD) | 53,007 | 19,694 | 75,919 | 594 |
| up-vector inline only | **53,005** | 19,688 | **75,917** | 594 |
| **+ route order swap (landed)** | 53,006 | **19,684** | 75,918 | 594 |
| + radio-14 moved to state-0 block | 53,012 | 19,684 | 75,924 | 594 |
| + radio-14 move, no route swap | 53,012 | 19,690 | — | — |
| + radio-14 move + combined `m_nState<=1 && getHitpoints()` | 53,012 | 19,684 | 413,544 | 1,518 |
| + `__builtin_expect(cond,0)` (size 57,236, +4) | 53,191 | 19,680 | — | — |

## Shared-tail finding not landed (radio-14 placement)

The original has **only two** `Level::createRadioMessage` calls in the whole
function: radio **15** in the fail branch (0x144f32) and radio **14** in the
*state-0 success block* (0x144fc8), i.e.

```
if (m_nState == 0) {
    ... distance ...
    if (distance <= 49999 && field_5e == 0) {
        for (...) { setAlwaysEnemy; if (i && prev->field_0x3e) break; }
        m_pLevel->createRadioMessage(14, ...);
        m_nState = 1;
    }
}
if (m_nState <= 1) { ...fail branch: radio 15, setAutoPilot, setRoute, setPlayerRoute, m_nState=2... }
```

Our source currently emits radio 14 at the *end of the fail branch* (after
`m_nState = 2`) and has no radio call in the state-0 block.  Moving it to the
state-0 block (after the `setAlwaysEnemy` loop, before `m_nState = 1`) is
structurally faithful but metric-neutral: aligned stays 19,684 while raw rises to
53,012 and `.text` to 75,924.  Reverted per the "no measured win -> revert" rule;
left here as the next concrete target for a worker who can also fix the block
placement.

Why the metric does not reward it: the original places the state-0 success block
**out of line** after the epilogue (`0x144f80`, reached by `beq.n 144f80` from
0x144e9e) and jumps back to the fail check (`b.n 144eae`), skipping the
`ldr r0,[r9,#28]; cmp r0,#1` reload on that path.  Our build keeps the block
inline (`0x144b70`) and re-loads `m_nState` at `0x144bbc` before the check.  The
`__builtin_expect(cond, 0)` hint reproduces the out-of-line placement and drops
aligned to 19,680, but costs +4 B (57,236) because a branch crosses the short
range; it needs an exact 4-byte compensation elsewhere to be landable.

## Other rejected experiments

* Removing the `KIPlayer *e` cache in the pre-switch state-0 loop
  (`(*enemies)[i]->geometry->setDirection(...)` / `->setVisible(...)` twice) is
  size-fatal: **57,348 (+116)**, raw 55,555, aligned 23,901.  The original does
  reload `enemies->data[i]->geometry` for each call, but expressing it that way
  makes clang 7.0.2 duplicate far more than the two `ldr`s.
* case-158 state 2 and state 3 bodies are instruction-identical modulo base
  registers and block targets; state 3's timer store (`m_nScriptTimerB = 0`)
  precedes the `field_0x140` clz toggle in the original (0x14586c), but putting
  the store first is raw **+1** (53,006 vs 53,005) with aligned unchanged, so it
  was left in the toggle-first form.

## Residual shape

The function is 812 B (`0x32c`) "ahead" of the original by the time it reaches
the shared tail (ours `0x144b0a`, original `0x144e36`).  Instruction counts:
ours 20,725 vs original 20,830 (105 fewer).  The raw metric is dominated by this
address shift, so local tail edits move raw by only a handful of bytes; the
aligned metric is the more reliable signal for tail work.
