# Radar::draw wave 169 (w11408) — branch layout / cleanup pads re-sweep

**Status:** analysis pass, no source change landed (2026-11-11). Worktree
`wt/w11408`, branch `wave169-w11408`, base `master` `ec3ceb07c`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave168-w11308.md` (±24000 bound order, landed),
`radar-draw-wave168-w11307.md` (campaign/sl, loop block order),
`radar-draw-wave167-w11207.md` and `radar-draw-wave166-w11107.md`.

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,486 |
| instruction-index alignment | 1,456 |
| instructions | 3,536 ours / 3,532 orig |
| `.text` differing bytes | 82,327 |
| `.ARM.extab` differing bytes | 668 |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, parity 5370) |

## 1. First divergence is still the zero/campaign register choice

The whole prologue is byte-identical up to `12e0d8`:

| | original | ours |
|---|---|---|
| `12e0d8` | `mov.w fp,#0` | `mov.w sl,#0` |
| `12e0e8` turretMode | `str r0,[sp,#148]` | `str r0,[sp,#152]` |
| `12e0f8` campaign | `mov sl,r0` | `str r0,[sp,#144]` |
| `12e3c6` stations | `mov sl,r0` | `str r0,[sp,#132]` |
| `12e3c8` zero | `mov.w fp,#0` | `mov.w sl,#0` |

The RA picks `sl` for the missionBlocksPlanets/zero vreg in ours and `fp` in
the original; because `sl` is then occupied, `campaign` (one use, line 532)
is spilled.  Everything downstream (the ~40 % instruction mismatch) is this
one choice.  Re-tried this pass, all no-ops or worse (base 7,486/1,456/9,908):
`campaign > 1`, `campaign >= 0`/`!= -1`/`& 1 >= 0` redundant conjuncts
(identical codegen), `campaign < 1000` (8,461 / +8 B).

## 2. Exception-cleanup landing pads: the "7 vs 6" is a slot-colouring artefact

Decoded the LSDA call-site tables (both have 19 rows, `lpstart`/`ttype`
omitted, `cs_enc=uleb128`, all `action=0`).  The pad *chain* lengths are the
same: the largest cleanup destroys `sp+232,196,184,220,208` in both.  The
difference is the **generic final pad**:

* original `1306e0`: `mov r4,r0; add r0,sp,#232; blx StringD1Ev; resume`
* ours `1306e0`: `mov r4,r0; add r0,sp,#220; blx StringD1Ev; resume`
* ours also has a separate `1306d6`: `mov r4,r0; add r0,sp,#232; b 1306e4`

So ours has 7 `mov r4,r0` pad entries vs the original's 6.  Root cause: the
`this->lockLabel = this->calcDistance(...)` temporary at source line 795 is
coloured to **sp+220** in ours and **sp+232** in the original (the other five
`calcDistance` sites already use sp+232 in both).  If that one temp moved to
sp+232 the generic pad would merge and `.ARM.extab` would shrink.  This is a
stack-colouring consequence of the campaign spill; no source lever found
(`bool/int wreck`, `inSight`/`delta` decl order, `wreck` respellings all
byte-identical).

## 3. Landmark `!landmarkHit && i != 3 && notTurretMode && notOtherLock`

Original groups the four operands into **two** short-circuit checks
(`12e84c`-`12e868`): `(!landmarkHit && notOtherLock)` then
`(i != 3 && notTurretMode)`.  Ours flattens all four into one bitwise-AND
chain (`12e824`-`12e83e`).  Every source spelling that forces the two-group
form is worse (base 7,486/1,456/9,908):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `!landmarkHit && notOtherLock && i != 3 && notTurretMode` | 8,433 | 1,469 | 9,912 |
| `(!landmarkHit && notOtherLock) && (i != 3 && notTurretMode)` | 8,433 | 1,469 | 9,912 |
| `__builtin_expect(!landmarkHit && notOtherLock, 0/1) && ...` | 8,433 | 1,469 | 9,912 |
| `!landmarkHit && (i != 3 && notTurretMode) && notOtherLock` | 7,486 | 1,456 | 9,908 |
| `!landmarkHit && i != 3 && (notTurretMode && notOtherLock)` | 7,486 | 1,456 | 9,908 |

The two-group form is reproduced (`bics; ands; cmp; bne; ands; beq`) but the
+4 B is a register-allocation side effect of the reordered loads.

## 4. Sound ladder: base is still the expect optimum

Re-swept all 8 combinations of the three `field_0x1bc` expects on this base
(the wave-166 sweep was on an older base).  `o=1,a=0,b=0` (landed shape) is
the unique best: 7,486/1,456/9,908.  `a=1` alone is 7,536 (+4 B); `b=1`
alone is 7,538; `o=0` variants jump to 8,196-8,237 and `.text` 387k (the
whole tail re-lays out).

`if (this->field_0x1bc != 0)` instead of `>= 1` gives a much better aligned
score (**1,311** vs 1,456) and 3,530/3,532 instructions, but raw **7,528**
(+42) — rejected because raw is the ground truth.  The original's
`cmp r2,#1; blt` is `>= 1`, so the base spelling is also the faithful one.

## 5. `cbz` vs `cmp/beq.n` are branch-range artefacts

The `cbz`/`beq.n` mismatches are not source-shape bugs:

* `12f0c0` ours `cmp r6,#0; beq.n` vs `12f0e0` orig `ldr; cbz` — the value is
  in a register in ours, in memory in the original (regalloc).
* `12f0fe` ours `cmp r0,#0; beq.n 12f180` vs `12f11c` orig `cbz` — the target
  is 130 B away, beyond the T1 `cbz` forward range (126 B), so `beq.n` is
  forced.  Same for the second site.

## 6. Sound-block block placement

Original: hot sound block → literal pool (`1302dc`) → cold ambient body
(`1302f0`) → epilogue (`130326`) → `field_0x1bc` ladder (`130348`).
Ours: hot sound block → epilogue (`1302e2`) → literal pool (`130304`) →
ladder (`130318`) → … → cold ambient body (`1304fe`, `blt.w`).  The
`if (field_0x1bc >= 1)` else-body placement is the visible difference; it
follows the same RA/layout cascade as §1 and no expect spelling moves it
without breaking size.

## 7. Conclusion

The function stays a local optimum at raw **7,486** / aligned **1,456** /
9,908 B, `.ARM.extab` 668, `.rel.plt` 0.  The single dominant lever remains
the `sl`/`fp` choice at `12e0d8` (campaign spill); the cleanup-pad count and
the `cbz`/`beq.n` placement all follow from it.  No source change landed;
tree left clean at `ec3ceb07c`.

## 8. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11408/{measure.sh,try*.py,lsda2.py,sdiff2.py}
```
