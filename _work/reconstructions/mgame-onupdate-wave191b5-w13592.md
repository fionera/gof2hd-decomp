# MGame::OnUpdate — non-anchor r9 pins and the store/register coupling, wave 191b / w13592

Status: **no landed `src/` change** (negative sweep).  Worktree `wt/w13592`,
branch `wave191b-w13592`, base master `782318ada`.  Target
`_ZN5MGame8OnUpdateEv` stays at **raw 10 / aligned 112 / 11,440 B,
insn 4003:4002**.  `check` re-run at the end of the sweep: **RATCHET PASS**
(byte 4508, linked 4508, size 4524, data 843/843, imports 207, lint CLEAN,
drift 0).  No baseline touched.

Follows `mgame-onupdate-wave191-w13555.md`,
`…wave191b2-w13565.md`, `…wave191b3-w13565.md`, `…wave191b4-w13565.md`.

## 1. Residual (unchanged)

* cond `0x17e2b9..c5` (4 B): orig `ldr.w r2,[r9]; ldr r4,[pc](=3600000);
  ldrd r2,r3,[r2,#256]`; HEAD `ldr.w r4,[r9]; ldr r5,[pc];
  ldrd r2,r3,[r4,#256]` (`subs r0,r5` follows).
* body `0x17f190..1b1` (6 B): orig `mov r4,r0; ldr.w r0,[r9]; blx ccm; …
  add r2,r4`; HEAD `mov r5,r0; mov r0,r4; str r4,[sp,#120]; blx ccm; …
  add r2,r5`.

Both are the anchor's r4 occupancy.  The ideal flip (anchor r5, constant
r4, `radioIndex` r4) would be raw 6.

## 2. New evidence: the r9 pin trigger is narrower than "live across a call"

The pin is **not** "any `Status *` local live across a call".  It needs the
local to be live across the m23 branch's `getPlayingTime()` call at
`0x17e0b8`:

| shape (`Status *st` before the if-chain, plain body + `volatile Status *dummy = st;` at the tail) | size | raw |
|---|---:|---:|
| used at the m23 `getPlayingTime()` receiver (`s4`) | 11,440 | **41** |
| used at the m23 `getMission()` receiver (dies at `0x17e0a8`) | 11,396 | 5,173 |
| no receiver use at all (store only, `s4b`) | 11,392 | 5,196 |
| assigned at the receiver via a comma (`(st = Globals::status)->…`) | 11,392 | 5,196 |
| declared at the top of `OnUpdate` (sunk to the use) | 11,392 | 5,196 |

So the allocator keeps `%1053` (GOT base of `@Globals::status`) in r9 only
when the local's live range covers the `getPlayingTime` call at
`0x17e0b8` (the last call before the 122 branch).

## 3. `s4`: the closest structural match (cond AND body byte-exact)

`s4` = `Status *st = Globals::status;` before the if-chain, the m23
condition's `getPlayingTime()` receiver uses `st`, the m122 body is the
plain shape, and `volatile Status *dummy = st;` sits before the tail
store.

`st` lands in **r5** (the ideal-flip register).  Both the cond
(`ldr.w r2,[r9]; ldr r4,[pc]; ldrd r2,r3,[r2,#256]`) and the whole m122
body (`mov r4,r0; ldr.w r0,[r9]; …; add r2,r4`) are **byte-exact**.  The
entire 41-byte residual is one contiguous 2-byte-shift run
`0x17e093..0x17e0bc`:

* `+2` at `0x17e094`: the first `getCurrentCampaignMission()` receiver
  becomes `ldr.w r5,[r9]; mov r0,r5` because the compiler CSEs the first
  `Globals::status` load with `st`;
* `-2` at `0x17e0ba`: the m23 `getPlayingTime()` receiver becomes
  `mov r0,r5` instead of `ldr.w r0,[r9]`.

Net 0 bytes, so the size stays 11,440, but everything in between shifts by
2.  This is the same failure family as w13565's `st_before_pt` /
`cond_head_S`, just with the shift between two receivers instead of at a
call receiver.

## 4. The store position and the anchor register are coupled

| shape | st reg | store position | raw | aligned | size |
|---|---|---:|---:|---:|---:|
| `s_base` (store in the `radioBase` comma) | r4 | before `blx ccm` | 10 | 112 | 11,440 |
| `q_lv` (store in the `Level *level = (dummy = st, this->level)` comma) | **r5** | after `blx ccm` | 10 | 108 | 11,440 |
| `q10`/`j` (store as a separate statement after the level load) | r5 | after `blx ccm` | 344 | **18** | 11,440 |
| `sac3` (store between the ccm and the level load) | r5 | after `blx ccm` | 345 | 22 | 11,440 |

The volatile store cannot move across the `ccm` call, so putting it in the
source before the ccm always yields st = r4 (`s_base` = HEAD), and putting
it after always yields st = r5 (`q_lv`/`q10`).  The "ideal flip" (st r5 +
store before the ccm) was not reached by any spelling tried.

`q10` is the closest **instruction-index** match found: the cond is 2 B
(`ldr.w r5,[r9]` / `ldrd r2,r3,[r5,#256]`) and the whole m122 body is
byte-exact except a single inserted `str r5,[sp,#120]` at `0x17f19a`.
That insertion is absorbed by the pool `nop` at `0x17f2f2` (0x158 bytes
later), so raw is 344 while aligned is 18 and insn is 4002:4002.

## 5. Other families measured this session

All size 11,440 unless noted; `raw`/`aligned` from
`tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv`.

| shape | raw | aligned | insn | note |
|---|---:|---:|---:|---|
| **HEAD anchor** | **10** | 112 | 4003 | |
| plain (no anchor) | 5,196 | 597 | 3974 | 11,392 B |
| `q2`/`q3`/`a` (store after the level load / separate statement) | 10 | 108 | 4003 | = `q_lv` |
| `q4`/`q5`/`c` (store before `nextInt`) | 10 | 112 | 4003 | = HEAD |
| `t3` (dead `if (st) {}` after the ccm) | 10 | 112 | 4003 | optimised away |
| `lv_nt_ccm` (`Level *level` before `nextInt`) | 33 | 8 | 4002 | level load moved |
| `hsf`/`hsf2` (store before `nextInt`) | 15 | 112 | 4003 | |
| `e`/`g` (store after `radioBase`/`level`) | 20 | 110 | 4003 | |
| `b` (store before `nextInt`, `radioBase` first) | 29 | 117 | 4003 | |
| `lv_anchor1` (level anchor, store after the call) | 362 | 24 | 4002 | |
| `lv_anchor3` (level anchor after `nextInt`) | 345 | 22 | 4002 | |
| `vv3`/`vv4` (volatile-load anchor) | 361–362 | 20–21 | 4002 | |
| `two1`/`two2` (two volatile stores) | 346–348 | 112–114 | 4003 | |
| `m24recv` (`st` at the m24 `getPlayingTime`) | 270 | 350 | 4003 | |
| `b122recv` (`st` at the 122 `getPlayingTime`) | 540 | 354 | 4003 | |
| `s4_rb`/`s4_nt` (`s4` + store before the ccm) | 387/397 | 350 | 4003 | |
| `q8` (store only, no receiver use) | 918 | 109 | 4001 | 11,436 B |
| `q9`/`vv2` | 898/941 | 125–127 | 4003 | 11,444 B |
| `sac1`/`sac2` (store after the ccm, level before) | 942 | 135 | 4005 | 11,448 B |
| `sac4` (store at the very end) | 5,329 | 758 | 4022 | 11,496 B |
| `m23+b122ccm` / `m23+m24recv` | 4,569/4,785 | 91/97 | 4002 | 11,436 B |
| `m23recv_mission` | 4,986 | 99 | 4002 | 11,436 B |
| `hint_ship`/`hint_ccm`/`hint_both` | 5,081–5,084 | 792–813 | 3995–3996 | 11,464 B |
| `h48_*` (store in the hints[48] block) | 6,198–6,258 | 988–1,017 | 4019–4020 | 11,480–11,484 B |
| `top_st` (top-of-function `Status *st`) | 5,196 | 597 | 3974 | 11,392 B |
| `stccm` | 918 | 109 | 4001 | 11,436 B |

## 6. Conclusion

HEAD's raw 10 remains the local optimum.  Every shape that gets the anchor
into r5 (the ideal-flip register) either moves the volatile store after
the `ccm` call (`q_lv`/`q10`, 2-byte insertion wall) or needs a receiver
use that costs a 2-byte shift (`s4`).  Every shape that keeps the store
before the ccm puts the anchor back in r4 (HEAD).  No size-exact win.

## 7. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13592/`
(`batch2.py`, `gen1.py`, `try2.py`, `bd.py`, `fd.py`, `mirdump.sh`,
`MGame.cpp.head`, `onupdate_*.mir`, `mir_*.err`).

## 8. Addendum (same worker, second sweep): the natural r9 pin and why it still is not byte-exact

Follow-up sweep after the note above was committed.  `src/` unchanged; target
still **raw 10 / aligned 112 / 11,440 B**, `check` re-run: **RATCHET PASS**
(byte 4508, linked 4508, size 4524, data 843/843, imports 207, lint CLEAN).

### 8.1 The plain shape already keeps `&Globals::status` in r9

Disassembly of the *plain* (no anchor) shape shows the address is **not** moved
to r5 globally: `0x17e090 ldr.w r9,[r0]` still loads `&Globals::status` into
r9, exactly like the original.  What the plain shape does instead is

```
17e094: ldr.w r6,[r9]        ; cache the *value* Globals::status in r6
17e098: mov   r0,r6          ; +2 bytes at the first receiver
...
17e256: mov   r5,r9          ; copy the address into r5 for the 122 branch
17e272: ldr   r0,[r5,#0]     ; 2-byte loads instead of 4-byte ldr.w [r9]
```

i.e. the 48-byte shrink is a *local* re-materialisation of the address into r5
for the 122 branch (2-byte `ldr`), not a global allocation change.  So the
anchor's job is narrower than "pin r9": it has to stop the
`mov r5,r9` copy at `0x17e256` (and the r6 value cache at `0x17e094`).

### 8.2 A non-anchor shape that does stop the copy: level declared before `radioBase`

```
int radioIndex = Globals::rnd->nextInt(2);
Level *level = this->level;
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
level->createRadioMessage(28, radioIndex + radioBase);
```

This is the only non-anchor family found that keeps `&Globals::status` in r9
through the 122 branch.  The level load (r5) is live across the `ccm` call and
occupies r5, so the allocator cannot copy the address there.  Result:

| shape | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| `o_nt_lv_rb` (= `lv_after_nt`, `p1`..`p4`) | **16** | **6** | 4002 | 11,440 |
| `o_lv_nt_rb` (= `lv_before_nt`) | 33 | 8 | 4002 | 11,440 |
| HEAD anchor | 10 | 112 | 4003 | 11,440 |

`o_nt_lv_rb` has a **byte-exact cond** (`ldr.w r2,[r9]; ldr r4,[pc];
ldrd r2,r3,[r2,#256]`) and a byte-exact `mov r4,r0` / `ldr.w r0,[r9]` in the
body.  Its whole 16-byte residual is one 4-byte shift around the level load:

```
O 17f19a movw r2,#3166 ; ldr.w r1,[fp,#120] ; cmp ; it eq ; movweq r2,#3164
  ; add r2,r4 ; mov r0,r1 ; movs r1,#28 ; blx
U 17f196 ldr.w r5,[fp,#120] ; blx ccm
  ; movw r1,#3166 ; cmp ; it eq ; movweq r1,#3164 ; mov r0,r5 ; adds r2,r1,r4 ; ...
```

The level load cannot be sunk past the `getCurrentCampaignMission` call (the
compiler cannot prove the Status call does not alias `this->level`), so
declaring it before `radioBase` always moves it 4 bytes earlier.  Declaring it
after `radioBase` puts it back at `0x17f19e` but lets the allocator emit
`mov r5,r9` again (plain, 11,392 B).  This is the same trade-off as the anchor
store position, now on the level load.

### 8.3 Other new shapes measured (all size 11,440 unless noted)

| shape | raw | aligned | note |
|---|---:|---:|---|
| `ns_tailrecv` (no volatile store, `st` at receiver + tail receiver) | 37 | 109 | ideal body, tail `mov r0,r5` +2 |
| `ns_tailtgt` (tail target) | 5,245 | 758 | 11,488 B |
| `ns_tailboth` | 5,254 | 758 | 11,488 B |
| `ns_lv` (no store, level before radioBase) | 918 | 114 | 11,436 B |
| `ns_lv_tail` | 37 | 115 | |
| `t_recv` (store before ccm + tail receiver) | 322 | 113 | ideal body, duplicated tail |
| `t_store` / `t_both` (tail target) | 5,305 / 5,249 | 759 / 758 | 11,492 / 11,488 B |
| `ext_orbit` (`st` at `inAlienOrbit`) | 602 | 365 | stack frame +0x80 |
| `ext_wing` / `ext_hint` / `ext_hint2` | 4,668 / 5,082 / 5,083 | 756 / 787 / 787 | 11,456 / 11,464 B |
| `h_lv` (level hoisted before the if-chain) | 4,135 | 302 | |
| `h_rnd` (`Globals::rnd` hoisted) | 5,045 | 429 | 11,420 B |
| `h_lv_st` | 4,458 | 321 | |
| `o_lv_rb_nt` / `o_rb_lv_nt` / `o_rb_nt_lv` | 1,239 / 1,240 / 1,241 | 170 / 170 / 154 | 11,436 B |
| `v_int` (volatile `int` store) | 10 | 112 | identical to HEAD |
| `v_void` / `v_char` (volatile `void*`/`char*`) | 918 | 109 | 11,436 B |
| `v_rmw` (two stores) | 355 | 116 | |
| `v_ri` (store in `radioIndex` init) | 15 | 112 | |
| `v_bef` (store before nextInt) | 15 | 112 | |
| `cond_swap` / `cond_ge` / `cond_swap_ge` | 10 | 112 | operand order irrelevant |
| `all_st` (outer `st`, all 3 tail stores) | 5,321 | 739 | 11,484 B |

### 8.4 Conclusion of the addendum

`o_nt_lv_rb` (raw 16 / aligned 6) is the closest **instruction-level** match
found and is a genuine non-anchor r9 pin, but its raw count is worse than
HEAD's 10 and it does not reach `byte_exact`, so nothing was landed.  The
ideal flip (anchor in r5, store before `ccm`) is still unreached: every shape
that gets r5 either moves the store after the `ccm` (`q_lv`/`q10`) or needs a
post-`ccm` use that duplicates the shared tail (`t_recv`, +2 shift).
