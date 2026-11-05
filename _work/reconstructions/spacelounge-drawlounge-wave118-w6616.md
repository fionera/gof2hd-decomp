# SpaceLounge::drawLounge — wave-118 RA study (w6616)

Status: research note (2026-11-05). Worktree `wt/w6616`, branch
`wave118-w6616`, base `3d4a98a3e`. **No `src/` change landed**: the committed
HEAD is still the best measured shape. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4501 / linked 4501, data 843/843, imports 207,
  lint CLEAN, drift 0, 27 sections, file size 2,187,328 B exact (unchanged)

Prior work: `spacelounge-drawlounge-wave117-w6517.md` (the 292-variant sweep and
the `boxW` IR-order finding), `-wave116-w6417.md`, `-wave115-w6318.md`,
`-wave114-w6218.md`, `-wave113-w6117.md`, `-wave112-w6015.md`,
`-wave110-w5818.md`. This note supersedes nothing.

## 1. The residual in one paragraph

The whole function is byte-identical to the original except the clamp / join
region `0x1712b2..0x171330`. Exactly two values get different physical
registers: `pad` and `x`.

```text
value        original        HEAD (committed)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole interval)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11) -> r5
pad2 (sl)    r10             r10
lineHeight   fp (r11)        fp (r11)
```

The committed `short pad3 = pad;` is a 1.7-point workaround for `pad` being in
`fp`: with `pad` in `r9` the plain `width + pad * 2` emits the original 8-byte
right arm (`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`), but the extra
`mov r5,fp` copies at the branch targets cost more than the arm saves.

## 2. What wave-118 added

### 2.1 Scratch compiler instrumentation (not tracked)

A scratch LLVM 7.0.2 at `/tmp/llvm_android2` + `/tmp/llvm_android_build`
(same source as the NDK r18b clang) was extended with hidden `-mllvm` knobs.
They are **not** in `CMakeLists.txt` and must not be landed (NDK clang-7 does
not know them):

| flag | effect |
|---|---|
| `-w6616-hintpad` | set the `1<<30` hint-priority bit on the pad interval (size 6300..6450) |
| `-w6616-nohint46` | clear the `1<<30` hint-priority bit on the top-pad interval (size 736) |
| `-w6616-nohintx` | clear the `1<<30` hint-priority bit on the x interval (size 4150..4300) |
| `-w6616-forcepad9` | `setRegAllocationHint(pad, 0, r9)` |
| `-w6616-forcex5` | `setRegAllocationHint(x, 0, r5)` |
| `-w6616-lock` | hard-return r9 for pad / r5 for x when interference-free |
| `-w6616-lock46` | hard-return r5 for top-pad when interference-free |
| `-w6616-boostx` | multiply the x spill weight by 3 in `CalcSpillWeights` |

Reproduce a patched object with `/dev/shm/w6616/cc7.sh` (identical flags to
the cmake build) and then run the gold link command
(`ninja -t commands libgof2hdaa.so | tail -1`).

### 2.2 The important new result

**Faithful clamp + `-w6616-boostx -w6616-forcepad9 -w6616-nohint46` reproduces
the original's clamp and right arm exactly:**

```text
1712b2  vmov    r5, s18          ; x = centerX            (orig r5)
1712bc  sub.w   r1, r5, r9       ; x - pad                (orig r5,r9)
1712c4  add.w   r5, r0, r9       ; x = inset + pad        (orig r5,r9)
1712d4  add.w   r1, r9, r5       ; pad + x                (orig r9,r5)
1712de  addgt.w r1, r6, sl       ; width + pad2           (orig sl)
1712e2  subgt   r5, r0, r1       ; x = (w-inset) - ...    (orig r5, 2 B!)
1712ea  ldr.w   fp, [r4, #48]    ; lineHeight             (orig fp)
1712f8  vmov    r2, s16          ; top                    (orig r2)
1712fc  mov     r1, r9           ; pad                    (orig r9)
1712fe  add     sl, r6           ; boxW                   (orig sl)
171300  subs    r6, r5, r1       ; x - pad                (orig r6)
171302  sub.w   r9, r2, r9       ; top - pad              (orig r5!)
171306  mov     r8, r2           ; top                    (orig r8)
171308  strd    sl, fp, [sp]     ; boxW, lineHeight       (orig)
```

i.e. `pad -> r9`, `x -> r5`, `pad2 -> sl`, `lineHeight -> fp`, `top -> r8`,
and the **8-byte** right arm. This is the first time the whole clamp has been
reproduced from the faithful source.

It still only scores **93.617021 %** (1944 B, 4 B short) because the join is
not identical: the original splits x at the join and reuses r5 for `top - pad`:

```text
original join                         this variant
vmov  r2, s16                         vmov  r2, s16
mov   r3, r5        ; x copy           mov   r1, r9
add   sl, r6                          add   sl, r6
mov   r1, r9                          subs  r6, r5, r1
subs  r6, r3, r1                      sub.w r9, r2, r9
str.w r8, [sp, #12]                   mov   r8, r2
sub.w r5, r2, r9                      strd  sl, fp, [sp]
mov   r9, r3
mov   r8, r2
strd  sl, fp, [sp]
```

The original's `mov r3,r5` / `mov r9,r3` pair is a live-range split of x
(the r5 part ends at the join, the r9 part continues to the DrawString calls).
Our variant keeps x wholly in r5, so `top - pad` has to take r9, and the join
is 4 B shorter.

### 2.3 Why the split does not happen from source

RA trace of HEAD (`-mllvm -debug-only=regalloc`), drawLounge only:

* `x` (`%607`, size 4228, weight 2.946140e-04, hint `$r3` from the first
  `DrawString` x argument) is enqueued at `(1<<31)|(1<<30)|4228`.
* `pad` (`%2`, size 6368, weight 2.077779e-04, **no hint**) is enqueued at
  `(1<<31)|(1<<29)|6368` — i.e. after x.
* x is assigned `r5`; then `%46` (`top - pad`, size 736, weight 6.251309e-04,
  hint `$r3 $r2`) evicts x from r5 (`shouldEvict` = `A.weight > B.weight`),
  takes r5, and x is re-selected into the free `r9`. `pad` is selected much
  later and lands in `fp`.

For the original allocation the eviction must instead leave x split, which
needs `r9` to be occupied by `pad` at the moment x is re-selected. `pad` can
only be selected before `%46` if it carries a hint-priority bit, and it can
only land in r9 if r9 is free over its whole interval. Both together are what
`-w6616-boostx -w6616-forcepad9 -w6616-nohint46` arranges (boostx stops the
x eviction entirely; forcepad9 puts pad in r9; nohint46 keeps `%46` from
jumping the queue). No source-level expression tried so far reproduces that
combination: the hint on `%46` comes from the `$r3 = COPY top-pad` argument
copy of `drawBox`, the hint on x from the `$r3 = COPY x` argument copy of the
first `DrawString`, and `pad` has no COPY at all in the IR (it is only used
by `t2SUBrr`/`t2ADDrr`/`t2LSLri`), so it cannot acquire a hint from source.

## 3. Measurements this wave

Harness: patch `src/game/menu/SpaceLounge.cpp`, `cmake --build
cmake-build-match --target link_so -j6` (~1.2 s), then the official
`verify.py` internals (`/dev/shm/w6616/fastscore.py`, `autojunk=False`).
"pad/x" is the register of `ldr.w X,[r6,#148]` and `vmov X,s18`.

### 3.1 Source variants (all <= 95.676047)

| variant | raw % | size | pad | x |
|---|---:|---:|---|---|
| HEAD (`short pad3`) | **95.676047** | 1948 | fp | r9 |
| `short pad3` after `int x` | 95.676047 | 1948 | fp | r9 |
| `pad3` in the condition (`pad3 + x + width`) | 95.401510 | 1948 | fp | r9 |
| faithful `width + pad * 2` | 93.964335 | 1948 | fp | r9 |
| faithful, `lineHeight` inlined into `drawBox` | 93.964335 | 1948 | fp | r9 |
| faithful, `int pad2 = pad*2` local | 93.964335 | 1948 | fp | r9 |
| faithful, `int x2 = x` after clamp / after lineHeight / before clamp | 93.964335 | 1948 | fp | r9 |
| faithful, `x + GetTextWidth(...)` in 2nd DrawString | 95.676047 | 1948 | fp | r9 |
| faithful, `x + 0` as 1st DrawString x | 95.676047 | 1948 | fp | r9 |
| faithful, `-pad + top` / `top + -pad` for drawBox y | 93.415638 | 1948 | fp | r9 |
| faithful, `-pad + x` for drawBox x | 93.964335 | 1948 | fp | r9 |
| faithful, `width + pad + pad` for drawBox w | 72.889499 | 1960 | fp | r6 |
| faithful, `pad + pad + width` for drawBox w | 72.752231 | 1960 | fp | r6 |
| `boxW = width + pad*2` after clamp | 72.602740 | 1968 | **r9** | **r5** |
| `boxW` after `lineHeight` (both orders) | 72.602740 | 1968 | r9 | r5 |
| `boxW` + `lineHeight` inlined | 72.602740 | 1968 | r9 | r5 |
| `const int boxW` | 72.602740 | 1968 | r9 | r5 |
| `boxW` before the clamp | 89.759450 | 1940 | fp | r9 |
| `int boxY = top - pad` after clamp (drawBox only) | 92.307692 | 1944 | r5 | r9 |
| `pad3` used in `top - pad` | 91.208791 | 1952 | sl | r9 |
| `pad3` used in drawBox w | 71.819425 | 1972 | r5 | fp |

### 3.2 Patched-compiler runs on the faithful source

| flags | raw % | size | pad | x | note |
|---|---:|---:|---|---|---|
| (none) | 93.964335 | 1948 | fp | r9 | identical to stock |
| `-w6616-hintpad` | 78.302533 | 1972 | r8 | sl | pad jumped the queue, wrong reg |
| `-w6616-forcepad9` | 83.321894 | 1960 | r9 | sl | pad in r9, x moved to sl |
| `-w6616-nohintx` | 69.088417 | 1964 | r4 | fp | pad in r4 |
| `-w6616-boostx` | 81.369863 | 1968 | fp | r5 | x keeps r5, pad still fp |
| `-w6616-nohint46` | 93.964335 | 1948 | fp | r9 | no change |
| **`-w6616-boostx -w6616-forcepad9 -w6616-nohint46`** | **93.617021** | **1944** | **r9** | **r5** | **exact clamp + 8 B arm; join 4 B short** |
| `-w6616-boostx -w6616-hintpad -w6616-nohint46` | 88.797251 | 1940 | — | — | pad not r9 |
| `-w6616-lock -w6616-lock46` | 93.964335 | 1948 | fp | r9 | locks not honoured (interference) |
| `boxW` + `-w6616-boostx -w6616-forcepad9 -w6616-nohint46` | 72.902338 | 1948 | r6 | r5 | size exact, wrong regs |

## 4. Conclusion / next step

The clamp itself is now fully understood and reproducible from the faithful
source (see 2.2). The last missing piece is the **x live-range split at the
join**, which is what frees r5 for `top - pad` and puts x in r9. In the
original the split is a consequence of `%46` evicting x while `pad` already
occupies r9; in every source shape measured the allocator either keeps x
wholly in r5 (top-pad takes r9) or moves x wholly to r9 (pad takes fp).

A promising next experiment is a source shape that gives `pad` a COPY to a
physical register in the IR (so it acquires a hint) without changing the
generated code, or one that makes `%46`'s argument copy unhinted — neither is
obvious from the current source, because the whole function outside the clamp
is already byte-identical. HEAD (`short pad3`, 95.676047 %) remains the best
measured shape.
