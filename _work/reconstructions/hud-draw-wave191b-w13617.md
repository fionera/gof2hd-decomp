# Hud::draw — damage-block anchor sweep (wave 191b, w13617)

Worker `w13617`, branch `wave191b-w13617`, base master `7301219a1`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13617`.

**No code change landed.** `Hud::draw` stays at the HEAD shape (raw 19 /
aligned 6 / 9,332 B / 3,226 insns). Tree left clean; gate re-run green:
`check` **RATCHET PASS** — byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.

| target | symbol | size | raw/aligned | insn |
|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9,332 | 19 / 6 | 3226/3226 |

## 0. The residual, restated

Everything outside `0x164184-0x16419f` is byte-identical. The 28-byte block:

```
orig  164184 f011 0f24  tst.w  r1,#36
      164188 bf1c       itt    ne
      16418a f44f 7096  movne.w r0,#300
      16418e f8ca 04b8  strne.w r0,[sl,#1208]   ; field_0x4b8
      164192 f011 0f18  tst.w  r1,#24
      164196 bf1c       itt    ne
      164198 f44f 7096  movne.w r0,#300
      16419c f8ca 04b4  strne.w r0,[sl,#1204]   ; field_0x4b4

HEAD  164184 f011 0f24  tst.w  r1,#36
      164188 f44f 7096  mov.w  r0,#300
      16418c bf18       it     ne
      16418e f8cd 80d0  strne.w r8,[sp,#208]    ; anchor store
      164192 f8ca 04b8  str.w  r0,[sl,#1208]    ; unconditional
      164196 f011 0f18  tst.w  r1,#24
      16419a bf18       it     ne
      16419c f8ca 04b4  strne.w r0,[sl,#1204]
```

The original's block is *exactly* the plain source
`if (hitFlags & 0x24) this->field_0x4b8 = 300;` (each store carries its own
`movne`).  HEAD's unbraced anchor
`if (hitFlags & 0x24) Status **volatile p = &Globals::status; this->field_0x4b8 = 300;`
moves the `field_0x4b8` store into the join block, so the shared `mov r0,#300`
is hoisted and the anchor's own `strne` occupies the slot of the second
`movne`.  Both are 28 B, so the anchor is a *substitution*, not an addition.

The anchor is also load-bearing for the whole-function allocation: it forces
`&Globals::status` into r8 at `0x164176` (`ldr.w r8,[sp,#108]`), which is the
same reload the original has there — but **dead** in the original (r8 is
clobbered at `0x1641e4`).  Removing the anchor gives the exact shipped block
but drops the frame to 216 and re-colours the entire function (raw 7,340).

So the target is precisely: **plain block + a dead `ldr.w r8,[sp,#108]` at
`0x164176` + the HEAD 224-byte frame layout (first spill store at `sp+112`,
`frontImage` at `sp+96`, String temp at `sp+208`)**.

## 1. New evidence this wave

* The original's `&2` block is byte-identical to HEAD's, *including* the dead
  reload at `0x164176`.  So the residual is confined to the `&0x24` block;
  the `&2` block needs no change.
* A `volatile` local declared in a **nested scope** around the `&0x24` block
  (`{ Status **volatile p = &Globals::status; ... }`) is the only construct
  found that keeps the exact HEAD frame layout (alloca stack-coloured to
  `sp+208`, spills at the original offsets) while emitting the plain block.
  Its store lands at `0x164184+4` (start of the `&0x24` block), i.e. 0x12 B
  after the original's dead load, and costs +4 B.
* A **function-scope** `volatile int` / `Status **volatile` read gives frame
  224 but its slot lands at `sp+216`, which shifts *every* spill offset by 4
  (first store at `sp+108` instead of `sp+112`) — raw ~3,000.
* The original's dead reload is an RA live-range-split artefact of a long
  `&Globals::status` live range (gamma block `0x164022` -> mission panel
  `0x164558`), not a source-level load: the plain build keeps the GOT
  *pointer* in a register (`%647`), HEAD spills the GOT *value* (`%stack.51`).

## 2. Measured this wave (fast single-TU harness, then linked)

`size`/`frame` are from the `Hud.cpp` object; `raw`/`aligned` from
`tools/fn_diff.py` on the linked `.so`.  All variants compile clean.

| variant (replaces the 3-line anchor) | raw | aligned | size | frame |
|---|---:|---:|---:|---:|
| **HEAD** `Status **volatile p = &Globals::status;` unbraced | **19** | **6** | **9332** | **224** |
| plain (`if (hitFlags&0x24) field_0x4b8=300;`) | 7340 | 914 | 9328 | 216 |
| `volread_in_if` — `(void)*(Status *volatile *)&Globals::status;` as the only if body | **25** | **8** | 9332 | 224 |
| `volptr_uninit_store_in_if` — fn-scope `Status **volatile q;` + `q=&Globals::status;` in if | 369 | 363 | 9332 | 224 |
| `vp_read_in_if` — fn-scope `Status **volatile q=&Globals::status;` + `(void)q;` in if | 1238 | 429 | 9332 | 224 |
| `vint_init_P3` — fn-scope `volatile int q=0;` between `&1`/`&2` | 2880 | 875 | 9332 | 224 |
| `vint_init_P9` — fn-scope `volatile int q=0;` after `&0x18` | 2916 | 875 | 9332 | 224 |
| `vint_read_P1/P3/P9` — fn-scope `volatile int q; (void)q;` | 3042/3027/3027 | 872/871/872 | 9332 | 224 |
| `vq_read_P1/P3/P9` — fn-scope `Status **volatile q; (void)q;` | 3042/3027/3027 | 872/871/872 | 9332 | 224 |
| `pf_after18` — `__builtin_prefetch(&Globals::status);` after `&0x18` | 7070 | 407 | 9336 | 224 |
| `p_after18` — nested `{Status **volatile p=&Globals::status;}` after `&0x18` | 7070 | 407 | 9336 | 224 |
| `p_24_18` — same, between `&0x24`/`&0x18` | 7083 | 407 | 9336 | 224 |
| `anchor_neg` — `if(!(h&0x24)) volatile store; else field=300;` | 7090 | 410 | 9336 | 224 |
| `anchor_if_plain_else` — `if(h&0x24) field=300; else volatile store;` | 7092 | 408 | 9336 | 224 |
| `scope_vp_store_only` — nested `{volatile store} if(h&0x24) field=300;` | 7093 | 407 | 9336 | 224 |
| `p_2_24` / `pf_2_24` — same between `&2`/`&0x24` (store at `0x164188`) | 7093 | 407 | 9336 | 224 |
| `vread_val_2_24` — `(void)*(Status *volatile *)&Globals::status;` between `&2`/`&0x24` | 7099 | 409 | 9336 | 224 |
| `B_2scope` / `C_2braced` / `vread_val_1_2` / `pf_1_2` (exact block, layout +4) | 8063 | 838 | 9360 | 224 |
| `vstp_panel` — fn-scope `Status **volatile stp=&Globals::status;`, panel uses `(*stp)` | 7135 | 779 | 9336 | 224 |
| `stp_panel` — fn-scope `Status **stp=&Globals::status;`, panel uses `(*stp)` | 7340 | 914 | 9328 | 216 |

Negative results (no change vs plain, 9328/216): `unsigned/short/char/long/const
hitFlags`; `(hitFlags&0x24)!=0`; braces around the ifs; `?:`/comma/`do{}while(0)`/
`(bool)` forms; `(void)Globals::status;`/`(void)&Globals::status;` dead uses;
`Status **stp=&Globals::status;` (coalesced with the existing spill);
`__builtin_expect` on any subset of the four conditions.

## 3. Why no win

The raw metric normalises nothing, so *any* 4-byte insertion shifts every
following branch displacement and PC-relative literal.  A 4-byte insert placed
anywhere except exactly `0x164176` costs ~7,000 raw bytes.  The only
constructs that reproduce the exact HEAD frame layout (nested-scope volatile
`Status**` store) place their extra 4 bytes at `0x164184+4`; constructs that
put a 4-byte instruction at `0x164176` (volatile reads of `Globals::status`,
`__builtin_prefetch`) either emit two instructions or re-colour the frame.

The closest non-HEAD shape is `volread_in_if` (raw 25 / aligned 8), which is
size- and frame-exact and keeps the layout, but its block is
`tst / it ne / ldrne r0,[r8] / tst / mov / str / it ne / strne` — the
`field_0x4b8` store is still unconditional, so 25 > 19.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13617
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch harness: `scratch/w13617/` — `fast.py` (single-TU size/frame/block
probe, ~0.3 s), `batch.py` / `batch2.py` (multi-variant sweeps, `-v` for the
block), `link.py` / `link2.py` (link + `fn_diff`), `idiff.py` (aligned
instruction diff vs the original), `Hud.cpp.base` (pristine HEAD),
`mir.txt` / `plain_mir.txt` (`-mllvm -print-after-all`, HEAD / plain).
