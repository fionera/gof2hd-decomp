# Globals::Globals C1/C2 (80.5%, 612 B) — w103/wf309

Status: research note, 2026-11-04. Worktree `wt/wf309`, branch `wave103-wf309`,
base `1d2b4398e`. **No source change landed** — the w3808..w5106 baseline source
order remains the best measured shape (80.5%). This commit contains only this
note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* The new wave-103 oracle confirms the **source shape** of the quality block:
  `Globals.c` and `MenuTouchWindow.c` both show
  `options._84_4_ = 0x247;` (direct `resWidth` store) followed by conditional
  direct `resWidth` stores, while `resHeight` uses a local (`iVar7 = 0x201;
  ...; options._88_4_ = iVar7;`).  Translating that shape into our source
  compiles to the **same 80.5 / 612 B** code as the current local-local form:
  at `-Oz` clang merges the direct stores and rematerialises the default.
* The two remaining deltas are unchanged and are register-allocation outcomes:
  the `movw r6,#583` hoist (original index 34, ours 139) and the two
  Thumb-16→Thumb-32 `ip` promotions for `&Globals::imageFactory`.  Neither
  could be recovered from source this pass; every new shape measured below is
  ≤ 80.5.

## 1. Baseline and gate

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
```

```text
Globals::Globals()  orig 612B @0xe31e0 | ours 612B @0xe31e0
(length differs: orig 225 vs ours 227 insns)
```

The two extra normalised entries are the two `nop`s that pad our 4-byte-shorter
code body up to the 4-byte-aligned literal pool. `C1` is a plain alias of `C2`;
every experiment was scored on `C1` (identical stream).

## 2. New oracle evidence

The wave-103 oracle is a Ghidra 12.1.4 decompilation of a KiritoJPK
NDK-r14/clang-3.8 `-O2` rebuild.  It is a logic/source-shape oracle, not a byte
oracle.

`game/Globals.c`, `Globals::Globals` (lines 5–88), gives this quality block
(abridged; `_84_4_` = `resWidth` @0x54, `_88_4_` = `resHeight` @0x58,
`_68_4_` = `qualityLevel` @0x44):

```c
options._84_4_ = 0x247;             // direct default store, early
...
if (1.0 <= (float)options._68_4_) {
  options._84_4_ = 0x33e;           // direct conditional store
}
options._88_4_ = 0x201;             // height default
if ((float)options._68_4_ <= 0.0) {
  options._84_4_ = 0x19f;           // direct conditional store
}
if (1.0 <= (float)options._68_4_) {
  options._88_4_ = 0x2da;
}
if ((float)options._68_4_ <= 0.0) {
  options._88_4_ = 0x16d;
}
```

`game/MenuTouchWindow.c`, `MenuTouchWindow::OnTouchEnd` (lines 5885–5902),
shows the same source family even more clearly:

```c
Globals::options._84_4_ = 0x247;
iVar7 = 0x201;
if (!NAN((float)Globals::options._68_4_)) {
  Globals::options._84_4_ = 0x33e;
}
if ((float)Globals::options._68_4_ <= 0.0) {
  Globals::options._84_4_ = 0x19f;
}
...
if (!NAN((float)Globals::options._68_4_)) {
  iVar7 = 0x2da;
}
...
if (...) {
  iVar7 = 0x16d;
}
Globals::options._88_4_ = iVar7;
```

So the original source almost certainly used a **direct `resWidth` store with
conditional direct stores, and a local for `resHeight`**, not the local-local
form currently in our tree.  This is a source-shape fact, not yet a byte win.

## 3. Measurements (new this pass)

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -ffunction-sections -funwind-tables -Oz -g -DANDROID
-DNDEBUG -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`difflib.SequenceMatcher(autojunk=False)`).
A minimal single-TU harness (the anonymous-namespace structs + the constructor
only) reproduces the baseline 80.5 / 227 / 225 exactly and evaluates in
~0.14 s.

| variant | pct | size | `movw #583` index |
|---|---:|---:|---:|
| baseline (local width, local height) | **80.5** | 612 | 139 |
| oracle `Globals.c` shape: direct width stores + local height | **80.5** | 612 | 139 |
| oracle `MenuTouchWindow.c` shape: direct width stores + local height, height default before width conditionals | **80.5** | 612 | 139 |
| direct width + direct height stores | 34.4 | 628 | 139 |
| oracle full ctor store order (direct stores) | 28.8 | 628 | — |
| oracle full ctor order with locals | 49.6 | 612 | 139 |
| original machine store order as source (baseline early) | 46.9 | 612 | 139 |
| original machine store order as source (WIN early) | 49.1 | 612 | 139 |
| WIN early block + baseline late block | 78.3 | 612 | 135 |
| WIN early block + quality at late pos 12/13/14 | 74.3–76.1 | 612 | **34** |
| baseline early + quality at late pos 12/13/14 | 71.7–73.0 | 612 | **34** |
| `imageFactory = 0` moved inside the quality block | 80.1 | 612 | 139 |
| `ImageFactory *&pif` / `ImageFactory **pif` before quality, store after | 80.5 | 612 | 139 |
| `int v54` declared at function top / after `appManager` | 80.5 | 612 | 139 |
| `int v54 = getDefaultWidth()` (static helper, inlined at top) | 80.5 | 612 | 139 |
| `GameSettings` struct type instead of `bool[100]` cast | 80.5 | 612 | 139 |
| direct member access (`Globals::options.colorR = ...`) | 80.5 | 612 | 139 |
| full-order simulated annealing (600×8 swap/move evals) | **80.5** | 612 | 139 |
| complete single-move early-block neighbourhood (182) | **80.5** | 612 | 139 |
| complete single-move settings-block neighbourhood (351) | **80.5** | 612 | 139 |
| complete single-move late-block neighbourhood (342) | **80.5** | 612 | 139 |

The `movw #583` hoist is reproducible only when the quality block is moved to
late positions 12–14, and every such shape loses 4.4–8.8 points elsewhere.  The
`imageFactory` `ip` promotion appears when the store is moved inside the
quality block, but the store lands at normalised index 183 instead of the
original 181 and the score drops to 80.1.

## 4. What is proven vs still hypothesis

**Proven (measured or read off the binary/oracle):**

1. The original source shape for the quality block is direct `resWidth`
   stores + a local for `resHeight` (oracle `Globals.c` and
   `MenuTouchWindow.c` agree).
2. At `-Oz` that shape compiles to exactly the same 80.5 / 612 B code as the
   current local-local form; clang merges the direct stores and rematerialises
   the default.
3. The baseline is a local optimum under complete single-move neighbourhoods of
   the early, settings and late blocks, and under 4 800 full-order SA moves.
4. The `movw #583` hoist and the `imageFactory` `ip` promotions are coupled to
   the late-block/quality-block schedule, not to the source expression form.

**Hypothesis (not proven):**

1. The original's `movw r6,#583` hoist is the scheduler reusing `r6` the
   instant it dies at `ldr.w ip,[r6]`; reproducing it without moving the
   quality block would require a source shape that makes `v54` live from
   index 34, which no tested declaration/helper/type form achieves.
2. The `imageFactory` `ip` promotion requires the store to remain after the
   quality block while the GOT load stays at index 128; every pointer-local and
   reference form tried here folds the load and store together or hoists the
   load to the prologue.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf309
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC1Ev
```

Scratch harness (not tracked, `/dev/shm/wf309`): `fasttry.py` (single-TU
compile + gold link + official score, ~0.14 s/eval), `scorelib.py`
(normalised-stream access), `trace_stores.py` (PIC GOT store tracer),
`search_late.py` / `search_early.py` / `search_settings.py` / `search_ip.py` /
`sa_full.py` (the neighbourhood and SA searches above).
