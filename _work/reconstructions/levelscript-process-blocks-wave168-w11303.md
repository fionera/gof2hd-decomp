# LevelScript::process — wave 168 / w11303: mission-0 switch CFG shape, default block, tails

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Worktree `wt/w11303`, branch `wave168-w11303`, base master
`631c26f0b` (wave 167).  Scratch `/dev/shm/w11303/`.

Follow-up to `levelscript-process-wave167-w11203.md` (mission-0 switch order +
state-16 block) and `levelscript-process-wave166-w11102.md`.

## Baseline (HEAD, tree left clean)

```
process raw diffbytes    54,031
process aligned insns    17,764
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. The original mission-0 switch is `switch {6..15} + default: state-16 check`

Re-decoded the original dispatch (`0x139b16`):

```
139b16  ldr.w r1,[sl,#28]      ; m_nState
139b1a  subs  r0,r1,#6
139b1c  cmp   r0,#9
139b1e  bhi.w 13f906           ; default -> "cmp r1,#16"
139b22  tbh   [pc,r0,lsl#1]    ; 10 entries, states 6..15
```

and the default block:

```
13f902  ldr r1,[sl,#28]        ; entry from case 15 (state reload)
13f906  cmp r1,#16
13f908  bne 137f7c             ; shared active tail
13f90a  ... isFading ...
13f924  enableFillScreen(true)
13f92e  nextCampaignMission(true)
13f940  switch_to_target_setting = 1
13f944  SetCurrentApplicationModule(2)
        b 13b158               ; shared tail: initStreamOutPosition = 0; return 0
```

Key structural fact: **every case body 6..14 `break`s straight to the shared
active tail `0x137f7c`, not to the state-16 check.**  Only the switch
`default` (dispatch `bhi`, state already in r1) and the state-15 *not-over*
path (`0x13d4d2 beq.w 0x13f902`, reload) reach the check.  Therefore the
state-16 check is the switch's **`default:` block**, and `case 15` **falls
through into it** when `msg[22]` is not over; the over-path does
`startFade; m_nState = 16; break` (T2 tail `0x13d4ec`) and exits.

HEAD's shape (`switch` then `if (m_nState == 16)` after it) instead makes
every case body branch to the check block (ours `b.w 0x13e262`), i.e. the
case `break` target is wrong.

## 2. Case-11 body has a spurious `enableFillScreen(true)`

Original `enableFillScreen` call sites in `process` (4):

```
0x138a6a  mission 1 state 1
0x139186  mission 0x51-ish state 3
0x13b100  mission 0x51 state 3
0x13f924  mission-0 state 16
```

Ours (HEAD) has **5**: the extra one is the first statement of mission-0
`case 11` (`src/game/world/LevelScript.cpp:853`).  The original state-11 body
(`0x13d2d4`) starts directly with `ldrd r0,r1,[sl,#144]` (timer) and never
calls `enableFillScreen`.  Removing it is worth **-16 B** and -65 raw on the
`default:`-inside shape (but -16 B / +633 raw on HEAD alone -> size-fatal).

## 3. Hoisted common tails (original) are reproduced once `default:` is inside

With the `default:`-inside shape the original's BranchFolder tails appear:

| tail | original | shared by | shapes |
|---|---|---|---|
| T1 | `0x13d4ba` `m_nState = X; m_nScriptTimerA = 0;` | state 7 (jump), state 13 (fallthrough) | yes |
| T2 | `0x13d4ec` `m_nState = X;` | state 12 (jump), state 15 (fallthrough) | yes |
| T3 | `0x13d592` `m_nScriptTimerA = 0;` | state 10 (jump), state 14 (fallthrough) | yes |

HEAD does **not** share T3 (state 10 inlines `strd r8,r8,[r6]` and branches to
the check block; state 14 inlines it and branches to the exit), because the
two `break` targets differ.  The `default:`-inside shape makes both targets
the shared exit and BranchFolder merges them (verified: state 10 `b.n` to the
state-14 tail block).

## 4. Measured shapes (full link, `tools/fn_diff.py`)

All include the mission-0 dispatch reorder `4,3,5` (the 10-entry table) unless
noted; `+c11` = case-11 `enableFillScreen` removed.

| shape | raw | aligned | size | tbh |
|---|---:|---:|---:|---|
| **HEAD (4,5,3)** | **54,031** | **17,764** | **57,232** | 13 (3..15) |
| HEAD - c11 | 54,664 | 17,767 | 57,216 | 13 |
| 4,3,5 | 55,105 | 18,469 | 57,240 | 10 (6..15) |
| 4,3,5 + default-inside + state16 fix + c11 | 55,329 | 18,817 | 57,264 | 10 |
| 4,3,5 + default-inside + state16 fix + c11, cases `6,10,7,8,9,11,12,13,15,14` | 55,382 | 20,111 | 57,252 | **11 (6..16)** |
| HEAD order + default-inside + c11 | 55,408 | 19,832 | 57,280 | 13 |
| 4,3,5 + default-inside + c11 + `Status *volatile` | 55,379 | 19,477 | 57,288 | 10 |

Notes:
* `4,3,5` alone reproduces the original's 10-entry table and the original's
  dispatch shape (`subs r0,r1,#6; cmp r0,#9; bhi`), but is +8 B and +1,074 raw.
* With `default:` inside the switch the **case source order becomes
  codegen-relevant** (it was not on HEAD's switch): the original physical
  order `6,10,7,8,9,11,12,13,15,14` makes SimplifyCFG fold the
  `if (m_nState == 16)` into an 11th table entry (states 6..16), so it is
  *not* the source order that yields the original table.
* The `default:`-inside shape's physical order is
  `6,8,10,15,7,9,11,12,13,14` (vs original `6,10,7,8,9,11,12,13,15,14`).
  The tail grouping is right but the chain order still differs.
* `Status *volatile` no longer restores size-exactness on this base (+24).

## 5. Conclusion

* Exact original CFG: `switch (m_nState) { case 6..15 }` with the state-16
  check as the switch **`default:`**; `case 15` falls through to `default`
  when `msg[22]` is not over; all case bodies `break` to the shared active
  tail.  Source order of the pre-switch arms is `4,3,5`.
* Genuine bug: mission-0 `case 11` has an extra
  `Globals::layout->enableFillScreen(true)` (5 vs 4 call sites).
* The faithful shape is **+32..+48 B** and raw-worse than HEAD on this base
  (the mission-0 switch region shifts by ~0x2100), so nothing was landed.
  Landing it needs a -32..-48 B companion in `process` (the `default:`-inside
  shape already removes 16 B via case 11).
* Tree left clean at the entry baseline.
