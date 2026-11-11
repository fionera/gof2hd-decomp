# LevelScript::process — wave 173 / w11806: outer dispatch, state-arm order, mission-5 deactivation

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11806`, branch `wave173-w11806`, base master `3fafa5b03`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11806/`.

Follow-up to `levelscript-process-blockorder-wave171-w11706.md` and
`levelscript-process-mission0-state8-tail-wave172-w11703.md`.

## Result (no source change landed; note only)

```
                         base (HEAD)      this wave
process raw diffbytes    53,750           53,750     (no change)
process aligned insns    19,949           19,949     (no change)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,169 / 20,064  20,169 / 20,064
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
RATCHET                  PASS             PASS
```

`raw` is the byte-for-byte count over the function body (literal pools included).
`aligned` here is the positional normalized-instruction diff count produced by
`tools/verify/verify.py`'s `disassemble()` + `reconcile()` (the older wave notes
used a separate `align_diff.py`, so the absolute number is not comparable; the
relative ordering measured in this wave is).

## 1. Baseline layout (why the raw number is layout-dominated)

The prologue is still the original/ours permutation documented in
`levelscript-process-prologue-r4-wave158-w10401.md` and
`levelscript-process-tail-inline-wave171-w11705.md`:

```
original                              ours (base)
mov sl, r0        this -> sl          mov fp, r0        this -> fp
ldr.w fp,[r0]     canary -> fp        ldr.w sl,[r0]     canary -> sl
ldr r4,[r0]       &Globals::status    ldr.w r9,[r0]     &Globals::status
ldr r0,[r4]       status value        ldr.w r4,[r9]     status value
                                      mov r0,r4         (extra 2 B)
```

The extra `mov r0,r4` makes the prologue +4 B, and the mission-0 3-ship loop
is 12 B short (the original hoists `enemies` into `r9`; ours spills it to
`[sp+96]` because `r9` holds `&Globals::status`).  The shared tail therefore
starts at `0x137f76` instead of `0x137f7c` (-6 B), and every later instruction
is positionally shifted.  That shift is the dominant raw-diff contributor:
the byte-diff is ~95% in every 4 KiB chunk after the prologue, even though the
switch dispatch chain is instruction-for-instruction identical.

## 2. Measurements (all full link + size checked)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| **base (HEAD)** | **57,232** | **53,750** | **19,949** |
| drop `Status *status` local, use `Globals::status` (nolocal) | 57,232 | 53,751 | 19,792 |
| mission-50 condition order `mission == 50 && messages` | 57,232 | 53,751 | 19,949 |
| wormhole body `count >= 3` tested first | 57,232 | 53,753 | 19,949 |
| mission-50 order + count-first | 57,232 | 53,754 | 19,949 |
| whole-condition `expect(deactivation,1)` (no separate if) | 57,232 | 53,775 | 19,937 |
| state arms source order `4,3,5` | 57,232 | 54,468 | 19,957 |
| state arms source order `3,5,4` | 57,232 | 54,119 | 19,947 |
| state arms source order `3,4,5` | 57,248 | 55,258 | 19,938 |
| state arms source order `5,4,3` | 57,252 | 55,489 | 19,963 |
| state arms source order `5,3,4` | 57,252 | 55,489 | 19,963 |
| separate `if` for mission 5 (`sepif`) | 57,200 | 55,440 | 19,926 |
| sepif + whole-condition `expect(deactivation,1)` | 57,200 | 55,431 | 19,913 |
| sepif + whole-condition expect + nolocal | 57,200 | 55,434 | 19,714 |
| sepif + whole-condition expect + arms `3,5,4` | 57,184 | 54,813 | 19,856 |
| sepif + whole-condition expect + arms `4,3,5` | 57,200 | 55,337 | 19,901 |
| sepif + no expect on deactivation | 57,208 | 55,276 | 19,902 |
| no expect on deactivation (base shape) | 57,240 | 55,092 | 19,920 |
| mission-5 chain order `5,50,26,51` (base shape) | 57,240 | 55,259 | 19,840 |
| mission-5 chain order `5,26,51,50` (base shape) | 57,252 | 54,792 | 19,948 |
| mission-5 chain order `5,51,26,50` (base shape) | 57,256 | 55,276 | 19,966 |
| guard-local `Status *guardStatus` in the switch guard | 57,224 | 53,876 | 19,938 |
| guard-local + no expect on deactivation | 57,232 | 54,978 | 19,929 |
| body-local `Status *status` for the whole body (`&status` -> r4) | 57,252 | 55,463 | 19,929 |
| body-local + faithful 3-ship loop (`(*enemies)[i]` twice) | 57,088 | 55,314 | 19,850 |
| `Level *self = m_pLevel` alias, n=2 uses | 57,240 | 55,100 | 19,935 |
| `Level *self` alias, n=9 uses | 57,300 | 54,984 | 19,940 |

## 3. Findings

1. **The separate `if` for mission 5 is the faithful outer shape.**  With
   `if (mission == 5)` instead of `else if`, and a whole-condition
   `__builtin_expect(deactivation, 1)`, the deactivation block stays inline
   and mission 5 lands at `0x138122` — only 8 B before the original's
   `0x13812a`.  But the function shrinks to 57,200 B (-32), so every later
   case body shifts and raw rises to 55,431.  The missing 32 B is not a
   single block: the switch guard moves from `0x13879e` to `0x138294` and the
   mission-26/Chain-A interleave changes.

2. **The original block order is `5, 81, 26, 78, 50, 89, 51, guard`.**  The
   base emits `81, 5, 78, 26, 89, 50, 51, guard`; the sepif shape emits
   `5, 26, 81, 50, 51, guard, 78, 89`.  No source permutation of the
   mission-5/26/50/51 chain alone reproduces the original interleave, because
   the interleave is MachineBlockPlacement of the two independent chains
   (Chain A is the top-level `else`; the specials chain is after the shared
   tail).  The original's `5,81,26,78,50,89,51` alternation is the placement
   result, not source order.

3. **The state-arm compare order is not source-controllable.**  The original
   mission-0 dispatch at `0x1389c8` compares `4,3,5` with the state-5 body
   last.  Base source order `4,5,3` emits `5,4,3`; source `4,3,5` emits
   `3,5,4`; source `3,5,4` emits yet another order.  All six permutations
   were measured; none beat base raw 53,750 (best non-base: `3,5,4` at
   54,119).

4. **The body-local `Status *status` shape does put `&Globals::status` in
   `r4` and `this` in `sl`**, matching the original's two key callee-saved
   registers.  But it also moves the canary to `r9`, shrinks the frame to
   424 B, and grows the function to 57,252 (+20).  Its raw is 55,463 because
   the +20 shift and the extra stack reloads outweigh the register match.
   The faithful 3-ship loop on top of it is 57,088 (-144), raw 55,314.

5. **The switch guard is structurally identical to the original** (same
   `getMission()->isCampaignMission()` / `isEmpty()` / `mission == 42`
   sequence, same range checks `131..145` and `64..73`, same linear compare
   chain `80,91,92,94,102,105,114,125,154,157,158`).  The only differences are
   the status register (`r9` vs `r4`) and the guard's cached `r6` vs the
   original's reload.  A guard-local `Status *guardStatus` shrinks the
   function by 8 B and does not help raw.

6. **The mission-5 deactivation block is instruction-for-instruction
   identical** apart from the `sl`/`fp` register swap and the `ittt` vs
   separate `cmp` shape produced by the `__builtin_expect(mission >= 2, 1)`
   hint.  Removing the hint restores the original's separate `cmp` shape but
   grows the function by 8 B (57,240) and raw rises to 55,092.

## 4. Conclusion

No source change beat base raw 53,750 while staying exactly 57,232 B.  The
remaining lever is still the prologue register allocation: getting
`&Globals::status` into `r4` *and* the canary into `fp` (so `this` stays in
`sl` and `r9` is free for the 3-ship loop's `enemies` hoist) would align the
mission-0 body and the whole post-tail stream.  Every shape measured this
wave that reaches `&status` -> `r4` either changes the size (body-local,
+20) or moves the canary and re-colours the rest of the prologue.  A genuine
size-exact companion for the body-local shape is the next step.

Tree left at base; `git status --porcelain` clean except this note.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -- -j2
python3 /home/debian/GalaxyOnFire2/scratch/w11806/score.py
# -> size 57232/57232 raw 53750 aligned 19949 insns 20169/20064
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Scratch harnesses: `score.py` (raw/aligned/size), `search.py`,
`search2.py`, `search3.py`, `search4.py`, `search5.py`, `search6.py`,
`chain.py`, `selfsweep.py`, `combo.py`.
