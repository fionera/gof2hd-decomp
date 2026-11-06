# `Gun::calcCharacterCollision` wave 125 (w7518): HEAD's 93.4 % still wins;
# `i*3 = r4` is inseparable from a non-`ABCDE` tail in every source shape tried

Worker: w7518. Base master `02513de33` (wave 124 final). Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000, sha256
`d4c17be6…bed9483`). Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`, 650 insns. Scratch: `/dev/shm/w7518/` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `02513de33`) | **4506 / 4506** | **93.4000** | 1984 |
| after this pass | **4506 / 4506** | **93.4000** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4506/4524,
linked 4506/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, avg 99.93 %. Section deltas unchanged: `.text` **85,975** differing
bytes (size exact), `.ARM.exidx` **0** (byte-identical), `.ARM.extab` **1,474**
(size/base exact `0x26298`), `.note.gnu.build-id` 20. `git status --short`
clean.

## 1. Correction to the wave-124 handoff's prologue claim (re-verified)

HEAD's prologue is byte-identical to the shipped one, including the
`explosive`/`td` spill slots:

```
str r2, [sp, #20]   ; explosive   (shipped 152fe4 / HEAD 0x54)
str r6, [sp, #24]   ; td          (shipped 153010 / HEAD 0x80)
```

`check.props()` on HEAD: `s20=r2, s24=r6` (correct). The transposition
(`s20=r6, s24=r2`) only appears in the `B…`-first tail family, which is why
those variants read `ldr r6,[sp,#20]`.

## 2. The exact residual of HEAD (re-measured, 650/650 insns, 92.2 % in the
##    local `score.py` clone; official `verify` 93.4 %)

`difflib` on the normalised instruction streams (`score.py`'s
`orig_stream`/`norm_ours`/`reconcile`) gives 616/650 matched. The 34
unmatched per side are exactly:

| # | cluster | shipped | HEAD | sites |
|---|---|---|---|---|
| 1 | `i*3` register | `r4` | `r5` | `0xdc 0xe8 0xfc 0x1ca 0x4a6 0x678 0x68c 0x6a8` |
| 2 | `playerPosition` slot | `sp+44` | `sp+104` | `0x142` |
| 3 | `delta` slot | `sp+104` | `sp+44` | `0x14c` |
| 4 | `mov r6, rX` spill of `i*3` | `mov r6, r4` | `mov r6, r5` | `0x13c` |
| 5 | post-`isAsteroid` reload | `ldr.w fp,[sp,#40]` | `add r4,sp,#104` | `0x208`, `0x2d0` |
| 6 | `kNoAttribute` load | `ldr r0,[pc,#132]` | `ldr r0,[pc,#136]` + `ldr.w fp,[sp,#40]` | `0x2e6` |
| 7 | `view` block remat | `add r5,sp,#104` / `mov r0,r5` / `mov r1,r5` / `mov r2,r5` | `add r0,sp,#104` / `add r1,sp,#104` / `add r4,sp,#104` / `mov r2,r4` | `0x438`–`0x464` |
| 8 | k-loop layout | rotated (`b.n` to header at `0x56c`; body at `0x4b2`) | header first at `0x4b0`, `bcs.w` exit, `b.n` back | `0x4b0`–`0x576` |

`fp40` (`ldr.w fp,[sp,#40]`) is **5** in the shipped and **4** in HEAD; the
missing one is the `0x208`/`0x1fc` site, where HEAD rematerialises
`add r4,sp,#104` instead.

## 3. Full 120-permutation tail sweep, both scatter forms (re-run this wave)

`gen.make(scat, order)` + `check.props()` + `score.py`, for
`scat ∈ {stmt, expr}` and all `5! = 120` tail orders. Findings:

* `i*3 = r4` is reached by exactly these orders (identical set for `stmt`
  and `expr`):
  `ACBDE ACBED ACEBD ACEDB AECBD AECDB BCEAD BECAD EACBD EACDB`.
* **Every** one of them gives `s20 = r6, s24 = r2` (transposed) and
  `pool = 0x358` (12 B early), i.e. the wrong prologue slots and the wrong
  literal-pool base.
* `ACEDB` / `AECDB` / `EACDB` additionally give `fp40 = 5` and `n = 650`
  (the only variants that reproduce the shipped reload count) but score
  ~84.5 because the tail is emitted `A C E D B`.
* `ABCDE` (the shipped order) gives `i*3 = r5` on **both** forms:
  `stmt` → `pp=r4/sp+104, delta=r5/sp+44` (92.2 local / 93.4 official);
  `expr` → `pp=r5/sp+44, delta=r4/sp+104` (91.4 local / 92.6 official).
* `expr + BCEAD` is the only variant that reproduces the shipped scatter
  block **byte-for-byte** (`0x13c`–`0x15e`: `mov r6,r4` / `getPlayer` /
  `add r4,sp,#44` / `getPosition` / `add r5,sp,#104` / `operator-` /
  `VectorLength` / `mov r4,r6`) — 89.8 local.

So the shipped's four scatter properties (`i*3=r4`, `pp=r4/sp+44`,
`delta=r5/sp+104`, and the `mov r6,r4`/`mov r4,r6` spill pair) are mutually
consistent and reachable — but only with a tail whose first statement is
`playerGun` (`B…`), never with the shipped `ABCDE` asm order.

## 4. Tail asm order == source order (verified for all 120 orders)

`tailseq.py` classifies the emitted tail after the `damage` call for every
permutation. The asm order is the source order in every case (the only
"moves" are `A` being sunk past `D` for a handful of orders, e.g.
`BCDAE -> B C D A E`). There is therefore **no** source order that emits
`A B C D E` other than literally `A B C D E`.

## 5. MIR: the mechanism is the hoisting of `&stack.10`, not the tail order
##    per se

`-mllvm -print-after-all -mllvm -filter-print-funcs=…` on `expr+ABCDE` vs
`expr+BCEAD` (scratch `mir_ea.txt` / `mir_eb.txt`):

* The LLVM IR after `Float to int` differs **only** in the tail statement
  order (verified by diffing the two dumps with vregs normalised).
* In `expr+ABCDE`, the post-`isAsteroid` block `bb.43` contains

  ```
  %504 = t2ADDri %stack.10, 0     ; &delta / &view  (sp+104)
  %513 = COPY %504
  %533 = t2LDRi12 %stack.20, 0    ; td
  ```

  and `%513` is live from `bb.43` (6824 B) all the way to the k-loop
  (16080 B) — a ~9.3 kB live range, longer than `i*3`'s (`%526`, 2896 B →
  10624 B). Greedy RA therefore gives `%513` → `r4` and `i*3` → `r5`.
* In `expr+BCEAD`, `bb.43` contains neither `%513` nor `%533`; the delta
  address is rematerialised at the k-loop (`add r4,sp,#104`), so `i*3`
  becomes the longest-lived vreg and gets `r4`, and `td` is not kept in `r6`
  at all.
* In the shipped, `bb.43` reloads `i` (`ldr.w fp,[sp,#40]`) and `td`
  (`ldr r6,[sp,#24]`) but does **not** compute `&stack.10`; the delta
  address is rematerialised at the k-loop (`add r4,sp,#104` at `0x153444`).
  So the shipped behaves like `BCEAD` for `&stack.10` while keeping the
  `ABCDE` tail — a third allocation that no source shape tried reaches.

## 6. New source forms tried this wave (all keep `i*3 = r5` with `ABCDE`)

Format: `pct(local) / size / i3 / pp / delta / s20 / s24 / pool / fp40 / n`.

Scatter spellings (on the `ABCDE` tail):

* `Vector delta = enemyPosition - getPlayer()->getPosition(); VectorLength(delta)`
  → 91.4 / 1984 / r5 / r5+44 / r4+104 (same as `expr`).
* same with `const Vector delta` → identical.
* `const Vector playerPosition = …` / `const Vector &playerPosition = …` /
  `auto playerPosition = …` / `Vector playerPosition(…)` /
  `Vector playerPosition = (…)` → 92.2 / 1984 / r5 / r4+104 / r5+44
  (identical to the `stmt` form).
* reversed operand order (`pp - enemyPosition`, both forms) → 91.4 / 91.8.
* `AbyssEngine::AEMath::operator-(enemyPosition, pp)` explicit → 91.4.
* `int distance; distance = (int) VectorLength(…)` / `int distance = 0; …`
  → 91.4 (identical to `expr`).
* `float distance` / `float dist; int distance = (int) dist` → 56.5 / 1964
  (float comparisons restructure the radius block; `fp40 = 0`); the
  `float dist; int distance = (int) dist` form collapses back to `expr`.

Tail spellings (on `expr+ABCDE`):

* `if (this->playerGun)` → `!= 0` / `__builtin_expect(…,1)` /
  `__builtin_expect(…,0)` / `bool pg = …; if (pg)` → all identical.
* `reinterpret_cast<uint8_t*>(&enemy->field_54)[0] = (uint8_t)1` /
  `reinterpret_cast<char*>(…)` / `((Level*)level)->field_30 = (uint8_t)1` →
  identical.
* `this->lifetimes[i] = -1000000L` / `= 0 - 1000000` /
  `*(this->lifetimes + i) = -1000000` → identical.
* `lastHitKIPlayer = (int)(long) getKIPlayer()` → identical.
* `const uint8_t one = 1;` shared by A and B → identical.
* tail wrapped in a bare `{ … }` block → identical.
* `setHitVector(0.0f - vx, …)` → 84.5 / 2004 (extra `vmov`s).
* `hitPositions.data()[i] = this->positions.data()[i]` → 86.0 / 1988;
  `*(this->hitPositions.data() + i) = position` → identical.

Other levers:

* `mine`/`scatter` as `int` / `const int` (with `expr`) → identical.
* `td` hoisted into the `e`-loop, `radius` declared in the `e`-loop →
  identical.
* `posData`/`velData` pointer hoists → 74.1 / 1992; `npos` count hoist →
  identical; `*(this->positions.data() + i)` → identical.
* view block: `Matrix translated = MatrixSetTranslation(view, …)` →
  81.7 / 1988 with `i3 = r6`; `Matrix view; view = …` → 66.6 / 2056;
  `Matrix view;` declared before the `if (explosive)` or at the top of the
  i-loop → identical; `else { if (geometries) { … } }` nested → identical.
* k-loop: named `Vector ep = enemy->getPosition(); *td = ep;` → identical.
* `__builtin_expect` on `mine` (0) → 88.8 / 1984 / i3=r6 / pool 0x35c;
  on `scatter` (1) → 84.9 / 1980 / i3=r6 / pool 0x360; on `scatter` (0) →
  71.9 / 1992; on `isAsteroid` (0/1) → 54.7 / 43.8; on the collision
  `!(outer && …)` (0/1) → 34.8 / 48.2; on `explodeOnContact` (0) →
  83.3 / 1984 / i3=r5; on `geometries` (1/0) → 70.9 / 61.8.
* 1500 random 2–4-edit combinations over a 61-edit pool (loops, radius,
  ki/outer, view, impact, scatter, k-loop, prologue spellings): **0 hits**
  with `i3 = r4` and `tail = ABCDE`.

## 7. Root cause / conclusion

The shipped function needs, simultaneously:

1. `this=r9, enemy=r8, td=r6, i=fp` (HEAD has this),
2. `i*3 = r4`,
3. `pp = r4/sp+44, delta = r5/sp+104`,
4. the `mov r6,r4` / `mov r4,r6` spill pair around `getPlayer`,
5. `fp40 = 5` (the post-`isAsteroid` `ldr.w fp,[sp,#40]` reloads),
6. `s20 = r2, s24 = r6`, pool at `+0x364`, `n = 650`,
7. the tail emitted `A B C D E`,
8. the rotated k-loop.

(2)+(3)+(4) are reachable only with a `B…`-first tail (`BCEAD`/`BECAD`),
which contradicts (7) and drags in the transposed prologue slots and the
`0x358` pool. Every `ABCDE` tail keeps `i*3 = r5`. The physical cause is
that with `ABCDE` the compiler hoists `&stack.10` (the `delta`/`Matrix view`
address) into both post-`isAsteroid` blocks, giving it a ~9.3 kB live range
that outranks `i*3` for `r4`; the shipped rematerialises that address at the
k-loop instead, which is what frees `r4` for `i*3`.

Best measured variant remains **HEAD at 93.4000 % / 1984 B (650 insns)** —
its instruction offsets are the shipped ones everywhere except the eight
clusters in §2.

## 8. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
cmake --build cmake-build-match --target drift -j6        # 0 fields
cmake --build cmake-build-match --target diff -j6         # symbol parity 5370/5370
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

MIR: `clang++ … -mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv` (release clang: no
`-debug-only=regalloc`; `-print-after-all` still shows `Merge disjoint stack
slots`, `Early Machine LICM`, `Greedy Register Allocator`, `Stack Slot
Coloring`, `Prologue/Epilogue Insertion`).
