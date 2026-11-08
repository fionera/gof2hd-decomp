# LevelScript::process — wave 162 / w10802: mission-95/99 arm block placement

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10802`, base master
`29248c1d0` (wave 161).  Scratch `/dev/shm/w10802/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    55,013
process aligned insns    17,308
orig / ours size         57232 / 57232     (size-exact)
.text diffbytes          85,374
.ARM.extab diffbytes     725   (size 156,312 exact)
.rel.plt                 0     (BYTE-IDENTICAL, verified byte-for-byte)
.rodata/.data/.bss/.ARM.exidx  byte-identical
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
lint CLEAN / drift 0 / RATCHET PASS
```

## 1. The mission-95/99 arm in the base already matches the original

The original's arm block (0x13c4e0) is:

```
13c4e0  ldr.w r0,[sl,#28]   ; m_nState   (sl = this)
13c4e4  ldr r2,[sp,#92]     ; &Globals::status  (spilled)
13c4e6  cmp r0,#2
13c4e8  bne.w 13df76        ; not state 2 -> far shared `movs r5,#1; b.w epilogue`
13c4ec  ldrd r0,r1,[r4]     ; m_nScriptTimerA
13c4f0  subs.w r0,r0,#2000
13c4f4  sbcs.w r0,r1,#0
13c4f8  blt.w 13df76
13c4fc  ldr r0,[r2,#0]      ; status
13c4fe  blx getCampaignMission
13c502  movs r1,#1
13c504  movs r5,#1          ; return value materialised before the call
13c506  blx setStatusValue
13c50a  b.w 144f60          ; epilogue, no canary reload
```

The **base** lowers this to (0x13caa4):

```
13caa4  ldr.w r0,[fp,#28]   ; m_nState   (fp = this)
13caa8  cmp r0,#2
13caaa  bne.w 13ee92        ; far shared return-1 block
13caae  ldrd r0,r1,[r4]
...
13cabe  ldr.w r0,[r9]       ; status     (r9 = &Globals::status)
13cac2  blx getCampaignMission
13cac6  movs r1,#1
13cac8  movs r5,#1
13caca  blx setStatusValue
13cace  b.w 144ff4          ; epilogue, no canary reload
```

Structurally identical: no extra branch, no `ldr.w fp,[sp,#68]`, and the
"not state 2" path is a *separate* far return-1 block (13ee92), exactly as
the original's 13df76.  The only differences are the `this`/canary register
tie (`fp` vs `sl`) and `&Globals::status` living in `r9` instead of being
spilled to `[sp,#92]`.

## 2. Where the +44 comes from (confirmation of w10701)

The plain declaration reorder `messages,status,enemies,player` with a
`Status *status` local gives **55,416 / 17,149 / 57,300 (+68)** on this
tree.  Its block (0x13cadc) is:

```
13cadc  ldr.w r0,[sl,#28]
13cae0  cmp r0,#2
13cae2  bne.n 13cb04        ; <-- short branch, not .w
13cae4  ldrd r0,r1,[r4]
...
13caf2  ldr.w r0,[fp]       ; fp = &Globals::status (fp was the canary base!)
13caf6  blx getCampaignMission
13cafa  movs r1,#1
13cafc  movs r5,#1
13cafe  blx setStatusValue
13cb02  b.n 13cb06          ; <-- extra b.n
13cb04  movs r5,#1          ; <-- merged fall-through return value
13cb06  ldr.w fp,[sp,#68]   ; <-- canary-base reload
13cb0a  b.w 145038
```

The reorder's allocator puts `&Globals::status` in `fp` (the canary-base
register) and spills the canary base to `[sp,#68]`.  The "not state 2"
return-1 block then lands immediately after the setStatusValue block, so
SimplifyCFG tail-merges the two `movs r5,#1` + reload + `b.w` tails.  The
merged tail is the +44 (extra `b.n`, extra `movs`, the canary reload, and
the literal-pool shift).

## 3. A declaration order that reproduces the original block

`messages,player,status,enemies` with a plain `Status *status` local gives
**54,847 / 17,504 / 57,264 (+32)** and its block at 0x13cad6 is *clean*:

```
13cabc  ldr.w r0,[r9,#28]
13cac0  cmp r0,#2
13cac2  bne.w 13eeae        ; far shared return-1 block
13cac6  ldrd r0,r1,[r4]
...
13cad6  ldr.w r0,[fp]       ; status
13cada  blx getCampaignMission
13cade  movs r1,#1
13cae0  movs r5,#1
13cae2  blx setStatusValue
13cae6  b.w 145014          ; no extra branch, no canary reload
```

So the +44 is removable by source: it is the `messages,status,enemies,player`
order that triggers the tail merge, not the arm itself.  The +32 residual of
this order is *not* localised — the base-vs-variant address delta oscillates
between -12 and +28 across the whole function (2-byte vs 4-byte encodings)
and ends at +32, so there is no single -32 companion block.

## 4. New lead: dropping the `isRunningHDonWeakDevice` expect

Dropping the `__builtin_expect(Globals::isRunningHDonWeakDevice, 1)` hint at
the mission-157 state-5 `destroyed` loop (source line ~4861) gives:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 55,013 | 17,308 | 57,232 |
| **base − isRunningHD expect** | **54,557** | **17,338** | **57,240 (+8)** |
| base − isRunningHD expect + `Status *&` | 54,558 | 17,325 | 57,240 (+8) |

This is a **-456 raw** move and it also moves the cold `isDead` loop from the
function tail (0x145bd0) back into the body (0x144370, original 0x144a22),
i.e. it fixes a real block-placement divergence.  The +8 is again a
cumulative 2/4-byte encoding effect (delta oscillates ±12 and ends at +8),
so it is gate-fatal today but is the strongest raw lead found on this tree.
A future wave should hunt the -8 encoding companion.

## 5. Other measured (all full link)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| base | 55,013 | 17,308 | 57,232 | |
| **base + `Status *&status`** | **55,014** | **17,295** | **57,232** | landed |
| base, no `Status` local (all `Globals::status->`) | 55,014 | 17,295 | 57,232 | identical codegen to `Status *&` |
| `messages,player,status,enemies` | 54,847 | 17,504 | 57,264 | clean arm, +32 |
| `messages,status,enemies,player` (plain reorder) | 55,416 | 17,149 | 57,300 | +44 tail merge |
| base + `expect(m_bStartSequence != 0, 0)` | 55,391 | 16,940 | 57,268 | flips tie (prologue byte-identical) but moves block cold |
| base + `expect(m_bStartSequence != 0, 1)` (w10705) | — | — | 57,420 | |
| base + state-3/5 swap (4,3,5) | 55,508 | 17,978 | 57,240 | w10704's 57,232 no longer holds after wave 161 |
| base + `||` chain reversed | 55,013 | 17,350 | 57,232 | no raw change |
| base + `||` chain 95/99 moved last | 55,013 | 17,344 | 57,232 | no raw change |
| base + hoist `status` ptr in arm condition | 55,232 | 18,349 | 57,328 | +96 |
| base + L4026:1 | 55,016 | 17,315 | 57,232 | |
| base + L1532:0 | 55,275 | 17,378 | 57,232 | |
| base + L728:0 | 55,209 | 17,303 | 57,240 | |
| base + L1031:1 | 54,872 | 18,427 | 57,264 | |
| base + L931:1 | 54,872 | 18,427 | 57,264 | |
| base + state-3/5 swap + `messages,player,status,enemies` | 55,403 | 17,879 | 57,292 | |
| 24 declaration permutations (plain `Status *`) | best size-exact = base 55,013 | | | |
| 24 declaration permutations (`Status *&`) | all size-exact; best 55,014/17,295 | | | |

## 6. Landed change (1 line)

```cpp
-    Status *status = Globals::status;
+    Status *&status = Globals::status;
```

A reference to the global pointer makes every `status->` use re-read
`Globals::status`, matching the original's per-use reload (w10601/w10605/
w10701).  It is semantically the same as writing `Globals::status->` at
every use (the no-local form produces identical codegen), but keeps the
source readable.  Size stays 57,232 B, `.rel.plt` stays byte-identical,
and the aligned metric improves 17,308 -> 17,295.

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10802/fdiff.py _ZN11LevelScript7processEi        # 55,014 / 57,232
python3 /dev/shm/w10802/align_diff.py _ZN11LevelScript7processEi  # 17,295
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. The `-456` raw lead (drop the `isRunningHDonWeakDevice` expect) needs an
   -8 B encoding companion to become size-exact.  The +8 is cumulative, not
   a single block.
2. The `this`/canary tie flip (`expect(m_bStartSequence != 0, 0)`) makes the
   prologue byte-identical but moves the m_bStartSequence block cold (+36).
   A non-expect source shape that flips the tie without moving the block
   would be a very large raw win.
3. The `messages,player,status,enemies` order has the original's clean arm
   block but +32 B of cumulative encoding drift.
