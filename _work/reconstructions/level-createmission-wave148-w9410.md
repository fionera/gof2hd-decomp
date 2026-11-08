# w9410 (wave 148): `Level::createMission` — alien-loop declaration order,
# sret-temp hoist still not reached

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9410`, based on master
`cbe3b5f7f` (wave 147). Continues `level-createmission-wave146-w9310.md`,
`…-wave146-w9208.md`, `…-wave145b-w9014.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w9410/fdiff.py`); instruction-index alignment is
`_work/reconstructions/align_diff.py`.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5560   align 728
.text 86,160   .ARM.extab 1,309   .rel.plt 4   .note.gnu.build-id 20
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. The original's preheader init order is `Matrix`, `v`, `base`

The original's alien-gun preheader (`b323c..b3284`) initialises in this order:

```text
b323c..b326a  Matrix rotation   (identity, sp+312)
b326c..b3272  Vector v = {0,0,0} (sp+372)
b3274..b3284  base = getPosition (sret sp+224)
```

So the original source order is `Matrix rotation; Vector v; ... Vector base = ...;`
— `v` **before** `base`. HEAD (wave-146 w9208 "basefirst") has
`Matrix rotation; Vector base = ...; Vector v = ...;`, which initialises
`base` before `v` and puts `v` at `sp+224`, `base` at `sp+372` (the original is
`v` at `sp+372`, `base` at `sp+224`).

## 2. Landed change: `Vector v; Matrix rotation; int k; Vector base;`

```diff
     if (alienGuns != 0) {
-        Matrix rotation;
-        Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
         Vector v = {0.0f, 0.0f, 0.0f};
+        Matrix rotation;
         int k = 0;
+        Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
```

This restores the original's stack-slot assignment (`v` at `sp+372`, `base` at
`sp+224`) and the `v`-init-before-`base` order. The emitted preheader is now

```text
b3238  movs r0, #0
b323a  strd r0, r0, [sp, #372]     ; v
b323e  str r0, [sp, #380]
b3240  add r0, sp, #312            ; rotation
...
b3268  ldr.w r0, [r4, #248]        ; getPosition
b3276  add r0, sp, #224            ; base
```

The alien loop itself (`b328c..`) is **unchanged** from HEAD: `this=r5`,
`gun=r4`, `i=r4` (spilled `[sp,#116]`), `t=r6`, sret temp `sp+212` in `fp`.

### Metrics (before -> after)

```text
raw same-address diff   5560 -> 5559   (-1)
instruction alignment    728 ->  734   (+6)
size                   17172 -> 17172
.text diff            86,160 -> 86,159 (-1)
```

The raw win is the pre-alien region (`0x0000..0x32c2`): 2587 -> 2586. All other
regions are byte-for-byte unchanged from HEAD (alien loop 215, alien tail 17,
pirate loop 196, pirate tail 196, raid 484, tail 1865).

The alignment regression (+6) is the greedy stream alignment reacting to the
`v`-first preheader; it is not a structural change in the alien loop. The
alternative order `Matrix rotation; Vector v; int k; Vector base;` (matches the
original's init order exactly) measures **raw 5561 / align 716 / .text 86,161**
— a 12-instruction alignment win for a 1-byte raw cost. Raw was chosen as the
primary metric; the coordinator may prefer the `Matrix`-first order if the
aligned total is weighted higher.

## 3. The sret-temp hoist (`sp+212` -> `r5`) was NOT reached

The w9310 target chain is `temp=r5 -> i=fp -> this=r4 -> gun=r6 -> t=sl`.
New measurements this wave (all 17,172 B unless noted):

| variant | size | raw | align | alien map (temp,this,gun,i) |
|---|---:|---:|---:|---|
| HEAD | 17172 | 5560 | 728 | fp, r5, r4, r4 |
| **landed `v M k b`** | **17172** | **5559** | **734** | fp, r5, r4, r4 |
| `M v k b` (original init order) | 17172 | 5561 | 716 | fp, r5, r4, r4 |
| `M v b k` | 17172 | 5561 | 716 | fp, r5, r4, r4 |
| `v M b k` | 17172 | 5559 | 734 | fp, r5, r4, r4 |
| `Vector tmp` in loop + `v=tmp` (copy-init) | 17188 | 6471 | 924 | fp, sl, r4, r4 |
| `Vector tmp` outside + `tmp=f()` + `v=tmp` | 17320 | 16605 | 3134 | r9, fp, r4, — |
| `Vector tmp` outside init + `tmp=f()` + `v=tmp` | 17188 | 6471 | 924 | fp, sl, r4, r4 |
| `Vector &tv = v` alias | 17172 | 5560 | 728 | fp, r5, r4, r4 |
| `Vector *pv = &v` before loop | 17172 | 5560 | 728 | fp, r5, r4, r4 |
| `Vector &v = vtmp` (ref bind before loop) | 17172 | 5560 | 728 | fp, r5, r4, r4 |
| named `Vector pos` for setPosition | 17168 | 5846 | 1201 | r5, sl, r4, — |
| `Vector p` copy-init both rotates | 17156 | 6510 | 1203 | r5, r5, r4, — |
| `Vector p` copy-init 2nd rotate | 17172 | 5649 | 736 | fp, r5, r4, r4 |
| `Vector p` outside, assign both rotates | 17320 | 16605 | 3134 | r9, fp, r4, — |
| no `(i<0)` anchor (plain `bigAlienWave`) | 17140 | 6587 | 1395 | r9, r5, r4, r4 |
| `int end = ...` cached loop bound | 17164 | 6560 | 1373 | sl, r4, r4, — |
| `M v k b` + `setScaling` before `setRotation` | 17172 | 5564 | 712 | fp, r5, r4, r4 |
| `M v k b` + `Dlast` (setScaling after setPosition) | 17188 | 6481 | 862 | r9, r5, r4, — |
| `M v k b` + `p` copy-init both | 17156 | 6513 | 1182 | r5, r5, r4, — |

Notes:

* Named copy-init temps (`Vector p = MatrixRotateVector(...); v = p;`) do move
  the sret slot into a **low** register (`r5`), but they change the frame
  (17156/17168) and blow up raw (6.5k). The address is still materialised in the
  body, not the preheader.
* Caching the loop bound (`int end = ...`) moves `this` to `r4` (half the target
  chain) but shrinks the function to 17164 and raw jumps to 6560.
* `Vector&`/`Vector*` aliases and `&v` taken before the loop are all optimised
  away by SROA; no change.
* `__builtin_expect` on the anchor or the loop condition does not change the map.

### Why the temp is not hoisted (LLVM 7 pass trace)

`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level13createMissionEv`
shows that **Early Machine LICM does hoist** `t2ADDri %stack.9, 0` (the sret
temp address) into the preheader (`bb.214`), together with the input-vector and
`v` addresses. The **Greedy Register Allocator** then rematerialises it back into
the loop body (`%5447 = t2ADDri %stack.9, 0` in `bb.252`, reused 5x via
`%5446 = COPY %5447`), because the interval is live across the loop and the
body is at register pressure. The original's allocator kept the same value in
`r5` instead. This is the single allocator decision w9310 identified; no source
shape tried this wave changes it without also changing the frame size.

## 4. Gate counts (before -> after)

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
.text diff      86,160 -> 86,159   (-1)
.ARM.extab      1,309  -> 1,309
.rel.plt        4      -> 4
_ZN5Level13createMissionEv raw diff 5560 -> 5559, align 728 -> 734, size 17172
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9410/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9410/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9410/{fdiff.py,align_diff.py,regmap.py,alienmap.py,try.py,
try2.py,rdiff.py,variants1..11.py,Level.pristine.cpp,f.log,pa.log}`.

## Open questions for the next wave

1. The sret temp hoist needs a source shape that keeps the frame at 17,172 B
   while making the `%stack.9` interval unattractive to rematerialise. The
   LLVM-7 lever is `MachineLICM`/`RegAllocGreedy` heuristics; a named temp
   changes the frame. One untried idea: make the temp address *escape* the loop
   (e.g. store `&tmp` into a local pointer used after the loop) so the address
   is live across the back-edge and cannot be rematerialised.
2. The `(i < 0)` anchor is unfaithful to the original's `cbz` (the original's
   table select has no anchor). Removing it gives 17,140 B; +32 B of faithful
   code elsewhere is needed to recover the size.
3. Once `this=r4`, re-test w9014 candidate (b): `fortressGuns` in `fp`
   (`ldr.w fp,[sp,#40]`, +4 B) and `Globals::status` in `r8` in the raid
   prologue.
