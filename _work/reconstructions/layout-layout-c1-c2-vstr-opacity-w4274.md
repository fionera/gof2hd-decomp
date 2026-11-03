# Layout::Layout (`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`) — vstr opacity, the 0x150/0x1e4 float-select forms, and the literal-pool oracle

Worker **w4274** (wave 92), branch `wave92-w4274`, base `b379d00d`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at `0xd1028`, original size **7072 B**.
Baseline match: **47.655 %** (verifier `score` rounds to 47.7), `usize` 7072.
No code change landed (see §7); this note is the deliverable.

All addresses are **original-image VMAs** (image base `0x10000`). The function is
`0xd1028 .. 0xd2bc8`; its literal pools live inside that span (`.text` is
`0x69440 .. 0x1cb808`, so the pools are *not* `.rodata`).

Read alongside `layout-layout-c1-c2-hd-block-order-w4221.md` (w4221),
`layout-layout-c1-c2-hd-vector-w4101.md`, `layout-layout-c1-c2-sweeps-w4001b.md`,
`layout-layout-c1-c2-source-shape-w3905.md`, `layout-layout-c1-c2-midtail-vstr-w4001.md`.

---

## 1. Correction to the w4221 handoff: the current `vstr` sets

The w4221 note / handoff lists **7 missing** `vstr`s including `0x1e4`. The
current tree already emits a `vstr` at `0x1e4`, so the real symmetric difference
is **6 missing / 6 extra** (both sides 27 `vstr`s):

| set | offsets (decimal byte offsets) |
|---|---|
| original (27) | 96 `0x60`, 104 `0x68`, 108 `0x6c`, 112 `0x70`, 140 `0x8c`, 144 `0x90`, **156 `0x9c`**, 168 `0xa8`, 220 `0xdc`, 224 `0xe0`, 228 `0xe4`, 232 `0xe8`, 312 `0x138`, **336 `0x150`**, **344 `0x158`**, **348 `0x15c`**, 352 `0x160`, 360 `0x168`, **368 `0x170`**, 424 `0x1a8`, 480 `0x1e0`, 484 `0x1e4`, 712 `0x2c8`, 720 `0x2d0`, 760 `0x2f8`, 992 `0x3e0`, **1016 `0x3f8`** |
| ours (27) | **32 `0x20`**, 96, 104, 108, 112, 140, 144, 168, 220, 224, 228, 232, **292 `0x124`**, 312, **328 `0x148`**, 352, 360, 424, 480, 484, **516 `0x204`**, 712, 720, 760, 992, **1000 `0x3e8`**, **500 `0x1f4`** |

**6 missing**: `0x9c 0x150 0x158 0x15c 0x170 0x3f8`.
**6 extra**: `0x20 0x124 0x148 0x1f4 0x204 0x3e8`.
(`0x1e4` is present on both sides; it is *not* missing.)

The original's `0x150` is stored **once per path**: inside the HD block as part of
the `vst1.32 {d18-d19}` group at `d11fe` (base `add.w r1,r0,#336` at `d11fa`), and
in the default block as `vstr s0,[r0,#336]` at `d1d32`. That is the normal
one-store-per-path shape of a single source assignment, not a double store.

---

## 2. Ground truth: the exact codegen at every missing `vstr`

### 2.1 `0x9c` — float select, conversion shared across the non-HD branches

```
d1960: beq.n d1972          ; if (!n9) -> retina/default
d1962: movs  r3, #15
d1964: vldr  s0, [pc,#-104] ; s0 = 75.0f  (literal at d1900 = 0x42960000)
d1968: movw  r1, #630
d196c: str.w r3, [r0,#148]  ; 0x94 = 15
d1970: b.n   d199a
d1972: mov.w r1, #840
d1976: addw  r3, pc,#3812   ; r3 = &{100.0f, 50.0f} at d285c
d197a: cmp.w r8, #0         ; retina
d197e: it eq
d1980: moveq.w r1, #420
d1984: it eq
d1986: addeq r3, #4
d1988: movs  r6, #20
d198a: vldr  s0, [r3]       ; s0 = retina ? 100.0f : 50.0f
d198e: cmp.w r8, #0
d1992: it eq
d1994: moveq r6, #10
d1996: str.w r6, [r0,#148]  ; 0x94
d199a: str.w r1, [r0,#152]  ; 0x98
d199e: vcvt.s32.f32 s0, s0
d19a8: vstr  s0, [r0,#156]  ; 0x9c
```

So the source is a **single nested conditional of `float` constants** whose
`(int)` conversion is shared by the n9/retina/default branches, while the HD
branch's conversion is constant-folded to the plain int `70` (the HD block stores
`0x9c` inside `stmia.w r4,{r5,r8,ip,lr}` at `d113e`, `ip = 70`). Values:
HD 70, n9 75, retina 100, default 50.

### 2.2 `0x150` — float select (`8.0f` / `4.0f`), conversion in the default block

```
d1cd4: vmov.f32 s4, #16     ; s4 = 4.0
d1cec: vmov.f32 s0, #32     ; s0 = 8.0
d1d1e: it eq
d1d1e: vmoveq.f32 s0, s4    ; if !retina s0 = 4.0
d1d2a: vcvt.s32.f32 s0, s0
d1d32: vstr  s0, [r0,#336]  ; 0x150
```

The HD and n9 paths store `0x150` as an int (HD vector group `d11fe`; n9 vector
group), so their conversions are constant-folded. Values: HD 5, n9 6,
retina 8, default 4.

### 2.3 `0x158` — *int* round-trip (`vmov s0,r4`), conversion in the default block

```
d1d9c: movs  r1, #14
d1d9e: movs  r4, #13
d1da0: str.w r1, [r0,#340]  ; 0x154 = 14
d1da4: movs  r1, #0
d1da6: vmov  s0, r4         ; s0 = int value (13/14/28/30/50/100)
d1daa: movs  r4, #136
d1dae: cmp   r2, #0
d1db0: vcvt.f32.s32 s0, s0
d1db4: vcvt.s32.f32 s0, s0
d1db8: vstr  s0, [r0,#344]  ; 0x158
```

The value is an **int** select; the source contains an `(int)(float)` round-trip.
Values: HD 70, n9 13, iPadLarge 100, iPad 50, retina 28, default 14. The HD block
stores `0x158` as int in the `0x150` vector group.

### 2.4 `0x15c` — int round-trip, same shape

```
d1dd8: vmov  s0, r4
d1ddc: movs  r2, #13
d1dde: vcvt.f32.s32 s0, s0
d1de2: vcvt.s32.f32 s0, s0
d1de6: vstr  s0, [r0,#348]  ; 0x15c
```

`r4` is selected as `147*1.40625f`(HD)/294/147/136/158/79. The HD value is
`147 * 1.40625f = 206.71875 -> 206`.

### 2.5 `0x170` — int round-trip

```
d1efc: cbz   r5, d1f1a
d1efe: movs  r1, #138
d1f00: cmp.w r8, #0
d1f04: it eq
d1f06: moveq r1, #69
d1f0c: str.w r1, [r0,#364]  ; 0x16c
d1f10: mov.w r1, #128
d1f14: it eq
d1f16: moveq r1, #64
d1f18: b.n   d1f24
d1f1a: movs  r1, #115
d1f1e: str.w r1, [r0,#364]  ; 0x16c
d1f22: movs  r1, #106
d1f24: vmov  s0, r1
d1f2a: cmp   r4, #0
d1f2c: vcvt.f32.s32 s0, s0
d1f30: vcvt.s32.f32 s0, s0
d1f34: vstr  s0, [r0,#368]  ; 0x170
```

Values: HD 174, n9 106, iPadLarge 248, iPad 124, retina 128, default 64.

### 2.6 `0x1e4` — float select in *both* paths (already present in our build)

`0x1e4` is the one field in the w4221 "missing" list that our tree already
matches. The original:

```
d1254: vldr  s0, [pc,#640]   ; s0 = 5.625 = 4 * 1.40625f   (HD path)
d1268: b.w   d21e2
...
d20e8: vmov.f32 s0, #32      ; s0 = 8.0   (retina)
d20fa: vmov.f32 s4, #16      ; s4 = 4.0
d211e: it eq
d2120: vmoveq.f32 s0, s4     ; if !retina s0 = 4.0
d215c: b.n   d21e2
...
d21e2: vcvt.s32.f32 s0, s0
d21fe: vstr  s0, [r0,#484]   ; 0x1e4
```

So the source is `Globals::iPadHD ? 4 * 1.40625f : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4))`
(a mixed int/float conditional), and the conversion is shared by the HD and
non-HD paths. Our build emits the same `vstr` (at `d2252`) from a `phi i32` of
`{5,6,8,4}`; the address and surrounding schedule differ, but the conversion is
present. The variant `0x1e4 = ? 4 * 1.40625f : (? 6 : (? 8 : 4))` reproduces the
original's *shape* (7024 B / 46.139 %), which is 48 bytes short.

### 2.7 `0x3f8` — round-trip on an opaque member load (proven in w4221)

```
d2514: add.w r2, r1, #17
d2518: lsl.w r2, r2, r8     ; (rewardBoxY + 17) << retinaDisplay
d251c: vmov  s0, r2
d2520: vcvt.f32.s32 s0, s0
d2524: vcvt.s32.f32 s0, s0
d2528: str.w r1, [r0,#1012] ; 0x3f4 = rewardBoxY
d252c: vstr  s0, [r0,#1016] ; 0x3f8
```

This is the only missing `vstr` whose source value is genuinely opaque
(`this->rewardBoxY` is a member load), and it is the one w4221 proved: the
faithful source is
`this->rewardBoxY2 = (int)(float)((this->rewardBoxY + 17) << Globals::retinaDisplay)`
for the non-HD/non-n9/non-Large/non-iPad/non-retina branch. The restructure alone
is **7024 B / 48.0 %** (48 bytes short).

---

## 3. The 6 extra `vstr`s

For these 6 fields our source uses an `N * 1.40625f` HD branch, which makes the
conditional `float`-typed and leaves a conversion in the non-HD path; the
original's HD branch is a plain `int`, so its non-HD path is a plain `str`.

| field | our HD expr | HD value (emulator) | original HD branch |
|---|---|---|---|
| `0x20`  | `5 * 1.40625f`   | 7   | `7`   |
| `0x124` | `1 * 1.40625f`   | 1   | `1`   |
| `0x148` | `2 * 1.40625f`   | 2   | `2`   |
| `0x1f4` | `16 * 1.40625f`  | 22  | `22`  |
| `0x204` | `9 * 1.40625f`   | 12  | `12`  |
| `0x3e8` | `100 * 1.40625f` | 140 | `140` |

Example, original `0x20` in the default block:

```
d15d0: mov.w ip, #10
d15d4: it eq
d15d6: moveq.w ip, #5       ; ip = retina ? 10 : 5
d15da: strd r1, r4, [r0,#4]
d15e2: stmia r1!, {r2,r5,r6}
d15ea: strd r3, lr, [r0,#24]
d15ee: strd ip, ip, [r0,#32] ; 0x20 = 0x24 = ip  (plain int)
d15f2: str.w ip, [r0,#40]    ; 0x28 = ip
```

The original CSEs `0x20`, `0x24`, `0x28` into one int value and stores them with
`strd`/`str`. Our source's `5 * 1.40625f` HD branches prevent this: the compiler
CSEs the value but applies the float round-trip to `0x20` only, emitting
`vstr s0,[r0,#32]` at `d160c`.

---

## 4. clang 7.0.2 fold rules (measured with the exact project flags)

`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -Oz`.

| source shape | codegen |
|---|---|
| `io = (int)(float)128;` | folds to `movs r1,#128; str` |
| `int v = 128; io = (int)(float)v;` | folds |
| `float v = 128.0f; io = (int)v;` | folds |
| `io = (int)(cond ? 100.0f : 50.0f);` | folds to an int select |
| `int v; if(...) v=...; io=(int)(float)v;` | folds (phi of constants) |
| `float v; if/else chain; io=(int)v;` | folds (phi of float constants) |
| `io = (int)(float)(cond ? 70 : 75);` | folds |
| `float v = cond ? 100.0f : 50.0f; fo = v;` (float field) | **keeps** `vldr s0,[pool]; vstr` |
| `(int)(float)(member load)` | **keeps** `vmov/vcvt.f32.s32/vcvt.s32.f32/vstr` |
| `(int)(float)((member + k) << var)` | **keeps** the round-trip |

So in this TU the front end folds *any* `(int)(float)` of a constant select or
phi. The original keeps the conversion for `0x9c`, `0x150`, `0x158`, `0x15c`,
`0x170` and `0x3f8`; the only source shapes that survive are (a) a genuinely
opaque value (member load / shifted member, as in `0x3f8`), or (b) the specific
mixed int/float conditional forms below, which the front end happens to leave
alone in this TU.

---

## 5. Measured variants (official verifier, `autojunk=False`)

Baseline: **7072 B / 47.655 %**. All numbers are `usize / pct`; `vstr` marks
whether the target offset gained a `vstr`.

### 5.1 Adding the missing `vstr`s

| variant | result | target `vstr`? |
|---|---|---|
| `0x150` float ternary (`? 5.0f : (? 6.0f : (? 8.0f : 4.0f))`) | **7080 / 47.377** | yes (`0x150`) |
| `0x150` mixed (`? 5 : (? 6 : (? 8.0f : 4.0f))`) | 7080 / 47.4 | yes (`0x150`) |
| `0x150` `(int)(float)(int ternary)` | **7080 / 47.685** | yes (`0x150`) |
| `0x1e4` `? 4 * 1.40625f : (? 6 : (? 8 : 4))` | **7024 / 46.139** | yes (`0x1e4`) |
| `0x1e4` `? 5.0f : (? 6 : (? 8 : 4))` | 7024 / 45.5 | yes (`0x1e4`) |
| `0x9c` if/else → float ternary | 7064 / 47.564 | no |
| `0x9c` `(int)(float)(int ternary)` | 7064 / 47.564 | no |
| `0x9c` float local / if-else float local / `(int)(float)v` | 7064 / 47.6 | no |
| `0x158` float ternary or `(int)(float)(int ternary)` | 7072 / 47.655 | no |
| `0x15c` float ternary or round-trip | 7072 / 47.655 | no |
| `0x170` float ternary or round-trip | 7072 / 47.655 | no |
| `0x158`/`0x15c`/`0x170` if/else + `(int)(float)v` phi | 7072 / 47.655 | no |

### 5.2 Removing the extra `vstr`s (HD branch → plain int)

| variant | result | removed `vstr` |
|---|---|---|
| `0x20` → `7` | 6968 / 20.888 | `0x20` **and** `0x60,0x68,0x6c,0x70,0x124,0x2d0` |
| `0x24` → `7` | 6936 / 21.203 | `0x20,0x60,0x68,0x6c,0x70,0x124,0x2d0` (gains `0x8`) |
| `0x20+0x24+0x28` → `7` | 7016 / 21.671 | `0x20,0x60,0x68,0x6c,0x70,0x124,0x2d0` (gains `0x8`) |
| `0x124` → `1` | 7008 / 42.562 | `0x124` only |
| `0x148` → `2` | 7056 / 47.503 | `0x148` only |
| `0x1f4` → `22` | 7056 / 47.623 | `0x1f4` only |
| `0x204` → `12` | 7064 / 47.396 | `0x204` only |
| `0x3e8` → `140` | 7076 / 42.354 | `0x3e8` only |

The `0x20`/`0x24` edits are the destructive ones: the compiler CSEs
`0x20/0x24/0x28` and the whole `0x60..0x70` float chain with it, so an int `0x20`
also kills the (correct) `0x60/0x68/0x6c/0x70/0x2d0` `vstr`s.

### 5.3 Pairs and triples

All 66 pairs of {`150f`,`150rt`,`1e4f`,`9ct`,`9crt`,`20i`,`124i`,`148i`,`1f4i`,
`204i`,`3e8i`} were measured. The best pair at exactly 7072 B is
`150rt + 1f4i` = **7072 / 47.615** (still below baseline). Next:
`150rt + 148i` = 7072 / 47.456. Every other pair is worse or a different size.

All 165 triples were also swept. The best 7072 B triples are
`150rt + 9ct + 204i` and `150rt + 9crt + 204i`, both **7072 / 47.337**; the best
triple overall is `150rt + 9ct + 1f4i` = 7064 / 47.564. No pair or triple at
7072 B beats the 47.655 % baseline.

### 5.4 Per-region match (baseline)

`region2.py` buckets the reconciled instruction stream by original address:

| region | match |
|---|---|
| `hd` `0xd1042..0xd1268` | 42/164 = 25.6 % |
| `n9` `0xd127e..0xd156c` | 281/340 = 82.6 % |
| `def` `0xd1570..0xd215c` | 373/1013 = 36.8 % |
| `tail` `0xd21e2..0xd2bd0` | 478/953 = 50.2 % |
| other (prologue/pools) | 25/59 |
| overall | 1199/2529 (ratio 47.655 %) |

The `def` region is the largest deficit. It is also where the original is *more*
vectorized than ours: in `0xd1570..0xd215c` the original has 24 `vst1.32`
groups, 32 `strd` and 67 `str`; ours has 15 `vst1.32`, 21 `strd` and 106 `str`.
For example the original stores `0x16c/0x170/0x174/0x178` with scalar
`str`/`strd` (`d1f0c`, `d1f34`, `d1f4a`, `d1f5e`), while ours emits one
`vst1.32 {d16-d17}` group at `d1f52` (base `r0+364`). Reordering those five
assignments in every order tried does not change our codegen.

### 5.4 Order experiments

| order | result |
|---|---|
| current source order | 7072 / 47.655 |
| `0x16c,0x170,0x174,0x178,0x17c` ascending | 7072 / 47.655 (identical codegen) |
| `0x178` moved after `0x17c` | 7072 / 47.655 (identical) |
| `0x170` moved after `0x17c` | 7072 / 47.655 (identical) |
| `0x174,0x178,0x17c` ascending | 7072 / 47.585 |
| `0x140,0x144,0x148,0x14c` ascending | 6996 / 45.049 |
| `0x94,0x98,0x9c,0xa0` ascending | 7000 / 42.826 |
| all HD-specific units in original HD-store order, then the rest | 6824 / 36.8 |
| HD units stably permuted into HD-store order, common units fixed | 6864 / 36.5 |

The last row reproduces w4221's "HD-store order as source order" result: it is
*not* enough to sort the HD fields, because the original source interleaves the
HD-specific and the common (tail) assignments.

---

## 6. Literal-pool first-use oracle (re-checked)

The original's first pool at `0xd1308` begins `9 11 5 253` — the HD values of
`0x38/0xbc/0xc8/0xcc`. These are loaded by the **first** `vld1.64 {d0-d1}` at
`d1046` and stored to `0xc0` at `d1158`. The first *store* group, however, is
`0x14` (`vst1.32 {d2-d3}` at `d10a4`). So the pool order is the scheduler's
first-use order, not the source order; it is a weak oracle. The useful signal is
the `0.462222, 1.40625` adjacency at `0xd1440/0xd1444` (HD `0x90` immediately
followed by a `1.40625f`-scaled assignment), which our pool reproduces only
partially (`0.462222` is followed by `bf00bf00` in ours).

---

## 7. Why no code change landed

Every single-field edit that reproduces a missing `vstr` changes the function
size (`0x150` +8, `0x1e4` -48, `0x9c` -8), and every edit that removes an extra
`vstr` changes it too (`0x20` -104, `0x124` -64, `0x148`/`0x1f4` -16, `0x204`
-8, `0x3e8` +4). Because `byte_exact` compares raw bytes, any size change shifts
every downstream function and destroys its byte identity (w4221 measured the
`0x3f8` restructure alone: `byte_exact` 4498 -> 2454). A landable edit must keep
`usize == 7072`.

The best 7072 B variant found is `150rt + 1f4i` at **47.615 %**, which is a
*regression* from the 47.655 % baseline. The current body is a strong local
optimum: no single edit, no pair, and no tested reorder improves the match at
7072 B.

---

## 8. Remaining work (ranked)

1. **Recover the original source interleaving of HD-specific and common
   assignments.** The HD block's store order is known exactly
   (`14(4) 4(4) 24(4) 34(4) 44(4) 54(4) 64(4) 2c8(4) 7c(4) 74(2) 2c0(2) 2d8(1)
   ac(1) 8c(2) 94(4) a4(2) c0(4) b0(4) ec(4) 294(4) 2a4(4) 108(4) 118(4) d0(3)
   dc(2) e4(2) fc(2) 104(1) 2b4(2) 128(4) 140(4) 150(4) 160(4) 170(4) 180(4)
   190(4) 1a0(4) 1b0(4) 1c0(4) 1d0(4) 138(2) 1e0(1)`), and the tail block's
   store order is the common-assignment order. Reconstructing the merge needs
   the tail order too; the 45-instruction `d1f48` cluster
   (`0x174/0x178/0x17c`) is the largest single mismatch in the `def` region.
2. **Find a source shape that keeps the `(int)(float)` conversion for
   `0x158/0x15c/0x170`.** These are int round-trips on constant selects; the
   front end folds every shape tried. The `0x3f8` opaque-member form is the only
   proven survivor. If the original computed these from another member (e.g.
   `0x154`), that would explain the round-trip and is testable against the
   emulator values (`0x154 = 0x158 + {0,1,2}` by config).
3. **Land the `0x3f8` restructure with >=3 recovered `vstr`s** so the body is
   exactly 7072 B. Alone it is 7024 B (-48).
4. **The 6 extra `vstr`s** must be fixed together with the `0x20/0x24/0x28` CSE
   chain, not individually.

---

## 9. Reproduction

Scratch (all under `/dev/shm/w4274`):

* `pscore.py` — exact (unrounded) verifier ratio for `_ZN6LayoutC1Ev`/`C2Ev`.
* `probe.py <variants.py>` — apply `VARIANTS=[(name,[(old,new),...]),...]`,
  rebuild `link_so`, print `(usize, pct, vstr_offsets)`, restore.
* `cand.py` / `allcand.py` — the single-change candidate set of §5.
* `pairsearch.py` / `triplesearch.py` — exhaustive pair/triple sweeps.
* `reorder.py`, `trymove.py`, `tryhd.py` — source-order experiments.
* `region2.py` — per-region match (hd/n9/def/tail) with original addresses.
* `diffmap.py` — diff opcodes with original address ranges.
* `emu2.py` — Thumb/VFP emulator over the original (`EMU_SO` env var), prints
  every store to `this` as `offset size value int float`.
* `stores.py` — store-group extraction for an address range.

Commands:

```sh
cmake --build cmake-build-match --target check -j8
python3 /dev/shm/w4274/pscore.py
python3 /dev/shm/w4274/region2.py
python3 /dev/shm/w4274/emu2.py _ZN6LayoutC1Ev iPadHD=1 n9=0 iPad=0 iPadLarge=0 retinaDisplay=0
```

Note: this worktree's `cmake-build-match` must be configured with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
and `-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.

---

## 10. Gate state

Before and after this pass (no code change): `RATCHET PASS` —
`byte_exact 4499/4524`, `linked_exact 4499/4524`, `size-exact 4524/4524`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data_exact 843/843`,
`imports 207`, `lint CLEAN`, `drift 0`.
Target function: `osize 7072`, `usize 7072`, `pct 47.655` (verifier prints 47.7).
