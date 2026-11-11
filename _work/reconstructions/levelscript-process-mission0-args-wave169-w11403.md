# LevelScript::process — wave 169 / w11403: mission-0 argument operands

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11403`, branch `wave169-w11403`, base master
`ec3ceb07c`.  Scratch `/dev/shm/w11403/`.

## Baseline (HEAD)

```
process raw diffbytes    54,015
process aligned insns    17,788
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Landed: two wrong `FModSound::stop` operands in the mission-0 arms

Both are size-neutral (same 2-byte `ldr r1,[...]` encoding) and match the
original instruction-for-instruction.

1. **state 4** (`src/game/world/LevelScript.cpp`, `if (m_nState == 4)`):
   `Globals::sound->stop(player->field_0x1c)` ->
   `Globals::sound->stop(Globals::sound->currentMusicEvent)`.

   Original `0x138d3a..0x138d44`:
   ```
   138d3a ldr r0,[pc]        ; &Globals::sound
   138d3c add r0,pc
   138d3e ldr r4,[r0,#0]     ; = &Globals::sound
   138d40 ldr r0,[r4,#0]     ; this = Globals::sound
   138d42 ldr r1,[r0,#0]     ; arg  = Globals::sound->currentMusicEvent
   138d44 blx stop
   ```
   HEAD loaded `[r5,#28]` (player->field_0x1c) instead.  This is the
   "stop current sting, then play(143)" pattern used by state 2 / state 8.

2. **state 5** (`else if (m_nState == 5 && ...)`): the reverse —
   `Globals::sound->stop(Globals::sound->currentMusicEvent)` ->
   `Globals::sound->stop(player->field_0x1c)`.

   Original `0x139064`: `ldr r1,[r5,#28]` (r5 = player) before `blx stop`;
   HEAD loaded `[r0,#0]` (currentMusicEvent).  This is the engine-sound
   stop in the docking brake sequence.

Measured (full links, `tools/fn_diff.py`):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | 54,015 | 17,788 | 57,232 |
| state-4 stop arg | 54,012 | 17,787 | 57,232 |
| + state-5 stop arg | 54,012 | 17,787 | 57,232 |

`check` green: byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 /
imports 207, lint CLEAN, RATCHET PASS.

## Other mission-0 findings (measured, not landed)

* **case 6 statement order**: original does `m_pCamera->translate(...)` *before*
  `m_nScriptTimerA += delta` (original `0x139bdc` timer add after the
  `translate` call at `0x139bd8`; HEAD adds the timer first).  Swapping the two
  lines makes the body instruction-for-instruction faithful (translate, then
  timer add kept in r4/r5 for `__aeabi_l2f`, store after the divide), but is
  size-neutral and raw **+4** (54,019 / 17,790 / 57,232), so it was not
  landed.
* **case 12 `setPosition` overload**: original `0x13d42e` passes a `Vector&`
  (`mov r1,r5; blx setPosition`); HEAD used the 3-float overload
  (`add r3,fp,#40; ldmia r3,{r1,r2,r3}`).  Changing
  `setPosition(m_tempVec.x, m_tempVec.y, m_tempVec.z)` to
  `setPosition(m_tempVec)` is genuine but **-8 B** (57,224) and raw +444, so
  it needs an 8-byte companion before it can land.
* **case 11 extra `enableFillScreen(true)`** (confirmed again): -16 B.
* **state-16 block** (missing `enableFillScreen`, spurious timer add):
  -16 B on this base.
* The mission-0 switch bodies themselves are individually within a few bytes
  of the original (state 4: 206 vs 208 B; state 5: 296 vs 308 B; state 6:
  298 vs 302 B).  The remaining mismatch is global block placement
  (`tbh` at `0x13904e` HEAD vs `0x139b16` original) plus shared-tail
  ownership, not the per-case source.
* The `4,3,5` pre-switch reorder still gives the original 10-entry
  `tbh` (states 6..15) but is +16 B on this base and raw 55,210; the
  default-inside shape is +44 B and raw 55,394.  Neither is landable
  without a large size companion.
