# Radar::draw wave 173 (w11807) — image-chain / exclude-flag / shipGroup re-sweep, no win

**Status:** no source change landed, 2026-11-11. Worktree `wt/w11807`, branch
`wave173-w11807`, base `master` `3fafa5b03`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave171-w11707.md` (the two landed expects) and
`radar-draw-wave171-w11708.md` (condition-order / cbz / tail sweeps).

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,054 |
| instruction-index alignment | 1,582 |
| instructions | 3,540 ours / 3,532 orig |
| `.text` differing bytes | 81,503 |
| `.ARM.extab` differing bytes | 711 |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN) |

## 1. Scope of this pass

Re-swept every condition named by the wave-173 brief on the post-wave-172 base:

* enemy frame/bar image chain (`if (target->enemyFlagsLo) … else if
  (target->carriesFriendCargoFlag)`, lines 727/730);
* lock-image chain (`if (enemy->player->enemyFlagsLo) … else if
  (…carriesFriendCargoFlag)`, lines 755/757);
* on-screen image chain (`if (enemy->player->enemyFlagsLo) … else if
  (…carriesFriendCargoFlag)`, lines 770/772/777/779);
* off-screen image chain (lines 806/808/810/816/818);
* `countsAsEnemyExcludeFlag` at the wreck chain (808), the enemy-selection
  guard (694) and the tractor guard (829);
* `shipGroup == 9` (810) and `shipGroup == 10` (681/697).

Each condition was wrapped whole and per top-level `&&`/`||` operand in
`__builtin_expect((cond), 0)` and `(cond), 1)`; neutral variants were then
combined pairwise (120 pairs).  All builds were `-j2` under
`ulimit -v 12000000`.

## 2. Result: every cell is neutral or worse

Single-condition sweep (raw / aligned / size), base = 7,054 / 1,582 / 9,908:

| site | expect | raw | aligned | size |
|---|---|---:|---:|---:|
| 727 `target->enemyFlagsLo` | 0 | 7,054 | 1,589 | 9,908 |
| 727 `target->enemyFlagsLo` | 1 | 8,451 | 1,626 | 9,912 |
| 730 `target->carriesFriendCargoFlag` | 0/1 | 7,054 | 1,582 | 9,908 |
| 755 lock `enemyFlagsLo` | 0 | 7,054 | 1,582 | 9,908 |
| 755 lock `enemyFlagsLo` | 1 | 7,229 | 1,602 | 9,908 |
| 757 lock `carriesFriendCargoFlag` | 0/1 | 7,054 | 1,582 | 9,908 |
| 770 `enemyFlagsLo` (locked) | 0 | 7,054 | 1,593 | 9,908 |
| 770 `enemyFlagsLo` (locked) | 1 | 7,054 | 1,620 | 9,908 |
| 772 `carriesFriendCargoFlag` (locked) | 0/1 | 7,054 | 1,582 | 9,908 |
| 777 `enemyFlagsLo` (plain, hot arm) | 1→0 | 7,055 | 1,619 | 9,908 |
| 777 `enemyFlagsLo` (remove expect) | — | 7,054 | 1,620 | 9,908 |
| 779 `carriesFriendCargoFlag` (hot arm) | 0/1 | 7,054 | 1,582 | 9,908 |
| 808 `e->countsAsEnemyExcludeFlag` | 0 | 7,054 | 1,624 | 9,908 |
| 808 `e->countsAsEnemyExcludeFlag` | 1 | 7,054 | 1,603 | 9,908 |
| 810 `e->shipGroup == 9` | 0/1 | 7,054 | 1,582 | 9,908 |
| 816 `enemyFlagsLo` (off-screen) | 0→1 | 8,664 | 1,630 | 9,892 |
| 816 `enemyFlagsLo` (remove expect) | — | 8,410 | 1,646 | 9,912 |
| 818 off-screen `carriesFriendCargoFlag` | 0 | 7,819 | 1,634 | 9,900 |
| 818 off-screen `carriesFriendCargoFlag` | 1 | 7,818 | 1,634 | 9,900 |
| 829 exclude-flag term | 0 | 8,564 | 2,719 | 9,896 |
| 829 exclude-flag term | 1 | 8,793 | 2,821 | 9,992 |
| 694 exclude-flag term | 0 | 8,665 | 1,619 | 9,888 |
| 694 exclude-flag term | 1 | 9,197 | 1,978 | 9,928 |

Boolean re-spellings are codegen-neutral (identical 7,054 / 1,582 / 9,908):
`countsAsEnemyExcludeFlag == 0 && !wreck`, `(… == 0 || wreck)`,
`e->countsAsEnemyExcludeFlag != 0`, `bool`/`unsigned`/`char` `wreck`, and
`campaign > 1` / `2 <= campaign` / `campaign - 2 >= 0`.

Pairwise combinations (120 pairs of the neutral single cells) are all
7,054 / 1,582 / 9,908 or worse; none reaches raw < 7,054.

## 3. Why the chains cannot win: they are already instruction-identical

The aligned diff shows the image chains have the same instruction sequence as
the original; only register names and stack slots differ.  E.g. the off-screen
chain is, in both:

```
ldr   wreck; cmp #0; ldr image_0xa0; bne draw
ldr   player; ldr locked; ldrb enemyFlagsLo; cmp enemy,locked; beq locked-chain
cmp   enemyFlagsLo,#0; ldr image_0x60; bne draw
ldrb  carriesFriend; cmp #0; ldr image_0x68; it eq; moveq image_0x78
locked-chain: cmp enemyFlagsLo,#0; ldr image_0x7c; bne draw
              ldrb carriesFriend; cmp #0; ldr image_0x80; it eq; moveq image_0x84
```

The wreck chain is likewise identical (`ldrb [e,#0x3d]; cmp #0; ldr
image_0xb0; bne; ldr [e,#0x28]; cmp #9; ldr image_0xa8; it eq; moveq
image_0xb4`).  The frame/bar chain and the lock-image chain are also
instruction-identical.  The only differences are the hoisted image-pointer
stack slots (`sp+72/76` vs `sp+76/72`, `sp+84` vs `sp+92`, …), the
`fp`/`sl`/`r5`/`r6`/`r9` shuffles, and branch displacements.

## 4. Root cause remains the `campaign` allocation

The first byte divergence is at fn+0x300 (linked `12e350`), the campaign
guard:

```
orig:  12e38a  cmp.w  sl, #2        ; campaign lives in sl
       12e38e  str    r0, [sp,#144]
ours:  12e38a  str    r0, [sp,#148]
       12e38c  ldr    r0, [sp,#144] ; campaign spilled
       12e38e  cmp    r0, #2
```

The original keeps `campaign` in `sl` and `missionBlocksPlanets` in `fp`;
ours keeps `missionBlocksPlanets` in `sl` and spills `campaign` to `sp+144`.
Everything after the guard then shifts (the planet loop index moves from `fp`
to `sl`, `stations` is reloaded from `sp+132` every iteration, and the whole
enemy/asteroid/music tail inherits the 4-byte stack-slot cascade).  The
per-256-byte raw histogram is 11/1/1 differing bytes for `fn+0x000..0x2ff`,
then ~240/256 for the rest, and 3/3/7/6 again at `fn+0x1800..0x1bff`.

Declaration-order and type experiments on the two variables were re-run on
this base and are all neutral or worse:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `int campaign;` declared before `Mission *mission` (assign in place) | 7,054 | 1,582 | 9,908 |
| `int campaign = 0;` early, assign in place | 7,054 | 1,582 | 9,908 |
| `campaign` register / `missionBlocksPlanets` register | 7,054 | 1,582 | 9,908 |
| `campaign` unsigned | 7,055 | 1,582 | 9,908 |
| `campaign` short | 8,451 | 1,607 | 9,912 |
| `campaign` char | 7,435 | 1,657 | 9,904 |
| `missionBlocksPlanets` int / uint8 | 7,077 | 1,584 | 9,904 |
| `missionBlocksPlanets` volatile | 9,555 | 2,931 | 9,832 |
| `campaign` volatile | 8,594 | 2,836 | 9,856 |
| `campaign` static | 9,309 | 1,644 | 9,964 |
| campaign call moved before `getMission()` | 7,929 | 1,587 | 9,900 |
| campaign call after `getMission()` | 7,929 | 1,587 | 9,900 |
| campaign call before `turretMode` | 7,877 | 1,579 | 9,900 |
| campaign call after `missionBlocksPlanets` | 8,498 | 1,607 | 9,900 |

Moving `alwaysEnemySeen` / `alwaysEnemy` to the blank line before the planet
block (the only legal sink point before the `goto enemies` labels) is also a
codegen no-op (7,054 / 1,582 / 9,908).

## 5. One real codegen difference in the enemy loop (not source-reachable)

The original re-evaluates `shipGroup == 10` after `this->field_0x1bc++` in the
inactive-enemy branch:

```
orig: 12ed48  ldr r0,[r4,#444]; adds r0,#1; str r0,[r4,#444]
      12ed52  ldr r0,[r8,#40]; cmp r0,#10; it eq; moveq r6,#1
ours: 12ed0c  ldr r0,[r4,#444]; movs r5,#1; adds r0,#1; str r0,[r4,#444]
```

i.e. the original reloads `enemy->shipGroup` across the `field_0x1bc` store
(the compiler cannot prove `this` and `enemy` do not alias), while ours keeps
the known-true condition and sets `bossSeen` unconditionally.  Forcing the
re-check from source (a nested `if (enemy->shipGroup == 10) bossSeen = true;`,
`bossSeen = (shipGroup == 10)`, `bossSeen |= (shipGroup == 10)`) is much
worse: 9,334 / 9,988; 8,592 / 9,916; 9,137 / 9,992.  This is an allocator /
alias-analysis artifact, not a source-shape knob.

Per-TU flag re-check (Radar.cpp `COMPILE_OPTIONS`) confirms the checked-in
knobs are still best: base 7,054; drop `-disable-postra-machine-licm` 8,425
(size 9,912); drop `+cheap-predicable-cpsr` 7,056; add
`-disable-postra-machine-sink` 7,177; `-regalloc=basic` 9,826 (size 10,156);
`-regalloc=fast` 13,543; `-regalloc=pbqp` 9,736.

## 6. Conclusion

No size-exact raw win in the enemy-image chains, lock-image chain, off-screen
image chain, `countsAsEnemyExcludeFlag` or `shipGroup == 9` on this base.  The
chains are already instruction-identical; the residual is the `campaign`-in-
`sl` vs `sp+144` allocation and the resulting stack-slot / block-layout
cascade.  `src/` is left at the base.  Gate after the pass: byte 4,507 /
linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 /
`.text` 81,503 / `.ARM.extab` 711 / `.rel.plt` 0.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /home/debian/GalaxyOnFire2/scratch/w11807/{sweep*.py,sweep*.log,
#          flagsweep.py,flagsweep.log,adiff.py,Radar.cpp.orig,CMakeLists.txt.orig}
```
