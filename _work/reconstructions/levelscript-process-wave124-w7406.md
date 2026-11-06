# LevelScript::process — wave 124 / w7406 (EH-pad probe + two fidelity fixes)

Status: **code change landed, 2026-11-06**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `6f8ee2320` (wave-123 final). Owned TU:
`src/game/world/LevelScript.cpp`.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.256665340      60.250192743     (-0.006472597)
official verify pct      60.3              60.3             (same, rounded)
orig / ours insns        20169 / 20039     20169 / 20040
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54640             54619            (-21)
whole .text diffbytes    87014             86993            (-21)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
.ARM.exidx diffbytes     0                 0                (byte-identical)
.ARM.extab diffbytes     1446              1446             (size/base exact)
.rel.plt diffbytes       4                 4
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so` (helper `/dev/shm/w7406/score.py`).  The raw ratio dips by 0.0065
(≈1.3 instructions) but the whole-file `.text` byte delta improves by 21 B and
the official rounded pct is unchanged (60.3).

## Landed changes (case 157, `m_nState == 11` / supernova tail)

Two independent fidelity fixes in the `m_nScriptTimerA >= 5700` / `>= 6500`
blocks (source lines ~5110 and ~5128):

1. **5700 block second `setVisible` target.**  The original's second call is
   `KIPlayer::setVisible` on `(*enemies)[stationIndex]`, not
   `AEGeometry::setVisible` on `m_pGeometry2`:

   ```diff
                        if (m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700) {
                            m_pGeometry1->setVisible(false);
   -                        m_pGeometry2->setVisible(false);
   +                        (*enemies)[stationIndex]->setVisible(false);
   ```

   Evidence — original `0x145a70..0x145a84`:

   ```
   145a70: ldr.w r0, [r8, #188]          ; m_pGeometry1
   145a74: movs  r1, #0
   145a76: blx   AEGeometry::setVisible(bool)
   145a7a: ldr   r0, [sp, #96]           ; enemies
   145a7c: movs  r1, #0
   145a7e: ldr   r0, [r0, #4]
   145a80: ldr.w r0, [r0, r9, lsl #2]    ; (*enemies)[stationIndex]
   145a84: blx   KIPlayer::setVisible(bool)
   ```

   (The *other* half of the swap — the `m_nState == 11` block at
   `0x145430..0x145440`, where the original calls `AEGeometry::setVisible` on
   `m_pGeometry2` @0xc0 — was measured as variant "A" below.  It is faithful
   but costs more than it gains: A+B = 60.2054 with process `.text` diffbytes
   55,114.  It is left for a later pass.)

2. **Bogus `m_bCinematicBreak = 1` in the 6500 block.**  The original goes
   straight from `str [r1,#28]` (m_nState) to `nextCampaignMission`; there is
   no `m_bCinematicBreak` store there.  The store the original does have is
   `strb.w r4, [r9,#17]` at `0x145002` (a `m_bCinematicBreak = 0` in the
   `m_nState == 12` block), which our source already matches.

   ```diff
                        if (m_nScriptTimerA >= 6500) {
   -                        m_bCinematicBreak = 1;
                            m_nState++;
   ```

Both edits add/remove no call, symbol or literal; `.rel.plt` stays at the
baseline 4-byte delta and the body stays exactly 57232 B.

## EH landing pads — the assignment's focus

### The 14 late `mov sl,r0` cleanups are a *whole-function* allocation artifact

The pad block (`0x145bf0..0x145c6c` original, `0x145bf4..0x145c6c` ours) is
the 24-entry landing-pad region shared by all 46 LSDA call-site rows.  The
14 pads that copy the exception object and run a cleanup are:

| orig addr | action | ours addr | action |
|---|---|---|---|
| 0x145bf2 | `mov sl,r0; mov r0,fp` | — | **missing** (ours uses r6) |
| 0x145bfa | `mov sl,r0; add r0,sp,#124` | 0x145bfa | `mov r9,r0; add r0,sp,#124` |
| 0x145c04 | `mov sl,r0; mov r0,r4` | 0x145c02 | `mov r9,r0; mov r0,r4` |
| 0x145c0a | `mov sl,r0; add r0,sp,#220` | 0x145c08 | `mov r9,r0; add r0,sp,#220` |
| 0x145c10 | `mov sl,r0; add r0,sp,#112` | 0x145c10 | `mov r9,r0; add r0,sp,#112` |
| 0x145c1a | `mov sl,r0; ldr r0,[sp,#72]` | 0x145c18 | `mov r9,r0; ldr r0,[sp,#72]` |
| 0x145c22 | `mov sl,r0; add r0,sp,#232` | 0x145c24 | `mov r9,r0; add r0,sp,#232` |
| 0x145c2c | `mov sl,r0; mov r0,r8` | 0x145c2e | `mov r9,r0; mov r0,r8` |
| 0x145c34 | `mov sl,r0; mov r0,r6` | 0x145c34 | `mov r9,r0; mov r0,r6` |
| 0x145c3a | `mov sl,r0; mov r0,r5` | 0x145c3a | `mov r9,r0; mov r0,r5` |
| 0x145c44 | `mov sl,r0; add r0,sp,#136` | 0x145c44 | `mov r9,r0; add r0,sp,#136` |
| 0x145c4e | `mov sl,r0; add r0,sp,#268` | 0x145c4e | `mov r9,r0; add r0,sp,#268` |
| 0x145c58 | `mov sl,r0; add r0,sp,#368` | 0x145c58 | `mov r9,r0; add r0,sp,#368` |
| 0x145c62 | `mov sl,r0; add r0,sp,#280` | 0x145c62 | `mov r9,r0; add r0,sp,#280` |

The String stack slots are **identical** on both sides (`sp+112, +124, +136,
+220, +232, +268, +280, +368`), so the EH *scope structure* is faithful.  The
only differences are (a) the scratch register (`sl` vs `r9`) and (b) the
order in which the pads are laid out — which is the address order of the
throwing regions, i.e. a MachineBlockPlacement consequence, not a separate
EH-scope difference.

The scratch register is the phi of the exception object in the resume block
`bb.2085` (`$r0 = tMOVr killed $r9; tBL @_Unwind_Resume` in the final
MachineIR, `/dev/shm/w7406/afterall.txt` line 6039629).  In the original the
phi coalesces to `sl`; in ours to `r9`.  Root cause (unchanged from waves
121–123): the original spills `this` to `[sp,#88]` (328 refs) and keeps `sl`
free, while ours keeps `this` live in `sl` (196 refs) and puts
`&Globals::status` in `r9`.  The allocator picks the first free callee-saved
register at the pad; with `this` live in `sl`, that is `r9`.

### Variants measured this session (all reverted unless noted)

| variant | raw % | size | pads | process .text diffbytes |
|---|---:|---:|---|---:|
| **baseline** | **60.256665340** | 57232 | r9 | 54640 |
| **B+C (landed)** | **60.250192743** | **57232** | r9 | **54619** |
| A only (m_nState==11 setVisible target) | 60.098987738 | 57228 | r9 | 55182 |
| B only (5700 setVisible target) | 60.164138274 | 57236 | r9 | 54680 |
| A+B | 60.205426646 | 57232 | r9 | 55114 |
| A+B+C | 60.092518902 | 57228 | r9 | — |
| A+C | 60.090533751 | 57224 | r9 | — |
| C only (drop `m_bCinematicBreak=1`) | 60.113910513 | 57228 | r9 | — |
| B+C + 5700 calls swapped order | 60.240244721 | 57232 | r9 | — |
| B+C + case-125 `field_0x74` first | 59.512971669 | 57216 | r9 | — |
| B+C + case-42 `objectivesB` null-first | 59.466852340 | 57240 | r9 | — |
| B+C + `String("")` (3 sites) | 60.250192743 | 57232 | r9 | neutral |
| B+C + `delete m_pGeometry6` null-first | 60.250192743 | 57232 | r9 | neutral |

### String / delete scoping probes (assignment hint) — all reverted

| variant | raw % | size | pads |
|---|---:|---:|---|
| case-125 loop faithful order (`setName`, `setActive`, `field`) | 59.518479829 | 57224 | r9 |
| case-125 loop `setName` in an explicit `{ String name; }` scope | 59.418740522 | 57256 | r9 |
| case-125 `port` declared outside the loop | 60.256665340 | 57232 | r9 (neutral) |
| case-125 `i<11` / `i!=11` / `i-8<3` | 60.256665340 | 57232 | r9 (neutral) |
| case-42 `objectivesB`/`objectivesA` null-first | 59.473304653 | 57240 | r9 |
| case-91 `port` local | 60.256665340 | 57232 | r9 (neutral) |
| case-91 `String name(...)` local | 59.473841257 | 57252 | r9 |
| `delete frame` wrapped in an explicit scope | 60.256665340 | 57232 | r9 (neutral) |
| faithful prologue `messages,player,enemies` | 58.783430775 | 57416 | **r1** |
| faithful prologue + A+B | 58.810358982 | 57420 | r1 |
| `Level *level = m_pLevel` body-wide | 51.318049436 | 57016 | r4 |
| case-125 `if (state==1)` → `else if` | 58.006913875 | 57248 | r9 |
| case-125 `else if` for state 1 **and** 2 | 50.504397953 | 57356 | r1 |
| epilogue `if (Globals::status == nullptr) return false;` | 53.688065167 | 57432 | r1 |
| epilogue `if (player2 == nullptr) return false;` | 46.042413381 | 57476 | r1 |

Key negative results:

* **The pad register cannot be forced to `sl` from source.**  Making
  `&Globals::status` live at the epilogue makes the phi fall to `r1` (a
  caller-saved register), never `sl`; adding live dummy pointers makes it
  `r1`; the faithful prologue makes it `r1`.  The only variant that produced
  `r4` was an early `String` dummy (handoff measurement).  `sl` requires the
  original's `this`-spill, which is a PEI spill-order decision, not a source
  ordering one.
* **The pad *order* is block placement.**  Ours is missing the original's
  `del fp` pad (`new AEGeometry` lands in `r6` here vs `fp` in the original);
  the String pads are otherwise the same 8 slots in a different address
  order.  Both follow from the same MBP permutation that the wave-121/123
  notes describe.

### Random statement search

80 random adjacent-statement swaps over the whole `process` body (lines
605–5293, same-indent single-line `;` statements): best was the baseline
60.256665340; all variants ≤ 60.256665340 or lost size.  Consistent with the
wave-119 greedy result — the baseline is a strong local optimum.

## Remaining root cause

Unchanged: (1) prologue accessor order / `this`-spill colouring
(`messages` r4 vs r5, `&Globals::status` r9 vs r4, `this` in `sl` vs
`[sp,#88]`), and (2) MachineBlockPlacement sub-block permutation inside the
big cases (largest difflib chunks: 329-instruction case-125 state-0 tail,
314-instruction case-125 state-1 body, 296-instruction case-154 state-10
block).  The EH pads are a symptom of both, not an independent lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check  -j6   # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift  -j6   # 0 fields
cmake --build cmake-build-match --target lint         # CLEAN
python3 /dev/shm/w7406/score.py                       # 60.250192743
python3 /dev/shm/w7406/textdiff.py cmake-build-match/libgof2hdaa.so  # .text 86993
```

Scratch helpers (`score.py`, `tryvar.py`, `measure.py`, `textdiff.py`,
`lsdacmp.py`, `lsdapads.py`, `regions.py`, `region_ctx.py`, `randswap.py`,
`randmove.py`, `batchA..G.py`, `LevelScript.base.cpp`) live in
`/dev/shm/w7406/` and are not part of the tree.
