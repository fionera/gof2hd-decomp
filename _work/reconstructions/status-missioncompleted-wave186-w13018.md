# `Status::missionCompleted` — wave 186 (w13018): why the plain switch cannot
# reproduce the original's case-body order

Worktree `wt/w13018`, branch `wave186-w13018`, base master `2d02567f6`
(wave 185).  Target `_ZN6Status16missionCompletedEbbx`
(`src/game/mission/Status.cpp`, 1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13018`.

**Status: no ordinary-C++ win landed.**  Base is raw **721**, aligned 327,
size 1,432/1,432, insns 529/527, `.text` diff 74,402, `.ARM.extab` 647
(size/base exact at 0x1a3b78, 156,312 B), `.rel.plt` byte-identical.

## 0. Bottom line

The original lays the big switch bodies out in **two sorted runs**
(`167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172`), while clang 7 in this build
lays them out **ascending by case value** (`150..172, 184, 189`).  The
wave-185 hardcoded 192-byte rank table (`0x20b977`, manual data) reaches
raw 449; every ordinary-C++ table form pays a PIC `ldr+add pc` (+4 B)
and shifts the whole body block, so it is worse than the plain switch.

## 1. The original's physical order (from the TBH tables)

`tools/fn_diff.py` + a scratch TBH decoder (`order.py`) read the two
jump tables:

| | original | ours (base) |
|---|---|---|
| outer TBH (types 150..189) | `167,174, 150,151,152,153,154,155,158,162,166,170,184,189, 156,157,160,163,164,165,168,169,171,172` | `167,174, 150,151,152,153,154,155,156,157,158,160,162,163,164,165,166,168,169,170,171,172,184,189` |
| inner TBH (types 8..14) | `8,11,13,14, 9,10,12` | identical |

The first body (`167/174`) is the TBH fallthrough in both builds.  The
inner TBH (types 8,11,13,14) and the outer non-inner bodies
(`167/174`, `8`, `11`, `13`, `14`, `0`) are already byte-identical to
the original; only the 22 inner bodies are permuted.

The original's order is exactly `[group A sorted] ++ [group B sorted]`
with group A = `{150..155,158,162,166,170,184,189}` and group B =
`{156,157,160,163,164,165,168,169,171,172}` — the signature of two
merged switches whose case lists are concatenated group-by-group.

## 2. What was measured this wave (all reverted)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base (nested outer `{167,174,8,0,11,13,14}` / inner `{150..172}`) | **721** | 327 | 1,432 |
| flat switch, all cases in one switch | 722 | 322 | 1,432 |
| flat switch, cases reordered to the original physical order | 722 | 322 | 1,432 |
| inner switch cases reordered to the original physical order | 722 | 322 | 1,432 |
| inner cases reordered (152 first) | 721 | 327 | 1,432 |
| outer `{150..189}` / default→inner `{8,0,11,13,14}` | 1,130 | 376 | 1,380 |
| three-level nest (group A / group B / `{8,0,11,13,14}`) | 1,130 | 376 | 1,380 |
| inner switch replaced by an `if/else if` chain (original order) | 1,302 | 445 | 1,388 |
| `switch (__builtin_expect(mtype, 167))` | 1,314 | 540 | 1,376 |
| `switch (__builtin_expect(mtype, 150))` | 1,304 | 554 | 1,372 |
| all `__builtin_expect` body anchors removed (plain) | 897 | 347 | 1,428 |
| `static const unsigned char kMissionRank[192]` in `Status.cpp`, flat rank switch | 1,219 | 392 | 1,332 |
| `static const unsigned char kMissionRank[192]` in `Status.cpp`, nested rank switch (inner only ranked) | 1,316 | 340 | 1,112 |
| static rank table in `Status.cpp` + rank labels (wave-185 form) | 1,200 | 183 | 1,436 |
| static rank table + `Status *st = Globals::status;` in case 10 | 1,005 | — | 1,432 |
| hardcoded rank table `0x20b977` (wave-185, manual data) | 449 | 160 | 1,432 |

### Key negative result: source case order is irrelevant

Reordering the 22 case bodies in the source to the original physical
order, or moving `case 172` to the front, produces **byte-identical**
`.text` (verified by md5 of the 0x598-byte function image).  clang
canonicalises the `SwitchInst` case order before codegen, so the
physical order is purely a `MachineBlockPlacement` function of the CFG
and branch weights, not of source order.

### Per-TU flag sweep (object-level, no link)

`-mllvm -enable-tail-merge=false`, `-tail-dup-placement=false`,
`-branch-fold-placement=false`, `-misched-postra=false`,
`-enable-post-misched=false`, `-enable-earlycse-memssa=false`,
`-likely-branch-weight=7/100/1000`, `-unlikely-branch-weight=1`,
`-disable-block-placement`: none changes the ascending body order.
`-disable-block-placement` changes the *pre-placement* layout
(`153,150,155,156,182,175,...`) but the final order is still ascending.

## 3. Why the rank table is the only 449 route

The original's bodies are in rank order, not case-value order.  A
`switch (kRank[mtype])` makes the case values themselves be the rank,
so `MachineBlockPlacement`'s value-sorted successor list is the
original order.  But a `static const` table in the same TU needs a PIC
address (`ldr r0,[pc,#imm]; add r0,pc; ldrb r0,[r0,r1]`), 4 bytes more
than the fixed-address `ldr r0,[pc,#imm]; ldrb r0,[r0,r1]` used by the
wave-185 hardcoded table.  That +4 shifts every body address by 4 and
takes raw from 449 to 1,200 (or 1,005 with a compensating local that
recovers the size).  Both are worse than the plain switch's 721.

No arithmetic rank function (offset/scale/negate) reproduces the
two-run order, and no C++11 construct other than a table can make the
case values non-monotonic.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13018
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch: `Status_base.cpp`, `order.py` (TBH decoder), `reorder.py`,
`flat.py`, `revnest.py`, `deepnest.py`, `ifchain.py`, `rankflat.py`,
`objorder.py`, `objsw.py`, `flagsw.py`.
