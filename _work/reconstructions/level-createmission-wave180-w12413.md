# w12413 — `Level::createMission` wave 180: turret-select anchor + `switch (k)` store order

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w12413`, branch `wave180-w12413`,
base `99e341e9b` (wave-179 harvest 3). Ground truth:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (image base 0x10000; real address =
Ghidra address − 0x10000). This pass changes `src/game/world/Level.cpp` only.

## Result

```text
_ZN5Level13createMissionEv   raw 3832 -> 3809   (-23)
                             aligned 977 -> 736 (-241)
                             verify ratio 92.871287 -> 93.258520 %  (report 92.9 -> 93.3)
                             size 17172 / 17172 exact, insn 6207 -> 6206 / 6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843, imports 207,
      lint CLEAN, drift 0, parity 5370/5370, 27/27 headers, file size 2,187,328
.text diff 76870 -> 76847; .ARM.extab diff 709 (unchanged; createMission record 121,
      unchanged; section size/base exact 0x1a3b78, 156,312 B); .ARM.exidx 0;
      .rel.plt byte-identical; total differing bytes 77598 -> 77575.
```

No baseline update: `byte_exact`/`linked_exact` stay 4507 (the function is still not
byte-exact), so `tools/verify_baseline.json` is untouched.

## 1. The landed body

Two edits, both in `src/game/world/Level.cpp`.

### 1a. `alienGuns` turret-table selector (`~line 1968`)

```cpp
-            const float (*t)[6] = (bigAlienWave + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
+            const float (*t)[6] = (bigAlienWave + 2 * (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
```

`i` is the loop index and is always non-negative, so `2 * (i < 0)` is always 0 and the
expression is semantically identical to `bigAlienWave ? … : …`. It is a **codegen
anchor**: the wave-98 note (`level-createmission-wave98-w4816.md`) established that the
plain selector is hoisted out of the loop by clang, which shrinks the object to 17160 B
and moves the post-loop `if (raid)` / `pirateFortress` blocks. The `(i < 0)` anchor keeps
the select in the loop; multiplying it by 2 changes the *shape* of the branchless select
clang emits and is what snaps the loop body into the original's base+offset form.

Emitted selector, before (wave-98 anchor `(i < 0)`):

```text
  ldr   r1, [pc, #732]          ; kAlienStationTurrets
  ldr   r5, [pc, #732]          ; kAlienOutpostTurrets
  ldr   r1, [sp, #104]          ; bigAlienWave
  add   r0, pc ; add r5, pc
  cmp.w r1, sl, lsr #31
  it    ne
  movne r5, r0
  adds  r0, r5, r6              ; base + k*24
  ldrd  r8, r1, [r0, #4]        ; t[k][1..2]
  ldrd  r1, r2, [r0, #12]       ; t[k][3..4]
```

Emitted selector, after (`2 * (i < 0)`):

```text
  ldr   r0, [pc, #740]          ; kAlienStationTurrets
  movs  r1, #2
  ldr   r5, [pc, #740]          ; kAlienOutpostTurrets
  and.w r1, r1, sl, lsr #30     ; 2 * (i < 0)
  ldr   r2, [sp, #64]           ; bigAlienWave
  add   r0, pc ; add r5, pc
  orrs  r1, r2
  it    ne
  movne r5, r0
  adds  r0, r5, r6              ; base + k*24  (same as the original's `add.w r0,r1,r9`)
```

The original keeps the select branchy (`ldr [sp,#64]; cbz; ldr; add; b; ldr; add`) and
we still emit a branchless `orrs/it/movne`, but the two sides now agree on the
base+offset addressing of `t[k][0..5]`, the loop-carried `k*24` (`r6`/`r9`), the
`MatrixRotateVector`/`Vector::operator=`/`setRotation` sequence, and the loop back-edge,
so the `sizediff` block-move at `0xb32bc`/`0xb34b6` (orig 182 B vs ours 2 B / orig 1 B vs
ours 164 B, `delta ±180`) disappears.

### 1b. `switch (k)` store order in the convoy block (`~line 2197`)

```cpp
                         case 1:
                             offset[1] = -7000.0f;
-                            offset[0] = 10000.0f;
                             offset[2] = -30000.0f;
+                            offset[0] = 10000.0f;
                             break;
                         case 2:
-                            offset[2] = -36000.0f;
-                            offset[1] = 0.0f;
                             offset[0] = 25000.0f;
+                            offset[1] = 0.0f;
+                            offset[2] = -36000.0f;
                             break;
```

Per-case store order becomes `case0=201, case1=120, case2=012, default=201` (the
digits are the `offset[]` indices in emission order). `case 0` and `default` are unchanged
(`201`); only `case 1` and `case 2` move. Worth 1 raw byte on top of 1a (raw 3810 -> 3809
if 1a is held fixed; 3832 -> 3831 on the wave-179 base without 1a).

The original's `switch (k)` lowering (tbh table `0xb02a8`, code at `0xb1418`) decodes as
`beq -> case2`, `cmp #1; beq -> case1`, `cbnz -> default`, fall-through case0, and each
case stores `[1],[0],[2]` (offsets sp+376/372/380 = slot1/slot0/slot2) before a shared
`b.n` to the common store. A uniform `102`/`012` order in all four cases makes clang's
SimplifyCFG `SwitchToLookupTable` fire (`bhi` + table loads), shrinking the object to
17164 B; only mixed orders keep the branchy `tbh` form at 17172 B.

## 2. Why the anchor is 2×, not 1×

The wave-98 `(i < 0)` anchor is size-exact (17172) but leaves the loop body in the
running-pointer form (`add r2, r6, #12; ldmia r2, {r0,r1,r2}` / `ldmia.w r6, {r0,r1,r2}`,
`adds r6, #24`). `2 * (i < 0)` keeps the size (17172) *and* switches the loop body to the
original's base+offset form. Measured anchor sweep (all real links, `201 120 012 201`
switch order):

| anchor condition | raw | aligned | size |
|---|---:|---:|---:|
| `(i < 0)` (wave-98 base) | 3831 | 966 | 17172 |
| `(bigAlienWave + 2 * (i < 0))` **(landed)** | **3809** | **736** | **17172** |
| `(bigAlienWave + 4 * (i < 0))` | 3809 | 736 | 17172 |
| `(bigAlienWave + ((i < 0) << 1))` | 3809 | 736 | 17172 |
| `(bigAlienWave | (2 * (i < 0)))` | 3809 | 736 | 17172 |
| `(bigAlienWave + (i < 0) + (i < 0))` | 3805 | 931 | 17172 |
| `(bigAlienWave + 3 * (i < 0))` | 4129 | 961 | 17172 |
| `(bigAlienWave + 2 * (i >> 31))` | 5086 | 968 | 17168 |
| `(bigAlienWave + 2 * (i < 0u))` | 5474 | 936 | 17160 |
| plain `bigAlienWave` (no anchor) | 5474 | 936 | 17160 |
| `(i < 0 ? 2 : bigAlienWave)` | 5228 | 821 | 17184 |

`add2` (`+ (i<0) + (i<0)`) is 4 raw bytes better but 195 aligned instructions worse; the
landed `2×` form is the best combined shape (raw 3809 / aligned 736 / ratio 93.2585).
Every other anchor spelling tried (`(i<0)*2`, `((i<0)?2:0)`, `+0`, `& 3`, `>> 0`,
`* 1 +`, `0 > i`, `if`/`else` with the anchored condition, `__builtin_expect`) produces
byte-identical code to the landed form.

## 3. Negative results (all real builds)

| experiment | raw | aligned | size | note |
|---|---:|---:|---:|---|
| base (wave-179 harvest 3) | 3832 | 977 | 17172 | — |
| `switch(k)` uniform `102 102 102 102` | 11279 | 1025 | 17164 | SimplifyCFG lookup table |
| `switch(k)` uniform `012 012 012 012` | 11282 | 1025 | 17164 | lookup table |
| `switch(k)` uniform `201 201 201 201` | 11283 | 1023 | 17164 | lookup table |
| `switch(k)` `201 210 012 201` | 3809 | 736 | 17172 | tied with landed |
| `switch(k)` `210 201 012 201` | 3811 | 722 | 17172 | ratio 93.2255 (lower) |
| `switch(k)` `201 102 012 201` | 3819 | 733 | 17172 | — |
| `switch(k)` `102 120 012 201` | 3813 | 735 | 17172 | — |
| `case 3:` label instead of `default:` | — | — | 17124 | mixed order; uniform 17124 |
| `if`/`else if` chain instead of `switch(k)` | — | — | 17224 / 17172 | — |
| dead duplicate store in a case | — | — | 17164 | lookup table |
| `Vector asteroidPos` before `coords[6]` | 3839 | — | 17172 | worse |
| `Vector offset` + `.x/.y/.z` | 4117 | — | 17168 | — |
| remove `Array<KIPlayer*> *enemies` local | 10698 | — | 17180 | — |
| partial enemies-local variants (p3/p4) | 5556/5557 | — | 17172 | — |
| remove `Globals *globals` local | 3937 | — | 17172 | — |
| remove `enemies` local | 3987 | — | 17168 | — |
| `raid`: 30+ if/else, ternary, goto, `!(...)`, `__builtin_expect`, `calm` bool, threshold-chain forms | 3832 | 977 | 17172 | no codegen change; clang already emits `switch(security)` + `select` |
| `pirateEscorts` `setPosition((int)(pirateBase.x+ox), …)` (4 forms) | 3809 | 736 | 17172 | no codegen change (Vector already folded) |

The remaining `sizediff` deltas after the landed change are small and register-allocation
driven: `0xb3c6c` +12 (`vldr s2,[sp,#312]` vs the original's `vadd.f32 s2,s16,s22` in the
`pirateEscorts` loop), `0xb15d0` −8 / `0xb15c8` +6 (inline literal-pool ordering),
`0xb3372` −7, `0xb3df0` +6, `0xb4260` +6 (EH landing-pad order), `0xb16da` −5,
`0xb32c6` +5 (the branchless select above), `0xb141a` −4, `0xb3436` −4. The
`0xb4168`–`0xb42a0` run is the EH cleanup/landing-pad table: the pad blocks are the same
shapes but permuted by one slot, which is a source-scope/destructor-order property.

## 4. Reproduce

```bash
cd /home/debian/GalaxyOnFire2/wt/w12413
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
# raw/aligned/size + section diffs (scratch tool)
python3 /home/debian/GalaxyOnFire2/scratch/w12413/measure.py
#   raw=3809 aligned=736 size=17172/17172 insn=6206/6197
#   .text diff=76847  .ARM.extab diff=709  createMission-extab-rec diff=121
#   .rel.plt identical=True  .ARM.exidx diff=0
cmake --build cmake-build-match --target diff      # SYMBOL PARITY ok
cmake --build cmake-build-match --target drift     # 0 fields
cmake --build cmake-build-match --target check     # RATCHET PASS
```

`measure.py` (scratch) is the byte-diff/aligned tool used for the raw/aligned columns;
the official score is `tools/verify/verify.py`'s difflib ratio (92.871287 -> 93.258520,
reported as 92.9 -> 93.3).
