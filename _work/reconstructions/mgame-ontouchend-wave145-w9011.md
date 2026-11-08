# MGame::OnTouchEnd — wave 145 (w9011): type12 chain flips behind bb.205 via `failed == 1`

Status: source change landed (METRIC-ONLY layout reshape), 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9011`, base `eb13ada9b` (wave 144). Owned TU
`src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w9011`.
Full `check` green before and after: byte **4507**, linked **4507**, size
**4524/4524**, data **843/843**, imports **207**, lint **CLEAN**, RATCHET PASS.

Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base | landed |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4399 | **4691** |
| type12 chain (`cmp r5,#12`) | `0x17af8a` | **`0x17b03a`** |
| mission-selection (`isInstantActionMission`) | `0x17b008` | **`0x17af98`** |
| cargo (`getCargo`) | `0x17b0b6` | **`0x17b34a`** |

Original: type12 `0x17b096`, mission-selection `0x17af7e`, cargo `0x17b374`.

## 1. Mission / result

The base places the type12 chain (failed-branch `getType()==12`) **before** the
won-branch mission-selection chain; the original places it **after** it.  A
semantically-equivalent reshape of the `if (failed)` test flips the block order:

```cpp
-                bool failed = Globals::status->getMission()->hasFailed();
-                if (failed) {
+                int failed = Globals::status->getMission()->hasFailed();
+                if (failed == 1) {
```

`hasFailed()` returns `unsigned char`; `if (failed)` compiles to
`icmp eq i8 %x, 0` and gets `BranchProbabilityInfo`'s zero heuristic
(20/32 = 62.5 % not-taken).  `failed == 1` is a non-zero constant comparison, so
the zero heuristic does not apply and the edge is 50/50.  That small weight
change is enough to move the type12 chain (bb.198) out of the early position and
behind the mission-selection chain (bb.205), matching the original's relative
block order.  `char failed` / `unsigned char failed` with `== 1` give the
identical layout (4691); `int failed` with `!= 0`, `> 0`, or plain `if (failed)`
revert to the base layout (4399).

**METRIC-ONLY / LAYOUT reshape**: the witnessed original at `0x17ab38` is
`beq.w 17adc8` (won branch is the `else` of `if (failed)`), i.e. the plain
`bool failed; if (failed)` form.  `== 1` is semantically identical for a 0/1
`unsigned char` but is not the witnessed source.  The coordinator may prefer the
faithful base form at 4399.

## 2. Why the raw diff still rises (4691 vs 4399)

The flip is correct but incomplete.  In the landed build the type12 chain lands
at `0x17b03a`, i.e. **0x5c (92 B) before** the original's `0x17b096`, because the
`hudTouchFlags` join block is missing from the region between the
mission-selection chain and the type12 chain:

* original `0x17af5a..0x17b07e` = `0x124` B (includes the join block
  `0x17afd8..0x17b04e`, ~`0x78` B, then the ChoiceWindow/`fp,#350` blocks);
* landed `0x17af74..0x17b022` = `0xae` B (ChoiceWindow/`fp,#350` blocks but no
  join block; the join block stays late at `0x17b93c`).

The base's raw 4399 is partly a *lucky* tail alignment: its late join block sits
at `0x17b93c`, exactly where the original's `==155` test is, so the
`0x17bd84..0x17c0c4` tail matches (window diffs 4/36/8/5).  The flip shifts the
tail and loses that (~180 B), so the campaign-region win is not yet realised.

Measured shift sweep over `0x17ad44..0x17bd84` (ours shifted -8..+8 vs orig):
best 3950 B at shift +2, i.e. the region is genuinely reordered, not a pure
shift.

## 3. Variants measured this wave (all 8080 B unless noted)

| variant | raw | t12 | mis | cargo |
|---|---:|---|---|---|
| base | **4399** | 17af8a | 17b008 | 17b0b6 |
| `int failed; if (failed == 1)` (**landed**) | 4691 | 17b03a | 17af98 | 17b34a |
| `char failed; if (failed == 1)` | 4691 | 17b03a | 17af98 | 17b34a |
| `unsigned char failed; if (failed == 1)` | 4691 | 17b03a | 17af98 | 17b34a |
| `int failed; if (failed != 0)` | 4399 | 17af8a | 17b008 | 17b0b6 |
| `int failed; if (failed > 0)` | 4399 | 17af8a | 17b008 | 17b0b6 |
| landed + `keepObjectives hasWon()==1` | 4686 | 17b03a | 17af98 | 17b34a |
| landed + `won` bare | 5888 | 17b092 | 17b00a | 17b3a2 |
| landed + `won` bare + `keepObjectives==1` | 5867 | 17b094 | 17b00c | 17b3a4 |
| landed + `won` bare + `inst==1` | 4731 | 17b03e | 17af98 | 17b40a |
| landed + `goingToWaypoint()==1` | 4804 | 17b03a | 17af98 | 17b34c |
| landed + `int mtype` local | 5175 | — | 17af86 | 17b042 |
| landed + `Mission *fm` local | 5192 | — | 17af86 | 17b300 |
| `__builtin_expect(failed,0)` | 5115 | 17bf08 | 17ae94 | 17bf4a |
| `__builtin_expect(failed,1)` | 5138 | 17aefe | 17b560 | 17af9a |
| `__builtin_expect(failed==1,0/1)` | 5115/5139 | 17bf08/17aefe | 17ae94/17b560 | 17bf4a/17af9a |
| `won = __builtin_expect(won,1)` | 4734 | 17af8c | 17affc | 17b0b6 |
| `__builtin_expect(hudTouchFlags!=0,1)` | 7328 | — | 17afac | 17b3f2 |
| `__builtin_expect(hudTouchFlags!=0,0)` | 7308 | 17af5e | 17aecc | 17b366 |
| `if (hudTouchFlags==0)` (inverted) | 7133 | 17aea2 | 17af0a | 17afb2 |

`__builtin_expect` on the join test (all four spellings) destroys the layout
(7100-7400); it is not the lever.

## 4. Open questions

1. The remaining `0x5c` gap is the `hudTouchFlags` join block.  A shape that
   places the join block between the mission-selection chain and the type12
   chain (original `0x17afd8..0x17b04e`) would close the gap and likely drop the
   raw diff far below 4399.  `__builtin_expect` on the join test overshoots.
2. `won` bare (`hasWon()` instead of `hasWon() == true`) puts the type12 chain at
   `0x17b092` — 4 B before the original — but re-lays the whole first half
   (raw 5888).  A +4 B faithful companion that shifts the won branch back is the
   other near-miss lead (see wave 144 `swap_int + won2 bare`).
3. `0x17a5c4` (25 diff bytes) is a separate residual: `cmp.w r9, #0` vs
   `cmp.w r9, #4294967295` at `0x17a5ca`, unrelated to this chain.

## 5. Reproduction

```
cp /dev/shm/w9011/MGame.base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9011/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4399 / 8080
python3 /dev/shm/w9011/try.py failed_i1                      # 4691 / 8080
python3 /dev/shm/w9011/batch.py F1 W0,W1,W2,W3 K0,K1,K2 I0,I1,I2
python3 /dev/shm/w9011/batch2.py     # __builtin_expect on failed / won
python3 /dev/shm/w9011/batch3.py     # __builtin_expect on the join test
python3 /dev/shm/w9011/batch4.py     # type12 local / goingToWaypoint combos
python3 /dev/shm/w9011/batch5.py     # won spellings
```

Scratch: `/dev/shm/w9011/{fdiff.py,try.py,variants.py,batch*.py,windows.py,
shift.py,orig_full.txt,ours_full.txt,post_bpp.txt,bfi*.txt,MGame.base.cpp}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
