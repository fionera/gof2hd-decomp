# MenuTouchWindow::OnTouchBegin wave 191b (w13712): IR-hybrid sweep, no win

Date: 2026-11-13. Worktree `wt/w13712`, branch `wave191b-w13712`, base
`df4e658df`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, function at `0x129bd4`, 1,896 B).
Baseline and best measured this wave: **raw 28 / aligned 16 / size
1896/1896 / 754/754 insns**. `src/` is unchanged; the gate is green.

Gate at HEAD and after this wave (`check`): `RATCHET PASS` — byte_exact
**4508/4524**, linked_exact 4508/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370 (0 extra / 0
missing), stub_zero_size 0. Section deltas unchanged: `.text` 71,375,
`.ARM.extab` 577, `.ARM.exidx` 0, `.rodata` 0, `.note.gnu.build-id` 20;
`.rel.plt` byte-identical.

## 1. Residual map at HEAD (28 bytes, 16 aligned lines)

All four regions are pure register-allocation differences; every opcode and
operand shape is otherwise identical (754/754 instructions).

| region | byte offsets | original | ours |
|---|---|---|---|
| A (case 8, 1st `if`) | `0x401`,`0x404` | `ldr.w ip,[sl,#340]` / `add r3,ip` | `ldr.w r4,...` / `add r3,r4` |
| B (case 15 c1) | `0x60f`..`0x630` | `r3=heapBufB`, `r6=field_0x1e0`, `r5=w`, `r2=layout value` | `r2/r3/r4/r6` permutation |
| C (case 15 c2) | `0x640`..`0x652` | fresh `Globals::layout` value after the first call; `lm` (off 12) **and** `top` (off 32) both from it | `lm` from cached `Layout&` (`%29`, r6); `top` fresh |
| D (case 15 c3) | `0x672`,`0x678` | `r5=buttonInsetX` | `r4=buttonInsetX` |

`[sp,#16]` spill/reload pattern is identical to HEAD: 8 sites (entry store
at `0x129c14`, loads at `0x129d20`, `0x129f84`, `0x129ffa`, `0x12a1bc`,
`0x12a212`, `0x12a23c`, `0x12a2b6`). `fp` = `&Globals::Canvas` in both
(`ldr.w fp,[r1]` at `0x12a1ec`; deref `f8db 0000` at `0x12a1f8/0x12a21a/
0x12a242`).

## 2. Key IR finding: the original is a HEAD/D hybrid

Post-RA MIR (`-mllvm -print-machineinstrs`, "After Virtual Register
Rewriter") pins the two shapes:

* **HEAD** (28/16/1896, 7 address loads): `%29` = layout value loaded in
  bb.42 and **live across the first call**, used for `lm` in bb.44;
  `%99` (`&Globals::layout`) is rematerialised at every use; `fp` =
  `&Globals::Canvas`.
* **D = all-direct `Globals::layout->`** (362/76/1892, 5 address loads):
  `%29` is dead after c1; bb.44 loads one fresh value for `lm`+`top`;
  Greedy-RA keeps `%99` in `fp` and demotes `&Globals::Canvas` to `r5`.

The original needs **HEAD's 7 address loads (754 insns) with D's fresh c2
value**. In other words: `%29` dead, `%99` rematerialised, `fp` =
`&Globals::Canvas`. That third combination is exactly the one w13666
reported unreachable; this wave re-confirmed it and adds the instruction
count argument (D is 2 instructions / 4 B short precisely because the RA
keeps `%99`).

Mapped against the original asm, the wanted bb.44 is:

```
12a212  ldr  r2,[sp,#16]        ; %99 remat
12a214  ldr.w r1,[r0,r1,lsl#2]  ; texB
12a218  ldr  r2,[r2,#0]         ; fresh layout value
12a21a  ldr.w r0,[fp]           ; Canvas
12a21e  ldr  r5,[r2,#12]        ; lm
12a220  ldr  r6,[r2,#32]        ; top
```

## 3. Measured table (all reverted; 1896 B unless noted)

Every candidate was a full gold link measured with `tools/fn_diff.py`
(scratch harness `/home/debian/GalaxyOnFire2/scratch/w13712/try.py`,
`batchfull.py`).

### 3.1 case-15 block shapes (the c2 value source)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`lm` cached, `top` fresh) | **28** | **16** | 1896 |
| topcached (`lm` fresh, `top` cached) | 29 | 15 | 1896 |
| c1c_topc (c1 cached + `top` cached) | 29 | 15 | 1896 |
| two_ref_top (2nd `Layout&` for `top`) | 29 | 15 | 1896 |
| D all-direct | 362 | 76 | 1892 |
| D + `(*Globals::layout).field` | 362 | 76 | 1892 |
| D + `Layout *&lp = Globals::layout` | 362 | 76 | 1892 |
| D + unused `Layout&` / `(void)layout` / `volatile` ref | 362 | 76 | 1892 |
| c1c_only (c1 cached, c2 fresh) | 362 | 76 | 1892 |
| c3c_only (c3 cached) | 322 | 74 | 1888 |
| c1c_c3c (c1+c3 cached, c2 fresh) | 322 | 74 | 1888 |
| allcached (c1+lm+top+c3 cached) | 316 | 76 | 1888 |
| c2both (lm+top cached, c1 direct) | 403 | 85 | 1872 |
| c2both_c1c | 403 | 85 | 1872 |
| d_lmlocal (`int lm = ...` before if) | 320 | 33 | 1892 |
| d_toplocal (`int top = ...` before if) | 320 | 33 | 1892 |
| d_lmtop (both locals) | 375 | 83 | 1872 |
| d_barrier (`& 0x7fffffff` on lm) | 121 | 69 | 1896 |
| allc_barrier (barrier on every cached field) | 306 | 47 | 1896 |
| head_lm_barrier (barrier on cached lm) | 28 | 16 | 1896 |
| ref_lmtop / lmtop_ref / lmref_top | 355 | 42 | 1888 |
| nested1 / nested2 / split blocks | 362-363 | 76-84 | 1892-1896 |
| two_ref_c2both / two_ref_all2 / two_ptr_c2both | 308-316 | 73-76 | 1888-1892 |
| ptrref / ptrref_lm / ptrref_c1 | 362 | 76 | 1892 |
| deref / deref2 | 362 | 76 | 1892 |
| d_canvas_c1c2 | 316 | 40 | 1892 |
| h_canvas_c1c2 | 49 | 69 | 1896 |

### 3.2 case-15 c1/c2/c3 association and operand order (48-cell sweep)

All combinations of c1 `{w-inset-W, w-(inset+W), (w-W)-inset, w+(-inset-W)}`
x c2 `{top+(lm+H), (top+lm)+H, lm+(top+H), (lm+top)+H, H+(top+lm),
top+lm+H}` x c3 `{inset+W, W+inset}` on the D shape: best **345/85/1884**,
none below 300.

### 3.3 c3 (region D) spellings

| c3 expression | raw | aligned | size |
|---|---:|---:|---:|
| `x < inset + W` (HEAD) | **28** | **16** | 1896 |
| `inset + W > x` | 28 | 16 | 1896 |
| `x < (inset + W)` / `x < inset + (W)` / `!(x >= ...)` / `inset+W-x > 0` | 28 | 16 | 1896 |
| `x < W + inset` | 42 | 22 | 1896 |
| `x - W < inset` | 268 | 39 | 1896 |
| `x - inset < W` | 274 | 37 | 1896 |
| `(unsigned)x < (unsigned)inset + (unsigned)W` | 29 | 17 | 1896 |

### 3.4 case 8 (region A) and neighbouring code

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **28** | **16** | 1896 |
| `5 != i` (case 7) | 28 | 16 | 1896 |
| `sliderSlot5Enabled` without `!= 0` | 28 | 16 | 1896 |
| `(int)this->listEntryWidth` | 28 | 16 | 1896 |
| swap 3rd/4th first-`if` conditions | 28 | 16 | 1896 |
| `id == 52 \|\| id == 60` (case 15 loop) | 30 | 20 | 1896 |
| `int i` for the case-15 scrollSlots loop | 29 | 17 | 1896 |
| `int i` for case-4 / case-16 loops | 29 | 17 | 1896 |
| swap case-7 optBtn call order | 30 | 18 | 1896 |
| swap case-8 local declarations | 30 | 18 | 1896 |
| `listEntryWidth` first / middle in c2 sum | 33 | 17 | 1896 |
| case-7 `Globals::layout->` -> function-top `layout->` | 93 | 36 | 1896 |
| case-8 `Globals::layout->buttonInsetX` -> `layout->` | 567 | 72 | 1896 |
| final `Globals::layout->OnTouchBegin` -> `layout->` | 1740 | 452 | 1900 |
| function-top `layout` used for case-15 c1/c2/c3 | 1746-1756 | 454-846 | 1892-1896 |
| move case-15 block before the scrollSlots loop | 453 | 90 | 1864 |

## 4. Conclusion

The residual is the Greedy-RA keep-vs-remat tie-break on `%99`
(`&Globals::layout`) coupled to `%29` (the cached layout value) liveness.
Every source shape lands in one of three buckets:

* `%29` live across the first call -> 1896 B, `fp = &Globals::Canvas`,
  `%99` remat'd; raw 28 (`lm` cached) or 29 (`top` cached).
* `%29` dead -> 1892 B (or smaller), `%99` kept in `fp`,
  `&Globals::Canvas` in `r5`; raw 291-362.
* all-cached -> 1888 B, both `%99` and `%29` kept; raw 316.

The original's hybrid (`%29` dead, `%99` remat'd, `fp = &Globals::Canvas`,
754 insns) was not reachable by the ~150 new shapes measured this wave.
Region A (`ip` vs `r4`) and region D (`r5` vs `r4`) are independent
Greedy-RA tie-breaks; every spelling that flips them also perturbs the
literal pool or the downstream branch offsets. HEAD is unchanged; the gate
is green.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13712
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# raw=28 aligned=16 size=1896/1896 insn=754/754
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13712` (`try.py`,
`batch.py`, `batchfull.py`, `v1..v23.py`, `head.mir`/`d.mir` full
`-print-machineinstrs` dumps, `orig.asm`/`ours.asm`/`d.asm`/`allc.asm`/
`lml.asm`/`topc.asm`/`bar.asm`).
