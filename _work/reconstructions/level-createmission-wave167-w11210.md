# w11210 (wave 167): `Level::createMission` — guard `icmp` de-sinking sweep
# (no raw win; 5,436 / 767 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11210`, branch
`wave167-w11210`, base master `d00057e6d`.  Continues
`level-createmission-wave166-w11111.md` (frame map + phi/block/decl sweep)
and the w11110 `SinkCmpExpression` finding.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5436   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. IR shape and where the `and` is split

The last pre-isel IR (`-mllvm -print-after-all`, last `Simplify the CFG`)
has the guard as an **`and` of two `icmp`s**:

```llvm
; preheader %1123
%1124 = icmp ne i32 %1116, 0          ; alienWave != 0  (i32 phi in preheader)
...
; loop body %1409
%1403 = add nsw i32 %1400, %1401      ; first (loop header %1399)
%1410 = icmp eq i32 %1402, %1403      ; i == first
%1411 = and i1 %1124, %1410
br i1 %1411, label %1412, label %1528 ; body / else-if
```

`CodeGenPrepare::SinkCmpExpression` (cgp.cpp:1187) sinks `%1124` from the
preheader into `%1409`, so at isel both `and` operands are `ICmpInst`s in
one block.  `SelectionDAGBuilder::visitBr` (sdb.cpp:1990) then calls
`FindMergedConditions` on the one-use `and` and `ShouldEmitAsBranches`
(sdb.cpp:1938) returns true (the two compares have different LHS/RHS), so
the `and` is emitted as **two conditional branches**:

```text
2e2c: ldr  r0, [sp, #60]   ; alienWave
2e2e: cmp  r0, #0
2e30: beq.w 2f48           ; -> else-if
2e34: cmp  fp, r3          ; i vs first
2e36: bne.w 2f48           ; -> else-if
```

The original instead **materialises both operands and ORs them** at the
same function offset `0x2e2c`:

```text
b2e04: subs.w r0, fp, r3   ; i - first
b2e08: mov    r1, r0
b2e0a: it     ne
b2e0c: movne  r1, #1       ; r1 = (i != first)
b2e0e: ldr    r2, [sp, #60]; alienWave (same slot as ours)
b2e10: eor.w  r2, r2, #1   ; r2 = !alienWave (i1 xor, not an icmp)
b2e14: orrs   r1, r2
b2e16: bne.w  b2f2a        ; -> else-if
```

i.e. the original's isel-time condition is `or i1 (icmp ne i,first), (xor alienWave,true)`
— the `xor` is **not** an `ICmpInst`, so `FindMergedConditions` cannot split
it and `orrs` survives.  The second (`pirateFortress`) guard is the same
pattern at `b2f2a` (`cmp fp,r3` + `eor` + `orrs`).

## 2. Source shapes tried (all size-checked, linked `fn_diff`)

`raw` is `tools/fn_diff.py` on the linked `.so`.

| # | guard source shape | size | raw | aligned |
|---|---|---:|---:|---:|
| HEAD | `i == first && alienWave` | 17172 | **5436** | **767** |
| v6 | `bool alienWave` + `bool pirateFortress` + `alienWave && i==first` | 17160 | 12363 | 1019 |
| b0 | `bool` both, `i==first && alienWave` (operand order kept) | 17168 | 15965 | 3334 |
| h6 | `if (!((i != first) \| !alienWave))` | 17172 | 12460 | 1685 |
| i2 | h6 first, `(i==first) & pirateFortress` second | 17172 | 12563 | 1706 |
| l1 | `if (!((i != first) \| (alienWave == 0)))` | 17172 | 5447 | 790 |
| l4 | `if (((i != first) \| !alienWave) == 0)` | 17172 | 5436 | 767 |
| l3/l5 | `!((i != first) \| !alienWave)` / `(bool)(alienWave^1)` | 17172 | 5447 | 790 |
| m1 | `bool alienWaveFlag = alienWave != 0;` before loop, used in guard | 17172 | 5436 | 767 |
| m2 | m1 + `bool pirateFortressFlag` | 17168 | 13234 | 2222 |
| g1..g8 | `(alienWave & 1)`, `(alienWave ^ 1) == 0`, `!(alienWave==0)`, `alienWave == 1`, … | 17172 | 5436 | 767 |
| k1..k5 | `__builtin_expect` on the guard (0/1, inner/outer) | 17140–17192 | — | — |

### h6 is the only shape that de-sinks the `icmp` at size 17,172

`h6` = `if (!((i != this->friendCount + this->field_164) | !alienWave)) {`
emits the OR form (`subs.w r0, fp, r3; it ne; movne r0,#1; ldr r1,[sp,#116];
orrs r0,r1; beq`) for the first guard, but:

* the `!alienWave` is hoisted to the preheader and spilled to **sp+116**
  (original keeps it inline from **sp+60** with `eor #1`), and
* the guard lands at function offset `0x2e20`, 12 B *before* the original's
  `0x2e2c`, because the hoisted `!alienWave` shortens the pre-guard code.

Result: raw **5436 -> 12460** (worse).  `l4`
(`if (((i != first) | !alienWave) == 0)`) is size-exact and raw-neutral
(5436/767, byte-identical codegen) but leaves the `and` split intact, so it
does not achieve the de-sinking.

## 3. Why the residual raw is not a block shift

The post-guard region is **not** a constant-offset shift of the original:
comparing ours at offset `X` with the original at `X+s` for `s = 0..16`
over `0x2e2c..0x4314` gives at most 650/5340 matching bytes (12%).  The
aligned diff is 767 instructions over 433 regions, spread across the whole
function and concentrated in `0x3000..0x4300`:

```text
bucket  aligned diffs      bucket  aligned diffs
0x0000   3                 0x2400  24
0x0400   1                 0x2800  33
0x0800   2                 0x2c00  52   <- guard
0x0c00   7                 0x3000  98
0x1000  18                 0x3400 116
0x1400  63                 0x3800 106
0x1800  33                 0x3c00  39
0x1c00  26                 0x4000 124
0x2000  22
```

So the guard is only ~50 of the 767 residual instructions; the rest is
pervasive register-allocation/scheduling difference.  Any source change
that alters the guard's instruction count (16 B split form vs 24 B OR form)
also perturbs the preheader (`!alienWave` hoisting / spill-slot colouring),
which is why every de-sinking shape measured *worse* on raw.

## 4. Conclusion

* The w11110 diagnosis is confirmed: `SinkCmpExpression` sinking `%1124`
  is what lets `FindMergedConditions` split the `and`.  A source shape that
  keeps the `xor` (i1 `!alienWave`) in the loop block does produce the
  original's `subs`/`orrs` guard.
* But every size-exact de-sinking shape measured is raw-worse
  (12,460–12,563 vs 5,436); the `bool` variants also re-colour the frame
  (alienWave moves sp+60 -> sp+116).
* Nothing landed: tree left clean at the baseline (5,436 / 767 / 17,172 B).

## Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11210/{var.py,mkv.py,gtest.py,htest.py,itest.py,ktest.py,
ltest.py,rawoff.py,buckets.py,shift.py,dd.py,obj.dis,orig.dis,cgp2.log}`.
