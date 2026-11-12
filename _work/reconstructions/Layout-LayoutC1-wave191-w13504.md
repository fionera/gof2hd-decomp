# `Layout::Layout` C1/C2 — wave 191 / worker w13504

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13504`, base master `554e18a35`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,243,
aligned 2,728, size 7,072/7,072, insn 2,521/2,552).**  Tree clean; gate re-run
green (`RATCHET PASS`, byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, imports 207, lint CLEAN).

This note records the fresh lens asked for by the brief — **cold-tail outlining
+ callee-saved assignment** — and the cells it measured that are not already in
`Layout-LayoutC1-wave190-w13420.md`, `...-w13443.md`, `...-w13458.md`,
`...-w13467.md`, `...-w13473.md`, `...-w13490.md` (read those first).

## 1. Cold-region census (re-verified on the wave-191 base)

The function is `0xd1028..0xd2bc8`.  Last hot instruction / epilogue:

| | epilogue | cold blocks after `pop` | end pool |
|---|---|---|---|
| original | `d27fc ldmia.w sp!,{r2,r3,r8,r9,sl,fp}` / `d2800 pop` | `d2802..d2852` = **80 B in 4 blocks** | `d2854..d2bc8` = 884 B (221 words) |
| ours | `d28b2 ldmia.w sp!,{r3,r8,r9,sl,fp}` / `d28b6 pop` | `d28b8..d28d6` = **30 B in 2 blocks** | `d28d8..d2bc8` = 752 B (188 words) |

The original's four cold blocks (all reached by `beq.w` from the non-HD path;
the brief's "5" counts the `d2852 nop` pad / pool boundary):

| block | addr | source arm | join |
|---|---|---|---|
| A | `d2802` (38 B) | `field_0x130/0x134/0x138` final `else` (`iPad ? 56 : 40`, …) | `b.w d1cba` |
| B | `d2828` (14 B) | `t_field_0x2c8` innermost `iPad ? 45 : 30` | `b.w d1814` |
| C | `d2836` (14 B) | `field_0x2d0` innermost `iPad ? 45 : 30` | `b.w d1860` |
| D | `d2844` (14 B) | `field_0x60` innermost `iPad ? 64 : 44` | `b.w d16d6` |

Ours cold blocks:

* `d28b8` (20 B incl. branch): `field_0x18c` innermost `retinaDisplay ? 348 : 174`
  — the original keeps this **inline** (`d1eb0 strd r2,fp,[r0,#396]`).
* `d28ca` (14 B): `field_0x60` innermost `iPad ? 64 : 44` — **matches** the
  original's block D byte-for-byte (14 B, same `ldrb/movs/cmp/it/moveq/b.w`).

So the delta is: original outlines A/B/C (66 B) and inlines 0x18c; ours
outlines 0x18c (20 B) and keeps A/B/C inline.  Net code delta +64 B (ours hot
code is 132 B larger; the pool is 132 B smaller).

## 2. Callee-saved rotation (re-verified, exact)

| | prologue | HD flag | n9 flag | retina ptr | non-HD spill |
|---|---|---|---|---|---|
| original | `push {r4,r5,r6,r7,lr}` + `stmdb sp!,{r6,r7,r8,r9,sl,fp}` (11 words, frame 44) | `sl` (`ldrb.w sl,[r1]` `d1036`) | `fp` (`ldrb.w fp,[r1]` `d1272`) | `r8` (`ldr.w r8,[r2]` `d158c`) | `strd fp,sl,[sp]` `d1570`; `ldrd fp,sl,[sp]` `d16a4/d16d2/d1710` |
| ours | `push {r4,r5,r6,r7,lr}` + `stmdb sp!,{r7,r8,r9,sl,fp}` (10 words, frame 40) | `lr` (`ldrb.w lr,[r1]` `d1036`) | `fp` (never clobbered) | `ip` (`ldr.w ip,[r1]` `d1586`) | `str.w lr,[sp]` `d1568`; `ldr.w lr,[sp]` `d16a4/d16d2/d1710` |

Consequences in the hot path (all 4-byte instructions, size-neutral):

* ours has an extra `mov r5,lr` (`d105e`) + `mov sl,r5` (`d11d8`) pair because
  `sl` is used as scratch in the HD block and `r5` is needed later; the
  original keeps HD in `sl` throughout and needs neither move.
* the original's `ldrd fp,sl,[sp]` (4 B) is the same size as ours'
  `ldr.w lr,[sp]` (4 B), so **the rotation itself is size-neutral**; matching it
  would fix the `d16a4/d16d2/d1710` bytes plus the prologue/`[sp,#4]` offsets.
* the original clobbers `fp` (n9) in the non-HD block (`mov.w fp,#10` `d1592`,
  `moveq fp,#5` `d1654`) and reloads it; ours keeps n9 live and uses other
  registers.  This is the direct cause of the extra frame word.

## 3. Why the rotation is not reachable from source (measured)

All variants below were built with the real link (`link_so`, `-j2`) and scored
with `tools/fn_diff.py`.  Baseline `(raw, aligned, size) = (5243, 2728, 7072)`.

### 3.1 Hoisting the retina pointer

| variant | raw | size |
|---|---:|---:|
| `const unsigned char *rd = &Globals::retinaDisplay;` at ctor top, every `Globals::retinaDisplay` → `*rd` | 5,422 | 7,040 |
| same but `volatile unsigned char *rd` | 6,562 | 7,308 |
| `const bool retinaFlag = Globals::retinaDisplay;` at top, used everywhere | 6,649 | **5,080** |

The non-volatile top pointer still allocates **`lr` = HD / `ip` = retina**
(verified by disassembly: `ldrb.w lr,[r1]` at `d1036`, `ldr.w ip,[r1]` at
`d1586`) — hoisting the pointer does not change its register class.  The
volatile version moves HD to `ip` but costs +236 B.

### 3.2 Hoisting the flags as locals

| flag | raw | size |
|---|---:|---:|
| `const bool hdFlag = Globals::iPadHD;` used everywhere | 5,907 | 6,880 |
| `const bool n9Flag = Globals::n9;` used everywhere | 6,716 | 7,240 |

With `hdFlag` the allocator puts HD in **`r5`** (`ldrb r5,[r1]` `d1036`), not
`sl`, and the body shrinks 192 B.  No hoist produces `sl`.

### 3.3 `__builtin_expect` on the cold-tail conditions

| site | raw | size |
|---|---:|---:|
| `iPad` in `t_field_0x2c8` innermost | 5,243 | 7,072 (codegen-identical) |
| `iPad` in `field_0x2d0` innermost | 5,243 | 7,072 (codegen-identical) |
| `iPad` in `field_0x60` innermost | 5,243 | 7,072 (codegen-identical) |
| `iPadHD` / `n9` in the `0x130` if/else chain | 5,243 | 7,072 (identical) |
| `iPadLarge` in the `0x130` chain | 5,842 | 7,040 |
| `retinaDisplay` in the `0x130` chain (`expect 0` or `1`) | 5,835 / 5,841 | 7,044 / 7,040 |

The innermost `iPad` arms are already treated as unlikely by clang (expect is a
no-op), and forcing the outer `0x130` tests makes the body smaller.

### 3.4 Cold-tail source-shape rewrites

| variant | raw | size |
|---|---:|---:|
| `0x130/0x134/0x138` if/else → three flat ternaries | 5,401 | 7,080 |
| `field_0x18c` chain reordered `iPad` last (`n9 ? 320 : (retina ? 348 : (iPad ? 223 : 174))`) | 5,361 | 7,064 |
| `field_0x18c` chain `retina` first | 5,725 | 6,928 |
| `field_0x18c` chain `retina` before `iPad` | 5,729 | 6,928 |
| `field_0x18c` `iPad`-last + `t_field_0x2d0` local | 5,400 | **7,072** (size-exact, raw worse) |
| `!Globals::iPad ? 30 : 45` (reverse innermost ternary) on 0x2c8/0x2d0/0x60/0x130 | 5,243 | 7,072 (codegen-identical) |
| `!Globals::retinaDisplay ? … : 60` on 0x60 / 0x18c | 5,243 | 7,072 (codegen-identical) |
| `float t_field_0x2c8` + implicit store | 5,774 | 7,096 |
| `volatile int t_field_0x2c8` | 6,257 | 6,896 |

### 3.5 `0x2c8`/`0x2d0`/`0x2d8` statement moves (real builds)

* all 60 permutations of `{temp_2c8, 0x2cc, 0x2d0, 0x2d8, 0x2c8-store}`:
  identity is the unique best size-exact raw (5,243); next best 5,246.
* moving the `0x2c8` store to just before `0x2cc` (original store order) is
  **+16 B** (7,088).
* moving `0x2d8` after the `0x2d4` if/else (original store order, invites
  `strd 0x2d4/0x2d8`) is **−192 B** (6,880) — exactly the "reorder shrinks"
  effect documented by w6932.
* grid of `0x2c8`-store × `0x2d8` placements: best size-exact raw 5,243
  (identity); 5,246/5,249/5,250 are worse.
* `t_field_0x2d0` local (mirror of `t_field_0x2c8`): 5,399 / 7,080.
* `t_field_0x2d0` local + `field_0x1f4` HD arm `22` (int): 5,310 / **7,072**
  (size-exact, raw worse).

### 3.6 Equal-arm boolean simplifications (new cells)

All value-identical for the 32 flag combinations, all measured size-fatal:

| field | rewrite | raw | size |
|---|---|---:|---:|
| `0x124` | `(n9 \|\| retina) ? 2 : 1` | 6,229 | 6,776 |
| `0x128` | `(n9 \|\| retina) ? 2 : 1` | 6,126 | 6,824 |
| `0x110_strip58` | `(n9 \|\| retina) ? 1 : 2` | 6,245 | 6,800 |
| `0x108` | `(iPadHD \|\| retina) ? -2 : (n9 ? -1 : 0)` | 6,079 | 6,852 |
| `0x1bc` | `(n9 \|\| retina) ? 2 : 1` | 6,027 | 6,992 |
| `0x2c0` | `(iPadHD \|\| (!n9 && !retina)) ? -2 : (n9 ? -3 : -4)` | 5,948 | 6,912 |
| `quickMenuIconGap` | `(n9 \|\| retina) ? 2 : 1` | 6,046 | 6,988 |

The original genuinely emits the 4-arm chains for these fields; collapsing the
equal arms shrinks the body below 7,072.

## 4. Conclusion

The cold-tail and callee-saved residue is real and fully characterised above,
but neither is reachable by a size-exact source edit on the wave-191 base:

* the outlining decision is `MachineBlockPlacement`'s cold-block choice; every
  source shape that flips it (`expect`, flat ternaries, chain reorders) also
  changes the body size (7,040–7,096), and the only size-exact combination
  found (`0x18c` iPad-last + `0x2d0` local) is raw-worse (5,400 vs 5,243);
* the callee-saved rotation (`sl`/`lr`, `r8`/`ip`) is an allocator choice that
  no source-level hoist reproduces; hoisting the pointer leaves `lr`/`ip`
  unchanged, and hoisting the flag moves it to `r5` and shrinks 192 B.

A raw win still needs the full structural reconstruction: the original's
non-HD store order with `strd 0x2d4/0x2d8` (≈ −192 B), the four outlined
`iPad` tails (+66 B), the 8 missing SLP `vld1.64` groups (+32 pool words) and
the `0x2c8`/`0x2d0` separate `vcvt` sequences, all landing on exactly 7,072 B.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13504
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5243 aligned=2728 size=7072/7072 insn=2521/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13504/`: `measure.sh` (real
incremental link + fn_diff), `try.py` (patch/build/score), `mv.py`/`multi.py`
(statement moves), `perm.py` (60-perm sweep), `sweep_expect.py` (expect sweep),
`Layout.base.cpp` (baseline source), `orig.dis`/`ours.dis` (objdump).
