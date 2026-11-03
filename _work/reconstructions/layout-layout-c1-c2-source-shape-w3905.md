# Layout::Layout C1/C2 — iOS source-order/expression recovery and size-7072 shape sweep (w3905)

Status: analysis-only pass (no code change). Worktree `wt/w3905`, branch
`wave89-w3905`, base `36c65db7`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` stay at **47.7% / 7072 B**. The gate is
unchanged before and after this pass:

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4498 / 4524 | **4498 / 4524** |
| `linked_exact` | 4498 / 4524 | **4498 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| `.text` | `0x1337c8` | **`0x1337c8`** |
| `.rodata` | `0x41b67` | **`0x41b67`** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`. The current `src/game/ui/Layout.cpp` body is a local optimum
for the verifier's fuzzy metric at the required size: every source-shape
variant tried below is either smaller than 7072 B (and therefore shifts
`.text`, see §1) or scores `<= 47.7` at 7072 B.

## 1. The hard constraint: the body must stay exactly 7072 B

The whole-`.so` `byte_exact` count depends on `Layout::C1` being exactly
7072 bytes. Applying the 7016-byte `field_0x98` if/else variant shrinks
`.text` from `0x1337c8` to `0x1337a8` and drops `byte_exact` from 4498 to
4344, because every following function shifts. Any candidate must therefore
keep `usize == 7072`; a smaller (or larger) body is not landable even if its
fuzzy score is higher. Padding anchors are forbidden by the task.

## 2. The iOS 1.1.4 oracle

The iOS 1.1.4 armv7 binary is available in
`/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa`
(extracted at `/tmp/ipa114`). `Layout::Layout()` starts at **VMA `0x1006d0`**
(not `0x1006d1`: the `LC_FUNCTION_STARTS` entry is odd, but the first Thumb
instruction is the even address `0x1006d0`), file offset `0x1006d0 - 0x1000 =
0xff6d0`, size `0x3850`.

The iOS build was compiled without the Android `-Oz` if-conversion, so its
constructor is a straight line of one flag-test chain per field, ending in a
single store. **The store order is the source order.** There are exactly 204
stores, one per `Layout` field, and the offset set is identical to the
Android 2.0.16 struct.

### Recovering the source order and the per-field expression trees

The parser (capstone, Thumb) is self-contained:

```python
from capstone import *
data = open('/tmp/ipa114/Payload/Galaxy on Fire 2 HD.app/Galaxy on Fire 2 HD','rb').read()
base, size = 0x1006d0, 0x3850
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB); md.detail = True
ins = list(md.disasm(data[base-0x1000:base-0x1000+size], base))

# Globals are reached through GOT slots; 0x4220c0 is the preloaded `sb`
# (iPadHD), `r3` is iPadLarge (0x4220c4), `ip` is n9 (0x4220b8), and the
# remaining slots are retinaDisplay (0x4220a8) and iPad (0x4220bc).
# Walk the linear stream: a `tst.w rX,#1` after a global `ldrb` is one flag
# test; a following `bne`/`it ne; movne` is the selected value; a store ends
# the field.  See the inline parser in this note's appendix.
```

The recovered **iOS source order** (204 offsets, hex) is:

```
4 8 c 10 14 18 1c 20 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68 6c
2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8 a0 a4
b0 b8 b4 bc c0 c4 c8 cc d0 d4 d8 dc e0 e4 e8 ec f0 f4 f8 fc 100 104
294 298 29c 2a0 2a4 2a8 2ac 2b0 2b4 2b8
108 10c 110 114 118 11c 120 124 128 12c 130 134 138 13c 140 144 148 14c 150
154 158 15c 160 164 168 16c 170 174 178 17c 180 184 188 18c 190 194 198 19c
1a0 1a4 1a8 1ac 1b0 1b4 1b8 1bc 1c0 1c4 1c8 1cc 1d0 1d4 1d8 1dc 1e0 1e4 1e8
1ec 1fc 204 208 20c 210 214 218 21c 220 224 228 22c 230 234 238 23c 240 244
248 24c 250 254 258 25c 260 264 268 26c 270 274 278 27c 280
3e0 3e4 3e8 3f4 3f8 3ec 3f0 3fc 2bc 1f0 1f4 200 1f8 284 285 286 288 28c 290
2f4 2f8 2fc 300 304 308 30c 310 314 318 31c 320
```

The Android default-path store trace starts
`4 8 c 10 14 18 1c 20 24 28 30 2c 38 34 3c 44 40 4c 48 54 50 58 5c 60 64 68
6c 2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 ...` — the same order modulo the
`strd` pair swaps (`30 2c`, `38 34`, `44 40`, `4c 48`, `54 50`). So the
Android 2.0.16 source order **is** the iOS order; the differences are
scheduling and the Android-only value/condition edits.

### Per-field expression trees

The parser returns a fully parenthesised tree per field, e.g.

```
004  (iPadHD ? 21 : (n9 ? 22 : (retinaDisplay ? 30 : 15)))
2d4  (iPadHD ? 112 : (n9 ? (iPad ? 160 : 100) :
                      (retinaDisplay ? (iPad ? 160 : 100) : (iPad ? 100 : 60))))
31c  (iPadHD ? 331 : (iPadLarge ? 472 : (iPad ? 236 : (n9 ? 236 :
                      (retinaDisplay ? 236 : 118)))))
```

The Android values (recovered by emulating the original `.so` over all 32
`Globals` flag combinations) differ from the iOS 1.1.4 values for many
fields, and some Android fields gained an extra condition (e.g. `0x2c0` is
`H ? -2 : (N ? -3 : -2)` on iOS but `H ? -2 : (N ? -3 : (R ? -4 : -2))` on
Android). The **flag-test order** is unchanged for 176 fields; it differs
for these 28:

| offset | field | iOS order | current order |
|---|---|---|---|
| `2c0` | field_0x2c0 | iPadHD n9 | iPadHD n9 retinaDisplay |
| `2d4` | field_0x2d4 | iPadHD n9 iPad retinaDisplay | (if/else) |
| `2d8` | field_0x2d8 | iPadHD n9 iPad retinaDisplay | iPadHD retinaDisplay n9 iPad |
| `09c` | field_0x9c | iPadHD n9 retinaDisplay | (if/else) |
| `0ac` | field_0xac | iPadHD iPadLarge iPad n9 retinaDisplay | iPadHD iPadLarge n9 iPad retinaDisplay |
| `0a8` | field_0xa8 | iPadHD iPadLarge iPad n9 retinaDisplay | iPadHD iPadLarge n9 iPad retinaDisplay |
| `108` | field_0x108 | iPadHD n9 | iPadHD n9 retinaDisplay |
| `184` | field_0x184 | iPadHD iPadLarge iPad n9 retinaDisplay | (if/else) |
| `190` | field_0x190 | iPadHD iPadLarge iPad n9 | (if/else) |
| `1cc` | field_0x1cc | iPadHD n9 retinaDisplay | (if/else) |
| `1e8` | field_0x1e8 | iPadHD n9 | retinaDisplay iPadHD n9 |
| `20c` | field_0x20c | iPadHD n9 retinaDisplay | (if/else) |
| `214` | field_0x214 | iPadHD n9 | iPadHD n9 retinaDisplay |
| `218` | field_0x218 | iPadHD n9 retinaDisplay | (if/else) |
| `21c` | orbitInfoTextX | iPadHD n9 retinaDisplay | (if/else) |
| `220` | orbitInfoStationY | iPadHD n9 retinaDisplay | (if/else) |
| `24c` | field_0x24c | iPadHD n9 | iPadHD n9 retinaDisplay |
| `260` | field_0x260 | iPadHD n9 | (if/else) |
| `280` | field_0x280 | iPadHD n9 | (if/else) |
| `3e4` | scrollBarHandle | iPadHD n9 | iPadHD n9 retinaDisplay |
| `3f8` | rewardBoxY2 | iPadHD n9 | retinaDisplay |
| `1f4` | field_0x1f4 | iPadHD iPad | iPadHD retinaDisplay iPad |
| `200` | field_0x200 | iPadHD n9 | iPadHD n9 retinaDisplay |
| `1f8` | field_0x1f8 | iPadHD iPad | iPadHD retinaDisplay iPad |
| `310` | field_0x310 | iPadLarge iPad n9 | iPadLarge iPad n9 retinaDisplay |
| `314` | field_0x314 | iPadLarge iPad n9 | iPadLarge iPad n9 retinaDisplay |
| `318` | field_0x318 | iPadLarge | iPadLarge iPad |
| `31c` | helpWindowX | iPadHD iPadLarge iPad n9 retinaDisplay | iPadHD iPadLarge retinaDisplay n9 iPad |

Changing any one of these to the iOS order (with the Android values, keeping
the current statement order) was measured field-by-field: all 165 such
variants stay at 7072 B and `<= 47.7`; none improves.

## 3. Android value table and the `* 1.40625f` form

The original's per-field values were recovered by emulating
`_ZN6LayoutC1Ev` for all 32 combinations of
`iPadHD/n9/iPad/iPadLarge/retinaDisplay` (`_work/reconstructions/layout_ctor/emu.py`
and `table.py`; the w3905 copy is `/dev/shm/w3905/emu.py`). The HD value is
usually `int(ipad_value * 1.40625)`, which the source writes as
`ipad_value * 1.40625f`. Removing that form (plain folded integers) gives
6888 B / 11.7%; writing it for every HD value that matches gives
7040 B / 20.5%. The current mixed form is the best measured.

## 4. Sweep results (all measured with the official verifier)

Every row is the full `Layout::Layout` body compiled with the repo's NDK
r18b clang 7.0.2 `-Oz` flags and scored against the original.

| variant | size | pct |
|---|---:|---:|
| current `src/game/ui/Layout.cpp` (if/else-mixed, hill-climbed order) | **7072** | **47.7** |
| iOS source order, current expressions | 6880 | 37.4 |
| iOS source order, iOS trees with Android values (173 fields; 31 fallback) | 6580 | 13.8 |
| iOS source order, pure chains from Android values | 6548 | 37.5 |
| member-initializer list, declaration order, current expressions | 6536 | 16.1 |
| cache all five flags in `const bool` locals | 1356 | 7.9 |
| explicit `if (hd) {HD constants} else {n9/ret constants; mid} ; tail` block tree | 5900 | 19.9 |
| all 13 if/else blocks converted to ternaries, current order | 7164 | 45.7 |
| `field_0x98` if/else only | 7016 | 47.8 |
| `field_0x98` if/else + `field_0x9c` + `rewardBoxHeight` ternaries | 7032 | 47.9 |
| `field_0x98` if/else + `field_0x20c` ternary | 7052 | 46.6 |
| `field_0x98` if/else + `field_0x180` + `field_0x2f4` ternaries | 7072 | 47.5 |
| `field_0x98` if/else, block moved to position 43/44 | 6968 | 48.0 |
| braces on every if/else body | 7072 | 47.7 |
| nested `else { if ... }` spelling | 7072 | 47.7 |

Exhaustive sub-sweeps (all size-7072 results, none above 47.7):

* all 203 adjacent statement swaps (204-unit body): no improvement;
  sizes 6932-7188, best score at 7072 is 47.7.
* all 13 single if/else→ternary conversions: 7072-7108, best 47.7.
* all 78 pairs and all 715 four-way combinations of the 13
  if/else→ternary conversions: best 7072/47.7.
* `field_0x98` if/else + all 190 single ternary→if/else conversions:
  7072 only for `field_0x84` (46.4) and `field_0xac` (47.1).
* `field_0x98` if/else + all 116 pairs whose single size deltas sum to
  +40: best 7072/47.5.
* `field_0x98` if/else block moved to all 203 other positions: no 7072;
  best 6968/48.0.

The earlier w3821 sweep (all-ternary body + 272-byte pad) remains the
highest fuzzy score ever measured (50.8 at 6800+272), but the pad is
forbidden and the 6800-byte body shifts `.text`.

## 5. Branch-tree block map and store traces

The original `_ZN6LayoutC1Ev` is a tail-merged branch tree (real VMA
`0xd1028`):

| region | addresses | fields | role |
|---|---|---:|---|
| HD block | `d1042..d1268` | 137 | `if (iPadHD)` constants; `vst1.32`/`strd`/`stmia`; then `b.w d21e2` |
| n9 block | `d127e..d12fc` | 34 | `if (n9)` constants; `b.n d1860` |
| retina/default block | `d1570..d1830` | 34 | retina/default constants; falls into mid |
| mid block | `d1860..d215c` | 101 | shared by n9 and non-n9; `b.n d21e2` |
| tail | `d21e2..d2bd0` | 67 | common; contains a second `if (iPadHD)` |

The HD block's first stores are `0x14 0x18 0x1c 0x20` (one `vst1.32`),
then `0x4 0x8 0xc 0x10`, then `0x24 0x28 0x2c 0x30`, ... — the iOS source
order after scheduling. The per-flag store traces are in
`/dev/shm/w3905/trace_orig.txt` (`default`, `n9`, `hd`); the default trace
is reproduced in §2. The current source writes `0x98` and `0x94` twice
(different branches); the original writes each once per path.

## 6. Conclusion and remaining work

The remaining delta is **instruction selection / scheduling inside the
tail-merged blocks**, not missing source semantics:

* the values are bit-exact for all 32 flag combinations (0 bad combos,
  807 bytes / 202 words);
* the literal-pool content and order match (the prior w3821 note: the
  Android pool is the iOS-implied pool, 4 words short at the front for
  `field_0x90`/`field_0x9c`);
* the original uses more 16-bit flag/branch code and more
  `strd`/`vst1.32` pairs, fewer `str.w`; our all-ternary body is 6800 B,
  the original 7072 B.

The source order is under-determined from the binary alone: the `-Oz`
scheduler reorders stores within each basic block, and the iOS order (the
true source order) scores worse on the fuzzy metric than the current
hill-climbed order. A future pass should either (a) recover the exact
Android 2.0.16 expression trees from the mid/tail blocks (the `cmp; it eq;
moveq` sequences) and regenerate the body, or (b) find a 272-byte
real-code construct that restores the all-ternary body to 7072 B without
changing the instruction stream.

## 7. Reproduction

```sh
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN6LayoutC1Ev

# variant scorer (w3905 scratch copy)
python3 /dev/shm/w3905/score_variant.py <Layout.cpp> <tag>

# iOS oracle: extract the IPA, then
#   /tmp/ipa114/Payload/Galaxy on Fire 2 HD.app/Galaxy on Fire 2 HD
#   Layout::Layout at VMA 0x1006d0, file offset 0xff6d0, size 0x3850
```

## Appendix: iOS parser (self-contained)

```python
from capstone import *
import re
data = open('/tmp/ipa114/Payload/Galaxy on Fire 2 HD.app/Galaxy on Fire 2 HD','rb').read()
base, size = 0x1006d0, 0x3850
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB); md.detail = True
ins = list(md.disasm(data[base-0x1000:base-0x1000+size], base))
SB = 0x4220c0
G = {0x4220c0:'iPadHD', 0x4220b8:'n9', 0x4220a8:'retinaDisplay',
     0x4220bc:'iPad', 0x4220c4:'iPadLarge'}
# preloaded r3 = iPadLarge, ip = n9 (recovered from the movw/movt/add
# sequences at 0x103d44 and 0x103982)
REG_GOT = {'r3':0x4220c4, 'ip':0x4220b8}
ann = {}
last_got = pending = off = last_movs = cur_got = None
for i, x in enumerate(ins):
    m, o = x.mnemonic, x.op_str
    if m == 'movw' and o.startswith('r1, #'): pending = int(o.split('#')[1], 0)
    elif m == 'movt' and o.startswith('r1, #') and pending is not None:
        off = pending | (int(o.split('#')[1], 0) << 16); pending = None
    elif m == 'add' and o.startswith('r1, pc') and off is not None:
        last_got = (x.address + 4 + off) & 0xffffffff; off = None
    elif m in ('movs','mov.w') and o.startswith('r1, #'):
        last_movs = int(o.split('#')[1], 0)
    elif m in ('ldrb','ldrb.w'):
        if '[sb]' in o: cur_got = SB
        elif '[r3]' in o: cur_got = REG_GOT['r3']
        elif '[ip]' in o: cur_got = REG_GOT['ip']
        else: cur_got = last_got
    ann[x.address] = dict(got=cur_got, movs=last_movs)
addr2i = {x.address:i for i,x in enumerate(ins)}
def parse(start_i, store_i, val=None):
    i = start_i
    while i <= store_i:
        x = ins[i]; m, o = x.mnemonic, x.op_str
        if m.startswith('str') or m == 'vstr': return val
        if m in ('movs','mov.w') and o.startswith('r1, #'):
            val = str(int(o.split('#')[1], 0))
        elif m in ('mvn','mvn.w') and o.startswith('r1, #'):
            val = str((~int(o.split('#')[1], 0)) & 0xffffffff)
        elif m == 'tst.w':
            flag = G.get(ann[x.address]['got'], '?')
            nxt = ins[i+1] if i+1 <= store_i else None
            if nxt and nxt.mnemonic == 'it':
                j = i+2
                if j <= store_i and ins[j].mnemonic.startswith('mov') and 'ne' in ins[j].mnemonic:
                    v = int(ins[j].op_str.split('#')[1], 0)
                    val = '(%s ? %s : %s)' % (flag, v, val); i = j+1; continue
                i += 1; continue
            elif nxt and nxt.mnemonic.startswith('bne'):
                tgt = int(nxt.op_str.split('#')[1], 0); ti = addr2i[tgt]
                a = parse(i+2, store_i, val); b = parse(ti, store_i, val)
                return '(%s ? %s : %s)' % (flag, b, a)
            elif nxt and nxt.mnemonic.startswith('beq'):
                tgt = int(nxt.op_str.split('#')[1], 0); ti = addr2i[tgt]
                a = parse(i+2, store_i, val); b = parse(ti, store_i, val)
                return '(%s ? %s : %s)' % (flag, a, b)
        i += 1
    return val
prev = 0
for i, x in enumerate(ins):
    if (x.mnemonic.startswith('str') or x.mnemonic == 'vstr') and '[r0' in x.op_str:
        off = int(x.op_str.split('#')[1].rstrip(']'), 0)
        print('%03x %s' % (off, parse(prev, i)))
        prev = i + 1
```
