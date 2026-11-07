# `.ARM.extab` LSDA scope follow-up (w7681, second pass)

Status: research note, **no landed source change**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7681` (recreated after the worktree was removed
mid-session; branch `wave130-w7681` at `de8b8bd46`). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

| metric | before | after |
|---|---:|---:|
| `.ARM.extab` differing bytes | 1427 | **1427** |
| `.ARM.extab` differing records | 13 | **13** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` differing bytes | 0 | **0** |
| byte_exact / linked_exact | 4506 / 4506 | **4506 / 4506** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

Full `check` is green (`RATCHET PASS`, byte 4506, linked 4506, imports 207).

## New hypotheses tested this pass

All experiments compiled the real TU with the project flags and decoded the
object's `.ARM.extab.text.<mangled>` record with the committed decoder.  The
264-byte `PlayerJumpgate::PlayerJumpgate` body was preserved by every variant
unless noted; the LSDA row table was compared against the original:

```
orig (0,48,0) (48,16,238) (66,4,224) (74,8,238) (90,22,236)
     (126,12,234) (156,20,222) (202,8,238) (210,54,0)
ours (0,48,0) (48,8,236) (58,6,234) (66,4,224) (74,8,234)
     (90,48,238) (156,20,222) (202,8,236) (210,54,0)
```

The key grouping difference is the **if-body cleanup scope**:

* original pad A (238): `setVisible`, array `operator new`, `ArraySetLength`,
  `setRotation`; pad B (236): `inAlienOrbit/getSystem/getRace`; pad C (234):
  `setRadius` + sphere `operator new`.
* ours pad A (236): `setVisible`, `setRotation`; pad B (234): array `new`,
  `ArraySetLength`, `inAlienOrbit/getSystem/getRace`; pad C (238):
  `setRadius` + sphere `operator new`.

So the original's if-body scope starts **after** `ArraySetLength`, while ours
starts at the array `new`.  The following source shapes were tried and all
either leave the rows unchanged or move to an 8-row table:

1. `if (!visible) goto skip; ... skip:` early-skip body (8 rows; new pad 234,
   `ArraySetLength` pad 236, radius pad 236).
2. Single-statement comma-expression body:
   `if (visible) volumes = new ..., boundingVolumes = volumes,
    ArraySetLength(...), radius = ..., setRadius(radius), new BoundingSphere(...);`
   (8 rows, same as goto).
3. Inner `if (visible) { ... }` around the radius group, around
   `setRadius`+sphere, and around both (compiler folds the inner branch; rows
   unchanged).
4. Plain nested `{ }` around the whole radius group, around `setRadius`+sphere,
   and around the radius declaration (rows unchanged).
5. `if (1)` / `if (true)` around the pre-if read or body; `do { } while(0)`;
   `for(;;){break;}`; `switch(0){case 0:...}` (rows unchanged).
6. Allocation in the `if` condition via comma operator
   (`if (visible && (volumes = new ..., ..., true))`) (8/9 rows, new and
   `ArraySetLength` on different pads).
7. Direct assignment without the `volumes` local; assignment after
   `ArraySetLength`; assignment inside the declaration (rows unchanged or
   8 rows).

Conclusion: plain lexical scopes do not create ARM landing-pad blocks; only
EH cleanup scopes do.  The if-body boundary observed in our build is tied to
the `new` temporary's `ExprWithCleanups` scope.  Moving the array allocation
out of that scope changes the grouping, but no source spelling found so far
reproduces the original A/B/C split while keeping the 264-byte body.

## `RecordHandler::recordStoreRead` (3 bytes)

Rows 161/162/164 are the same two identical `String path` cleanup trampolines
(5078/5100) swapped.  New variants tested:

* `if (1) { AEFile::Read(equipmentCount, fd); }`
* `{ AEFile::Read(equipmentCount, fd); }`
* `do { AEFile::Read(equipmentCount, fd); } while (0);`
* `if (true) { ... }`

All leave the swap unchanged.  The two trampolines are distinguished by the
cleanup-scope depth, not by a plain block.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7681
cmake --build cmake-build-match --target check -j16
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1427, records 13, len-diff 0
```

Scratch harnesses: `/dev/shm/w7681/pjharness.py`, `/dev/shm/w7681/rhharness.py`,
`/dev/shm/w7681/objlsda.py`, `/dev/shm/w7681/try.sh`.
