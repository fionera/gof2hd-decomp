# `Radar::draw` — LSDA pass (wave 192, w13911)

Target: `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Size 9,908 B (exact). Base master `0b36ac59e`.

Third independent Radar agent, on the **LSDA** angle: decode the original's
call-site table (extab offset `0x1c203c`, LSDA record base `0x12e050`, 132 B)
and ours, diff the ranges, and fix the `try`/`catch` scope boundaries that
produce them.  `.text` clusters are w13893/w13901 territory and were not
touched.

## Landed change: reload layout row height in the lock-label arm

The original reloads `Globals::layout->field_0x4` *inline* for the `i != 0`
lock-label `DrawString` (`12e974`: `ldr r0,[r4,#252]; ldr r2,[r0,#4]`), and
loads it separately inside the `i == 0` tech arm.  The source cached it once
before the `if (i <= 1)` block.  Fix:

```cpp
if (i <= 1) {
    if (i == 0 && !alienOrbit) {
        int rowHeight = Globals::layout->field_0x4;   // tech arm
        ...
    } else {
        ...
        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                                    this->screenY + Globals::layout->field_0x4, false);  // inline reload
    }
}
```

Effect: the 132 B LSDA record diff drops (`extab_map.py`: 88 -> 43 differing bytes), call-site table length
is now **112/112** (19 rows both).

## Measurements

Measured on master `b674b8969` (the original agent's numbers were on the older
base `0b36ac59e`, where `.text` was 76,882):

| metric | before | after |
|---|---:|---:|
| raw | 6,909 | **6,945** (+36) |
| aligned | 1,301 | **1,322** (+21) |
| size | 9,908 | **9,908** exact |
| `.text` contentdiff | 76,875 | 76,911 (+36) |
| `.ARM.extab` contentdiff | 597 | **552** (-45) |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 | 0 / 0 / 0 |
| LSDA record (differing bytes, `extab_map.py`) | 88 | **43** |
| call-site table | 111 | **112 / 112** |

Net section bytes: `.text` +36, `.ARM.extab` -45 -> **-9** overall, so the
whole-file diff drops 77,492 -> **77,483**.  `.rel.plt` stays byte-identical
(29,392 B).  Full `check` green (byte 4508, linked 4508, stubs 0, missing 0,
extra 0, data 843/843, imports 207, lint CLEAN).

## Call-site table rows (candidate vs original)

Row fields: (start, length, landing-pad, action).  Rows 0/16/18 match; the
divergence is confined to the `if (i <= 1)` tech/lock-label block, where our
code sits ~44 B earlier:

```
 1 orig (568,8,9818)   ours (568,8,9810)
 2 orig (576,1818,0)   ours (576,1774,0)
 3 orig (2394,10,9856) ours (2350,12,9854)   <- 10 B vs 12 B cleanup range
 4 orig (2404,158,0)   ours (2362,144,0)
 5 orig (2562,10,9852) ours (2506,10,9850)
 6 orig (2576,16,9858) ours (2520,20,9856)
 7 orig (2592,12,9842) ours (2540,10,9840)
 8 orig (2604,16,9832) ours (2550,12,9830)
 9 orig (2638,20,9822) ours (2574,20,9820)
10 orig (2658,78,0)    ours (2594,76,0)
11 orig (2736,14,9820) ours (2670,10,9812)
12 orig (2750,1256,0)  ours (2680,1338,0)    <- cumulative code-size shift
13 orig (4006,8,9872)  ours (4018,8,9868)
14 orig (4014,1796,0)  ours (4026,1784,0)
15 orig (5810,8,9870)  ours (5810,8,9874)
17 orig (6606,8,9816)  ours (6606,8,9808)
```

Rows 12-17 differ only by the cumulative code-size shift; rows 5-11 cover the
landing-pad destructor calls in the tech arm.

## Landing-pad chain

Original (fn-relative, base `0x12e050`):

- pad A `1306a4` (`adds r7,r1,#2; lsls r4,r7,#8; b.n 1306e0`)
- pads B/C `1306aa`/`1306ac` both `b.n 1306e0`
- LP1 `1306ae` (sp+232) -> LP2 `1306ba` (sp+196) -> LP3 `1306c4` (sp+184)
  -> LP4 `1306d4` (sp+220) -> LP5 `1306da` (sp+208) -> LP6 `1306e0` (sp+232)
  -> `_Unwind_Resume`
- `.word 0x000cb1d2` at `130700`

Ours has one EXTRA landing pad at fn+`0x268c` (`mov r4,r0; add r0,sp,#220;
b.n 2696`) — a call site whose live-temp set is `{sp+220}` alone, and no
`adds/lsls` pad A.  Pool word differs (`0x023c1c8f` vs `0x000cb1d2`).

Tech-arm temp layout ours: `getText(133)`->r6; `StringC1(sp+208,": ",false)`;
`pl(sp+220,r6,sp+208)`; `Status::getStation`; `getTecLevel`;
`StringC2(sp+184,tec)`; `StringC1RKS0b(sp+196,sp+184,false)`;
`pl(sp+232,sp+220,sp+196)`; DrawString; destroys 232,196,184,220,208.
Construction order 208,220,184,196,232 — no obvious `{220}`-only source scope.

## Original disasm evidence

- `12e922` loads `fp` pc-relative once; `12e924 mov.w r9,#560; 12e92c cmp.w
  sl,#0; it eq; moveq.w r9,#556` -> offset = i!=0 ? originY : originX;
  then `12e942 ldr.w r5,[r4,r9]`.
- Label DrawString `12e95a`: `r3 = screenX + r5`, `str r6 [sp,#0]` (screenY),
  `str #0 [sp,#4]`.
- `12e95e: cmp.w sl,#1; bhi.n 12ea1e` — `i<=1` gate.
- `12e964..12e972`: combined `(i!=0 | alienOrbit)==0` test via single
  `orrs.w r0,r0,r8`.
- Else arm `12e974`: calcDistance into `sp+232`, `String::operator=` into
  `[sp+104]` (lockLabel), `D1(sp+232)`, then DrawString reloading offset
  `ip=[r4,r9]`, `ldrd r3,r5,[r4,#252]`, `r2 = Globals::layout->field_0x4`,
  `r2 += screenY`, `str r2,[sp,#0]`, `r3 += ip`, `r1=[fp]`.
- Tech-arm second DrawString `12eb14..12eb36`: reloads `[r4+556]`,
  `add.w r2, r6, r2, lsl #1` (rowHeight*2), `mov r2,r9` (lockLabel at sp+104).

## Variant table (all textsize 9,908 unless noted)

| variant | textsize | extabdiff | cst |
|---|---:|---:|---|
| baseline HEAD | 9908 | 92 | 111 |
| a_alllocal | 9896 | 91 | fatal -12 |
| b_v2_l470 | 9892 | 92 | fatal -16 |
| **c_v2_declin (landed)** | **9908** | **47** | **112** |
| c3_v2_useheight | 9904 | 49 | 112 fatal -4 |
| c4_scope_useheight | 9892 | 92 | 111 fatal -16 |
| c5_late_decl | 9900 | 49 | 112 fatal -8 |
| e_v2_notech | 9912 | 46 | fatal +4 |
| f_v3_l470 | 9900 | 91 | fatal -8 |
| v1_norowheight | 9900 | 49 | 112 fatal -8 |
| v2_elseinline | 9912 | 46 | 112 fatal +4 |
| v3_i0inline | 9900 | 92 | 111 fatal -8 |
| h1 (rowHeight before `if`, else reloads layout) | 9912 | 46 | fatal +4 |
| i1_namedlocal (`String techText = ...; DrawString(techFont, techText, ...)`) | 9924 | record 144 B (cst 124/21 rows) | fatal +16 |
| i2_teclocal (`String tecLevel(...)`; concat inline) | 9920 | 152 B (cst 129/22 rows) | fatal +12 |
| j1_regroup (`getText(133) + (": " + tec)`) | 9888 | — | fatal -20 |

## Remaining structural divergences (w13893/w13901 territory — not touched)

Register cascade, `(i-2)<=253` vs `cmp.w sl,#2; blt`, landmarks spilled
`sp+132` vs `sl`, crosshair-hit predication, Canvas `sp+88` vs `sp+96`,
originY `sp+104`, rowHeight cached in `r8`, fonts `r5`/`r9`.  The residual
row-3 10 B vs 12 B cleanup range and the extra `{sp+220}` landing pad remain
open on the LSDA side.
