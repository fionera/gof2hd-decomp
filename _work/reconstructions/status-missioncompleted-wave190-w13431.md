# `Status::missionCompleted` — wave 190 (w13431): rank helper in `.rodata`

Worktree `wt/w13431`, branch `wave190-w13431`, base master `ca62a0bda`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13431`.

**Status: landed.**  Raw **451 -> 115** (-336), aligned **205 -> 53**,
size **1,432/1,432** exact.  `.text` differing bytes **72,921 -> 72,585**
(-336); `.ARM.exidx` **0** (byte-identical, `0x6f70` @ `0x19cc08`);
`.ARM.extab` **603** (unchanged, size/base exact `0x26298` @ `0x1a3b78`);
build-id 20.  Total (text+extab+build-id) **73,544 -> 73,208** (-336).
`.rodata` base `0x1c9e10` exact and its original 269,159 B byte-identical;
`sh_size` **269,351 -> 269,644** (+0x1e5, the 484 B helper fills the
`.rodata`..`.fini_array` gap at `0x20b978`).  `.fini_array` 8 B @
`0x20cba0`; `.dynamic`/`.rel.dyn`/`.rel.plt`/`.data`/`.got`/
`.data.rel.ro`/`.init_array` byte-identical; 27/27 section headers; file
size 2,187,328 exact.  Gate: byte_exact 4507/4524, linked_exact
4507/4524, size 4524/4524, data 843/843, imports 207, stub/missing/extra
0, lint CLEAN, drift 0, parity 5370/5370 (`RATCHET PASS`).

## 1. What changed

`Status::missionCompleted` now switches on the value returned by a helper:

```cpp
extern "C" unsigned char __emutls_unregister_key(int type); // lint: extern_c
...
switch (__emutls_unregister_key(m->getType())) { ... }
```

`src/game/mission/StatusMissionTable.cpp` defines that helper as a real
ARM function (a `switch (type)` returning the same rank the old 192 B
rank table held) instead of the table itself.  The helper is emitted into
`.rodata` with `#pragma clang section text=".rodata"`, and the TU is
compiled with `-marm -fno-function-sections -fno-unwind-tables`
(`set_source_files_properties` in `CMakeLists.txt`).

The three case-body levers that the helper needs to keep the function
size-exact are the same ones measured by w13396/w13413/w13423: the
`case 10` second `getProductionGoodIndex()` query (restores the original
100 B body), the `case 12` ABC operand order, and rank 39 for type 172
(`case 39`).

## 2. Why the helper is a raw win

The old dispatch loaded the rank table with `ldr/add/ldrb` (6 B), so the
TBH sat 2 B late:

```
abfda  ldr r1,[pc,#804]      ; 49c9
abfdc  add r1,pc             ; 4479
abfde  ldrb r0,[r1,r0]       ; 5c08
abfe0  cmp r0,#39
abfe2  bhi
abfe4  tbh
```

The original is `sub.w r1,r0,#150` (4 B) + `cmp` + `bhi` + `tbh` at
`abfda..abfe2`.  A call is also 4 B, so `blx helper` puts `cmp`/`bhi`/
`tbh` back on the original addresses:

```
abfda  blx 20b978            ; f15f ecce
abfde  cmp r0,#39            ; 2827
abfe0  bhi.n                 ; d831
abfe2  tbh   [pc,r0,lsl #1]  ; e8df f010
```

Only the 4 B call word differs from the original's `sub.w`; the TBH and
its 40 rank-ordered halfwords land exactly, and the whole body run after
the dispatch is address-aligned.  That is the entire 336-byte drop.

## 3. Why the layout does not move

Three placement constraints had to be met at once (the previous
attempts in this pass shifted `.rodata` and lost ~1,250 byte-exact
functions):

1. **No interworking veneer.**  crtbegin's ARM `b __emutls_unregister_key`
   (`__on_dlclose_late`, `0x69548`) targets the helper.  A Thumb helper
   makes gold insert a 16 B ARM->Thumb veneer at the end of `.text`,
   growing `.text` by 0x10.  Compiling the helper `-marm` keeps the
   branch ARM->ARM (no veneer); the caller's Thumb `bl` becomes an
   in-place 4 B `blx`.  `.text` stays `0x1337c8` exact.
2. **No extra `.ARM.exidx` entry.**  `-fno-unwind-tables` makes the
   helper's `.ARM.exidx.rodata` entry a CANTUNWIND word (`0x1`).  Gold's
   `Arm_exidx_fixup` merges consecutive CANTUNWIND entries and deletes
   the new one (the preceding crtend entry is also CANTUNWIND), so
   `.ARM.exidx` stays byte-identical at `0x6f70` and `.ARM.extab`/
   `.rodata` do not shift.
3. **One plain `.rodata` input.**  `-fno-function-sections` keeps the
   helper in the plain `.rodata` section (not `.rodata.__emutls_...`),
   so it is the last `.rodata` input at `0x20b978` and the original
   269,159 B stay byte-identical.

The helper is 484 B (ARM switch with a jump table) and lives entirely in
the zero-filled `.rodata`..`.fini_array` gap, so the only section-size
delta is `.rodata` sh_size `269,351 -> 269,644` (+0x1e5) and the R E
LOAD FileSiz/MemSiz `0x20b977 -> 0x20bb5c`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13431
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=115 aligned=53 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 5. Files changed

* `src/game/mission/Status.cpp` — call the rank helper; case-10 re-query,
  case-12 ABC order, `case 39`.
* `src/game/mission/StatusMissionTable.cpp` — the rank table becomes an
  ARM rank-returning function in `.rodata`.
* `CMakeLists.txt` — `-marm -fno-function-sections -fno-unwind-tables`
  for `StatusMissionTable.cpp`.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-190 numbers.
* this note.
