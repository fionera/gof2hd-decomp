# `.ARM.exidx` reconciliation (wave 103, wm201)

Status: research note + one landed source change. Worktree
`/home/debian/GalaxyOnFire2/wt/wm201`, branch `wave103-wm201`, base `1d2b4398e`.
Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(real address = Ghidra address - 0x10000). Scratch: `/dev/shm/wm201`.

## Result

`.ARM.exidx` is **not yet byte-exact**, but the remaining gap is now fully
characterised and one independent defect is fixed. The landed change moves the
touch-buffer static initializer from `LODManager.cpp` back to its original TU,
`NdkAutosave.cpp`. It is a strict improvement and changes no section size or
base:

| metric | before | after |
|---|---:|---:|
| `.ARM.exidx` differing bytes | 842 | **829** |
| `.ARM.exidx` differing entries | 822 | **812** |
| `.ARM.extab` size / base | `0x26298` @ `0x1a3b78` | same |
| `.text` / `.rodata` / `.got` | exact | exact |
| byte_exact / linked_exact | 4500 / 4500 | **4500 / 4500** |
| size-exact / data / imports / lint / drift | 4524, 843, 207, CLEAN, 0 | same |

The three remaining causes of the 812 differing entries are:

| region | entries | cause |
|---|---:|---|
| `Generator::createMission` .. `LevelScript::process` | 811 | Generator extab record is 168 vs 172 (-4); every record up to LevelScript is shifted -4 |
| `LevelScript::process` own entry | 1 | its extab record is 352 vs 348 (+4), so its own `w1` target is 4 B low |
| all other entries | 0 | exact |

So **both** `Generator::createMission` (+4) and `LevelScript::process` (-4) must
change together: their lengths cancel, keeping `.ARM.extab` at `0x26298`, and
then every `w1` becomes exact. `Generator` alone is *not* an improvement: it
grows `.ARM.extab` to `0x2629c` and moves the 811-record shift to the 817
records after `LevelScript` (818 differing entries).

## Landed fix: touch-buffer initializer TU (10 entries)

### Evidence

`exidx_table.py --diff` reports no length difference for this region; the
defect is an *ordering* difference. In the original, the compact-model
`.ARM.extab` record for the local function at `0x694c0`
(`_GLOBAL__sub_I_LODManager.cpp`, the `touches = realloc(touches, 16)`
initializer) is at `0x1a768c`, immediately after `AddTouch`'s record at
`0x1a7680` and before `LODManager::LODManager()`'s record at `0x1a7698`. In the
base build it was emitted after every `LODManager` method record, at
`0x1a770c`. This shifts the initializer's own exidx entry (+128) and the 12
bytes of records between its old and new positions (-12), i.e. 10 entries.

The original `.text` layout already puts the initializer in the `.text.startup`
region at `0x694c0` (after `Engine.cpp`'s initializer at `0x69440`, before
`ParticleSettings.cpp`'s at `0x694dc`), and the `.init_array` entries are
`0x69441, 0x694c1, 0x694dd` in both builds. The only thing wrong was the
*object* that owned the initializer: `LODManager.cpp` (link position 94)
instead of `NdkAutosave.cpp` (position 93, the TU that defines `touches`).
`NdkAutosave.cpp` has no other `.text.startup` section, so the initializer
still lands at `0x694c0`, but its `.ARM.extab.text.startup` section is emitted
after `AddTouch`'s record, exactly as in the original.

### Change

```diff
--- a/src/engine/render/LODManager.cpp
+++ b/src/engine/render/LODManager.cpp
@@
-// Defined in NdkAutosave.cpp: the original first mentions these globals before
-// Globals::Canvas, which is first referenced in this TU.  The initializer stays
-// here so the GOT first-reference order (curTouchSize, maxTouchSize, touches,
-// Canvas) is unchanged.
-extern Touch *touches;
-extern int maxTouchSize;
-extern int curTouchSize;
-
-namespace {
-struct TouchBufferInit {
-    TouchBufferInit() { touches = static_cast<Touch *>(realloc(touches, sizeof(Touch))); }
-};
-TouchBufferInit touchBufferInit;
-}
-
 LODManager::LODManager() {
--- a/src/platform/android/NdkAutosave.cpp
+++ b/src/platform/android/NdkAutosave.cpp
@@
 Touch *touches;
 int maxTouchSize = 0;
 int curTouchSize = 0;
 
+namespace {
+struct TouchBufferInit {
+    TouchBufferInit() { touches = static_cast<Touch *>(realloc(touches, sizeof(Touch))); }
+};
+TouchBufferInit touchBufferInit;
+}
+
 void AddTouch(int x, int y, int id, int action) {
```

Measured on a clean scratch build of the whole tree:

| section | orig size @ base | ours size @ base | differing bytes |
|---|---|---|---:|
| `.text` | `0x1337c8` @ `0x69440` | same | 102798 |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | same | 842 -> **829** |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | same | 108354 -> **108292** |
| `.rodata` | `0x41b67` @ `0x1c9e10` | same | 0 |
| `.got` / `.data` | exact | exact | 0 / 0 |

The moved record region is now byte-identical to the original
(`0x1a7680..0x1a7718`), and `.init_array` is unchanged
(`0x69441, 0x694c1, 0x694dd`).

## `Generator::createMission`: exact 172-byte shape found (not landed)

The original LSDA has **26 rows / tablelen 151**; the base has 25 rows /
tablelen 147. Original rows (starts):

```
0,588,636,730,872,922,944,968,1000,1128,1272,1320,1340,1440,1448,1458,
1500,1524,1602,1610,1644,1686,1724,1758,1764,1772
```

The original has a `lp=0` row at `[968,1000)` (the `nextInt(2)` block) and one
merged distance row `[1128,1172)`. A source shape that produces a **172-byte**
record (26 rows, tablelen 151) while keeping the body at exactly **1996 B** was
found and measured on an isolated `Generator.cpp` compile:

```cpp
// 1. padA becomes a float (was volatile double)
volatile float padA = 0; (void) padA;

// 2. the distance block keeps an explicit toSystem temporary
Galaxy *galaxy = Globals::galaxy;
SolarSystem *toSystem = systems->data()[galaxy->getStation(targetStation)->getSystem()];
float reward = (galaxy->distance(systems->data()[Globals::status->getStation()->getSystem()], toSystem) / 1200.0f + 1.0f) *
               (float) ((int) ((float) difficulty / 10.0f * 5500.0f) + 1500);

// 3. a volatile sink immediately before the case-15 stationIsInSystem test
for (uint32_t i = 0; i < systems->size(); ++i) {
    volatile int tsink = 0; (void) tsink;
    if (systems->data()[i]->stationIsInSystem(targetStation)) {
```

Measured: `.text._ZN9Generator13createMission...` = 1996 (exact),
`.ARM.extab.text._ZN9Generator13createMission...` = 172 (exact), 26 rows,
tablelen 151. Equivalent pad/sink type combinations also give 1996/172/26/151
(`volatile int`/`short`/`char` pad + `volatile int` sink; `volatile float` pad +
`volatile short tsink = (short) i`; `volatile long long` pad + `volatile int
tsink = (int) i`).

**Caveat (proven):** the row *starts* of this shape are not the original's
(`0,612,660,746,882,928,958,1086,1112,1278,1298,1394,1402,1414,1460,1484,1562,
1570,1604,1646,1684,1718,1724,1732,1842,1866`). This does not matter for
`.ARM.exidx` (which only stores the record address), but it means the shape is
an anchor, not a proof of the original source shape. It must be landed together
with the `LevelScript` -4 change or `.ARM.extab` grows to `0x2629c`.

## `LevelScript::process`: 348-byte record needs a body-shape pass

Original LSDA: 46 rows, tablelen 324, record 348. Base: 46 rows, tablelen 331,
record 352. Only rows 13-19 differ, and only in ULEB width: their `start`
values are >16383 in ours (3-byte ULEB) but <16384 in the original (2-byte):

| row | orig start | base start |
|---:|---:|---:|
| 13 | 12162 | 18604 |
| 14 | 12170 | 18612 |
| 15 | 14224 | 19802 |
| 16 | 14242 | 19814 |
| 17 | 14250 | 19824 |
| 18 | 14262 | 19838 |
| 19 | 14270 | 19846 |

The body size is already exact (`0xdf90` = 57232), so the fix is a
redistribution, not a size change. Row 11 is case 157's
`m_pGeometry4 = new AEGeometry(Globals::Canvas)` at `0x13a3ee`; row 13 is
case 91's `setName(String(*Globals::gameText->getText(3211), false))` at
`0x13ac5c`. Between those two call sites the original has 2214 B; ours has
9340 B. In other words the reconstructed case bodies emitted between case 157
and case 91 (cases 125/40/102/154/24/14/21/42) are ~7 KB larger than the
original's, and the rest of the function is correspondingly smaller. The
original row 13 has `lp=57098`; ours has `lp=57102`.

Experiments (all isolated `LevelScript.cpp` compiles):

| variant | body | extab |
|---|---:|---:|
| base | 57232 | 352 |
| `new (std::nothrow) Route(route1, 3)` for `escort1Route` | 57316 | **348** |
| `::operator new(sizeof(Route), std::nothrow)` + placement new | 57236 | 340 |
| both Routes `std::nothrow` | 57316 | 356 |
| move case 91 after case 157 in source | 57232 | 352 |
| `String` expression rewrites in case 125 | 57240..57376 | 360..376 |

The `std::nothrow` form is the only one that reaches 348, and it costs +84 B
(the compiler's null-check branch). The original disassembly at `0x141faa`
shows a plain `blx _Znwj` with no null check, so the original really uses
throwing `new`; the 348 is a codegen coincidence. No shape with body 57232 and
extab 348 was found.

### Exact source requirement

To finish `.ARM.exidx` byte-exactly, `LevelScript::process` must keep its
57232-byte body while producing a 348-byte LSDA record, i.e. tablelen <= 327.
That means either:

1. matching the original block layout so rows 13-19 start below 16384 (the
   real fix: the ~7 KB of case bodies currently emitted between case 157 and
   case 91 must move after case 91), or
2. finding a row merge that removes >=4 table bytes without changing the body
   size.

Then apply the `Generator` shape above in the same commit: the +4/-4 record
lengths cancel, `.ARM.extab` stays `0x26298`, and with the landed
`NdkAutosave` move every `.ARM.exidx` `w1` becomes exact.

## Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/wm201
# baseline (canonical build, untouched)
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so

# scratch build (never touches cmake-build-match)
cmake -S . -B /dev/shm/wm201/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake/toolchain.cmake -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/wm201/build --target link_so -j32
cmake --build /dev/shm/wm201/build --target verify  -j32
python3 tools/ratchet.py --build-dir /dev/shm/wm201/build --no-rebuild
cmake --build /dev/shm/wm201/build --target drift   -j32

# per-record diff
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  /dev/shm/wm201/build/libgof2hdaa.so

# isolated Generator candidate
/dev/shm/wm201/cc.sh <candidate.cpp> <candidate.o>
python3 /dev/shm/wm201/dump_lsda.py <candidate.o> \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```

## Proven vs hypothesis

**Proven**

- The 10-entry `LODManager` defect is an extab *ordering* difference, not a
  length difference; moving the initializer to `NdkAutosave.cpp` makes the
  record region `0x1a7680..0x1a7718` byte-identical and reduces differing
  exidx entries 822 -> 812 (bytes 842 -> 829) with every section size/base
  unchanged and the full gate green.
- The remaining 812 entries are exactly 811 records between
  `Generator::createMission` and `LevelScript::process` (shifted -4) plus
  `LevelScript`'s own entry (its extab record is +4).
- The original `Generator::createMission` LSDA is 26 rows / tablelen 151 /
  172 B; the original `LevelScript::process` LSDA is 46 rows / tablelen 324 /
  348 B, with rows 13-19 below the 16384 ULEB boundary.
- A Generator source shape with body 1996 and extab 172 exists and was
  measured; its row starts differ from the original's.
- `LevelScript`'s body is already 57232; the 348-B record requires a block
  redistribution (or a row merge), not a size change.

**Hypothesis**

- That the Generator shape is the original source shape (it is a valid
  length anchor; the row starts are not the original's).
- That the original's `LevelScript` block order moves the ~7 KB of case bodies
  currently between case 157 and case 91 to after case 91. The disassembly
  proves the *size* of the gap (2214 vs 9340 B) and the identity of the two
  bounding call sites, but not the exact case order.
