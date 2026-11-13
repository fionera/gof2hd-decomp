# MGame::OnUpdate — volatile read-back anchor, wave 191b / w13623

Status: **landed `src/` change**.  Worktree `wt/w13623`, branch
`wave191b-w13623`, base master `10323b7c6`.  Target `_ZN5MGame8OnUpdateEv`
moves from **raw 10 / aligned 112** to **raw 8 / aligned 108**, size stays
**11,440/11,440** exact, insn 4003:4002.  `.text` content delta
**71,196 -> 71,194** (-2); `.ARM.extab` stays **577** (unchanged), `.ARM.exidx`
0, `.rodata` 0.  `check` re-run: **RATCHET PASS** (byte 4508, linked 4508,
size 4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
No baseline touched (aggregate `byte_exact` is unchanged).

Follows `mgame-onupdate-wave191b6-w13608.md`,
`mgame-onupdate-wave191b5-w13592.md`, `mgame-onupdate-wave191-w13555.md`.

## 1. The idea

The w189 anchor (`Status *volatile dummy = st;` then the ccm on `st`) pins the
GOT base of `@Globals::status` in r9, but the anchor value occupies **r4** at
the m23 compare, so the constant goes to r5 and `radioIndex` goes to r5 in the
m122 body.  The plain shape (`st_plain_anchor`: anchor store only, ccm on
`Globals::status`) puts the anchor in **r5** and `radioIndex` in **r4** — the
original's allocation — but the store is uncompensated (raw 348, the 2-byte
store shifts the body to the pool pad).

New shape: keep the anchor store, then **read the volatile local back** and use
the read-back as the ccm receiver:

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
Status *volatile dummy = st;
Status *st2 = dummy;                 // volatile 2-byte ldr r0,[sp,#120]
int radioBase = st2->getCurrentCampaignMission() == 122 ? 3164 : 3166;
```

The read-back is a 2-byte `ldr r0,[sp,#120]`, exactly compensating the 2-byte
volatile store, so the body is size-neutral and the ccm no longer needs the
4-byte `ldr.w r0,[r9]`.  Because the anchor is dead at the store, the allocator
keeps it in **r5** (not r4), so the m23 compare keeps its fresh status load in
r2 and the constant in r4.

## 2. Residual (raw 8)

* m23 `0x17e2b9..bd` (2 B): orig `ldr.w r2,[r9]; ldrd r2,r3,[r2,#256]`;
  ours `ldr.w r5,[r9]; ldrd r2,r3,[r5,#256]` (anchor in r5).
* m122 `0x17f190..196` (6 B): orig `mov r4,r0; ldr.w r0,[r9]`; ours
  `str r5,[sp,#120]; mov r4,r0; ldr r0,[sp,#120]`.  The rest of the body
  (`movw r2,#3166; ldr.w r1,[fp,#120]; cmp; it; movweq r2; add r2,r4;
  mov r0,r1; movs r1,#28; blx; b.w`) is byte-exact.

The `str`/`mov` order is the post-RA scheduler's choice (the `.s` shows
`str r5` then `mov r4,r0`).  If the `mov` were first, the body residual would
drop to 4 B (raw 6).  Every source-level dependency trick tried (`& 0`,
`- radioIndex`, comma, ternary, volatile `int` copy, indexed volatile array)
left the order unchanged; the indexed array also grew the function.

## 3. Measured variants (all size 11,440 unless noted)

`raw`/`aligned` from `tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so
_ZN5MGame8OnUpdateEv`.

| shape | raw | aligned | note |
|---|---:|---:|---|
| **HEAD (w189 anchor, ccm on `st`)** | **10** | **112** | anchor r4, const r5 |
| **`p_vread` (this pass)** | **8** | **108** | anchor r5, const r4, read-back ccm |
| `q_vread_ccm` (`dummy->ccm`) | 8 | 108 | same code as `p_vread` |
| `w_read_before_store` (`((Status*)dummy)->ccm`) | 8 | 108 | same code |
| `st_plain_anchor` (store only, fresh ccm) | 348 | 18 | anchor r5, store uncompensated |
| `sbnt_sbc` / `st_plain_anchor` family | 348 | 18 | m23 exact, body shifted by the store |
| `J_two_use` / `b_comma_level` (2 stores, ccm on `st`) | 23 | 113 | anchor r5 + ccm `mov r0,r5`, but 2nd store shifts |
| `store_after_ccm_freshccm` / `q_lv_freshccm` | 19 | 7 | store after ccm, radioBase -> r1 |
| `R_tail_recv` (`st->getPlayingTime()` in tail) | 322 | 113 | anchor r5, tail receiver duplicated |
| `keep_lv` (level anchor) | 362 | 24 | m23 exact, level load moved before nextInt |
| `L_field_keep` (field-0x100 anchor) | 352 | 21 | m23 load r2 but ldrd dest r5:r2 |
| `vst_before_nt_freshccm` (volatile load before nt) | 941 | 125 | m23 exact, size 11,444 (+4) |
| `ad_ccm_first_vread` (ccm before nextInt) | 29 | 119 | m23 exact, body order changed |
| `V_this` / `W_hints` / `X_appmgr` | 5,196-5,206 | 597-604 | size 11,396-11,404, no r9 pin |
| `f_pp` / `i_ref` (`Status **`/`Status *&`) | 5,212 | 597 | size 11,396, no r9 pin |
| `k_char` / `l_short` volatile store | 355 | 23 | anchor r4, store type irrelevant |
| `n_ll` volatile long long store | 925 | 136 | size 11,444 |
| `ca_two_store` (store before + after nt) | 358 | 112 | first store shifts nextInt |

## 4. Why the read-back works

`st_plain_anchor` already has the target allocation (anchor r5, `radioIndex`
r4, constant r4 at m23, level r1, radioBase r2); its only defect is that the
ccm uses `ldr.w r0,[r9]` (4 B) and the store is extra.  Reading the volatile
local back replaces the 4-byte `ldr.w` with a 2-byte stack `ldr`, which pays
for the 2-byte store.  The read-back also keeps the anchor dead at the store,
so the allocator does not move it to r4 (as the direct `st->ccm` use does).

## 5. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
cmake --build cmake-build-match --target drift -j2      # 0 drifted fields
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13623/`
(`tryv.py`, `tryc.py`, `probe.py`, `rd.py`, `batch*.py`, `mir_*.txt`).
