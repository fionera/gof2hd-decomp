# LevelScript::process — wave 162 / w10801: the reorder companion is
# `Status *&` + a `stars4` local (size-exact, 9 startFade calls)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10801`, base master
`29248c1d0` (wave 161).  Scratch `/dev/shm/w10801/`.

## Result (landed)

```
                         base (29248c1d0)   landed
process raw diffbytes    55,013             54,996            (-17)
process aligned insns    17,308             17,311            (+3)
orig / ours size         57232 / 57232      57232 / 57232     (size-exact)
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN              CLEAN
drift                    0                  0
RATCHET                  PASS               PASS
.text diffbytes          85,374             85,357            (-17)
.rel.plt                 byte-identical     byte-identical
.ARM.extab               725                725
.ARM.exidx / .rodata / .data   0 / 0 / 0   0 / 0 / 0
```

Landed source change (2 hunks):

```cpp
    Array<RadioMessage *> *messages = m_pLevel->getMessages();
    Status *&status = Globals::status;          // was: PlayerEgo *player = ...
    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
    PlayerEgo *player = m_pLevel->getPlayer();  // was: Status *status = ...
...
            case 4: {
                StarSystem *stars4 = m_pLevel->getStarSystem();   // was two calls
                stars4->scaleSunDuringSupernovaIntro(delta);
                stars4->updateSupernova(delta);
```

`Level::getStarSystem()` is a pure getter (`return starSystem;`), so the
single local is semantically identical to the two calls w10702 introduced.

## 1. The plain reorder on the CURRENT base is +68, not +60

The wave-160 w10701 number (`54,929 / 16,920 / 57,292`, +60) is on the
wave-160 base `01c7f92d6`.  On the wave-161 base the same plain reorder
(`messages,status,enemies,player`, plain `Status *status`) is:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base `messages,player,enemies,status` | 55,013 | 17,308 | 57,232 |
| plain reorder (`msep`) | 55,416 | 17,149 | 57,300 (+68) |
| plain reorder + w10702 h1+h2 reverted | 54,929 | 16,920 | 57,292 (+60) |

So w10702's two hunks (drop `enemies->data_` guard; mission-89 case-4
8000 transition / double `getStarSystem`) interact with the reorder: +8
size and +487 raw.  The old w10701 +60 decomposition (+10 fp spill,
+44 block placement, +4 extra `startFade`) is therefore the wave-160
accounting; on the current base the gap is +68.

## 2. The +4 call: `Status *&` closes it

The plain reorder has **10** `Layout::startFade` calls (original has 9).
The mission-89 case-2 site does not tail-merge with the mission-81
`startFade(true,255,5000); m_nState=3; return 1;` site because the two
suffixes differ (store register and a `ldr.w fp,[sp,#68]` canary reload).

Using a reference to the global (`Status *&status`) instead of a plain
`Status *status` restores the base's register allocation and gives
**9** calls, with the mission-89 case-2 site branching to the mission-81
call site exactly like the original (`13b3a6 -> 139154`):

| shape | raw | aligned | size | startFade |
|---|---:|---:|---:|---:|
| plain reorder | 55,416 | 17,149 | 57,300 | 10 |
| **reorder + `Status *&`** | **55,026** | **17,310** | **57,232** | **9** |
| reorder + `Status *&` + `stars4` | **54,996** | **17,311** | **57,232** | **9** |

So `Status *&` alone closes the +68 (and the +4 / +10 / +44 symptoms);
the `stars4` local is a further -30 raw.

## 3. Full permutation sweep on the current base (plain `Status *`)

24 declaration permutations, full link each:

| perm | raw | aligned | size |
|---|---:|---:|---:|
| `mpes` (base) | 55,013 | 17,308 | 57,232 |
| `meps` / `pmes` | 55,025 | 17,291/17,294 | 57,232 |
| `epms` | 55,017 | 17,310 | 57,232 |
| `mpse` | 54,847 | 17,504 | 57,264 (+32) |
| `msep` | 55,416 | 17,149 | 57,300 (+68) |
| `mesp` / `emps` | 55,340 | 17,838/17,830 | 57,224 (-8) |
| others | 55,033..55,139 | — | 57,232..57,308 |

`mpse` is the best raw but its +32 is an extra exception-landing-pad
cleanup (the cold EH block grows by 32 B), not the fp spill.

## 4. `__builtin_expect` on the current base

Re-ran the w10705 size-exact flippers on the wave-161 base:

| guard | expect | raw | aligned | size |
|---|---|---:|---:|---:|
| L1455 `m_bStartSequence != 0` | 0 | 55,391 | 16,940 | 57,268 (+36) |
| L4026 `state>=4&&<=6` | 1 | 55,016 | 17,315 | 57,232 |
| L1532 `count>=3&&...` | 0 | 55,275 | 17,378 | 57,232 |
| L727 `messages[13]->isOver()` | 0 | 55,209 | 17,303 | 57,240 |
| L1030 `!isFading()` | 1 | 54,872 | 18,427 | 57,264 |

L1455:0 still flips the tie (`sl`=this, `fp`=canary) and gives 9 calls,
but costs +36 on this base (was +20 on wave-160) and moves the
`m_bStartSequence` block cold, so raw worsens.  No expect-only companion
reaches 57,232 with a raw win.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10801/fdiff.py _ZN11LevelScript7processEi        # 54,996 / 57,232
python3 /dev/shm/w10801/align_diff.py _ZN11LevelScript7processEi  # 17,311
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. `mpes` + `Status *&` + `stars4` (base order, no reorder) gives the same
   54,984 / 17,296 / 57,232 as `mpse` + ref + stars4 — 12 raw bytes better
   than the landed `msep` form.  The reorder is therefore not required for
   the raw win; `Status *&` + `stars4` is the whole companion.
2. The `stars4` local (one `getStarSystem` instead of the original's two)
   is what enables the mission-81/89 call tail-merge; the original's two
   calls are semantically identical because `Level::getStarSystem()` is a
   pure getter.  If exact call count is preferred, the plain `Status *&`
   form is 55,026 / 17,310 / 57,232 with 9 calls.
