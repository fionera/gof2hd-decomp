# Layout::Layout() — the original source order, recovered from the iOS binary

Status: analysis artifact (no code change). The Android ctor is still at 45.8 %
(6636 B) with the hill-climbed statement order; the *true* source order below is
structurally much closer but scores worse on the fuzzy instruction metric.

## Where the function is

`Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa` (extracted at
`/tmp/ipa114`) contains an **armv7** Mach-O (`Galaxy on Fire 2 HD`, not arm64 —
the 1.1.19 arm64 object in `../source file list dump/` is a different build).

* `__TEXT,__text` VMA `0x3830`, file offset `0x2830` (file offset = VMA - 0x1000).
* `LC_FUNCTION_STARTS` gives the function at **`0x1006d1`, size 0x3850 (14416 B)**.
* It is `Layout::Layout()`: the first store is `str r1,[r0,#0x4]` with values
  21/22/30/15, i.e. exactly `field_0x4 = iPadHD ? 21 : n9 ? 22 : retina ? 30 : 15`.

## Shape

The iOS build is compiled without the Android `-Oz` if-conversion, so the ctor is
a **straight line of one flag-test chain per field**, each ending in a single
store. That means **the store order is the source order**. There are exactly
**204 stores, one per field**, and the offset set is *identical* to the Android
2.0.16 `Layout` struct (no field added/removed between 1.1.4 and 2.0.16).

There are **no `bl`/`blx`** — the ctor is pure field initialisation, no
TouchButton/String/Array sub-object construction. (Confirms the earlier finding.)

## The order

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

(offsets in hex, source order)

Notes on the non-monotonic runs (these are real, not scheduler noise):
`2c0` sits between `6c` and `70`; `2c4..2d8` between `78` and `7c`;
`ac a8 a0 a4`; `b0 b8 b4 bc`; `294..2b8` after `104`; `dc e0 e4 e8` after `d8`;
`3e0 3e4 3e8 3f4 3f8 3ec 3f0 3fc 2bc` before `1f0`; `1f0 1f4 200 1f8`;
`284 285 286 288 28c 290`; `2f4 2f8 ...`.

## Per-field condition order

The same disassembly also yields the exact flag-test order per field, e.g.

```
field_0x4  : f1 ? 21 : f2 ? 22 : f3 ? 30 : 15        (f1..f3 = iPadHD, n9, retina)
field_0x8  : f1 ? 42 : f2 ? 45 : f3 ? 60 : 30
field_0x2c : f1 ? 4  : f2 ? 6  : f3 ? 6  : 3
```

For all 76 fields whose 1.1.4 values still match the 2.0.16 values, the flag
order recovered from iOS is **identical** to the current `Layout.cpp`
expressions, so the ternary forms are already right.

## Measured (Android `_ZN6LayoutC1Ev`, `score_layout.py`)

| variant | score | size | insns |
|---|---:|---:|---:|
| current hill-climbed order (HEAD) | **45.8** | 6636 | 2366 |
| iOS source order | 33.7 | 6840 | 2423 |
| iOS order + iOS-recovered ternaries (76 fields) | 22.1 | 6912 | 2527 |
| iOS order, adjacent-swap hill climb (4 passes) | 43.9 | 6736 | 2413 |
| iOS order for head / current for tail (k=137) | 31.0 | 6684 | 2384 |
| current for head / iOS order for tail (k=137) | 34.5 | 6720 | 2375 |

Store-order agreement with the original (per-flag-combination store trace,
`compare_traces.py`): current order **62–75 %**, iOS order **70–93 %**.
So the iOS order is the structurally correct one; it loses on the fuzzy metric
only because instruction-level details (register allocation, literal-pool
island placement) shift. The Android literal pool at the end of the function
contains the same constants in the same order as the iOS order implies, but
ours is 4 words short at the front (0.325, 0.65, 100, 50 — the `field_0x90`
and `field_0x9c` constants, which the original groups into one 16-byte island
loaded with `addw r1,pc,#3848` from the common block).
