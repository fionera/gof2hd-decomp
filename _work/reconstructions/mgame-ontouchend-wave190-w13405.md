# MGame::OnTouchEnd — wave 190 (w13405): original chain order, IR-level mining-loop
# analysis, `__builtin_expect` sweep, pair/4-swap search

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13405`, branch
`wave190-w13405`, base master `dcb6b809c`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13405`.

Prior notes: `mgame-ontouchend-wave189-w13318.md` (full body), `-w13323.md` (96/127 split),
`-w13344.md` (split compensation), `-w13357.md` (escort win), `-w13374.md` /
`-w13378.md` (finish-block + escort permutations, body reconstruction).

## Result

**No size-exact raw win found. `src/` is unchanged (base).** This note records the
new angles measured so the next pass does not repeat them, plus one concrete new
finding (the mining-loop IR lowering and the exact source of the sorted-order `+4`).

| metric | base |
|---|---:|
| raw same-address diff bytes | 2,913 |
| aligned (instruction) diff | 1,706 |
| function size | 8,080 / 8,080 |
| `.text` differing bytes | 72,999 |
| `.ARM.extab` size / diff | 156,312 / 603 |
| `.ARM.exidx` diff | 0 |
| `.rodata` content diff | 0 |
| file size | 2,187,328 exact |

## 1. The original chain order really is the emitted order — but it is a +4 / raw-4,285 basin

Reordering the source chain to the original's emitted test order
`15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162` makes the
emitted `cmp r0,#N` sequence match the original **exactly** (verified at
`0x17b154…0x17be36`, all 18 tests in order). It is nevertheless **size 8,084
(+4), raw 4,285**, aligned 1,346 (better than the base's 1,706).

The `+4` is *not* a branch-width flip and *not* the literal pool: it is one extra
`strb.w r4, [fp, #200]` (the `field_0xc8 = 0` store) emitted at `0x17bfe0` in the
isMining block. Reordering the chain changes MachineBlockPlacement's tail-merge,
which duplicates that 4-byte store into a second block. The whole pre-chain region
then differs only by the resulting `+4` shift of the shared epilogue
(`0x17b008` vs `0x17b004`), i.e. ~30 one/two-byte branch-target diffs.

The natural ascending source order (`42` before `43`, `74` before `81`) does **not**
reproduce the original's `43,42` / `81,74` emitted order: clang emits the source
order verbatim. Size 8,096, raw 4,350.

Hill-climbing single moves from the sorted order converges to
`22,43,42,65,81,74,15,95,96,100,110,126,134,144,120,155,161,162`
at **raw 3,515 / 8,080** — still far worse than the base's 2,913. The base's
scrambled order is not a "wrong" order that can be replaced by the faithful one;
it is a byte-matching coincidence basin (see §5).

## 2. Why the base wins: it aligns *similar* bodies, not the same bodies

Body-start address deltas (ours − original) for the base order
`74,15,162,43,42,22,65,120,96,100,95,110,81,134,144,126,155,161`:

```
 42  +0     144  +0     95 body == original 110 body (both 0x17ba3c)
161 body == original 162 body (both 0x17be4a)
120  +2 (0x17b6a4 vs original 95 body 0x17b6a6)
```

Because all the campaign bodies are near-identical (HP/Shield/Armor/Gamma +
`departStation` + module), a permuted body can land on a *different* original
body's address and still match most bytes. That is why the faithful order is
raw-worse: it puts each body at its own address, where the neighbouring bytes are
different. Any future search must optimise this packing, not fidelity.

## 3. Pair-swap / adjacent-swap / random 4-swap sweeps: base is a local minimum

- Full 153 pair-swap sweep from the base: best **2,913** (two ties:
  `74,43,162,15,…` and `74,15,43,162,…`; the first keeps `.ARM.extab` diff 603,
  the second blows it up to 7,809).
- Full 17 adjacent-block-swap sweep: best 2,913 (swap 162↔43), everything else
  ≥ 2,916.
- 150 random 2-swap (4-position) perturbations: best 2,913, no improvement.
- Same pair-swap sweep on top of the faithful `switch_to_target_setting`-before-
  `active` variant (§4): best 2,914.

## 4. Small `won`-block fidelity tweaks are raw-neutral or +1

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| base | **2,913** | 1,706 | 8,080 | 603 |
| `switch_to_target_setting = 2;` before `active = 0;` (original store order) | 2,914 | **1,702** | 8,080 | 603 |
| `setFreelanceMission` before `showMissionRewardMessage` (original order) | 2,915 | 1,706 | 8,080 | 603 |
| both of the above | 2,916 | 1,702 | 8,080 | 603 |
| instantAction-first restructure (`else if` or flat `if`) | 4,706 | 2,104 | **8,084** | 1,324 |

The store-order fix is a genuine fidelity improvement (aligned 1,706→1,702) but
costs +1 raw, so it is not landable under the raw-first gate.

## 5. Mining-loop condition: new IR-level explanation

Compiling `MGame.cpp` to `-emit-llvm` (scratch `mgame_base.ll`, `mgame_ne0.ll`)
shows the base's `if (count >= 0)` is InstCombine'd to

```
%267 = sub nsw i32 0, %242        ; count = -amount
%268 = icmp slt i32 %242, 1       ; amount < 1
br i1 %268, label %269(body), label %343(skip)
```

while every equality spelling (`count != 0`, `amount != 0`, `count`, `!!count`,
`(unsigned)count != 0`, `(short)count != 0`, `count == 0`-with-empty-then,
`count` declared inside the `if`, a second `cnt2` copy, …) becomes

```
%268 = icmp eq i32 %242, 0
br i1 %268, label %343(skip), label %269(body)
```

The inverted branch polarity makes the ARM backend pick `%267` (the negated
counter) as the induction variable and canonicalise the loop to `add r8,#1`
instead of `sub r9,#1`. That flips the function-wide `sl`/`r9` allocation
(`sl=this` → `r9=this`) and grows the function to **8,088 (+8), raw 6,592**.
`amount < 0`, `amount <= -1`, `count > 0` stay size 8,080 but lower to
`cmp r9,#-1; bgt` (raw 2,915). `count > -1`, `amount < 1`, `amount <= 0` are
byte-identical to the base (raw 2,913). So the original's `beq.n` is unreachable
without the +8 allocation flip.

## 6. `__builtin_expect` sweep on the chain tests: all worse

Adding `__builtin_expect(mission->isCampaignMission(), 1)` to each chain test
individually (17 builds): raw 3,874–4,786, size 8,068–8,096. Adding
`__builtin_expect(getCurrentCampaignMission() == N, 0)`: raw 4,459–4,765, size
8,080–8,096 (only N=42 lands at 8,080, raw 4,562). Branch-weight changes shift
the whole chain layout; none helps.

## 7. Other re-confirmations

- `count != 0` / `amount != 0` / `String(-amount)` still 8,088 / raw 6,592.
- Sorted chain + original `won` shape + 96/127 split after 126: 8,088, raw 6,068,
  extab 1,318.
- The 22-byte `String("t ")` schedule at `+0x4a4` is unchanged and still the only
  pre-chain source diff besides the `count` branch; the IR is a clean left-assoc
  `((("\n"+count)+"t ")+getText(...))` chain, so the difference is pure backend
  frame-index scheduling (ours hoists `add.w r8,sp,#80` above the second
  `operator+`, the original places it after the literal load).

## Reproduction

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13405/`): `gen.py` (chain-order
generator with `96s`/`127*` split tokens), `won.py`, `measure.sh` / `measure2.sh`
(raw+size+extab), `secdiff.py`, `chainmap.py`, `idiff.py`, `mapdiff.py`,
`sweep_pairs.py`, `sweep_pairs2.py`, `sweep_rand4.py`, `sweep_expect.py`,
`mine_sweep{,2,3}.py`, `won_sweep.py`, `hill.py`. All builds `ninja -j2` under
`ulimit -v 12000000`.
