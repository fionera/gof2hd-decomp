# LevelScript::process — wave 157 / w10303: mission-0 loop `continue` guard
# (raw unchanged 55,095, aligned -2, size 57,232 B)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10303`, base master
`08575efff` (wave 156).  Scratch `/dev/shm/w10303/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,095            55,095            (0)
process aligned insns    17,131            17,129            (-2)
mission-0 region nonmatch 714              712               (-2)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,831            85,831            (0)
.ARM.extab diffbytes     1,292             1,292             (0)
.note.gnu.build-id       20                20
file size                2,187,328         2,187,328         (exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. The landed source change

```diff
-            for (int i = 0; i < 3 && enemies->data_ != nullptr; ++i) {
+            for (int i = 0; i < 3; ++i) {
+                if (!enemies->data_) continue;
                 KIPlayer *e = (*enemies)[i];
                 e->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
                 e->geometry->setVisible(true);
             }
```

The w10102 shape folded the `data_` null guard into the `for` condition,
which made the compiler emit `cmp r5,#2; bls` for the back-edge (the guard
is a second loop exit).  Hoisting the guard to a `continue` inside the body
restores the original's `cmp r5,#3; bne` back-edge while keeping the same
`cbz` guard and the same single element load.  The raw diff is unchanged
(the guard still costs the same 8 bytes) and the aligned metric drops 2.

## 2. Mission-0 block map (deliverable)

The mission-0 block is `0x137d18..0x13853a` in the original; ours is
`0x137d18..~0x1384c8`.  Region nonmatch (global alignment restricted to the
block) = **712** after this change (was 714).

| sub-block | original | ours | state |
|---|---|---|---|
| prologue / dispatch | `0x137d18..0x137d82` | `0x137d18..0x137d88` | `this`/canary swapped (`sl`/`fp`), status CSE |
| mission-1 intro tail | `0x1383a0..` (elsewhere) | `0x137e84..0x137e88` | block-order difference |
| state-0 setup + loop | `0x137e8a..0x137f76` | `0x137e8a..0x137f72` | loop guard now `continue` |
| shared tail | `0x137f7c..0x13812a` | `0x137f78..0x138254` | `sl`/`fp` swap |
| state-1/2/3 block | `0x1383fe..0x13853a` | `0x13838c..0x1384c8` | instruction-identical modulo `sl`/`fp` |

The state-3 `setExhaustVisible` loop (`0x1384f8..0x138536` original,
`0x138488..0x1384c6` ours) is already instruction-for-instruction identical.

### The state-0 loop (original vs landed)

```
original (0x137f34)                 landed (0x137f36)
ldr.w r0,[r9,#4]      ; data_       ldr  r0,[sp,#96]   ; enemies
mov   r1,r8                         ldr  r0,[r0,#4]    ; data_
mov   r2,fp                         cbz  r0, .inc      ; if (!data_) continue
ldr.w r0,[r0,r5,lsl#2]              ldr.w r4,[r0,r5,lsl#2]
ldr   r0,[r0,#8]                    mov  r1,r8
strd  r4,r6,[sp,#368]               mov  r2,r9
str   r6,[sp,#376]                  ldr  r0,[r4,#8]
strd  r6,r4,[sp,#280]               strd sl,r6,[sp,#368]
str   r6,[sp,#288]                  str  r6,[sp,#376]
blx   setDirection                  strd r6,sl,[sp,#280]
ldr.w r0,[r9,#4]      ; data_       str  r6,[sp,#288]
movs  r1,#1                         blx  setDirection
ldr.w r0,[r0,r5,lsl#2]              ldr  r0,[r4,#8]
ldr   r0,[r0,#8]                    movs r1,#1
blx   setVisible                    blx  setVisible
adds  r5,#1                         adds r5,#1
cmp   r5,#3                         cmp  r5,#3
bne.n 0x137f34                      bne.n 0x137f36
```

Both are 18 instructions.  The remaining deltas are (a) `enemies` is on the
stack in ours (`ldr r0,[sp,#96]`) vs `r9` in the original, (b) the `cbz`
guard, (c) the element is cached in `r4` so the second geometry load is a
single `ldr r0,[r4,#8]` instead of the original's `data_`/element/geometry
reload.  These are the w10102 correlated deltas, not new.

## 3. Sweep (all built + linked on the wave-156 base)

`raw` = same-address diff bytes; `aligned` = global instruction alignment;
`region` = mission-0 block nonmatch.  All size 57,232 unless noted.

| loop shape | raw | aligned | region |
|---|---:|---:|---:|
| HEAD `for (i<3 && data_)` + `e` (w10102) | 55,095 | 17,131 | 714 |
| **`for (i<3)` + `if (!data_) continue;` + `e` (landed)** | **55,095** | **17,129** | **712** |
| `for (i<3)` + `if (!enemies) continue;` + `e` | 55,098 | 17,128 | 711 |
| `for (i<3)` + `if (e) { ... }` | 55,096 | 17,130 | 713 |
| `for (i<3)` + `if (!e) continue;` | 55,096 | 17,130 | 713 |
| `for (i<3)` + `if (!e) break;` | 55,096 | 17,132 | 715 |
| `for (i<3)` + `if (data_) { ... }` | 55,095 | 17,129 | 712 |
| `for (i<3)` + `if (!data_) break;` | 55,095 | 17,131 | 714 |
| `for (i<3)` + `if (data_) continue;` + `enemies->data_[i]` | 55,095 | 17,129 | 712 |
| `for (i<3)` + `if (!enemies || !data_) continue;` | 55,279 | 17,153 | 733 |
| `for (i<3 && data_)` + `if (!enemies) continue;` | 55,095 | 17,131 | 714 |
| `for (i<3)` + `enemyData` local (pre-w10102) | 55,299 | 17,126 | 709 |
| `for (i<3 && data_)` + `enemyData` local | 55,275 | 17,146 | 726 |
| `for (i<3)` + `enemyData` local + `if (!e) continue;` | 55,096 | 17,130 | 713 |
| `for (i<3)` + `e` (no guard) | 55,120 | 17,149 | 724 |
| `for (i<3)` + `enemyData` local + `if (!data_) break;` | 55,275 | 17,146 | 726 |
| faithful double `(*enemies)[i]` (no guard) | 55,478 | 21,883 | 1,087 |
| faithful double `(*enemies)[i]` + `data_` bound | 55,477 | 21,885 | 1,087 |
| hoist `data_` + single element local | 55,466 | 21,280 | 1,112 |
| pointer-increment `p = data_` | 55,159 | 19,480 | — |
| `setVisible` before `setDirection` (w10102 shape) | 55,095 | 17,137 | — |
| `setVisible` before `setDirection` + `continue` | 55,097 | 17,135 | 718 |
| `for (i != 3)` + `continue` | 55,098 | 17,128 | 711 |
| `if (!enemies->data_) continue;` + `enemies->data_[i]` | 55,095 | 17,129 | 712 |

The `-4` target was not reached: the best raw-preserving variant is `-2`
(17,131 -> 17,129).  The pre-w10102 `enemyData`-local shape reaches
`aligned 17,126` (`-5`) but costs `+204` raw and is rejected by the
"keep the raw win" constraint.  `if (!enemies) continue;` reaches
`aligned 17,128` (`-3`) at `+3` raw.

## 4. Why `-4` is hard here

The mission-0 block's remaining 712 mismatches are dominated by the
whole-function `this`=`sl`/canary=`fp` swap (ours is `fp`/`sl`) and the
`&Globals::status` vreg living in `r9` instead of being spilled to
`[sp+92]`.  Both are greedy-allocator outcomes documented in w10007 /
w10101; no source shape found this wave moves them.  The loop itself
contributes ~19 nonmatches, of which the `enemies`-on-stack load and the
`cbz` guard are the only ones still reachable from source.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10303/fdiff.py _ZN11LevelScript7processEi        # 55,095 / 57,232
python3 /dev/shm/w10303/align_diff.py _ZN11LevelScript7processEi  # 17,129
python3 /dev/shm/w10303/blocks.py _ZN11LevelScript7processEi 0x137d18 0x138d2a  # 712
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## 6. Open questions

1. A shape that puts `enemies` in `r9` (the original's `ldr.w r9,[sp,#96]`
   before the loop) without the `(*enemies)[i]` double-load re-colour would
   likely remove the remaining loop nonmatches.  The allocator lever is not
   found.
2. The `this`/canary swap is the dominant mission-0 delta; matching it would
   remove hundreds of aligned nonmatches across the whole function.
3. The pre-w10102 `enemyData` shape is aligned `17,126`; a `-204` raw
   companion that does not re-colour would let it land.
