# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`) — wave 115 / worker w6301

Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), original size
**7072 B**, source `src/game/ui/Layout.cpp`.

Baseline at start (current master, `wave115-w6301`): unrounded difflib score
**48.545237146273 %**, 7072 B, 2489 normalised instructions vs the original's
2529. Gate: `RATCHET PASS`, byte_exact **4500**, linked_exact **4500**,
size_exact 4524, data_exact 843, imports 207, lint CLEAN, drift 0.

Outcome: **no landable improvement found**; `src/game/ui/Layout.cpp` is
unchanged. The best score seen (48.5828 %) is at 7048 B, which is unlandable
because any Layout size change collapses `byte_exact` (see below). All
measurements are recorded here so the next worker does not repeat them.

## Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w6301
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
OD=$PWD/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
GOF2_NDK_OBJDUMP=$OD python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN6LayoutC1Ev
```

The unrounded score used everywhere below is the same `difflib.SequenceMatcher`
ratio over the normalised instruction stream that `tools/verify/verify.py`
computes (`fscore.py`, reproduced at the bottom).

## Size accounting (measured)

| | original | current master |
|---|---:|---:|
| code (`0xd1028` … first end-pool word) | 6108 B (`0xd1028..0xd2804`) | 6292 B (`0xd1028..0xd28bc`) |
| end literal pool | 964 B = **241 words** (`0xd2804..0xd2bc8`) | 780 B = **195 words** (`0xd28bc..0xd2bc8`) |
| total | 7072 B | 7072 B |

So the original trades **46 more pool words (+184 B)** for **184 fewer code
bytes**. The original also has a *mid-function* literal pool at
`0xd12fe..0xd156e` (156 words) inside the code region; the current master has
one too, but at a different address/contents.

## Proven binary shape (original)

The original ctor is a **block tree over `(iPadHD, n9, iPad)`**, not a flat
ternary chain:

```
0xd103e  beq.w 0xd126c          ; if (!iPadHD) goto non-HD
         ...HD block, 0xd1042..0xd1268, straight-line, ends `b.w 0xd21e2`
0xd127a  beq.w 0xd1570          ; else if (!n9) goto non-n9
         ...n9 block, 0xd1272..0xd12fc, ends `b.n 0xd1860`
0xd1570  ...non-n9 block (nested retina / iPad / iPadLarge branches)...
0xd21e2  ...common tail (0x1e4/0x1e8 and everything after)...
0xd2800  pop {r4,r5,r6,r7,pc}
```

Emulating the function over all 32 flag combinations (a small Thumb2/VFP
emulator over capstone; `_work/reconstructions/layout_ctor/emu.py` works with
the `tools/verify` path patched to this worktree) shows the **store order
depends only on `(iPadHD, n9, iPad)`** — `iPadLarge` and `retinaDisplay`
change values but never the order. There are exactly **5 distinct store
orders**, each covering all 204 fields:

| leaf | flag combos | first stores |
|---|---|---|
| HD | `iPadHD=1` (16 combos) | `14 18 1c 20 4 8 c 10 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68 6c 70 2c8 2cc 2d0 2d4 7c 80 84 88 74 78 2c0 2c4 2d8 ac 8c 90 94 98 9c a0 a4 a8 c0 c4 c8 cc b0 b4 b8 bc ec f0 f4 f8 294 …` |
| `!n9 !iPad` | `00000 00001 00100 00101` | `4 8 c 10 14 18 1c 20 24 28 30 2c 38 34 3c 44 40 4c 48 54 50 58 5c 60 64 68 6c 2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8 d4 d0 a0 a4 d8 …` |
| `!n9 iPad` | `00010 00011 00110 00111` | same as above but `… 168 17c 16c 170 174 178 …` and `… 190 194 180 184 188 18c 198 …` |
| `n9 !iPad` | `01000 01001 01100 01101` | `74 78 2c0 2c4 2c8 2cc 14 18 1c 20 4 8 c 10 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68 6c 70 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8 a0 a4 d0 d4 d8 dc e0 e4 e8 fc 100 104 2b4 2b8 …` |
| `n9 iPad` | `01010 01011 01110 01111` | same as above but `… 168 17c 16c 170 174 178 …` and `… 190 194 180 184 188 18c 198 …` |

(offsets in hex; `00000` etc. are `(iPadHD, n9, iPad, iPadLarge, retinaDisplay)`)

The HD block is a straight-line sequence of stores that clang SLP-vectorises
into `vst1.32 {dA-dB}` (4 words), `strd`, and `stmia` groups; that is where the
extra 46 pool words come from (each vector group gets its own 16-byte literal).

Measured block boundaries:

| | original | current master |
|---|---|---|
| HD block | `0xd1042..0xd1268` (550 B), ends `b.w 0xd21e2` | `0xd1042..0xd1274` (562 B), ends `b.w 0xd2294` |
| non-HD + tail code | 5556 B | 5728 B |

## Where the fuzzy score is lost

Per-instruction match profile of the current master against the original
(`SequenceMatcher` opcodes over the normalised streams):

| original insn range | matched | comment |
|---|---:|---|
| 0..173 (HD block) | **22 / 173 (12.6 %)** | biggest single loss |
| 0..400 | 195 / 400 (48.6 %) | |
| 0..1000 | 531 / 1000 (53.0 %) | |
| 0..2529 | 1087 / 2529 (43.0 %) | ratio 48.5 % |

Longest equal runs: `288..377` (89 insns), `244..269` (25), `273..288` (15),
`204..217` (13). The HD block's first **36 stores already match the original
exactly**; divergence starts at index 36 (original `2c0 2c4 2d8 ac 8c 90 94 98
9c a0 a4 a8 c0 c4 c8 cc b0 b4 b8 bc ec …` vs ours `2c4 1e0 2d8 8c 90 98 9c ac
b4 b8 bc c0 c4 c8 cc d0 294 …`).

Typical diffs in the HD block are only register-number / literal-offset
differences (`vst1.32 {d2-d3}` vs `{d0-d1}`, `add r1, pc, #784` vs `#808`), but
difflib counts each as a mismatch, so the block scores ~0.

## Pool content diff (whole function, `ldr [pc]` / `vldr [pc]` / `add pc` targets)

Missing in ours (original has, we don't):

* floats: `0x40b40000` 5.625, `0x41400000` 12.0, `0x41700000` 15.0,
  `0x41e10000` 28.125, `0x42960000` 75.0
* ints: `4, 5, 9, 15, 56, 68, 174`
* 20 pointer words (`0x0013xxxx`)

Extra in ours (we have, original doesn't):

* floats: `0x42440000` 49.0, `0x42740000` 61.0, `0x42c40000` 98.0,
  `0x42f40000` 122.0, `0x430ca000` 140.625, `0x43160000` 150.0 ×2
* ints: `2, 11, 20, 35, 70`
* 20 pointer words

**Why:** the original materialises several constants with `ldr`/`vldr` from the
pool because the value is stored as an *int* through a merged `strd`/`stmia`
(store merging), whereas our source makes clang emit `vmov.f32 #imm` (VFP
immediates are encodable for 12.0, 15.0, 5.625, 28.125, 75.0) or fold the
float→int conversion per ternary arm (49.0/61.0/98.0/122.0/140.625/150.0 are
folded away in ours, kept in the original). Examples from the original:

```
d1a58  strd r3, r1, [r0, #224]   ; 0xe0 = 0x3fc00000 (1.5f), 0xe4 = 0x41400000 (12.0f)
d19e8  ldr  r2, [pc, #916]       ; 15.0f bits, then strd into 0xd8/0xdc
d1254  vldr s0, [pc, #640]       ; 5.625 = 4*1.40625f, then vcvt.s32.f32 -> 0x1e4
```

So the original's `field_0x1e4` HD arm really is `4 * 1.40625f` (not `5`), and
`field_0x1e8` is `(retinaDisplay || iPadHD) ? 1 : iPad` (not
`retina ? 1 : (iPadHD ? 1 : (n9 ? 1 : 0))`).

## Variants measured (all real `link_so` builds)

| variant | size | unrounded score | landable? |
|---|---:|---:|---|
| current master (baseline) | **7072** | **48.545237146273** | yes |
| `field_0x194` int (drop `(int)(float)`) | 7064 | 48.554913294798 | no (−8 B) |
| `0x9c` as float ternary (`70.0f/75.0f/100.0f/50.0f`) | 7096 | 48.299184404217 | no (+24 B) |
| `0xac`/`0xa8` as int ternaries | 7032 | 48.461845785058 | no (−40 B) |
| both of the above ("tboth") | 7048 | **48.582834331337** | no (−24 B) |
| tboth + `(int)(float)` on `0x2d8`+`0x150` | 7072 | 48.315726529799 | yes (worse) |
| tboth + `(int)(float)` on `0x2d0`+`0x150`+`0x194` | 7072 | 48.325358851675 | yes (worse) |
| tboth + `(int)(float)` on `0x2d8`+`0x2d0` | 7072 | 48.315726529799 | yes (worse) |
| tboth + `(int)(float)` on `0x1a4`+`marqueeWidth` | 7072 | 48.137079099422 | yes (worse) |
| tboth + faithful `0x1e8` (`\|\|` form) | 7072 | 35.627368841013 | yes (much worse) |
| tboth + `0x1e4` HD arm `4*1.40625f` | 7040 | 46.159984041492 | no |
| tboth + move `eventBannerScale` after `0x13c` | 7016 | 45.563549160671 | no |
| tboth + move `0x94` after `0x90` | 6972 | 46.409947853991 | no |
| tboth + move `0x2c0` after `0x78` | 7004 | 45.400000000000 | no |
| base + move `0x2c0` after `0x78` | 7028 | 45.127795527157 | no |
| base + move `0x94` after `0x90` | 6992 | 45.654785742891 | no |
| base + move `0xa0` after `0x9c` | 7088 | 43.414245921210 | no |
| base + move `0xa4` after `0xa0` | 7064 | 48.326693227092 | no |
| base + swap `0x1e4`/`0x1e8` | 6948 | 34.244372990354 | no |
| base + `0x2c0` after `0x78` + `2c4 2c8 2cc 2d0 2d4 2d8` reordered | 7028 | 45.327476038339 | no |
| same + `eventBannerScale` after `0x1e4` | 7028 | 44.205066826252 | no |
| base + faithful `0x1e8` (`\|\|` form) | 7096 | 35.650268870743 | no |
| base + `0x160` int arms (`49 * 1.40625f`, `98`, `49`, …) | 7048 | 48.444976076555 | no |
| base + `(int)(float)` on `0x180` | 7088 | 48.258013139558 | no |
| base + `(int)(float)` on `scrollBarInset` | 7100 | 23.070803500398 | no |
| base + `(int)(float)` on `0x1fc` | 7108 | 47.070506454816 | no |
| base + `(int)(float)` on `0x2c4` | 7132 | 48.482443959532 | no |
| wave-89 all-ternary body (older, simpler) | 6800 | 50.788515972503 | no |
| wave-89 body + `u9c` variable form | 6776 | 50.344268934791 | no |
| wave-89 body + `v` (`0xac`/`0xa8`) form | 6848 | 50.221684804514 | no |
| wave-89 body + `0x2d4` if-chain | 6856 | 50.141185962081 | no |
| wave-89 body + all 17 byte-growing casts | 6968 | 45.041073933080 | no |

### Statement-order experiments

The iOS 1.1.4 build's store order (`_work/reconstructions/layout_ctor/ios_source_order.md`)
is **not** the Android 2.0.16 source order:

| reorder | size | score |
|---|---:|---:|
| current master order | 7072 | 48.545237146273 |
| all 186 top-level statements in iOS order | 7092 | 17.196702002356 |
| all 186 top-level statements in HD-trace order | 6708 | 32.413793103448 |
| generated 5-leaf block tree (values from `lay_orig.json`, order from the traces) | 5736 | 15.247832851745 |

The generated block tree is 1336 B *smaller* than the original because clang
merges the identical constant stores across the five leaves; the original's
leaves are much less merged. So the original source is **not** a fully
duplicated 5-leaf tree either — only the HD leaf is fully straight-line, and
the non-HD path is the per-field ternary chain (which the current master
already reproduces).

### Searches that found nothing

* 120 + 250 random mutations (adjacent swap, arbitrary move, add/remove
  `(int)(float)` cast) from the current master, accepting only size 7072 and
  score > 48.545237146273 — **no hit**.
* 150 random multi-moves from `tboth` (7048) — **no 7072 hit at all**.
* Exhaustive single/double/triple `(int)(float)` cast additions on `tboth`
  (all subsets of 36 candidate fields) — every 7072 hit scores **≤ 48.33**.

## Why 7072 B is mandatory

`check` on the 7048-B `tboth` variant:

```
verify: byte 3221  linked 4500  stubs 0  missing 0  extra 0
FAIL  byte_exact 4500 -> 3221
RATCHET FAIL
```

`linked_exact` survives (branch offsets are PC-relative), but `byte_exact`
collapses because every function after `Layout` in `.text` shifts and its
literal-pool words (absolute addresses / `.rodata` pointers) change. So a
Layout size change is a hard gate failure even though the target function's own
score improves.

## Conclusion / next steps

The current master body is a sharp local optimum of the fuzzy metric: single
statement moves, adjacent swaps, cast additions/removals and expression
rewrites all lose more than they gain. The remaining 46 pool words and the
12.6 % HD-block match can only be recovered by reproducing the original's
**exact HD-block store order and constant materialisation**, which requires
the original statement order — and the iOS 1.1.4 order demonstrably is not it.

The most promising concrete lead: the original's non-HD path keeps float
constants (12.0, 15.0, 57.6, 64.0, 37.5, 50.0, 75.0, …) in the pool because
they are stored as **int** through merged `strd`/`stmia` groups, while our
source lets clang fold `(int)(float)` per ternary arm. Writing those fields so
the value flows through an `int` temporary that is *also* used as a float
somewhere (like the proven `field_0xac` / `field_0xa8` pair) should recover
several pool words and some HD-block instructions — but every attempt here
changed the function size, so it must be paired with a −N-byte change that
keeps the total at exactly 7072.

## `fscore.py`

```python
#!/usr/bin/env python3
import sys, difflib
sys.path.insert(0, "/home/debian/GalaxyOnFire2/wt/w6301/tools/verify")
import verify
from elf import Elf32
REPO = "/home/debian/GalaxyOnFire2/wt/w6301"
ORIG = "/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so"
OURS = REPO + "/cmake-build-match/libgof2hdaa.so"
OBJ = REPO + "/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump"

def get(name):
    of = Elf32(ORIG); oe = Elf32(OURS)
    orig = of.functions(); ours = oe.functions(); common = set(orig) & set(ours)
    def ar(e):
        m = [s for s in e.sections if s["addr"]]
        lo = next((s["addr"] for s in m if s["name"] == ".plt"), min(s["addr"] for s in m))
        return lo, max(s["addr"] + s["size"] for s in m)
    orr = verify.Resolver(orig, common, of.plt_map(), ar(of))
    our = verify.Resolver(ours, common, oe.plt_map(), ar(oe))
    co = {o[2] for o in of.objects()} & {o[2] for o in oe.objects()}
    for r, e in ((orr, of), (our, oe)): r.attach_data(e, co)
    imp = ({v for k, v in of.got_map().values() if k == "sym"}
           & {v for k, v in oe.got_map().values() if k == "sym"})
    orr.imports = our.imports = imp
    o, u = orig[name], ours[name]
    _, on = verify.disassemble(OBJ, of.bytes_at(o["value"], o["size"]), o["thumb"], o["value"], orr)
    _, un = verify.disassemble(OBJ, oe.bytes_at(u["value"], u["size"]), u["thumb"], u["value"], our)
    on, un = verify.reconcile(on, un)
    pct = 100.0 if on == un else difflib.SequenceMatcher(None, on, un, autojunk=False).ratio() * 100
    return o["size"], u["size"], pct

for name in sys.argv[1:] or ["_ZN6LayoutC1Ev"]:
    os_, us, p = get(name)
    print(f"{name}: orig {os_}B ours {us}B score={p:.15f}")
```
