# `Status::missionCompleted` — wave 119 (w6815): the rank table is at its ceiling;
# the MBP fallback reproduces the body order but not the score

Status: **research note (no source change landed)**.  Worktree `wt/w6815`, branch
`wave119-w6815`, base master `3981b32c5`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
r328903, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool,
long long)`), original `0xabf38`, size **1432 B**.  Baseline row: **92.5 % / 1432 B**
(byte False, linked False).  Gate after this pass:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

No 1432 B variant beat the committed wave-117 rank table.  This note records the
measurements from this pass, including the decisive new one: **the forced
MachineBlockPlacement fallback reproduces the original body order exactly but
scores only 60.1 %**, because it does not reproduce the original's shared-tail
placement.  The rank table is therefore not just a workaround for the dispatch —
it is also the only shape that reproduces the original's *tail* layout.

## 1. Structural state (measured fresh in this worktree)

The wave-118 handoff numbers are stale.  A fresh `link_so` gives:

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 | **exact** |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | **exact** |
| `.ARM.extab` | 0x26298 | 0x26294 | −4 |
| `.rodata` | 0x41b67 | 0x41c27 | +0xc0 (192 = the rank table) |

27 section headers, exact file size 2,187,328 B.  The only differing section
bytes are `.text`, `.ARM.extab` (4 B) and `.note.gnu.build-id`.  `.text` is
already size- and base-exact; the rank table is the last `.rodata` input and
sits at the fixed address 0x20b977 (first byte past the original `.rodata`
image), so it does not shift any existing `.rodata` byte.

## 2. Committed source shape (unchanged)

`switch (MISSION_CASE[m->getType()])` with
`#define MISSION_CASE ((const unsigned char *) 0x20b977)`; the 192-byte table
lives in `tools/mission_case_table.cpp` (hidden visibility, linked last).  The
table maps type → rank:

```
150→0 151→1 152→2 153→3 154→4 155→5 158→6 162→9 166→10 170→11
184→12 189→13 156→14 157→15 160→16 163→17 164→18 165→19 168→20
167→21 169→22 171→23 174→25 172→39
0→200 8→208 11→211 13→213 14→214
```

The rank order (0,1,2,3,4,5,6,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,25,39)
is the original's big-body layout order, so the MBP worklist (ascending switch
value) reproduces the original body order.  The 167/174 shared body has two case
labels (frequency 512) and is placed first, exactly as in the original.

Current per-body size deltas (all other bodies size-exact): **166: 100→96 (−4)**,
**189: 60→68 (+8)**, **168: 30→26 (−4)**.  The net is zero, which is what keeps
the loop latch at the original's relative address.  The `(unsigned short)` cast
in case 20 (type 168) emits one `uxth` (2 B) and is a size device, not semantics.

## 3. Measurements this pass (all `-Oz`, project flags, full link + verify)

| variant | usize | score | note |
|---|---:|---:|---|
| **committed rank table** | **1432** | **92.5** | baseline |
| `best.cpp` (ranks 0..22+39) | 1432 | 91.7 | size-exact, TBH ratio 0.625 |
| forced MBP fallback (patched llc, plain `int` switch) | 1412 | **60.1** | original body order, wrong tails |
| plain `int` switch (no rank table) | 1424 | 58.4 | ascending layout |
| `shift2` (ranks 150..189) | 1436 | 75.4 | redundant `sub #150` after `ldrb` |
| `k_before0` (case-10 block before case 0) | 1436 | 77.3 | body 166 exact, layout shifts |
| `n_uxth` (drop the `(unsigned short)` cast) | 1432 | 74.9 | size kept, addresses shift |
| `x_rq` (re-query `getProductionGoodIndex`) | 1440 | 75.8 | body 166 exact, +8 total |
| `lowid` (big ranks 150..172, low ranks identity) | 1404 | 71.7 | small dispatch matches, TBH shrinks |
| `lowid2` (big ranks 15..38, low ranks identity) | 1400 | 67.6 | small dispatch matches, 24-entry TBH |
| `p25` (25 positional TBH matches) | 1404 | 70.1 | max positional matches, wrong size |
| `a23` (ranks = original types, 23 TBH matches) | 1456 | 58.4 | 5-instruction dispatch |

The **size** is the dominant factor: every variant whose total size is not
1432 shifts the body addresses and loses 15–35 points, even when a body or the
dispatch becomes byte-exact.

## 4. Why the forced fallback is not the answer (new, decisive)

A local LLVM 7.0.0/7.0.2 `llc` with `MBP_FORCE_FALLBACK=1` (i.e.
`selectBestCandidateBlock` returns `nullptr`) compiles the plain `int` switch
(`Status_plain.cpp`, real types, cases in the original A-then-B order) and
reproduces the original's **body-entry order exactly**:

```
167/174 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default(latch)
```

(decoded from the emitted TBH; the unpatched build gives ascending value order).
But the forced build is **1412 B and only 60.1 %**, not better than 92.5 %.
The reason is the shared tails: in the original, bodies 150/151/153/155 all
branch to the shared `0xac172` tail (`mov r0,sl; blx getStatusValue; cmp; blt; b`),
which sits immediately after body 155; in the forced build the shared tail is
placed after body 150 (0x224) and the other bodies branch back to it.  The
rank-table build reproduces the original's tail placement because the value
order equals the layout order, so the tail-merge pass makes the same decision.
The fallback is therefore necessary but **not sufficient** for byte-exactness,
and it is not a score win.

## 5. The TBH is capped at 18/40 positional matches

The emitted TBH is indexed by rank; the original's is indexed by type−150.  A
halfword at index R matches iff the body assigned rank R is the original's
type `150+R` body.  The layout order fixes the ideal rank sequence

```
150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165
 0   1   2   3   4   5   8  12  16  20  34  39   6   7  10  13  14  15
168 167 169 171 174 172
 18  17  19  21  24  22
```

whose longest increasing subsequence has length 16, so at most 16 body slots can
ever match; with the default slots the committed assignment reaches **18/40**
positional matches (TBH difflib ratio 0.725).  A hill-climb over rank sets
(`tbhopt2.py`, `tbhrand.py`) finds nothing better.  The measured TBH halfword
list is `95 98 9b b5 b8 c4 cd 23a 23a fb 12e 15e 165 182 1a4 1b9 1d3 1ea 1f8
1fa 1fe 28 20b 20d 23a 28 23a*13 21a` vs the original's
`95 98 9b b5 b8 c4 1a2 1b7 cd 23a 1d1 23a fb 1e8 1f6 1f8 12e 28 1fc 20b 160
20d 21a 23a 28 23a*9 167 23a*4 184`.

## 6. Remaining diff (full instruction stream: 523 vs 524, 92.5 %)

1. Dispatch: `sub.w r1,r0,#150; cmp r1,#39` vs
   `ldr r1,[pc,#804]; ldrb r0,[r0,r1]; cmp r0,#39` (+1 instruction, one extra
   literal-pool word at 0x3c8).
2. TBH halfwords: 22 of 40 differ (§5).
3. Small dispatch: `sub #8` / `cmp #0` vs `sub #208` / `cmp #200`.
4. Body 152: one `ldr r6,[sp,#16]` scheduling slot.
5. Body 166: `mov r9,r0` / `ldr r0,[r6,#400]` vs
   `mov r5,r0` / `mov r1,r6; mov r6,r0; ldr r0,[r1,#400]`; the original also
   re-queries `getProductionGoodIndex()` for `hasEquipment`, which is the −4.
6. Body 189: +8 = the two literal-pool words (`0x0020b977`, `0x00000296`) that
   ARMConstantIslandPass places inside the loop at 0x3c8/0x3cc.
7. `uxth` at case 20 (2 B size device).
8. The original's `nop` + `Globals::status` pool word at 0xac3f6/0xac3f8 vs our
   pool word at the end.

## 7. Conclusion — proven vs open

**Proven (built and measured):**

1. The committed rank table is the best shape found: 92.5 % at the exact 1432 B.
   Every other rank assignment or body tweak tried is worse, mostly because the
   total size moves off 1432 and the body addresses shift.
2. The forced MBP fallback reproduces the original body-entry order exactly but
   scores only 60.1 % (1412 B) because the shared-tail placement differs.
3. The TBH is capped at 18/40 positional matches by the LIS bound; the committed
   assignment attains it.
4. The remaining body deltas are 166 −4, 189 +8 (literal pool), 168 −4 (the
   `uxth` compensates part of it); they net to zero, which is required for the
   latch to stay at the original's relative address.

**Open:** a source shape that keeps the 1432 B total, moves the two literal-pool
words out of body 189 (so 189 becomes 60 B), and grows body 166 by 4 B (the
`getProductionGoodIndex` re-query) and body 168 by 4 B (the `nop` + pool) without
moving the latch.  The ARMConstantIslandPass placement of the `Globals::status`
GOT word is the only remaining lever; the dispatch cannot be made `sub #150`
without either the fallback (which loses the tails) or a 5-instruction
`ldr/ldrb/sub` sequence.

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields

# one-function score (fast, ~2 s): compile + link + verify only this symbol
/dev/shm/w6815/measure1.sh <Status.cpp> [<table.cpp>]
```

Scratch (not committed): `/dev/shm/w6815/` — `measure1.sh`/`score1.py` (fast
one-function score), `genrank.py` (type→rank table + case-label generator),
`chk.py`/`tbhobj.py` (decode the emitted TBH), `diffdetail.py` (per-instruction
diff), `Status_plain.cpp`/`.ll` (plain `int` switch), `pf.o` (forced-fallback
object), `r_*.cpp`/`r_*_table.cpp` (rank-scheme variants), `best.cpp`,
`shift2.cpp`, `k_before0.cpp`, `n_uxth.cpp`, `x_rq.cpp`, and the instrumented
LLVM 7.0.2 `llc` (`/tmp/llvm_android_702b/build2/bin/llc`, `MBP_TRACE`,
`MBP_FORCE_FALLBACK`).
