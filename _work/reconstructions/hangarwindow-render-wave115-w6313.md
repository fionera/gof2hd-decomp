# HangarWindow::render — wave 115 (w6313): allocation/anchor search, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6313`, branch `wave115-w6313`.

Builds on `_work/reconstructions/hangarwindow-render-wave114-w6212.md`
(91.199559 landed). This pass: **91.199559 -> 91.199559** (no improvement
found). The deliverable is this measurement record plus a correction to the
wave-114 handoff.

## Result

| metric | before (HEAD b847d5d46) | landed |
|---|---:|---:|
| `render` fuzzy (verify, 0.1) | 91.2 | **91.2** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626 / 3635 | 3626 / 3635 |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | same |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4500 / 4500 | 4500 / 4500 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

## Correction to the wave-114 handoff (frame size)

The handoff claimed "our whole-function stack frame is 384 B vs original 376 B"
and that every `sp,#N` is shifted +8. **This is wrong.** Both prologues are
byte-identical:

```
1490d8: b5f0        push  {r4, r5, r6, r7, lr}
1490da: af03        add   r7, sp, #12
1490dc: e92d 0f80   stmdb sp!, {r7, r8, r9, sl, fp}
1490e0: ed2d 8b06   vpush {d8-d10}
1490e4: b0de        sub   sp, #376          <-- both, identical
```

and there is exactly one `sub sp,#N` in each function. The `String` temporaries
are also at identical slots in both: `sp+144,156,168,180,192,204,216,228,240,
252,264,276,288,300,312,324,336,348,360` (19 slots, stride 12). The
`add r0,sp,#N` histogram is identical except one extra `#288` use in ours
(12 vs 13), which is the `i == 0` branch placement, not a frame difference.
So the "biggest remaining lever" from the handoff does not exist.

Also corrected: the wave-111 note's claim that `buttonOffset` lives at
`[sp,#80]` in the original and `[sp,#308]` in ours is stale — both store it at
`[sp,#80]` at the same six sites (`0x1494f6, 0x1498f8, 0x149ad8, 0x149b92,
0x149d38, 0x14a6ce`).

## What the remaining diff actually is

`tools/verify` diff of the current build has 226 hunks / 639 diff lines
(315 vs 324 changed insns). Classified:

* **172 hunks** are register-role differences (`r4` vs `r0`, `sl` vs `r9`, …)
  or scheduling differences (`ldrd r5,r6` vs two `ldr`, `ite ne` vs
  `movw`+`it ne`).
* **54 hunks** are length mismatches (ours has 9 more instructions overall).

The call sequence is **identical** except one site: the free-credits loop's
`getWidth`/`getHeight` order (call 316/317). The original calls `getWidth`
first, ours calls `getHeight` first. Every other `blx` in the 418-call
sequence matches exactly, so the source *shape* is right and the residual is
clang register allocation + `ARMConstantIslandPass` block placement.

Confirmed from the original Thumb (free-credits loop, `0x14ad26`):

```
14ad26: mov r0,r4          ; button
14ad28: blx getWidth       ; -> sl
14ad2c: mov r1,sl          ; &Globals::layout
14ad2e: mov sl,r0
14ad30: ldr r0,[r1,#0]     ; layout
14ad32: ldr r9,[r0,#44]    ; layout->field_0x2c_rowHeight
14ad36: mov r0,r4
14ad38: blx getHeight      ; -> r4
```

and the original has **no** `vstr/vldr` around `textYf` (`14ad8c` is
`vmov r6,s0`), so the `volatile float textYf` and `volatile int pV` in our
source are codegen anchors, not faithful source. They are load-bearing for the
fuzzy score (see below).

## Measurements (base = 91.200000 unless noted)

Every row was built with `cmake --build --target link_so` and scored with
`tools/verify`; all rows keep 9700 B / `.text 0x1337c8` / `.ARM.extab 0x26298`
unless a different size is shown.

### Free-credits loop: call order / volatile / branch order

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `textX` before `height` (keep `lay`) | 76.9 | 9700 | 3626:3637 |
| `textX` before `height`, `Globals::layout` inline (no `lay`) | 77.5 | 9700 | 3626:3639 |
| `textX` first, `lay` declared after | 77.5 | 9700 | 3626:3639 |
| `int width = getWidth(); int height = getHeight(); textX = width + ...` (+`lay`) | 76.5 | 9696 | 3626:3636 |
| same, `Globals::layout` inline | 75.7 | 9688 | 3626:3633 |
| drop `volatile` from `textYf` | 76.0 | 9696 | 3626:3636 |
| drop `volatile`, `textX` first (both) | 76.6 | 9700 | 3626:3638 |
| `int textY = (int)((float)y + (float)height*0.2f)` inline | 76.0 | 9696 | 3626:3636 |
| `volatile int textY` | 88.1 | 9696 | 3626:3635 |
| `volatile float textYf` before `SetColor` | 88.4 | 9696 | 3626:3628 |
| `volatile double textYf` | 74.1 | 9704 | 3626:3636 |
| `int textY; volatile float textYf; textY = (int)textYf;` | 91.2 | 9700 | 3626:3635 |
| `i == 1 && options[73]` (all three operands) | 88.5 | 9692 | 3626:3637 |
| only `i == 1 && options[73]` swapped | 90.8 | 9700 | 3626:3638 |
| only `i == 2 && …` / `i == 3 && …` swapped | 91.2 | 9700 | 3626:3635 |
| swap `if (i != 0)` / `else` branch order | 91.0 | 9720 | 3626:3626 |
| swap branch order + drop `volatile` | 76.0 | 9752 | 3626:3633 |
| swap branch order + drop `pV` | 90.8 | 9720 | 3626:3626 |
| swap branch order + `textX` first | 75.7 | 9712 | 3626:3636 |
| `String("(+") + String(formatCredits(...)) + String(")")` | 91.2 | 9700 | 3626:3635 |
| `String("(+") + (formatCredits(...) + String(")"))` | 89.0 | 9692 | 3626:3630 |
| `formatCredits(...) + String(")")` (drop `(+`) | 68.5 | 9664 | 3626:3640 |
| `String("", false)` for the `i == 0` empty string | 91.2 | 9700 | 3626:3635 |
| `String()` (default ctor) for the empty string | 89.0 | 9692 | 3626:3632 |
| named `String empty;` for the empty string | 73.8 | 9704 | 3626:3642 |
| `int height, textX;` declared outside `if (visible)` | 91.2 | 9700 | 3626:3635 |
| `int textY;` declared outside `if (visible)` | 91.2 | 9700 | 3626:3635 |
| `int height, textX, textY;` declared outside | 91.2 | 9700 | 3626:3635 |
| all-faithful free-credits block (textX first, no volatile, `i==0` first) | 76.1 | 9756 | 3626:3637 |
| faithful without branch swap | 76.6 | 9700 | 3626:3638 |
| faithful without `textX` first | 76.0 | 9752 | 3626:3633 |

### `pV` anchor (`viewMode == 1` re-entrant render)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| drop `volatile int pV = 0;` | 91.0 | 9700 | 3626:3635 |
| `volatile int *pV = nullptr;` | 91.0 | 9700 | 3626:3635 |
| `volatile int pV[1] = {0};` | 91.2 | 9700 | 3626:3635 |
| `volatile int pV = this->viewMode;` | 91.2 | 9700 | 3626:3635 |
| `pV` declared after `this->render()` | 91.1 | 9700 | 3626:3635 |
| `(void)pV;` after `render()` | 88.9 | 9704 | 3626:3637 |

### Extra `volatile` anchors in the free-credits loop (all worse)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `volatile int zz = textX;` before `SetColor` | 87.0 | 9708 | 3626:3637 |
| `volatile int zz = baseX;` before the loop | 74.8 | 9740 | 3626:3650 |
| `volatile int zz = baseY;` before the loop | 75.2 | 9748 | 3626:3650 |
| `volatile int zz = row;` before `setVisible` | 76.7 | 9696 | 3626:3632 |
| `volatile int zz = i;` before `setVisible` | 89.0 | 9704 | 3626:3636 |
| `volatile int zz = y;` before `setPosition` | 89.1 | 9704 | 3626:3637 |
| `volatile int zz = textY;` before `DrawString` | 88.9 | 9708 | 3626:3637 |
| `volatile int height`, `volatile int textX`, `volatile int y/row/baseX/baseY` | 65.6–90.8 | varies | varies |

### `left` / `right` block (selected-item and common)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `int left, right;` instead of `int right, left;` (selected block) | 91.2 | 9700 | 3626:3635 |
| separate `int right; int left;` | 91.2 | 9700 | 3626:3635 |
| `int right = 0, left = 0;` | 91.2 | 9700 | 3626:3635 |
| common block `right` declared first | 79.2 | 9648 | 3626:3620 |
| common block as `if/else` (`int left, right;`) | 81.7 | 9652 | 3626:3618 |
| common block as `if/else` (`int right, left;`) | 79.4 | 9644 | 3626:3620 |

### Free-credits `TouchButton *button` local

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| remove the local, index `(*this->buttons)[18+i]` everywhere | 52.3 | 9692 | 3626:3658 |
| keep local only for `getWidth/getHeight` | 66.1 | 9688 | 3626:3654 |
| declare local after `visible` | 76.0 | 9696 | 3626:3636 |

### Combined "clean" variants (drop the wave-114 anchors)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| drop `textYf` volatile + `pV` | 75.8 | 9696 | 3626:3636 |
| drop both + `pad0` | 76.5 | 9696 | 3626:3636 |
| drop both + `pad0` + `pad3` + `padX` | 76.3 | 9696 | 3626:3639 |

## Conclusion

The landed wave-114 source is a strong local optimum at 91.2 under the fuzzy
metric. The residual 226 hunks are clang register-allocation and
constant-island block-placement choices, not recoverable source-shape errors:
the call sequence matches the original 418/418 apart from the one
`getWidth`/`getHeight` swap, which is itself worth −14.3 points because it
reshuffles the whole function's allocation. The `volatile` anchors
(`textYf`, `pV`, `pad0`, `pad3`, `padX`) are load-bearing: removing them costs
~15 points even though the original has no corresponding stores.

No source change measured above 91.199559, so no code change was landed; the
gate counts are unchanged (byte 4500 / linked 4500, lint CLEAN, drift 0,
`.ARM.extab 0x26298`, 27 sections, file size 2,187,328).
