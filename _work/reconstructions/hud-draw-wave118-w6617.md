# Hud::draw — wave-118: mission-panel `ldrd` operand order — w6617

**Status:** landed pass, 2026-11-05 (wave 118). Worktree `wt/w6617`, branch
`wave118-w6617`, base `master 3d4a98a3e`. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (file `0x163b90`).

| metric | before | after |
|---|---:|---:|
| fuzzy (`verify.py` ratio) | **95.890846 %** | **95.986038 %** |
| exact (`SequenceMatcher` on normalized insns) | 95.89084562906552 % | 95.98603839441536 % |
| size | 9332 B | 9332 B |
| `.text` | 0x1337c8 | 0x1337c8 |
| `.ARM.extab` | 0x2628c | 0x2628c (unchanged) |
| sections / file size | 27 / 2,187,328 | 27 / 2,187,328 |

Gate (`check`): **RATCHET PASS**, `byte_exact 4501`, `linked_exact 4501`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**.

---

## 1. What changed

One expression in the type-184 mission panel (`Hud.cpp` ~1355), the `y`
argument of the status-value `DrawString`:

```c++
-    this->field_0x43a + Globals::layout->field_0x1f4 + Globals::layout->field_0x1f8 * 2 +
+    this->field_0x43a + (Globals::layout->field_0x1f8 * 2 + Globals::layout->field_0x1f4) +
         Globals::Canvas->GetImage2DHeight(this->image_0x334),
```

The original loads the two adjacent `Layout` fields with one `ldrd` and adds
them in the order `(field_0x1f8 * 2) + field_0x1f4` (`ldrd r5,r8 [r1,#500]` /
`add r2,r5` / `add.w r2,r2,r8,lsl#1`).  The old spelling emitted the `ldrd`
with the pair swapped and the adds in the other order.  Parenthesising the
`field_0x1f8 * 2` term first makes clang emit the original's operand order.
Size is unchanged (9332 B) and no section moves.

## 2. Measured variants (all full build + `verify.py`, on the winning base)

| variant | size | fuzzy |
|---|---:|---:|
| **paren `(field_0x1f8*2 + field_0x1f4)`** | **9332** | **95.986038** |
| pristine HEAD (unparenthesised) | 9332 | 95.890846 |
| gamma block: `Globals::status` for arg2 (any spelling: direct, `Status*&`, `const` ref, `float` local, `Station*` local) | 9312 | 74.99 |
| gamma block: `st` local (current) | 9332 | 95.986038 |
| fireImage if/else (`x = autoTurretOnImage; if (cond) x = idle`) | 9332 | 95.986038 |
| fireImage `bool useIdle` local | 9332 | 95.986038 |
| fireImage reversed if/else | 9324 | 93.157644 |
| fireImage ternary duplicated in both `iPad` arms | 9364 | 92.395437 |
| boost colour via `unsigned`/`int`/`float` local | 9264 | 86.354249 |
| `int frameH` | 9328 | 94.302492 |
| `int total` | 9328 | 93.970168 |
| `int barW` | 9328 | 94.097112 |
| `int frameH` + `int total` | 9328 | 93.822455 |
| `unsigned short frameH` | 9332 | 95.954307 |
| `unsigned int transferred` | 9332 | 95.954307 |
| `unsigned short total` | 9332 | 95.954307 |
| `unsigned int textH` | 9332 | 95.954307 |
| first-bar decl order sweep (24 perms of F,T,D,S) | — | `FTDS` best; next `TFDS` 95.556966 |
| anchor `Globals::Canvas && 1 <= b8` / `> 0` / extra parens | 9332 | 95.986038 |
| anchor `Globals::Canvas && b8 != 0` / `!!b8` | 9332 | 95.922577 |
| anchor reversed `b8 >= 1 && Globals::Canvas` | 9264 | 83.259212 |
| `gammaY`/`gammaBarY` statement swap | 9332 | 95.986038 |
| `gammaY`/`gammaBarY` hoisted before the `if` | 9336 | 63.020252 |
| `armorY`/`armorBarY` assignment swap | 9332 | 95.954307 |
| `armorY` as `int` | 9332 | 95.986038 |
| `armorBarY` as `short` | 9332 | 95.954307 |
| `dividerOffset` moved after armour decls | 9332 | 95.732191 |
| `dividerOffset` unsigned | 9332 | 95.986038 |
| `barOffset` unsigned | 9332 | 95.986038 |
| `barOffset` declared without init, set inside the if | 9260 | 83.253893 |
| `bool`/`unsigned char`/`int` local for `field_0x279` | 9332 | 95.668729 |
| second bar `top` as `unsigned short` | 9332 | 95.986038 |
| second bar `barYOffset` before `top` / after `y` | 9332 | 95.668729 |
| second bar `barH` as `unsigned int` | 9332 | 95.954307 |
| second bar `halfW` as `unsigned int` | 9332 | 95.922577 |
| second bar `fadeColor`/`barYOffset` unsigned | 9332 | 95.986038 |
| second bar `y` unsigned | 9332 | 95.954307 |
| second bar `progress` as `double` | 9344 | 92.895655 |
| second bar `halfW` moved after the `String` ctor | 9260 | 83.611509 |
| second bar `top`/`y` after `SetColor` | 9336 | 93.876904 |
| second bar `fadeColor` after `top` | 9332 | 95.763922 |
| second bar `y` after `SetColor` | 9320 | 90.929274 |
| `PaintCanvas *canvas` local for frameH/textH | 9332 | 95.812183 |
| `canvas` local for the whole first-bar block | 9312 | 89.993647 |
| `canvas` local from `barW` onward | 9312 | 90.608613 |
| `canvas` local, first two GetImage2D on `Globals::Canvas` | 9308 | 92.547275 |

## 3. Remaining deltas (official normalised diff, 101 hunks)

1. **Gamma block `0x164010`.**  Original keeps `&Globals::status` in `r6`,
   loads the value once into `r4`, then reloads `[r6]` for
   `getCurrentCampaignMission()` and spills `r6` to `[sp,#108]` *after*
   `getIndex()`.  Ours spills the address immediately and reuses `r4` for the
   second value load.  The faithful spelling (use `Globals::status` for arg2)
   reproduces the original's two-load shape exactly, but the `st` local is
   what reserves the extra 8-byte stack slot that keeps the frame at 224 B;
   without it clang allocates a 216-byte frame, every `[sp,#N]` shifts by 8,
   and the score collapses to 74.99 (9312 B).  No source spelling found that
   gives both the two-load shape *and* the 8-byte slot.
2. **Shield/gamma block `0x164044`.**  Original parks `field_0x44a` in `fp`
   and `field_0x448` in `r9`; ours is the other way round (a register
   allocator tie-break; statement/declaration order does not move it).
3. **`Globals::Canvas &&` anchor `0x1642f6`.**  Intentional: the original has
   no null test there, but the extra `ldr/cmp/beq` keeps the function at
   9332 B (removing it gives 9264 B / 83.29).  `Globals::Canvas` is the best
   anchor object (see `hud-draw-wave117-w6516.md`).
4. **Boost block `0x16566c`.**  Original calls `getBoostRate()` first, then
   loads `Globals::Canvas`, and emits `ite pl; mvnpl #255; mvnmi #208`.
   Ours caches Canvas in `r4` across the call (`mov r5,r4; ldr r4,[r9]` …
   `mov r0,r4; mov r4,r5`) and emits `mvn #255; it mi; mvnmi #208`.  A
   `float rate`/colour local shrinks the function to 9264 B / 86.35.
5. **Fire button `0x165726`.**  Original loads the marker image directly
   (`ldr r1,[sl,#736]`); ours computes the address first
   (`add r0,sl,#736; ldr r1,[r0]`) because clang merges the two ternary arms'
   loads.  if/else and `bool` spellings are neutral; duplicating the ternary
   per `iPad` arm is worse (9364 B).
6. **Docking bar `0x1658a0`.**  Original parks `textH` in `r9` and
   `&Globals::Canvas` in `fp` (`mov r5,fp; mov fp,r9; mov r9,r0`); ours parks
   `textH` in `fp` and `&Globals::Canvas` in `r6`.  All 24 declaration
   permutations of `frameH/textH/transferred/total` were swept; `FTDS`
   (current) is the best.
7. **Second bar `0x165ba6`.**  Original builds the `String` before calling
   `GetImage2DWidth`; ours calls `GetImage2DWidth` first.  Moving the
   `String` first is faithful but shrinks the function to 9260 B / 83.61
   (the `String` stack slot moves and the landing pads re-lay out).
8. **Second bar `0x165c6c`.**  Original schedules the `top` and
   `layout->field_0x218` loads after the `fade` multiply, around `SetColor`;
   ours loads them before.  Declaration order and `unsigned short`/`int`
   spellings do not move it.
9. **EH tail `0x165fb6`.**  Original has 4+2 `b.n` filler branches in the
   `String` destructor landing pads; ours has 3+3.  The Hud LSDA is exact.

## 4. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness: `/dev/shm/w6617/` — `score.py`, `bs.sh`, `try.py`,
`sweep_perm.py`, `sweep_types.py`, `opdiff.py` (official normalised diff),
`realdiff.py`, `range.py`, `o.asm`, `u.asm`.
