# Generator::createMission — pick-first `pick == 10` + shared `assignType` latch (w4232)

Status: **landed** (wave 92, 2026-11-03). `createMission` fuzzy score **73.6 -> 74.3**
(`osize 1996  usize 1996`, 727 -> 727 instructions). No gate count moved:
`byte 4498 / linked 4498 / size-exact 4524 / data 843 / imports 207`, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26290 (-8)`, lint CLEAN, drift 0.
The function stays exactly **1996 B**, so no section address shifts.

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

This pass supersedes the "systems/used swap is the blocker" conclusion of
`generator-createmission-nested-pickfirst-w4104.md`: the pick-first `pick == 10` IR **is**
reachable together with `systems -> r8`, `agent -> r9`, frame 96 and `rnd -> [sp,#40]`. The
missing ingredient was the **shared `assignType` latch** (`goto`), which both reproduces the
original's `972d2: mov r2,r6` block and keeps the greedy allocator from spilling `agent`.

## 0. Before / after

```
before (HEAD b379d00d):  pct=73.6  osize=1996  usize=1996  n_orig=727  n_ours=727
after  (this commit):    pct=74.3  osize=1996  usize=1996  n_orig=727  n_ours=727
```

Gate (identical before and after):

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
.text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70  .ARM.extab 0x26290 (-8)
lint CLEAN  drift 0   RATCHET PASS
```

## 1. The landed source shape

Only the mission-type selection loop changed (plus a trailing newline):

```c++
    for (; !selected; --guard) {
        selected = guard < 1;
        short pick = Globals::rnd->nextInt(15);
        type = pick;
        if (pick == 8) goto assignType;
        if (__builtin_expect(pick == 10, 1)) {  // lint: arm_builtin
            type = 10;
            if (race > 3) continue;
        }
        if (used != nullptr) {
            if (used->data()[pick]) {
                int sum = 0;
                uint32_t i;
                for (i = 0; i < used->size(); ++i) {
                    sum += used->data()[i];
                }
                if (sum == 14) {
                    for (uint32_t i = 0; i < used->size(); ++i) {
                        used->data()[i] = false;
                    }
                    goto assignType;
                }
                type = pick;
                continue;
            }
            used->data()[pick] = true;
        }
        if (pick == 12) {
            selected = true;
            if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 25) {
                type = 12;
            } else {
                goto assignType;
            }
            continue;
        }
        selected = true;
    assignType:
        type = pick;
    }
```

Three things matter and were each measured independently (see the sweep tables in §4):

1. **`short pick`, not `int pick`.** This is the single biggest lever. `int pick` changes the
   greedy allocator's ordering and moves `systems` off `r8`/`used` off `sl`/frame to 104; the
   whole 600-instruction tail then loses its slot/register matches. `short pick` keeps
   `systems -> r8`, `agent -> r9`, `rnd -> [sp,#40]`, frame 96.
2. **Nested pick-first `pick == 10`, not `pick == 10 && race > 3`.** The original assigns
   `type = 10` *before* the race test (`movs r2,#10` sits between `ldr race` and `cmp race,#3`),
   so the source must be the nested form. The `&&` form compiles race-first (`cmp race,#4; bcc;
   cmp pick,#10`) and scores 73.6; the nested form without the branch hint scores 60.0.
3. **`goto assignType` shared latch.** The original has one `mov r2,r6` at `972d2` shared by
   `pick == 8`, the `sum == 14` reset path and the `pick != 12` else path. Spelling those as
   `continue` / fall-through moves the latch and re-orders the loop tail; the `goto` form
   reproduces it and is what makes the `pick == 12` select survive non-folded.

### 1.1 The `__builtin_expect(pick == 10, 1)` hint

The binary proves the weight: the original lays the `pick == 10` block out as the **fall-through**
(`9726a: cmp r6,#10; 9726c: bne.n 97276`) and branches away for `pick != 10`, i.e. it treats
`pick == 10` as the likely path. Without the hint clang 7.0.2 at `-Oz` picks the opposite layout
and the loop loses the pick-first order (60.0 %). The hint is therefore reproducing a measured
branch weight, not papering over one. It needs the same-line `// lint: arm_builtin` waiver
(the file already carries one at the `nextInt(5) == 0` cold-path hint).

## 2. Original loop (`0x97248..0x972da`, 146 B)

```
97248: f04f 0b00   mov.w   fp, #0
9724c: f1b9 0f01   cmp.w   r9, #1
97250: bfb8        it      lt
97252: f04f 0b01   movlt.w fp, #1
97256: 980a        ldr     r0, [sp, #40]        ; rnd
97258: 210f        movs    r1, #15
9725a: 6800        ldr     r0, [r0, #0]
9725c: f7ca eaf4   blx     nextInt
97260: f1a9 0901   sub.w   r9, r9, #1           ; --guard
97264: 4606        mov     r6, r0               ; pick
97266: 2808        cmp     r0, #8
97268: d033        beq.n   972d2                ; pick==8 -> shared latch
9726a: 2e0a        cmp     r6, #10
9726c: d103        bne.n   97276                ; pick!=10 -> used check
9726e: 9809        ldr     r0, [sp, #36]        ; race
97270: 220a        movs    r2, #10              ; type = 10  (BEFORE the test)
97272: 2803        cmp     r0, #3
97274: d82e        bhi.n   972d4                ; race>3 -> condition
97276: f1ba 0f00   cmp.w   sl, #0               ; used != nullptr
9727a: d019        beq.n   972b0
9727c: f8da 0004   ldr.w   r0, [sl, #4]
97280: 5d81        ldrb    r1, [r0, r6]
97282: b199        cbz     r1, 972ac
97284: f8da 1000   ldr.w   r1, [sl]
97288: 2200        movs    r2, #0
9728a: 2300        movs    r3, #0
9728c: e002        b.n     97294
9728e: 5c84        ldrb    r4, [r0, r2]
97290: 3201        adds    r2, #1
97292: 4423        add     r3, r4
97294: 4291        cmp     r1, r2
97296: d1fa        bne.n   9728e
97298: 2b0e        cmp     r3, #14
9729a: 4632        mov     r2, r6               ; type = pick (own copy)
9729c: d11a        bne.n   972d4
9729e: e002        b.n     972a6
972a0: f800 5b01   strb.w  r5, [r0], #1
972a4: 3901        subs    r1, #1
972a6: 2900        cmp     r1, #0
972a8: d1fa        bne.n   972a0
972aa: e012        b.n     972d2                ; -> shared latch
972ac: 2101        movs    r1, #1
972ae: 5581        strb    r1, [r0, r6]
972b0: 2e0c        cmp     r6, #12
972b2: d10c        bne.n   972ce
972b4: 9808        ldr     r0, [sp, #32]        ; status
972b6: 6800        ldr     r0, [r0, #0]
972b8: f7ca ebaa   blx     getSystem
972bc: f7ca ebde   blx     getIndex
972c0: 220c        movs    r2, #12
972c2: 2819        cmp     r0, #25
972c4: f04f 0b01   mov.w   fp, #1
972c8: bf18        it      ne
972ca: 4632        movne   r2, r6              ; non-folded select
972cc: e002        b.n     972d4
972ce: f04f 0b01   mov.w   fp, #1              ; else: selected = true
972d2: 4632        mov     r2, r6              ; <<< SHARED LATCH: type = pick
972d4: ea5f 70cb   movs.w  r0, fp, lsl #31
972d8: d0b6        beq.n   97248               ; while (!selected)
```

The three edges into `972d2` are exactly the three `goto assignType` sites in the landed
source (pick==8, sum==14, pick!=12). The `sum != 14` path keeps its own `mov r2,r6` at `9729a`
because it falls straight to `972d4`; the landed source spells that as `type = pick; continue;`
inside the `used->data()[pick]` branch.

## 3. Reconstructed loop (landed, `0x90..0x11a`-equivalent)

```
mov.w   fp, #0
cmp     r5, #1
it      lt
movlt.w fp, #1
ldr     r0, [sp, #40]        ; rnd
movs    r1, #15
ldr     r0, [r0, #0]
blx     nextInt
sxth    r6, r0               ; pick  (short -> sxth)
cmp.w   r9, r0, lsl #16      ; pick == 8   (short compare)
beq.n   loc_11a              ; -> shared latch
lsls    r1, r0, #16
cmp.w   r1, #655360          ; pick == 10  (10 << 16)
bne.n   loc_c6               ; -> used check
ldr     r0, [sp, #36]        ; race
cmp     r0, #3
bls.n   loc_c6               ; race <= 3 -> used check
movs    r6, #10              ; type = 10
b.n     loc_11a              ; -> shared latch
ldr     r3, [sp, #32]        ; used
cbz     r3, loc_f8
ldr     r0, [r3, #4]
ldrb    r2, [r0, r6]
cbz     r2, loc_f4
ldr     r1, [r3, #0]
movs    r2, #0
movs    r3, #0
b.n     loc_de
ldrb    r4, [r0, r2]
adds    r2, #1
add     r3, r4
cmp     r1, r2
bne.n   loc_d8
cmp     r3, #14
beq.n   loc_ee
b.n     loc_11a
strb.w  sl, [r0], #1
subs    r1, #1
cmp     r1, #0
bne.n   loc_e8
b.n     loc_11a
movs    r2, #1
strb    r2, [r0, r6]
cmp.w   r1, #786432          ; pick == 12  (12 << 16)
bne.n   loc_116
ldr     r0, [sp, #28]        ; status
ldr     r0, [r0, #0]
blx     getSystem
blx     getIndex
cmp     r0, #25
mov.w   fp, #1
it      eq
moveq   r6, #12              ; select survives (not folded)
b.n     loc_11a
mov.w   fp, #1               ; else: selected = true
subs    r5, #1               ; --guard
movs.w  r0, fp, lsl #31
beq.n   loc_96               ; while (!selected)
```

Differences that remain in the loop, all `short`-encoding artefacts (the original's `pick` is a
full `int`, so it gets `cmp r0,#8` / `cmp r6,#10` / `cmp r6,#12` and `mov r6,r0`):

* `sxth r6,r0` + `cmp.w r9,r0,lsl#16` instead of `mov r6,r0` + `cmp r0,#8`;
* `lsls r1,r0,#16` + `cmp.w r1,#655360` instead of `cmp r6,#10`;
* `cmp.w r1,#786432` instead of `cmp r6,#12`;
* the select is `cmp r0,#25; it eq; moveq r6,#12` instead of
  `movs r2,#12; cmp r0,#25; it ne; movne r2,r6`.

Everything else — the `pick == 8` first test, the `type = 10` before the race test, the
`used`/`sum`/memset block, the `pick == 12` select, the bottom-of-loop condition and the shared
`type = pick` latch — is in the original order. `systems -> r8`, `agent -> r9`,
`rnd -> [sp,#40]`, frame 96, `difficulty -> r6`, `.text` unchanged.

## 4. Sweep evidence (official verifier, autojunk=False)

`gen2.py`/`gen3.py` build every variant from the HEAD source by textual substitution; each row is
`evalsrc.sh` (compile one TU, gold-link, official `verify.py` score). Feature columns are from
`sig2.py` (one-file disassembly scan).

Key rows (full sweeps: 400 random combinations over `{picktype, loop, p10 form, expects, gotos,
amount/item init, galaxy position, decl order}`):

| variant | pct | usize | features |
|---|---:|---:|---|
| HEAD (`short pick`, `&& race>3`, ternary select) | 73.6 | 1996 | sys_r8 agent_r9 frame96 rnd_sp40 |
| `int pick` (all else HEAD) | 48.9 | 1960 | (systems -> fp) |
| cand1: `int pick` + nested + p10/p12 expect + gotos | 71.1 | 1996 | sys_r8 agent_r9 used_sl frame104 pick10_first sel |
| **landed: `short pick` + `for` + nested p10 + p10 expect + gotos** | **74.3** | **1996** | sys_r8 agent_r9 frame96 rnd_sp40 cond_bottom diff_r6 |
| same, no `__builtin_expect` (`pick==10 && race<=3` / `else if pick==10`) | 74.1 | 1996 | sys_r8 agent_r9 frame96 rnd_sp40 |
| nested p10 without expect | 60.0 | 1988 | sys_r8 agent_r9 frame96 rnd_sp40 |
| landed + `p12expect` | 73.3 | 1996 | sys_r8 agent_r9 frame96 rnd_sp40 |
| landed + `amount` uninitialised | 72.6 | 1992 | sys_r8 agent_r9 frame96 rnd_sp40 |
| landed + `itemId` uninitialised | 66.4 | 1984 | sys_r8 agent_r9 frame104 |
| landed + `galaxy` before clamp | 72.3 | 2004 | sys_r8 agent_r9 frame96 rnd_sp40 |
| landed + `while` instead of `for` | 65.2 | 2004 | agent_r9 frame96 rnd_sp40 |
| `int pick` + same structure | 69.0 | 1996 | sys_r8 agent_r9 used_sl frame104 pick10_first sel |

Single-`__builtin_expect` sweep on cand1 (18 conditions): only the `(type|4)==12` hint changes the
frame (104 -> 96) but it drops `systems -> r8` and scores 65.7. Type/declaration sweep
(`difficulty`/`level`/`type`/`guard`/`amount`/`itemId` as short/int/uint, `used` as `if (used)`,
reversed comparisons, `bonus = 0`, `costs` uint): every one is <= 74.3, and every `int pick`
shape stays at frame 104.

## 5. Why `int pick` cannot reach frame 96 (the w4104 blocker, explained)

With `int pick` the allocator ends up with **eight** local stack slots instead of the original's
seven: `difficulty` gets its own slot and `agent` is reloaded from the stack (`ldr r5,[sp,#36]`)
for the bonus block instead of staying in `r9`. The chain is:

1. `int pick` + the pick-first loop makes the tail keep `galaxy` in `r6` (never spilled), so
   `difficulty` cannot live in `r6` and moves to `r5`;
2. `r5` then holds `difficulty` across the bonus block, so `agent` is reloaded into `r5` and
   `bonus` is computed in `r9` — the two registers the original uses for `agent` and `bonus`
   respectively are swapped;
3. because `agent`'s slot is live through the bonus block, it cannot merge with `difficulty`'s
   slot, and the frame grows by 8 (104).

`short pick` avoids the whole chain: `difficulty` stays in `r6`, `galaxy` is spilled, `agent`
stays in `r9`, and the seven original slots reappear. The cost is the four `short`-encoding
differences listed in §3 — which is still a net win because the tail is ~600 instructions and
the loop only ~60.

## 6. What remains

* `used` is still in `r3` (spilled to `[sp,#32]`) instead of the original's `sl`; `sl` is the
  memset zero. Slot order is therefore `16=&guard, 20=targetStation, 24=agent, 28=status,
  32=used, 36=race, 40=rnd` vs the original's `16=galaxy/bonus, 20=&guard, 24=agent/difficulty,
  28=targetStation/amount, 32=status, 36=race/type, 40=rnd`.
* The `type == 8` block still sinks the `amount` store to the join instead of storing it inside
  the block (`str r0,[sp,#28]` before `mov r6,r1`), and `difficulty` lands in `r5` not `r6`.
* The `type == 13 / getRoutes()==nullptr` block is placed inline before `agent->getName()`
  instead of out-of-line at `97586`, and the `getRoutes()==nullptr` test is `cbz` not
  `cmp/beq.w`.
* The `pick == 12` select is `it eq / moveq r6,#12` rather than the original's
  `movs r2,#12; it ne; movne r2,r6` (a direct consequence of `short pick`).
* The `0.5000001f` costs anchor is still present. The original really is
  `vmov.f32 s2,#0.5` (`0x3f000000`), so the anchor is only holding the 4 bytes that the folded
  `pick == 12` select lost. Removing it requires the full-`int` select, i.e. the frame-104
  family.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4232
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4498
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --fail-on-wrong-type --top 0
# createMission row: osize 1996  usize 1996  fuzzy 74.3
```

Scratch harness (not committed, `/dev/shm/w4232`): `gen2.py`/`gen3.py` (source-variant
generator), `evalsrc.sh` (one-file build + official score), `sig2.py` (feature scan),
`bigsweep.py`/`finalsweep.py`/`p10sweep.py`/`misc2.py` (the sweeps tabulated in §4).
