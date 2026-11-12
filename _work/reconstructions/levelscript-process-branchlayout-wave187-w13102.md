# LevelScript::process — wave 187 / w13102: branch-layout pass (branch map + mission-106 statement move)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13102`, branch `wave187-w13102`, base master
`8b5d4281c` (wave-186 harvest 6, raw **51,968**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13102`.

Continuation of the wave-186 w13002 branch-layout work.  This pass rebuilds the
complete branch/`tbh` map of both bodies, groups every branch whose
target/offset differs by state block, and re-tests the one structural
divergence (the mission-0 switch merge).  One size-exact,
`.rel.plt`-identical raw win was found and landed.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,968           51,966     (-2)
process aligned insns    20,856           20,857     (+1)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,727
.text diffbytes          74,274           74,272     (-2)
.ARM.extab diffbytes     647              647        (size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
file size                2,187,328        2,187,328      (exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

## 1. Branch census (both bodies)

`objdump -d` over `0x137d18..0x145ca8`, counting every control-transfer
instruction except `bl`:

| mnemonic | original | ours | delta |
|---|---:|---:|---:|
| b    | 369 | 369 | 0 |
| bcc  | 51  | 45  | -6 |
| bcs  | 3   | 8   | +5 |
| beq  | 200 | 189 | -11 |
| bge  | 18  | 24  | +6 |
| bgt  | 12  | 12  | 0 |
| bhi  | 22  | 21  | -1 |
| ble  | 4   | 2   | -2 |
| bls  | 4   | 4   | 0 |
| blt  | 139 | 132 | -7 |
| bne  | 182 | 179 | -3 |
| bx   | 8   | 8   | 0 |
| cbnz | 24  | 26  | +2 |
| cbz  | 65  | 72  | +7 |
| tbh  | 15  | 15  | 0 |
| total | 1116 | 1106 | -10 |

The mnemonic deltas are the same branch-shape (polarity) differences documented
in wave 185: `cbz`/`cbnz` (+9) vs `beq`/`bne` (-14), `bcs` (+5) vs `bcc` (-6),
`bge` (+6) vs `blt` (-7) are the same comparisons lowered with the opposite
polarity (pointer-vs-int test, `>=` vs `<`).  They are spread across the whole
per-mission switch and are not localised to one state block.

## 2. `tbh` dispatch map

Fifteen `tbh` tables in both bodies; the guard before each `tbh` gives the
range (`subs #lo; cmp #n` => `n+1` entries).  Multiset of entry counts:

* original: `{12, 5, 16, 16, 4, 10, 7, 4, 10, 4, 6, 10, 6, 10, 5}`
* ours:     `{12, 5, 16, 16, 13, 6, 4, 7, 6, 10, 4, 10, 6, 10, 5}`

Two tables differ, both by the same SimplifyCFG merge:

1. **mission-0 `switch (m_nState)`** — original `subs #6; cmp #9` (10 entries,
   6..15) with the state-16 handler as the out-of-line default at `0x13f902`;
   ours `subs #3; cmp #12` (13 entries, 3..15) because clang merged the
   preceding `else if (m_nState == 3)` arm into the switch.  This is the known
   w12702/w12802 divergence.
2. **mission-106 state switch** (`0x1392d0` ours) — original `cmp r0,#3`
   (4 entries, 0..3); ours `cmp r4,#5` (6 entries, 0..5).  Same merge pattern
   on a smaller chain, and it does not affect the raw metric measurably.

## 3. Branch-target map (instruction alignment)

An opcode-normalised full-stream alignment (registers erased, branch targets
replaced by target instruction index) matches **10,293** of 20,727
instructions.  Among matched branch pairs only **2** have a different relative
target offset:

```
0x137f66 bne.n rel -17  |  0x137f60 bne.n rel -16
0x137f9c cbz   rel +18  |  0x137f9a cbz   rel +19
```

Both are in the shared "active tail" (`m_bStartSequence` loop) and are the same
loop rotated differently (original condition at the bottom, ours at the top;
see the `getWingmen()` loop at source 1471-1479).  Everything else is block
placement + register allocation, not missing/extra branches.

Grouping the 432 call-anchored windows whose branch shape differs by the
source line of the preceding call gives 119 windows with a branch-count
difference spread over 117 distinct source lines; no single state block
dominates.  The largest single-window deltas are window-boundary artefacts
(the window ends at a different relative call), not structural differences.

## 4. Structural re-tests (not landed)

* **mission-0 arm order 4,3,5** (original emitted compare order: state 4,
  then 3, then 5).  Moving the state-3 arm before state-5 prevents the merge
  but measures raw **54,989**, size **57,240/57,232** (+8, gate-fatal) and
  `.rel.plt` **a8d49cee…** (DIFF).  This re-confirms w12702 on the current
  body.
* **4,3,5 with the state-5 `&&` split into a nested `if`** measures raw
  **55,388**, size **57,208/57,232** (-24, gate-fatal), `.rel.plt`
  **4e540b3b…** (DIFF).
* Replacing the `__builtin_expect(wingman->isWingMan(), 0)` in the active-tail
  loop with a plain `if` (or `expect(...,1)`) gives raw **52,048** (+80), so
  the expect set stays locally optimal (w13008).

The mission-0 switch merge therefore remains unfixable without a size and/or
`.rel.plt` regression.

## 5. Landed change

Mission-106 state-1 arm (source lines 1365-1375).  The local
`PlayerFighter *fighter = (PlayerFighter *) (*enemies)[0];` declaration is
independent of the `m_nState = 2;` store; emitting the store first shortens
the live range of `fighter` across the state store and saves 2 raw bytes:

```cpp
            if (((RadioMessage *) ((*messages)[2]))->isOver()) {
                m_nState = 2;
                PlayerFighter *fighter = (PlayerFighter *) (*enemies)[0];
                fighter->setCloakingPossible(false);
                ...
```

Raw **51,968 -> 51,966** (-2), `.text` 74,274 -> 74,272, size-exact,
`.rel.plt` byte-identical, `.ARM.extab` unchanged (647, 156,312 B).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13102
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51966 aligned= 20857 size= 57232/57232 insn= 20727/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51966 size=57232/57232 text=74272 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`parse.py`, `calls.py`, `windows.py`, `fullalign.py`,
`gaps.py`, `linemap.py`, `stmts.py`, `sweep.py`, `variant.py`, `armperm.py`)
live in `/home/debian/GalaxyOnFire2/scratch/w13102/`; they are scratch, not
tracked.
