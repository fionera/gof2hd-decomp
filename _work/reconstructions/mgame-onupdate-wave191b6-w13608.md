# MGame::OnUpdate — store-position sweep and why the r9 pin cannot be made store-free, wave 191b / w13608

Status: **no landed `src/` change** (negative sweep).  Worktree `wt/w13608`,
branch `wave191b-w13608`, base master `05571f98b`.  Target
`_ZN5MGame8OnUpdateEv` stays at **raw 10 / aligned 112 / 11,440 B,
insn 4003:4002**, `.text` content delta **71,199 B**, `.ARM.extab` content
delta **577 B**, `.ARM.exidx` 0, `.rodata` 0, all section sizes exact.
`check` re-run at the end: **RATCHET PASS** (byte 4508, linked 4508, size
4524, data 843/843, imports 207, lint CLEAN, drift 0).  No baseline touched.

Follows `mgame-onupdate-wave191-w13555.md`, `…b2/b3/b4-w13565.md`,
`…b5-w13592.md`.

## 1. Residual (unchanged)

* cond `0x17e2b9..c5` (4 B): orig `ldr.w r2,[r9]; ldr r4,[pc](=3600000);
  ldrd r2,r3,[r2,#256]`; HEAD `ldr.w r4,[r9]; ldr r5,[pc];
  ldrd r2,r3,[r4,#256]` (`subs r0,r5` follows).
* body `0x17f190..1b1` (6 B): orig `mov r4,r0; ldr.w r0,[r9]; blx ccm; …
  add r2,r4`; HEAD `mov r5,r0; mov r0,r4; str r4,[sp,#120]; blx ccm; …
  add r2,r5`.

Both are the anchor's r4 occupancy (anchor = the m23 `Globals::status` load
`%142`, GVN'd into the ccm receiver and live across `nextInt`).

## 2. New hard constraint: any volatile store in the 122 body costs raw ≈ 300

The 122 body is a cold block at `0x17f182`; the next alignment `nop` is the
constant-pool pad at `0x17f2f2`.  A 2-byte `str rN,[sp,#120]` inserted
anywhere in the body shifts 0x13a..0x15c bytes of code before the pad is
deleted (the pad is the only absorber).  Measured:

| shape | raw | aligned | note |
|---|---:|---:|---|
| `sbnt_sbc` (`st` before `nextInt`, store before ccm, plain ccm+tail) | 348 | 18 | **m23 cond byte-exact, body byte-exact except the store** |
| `vd_before_nt` (`Status *volatile dummy = Globals::status;` before `nextInt`) | 361 | 21 | same, plus 2 `lsrs` RA diffs at 0x17f56c/70 |
| `st_merge` / `st_merge2` (store + a merged if) | 348 | 18 | identical to `sbnt_sbc` |
| `st_before_nt_store_before_ccm` | 348 | 18 | identical |

So the "ideal flip" (r5 anchor + store before ccm, raw ≈ 6) is not just
unreached — even if reached it is bounded by the 2-byte store shift unless
the store lands after the pool pad, which the 122 code path cannot do.

## 3. Store-free r9 pin is only the level-before-ccm family

`o_nt_lv_rb` (level declared before the `radioBase` ternary) is still the
only store-free shape that keeps `&Globals::status` in r9:

| shape | raw | aligned | insn | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|---:|---:|---:|
| HEAD anchor | **10** | 112 | 4003 | **71,199** | 577 |
| `q_lv` (store in the level-init comma) | **10** | 108 | 4003 | **71,199** | 577 |
| `o_nt_lv_rb` / `inline_ternary` / `lv_then_m` | 16 | **6** | **4002** | 71,205 | 577 |
| `st_sac` (`st` before nt, store after ccm) | 19 | 7 | 4002 | 71,208 | 577 |
| `q_before` | 19 | 7 | 4002 | — | — |
| `lv_before_nt` (level before `nextInt`) | 33 | 8 | 4002 | — | — |
| `st_before_nt_ccm_tail` | 37 | 109 | 4003 | — | — |
| `head_lv_nt` / `head_lv_nt2` (anchor + level before nt) | 37 | 118 | 4003 | — | — |
| `sbnt_sbc` | 348 | 18 | 4002 | 71,537 | 586 |
| `vd_before_nt` | 361 | 21 | 4002 | 71,550 | 586 |
| plain (no pin) | 5,196 | 597 | 3974 | — | — |

`o_nt_lv_rb` has a byte-exact cond and byte-exact `mov r4,r0` /
`ldr.w r0,[r9]`; its whole 16-byte residual is one 4-byte shift around the
level load (the load cannot be sunk past the `getCurrentCampaignMission`
call because of aliasing).  It is the closest *instruction* match
(aligned 6, insn 4002:4002) but **raw 16 > 10 and `.text` 71,205 > 71,199**,
so it is not a net win and was not landed.

`q_lv` reproduces HEAD's raw 10 and `.text` 71,199 exactly (aligned 108 vs
112) — a cosmetic tie, not a win.

## 4. New shapes measured this session

All size 11,440 unless noted.  `raw`/`aligned` from
`tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv`.

| shape | raw | aligned | insn | note |
|---|---:|---:|---:|---|
| `plain` | 5,196 | 597 | 3974 | 11,392 B (size-fatal) |
| `st_before_nt_tail` / `_nolv` | 318 | 19 | 4002 | body byte-exact, tail `mov r0,r5` |
| `st_before_nt_ccm_tail` | 37 | 109 | 4003 | |
| `st_after_nt_tail` | 926 | 127 | 4004 | 11,444 B |
| `lv_volatile` | 5,206 | 600 | 3976 | 11,396 B |
| `lv_ref` / `lv_const_ref` / `lv_ptr` | 5,196 | 597 | 3974 | 11,392 B |
| `lv_before_nt` | 33 | 8 | 4002 | |
| `base_const_hoist` / `expect` | 5,196 | 597 | 3974 | 11,392 B |
| `lv_and_st` | 334 | 25 | 4002 | |
| `lv_before_nt_st` | 351 | 26 | 4002 | |
| `st_store_tgt` | 5,305 | 761 | 4020 | 11,492 B |
| `stp` / `stref` | 5,196 | 597 | 3974 | 11,392 B |
| `inline_ternary` / `inline_ternary_lv` | 16 | 6 | 4002 | = `o_nt_lv_rb` |
| `inline_ternary_lv2` | 33 | 8 | 4002 | |
| `cond_first` | 5,196 | 597 | 3974 | 11,392 B |
| `st_after_ccm_tail` | 912 | 125 | 4003 | 11,444 B |
| `st_after_ccm_ccmuse` | 926 | 127 | 4004 | 11,444 B |
| `lv_nt_plain` / `lv_nt_ccm_st` | 33 | 8 | 4002 | |
| `two_calls_level` | 921 | 138 | 4007 | 11,452 B |
| `st_volatile_local` | 5,331 | 761 | 4023 | 11,500 B |
| `vstore_after_nt` / `vstore_direct_after_nt` | 5,212 | 597 | 3976 | 11,396 B |
| `vstore_direct_before_nt` | 361 | 21 | 4002 | = `vd_before_nt` |
| `vstore_after_ccm` | 5,205 | 601 | 3976 | 11,396 B |
| `vstore_after_call` | 5,211 | 597 | 3976 | 11,396 B |
| `vstore_before_nt_plus_lv` | 361 | 21 | 4002 | |
| `vstore_after_nt_ccmst` | 5,212 | 597 | 3976 | 11,396 B |
| `st_before_nt_store_after_ccm` | 19 | 7 | 4002 | = `st_sac` |
| `st_before_nt_store_before_ccm` | 348 | 18 | 4002 | = `sbnt_sbc` |
| `st_before_nt_store_before_nt2` | 361 | 21 | 4002 | |
| `tail_ll` / `tail_ll2` | 5,196 | 597 | 3974 | 11,392 B |
| `tail_via_field` | 5,460 | 940 | 3988 | 11,428 B |
| `lv_ll` | 16 | 6 | 4002 | = `o_nt_lv_rb` |
| `lv_before_nt_ll` / `lv_before_nt_pt` | 33 | 8 | 4002 | |
| `dead_st` / `dead_lv` / `dead_lv2` / `lv_cse` | 5,196 | 597 | 3974 | dead locals eliminated |
| `lv_cse2` | 9,490 | 1,171 | 3950 | 11,364 B |
| `dead_cond` | 9,483 | 1,164 | 3949 | 11,360 B |
| `st_before_nt_deadstore` | 5,196 | 597 | 3974 | 11,392 B |
| `st_before_nt_use_in_cond` | 918 | 109 | 4001 | 11,436 B |
| `lv_before_nt_use` | 33 | 8 | 4002 | |
| `lv_before_nt_use2` | 945 | 140 | 4007 | 11,452 B |
| `ll_ri` / `ll_rb` / `ll_both` | 5,196 | 597 | 3974 | 64-bit locals folded |
| `ll_lv` | 16 | 6 | 4002 | = `o_nt_lv_rb` |
| `ll_lv2` | 5,196 | 597 | 3974 | 11,392 B |
| `lv_before_nt_llrb` | 33 | 8 | 4002 | |
| `merge_if` / `loop1` / `scope` | 5,196 | 597 | 3974 | CFG folded |
| `st_merge` / `st_merge2` | 348 | 18 | 4002 | |
| `lv_merge` | 5,199 | 604 | 3978 | 11,404 B |
| `head_lv_nt` / `head_lv_nt2` | 37 | 118 | 4003 | level → r8 |
| `head_lv_after_nt` | 25 | 117 | 4003 | |
| `head_store_after_ccm` | 915 | 110 | 4001 | 11,436 B |
| `head_store_after_call` | 926 | 129 | 4004 | 11,444 B |
| `head_ccm_globals` | 348 | 18 | 4002 | |
| `m_then_lv` / `m_then_lv2` / `lv_after_m_plain` | 5,196 | 597 | 3974 | 11,392 B |
| `lv_then_m` | 16 | 6 | 4002 | = `o_nt_lv_rb` |
| `m_before_nt` | 1,241 | 154 | 4001 | 11,436 B |
| `q_lv` | 10 | 108 | 4003 | tie with HEAD |
| `q_lv_b` / `q_rb` / `q_arg` | 344/348/342 | 18/18/20 | 4002 | |
| `q_before` | 19 | 7 | 4002 | |
| `q_lv_nt` / `_nt2` / `_nt3` / `_nt4` | 926 | 126–128 | 4004 | 11,444 B |
| `s_after_ccm_comma` / `_sep` | 5,211 | 603 | 3976 | 11,396 B |
| `s_after_lv` / `s_after_ccm_comma2` | 5,205 | 603 | 3976 | 11,396 B |
| `vri` / `vri2` / `st_and_vri` | 5,206/5,212 | 597–604 | 3976 | 11,396 B |
| `lv_vri` / `lv_vri2` | 351/345 | 22 | 4002 | |
| `flip1` / `flip2` | 345 | 22 | 4002 | |
| `flip3` / `flip4` | 926/942 | 128/135 | 4004/4005 | 11,444/11,448 B |
| `store_after_tail` | 5,312 | 758 | 4020 | 11,492 B |
| `store_after_tail2` | 5,460 | 940 | 3988 | 11,428 B |
| `store_before_tail` | 898 | 127 | 4003 | 11,444 B |
| `st_vol_store_before_nt` | 361 | 21 | 4002 | = `vd_before_nt` |
| `st_vol_store_after_nt` / `_after_ccm` / `_after_call` | 5,212/5,211 | 597–603 | 3976 | 11,396 B |
| `st_vol_after_ccm_tailst` | 922 | 130 | 4003 | 11,444 B |
| `st_vol_before_nt_tailst` | 932 | 128 | 4004 | 11,444 B |
| `st_vol_before_nt_tailstatus` | 361 | 21 | 4002 | |
| `st_before_nt_store_after_ccm` (`head_lv_after_nt`-family) | 19 | 7 | 4002 | |

Whole-function `st` declared *before the if-chain* and stored after it (in
the hints block or just before it) changes the global allocation and grows
the function: `+100 B` (raw 6,363 / aligned 2,831) and `+80 B`
(raw 6,332 / aligned 2,853) respectively.

## 5. Conclusion

* Any volatile store in the 122 body costs raw ≈ 300 because the only
  absorber is the pool pad at `0x17f2f2`.
* The only store-free r9 pin is the level-before-ccm family (`o_nt_lv_rb`,
  raw 16), whose 4-byte level-load shift cannot be removed (aliasing).
* HEAD's raw 10 / `.text` 71,199 is the local optimum in raw **and** in
  `.text` content.  `q_lv` ties it (10 / 71,199, aligned 108).
* Nothing was landed; `src/` is clean.

## 6. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13608/`
(`harness.py`, `batch_w13608*.py`, `MGame.cpp.head`, `body_*.txt`,
`mir_*.txt`, `mg*.ll`).
