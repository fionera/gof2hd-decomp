# w9508 (wave 148): `Level::createMission` — sret-temp hoist (`sp+212` -> r5)
# not reachable from source; new shapes measured, no win over baseline

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9508`, based on master
`fece77a64` (wave 148). Continues `level-createmission-wave148-w9410.md`,
`…-wave148-w9411.md`, `…-wave146-w9310.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is
`/dev/shm/w9508/fdiff.py`; instruction-index alignment is
`/dev/shm/w9508/align_diff.py` (worktree-local copies of the shared tools).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5561   align 716
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

Register map of the alien loop (from our disassembly):

```text
OURS  temp=fp  this=r5  i=r4 (spilled [sp,#116])  gun=r4  t=r6  rotation=sl
ORIG  temp=r5  this=r4  i=fp (no spill)           gun=r6  t=sl  rotation=r4
```

## 1. The temp is already hoisted by MachineLICM; Greedy RA rematerialises it

`-mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level13createMissionEv`
confirms the w9410 finding. After **Early Machine LICM** the preheader
(`bb.214`) contains all three frame-address defs:

```text
%3085 = t2ADDri %stack.9   ; sret temp (later sp+212)
%3087 = t2ADDri %stack.10  ; input Vector aggregate
%3088 = t2ADDri %stack.6   ; rotation Matrix
```

After **Greedy Register Allocator** only the temp is rematerialised back into
the loop body (`bb.252`):

```text
65216B  ADJCALLSTACKDOWN
65220B  %5447 = t2ADDri %stack.9        ; remat in body
65224B  %5446 = COPY %5447             ; %5446 -> r11 (fp), reused 5x
```

`this` is a *separate* short interval loaded from `[sp,#124]` at `b3284`
(inside the alien block, after `getPosition`), whereas the original loads it
at `b3232` (before the `alienGuns` test) and keeps it in r4. Because `this`
starts before the temp interval, greedy gives `this`=r5 first, then the temp
falls to fp. The target chain is cyclic: `i=fp -> this=r4 -> temp=r5`, and
`temp=r5 -> i=fp`; the allocator cannot enter it from the current block order.

## 2. New source shapes measured this wave (all 17,172 B unless noted)

`M` = `Matrix rotation;`, `v` = `Vector v = {0,0,0};`, `k` = `int k = 0;`,
`b` = `Vector base = …getPosition();`. Baseline is `M v k b`.

| variant | size | raw | align | alien map (temp,this,i) |
|---|---:|---:|---:|---|
| **HEAD `M v k b`** | **17172** | **5561** | **716** | fp, r5, r4 |
| `v M k b` (w9410 raw winner) | 17172 | **5559** | 734 | fp, r5, r4 |
| `v M b k` | 17172 | 5559 | 734 | fp, r5, r4 |
| `i <= -1` anchor (same code) | 17172 | 5561 | 716 | fp, r5, r4 |
| `i >> 16` anchor (same code) | 17172 | 5561 | 716 | fp, r5, r4 |
| `i & 0x40000000` anchor | 17172 | 5657 | 719 | fp, r5, r4 |
| `i & 1` anchor | 17172 | 5657 | 719 | fp, r5, r4 |
| `i | 0` anchor | 17180 | 5808 | 834 | fp, r5, r4 |
| `i == -1` anchor | 17156 | 6526 | 1171 | r8, fp, r5 |
| `i < -1` anchor | 17188 | 6477 | 870 | fp, r5, r4 |
| `switch (bigAlienWave)` table select | 17140 | 6589 | 1234 | sl, r5, — |
| `__builtin_expect(bigAlienWave != 0,1) ? … : …` | 17140 | 6589 | 1234 | sl, r5, — |
| `volatile int ba = bigAlienWave; ba ? …` | 17172 | 5562 | 792 | r9, r5, r4 |
| `if (__builtin_expect(alienGuns != 0, 1))` | 17172 | 5580 | 752 | fp, r5, r4 |
| named `Vector pos` inside loop (setPosition) | 17172 | 5561 | 716 | fp, r5, r4 |
| named `Vector pos` outside loop | 17168 | 5845 | 1195 | r5, sl, r4 |
| `const Vector &r = MatrixRotateVector(...)` | 17156 | 6513 | 1182 | —, r5, r9 |
| `Vector tmp` outside + `tmp=f(); v=tmp` | 17320 | 16605 | 3162 | —, fp, — |
| `Vector tmp` outside, both rotates + setPosition | 17316 | 16599 | 3115 | —, fp, — |
| `Vector *volatile ptmp = &tmp` (full reuse) | 17188 | 6916 | 1759 | —, sl, r9 |
| `Vector &rt = tmp` (full reuse) | 17316 | 16599 | 3115 | —, fp, — |
| `setScaling` after `setPosition` (faithful, `Dlast`) | 17188 | 6481 | 862 | r9, r5, fp |
| `Dlast` + `M v b k` / `v M k b` / `M k v b` | 17188 | 6479–6481 | 862–880 | r9, r5, fp |

Notes:

* **`v M k b` is the raw winner (5559, −2 bytes)** but costs +18 aligned
  instructions; it is also *unfaithful* (the original initialises `rotation`
  before `v`). w9411 deliberately landed the faithful `M v k b` at raw +1 /
  align −12, so this wave leaves it in place.
* `Dlast` (the original's witnessed call order: `setPosition` then
  `setScaling`) moves `i` to `fp` — the only variant that does — but grows the
  function to 17,188 B and raw jumps to 6,481. Not landable under the size
  gate.
* Named temps always add a `Vector::operator=` copy (`tmp = MatrixRotateVector`
  cannot elide; only copy-initialisation elides, and that requires the
  declaration inside the loop). The extra copy is why every named-temp shape
  either grows the frame or blows up raw.
* `const Vector &r = MatrixRotateVector(...)` (lifetime-extended temporary)
  shrinks the frame to 17,156 B but raw 6,513.
* `volatile` copies/pointers force the address out of the preheader but add
  loads and change `this` away from r5.

## 3. Why no source shape reaches `temp=r5`

The temp address is a plain frame index (`t2ADDri %stack.9`), which LLVM 7's
greedy allocator treats as trivially rematerialisable. The original's
allocation is the product of its PGO-like branch weights (block layout, hence
interval order), not of a source-level temp. Every shape that keeps the frame
at 17,172 B leaves the greedy choice unchanged; every shape that disturbs the
address interval also changes the frame or `this`.

## 4. Gate counts (unchanged; no source landed)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv raw 5561 -> 5561, align 716 -> 716, size 17172
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9508/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9508/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9508/{fdiff.py,align_diff.py,sweep.py,gen.py,gen2.py,
run.py,run2.py,loop.py,v2..v6.json,Level.pristine.cpp,greedy.log}`.

## Open questions for the next wave

1. The temp hoist needs a *block-layout* lever, not a temp-shape lever. Try
   `__builtin_expect` on the `if (alienGuns != 0)` together with the faithful
   `M v k b` order and the `Dlast` call order, and inspect the greedy output
   for whether `%stack.9`'s preheader def survives.
2. The original's in-loop `cbz r0` table select (not a hoisted `cmov`) is the
   other PGO-weighted branch. A source form that keeps the select as an
   in-loop branch without an `(i<0)` anchor would remove the `i` spill and
   shrink the loop body from 262 to 250 B (matching the original).
3. Once `this=r4`, re-test the faithful raid block (no cached `Status *status`,
   `int guns` after the inner `if`).
