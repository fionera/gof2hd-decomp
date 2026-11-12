# MGame::OnTouchEnd — wave 189 (w13349): 96/127 split + compensation

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13349`, branch
`wave189-w13349`, base master `f258b29c0`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13349`.

## Result

**No size-exact raw win.** `src/` is unchanged (base). The split is real and can be
made size-exact in several ways, but every one raises the raw same-address diff above
2,928. The note records the exact +8 mechanism, the size-exact placements, and the
compensation candidates so the next pass does not repeat them.

| metric | base |
|---|---:|
| raw same-address diff bytes | 2,928 |
| aligned (instruction) diff | 1,670 |
| function size | 8,080 / 8,080 |
| insn count | 2,861 / 2,865 |
| `.text` differing bytes | 73,953 |
| `.ARM.extab` size / diff | 156,312 / 615 |
| `.ARM.exidx` diff | 0 |
| `.rel.plt` diff | 0 |
| build-id diff | 20 |
| total differing bytes | 74,588 |
| file size | 2,187,328 exact |

Gate after restoring base (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 headers.

## 1. The +8 is exactly the duplicated `isCampaignMission()` call

The merged source `(getCurrentCampaignMission() == 96 || ... == 127)` is *not* two
`getCurrentCampaignMission()` calls plus one `isCampaignMission()` call by accident:
the original also calls `getCurrentCampaignMission()` twice (once per id).  The only
difference is the campaign predicate.

Base (merged), `0x17b7f4`:

```
17b7f4  mov r0,r5
17b7f6  blx isCampaignMission        <- ONE shared call
17b7fa  cmp r0,#0
17b7fc  beq.w 17b99a
17b800  ldr r0,[r8]
17b804  blx getCurrentCampaignMission
17b808  cmp r0,#96
17b80a  beq.n 17b81a
17b80c  ldr r0,[r8]
17b810  blx getCurrentCampaignMission   <- second id call
17b814  cmp r0,#127
17b816  bne.w 17b99a
```

Split, `0x17b7f4` (96 test) and `0x17bdf6` (127 test):

```
17b7f4  mov r0,r5
17b7f6  blx isCampaignMission
17b7fa  cmp r0,#0
17b7fc  beq.w 17b99a
...
17bdf6  mov r0,r5
17bdf8  blx isCampaignMission        <- duplicated 8 B
17bdfc  cmp r0,#0
17bdfe  cbz.n 17be0c
17be02  ldr r0,[r8]
17be06  blx getCurrentCampaignMission
17be0a  cmp r0,#127
17be0c  beq.w 17b80e              <- tail-merged into the 96 body
```

`mov r0,r5` (2) + `blx isCampaignMission` (4) + `cmp r0,#0` (2) = **8 B**; the
predicate branch itself is shared with the block's `beq`/`cbz` successor.  The 127
test's `beq.w 17b80e` confirms the compiler tail-merges the duplicated body exactly
like the original's `beq.w 17b722`; the body is not duplicated.

Because the base is exactly 8,080 with the merged test, the split makes the *body*
8 B larger than the original's body.  A compensation must therefore be a genuine
8-byte body-fidelity fix, not a layout trick.

## 2. Size-exact splits found (all raw regressions)

Split after 126 (the original source position; duplicated body, or the equivalent
`goto`-label form — both measure identically):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| split after 126 | 3,889 | 1,658 | 8,088 |
| split after 126 + 96/127 getter reorder to HP,Shield,Armor,Gamma | 3,887 | 1,658 | 8,088 |
| split after 126 + `count > 0` | 3,891 | 1,659 | 8,088 |
| split after 126 + `count != 0` | 6,536 | 1,981 | 8,084 |
| split after 126 + block-100 `setmod` after `depart` | 3,892 | 1,673 | 8,084 |
| **split after 126 + block-100 `setmod` after `depart` + `count != 0`** | **6,243** | **1,968** | **8,080** |
| split after 126 + full original block-100 order | 3,839 | 1,522 | 8,060 |
| split after 126 + original chain order | 4,804 | 1,507 | 8,104 |
| split after 126 + original chain order + original block-100 order | 4,782 | 1,504 | 8,072 |

The only size-exact after-126 split found is `setmod_after_depart + count != 0`
(8,080).  It is a real fidelity fix on both counts (the original 100 block has
`SetCurrentApplicationModule(5)` after `departStation`, and the original emits
`cmp.w r9,#0; beq.n` for the ore loop), but `count != 0` flips the global register
allocation (`sl`/`r9` swap in the prologue) and the raw explodes to 6,243.  Full
section diff of that variant: `.text` 73,953 -> 77,268 (+3,315), `.ARM.extab`
615 -> 629 (+14), `.rel.plt` still byte-identical, size 8,080.

### `count` condition forms (all on the base, no split)

`count != 0`, `amount != 0`, `if (count)`, `if (amount)`, `if (0 != count)`,
`if (count == 0) continue;`, `if (!count) {} else {...}`, `if (count == 0) goto ...`
all emit the original `beq.n` but flip `sl`/`r9` and grow to 8,088 (raw 6,591).
`__builtin_expect(count != 0, 0)` is 8,084/7,298; `__builtin_expect(count != 0, 1)`
is 8,064/7,459.  `amount < 0` is size-exact 8,080 and keeps the original prologue,
but emits `bge.n` and is raw 2,930 (+2).  `count > 0` is size-exact 8,080, raw 2,930.

### `__builtin_expect` on the chain

Adding `__builtin_expect(mission->isCampaignMission(), 1)` to the 127 test is
8,092/3,882; to the 96 test is 8,096/3,889; to 126 is 8,096/3,898; to 134 is
8,100/3,909.  On the original-order split, expect on 96 is 8,064 and on 127 is
8,072 (both still far from 8,080).  No expect placement is size-exact on the
after-126 split.

## 3. Alternative 127-block placements are size-exact with no compensation

The compiler preserves the source order of the campaign chain, so the 127 block's
source position is its emitted position.  Moving it away from after-126 changes the
layout but can be size-exact by itself:

| 127 source position | raw | aligned | size |
|---|---:|---:|---:|
| after 42 (pos 5) | 3,778 | 1,637 | **8,080** |
| after 22 (pos 6) | 3,778 | 1,637 | **8,080** |
| after 65 (pos 7) | 3,578 | 1,659 | **8,080** |
| before 96 (pos 8) | 3,929 | 1,683 | 8,084 |
| after 96 (pos 9) | 3,929 | 1,683 | 8,084 |
| after 126 (pos 16, original) | 3,889 | 1,658 | 8,088 |

Full section diff of the pos-7 split: `.text` 73,953 -> 74,603 (+650),
`.ARM.extab` 615 -> **612 (-3)**, `.rel.plt` byte-identical, size 8,080.
The extab improvement is real but nowhere near the raw cost.

## 4. Chain-order re-tune with the split present

The base chain order is tuned for the merged form.  A single-block-move hill climb
(19 blocks x 19 positions, two rounds) from the pos-7 split found:

```
[15,162,43,74,42,22,65,127,120,96,100,95,110,81,134,144,155,126,161]
raw 3,375 / aligned 1,413 / size 8,080
```

`.text` 74,400 (+447), `.ARM.extab` 642 (+27), `.rel.plt` byte-identical.  The
second round only improved 3,384 -> 3,375, so the split family's raw floor is
~3,375, i.e. ~+450 over the merged base.  The extra block and its layout shift
cannot be recovered by reordering.

## 5. Original chain order (context)

For reference, reordering the source chain to the original emitted order
`15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,155,161,162`:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| original order, no split | 4,273 | 1,307 | 8,084 |
| original order, no split + block-100 `setmod` after `depart` | 3,930 | 1,298 | **8,080** |
| original order + split | 4,804 | 1,507 | 8,104 |
| original order + split + original block-100 order | 4,782 | 1,504 | 8,072 |

The original order is a large aligned win (1,670 -> 1,298) but is raw-hostile
because the per-block body sizes differ from the original, so the block addresses
drift.  That is the full-body/RA work, not this pass.

## Conclusion

The 96/127 split is structurally correct and can be made size-exact, but the merged
form's 8-byte saving is exactly cancelled by an 8-byte body excess elsewhere.  The
only size-exact after-126 compensation found (`setmod_after_depart + count != 0`)
is a genuine fidelity fix yet is raw 6,243 because `count != 0` flips the global
`sl`/`r9` allocation.  Every size-exact split placement is raw >= 3,375.  `src/` is
left at the base; the next MGame pass should first remove the 8-byte body excess
(the pre-chain `count`/String schedule and the block-100 tail merge are the known
candidates) so the split becomes a raw win.
