# `Level::createScene` — wave-136 w8120: the `spots` ternary and the `geo` load
# placement are the last two block/register deltas; function is now byte-exact

**Status:** landed, 2026-11-07. Worktree `wt/w8120`, base `2e51d765d`.
Target `_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`, `createScene`
only). Gate re-measured green with a **gain**: **byte_exact 4506 -> 4507**,
**linked_exact 4506 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, symbol parity 5370/5370, avg 99.94 %. The function is now
**byte-identical** (2252 B at the original `0xb42ec`), so 17 functions remain
non-byte-exact.

| | pct | size | insns | own | capacity | spotUsed | status |
|---|---|---|---|---|---|---|---|
| before (wave-129) | 96.490108 | 2252 | 783/784 | r4 | r6 | r4 | sl |
| **after (this pass)** | **100.0** | **2252** | **783/783** | r4 | r6 | r4 | sl |

Section deltas: `.text` differing bytes **84,960 -> 84,724** (-236, exactly the
createScene delta); `.ARM.extab` **1,404 -> 1,395** (-9, the createScene LSDA);
`.ARM.exidx` / `.rodata` / `.data` / `.bss` / `.rel.plt` stay exact;
`.note.gnu.build-id` 20.

## The two changes

Both are in the ship loop of `createScene`:

```diff
-            const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];
+            const int *spots;
+            if (sceneRace == 7)
+                spots = kHangarSpotsKhador[0];
+            else
+                spots = kHangarSpotsDefault[0];
...
-                AEGeometry *geo = ship->geometry;
                 ship->player->setAlwaysFriend(true);
                 ship->setToSleep();
+                AEGeometry *geo = ship->geometry;
                 geo->setRotation(0.0f, (float) rnd->nextInt(300) / 100.0f, 0.0f);
```

Semantics are unchanged (the ternary and the if/else compute the same pointer;
`geo` is only read after `setToSleep()`).

## Why the ternary was the largest hunk (236 B)

The residual was one extra `b.n` plus a duplicated `spots` reload in the
`switch (sceneRace)` hole-check. Original (`0xb4a6e`):

```
b4a6e ldr r1,[sp,#52]   ; sceneRace
b4a70 cmp r1,#8
b4a72 ldr r1,[sp,#44]   ; spots   <- phi copy inline in the range-check block
b4a74 bhi.n b4a88       ; -> join
b4a76 ldr r1,[sp,#16]   ; bitmask
b4a78 cmp r1,#0
b4a7a ldr r1,[sp,#44]   ; spots   <- phi copy inline in the bit-check block
b4a7c beq.n b4a88       ; -> join
b4a7e ldr r1,[pc,#308]  ; switch.table
b4a80 ldr r2,[sp,#52]
b4a82 add r1,pc
b4a84 ldr.w r1,[r1,r2,lsl#2]
b4a88 add.w r0,r0,r0,lsl#1   ; join, fall-through, NO branch
```

Before this pass we emitted a split block for the range-check edge
(`bhi -> 0x79c`), a `b.n` after the table load, and a `nop`:

```
782 ldr r1,[sp,#52]
784 cmp r1,#8
786 bhi.n 79c
788 ldr r1,[sp,#16]
78a cmp r1,#0
78c ldr r1,[sp,#28]   ; spots (bit-check copy)
78e beq.n 79e
790..796 table load
79a b.n 79e
79c ldr r1,[sp,#28]   ; spots (split range-check copy)
79e join
```

`SimplifyCFG::SwitchToLookupTable` lowers the switch to
`%256 = (sceneRace >= 9) | (bit == 0)`; `SelectionDAGBuilder::FindMergedConditions`
splits that `or` into the two branches, and the join phi is
`%53 = PHI [%49, range-check], [%49, bit-check], [%52, table]`. Because the
ternary's `spots` value is a single SSA value used by *both* phi operands,
`PHIElimination::SplitPHIEdges` sees `%49` live at the start of the bit-check
block (`isLiveOutPastPHIs` true) and splits the range-check->join critical edge
into a new block, leaving the `spots` load in that block and forcing the extra
`b.n`.

Rewriting the ternary as an if/else gives the two stores distinct SSA values
(the else arm's `kHangarSpotsDefault[0]` is materialised on its own path), so the
join phi operands are no longer the same vreg, `isLiveOutPastPHIs` is false, and
PHIElimination keeps both copies inline -- exactly the original layout. The
`default: table = spots;` switch itself is unchanged.

An equivalent fix is to recompute the select in the switch's `default:` arm
(`table = (sceneRace == 7) ? &kHangarSpotsKhador[0][0] : &kHangarSpotsDefault[0][0];`),
which scores the same; the if/else was chosen because it is the smaller,
more readable change.

## The `ldrd` (second change)

With the switch fixed the function was 2248 B (4 short): the remaining delta was
`ldrd r0, fp, [sl, #4]` fusing `ship->player` and `ship->geometry`, where the
original has two separate loads (`ldr.w r0,[sl,#4]` at `0xb4ac8` and
`ldr.w fp,[sl,#8]` at `0xb4ae0`, the latter after `setToSleep()`). Declaring
`geo` after `setToSleep()` splits the loads and restores the 2252 B size. The
wave-129 note reported that this move "reverts the whole ship-loop register
map"; with the switch fix in place the map is stable (`own=r4`, `capacity=r6`,
`spotUsed=r4`, `status=sl`), so it is now free.

After both changes `verify --show` reports no structural and no link-address
diffs: the function is byte-identical.

## Measured variants (score3 structural %, `orig 811` normalised insns)

| variant | pct | insns | switch shape |
|---|---|---|---|
| base (ternary) | 95.297806 | 784 | split block + extra `b.n` |
| `const int *table = spots;` + switch | 95.297806 | 784 | same |
| `default: table = spots;` (base) | 95.297806 | 784 | same |
| `spots` moved into the loop | 95.297806 | 784 | same (LICM hoists it) |
| `spots` moved after the loop | 95.297806 | 784 | same |
| **`spots` as if/else** | **96.296296** | **782** | **original** |
| `default:` recomputes the select | 96.296296 | 782 | original |
| `-mllvm -disable-phi-elim-edge-splitting` | 96.296296 | 782 | original (but changes 5 other Level.cpp functions) |
| if/else + `geo` after `setToSleep()` | **98.243413 -> 100.0** | **783** | original |

The `-mllvm -disable-phi-elim-edge-splitting` TU flag was rejected: it also
shrinks `createPlayer` (-20), `assignGuns` (-16), `getBoundingVolume` (-4),
`createStaticObject` (-4) and grows `createAsteroids` (+8), all of which are
size-exact in the original, so it shifts `.text` by 8 bytes and drops
byte_exact to 2478. It was useful only as an oracle that confirmed the target
block shape.

## Reproduction

Scratch is `/dev/shm/w8120` (`Level.cpp.base` pristine, `fast.sh` single-TU
compile, `score3.py` structural diff, `swcheck.py` switch-region dump,
`tryvar.py` variant harness).

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump $OBJDUMP --show _ZN5Level11createSceneEv
cmake --build cmake-build-match --target check -j6   # byte 4507 linked 4507
```
