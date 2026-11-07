# `Layout::Layout` C1/C2 — wave 132 / worker w7729
## Outer iPadHD/non-HD split hypothesis: tested, no source win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-132 master `144fc16b0` (7072 B / 48.943802 % unrounded; this
worktree's HEAD scores 49.003190 because the w7684 body is in place).

## Result — no landable source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 49.003189792663 % | **49.003189792663 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. Hypothesis

w7683's reward-box tree (`_work/reconstructions/Layout-LayoutC1-wave131-w7683.md`)
is the faithful body but only scores 20.939061 % because the enclosing
`if (iPadHD) { ...SLP... } else { ...reward tree... }` split is unmodelled.
Add that outer split to the w7683 body and re-test at 7072 B.

## 2. Ground truth: the original tail *is* one device split

Manual Thumb decode of `d21e2..d2548` (function-relative `0x11ba..0x1520`):

```
d21e2  field_0x1e8 / field_0x1e4 (common)
d2202  beq d22b8                 ; if (!iPadHD) -> non-HD arm
       --- HD arm d2204..d22b2 ---
       0x1fc=2  0x1ec=26  0x234=2  0x238=70  0x27c=28  0x280=0
       0x3e0=2  0x3e4=0   0x3e8=140 0x3f4=77 0x3f8=132
       SLP 0x204..0x278 (7 x vst1.32 {d16-d29}, 4 words each)
       if (n9) { 0x3ec=352; 0x3f0=75 } else { 0x3ec=retina?392:196; 0x3f0=retina?100:50 }
d22b8  cmp r9,#0 ; beq d240e     ; r9 = !n9 at this point
       --- non-HD non-n9 arm d22c0..d240c ---
       scalar 0x1fc,0x1ec,0x224,0x228,0x22c,0x230,0x234,0x238,0x24c,0x250
       + SLP / vbsl retina selection for the rest
       b d2498
       --- non-HD n9 arm d240e..d2494 ---
       0x1fc=4 0x1ec=34 0x234=2 0x238=90 0x27c=30 + SLP n9 constants
d2498  common non-HD reward tree:
       0x280 = r2 (from arm), 0x3e4 = n9?1:retina, 0x3e0 = (int)(float)r1
       if (n9) { 0x3e8=112; 0x3f4=57; 0x3f8=111; 0x3ec=352; 0x3f0=75 }
       else { iPadLarge/iPad/retina width+Y; 0x3f8=(Y+17)<<retina;
              0x3ec=retina?392:196; 0x3f0=retina?100:50 }
d2548  end of split (0x3f0 store); common code resumes at d254c
```

So the source shape is

```cpp
if (Globals::iPadHD) {
    /* HD tail fields + SLP + HD reward tail */
} else {
    if (Globals::n9) { /* n9 tail fields + SLP */ }
    else             { /* retina/default tail fields + SLP */ }
    /* common non-HD reward tree */
}
```

This is exactly the "outer split" the hypothesis asks for.

## 3. Variants built and scored (all real `link_so` builds)

| variant | source shape | size | ratio % |
|---|---|---:|---:|
| HEAD | w7684 field-major tail | **7072** | **49.003190** |
| w7683 | w7683 body (tree, no outer split) | 7072 | 20.939061 |
| graft | HEAD + w7683 reward tree only | 7052 | 40.151515 |
| graft+int | graft + `0xf0=112`, `0x1dc=14` | 7056 | 38.003992 |
| A | w7683 body + full outer split (n9/retina arms) | 7008 | 17.800202 |
| B | w7683 body, HD arm + field-major non-HD arm | 6400 | 29.573619 |
| D | HEAD body + full outer split (n9/retina arms) | 7092 | 19.872051 |
| E | HEAD body, tail ascending 0x204..0x27c | 7108 | 19.928543 |
| F | HEAD body, tail in original non-n9 store order | 6944 | 18.540117 |
| G | HEAD body, HD arm + field-major non-HD arm | 6544 | 32.725766 |
| H | HEAD, reward width/Y/height/X reordered | 7040 | 43.200000 |
| I | graft + scrollBarInset/Handle split | 7044 | 39.624526 |

Every outer-split variant is **smaller** than 7072 (6400–7092) and scores
**worse** than the current 49.0 %.  The only size-exact shapes are HEAD and
w7683; both are already 7072 B.

## 4. Why the split does not win

Two independent effects, both measured:

1. **Register allocation flips.**  The original keeps `iPadHD` in `sl`
   (`ldrb.w sl,[r1]` at `d1036`).  HEAD also uses `sl`.  Every device-major
   variant that restructures the tail makes clang put `iPadHD` in `fp`
   (`ldrb.w fp,[r1]`); the `push` and every subsequent register/offset then
   diverge.  This is not recoverable by statement order alone.
2. **The split lets clang shrink the non-HD arm.**  The original non-HD arm is
   ~466 B (two branches + vbsl retina selection).  A literal
   `if (n9) {...} else {...}` transcription compiles to 300–400 B of cmov /
   scalar stores because the current expressions are simpler than the
   original's SLP-vectorised form.  The 112–672 B deficit is real code, not
   pool placement.
3. **The w7683 body's 20.9 % is not caused by the reward tree.**  Its other
   device-major trees (`0x154/0x158/0x15c`, `0x164/0x168`,
   `0x16c/0x170`, `0x174/0x178/0x17c`, `0x19c`, `0x1a0`) are ~1000
   instructions of divergent code.  Grafting only its reward tree onto HEAD
   gives 40.15 %, i.e. the tree alone is *worse* than HEAD's field-major
   reward box; the outer split does not recover the difference.

The current field-major tail is a strong local optimum: the original compiler
itself lowered the field-major source into the observed branch tree, so
writing the tree explicitly changes the schedule in the wrong direction.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7729
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7729/score.py                         # 7072 / 49.003190
```

Scratch harnesses (not committed, `/dev/shm/w7729/`): `score.py` (exact
single-function unrounded scorer built on `tools/verify`), `gen_tail.py`
(variant A), `gen_b.py` (variant B), `Layout.{split,B,D,E,F,G,H,I}.cpp`
(variant sources), `orig.dis` / `ours*.dis`, `show*.txt`.

## 6. Open questions

* The original non-HD arm's `vbsl` retina selection needs the source to be a
  `if (retina) { ... } else { ... }` block over the *whole* 0x204..0x278 run,
  not per-field ternaries.  Variant A does this but pays the `fp` register
  flip and the 64 B deficit; recovering both is the next lead.
* The 64 B deficit in variant A is concentrated in the non-n9 arm
  (`0x22c0..0x240c`, 332 B original).  A faithful `vbsl` transcription of the
  retina arm (two constant vectors + `vbsl`) may restore it.
* `field_0x200` and `field_0x285` live *after* the split in the original
  (`d2608`/`d2620`); any future split transcription must keep them outside.
