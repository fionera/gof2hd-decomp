# LevelScript::process — wave 140 / w8505: mission-0 low-register map (r4–r7) sweep

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `d4857f174` (wave-139 docs head).
Scratch helpers in `/dev/shm/w8505/` (not tracked): `fdiff.py`, `regdiff.py`,
`try.py`, `batch*.py`.

## Result

```
                         before (master)   after (landed)
process raw diffbytes    54,262            54,260            (-2)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    84,651            84,649            (-2)
.ARM.extab diffbytes     1,395             1,395             (size 156,312 exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

(The prompt's `.text 85,273 / extab 1,404` baseline does not match this worktree's
build; the measured wave-139 head here is `.text 84,651 / extab 1,395`. The
process raw diff does match the prompt's 54,262 exactly.)

## The original mission-0 state-0 register map (confirmed)

Original `0x137e9c..0x137f7c`:

```
137e9c  mov   r4, sl          ; this -> r4   (sl freed)
137e9e  ldr   r5, [pc]        ; -12000.0f
137ea4  str   r0, [r4, #28]   ; m_nState = 1
137ea6  mov   sl, fp          ; stackguard -> sl (fp freed)
137eb2  ldr   r0, [r4, #20]   ; m_pCamera
137ebc  mov   r9, r2          ; 2000.0f -> r9
...
137eda  mov   r4, r8          ; enemies -> r4
137f1e  ldr   r4, [r1, #72]   ; 3rd setPosition target in r4 (clobbers enemies)
137f24  ldr.w r9, [sp, #96]   ; enemies reloaded -> r9
137f28  add.w fp, sp, #280    ; &temp2 -> fp
137f2c  mov.w r4, #1.0f       ; 1.0f -> r4
137f34  ldr.w r0, [r9, #4]    ; enemies->data (re-fetched each access)
```

So the original body map is `sl=stackguard, r4=1.0f, r9=enemies, fp=&temp2`,
with `this` copied to r4 for the two early accesses.

Ours (baseline) is the same cyclic permutation rotated by one:

```
137ea8  mov   r9, fp          ; stackguard -> r9
137ea4  str.w r0, [sl, #28]   ; this stays in sl
137ec2  mov   sl, r2          ; 2000.0f -> sl
137f2c  ldr   r4, [r4, #4]    ; enemies->data cached -> r4
137f2e  add.w sl, sp, #280    ; &temp2 -> sl
137f32  mov.w fp, #1.0f       ; 1.0f -> fp
```

i.e. `r4=enemies/data, r9=stackguard, sl=&temp2, fp=1.0f` — a 4-cycle
`r4 -> r9 -> sl -> fp -> r4` away from the original.

## What was swept (all built + linked + measured)

The loop is the cause. The original double-indexes `(*enemies)[i]` (reloads
`enemies->data` for both calls). The baseline caches `enemies->data_` in a local
outside the loop, so `r4` is live across the whole block and `this` stays in sl.

| variant | size | raw diff | map |
|---|---:|---:|---|
| **baseline** (`enemyData` outside loop) | **57232** | **54262** | sl=this→t2, r9=sg, fp=1.0, r4=data |
| `(*enemies)[i]` DI loop (A) | 57276 (+44) | 55446 | prologue re-colours: this→r5, delta→r4, sg→r6 |
| DI + `enemies->data_[i]` (B) | 57276 (+44) | 55446 | same |
| DI + `KIPlayer *e = (*enemies)[i]` (C) | 57224 (-8) | 55111 | same |
| `KIPlayer **enemyData` **inside** loop (D) | **57232** | **54260** | prologue unchanged; loop re-fetches data |
| D with `while` (ED) | 57232 | 54294 | same |
| D with `do/while` (EE) | 57232 | 54397 | same |
| D with `setVisible` first (EF) | 57232 | 54258 | unfaithful order |
| `(*m_pLevel->getEnemies())[i]` (R) | 57224 (-8) | 55332 | extra getEnemies call |
| local `es = m_pLevel->getEnemies()` (N) | 57248 (+16) | 55055 | enemies in r5, index r6 |
| `LevelScript *self = this` (HA) | 57232 | 54262 | no codegen change |
| prologue reorder messages-first (p) | 57328 (+96) | 55215 | matches original accessor order |
| p + DI (p_di) | 57400 (+168) | 55338 | body map still rotated |
| named `Vector dir/up` outside loop | 57288 (+56) | 55414 | — |
| `Vector(1,0,0)` ctor vs `{}` | 57276 (+44) | 55446 | — |
| `unsigned`/`size_t`/`!=`/post-inc index | 57276 (+44) | 55446 | — |
| `KIPlayer *const *` / `**const` / `auto` | 57232 | 54262 | no change |

Key negative results:

* **Every DI spelling costs +44 B** on this base (w8306 measured +60 on the
  wave-137 head). The +44 is a global re-colour of the prologue
  (`mov r5,sl; mov r4,r1; ldr r6,[r0]`), not the loop body.
* **The prologue reorder** (`messages, player, enemies`) reproduces the
  original's accessor order (`getMessages`→r5, `getPlayer`→[sp,#84],
  `getEnemies`→r8) but is **+96 B** and still leaves `Globals::status` in r9
  (the original has it in r4). It is not landable without a −96 companion.
* **The `self`/`this` copy** (`LevelScript *self = this; self->m_nState = 1;`)
  is optimised away; it does not produce the original `mov r4,sl`.
* The `messages` register (r4 vs r5) is a separate **4-byte** prologue
  difference (`ldr.w r9,[r0]` + `ldr.w r0,[r9]` vs `ldr r4,[r0]` +
  `ldr r0,[r4,#0]`); fixing it would align the whole state-0 body (whose
  literal pool is already identical and only shifted by 0x28).
* The state-1/2/3 wobble blocks are instruction-for-instruction identical
  (only branch targets and literal-pool displacements differ).

## The landed change

Move the cached data pointer into the loop:

```cpp
// before
KIPlayer **enemyData = enemies->data_;
for (int i = 0; i < 3; ++i) {
    enemyData[i]->geometry->setDirection(...);
    enemyData[i]->geometry->setVisible(true);
}
// after
for (int i = 0; i < 3; ++i) {
    KIPlayer **enemyData = enemies->data_;
    enemyData[i]->geometry->setDirection(...);
    enemyData[i]->geometry->setVisible(true);
}
```

This makes the loop re-fetch `enemies->data_` every iteration (the original
re-fetches it for both accesses), keeps the prologue map untouched, stays
exactly 57232 B, and lowers the raw diff by 2 B. It is a small but genuine
faithfulness improvement: the cached-data-outside-loop form was the reason the
original's `r4`-based map could not be reached.

## Open questions / next leads

1. The body map needs `sl=stackguard, fp=&temp2, r4=1.0f, r9=enemies`. The
   compiler only hoists `enemies` into r9 when r9 is free; r9 is occupied by
   the stackguard because fp is used for 1.0f. A source form that makes the
   compiler put 1.0f in r4 (or keeps the stackguard in fp) would rotate the
   map into the original. No local spelling found this pass.
2. The prologue reorder is the only way to get `messages`→r5 and
   `Globals::status`→r4, which would align the state-0 body (literal pool is
   identical). It needs a −96 B companion; the state-4 teardown `move4b` family
   (+8/+12/+16) is the wrong sign.
3. `.ARM.extab` remains 1,395 differing bytes at an exact 156,312 B size.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8505/fdiff.py _ZN11LevelScript7processEi   # 54,260
python3 /dev/shm/w8505/regdiff.py 0x16c 0x264               # state-0 body diff
```
