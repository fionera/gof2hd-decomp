# LevelScript::process — wave 148 / w9407: land the case-157 strong-arm
# `m_bCinematicBreak` removal (w9303 open question 2) at 57,232 B

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `cbe3b5f7f` (wave 147).
Scratch helpers in `/dev/shm/w9407/` (not tracked): `fdiff.py`, `align_diff.py`,
`regiondiff.py`, `callseq2.py`, `storeseq.py`, `apply2.py`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,913            54,906            (-7)
process aligned insns    15,740            15,811            (+71, see note)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    86,160            86,153            (-7)
.ARM.extab diffbytes     1,309             1,309             (unchanged)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 4                 4   (setVisible <-> deleteHackingGame)
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The landed change (6 edits, all in the case-157 state-5 block)

1. **Strong arm: `m_bCinematicBreak = 1` removed, `destroyed = 0` added.**
   Orig `0x144a14` is `ldr r6,[sp,#96]; movs r5,#11; movs r4,#0; b.n` — no
   `m_bCinematicBreak` store, but an explicit `destroyed = 0` (the baseline
   source left `int destroyed;` uninitialized in the `else` arm, which is UB).
2. **Timer block: `m_pHud->visible = 0` -> `m_nScriptTimerB = 0`.**
   Orig `0x144a4e` is `movs r1,#0; strd r1,r1,[r0]` (m_nScriptTimerB = 0) and
   the timer block has no `m_pHud` store.
3. **Later block: `m_nScriptTimerB = 0` removed; `m_bCinematicBreak = 1` and
   `m_pHud->visible = 0` added before `m_pRadar->enabled = 0`.**
   Orig `0x144ad4..0x144ae4` is exactly
   `ldr m_pHud; movs r1,#1; strb [this,#17] (m_bCinematicBreak=1);
    movs r1,#0; strb [m_pHud,#1] (visible=0); ldr m_pRadar; strb [m_pRadar,#72]`.
4. **`m_tempVec` order swapped** to `= stationShip->getPosition()` then
   `+= Vector{45000,2000,11000}`. Orig `0x144b1e..0x144b46` does the
   `Vector::operator=` first, then `Vector::operator+=` (the baseline source
   had them reversed, which was a real semantic bug: the camera sat at the
   station position instead of station+offset).
5. **Case-154 completion `m_nState++` -> `m_nState = 6`** (line 4712).
   This is the size-restoring companion. See below.

## Why the case-154 completion is the missing +4 (w9303's "+8 companion")

w9303 localised the block immediately before the case-157 strong arm as the
"case-157 state-4 completion". It is actually the **case-154 state-5
completion** (`setAlwaysFriend`/`setAlwaysEnemy`/`setEnemies`/
`setVulnerable`/`setFixed`/`resetGunDelay`/... /`m_nState++`), ending at
`0x144a12`, immediately before the strong arm at `0x144a14`.

That block's tail is inline in the original:

```
144a04: strd r4,r4,[r9]        ; m_nScriptTimerA = 0
144a08: ldr.w r0,[sl,#28]      ; m_nState
144a0c: adds r0,#1             ; m_nState++
144a0e: str.w r0,[sl,#28]
144a12: b.n 144e36
```

Ours tail-merges the `m_nState++` store into a shared cold stub
(`0x1415e2: str.w r0,[sl,#28]; b.w 144e8a`), so the block is 4 B short.
Writing the same transition as the semantically equivalent
`m_nState = 6;` (m_nState is 5 here) makes LLVM materialise the constant and
restores the 4 B, landing the function at exactly 57,232 B. The emitted
instruction is `movs r0,#6` rather than the original's `adds r0,#1`; this is
the only remaining instruction-level difference in that tail.

The strong-arm + timer + later-block fixes alone are **-4 B (57,228)**; the
case-154 tail form is the +4 companion that closes the size.

## Metrics note

Raw same-address diff improves by 7 B (54,913 -> 54,906). The instruction-index
alignment metric regresses by 71 (15,740 -> 15,811). The alignment metric is
dominated by the global `sl` colouring (`this` vs `delta`, see w8103/w8603) and
by the cold-block placement of the state-4/5 blocks, not by the state-5 body;
the tool's own header says LevelScript::process is "indicative only". The
faithful-tail variant `A B C D E F G K` (tail order setVisible/setActive/
setVulnerable/field_0x140/forceUpdate/m_nState/m_nScriptTimerA) is better on
both raw (54,841) and aligned (15,768) but is 57,228 B, so it cannot land
without another +4.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9407/fdiff.py _ZN11LevelScript7processEi        # 54,906 / 57,232
python3 /dev/shm/w9407/align_diff.py _ZN11LevelScript7processEi  # 15,811
```

## Open questions

1. The case-154 completion tail still emits `movs r0,#6` instead of the
   original's `adds r0,#1`. The shared `m_nState++` stub at `0x1415e2` is used
   by 5 blocks; changing the other `m_nState = m_nState + 1` site
   (`LevelScript.cpp:3684`, case 105's 6->7 transition) to a constant might
   free the stub and let the case-154 site inline, but it also re-rolls the
   case-105 block layout.
2. The faithful-tail variant is 57,228 B; finding its +4 would give raw
   54,841 / aligned 15,768 at the exact size.
