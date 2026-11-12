# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 189 / w13363

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave189-w13363`, base master `aafeff78d` (wave-189
harvest, raw **3,444**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: independent pass on the **branch-probability-hint** lens
(`__builtin_expect`). Prior wave-189 notes (w13311 branch layout / EH-LSDA,
w13316 semantic/data-flow, w13334 statement/register-allocation) had left the
function at a local optimum for their lenses. This pass is the first to change
the hints in the hidden-blueprint block.

## 1. Result — landed

```text
                                     size            raw    aligned  insn
base aafeff78d                       17172/17172     3444   1021     6206/6197
candidate (no blueprint hints)       17172/17172     3427   1003     6206/6197
```

Raw **3,444 -> 3,427** (-17), aligned **1,021 -> 1,003** (-18), size exact
17,172 B. `.rel.plt` stays byte-identical, `.ARM.extab` does not grow.

## 2. The change

Two `__builtin_expect(..., 1)` hints removed in the hidden-blueprint block
(around line 2087 / 2101):

```cpp
-    if (__builtin_expect(blueprint, 1)) { // lint: arm_builtin
+    if (blueprint) {
...
-        if (__builtin_expect(Globals::status->getStation()->stationHasHiddenBlueprint(false), 1)) { // lint: arm_builtin
+        if (Globals::status->getStation()->stationHasHiddenBlueprint(false)) {
```

Both hints claimed the branch is hot (`1`). The original binary's layout
matches the unhinted form: clang 7 at `-Oz` lays out the `if` body as the
fall-through path and the branch-around block with the plain conditional,
whereas the `expect(...,1)` form selects a different block order and a
different `it`/branch sequence in this function's register-pressure context.

## 3. Non-additivity (measured)

The two hints interact; neither alone is a win:

```text
variant                          size            raw     aligned
base (both hints)                17172/17172     3444    1021
remove outer expect only         17172/17172     3601    1179
remove inner expect only         17172/17172     3444    1021
remove both (landed)             17172/17172     3427    1003
```

Removing only the outer hint regresses badly (+157); removing only the inner
hint is codegen-identical; both together is the win. This is why the earlier
statement-order passes did not surface it.

## 4. Section deltas (this worktree, real `link_so`)

```text
.text        size 1,259,464 / 1,259,464   differing bytes 73,565 -> 73,548 (-17)
.ARM.extab   size   156,312 /   156,312   differing bytes    610 ->    610 (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash   exact
.note.gnu.build-id  36 B, 20 differing bytes
```

Total differing bytes (text+extab+build-id) **74,195 -> 74,178** (-17).
`.ARM.extab` growth is 0 (<= the 17 B raw win), so the change is a net win.
Known wave-189 irreducible deltas (`.fini_array` 8 -> 200, `.dynamic` 3,
`.rel.dyn` 4) are unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact,
`RATCHET PASS`. No baseline update.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13363
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3427 aligned=1003 size=17172/17172`, `.text` diff 73,548,
`.ARM.extab` diff 610, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13363/`
(`expect.py`, `h.py`, `rawdiff.py`, `dis.py`, `Level.base.cpp`).
