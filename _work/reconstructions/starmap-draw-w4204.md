# StarMap::draw — 15-byte residual and the `-likely-branch-weight=7` flag (w4204)

Status: analysis pass, 2026-11-03. Worktree `wt/w4204`, branch `wave92-w4204`,
base master `b379d00d`. Every number below was measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 and the official `tools/verify/verify.py` (`autojunk=False`).
Scratch drivers live in `/dev/shm/w4204` (`fdiff.py`, `fdiff2.py`, `cc.sh`, `mkv2.py`,
`mkpulse.py`, `mksv.py`).

Build note: this is a git worktree, so the default `../_work` paths do not resolve.
Configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.
`FN=... verify-fn` is broken here (`--show` receives no argument); call
`tools/verify/verify.py --show <mangled>` directly.

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4498 / 4524 | **4498 / 4524** |
| linked_exact | 4498 / 4524 | **4498 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (-8).
**No source change landed**: the flag experiment below is a regression, and no faithful
source shape reproduces the original allocation.

## 1. StarMap::draw — every remaining diff byte

Function @ `0xc90f4`, 2644 B, **15 differing bytes / 8 runs**. The diff is the same
r8/fp role swap + hoisted `this->systems` load documented in w3820/w4020/w4119; this
pass re-measured it and classifies every byte.

Raw runs (all 15 bytes; `orig` = original, `ours` = current source with the flag):

```
+0x246..+0x246 (1 B)  c933a  add.w r8, sp, #136      vs add.w fp, sp, #136
+0x24d..+0x24d (1 B)  c9341  mov   r1, r8            vs mov   r1, fp
+0x26d..+0x26f (3 B)  c9361  ldr.w fp, [sp, #16]     vs ldr.w r8, [r4, #84]
+0x2b1..+0x2b1 (1 B)  c93a5  mov   r1, r8            vs mov   r1, fp
+0x2be..+0x2bf (2 B)  c93b2  mov   r0, fp            vs ldr   r0, [sp, #16]
+0x3e2..+0x3e4 (3 B)  c94d6  ldr   r0, [r4, #84]     vs ldr.w r0, [r8]
                      c94d8  ldr   r0, [r0, #0]       vs (folded)
+0x8d0..+0x8d1 (2 B)  c99c4  mov   r0, fp            vs ldr   r0, [sp, #16]
+0x972..+0x973 (2 B)  c9a66  mov   r0, fp            vs ldr   r0, [sp, #16]
```

Classification (no encoding, scheduling, or semantic differences):

| bytes | site | class | cause |
|---:|---|---|---|
| 2 | `c933a`, `c9341` | RA | `&Vector temp` (sp+136) is r8 in the original, fp in ours |
| 3 | `c9361` | RA + source shape | original keeps `&this->scratchVector` (this+0x78) in fp; ours hoists `this->systems` (this+0x54) into r8 |
| 2 | `c93a5`, `c93b2` | RA | `Vector::operator=` source is r8/fp, destination is fp vs `[sp,#16]` |
| 3 | `c94d6` | source shape | original re-reads `this->systems` and `->size()` every iteration; ours reads the hoisted `[r8]` |
| 4 | `c99c4`, `c9a66` | RA | `&this->scratchVector` is fp in the original, spilled to `[sp,#16]` in ours (two animated-route copies) |

The original allocation in the second (route) loop is:

```
r8  = &Vector temp (sp+136)
fp  = &this->scratchVector (this+0x78)
sl  = 0 (the 64-bit zero for `101 - getPlayingTime()`), later Canvas, then 0 again
[r4,#0x54] = this->systems, re-read each iteration
[sp,#20]   = Globals::Canvas value (loaded once at entry, re-loaded into sl per iteration)
```

Ours (current source, `Array<SolarSystem *> &systems = *this->systems;` bound before the
loop):

```
fp  = &Vector temp (sp+136)
r8  = this->systems (hoisted once)
sl  = 0
[sp,#16] = &this->scratchVector (spilled)
[r4,#0x54] read once, then [r8] each iteration
```

The reference is a loop-invariant SSA value, so clang keeps `this->systems` in r8; that
displaces the `Vector` temp into fp and `&this->scratchVector` onto the stack. The
original re-reads `this->systems` and keeps `&this->scratchVector` in fp.

### 1.1 Direct form (`this->systems->size()`)

The direct form restores the re-read (`ldr r0,[r4,#84]; ldr r0,[r0,#0]`) and puts the
`Vector` temp back in r8, but the function shrinks to **2636 B (-8)** and the loop body
changes allocation: `Globals::Canvas` is hoisted into sl and the zero constant moves to
fp, so `&this->scratchVector` is still spilled. The 8 missing bytes are exactly the
Canvas reload (`ldr.w sl,[sp,#20]` + `mov.w sl,#0`) that the original pays inside the
loop. This was re-confirmed this pass: direct = 2636 B, 1656 raw diff bytes.

## 2. The `-likely-branch-weight=7` per-file override

`CMakeLists.txt` applies

```
set_source_files_properties(.../src/game/world/StarMap.cpp
    PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=7;-mllvm;-unlikely-branch-weight=1")
```

`StarMap.cpp` has exactly two `__builtin_expect(..., 1)` sites:

* line 696, `drawOnScreenInfo`: `getStationEnumIndex(93) >= 0` (the campaign-target arm);
* line 975, `draw`: `starSystemRoot != 0 && stations != 0`.

The flag sets the front-end `llvm.expect` weight to 7:1 (default 2000:1). In the emitted
IR the metadata is `!{!"branch_weights", i32 7, i32 1}` at the first site and
`!{!"branch_weights", i32 1, i32 7}` on the two `&&` branches of the second.

### 2.1 Removing the flag is a large regression

Removing the whole `set_source_files_properties` block and rebuilding:

| metric | with flag | without flag |
|---|---:|---:|
| `drawOnScreenInfo` size | 0xda0 (3488) | **0xdac (3500, +12)** |
| `draw` address | 0xc90f4 | 0xc9100 (+12) |
| `.text` size | 0x1337c8 | **0x1337e8 (+0x20)** |
| byte_exact | 4498 | **2404** |
| linked_exact | 4498 | **4497** |

The 12-byte growth of `drawOnScreenInfo` shifts `draw` and every later function; every
function with a PC-relative data reference then differs in raw bytes, hence the 2094
byte_exact loss. (The `.text` grows by 0x20, not 0xc, because of the following
32-byte-aligned section.) So the flag cannot simply be dropped.

### 2.2 Weight sweep: the original needs exactly 7:8

Compiling `StarMap.cpp` with `-mllvm -likely-branch-weight=N` (and
`-unlikely-branch-weight=1`) and reading the `drawOnScreenInfo` size / site-1 branch:

| N | `drawOnScreenInfo` | site-1 `getStationEnumIndex(93)` branch | allocation |
|---:|---:|---|---|
| 1..6 | 0xd98 | `bge` to then (out of line) | campaign `[sp,#24]`, freelance r6 |
| **7** | **0xda0** | **`bge` to then (out of line)** | **campaign r6, freelance `[sp,#20]`** |
| 8..2000 | 0xdac | `blt` to else (then falls through) | campaign r6, freelance `[sp,#20]` |

Only N=7 reproduces the original 0xda0 size and allocation. The natural LLVM heuristics
(`calcZeroHeuristics` = 20:12 = 0.625) give the 0xd98 allocation, not 0xda0. This is a
register-allocation threshold, not a block-placement threshold: the site-1 branch is
`bge` for N=1..7 and flips at N=8.

### 2.3 Faithful source shapes tried (all 0xd98, no flag)

Every natural spelling of the site-1 condition was compiled with no flag and checked
for the 0xda0 allocation:

| variant | size |
|---|---:|
| plain `>= 0` | 0xd98 |
| `(unsigned) x < 0x80000000u` | 0xd98 |
| `x != -1` | 0xd98 |
| `x > -2` | 0xd98 |
| `x + 1 > 0` | 0xd98 |
| `!(x < 0)` | 0xd98 |
| `(x >= 0) && true` | 0xd98 |
| `(x >= 0) \|\| false` | 0xd98 |
| reversed ternary (`!stationMode ? system : station`) | 0xd98 |

None reaches 0xda0. The 7:8 weight is therefore the original build's *effective* branch
probability at this site; there is no source-level comparison form in clang 7.0.2 that
produces it naturally (`__builtin_expect_with_probability` is not available in this
clang — it fails with "use of undeclared identifier"). The per-file override is the
faithful encoding of that probability, not an arbitrary anchor.

### 2.4 The flag does not affect `draw`

Compiling `StarMap.cpp` with N=7 and with N=2000 and comparing the object-level
instruction streams of `draw` (895 instructions, addresses/relocations stripped):

```
len 895 895   diffs 0
```

`draw`'s block layout and allocation are identical under 7:1 and 2000:1. The flag is
needed only for `drawOnScreenInfo`'s site-1 branch. Consequently the 15-byte `draw`
residual is unaffected by the flag; the extra raw diffs in the no-flag build are only
the 12-byte address shift.

## 3. Other source shapes tried for the 15-byte residual

All compiled with the flag (so `drawOnScreenInfo` stays 0xda0) and checked for the
original second-loop preheader (`ldr.w fp,[sp,#16]` + `mov.w sl,#0`):

| variant | `draw` size | preheader |
|---|---:|---|
| current (`Array& systems`, `systems.size()`) | 0xa54 (2644) | fp=temp, r8=systems, sl=0 (15 bytes) |
| direct (`this->systems->size()`) | 0xa4c (2636) | r8=temp, sl=Canvas, fp=0 |
| direct + `Vector &sv = this->scratchVector` used for assignment/hidden/pulse reads | 0xa4c | r8=temp, sl=Canvas, fp=0 |
| direct + `101LL` / `(long long)101` / `101u` pulse | 0xa4c | r8=temp, sl=Canvas, fp=0 |
| direct + `long long t = 101 - ...` / `long long t = getPlayingTime()` | 0xa4c | r8=temp, sl=Canvas, fp=0 |

The `Vector &` reference is fully rematerializable (`add rX, r4, #0x78`), so it does not
force fp; changing the pulse expression does not change the allocator's choice between
Canvas (sl) and the zero constant (fp). The residual is a genuine three-way allocator
tie between `&temp`, `&scratchVector`, and the zero/Canvas value, exactly as w4119
concluded.

## 4. Conclusion

- The 15 bytes are all register-allocation / source-shape: 2 bytes r8/fp temp address,
  3 bytes `&scratchVector` vs hoisted `this->systems`, 2 bytes operator= operands,
  3 bytes `this->systems` re-read vs hoisted `[r8]`, 4 bytes scratchVector spilled in
  the two animated-route copies. No encoding, scheduling, constant, or callee
  differences.
- The `-likely-branch-weight=7` override is load-bearing and specific to
  `drawOnScreenInfo`'s `getStationEnumIndex(93) >= 0` branch. Removing it grows
  `drawOnScreenInfo` by 12 bytes and drops byte_exact 4498 -> 2404. No natural source
  shape reproduces the required 7:8 probability; keep the flag.
- `draw` itself is unaffected by the flag (identical 895-instruction stream at 7:1 and
  2000:1). The 15-byte residual remains the same three-way allocator tie documented in
  w4119.
- No source change landed; the gate is unchanged at byte 4498 / linked 4498.
