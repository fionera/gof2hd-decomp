# MGame::OnUpdate — non-anchor r9 pins, wave 191b / w13565 (third note)

Status: **no landed `src/` change** (negative sweep).  Worktree `wt/w13565`,
branch `wave191b-w13565`, base master `113a6f7a0`.  Target
`_ZN5MGame8OnUpdateEv` stays at **raw 10 / aligned 112 / 11,440 B,
insn 4003:4002** (the w189 anchor shape).  `check` re-run this session:
**RATCHET PASS** (byte 4507, linked 4507, size 4524, data 843/843,
imports 207, lint CLEAN, drift 0).  No baseline touched.

Follows `mgame-onupdate-wave191-w13555.md`,
`mgame-onupdate-wave191b-w13565.md` and
`mgame-onupdate-wave191b2-w13565.md`.

## 1. Residual (unchanged)

* m23 `0x17e2b9..c5` (4 B): orig `ldr.w r2,[r9]; ldr r4,[pc](=3600000);
  ldrd r2,r3,[r2,#256]`; ours `ldr.w r4,[r9]; ldr r5,[pc];
  ldrd r2,r3,[r4,#256]` (`subs r0,r5` follows).
* m122 `0x17f190..1b1` (6 B): orig `mov r4,r0; ldr.w r0,[r9]; blx ccm; …
  add r2,r4`; ours `mov r5,r0; mov r0,r4; str r4,[sp,#120]; blx ccm; …
  add r2,r5`.

The anchor's value is the m23 condition's own `Globals::status` load
(GVN merges the body's `Status *st = Globals::status;` with it, see
`onupdate_head.ll`: `st` == `%1219`).  Because that value is live across
`nextInt`, it takes **r4** (first callee-saved in the ARM allocation
order), which is exactly what pushes the m23 constant to r5 and
`radioIndex` to r5.

Size accounting of the ideal flip (anchor in r5, `radioIndex` in r4):
m23 would be 2 B (`ldr.w r5,[r9]` vs `ldr.w r2,[r9]`, constant back in
r4) and m122 4 B (`mov r0,r5` + `str r5` vs `ldr.w r0,[r9]`), i.e.
**raw 6**.  No source shape found that puts the anchor in r5 without
adding an instruction.

## 2. Measured shapes (all size 11,440 unless noted)

`raw` from `tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so
_ZN5MGame8OnUpdateEv` after a full `link_so`.

| shape (m122 body) | raw | aligned | insn |
|---|---:|---:|---:|
| **HEAD anchor** (`st; nextInt; volatile dummy=st; radioBase=st->ccm()`) | **10** | 112 | 4003 |
| `two_lv_st` / `two_st_lv` / `two_st_gt` / `two_st_snd` (HEAD + a second `*volatile` store of another global) | 10 | 112 | 4003 |
| `head_store_first` (`volatile dummy=Globals::status; st; nextInt; …`) | 15 | 111 | 4003 |
| **`w163`** (`nextInt; Level *level=this->level; radioBase; level->call`) | **16** | **6** | **4002** |
| `lv_nt_ccm` (`Level *level` before nextInt) | 33 | 8 | 4002 |
| `lv_st` / `st_lv` (level + anchor, either order) | 37 | 8 | 4003 |
| `head_store_after_lvl` (store after `Level *level`) | 20 | 7 | 4003 |
| `head_ccm_plain2` (= `sac`: store after the ccm, ccm via `Globals::status`) | 19 | 7 | 4002 |
| `sac_ptr` (`volatile Status *` instead of `volatile int`) | 19 | 7 | 4002 |
| `head_ccm_plain` (HEAD, ccm via `Globals::status`) | 348 | 20 | 4002 |
| `head_ccm_plain3` (store before nextInt, ccm plain) | 361 | 20 | 4002 |
| `vlac` (volatile load anchor before nextInt, ccm via `st`) | 362 | 20 | 4002 |
| `vlac_plain` (volatile load, ccm plain) | 5211 | 597 | 3974 | (11,396 B) |
| `head_ccm_plain4` (store at end, ccm plain) | 898 | 59 | 4002 | (11,444 B) |
| `st_all` (`Status *st` before the if-chain, every `Globals::status` in the 23/24/122 branches replaced by `st`) | 6038 | — | — | (11,460 B) |
| `barrier_vol` / `barrier_zero` / `barrier_vload` (volatile clobber before `st` to break the GVN merge) | 932–944 | — | — | (11,444 B) |
| `nr_before` (`int m=this->needsRedraw; nextInt; …; volatile int d=m`) | 951 | — | — | (11,448 B) |
| `h_ri_after` (HEAD + `volatile int d2=radioIndex` after the anchor store) | 356 | — | 4002 |

## 3. What was learned

* **The wave-163 `Level *level` local is the only non-anchor construct
  that pins `%1052` to r9 with the original instruction count (4002) and
  exact size.**  `this->level` is a non-rematerialisable load; placing it
  before `getCurrentCampaignMission()` keeps it live across that call in
  r5, which stops the allocator splitting `%1052` into r5 and keeps it in
  r9 for the whole m23..m122 region.  Its whole residual is a 4-byte
  instruction move: the level load lands at `0x17f196` (before the ccm)
  instead of `0x17f19e` (after), which also swaps radioBase to r1 and
  gives `adds r2,r1,r4` instead of `add r2,r4; mov r0,r1` (16 raw,
  aligned 6, insn 4002:4002).
* A `Status *` local live only across the **ccm** does not pin r9: the
  compiler simply reloads `Globals::status` after the call
  (`st_after_ccm`, `h_ri_after` → plain 11,392 B).  The pin needs either
  (a) the GVN-merged m23 load kept live across `nextInt` (the anchor), or
  (b) a non-rematerialisable load (`this->level`) live across a call.
* Any second `*volatile` store of a global pointer alongside the anchor
  is free (the compiler coalesces the stores), so `two_*` shapes are
  byte-identical to HEAD.
* Breaking the GVN merge with a volatile clobber (`barrier_*`) costs
  4–6 B and lands at 11,444 B.
* `Status *st` used for the whole 23/24/122 chain (`st_all`) removes too
  many loads and grows the frame (11,460 B).
* Replacing `Globals::status` with a local in the m122 ccm (`sac`,
  `head_ccm_plain2`) moves the store between `movw` and the level load,
  which fuses `add r2,r4; mov r0,r1` into `adds r2,r1,r4` and keeps the
  size exact, but costs 17 raw bytes in the m122 tail (raw 19).

## 4. Conclusion

HEAD's 10 bytes remain the local optimum for this function.  The whole
residual is the anchor's r4 occupancy; the only shape found so far that
removes it is the wave-163 `Level *level` local, which trades the 10
bytes for a 16-byte instruction move (but is instruction-count-exact,
4002:4002, and aligned 6).

## 5. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13565/`
(`sw.py`, `b1.py`..`b17.py`, `body_w163.txt`, `onupdate_head.ll`,
`onupdate_sbn.ll`, `onupdate_t23.ll`, `mgame_head.ll`, `MGame.cpp.head`).
