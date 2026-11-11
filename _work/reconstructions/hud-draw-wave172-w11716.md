# Hud::draw — wave 172 (w11716): dock/show declaration-order sweep, no win

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.  Entry state (wave-170 / w11616):
raw **548**, aligned **220**, size 9,332, `.ARM.extab` 0x26298 (77 LSDA
entries), `.rel.plt` byte-identical.

No code change landed: every measured variant was neutral or worse.  The tree
is left at the 548-raw shape.  Gate re-checked green (byte_exact 4507/4524,
linked 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370).

## Damage block `hitFlags & 0x24` (20-byte span at 164188)

The original is the plain form:

```
tst r1,#0x24 ; itt ne ; movne r0,#300 ; strne r0,[sl,#0x4b8]
tst r1,#0x18 ; itt ne ; movne r0,#300 ; strne r0,[sl,#0x4b4]
```

The landed source keeps the `(void) *(Status * volatile *) &Globals::status;`
size anchor *and* the unbraced following store (so `0x4b8` is stored
unconditionally, unlike the original).  The anchor is a hard size anchor:

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---|
| landed anchor (unbraced store) | **548** | **220** | 9,332 | 0x26298 |
| plain `if (hitFlags & 0x24) field_0x4b8 = 300;` (no anchor) | 7,520 | 633 | **9,268** | 0x26298 |
| braced anchor + conditional store | 7,120 | 515 | 9,336 | 0x26298 |
| anchor before the 0x24 `if` | 7,126 | 514 | 9,336 | 0x26298 |
| anchor between the two `if`s | 7,112 | 514 | 9,336 | 0x26298 |
| anchor after the 0x18 `if` | 7,097 | 512 | 9,336 | 0x26298 |
| anchor inside the 0x18 `if` | 7,106 | 515 | 9,336 | 0x26298 |

Removing the anchor drops the function 64 bytes (the anchor changes the
global register allocation, not just one local load), so it cannot be removed
without finding the missing 64-byte structure elsewhere.

## Show block: all 120 declaration orders of `barH,text,halfW,dummy,barYOffset`

Re-measured with the landed dock types (`unsigned int barW; short barH`).
40 orders keep 77 LSDA entries (extab 0x26298); 40 give 78 (0x2629c); 40 are
size 9,336/9,344.  Best 77-entry orders:

| order | raw | aligned | size | extab |
|---|---:|---:|---:|---|
| `H T W D Y` (landed) | **548** | **220** | 9,332 | 0x26298 |
| `H T W Y D` | 548 | 220 | 9,332 | 0x26298 |
| `Y D H T W` | 552 | 224 | 9,332 | 0x26298 |
| `H T D Y W` | 553 | 226 | 9,332 | 0x26298 |

The best raw order overall is still `T W H D Y` at **515 raw / 211 aligned**,
but it is 78 entries (extab 0x2629c) and therefore gate-fatal.  `T H W D Y`
is 546 raw / 225 aligned, also 78 entries.

## Dock block declaration permutations

Permutations of `frameH, textH, barW, barH` (others fixed; `barOffset` needs
`frameH`/`textH`, `progress` needs `transferred`/`total`):

| order | raw | aligned | size |
|---|---:|---:|---:|
| `F X R T P O S W H` (landed) | **548** | **220** | 9,332 |
| `F X R T P O S H W` (Height before Width) | 548 | 219 | 9,332 |
| `X F R T P O S W H` (textH first) | 605 | 233 | 9,332 |
| `S F X R T P O W H` (String first) | 2,871 | 346 | 9,340 |

`F X R T P O S H W` is raw-neutral and byte-different (4 bytes at
165940–165950): it makes `barW -> r8` and `Canvas -> r5` match the original,
but it swaps the call order to Height-then-Width, while the original calls
Width (16593e) then Height (16594e).  It is a less faithful shape for no raw
gain, so it was not landed.

Other dock type re-tests with the current show order (all size 9,332 unless
noted): `int barW; int barH` 1,466 (9,328); `int barW; short barH` 1,585
(9,336); `unsigned int barW; int barH` 1,609 (9,324); `short barW; int barH`
610; `unsigned int barW; unsigned short barH` 549; show-block `barH` as
`unsigned int` 548, as `short` 3,046 (9,320).

## Gamma register swap (13-byte span at 164052–164081)

Original `0x44a -> fp`, `0x448 -> r9`; ours the reverse.  Reordering the
`gammaY`/`gammaBarY` declarations or parenthesising `0x448 - (0x44a - 0x448)`
produces byte-identical output (raw 548).  Writing it as
`0x448 + 0x448 - 0x44a` or `2*0x448 - 0x44a` breaks the shape (7,349 raw,
size 9,328).  No win.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j2
```
