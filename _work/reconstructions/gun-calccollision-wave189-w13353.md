# `Gun::calcCharacterCollision` wave 189 (w13353): the `i*3`/`&stack.10`
# r4/r5 swap is a greedy-RA size-priority artefact; ~120 new cells, no
# size-exact raw win (458/63/1984 stays)

Worker: w13353. Branch `wave189-w13353`, base master `fbc94984a`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13353`.

## 0. Result

**No `src/` change landed.**  Every size-exact shape measured this wave is
equal to or worse than the committed body.  Tree clean apart from this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `fbc94984a`) | **458** | **63** | 1984/1984 |
| after this pass (note only) | **458** | **63** | 1984/1984 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
`RATCHET PASS`, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, `.rel.plt` byte-identical, 27/27 headers, file size 2,187,328.

## 1. New root-cause evidence: the swap is a *greedy priority* decision

The wave-189 w13327 note identified the `r4`/`r5` swap as "a single
Early-Machine-LICM/remat decision on the `&view` stack address".  This wave
pins down the mechanism by reading the actual LLVM 7.0.2 sources
(`release_70` on llvm-mirror) and the full `-print-after-all` MIR of the
committed body (`scratch/w13353/base_all.mir`, 195 k lines, filtered).

1. `MachineLICMBase::IsProfitableToHoist` (MachineLICM.cpp) tests
   `TII->isTriviallyReMaterializable(MI, AA)` **before** the register
   pressure check.  `t2ADDri %stack.10, 0` has no virtual-register uses and
   no side effects, so `isReallyTriviallyReMaterializableGeneric`
   (TargetInstrInfo.cpp) returns true and the ADDri is **always** hoisted,
   regardless of `-hoist-cheap-insts` (which defaults to *false*).  This
   means the hoist itself is not a source-shape lever: any `Matrix view`
   (or `Vector` sret temp) in the loop produces one.
2. `RAGreedy::enqueue` (RegAllocGreedy.cpp) orders the work list by
   `Prio = (1u << 29) + Size` for *global* (multi-MBB) live ranges —
   larger ranges first.  The hoisted `%308 = t2ADDri %stack.10` is defined
   in the e-loop preheader (`bb.5`) and used as late as the scatter block,
   so its range is strictly larger than `i*3`'s (`add.w rX, fp, fp, lsl #1`,
   defined at the top of the i-loop body).  Greedy therefore assigns
   `%308 -> r4` first, and `i*3 -> r5`.  In the shipped body `i*3` is r4
   and `&stack.10` is r5, i.e. the shipped range sizes must have been the
   other way round.
3. The `%308` live range is load-bearing for the 2-byte shift at
   `0x153198` (`add r4,sp,#104` in ours vs `ldr.w fp,[sp,#40]` in the
   shipped body).  That 2-byte shift moves the whole collision-check block
   by 2 bytes and is responsible for the 158-byte run at
   `0x1531b3-0x153251` and the 42-byte run at `0x153252-0x15327c`, i.e.
   roughly 200 of the 458 raw-diff bytes (measured with
   `scratch/w13353/ranges.py`).

## 2. Confirmation that `i*3 -> r4` alone is not enough

Two families of source shapes reach the shipped `i*3 = r4` allocation but
are not size-exact:

| variant | raw | aligned | size | frame | i3 |
|---|---:|---:|---:|---:|---|
| committed | **458** | **63** | 1984 | 208 | r5 |
| `Matrix view;` at i-loop scope, assigned in the view block | 1880 | 379 | 2024 | 224 | **r4** |
| `Matrix view;` at e-loop scope, assigned in the view block | 1880 | 365 | 2024 | 224 | **r4** |
| `Vector playerPosition;` at i-loop scope, assigned in branch | 1614 | 270 | 1964 | 224 | **r4** |
| `Vector playerPosition;` at e-loop scope, assigned in branch | 1682 | 270 | 1964 | 224 | **r4** |

The 40-byte growth is the out-of-line `Matrix()` default constructor
(15 identity stores, not DSE'd through the `view = *(...)` copy); the
20-byte shrink of the `playerPosition` variants is the extra 16-byte frame
plus a different damage-arm layout.  So the shipped body must have
`i*3 = r4` **and** the original 208-byte frame, which needs the 12-byte
`playerPosition`/`delta` temps to keep sharing one of the two 60-byte
Matrix slots (sp+44 = `MatrixSetTranslation` sret temp, sp+104 = `view`).

## 3. Cells measured this wave (full `link_so` + `tools/fn_diff.py`, `-j2`)

All variants keep the function at 1984 B unless stated.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **458** | **63** | 1984 |
| view block: `Matrix &viewRef`, `Matrix *vp`, direct-init, `Matrix *src` local | 458 | 63 | 1984 |
| view block: `PaintCanvas *canvas` local / `unsigned cur` local | 1343 / 1447 | 167 / 288 | 1984 / 1968 |
| view block: `view = MatrixSetTranslation(view,...)` | 1097 | 107 | 1992 |
| view block: `Matrix ret = MatrixSetTranslation(...)` | 1255 | 145 | 1988 |
| view block: `unsigned geom = this->geometries[i]` | 1154 | 153 | 1988 |
| view block: `memcpy` / `__builtin_memcpy` | 1923 | 249 | 2064 |
| view block removed / only the copy | 1670 / 1221 | 373 / 180 | 1896 / 1960 |
| `playerPosition` as `const` / `const&` / `&&` | 458 | 63 | 1984 |
| `expr` form (`VectorLength(enemyPosition - getPlayer()->getPosition())`) | 462 | 73 | 1984 |
| `distance` via `float` temp | 458 | 63 | 1984 |
| `Player *pl = level->getPlayer()` | 458 | 63 | 1984 |
| e/i/k loop: `int`, post-inc, `!=`, `size()`, cached count, `unsigned long` | 458 (459 for `e !=`) | 63/64 | 1984 |
| `positions.data()+i` / `velocities.data()+i` / `enemies->data()[e]` | 458 | 63 | 1984 |
| `hitFlags`/`hitPositions` pointer form, `hitFlags`/`hitPositions` swap | 458 / 479 | 63 / 69 | 1984 |
| `if (scatter)` -> `if (weaponType == ITEM_SORT_SCATTER_GUN)` | 458 | 63 | 1984 |
| `k < size` -> `size > k` / `k <= size-1` | 458 / 946 | 63 / 95 | 1984 / 1984 |
| `falloff` two `if`s instead of `else if` | 1027 | 121 | 1960 |
| `empDamage >= 1` | 458 | 63 | 1984 |
| `td` removed / `Vector &td` / `td` inside the scatter block | 458 | 63 | 1984 |
| `mine` block `VectorNormalize` into a named `Vector nv` | 487 | 111 | 1984 |
| k-loop `Vector ep = getPosition(); ep -= position;` | 1880 | 461 | 1876 |
| k-loop `Vector ep = getPosition() - position;` | 1789 | 288 | 1952 |
| `weaponType`/`bomb`/`mine`/`scatter`/`explosive` as `int`/`bool` | 458 | 63 | 1984 |
| `explodeOnContact = bomb || mine;` instead of the `if` | 458 | 63 | 1984 |
| `lastHitKIPlayer` before/after `td` | 458 | 63 | 1984 |
| tail adjacent swaps (`field_54`/`playerGun`/`setHitVector`/`lifetimes`/`lastHitKIPlayer`) | 1154 / 1439 / 487 / 474 | 107/135/69/69 | 1984/1980/1984/1984 |
| i-loop adjacent swaps (`position`/`velocity`/`enemyPosition`/`radius`) | 473 / 503 / 518 | 86/87/72 | 1984 |
| `radius` before `enemyPosition` / `enemyPosition` before `velocity` | 518 / 503 | 72 / 87 | 1984 |
| `playerPosition` before `radius` | 1714 | 381 | 1940 |
| tail block order (all 6 permutations of explosive/impact/scatter) | 1221..1906 | 129..392 | 1900..1984 |
| `__builtin_expect(...,0/1)` on `impact->isRocket`, `impact!=null`, `empDamage`, `owner`, `bomb||mine`, `enemies==null`, `isActive`, `hitpoints`, `mine`, `isAsteroid`, `explodeOnContact`, `playerGun`, `geometries`, `scatter`, `useCustomRadius`, `ki==null`, `!egoFlag`, `outer`, `mine`/`scatter` init, k/i/e loop conditions | 555..1888 | 63..530 | 1880..1992 |
| `Matrix view` at function / e-loop / i-loop scope, assigned in block | 1884 / 1880 / 1880 | 408/365/379 | 1960/2024/2024 |
| `Vector playerPosition;` at i-loop / e-loop scope, assigned in branch | 1614 / 1682 | 270 | 1964 |

No size-exact shape beats 458/63/1984.

## 4. Conclusion

The residual is a genuine greedy-RA plateau, not a source-shape mistake:

* the hoisted `&stack.10` ADDri is *always* hoisted (trivially
  rematerializable) and its global live range starts in the e-loop
  preheader, so `RAGreedy` (size-descending for globals) always gives it
  r4 before `i*3`;
* reaching `i*3 = r4` requires moving `Matrix view` / `Vector
  playerPosition` out of the conditional, which either grows the frame
  (224 B) or emits the out-of-line `Matrix()` ctor (+40 B);
* the `&stack.10` remat at `0x153198` is what produces the 2-byte shift
  that accounts for ~200 of the 458 raw-diff bytes.

The committed body remains the best measured shape: **458 / 63 / 1984**.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13353
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=458 aligned=63 size=1984/1984`, `RATCHET PASS`,
byte 4507 / linked 4507, data 843/843, imports 207, lint CLEAN, drift 0.

Machine-IR evidence (scratch, not tracked):
`scratch/w13353/base_all.mir` (`-mllvm -print-after-all
-mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv`), plus the
LLVM 7.0.2 sources fetched into `scratch/w13353/{licm,tii,rag,looprotu}.cpp`.

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave189-w13353.md` only.
