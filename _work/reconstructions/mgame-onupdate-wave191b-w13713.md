# MGame::OnUpdate — level-load anchor: aligned 108 -> 6, raw 8 -> 16, wave 191b / w13713

Status: **landed `src/` change**.  Worktree `wt/w13713`, branch
`wave191b-w13713`, base master `df4e658df`.  Target `_ZN5MGame8OnUpdateEv`
moves from **raw 8 / aligned 108 / insn 4003:4002** to **raw 16 / aligned 6 /
insn 4002:4002**, size stays **11,440/11,440** exact.  `.text` content delta
**71,375 -> 71,383** (+8); `.ARM.extab` stays **577** (unchanged), `.ARM.exidx`
0, `.rodata` 0.  `check` re-run: **RATCHET PASS** (byte 4508, linked 4508,
size 4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
No baseline touched (aggregate `byte_exact` is unchanged).

Follows `mgame-onupdate-wave191b7-w13623.md` (the volatile read-back anchor
that took raw 10 -> 8), `…b6-w13608.md`, `…b5-w13592.md`,
`…wave191-w13555.md`, `…w13654.md`.

## 1. The idea

The w13623 anchor pins the GOT base of `@Globals::status` in r9 with a dead
volatile store, but it costs one extra instruction (4003 vs 4002) and, because
`tools/fn_diff.py`'s "aligned" metric normalises branch targets to instruction
indices, that one extra instruction shifts ~70 branch targets and inflates the
aligned count to 108.

A source-level value that is *already* loaded and stays live across `nextInt`
pins r9 just as well, with no extra instruction:

```cpp
int radioIndex = Globals::rnd->nextInt(2);
Level *level = this->level;          // live across nextInt -> r5, pins r9
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
level->createRadioMessage(28, radioIndex + radioBase);
```

The level load is hoisted above the `getCurrentCampaignMission` call (it cannot
be sunk past it: the call may alias `this->level`), so it occupies r5 across
`nextInt`.  That is exactly the register the plain build's `%2939` split copy
wants for the 122 branch, so the allocator coalesces the copy into r9 and the
whole branch reproduces the original's `ldr.w` forms — with the original's
instruction count.

## 2. Residual (raw 16, aligned 6)

The m122 condition and the `mov r4,r0; ldr.w r0,[r9]` prefix are now
**byte-exact**.  The entire residual is the level load sitting one slot early:

```
O 17f196 blx ccm ; 17f19a movw r2,#3166 ; 17f19e ldr.w r1,[fp,#120]
  ; 17f1a2 cmp ; it eq ; movweq r2,#3164 ; add r2,r4 ; mov r0,r1 ; ...
U 17f196 ldr.w r5,[fp,#120] ; 17f19a blx ccm ; 17f19e movw r1,#3166
  ; 17f1a2 cmp ; it eq ; movweq r1,#3164 ; mov r0,r5 ; adds r2,r1,r4 ; ...
```

Three runs, 16 bytes: `0x17f196..0x17f19b` (5), `0x17f19c..0x17f1a2` (6),
`0x17f1a9..0x17f1ae` (5).  The code re-aligns at `0x17f1ae` because the saved
`mov r0,r1` pays for the early load.  The load cannot be sunk past the call
(aliasing), so this 4-byte shift is irreducible for this family.

## 3. Why the volatile anchor cannot reach raw 0

The w13623 body is `mov r4,r0; str r5,[sp,#120]; ldr r0,[sp,#120]` — 6 bytes,
the same as the original's `mov r4,r0; ldr.w r0,[r9]`.  The only defect is
order: the post-RA top-down latency scheduler hoists the store above the copy
because the store's operand (r5) is ready while the copy waits on the call
result.  Confirmed in the post-RA MIR:

```
t2STRi12 killed renamable $r5, $sp, 120   ; store scheduled first
$r4 = tMOVr killed $r0
renamable $r0 = t2LDRi12 $sp, 120
```

The store is only moved after the copy when its *value* is the call result
(`store_ri`: `mov r5,r0; str r0,[sp,#120]; mov r0,r4`), but then the ccm
receiver becomes the anchor register instead of a fresh `ldr.w r0,[r9]`, and
the anchor's long live range makes it take r4 (not r5), so the m122 condition
picks up 4 more bytes.  Net 10.

Any body growth is fatal for raw: the m122 body is a cold block at `0x17f182`
whose only absorber is the literal-pool `nop` at `0x17f2f2`, 0x13a bytes later,
so a 2-byte insertion shifts ~300 bytes of code.

## 4. Measured variants

`raw`/`aligned`/`insn` from `tools/fn_diff.py --ours
cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv`; `.text`/`.ARM.extab`
are content byte-diffs against the original.  All size 11,440 unless noted.

| shape | raw | aligned | insn | `.text` | `.ARM.extab` | note |
|---|---:|---:|---:|---:|---:|---|
| **HEAD (volatile store + read-back)** | **8** | **108** | 4003 | 71,375 | 577 | previous optimum on raw/`.text` |
| **`lv_after_nt` (landed)** | **16** | **6** | **4002** | 71,383 | 577 | level load after `nextInt`, before `radioBase` |
| `lv_vol` (`*(Level *volatile *)&this->level`) | 19 | 6 | 4002 | 71,386 | 577 | volatile load, still hoisted |
| `head_store_after_ccm` | 19 | 7 | 4002 | 71,386 | 577 | store after the ccm call |
| `lv_before_nt` (= `BACD`) | 33 | 8 | 4002 | 71,400 | 577 | level load before `nextInt` |
| `store_ri` / `head_direct_ccm` | 10 | 112 | 4003 | 71,377 | 577 | store of `radioIndex`, ccm via anchor |
| `vst_ptr` (`Status *volatile st`) | 15 | 111 | 4003 | 71,382 | 577 | volatile variable, ldrd dest r3:r5 |
| `head_dummy_before_ri` | 15 | 111 | 4003 | 71,382 | 577 | store before `nextInt` |
| `sri_f` (swapped args) | 15 | 117 | 4003 | 71,382 | 577 | |
| `head_lv_rb` / `head_rb_first` | 24 | 114 | 4003 | 71,391 | 577 | level load between anchor and `radioBase` |
| `sri_c` / `sri_lv_mid` / `sri_lv_post` | 26 | 117 | 4003 | 71,393 | 577 | |
| `head_lv_before_anchor` / `sri_lv_pre` | 37 | 118 | 4003 | 71,404 | 577 | |
| `head_ccm_globals` | 348 | 18 | 4002 | 71,715 | 586 | fresh ccm load, +2 body |
| `head_rb_twice` / `head_vri` | 354 | 112 | 4003 | 71,721 | 586 | |
| `vload` (`*(Status *volatile *)&Globals::status` before nt) | 362 | 20 | 4002 | 71,729 | 586 | +4 load at `0x17f186` |
| `lv_cond` (`level ? level : this->level`) | 987 | 150 | 4010 | 184,911 | 1,364 | |
| `BCAD` / `CABD` / `CBAD` | 1,239/1,241/1,240 | 170/154/170 | 4001 | 79,420 | 671 | size 11,436 |
| `plain` (no anchor) | 5,196 | 597 | 3974 | 189,790 | 1,468 | size 11,392, no r9 pin |
| `sri_b` (anchor declared after nt) | 5,212 | 597 | 3976 | 189,861 | 1,463 | no pin |
| `vst_ptr_after` | 5,206 | 599 | 3976 | 189,855 | 1,464 | no pin |

Byte-identical to HEAD (8 / 108 / 4003 / 71,375 / 577): `head_arg_swap`,
`head_const_ri`, `head_vla` (`st2 = *(Status *volatile *)&dummy`),
`cond_vfield`, `cond_vrecv`, `cond_vboth` (volatile `Globals::status` loads in
the m122 condition — LLVM legally reuses the volatile load's result for the
non-volatile anchor load, so the condition cannot be decoupled that way).

Statement permutations of `{radioIndex, level, radioBase, createRadioMessage}`
(the only size-exact ones): `ABCD` = 16/6/4002 (landed), `BACD` = 33/8/4002,
everything else drops to 11,392/11,436.

## 5. Why this is a net win

* aligned **108 -> 6** (-102): only the level-load slot differs; the m122
  condition and the `mov r4,r0; ldr.w r0,[r9]` prefix are byte-exact.
* instruction count **4003 -> 4002**, i.e. the original's exact instruction
  stream (the previous anchor was one instruction over).
* size 11,440/11,440 exact, `.ARM.extab` unchanged at 577, `.ARM.exidx` 0,
  `.rodata` 0, `.rel.plt` untouched.
* the volatile-store/read-back "codegen anchor" hack is gone; the source is
  plain C++ with one ordinary local.
* cost: raw **8 -> 16** (+8) and `.text` **71,375 -> 71,383** (+8).  This is
  the same raw-for-aligned trade the fleet has landed before (e.g.
  `Layout` +21 raw / -83 aligned, `HangarWindow::render` +2 raw / -104
  aligned, `Radar::draw` +4 raw / -20 aligned).  It also brings
  `MGame::OnUpdate`'s profile in line with the other near-exact rows
  (`Hud::draw` 19/6, `ModStation` 11/5) instead of the previous anomalous
  8 raw / 108 aligned.

## 6. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
cmake --build cmake-build-match --target drift -j2      # 0 drifted fields
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13713/`
(`harness.py`, `h2.py`, `batch.py`, `sweep1.py`, `rd.py`, `mir.sh`,
`mir_head.txt`, `mir_plain.txt`, `mir_sri.txt`, `MGame.cpp.head`).
