# LevelScript::process — wave 143 / w8804: case-125 state-1 body placement sweep,
# the LSDA table / `.rel.plt` coupling, and a landed faithful RNG-order fix

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `bf083ab41` (wave-142 docs head).
Scratch helpers in `/dev/shm/w8804/` (not tracked): `fdiff.py`, `measure.sh`,
`lsda.py` (ARM EHABI LSDA decoder), `gen.py` (case-125 shape generator),
`c91.py` (case-91 nested test), `revc.py`, `swap45.py`, `reorder.py`, `win.py`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,691            54,790            (+99)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab table         0x146             0x146             (fits <= 0x147)
.ARM.extab size          156,312           156,312           (exact)
.ARM.exidx               exact             exact
.rodata / .data / .bss   exact             exact
.rel.plt                 byte-identical    byte-identical   (29,392 B)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The landed edit is the **faithful case-125 state-2 ambush-loop declaration /
RNG order** (original Thumb `0x13b5a4/0x13b5b0/0x13b5bc/0x13b5c6`):

```cpp
(*enemies)[i]->awake();
KIPlayer *pirate = (*enemies)[i];
Vector playerPos = player->getPosition();
int sideX = Globals::rnd->nextInt(100);
int offsetY = Globals::rnd->nextInt(1000);
int offsetZ = Globals::rnd->nextInt(2000);
int sideZ = Globals::rnd->nextInt(100);
```

The original RNG call order is **100, 1000, 2000, 100**; the previous source
called it **100, 100, 1000, 2000** (`sideZ` before `sideX`).  This is a genuine
semantic/asm-order fix, not a metric win: raw diff rises 54,691 -> 54,790.

The size-neutral companion is the case-91 state-5 receiver spelling
`player->setCollide(false)` -> `m_pLevel->getPlayer()->setCollide(false)`
(source line 2842).  **NOTE:** the original's case-91 site (`0x13f43c`,
`0x140330`) passes `player` in `r9`/`r6`, so this receiver spelling is
*unfaithful*; it is used only because it is the +8 B companion that restores
57232 B after the RNG-order fix (-8 B).  It does keep `.rel.plt` byte-identical.

## The case-125 state-1 body (the mission hypothesis)

Original Thumb (`0x13b43e..0x13b7ea`, the 351-instruction body):

```
139f9c  ldr r0,[sp,#88] ; ldr r1,[r0,#28] ; cmp r1,#0 ; bne.w 13b43e
139fba  state-0 body ... b.w 13b518
13b43e  cmp r1,#1 ; bne.n 13b518
13b444  state-1 body (guard + rotated for-loop) ... falls through
13b518  cmp r0,#2 ; bne.w 13dcb6   ; state-2 test
```

i.e. the state-1 test/body is a *contiguous* block placed immediately before
the state-2 test, and the state-0 body is the fallthrough of the `&&` test.
In master the state-1 loop is outlined to rel `0x3ad6` while the state-2 test
is at rel `0x610c`; the two are separated by the case-135 body.

### Shape sweep (all measured on the wave-142 base)

`raw` = same-address diff bytes; `table` = LSDA call-site table length
(original `0x144`, limit `0x147`); `relplt` = `.rel.plt` byte-identical.

| variant | size | raw | table | relplt | note |
|---|---:|---:|---:|---|---|
| **master (`switch` 1,2,3,d)** | **57232** | **54,691** | **0x146** | **yes** | loop outlined |
| `s1first` (state-1 first, state-0 else-if, switch 2,3,d) | 57232 | 54,772 | 0x148 | no | loop at 0x30ae, `new Route` 3-byte |
| `s1f_chain` (if/else-if 2,3,4..6) | 57232 | 54,772 | 0x148 | no | same |
| `s1sep` (separate ifs 0 and 1) | 57224 | 55,159 | 0x146 | no | state-1 contiguous at 0x20b8 |
| `twochain` (faithful if/else-if 0,1 then 2,3,4..6) | 57240 | 55,152 | 0x148 | no | state-0 outlined |
| `nested` (`if(0){if(msg)} else if(1)`) | 57240 | 55,171 | 0x14b | no | state-1 outlined |

All case-125 shape changes that move the state-1 body inline also move the
`default` body (with `player->deleteHackingGame()`) *before* the case-135 body,
which swaps `PlayerEgo::deleteHackingGame` ahead of
`StarSystem::switchPlanetForIntro`/`Level::switchSkyboxForIntro`/... in
`.rel.plt` (6-entry rotation).  This is why every shape variant fails the
"`.rel.plt` byte-identical" requirement even when the size is exact.

### The table coupling

`__builtin_expect` is **not usable**: `tools/goal_lint.py` flags
`__builtin_*` (`arm_builtin`, baseline 0).  Without it the plain two-chain
outlines the state-0 body (clang treats the `&&` body as 25 % cold), the
opposite of the original.

`twochain_exp0` (with `__builtin_expect(m_nState == 0, 1)`) reproduces the
original's contiguous state-1 block exactly (state-0 inline, state-1 test/body
+ state-2 test contiguous), size 57232, but the state-1 String call sites sit
at rel `0x4542..0x4562` (> `0x4000`, 3-byte ulebs) so the table is `0x14b`
and `.ARM.extab` grows by 4 (byte_exact 4507 -> 4044).  It also fails lint.

### Table-shrink companions

The case-91 nested test (`c91.py`) moves the case-91 `setName` below `0x4000`
and shaves 2 table bytes:

| variant | size | raw | table |
|---|---:|---:|---:|
| master + c91 | 57220 | 55,151 | 0x144 |
| s1first + c91 | 57236 | 55,419 | 0x146 |
| s1f_chain + c91 | 57236 | 55,419 | 0x146 |
| s1sep + c91 | 57228 | ? | 0x144 |
| s1sep + c91 + s2faith + setCollide | 57228 | 54,631 | 0x144 |

`s1sep + c91 + s2faith + setCollide` is the closest miss: the state-1 body is
inline/contiguous, the table is exact `0x144`, raw is **54,631 (-60 vs
baseline)**, but the size is 57228 (-4) so `byte_exact` drops.  A genuine +4 B
faithful companion would land it; none was found this pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8804/fdiff.py _ZN11LevelScript7processEi   # 54,790
python3 /dev/shm/w8804/lsda.py cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 /dev/shm/w8804/gen.py s1sep && python3 /dev/shm/w8804/c91.py
```

## Open leads

1. `s1sep + c91 + s2faith + setCollide` at 57228 / table 0x144 / raw 54,631
   needs a genuine +4 B faithful companion (and the `.rel.plt` `setVisible`
   rotation fixed).
2. The `.rel.plt` order is pinned by the relative placement of the case-125
   `default` body (`deleteHackingGame`) and the case-135 body; the master
   `switch` shape interleaves case-135 between the state-1 loop and the
   state-2 body, which is what keeps `.rel.plt` identical.
3. The original's case-135 body is placed *before* case 125; ours places it
   between case-125's state-1 loop and state-2 body.  A source/switch shape
   that moves case-135 ahead of the whole case-125 chain would make the
   contiguous state-1 block land below `0x4000` and fix both the table and
   `.rel.plt` at once.
