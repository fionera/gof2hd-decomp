# `.ARM.extab` LSDA cleanup scopes: `readOptionsFileAsByteArray` (w7642)

Status: research note + one landed source fix, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7642`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).

## Result

| metric | before | after |
|---|---:|---:|
| `.ARM.extab` differing bytes | 1436 | **1433** |
| `.ARM.extab` differing records | 15 | **14** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` differing bytes | 0 | **0** |
| byte_exact / linked_exact | 4506 / 4506 | **4506 / 4506** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`.ARM.extab` remains size- and base-exact at `0x26298` @ `0x1a3b78`;
`.text` `0x1337c8` @ `0x69440`, `.ARM.exidx` `0x6f70` @ `0x19cc08`,
`.rodata` `0x41b67` @ `0x1c9e10` are unchanged. `.rel.plt` (0x72d0) is still
byte-identical.

## The landed fix: `RecordHandler::readOptionsFileAsByteArray`

Source (`src/game/mission/RecordHandler.cpp`):

```diff
     if (AEFile::FileExist(tmp) != 0) {
         unsigned int fd;
         AEFile::OpenRead(tmp, &fd);
+        signed char *buf;
         sz = AEFile::GetFileSize(fd);
-        signed char *buf = new signed char[sz];
+        buf = new signed char[sz];
         *out = buf;
         AEFile::Read(sz, buf, fd);
         AEFile::Close(fd);
```

This is semantics-preserving and the emitted body stays **byte-exact**
(140 B, `byte: true`, `linked: true`). It changes only the IR cleanup scope:
`buf` is now in scope (and part of the cleanup region) at the
`GetFileSize` call, so clang gives `GetFileSize` its own landing pad instead
of merging it into the `OpenRead` call site.

Decoded LSDA call-site tables (record owner is the function at the exidx
entry, `RecordHandler::readOptionsFileAsByteArray` @ `0xcdd4c`):

```
before (ours): (0,34,0) (34,4,118) (40,14,116) (56,26,120) (82,58,0)
after  (ours): (0,34,0) (34,4,118) (40, 8,116) (50,32,120) (82,58,0)
original     : (0,34,0) (34,4,118) (40, 8,116) (50,32,120) (82,58,0)
```

Before the fix, `OpenRead` (offset 40) and `GetFileSize` (offset 50) were
merged into one call-site row `(40,14,116)` because both unwound to the same
landing pad. After the fix `OpenRead` is `(40,8,116)` and `GetFileSize`
starts a new row `(50,32,120)`, exactly as the original.

## Remaining 14 differing records (1433 bytes)

Owner = the function at the exidx entry (the nearest preceding dynsym is the
*previous* function; `exidx_raw_audit.py` names records by the preceding
dynsym, so its printed names are off by one).

| record owner | body byte-exact? | diff bytes | cause |
|---|---|---:|---|
| `PaintCanvas::DrawStringColor` | **yes** | 3 | `ArrayReleaseClasses<String*>` merged into `SetColor` call site |
| `PlayerJumpgate::PlayerJumpgate` | **yes** | 15 | cleanup-trampoline assignment (234/236/238) |
| `RecordHandler::recordStoreRead` | **yes** | 3 | two identical cleanup trampolines (5078/5100) swapped |
| `Radar::Radar` | **yes** | 7 | two identical cleanup trampolines (2462/2464) swapped |
| `ModStation::ModStation` | no (99.2%) | 2 | call-site range start differs by 8 |
| `Generator::createMission` | no (90.0%) | 27 | body layout |
| `Level::createMission` | no (92.3%) | 139 | body layout |
| `Level::createShip` | no (70.5%) | 70 | body layout |
| `Radar::draw` | no (75.3%) | 29 | body layout |
| `LevelScript::process` | no (60.2%) | 292 | body layout |
| `HangarWindow::render` | no (94.4%) | 762 | body layout |
| `Hud::draw` | no (96.6%) | 40 | body layout |
| `SpaceLounge::drawLounge` | no (95.7%) | 14 | body layout |
| `MGame::OnTouchEnd` | no (81.9%) | 30 | body layout |

The four byte-exact records are pure EH-shape deltas; the other ten will
close when their bodies become byte-exact.

## What the byte-exact records look like

All four are the same ARM-backend artifact: the IR has two or more
`landingpad` blocks whose cleanup code is identical. The backend emits
separate `b.n <cleanup>` trampolines for them and then assigns the trampolines
to call sites in an order that depends on the IR cleanup-block order. The
machine code is byte-identical; only the LSDA landing-pad operands differ.

* `PaintCanvas::DrawStringColor` (`0x78274`, 352 B): original rows
  `... (256,8,308) (264,88,0)`; ours `... (256,14,308) (270,82,0)`.
  `SetColor` is at 260 and `ArrayReleaseClasses<String*>` at 266. In the
  original the `ArrayReleaseClasses` call is outside the `str` cleanup scope
  (row `(264,88,0)` has `lp=0`); in ours it shares the `SetColor` landing pad
  308. This is the clang-7 `doesNotThrow`/first-template-instantiation
  behaviour: a standalone TU that calls `ArrayReleaseClasses<String*>`
  from a cleanup scope emits an `invoke`, while the second such template
  instantiation in the same TU is inferred `nounwind`. Adding `noexcept` to
  the template globally is **not** the answer (see below).
* `PlayerJumpgate::PlayerJumpgate` (`0xa4fa4`, 264 B): trampolines 234, 236,
  238 all branch to the same "destroy base" cleanup; original assigns
  rows 1/3/7 to 238, row 4 to 236, row 5 to 234; ours assigns 1/7 to 236,
  2/4 to 234, 5 to 238.
* `RecordHandler::recordStoreRead` (`0xcbd34`, 5416 B): rows 161/162/164;
  orig `(4028,6,5078) (4040,6,5100) (4054,6,5100)` vs ours
  `(4028,6,5100) (4040,6,5078) (4054,6,5078)`. Pads 5078 and 5100 are both
  `b.n 0xcd24e` (destroy local `String path`).
* `Radar::Radar` (`0x12d484`, 2488 B): pads 2462 and 2464 are both
  `b.n 0x12de28`; the two groups are swapped.

## Negative results (all reverted)

* **Global `noexcept` on `ArrayReleaseClasses`** (declaration in `Array.h`,
  definition in `ArrayReleaseClasses.h`, and every explicit instantiation):
  `.ARM.extab` records shrink by 16 B each and **byte_exact collapses
  4506 -> 2041**. The `noexcept` attribute makes clang emit a
  `__clang_call_terminate` personality/landing pad in the callee, changing
  its body, and every call site becomes nounwind. Not the original shape.
* **Explicit instantiation** of `ArrayReleaseClasses<String*>` in
  `PaintCanvas.cpp`: no change to the `DrawStringColor` LSDA (still
  `invoke`), and it changes the symbol from `linkonce_odr` to `weak_odr`.
* **`volatile int pad` before the call** in `DrawStringColor`: optimised
  away, no LSDA change.
* **Declaring `volumes` before the `if`** in `PlayerJumpgate::PlayerJumpgate`
  (with or without `= 0`): moves the trampoline assignment but does not
  match the original, and the `= 0` form changes the body.
* **Declaring `shipList` before the `AEFile::Read`** in
  `recordStoreRead`: no change.
* **`volatile int padS` between the Read and the `if`** in
  `recordStoreRead`: optimised away, no change.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7642
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields

python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1433, records differ 14, len-diff 0
```

The scratch LSDA decoder used for this pass is
`/dev/shm/w7642/extab_detail.py` (same layout as
`_work/reconstructions/exidx_table.py`; it additionally prints the decoded
call-site rows side by side for each differing record).
