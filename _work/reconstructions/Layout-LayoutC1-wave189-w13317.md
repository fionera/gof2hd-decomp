# `Layout::Layout` C1/C2 — wave 189 / worker w13317

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave189-w13317`, base master `b1455e159` (wave-189; raw **5,257**,
whole-`.text` diff **74,032**, `.ARM.extab` diff **639**).

**Result: one size-exact raw win.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,257** | **5,256** (-1) |
| aligned (difflib) | 2,728 | 2,728 |
| size | **7,072/7,072** | **7,072/7,072** |
| instructions | 2,522/2,552 | 2,522/2,552 |
| whole-`.text` diff | 74,032 | **74,031** |
| `.ARM.extab` diff | 639 | **639** (size/base exact `0x1a3b78`, 156,312 B) |

## Landed change

A 6-window permutation of the `field_0x108..0x12c` block:

```
field_0x118
field_0x128          ->  field_0x124
field_0x10c_rowBaseY     field_0x10c_rowBaseY
field_0x12c              field_0x12c
field_0x114_strip5c      field_0x128
field_0x11c              field_0x114_strip5c
field_0x124              field_0x11c
```

i.e. window `[0x128, 0x10c, 0x12c, 0x114, 0x11c, 0x124]` becomes
`[0x124, 0x10c, 0x12c, 0x128, 0x114, 0x11c]`.  The compile-only proxy
predicted 5,191 -> 5,190 (constant +66 offset from the linked body); the
linked raw moved 5,257 -> 5,256 one-for-one.  Instruction count, size,
`.rel.plt`, `.ARM.extab` content and all section bases are unchanged.

## Register / prologue study (why the body is a deep local optimum)

Both prologues are 8 bytes (`push {r4,r5,r6,r7,lr}; add r7,sp,#12;
stmdb sp!, {...}`); the only difference is the second register list:
original `{r6,r7,r8,r9,sl,fp}` vs ours `{r7,r8,r9,sl,fp}`.  The epilogues
are both `pop {r4,r5,r6,r7,pc}`.  Original first-block store schedule is
ascending field order (`0x14,0x18,0x1c,0x20,0x4,0x8,0xc,0x10,0x24..0x70,
0x2c8..0x2d4,0x7c..0x88`); ours groups `0x38..0x44` before `0x24..0x30`
etc.  The original is more `strd`/`vst1` heavy (62/81 vs 39/71) and uses
more `movs` (778 vs 714).  A full prologue register map was extracted
(`scratch/w13317/stores3.py`, `seq.py`): the first divergence is at byte
`0x24` where the original materialises `r6 = 161` (`0x74` HD value) and
`r5 = 369` (`0x78`) while ours materialises `r6 = 369` first.

## Searches re-run on the current (wave-188) body

All with the w13212 batched proxy (`scratch/w13317/batch_score.py`,
`scoreany.py`; proxy = linked - 66):

* exhaustive 2-opt (20,103 valid swaps): best **5,191** = base, no win.
* exhaustive K=4 (4,551 valid window permutations): best **5,191** = base.
* exhaustive K=5 (23,260 valid window permutations): best **5,191** = base.
* 3-opt distance <= 10 (43,015 permutations): best **5,191** = base.
* exhaustive single-unit moves distance 1..203 (40,803 valid moves):
  best **5,191** = base (most far moves are codegen-neutral).
* declaration-order sweep (move `t_field_0x2c8`, `rd`, `v` anywhere before
  first use; 117 variants): all size-exact variants **5,191**.
* one-use alias sweep (`auto tmp = RHS; this->field = tmp;` for all 189
  simple assignments): all **5,191**.
* conversion-form sweep (`(int)(float)(...)` add/remove/round-trip on all
  float-typed RHS; 223 variants): all size-exact variants **5,191**.
* ascending-field-offset partial sorts of the early block: size changes
  (-32 .. +24 B) and raw rises (5,351 .. 5,934), so none can land.

The K=6 window containing units 84..89 is the only improving permutation
found; it is the change landed above.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13317
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5256 aligned=2728 size=7072/7072 insn=2522/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13317/`
(`units.py`, `batch_score.py`, `scoreany.py`, `sweep.py`, `stores3.py`,
`seq.py`, `target_order.py`, `tryorder.py`, `partial.py`, `declmove.py`,
`alias.py`, `convsweep.py`, `inttype.py`, `variants.py`).
