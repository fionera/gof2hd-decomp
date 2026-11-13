# MGame::OnUpdate — anchor register flip and the 2-byte insertion wall, wave 191b / w13565 (fourth note)

Status: **no landed `src/` change** (negative sweep).  Worktree `wt/w13565`,
branch `wave191b-w13565`, base master `113a6f7a0`.  Target
`_ZN5MGame8OnUpdateEv` stays at **raw 10 / aligned 112 / 11,440 B,
insn 4003:4002** (the w189 anchor shape).  `check` re-run this session:
**RATCHET PASS** (byte 4507, linked 4507, size 4524, data 843/843,
imports 207, lint CLEAN, drift 0).  No baseline touched.

Follows `mgame-onupdate-wave191-w13555.md`,
`mgame-onupdate-wave191b-w13565.md`, `…b2…` and `…b3…`.

## 1. The residual is entirely inside the `>= 122 && <= 124` branch

The two mismatches are **the same branch**: "m23" is the 122-branch
*condition* (`0x17e27e..0x17e2de`) and "m122" is its *body*
(`0x17f182..0x17f1b4`).  The body sits `0xea4` bytes after the condition
because the one-time-hint section is the fall-through of the if-chain.

* cond `0x17e2b9..c5` (4 B): orig `ldr.w r2,[r9]; ldr r4,[pc](=3600000);
  ldrd r2,r3,[r2,#256]`; HEAD `ldr.w r4,[r9]; ldr r5,[pc];
  ldrd r2,r3,[r4,#256]` (`subs r0,r5` follows).
* body `0x17f190..1b1` (6 B): orig `mov r4,r0; ldr.w r0,[r9]; blx ccm; …
  add r2,r4`; HEAD `mov r5,r0; mov r0,r4; str r4,[sp,#120]; blx ccm; …
  add r2,r5`.

## 2. Why the plain shape is 48 B short (new evidence)

`fd2`-style structural diff of the **plain** body against the original
shows the entire `11,392` deficit comes from the `@Globals::status`
GOT-entry address:

* original: keeps it in `r9` from `0x17e088` to `0x17e31c`, then
  `str.w r9,[sp,#36]` and **rematerialises** `ldr r0,[sp,#36];
  ldr r0,[r0]` (6 B) at ~24 sites from `0x17e86a` to `0x17f3a4`, reusing
  `r9` for `this` (`str.w fp,[sp,#32]` / `ldr.w r9,[sp,#32]`).
* plain: keeps the same value in `r5` for the whole function, so every
  use is a 4 B `ldr.w rX,[r9]`; the `[sp,#36]` slot is freed and later
  reused for `this`.

24 × 2 B = the 48 B.  The anchor reproduces the original's
spill/rematerialise pattern exactly: HEAD has **exactly one structural
difference** from the original (the 3-instruction body substitution);
every other difference is a same-size substitution.

## 3. The anchor register flip (new)

The anchor's register is `r4` iff its **last use is before** the body's
`getCurrentCampaignMission()` call; it becomes `r5` iff the last use is
after that call.  The store position controls this, and the store
position is also what the byte layout needs — the two are coupled:

| anchor last use | reg | body layout | raw |
|---|---|---|---|
| store before the call (`p0`) | r4 | `mov r5,r0; mov r0,r4; str r4` | 10 |
| store after the call | r5 | `mov r4,r0; mov r0,r5; …; str r5` after `cmp`/`blx` | 20–25 |

Ideal flip (never reached): anchor in `r5`, field read uses it, store
before the call → cond 2 B + body 4 B = **raw 6**.

## 4. The 2-byte insertion wall (new)

The function's alignment `nop`s are at `0x17cb82 0x17cd5e 0x17e05a
0x17e3f2 0x17e79e 0x17f2f2`.  Any 2 B *insertion* in the body is
absorbed by the `nop` at `0x17f2f2` (`≈0x140` bytes later), so the size
stays 11,440 but raw jumps to ≈320–370.  Any 2 B insertion in the
condition is absorbed by `0x17e3f2` (`≈0x140` bytes later), same cost.
**Only a same-size substitution keeps raw small.**  The only such
substitution available is `mov r0,rX; str rX,[sp,#120]` (4 B) for
`ldr.w r0,[r9]` (4 B).

## 5. Measured shapes this session (all 11,440 B unless noted)

`raw`/`aligned` from `tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv`.

| shape | raw | aligned | notes |
|---|---:|---:|---|
| **HEAD anchor** | **10** | 112 | cond 4 B + body 6 B |
| **`q_lv`** `Status *st; Status *volatile dummy;` + `Level *level = (dummy = st, this->level);` | **10** | **108** | cond **2 B** (anchor r5, field read uses it), body 8 B (store lands after `blx`, before `movw`) |
| `v_lt` (`3600000 < getPlayingTime()-field`) + anchor | 10 | 112 | byte-identical to HEAD |
| `s_base` (comma store inside `radioBase`) | 10 | 112 | byte-identical to HEAD |
| `u_inline_early` (`this->level->createRadioMessage`) | 10 | 112 | byte-identical to HEAD |
| `w_st2` (`Status *st2 = st;`) | 10 | 112 | coalesced |
| `r_comma0`/`r_cmp0`/`r_cmp0b` (dead extra `st` uses in the arg) | 10 | 112 | optimised away |
| `st_vp`/`st_vi`/`st_vu` (store type variants, early) | 10 | 112 | |
| `q_nt` (comma store inside `nextInt`) | 15 | 112 | store before `nextInt` |
| `q_arg` (comma store in the `createRadioMessage` arg) | 20 | 110 | `movw`/`ldr` swap |
| `vrecv_p2` = `vl_sal` (store after `Level`) | 20 | 110 | anchor r5, store after `cmp` |
| `z_01`, `y_s2_lv` (two stores, 2nd between `radioBase` and `level`) | 23 | 113 | `movw r1`, `ldr r0` direct |
| `s_lvfirst` (`level` first, comma store in it) | 25 | 117 | level hoisted to r8 |
| `VR_early`/`rrecv` (`(st = Globals::status)->getPlayingTime()`) | 327 | 132 | anchor r5, **body byte-exact**, +2 `mov r0,r5` at `0x17e2b2` |
| `x_two_store` (store early + store after `level`) | 346 | 114 | anchor r5, body prefix exact, `movw`/`ldr` swap |
| `w_comma_level` (store early + comma store in `level`) | 348 | 112 | anchor r5, body perfect except a **duplicate** `str r5,[sp,#120]` |
| `t_c2` (anchor at the `<=124` receiver) | 359 | 132 | |
| **`cond_head_S`** (`(st = Globals::status)->getCurrentCampaignMission() >= 122`) | 370 | 132 | anchor r5, **cond AND body byte-exact**, +2 `mov r0,r5` at `0x17e282` |
| `t_c3` (anchor at the `getMission` receiver) | 838 | 162 | |
| `q_nt` family / `p1` (store between `radioBase` and `level`) | 915 | 110 | **11,436 B** (size-fatal) |
| `st_pv`/`st_vpp` | 918 | 109 | 11,436 B |
| `st_vll` | 925 | 136 | 11,444 B |
| `A_vrecv_G_tail` (tail `field_0x100 = …` via `st`) | 1003 | 134 | 11,456 B (tail duplicated) |
| `D_vrecv_G_tail0` | 5198 | 601 | 11,404 B |
| `v1..v7` (two `*volatile` stores of `st`) | 5210–5253 | 566–719 | **11,392 B** (pin lost) |
| plain (no anchor) | 5196 | 597 | 11,392 B |

## 6. What was learned

* `cond_head_S` is the closest **instruction-stream** match found so far:
  both the condition and the body are byte-exact; the whole residual is
  the 2 B `mov r0,r5` that defining the anchor at a call receiver always
  costs.  That 2 B is absorbed by the `nop` at `0x17e3f2` but shifts
  `0x140` bytes → raw 370.
* `q_lv` is the closest **raw** alternative to HEAD (raw 10, aligned 108,
  cond 2 B instead of 4 B) but pays 8 B in the body because the store
  lands after `blx`.
* Keeping the anchor's value live past the body's `ccm` call flips it to
  `r5`, but every construct that does so either moves the store into the
  wrong slot (8–18 raw B) or needs a second store (2 B insertion wall).
* A `Status *` local live only across the `ccm` call does not pin `%1053`
  (11,392 B); the pin needs the anchor live across `nextInt` (or the
  wave-163 `Level *level` local live across `ccm`).

## 7. Conclusion

HEAD's 10 bytes remain the local optimum.  The ideal flip (raw 6) needs
the anchor in `r5` with the store before the `ccm` call; the greedy
allocator only produces `r5` when the last use is *after* that call.
No source shape found breaks the coupling.

## 8. Commands

```bash
cp /home/debian/GalaxyOnFire2/scratch/w13565/MGame.cpp.head src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13565/`
(`h.py` harness; `e1.py`..`e26.py`; `rawdiff.py`; `fd2.py`; `dump.py`;
`mir.sh` + `mir_store.mir`/`mir_sal.mir`, the pre-RA MIR of the two
store positions — identical except for the `t2STRi12` slot).
