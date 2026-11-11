# LevelScript::process — wave 174 / w11906: deactivation `ittt` -> separate cmp + `lvl` alias

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11906`, branch `wave174-w11906`, base master `be9dd429b` (wave 173).
Scratch `/home/debian/GalaxyOnFire2/scratch/w11906/`.

Follow-up to `levelscript-process-blockorder-wave173-w11806.md` and
`levelscript-process-state0-wave173-w11803.md`.

## Result (code win landed)

```
                         base (HEAD)      this wave
process raw diffbytes    53,711           53,678     (-33)
process aligned insns    18,228           18,230     (+2)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,729
.text diffbytes          80,897           80,864     (-33)
.ARM.extab diffbytes     675              675        (unchanged)
.rel.plt diffbytes       0                0
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
RATCHET                  PASS             PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`.  `aligned` is the positional
normalized-instruction diff from the same tool.  Raw is the ground truth;
the +2 aligned is the usual register-colouring side effect of the alias.

## The change

In the shared-tail deactivation block (`0x137fda..0x138000`), two edits:

1. **Drop the `__builtin_expect(mission >= 2, 1)` hint.**  The original emits
   three independent compare/branch pairs (`cmp r0,#2; blt`, `cmp r0,r1; blt`);
   the hint made clang if-convert the last two tests into `ittt ge` +
   `ldrge/movwge/cmpge`.  Removing the hint restores the original's separate
   `cmp` shape.
2. **Add a block-scoped `Level *lvl = m_pLevel;`** and use it for the two
   `m_pLevel->getPlayer()` calls in the block.  `m_pLevel` is assigned only in
   the constructor (`LevelScript.cpp:57`), so the alias is semantics-preserving.
   The alias compensates the +8 B size growth from (1) exactly: the compiler
   folds `lvl` into the existing `ldrd r0,r4,[fp,#20]` and reuses `r4` for both
   `getPlayer()` calls, giving back the 8 B and improving the register
   allocation of the rest of the block.

```diff
         if (m_pLevel->getPlayer() != 0 &&
-            __builtin_expect(Globals::status->getCurrentCampaignMission() >= 2, 1) // lint: arm_builtin
- &&
+            Globals::status->getCurrentCampaignMission() >= 2 &&
             field_0x24 >= 7001) {
+            Level *lvl = m_pLevel;
             field_0x24 = 0;
             m_bStartSequence = 0;    // active = false
             m_bStartSequenceOver = 1; // startSequenceIsOver = true
             m_pCamera->setLookAtCam(false);
-            m_pLevel->getPlayer()->player->setVulnerable(true);
+            lvl->getPlayer()->player->setVulnerable(true);
             m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11)
-            m_pLevel->getPlayer()->setCollide(true);
+            lvl->getPlayer()->setCollide(true);
```

After the change the block is:

```
original 0x137fda                     ours 0x137fda
blx getPlayer                         blx getPlayer
cmp r0,#0                             cmp r0,#0
beq.w 0x13812a                        beq.w 0x138786     (else-if target; see below)
ldr r0,[sp,#92]; ldr r0,[r0]          ldr.w r0,[r9]      (status register)
blx getCurrentCampaignMission         blx getCurrentCampaignMission
cmp r0,#2                             cmp r0,#2
blt.w 0x13812a                        blt.w 0x138786
ldr.w r0,[sl,#36]                     ldr.w r0,[fp,#36]
movw r1,#7001                         movw r1,#7001
cmp r0,r1                             cmp r0,r1
blt.w 0x13812a                        blt.w 0x138786
movs r5,#0                            ldrd r0,r4,[fp,#20]   (m_pCamera, lvl)
mov.w r1,#256                         movs r5,#0
str.w r5,[sl,#36]                     mov.w r1,#256
ldr.w r0,[sl,#20]                     str.w r5,[fp,#36]
```

The `ittt ge` is gone; the block is instruction-for-instruction the original
apart from the `sl`/`fp` swap and the branch target.  The branch target is
still `0x138786` (the switch guard) instead of `0x13812a` (the mission-5
check) because the mission chain is still written as an `else if` of
`m_bStartSequence`; converting it to the faithful separate `if` (the Unity
oracle in `_work/oracles/.../LevelScript.c` shows a separate `if`) shrinks the
function to 57,200 B and was measured worse in wave 173.

## Why this is the right lever

Wave 173 (`levelscript-process-blockorder-wave173-w11806.md`) identified the
deactivation block as the only remaining *local* structural difference in the
shared tail: "instruction-for-instruction identical apart from the sl/fp
register swap and the `ittt` vs separate `cmp` shape produced by the
`__builtin_expect(mission >= 2, 1)` hint.  Removing the hint restores the
original's separate `cmp` shape but grows the function by 8 B (57,240) and raw
rises to 55,092."

The missing companion was found here: a block-scoped `Level *lvl` alias in the
same block is exactly -8 B and turns the +8 B shape into a size-exact shape,
while also removing the extra `ldr.w r0,[sl,#24]` reload before each
`getPlayer()` call.  The result is -33 raw at 57,232 B.

## Rejected variants (all size-checked on the landed base)

| variant | raw | size |
|---|---:|---:|
| base (expect + no alias) | 53,711 | 57,232 |
| drop expect only | 55,074 | 57,240 |
| drop expect + `lvl` alias (landed) | **53,678** | **57,232** |
| drop expect + `PlayerEgo *pl` alias | 53,688 | 57,232 |
| `lvl` alias only (keep expect) | 53,718 | 57,232 |
| `PlayerEgo *pl` alias only | 53,705 | 57,232 |
| landed + `lvl` also in hudEvent | 54,876 | 57,232 |
| landed + `lvl` also in wingman loop | 54,875 | 57,232 |
| landed + mission-50 condition swap | 53,679 | 57,232 |
| landed + mission-5 separate `if` | 55,395 | 57,208 |
| landed + guard `Status *gst` alias | 53,808 | 57,224 |
| landed + guard `Mission *gmis` alias | 54,709 | 57,216 |
| landed + wormhole `KIPlayer *e` loop alias | 55,324 | 57,216 |
| landed + `lvl` at function scope | 55,214 | 57,356 |

The `lvl` alias must stay **block-scoped** to the deactivation `if` and only
replace the two `getPlayer()` uses; hoisting it to the function scope or using
it in the `hudEvent`/wingman-loop sub-blocks changes the size or regresses raw.

## Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53678 aligned= 18230 size= 57232/57232 insn= 20729/20830
cmake --build cmake-build-match --target check -- -j2    # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2    # 0 fields
```

Scratch harnesses: `score.py`, `bins.py`, `seqdiff.py`, `shift.py`,
`sweep_decls.py`, `sweep_statearms.py`, `variants.py`, `alias_sweep.py`,
`deact_sweep.py`, `combo2.py`, `combo3.py`, `combo4.py`, `pos_sweep.py`,
`guard_sweep.py`, `spec_sweep.py`, `secdiff.py`.
