# LevelScript::process — wave 171 / w11706: outer dispatch + wingman loop follow-up

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11706`, branch `wave169-w11706`, base master `41ef4c8b6`.
Follow-up to `levelscript-process-blockorder-wave170-w11606.md`.

## Baseline (HEAD, tree left clean)

```
process raw diffbytes    53,812
process aligned insns    17,932
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20724
```

## Measurements (all size-checked; no source change landed)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **53,812** | **17,932** | **57,232** |
| first-loop expect removed (w11606) | 53,827 | 17,928 | 57,232 |
| drop local `Status *status`, call `Globals::status` directly | 53,813 | 17,928 | 57,232 |
| both of the above | 53,828 | 17,923 | 57,232 |
| shared exit-wormhole body: `count >= 3` tested first | 53,815 | 17,929 | 57,232 |
| mission-50 condition order swapped | 53,815 | 17,929 | 57,232 |
| state arms permuted 4,3,5 (source) | 54,513 | 18,464 | 57,232 |
| state arms permuted 3,5,4 (source) | 54,448 | **17,839** | 57,232 |
| separate `if` for mission 5 (w11606 shape) | 55,207 | 18,343 | 57,156 |
| separate `if` + whole deactivation condition `expect(...,1)` | 55,207 | 18,182 | 57,156 |
| reload `(*enemies)[i]` for both 3-ship loop calls | 55,554 | 22,206 | 57,344 |

## Findings

1. **Prologue is +6 B, not +2 B.**  Ours keeps `&Globals::status` in `r9`
   (`ldr.w r9,[r0]` + `ldr.w r0,[r9]`), the original uses `r4`
   (`ldr r4,[r0]` + `ldr r0,[r4]`).  Dropping the `Status *status` local makes
   the compiler emit `ldr.w r9,[r0]; ldr.w r0,[r9]` (no extra `mov r0,r4`),
   shrinking the prologue from +6 to +4 B.  Raw +1 only because the whole
   tail then shifts by another 2 B.

2. **The 12 B mission-0 shortfall is entirely the 3-ship `for` loop.**
   Original loop `0x137f24..0x137f66` = 66 B: it hoists `enemies` into `r9`
   and reloads `(*enemies)[i]` for both `setDirection` and `setVisible`.
   Ours `0x137f2a..0x137f60` = 54 B: it reloads `enemies` from `sp+96` inside
   the loop and caches `KIPlayer *e` in `r4` for the second call.  Writing
   `(*enemies)[i]` twice makes the loop body instruction-for-instruction
   faithful, but the extra live value changes the global register allocation
   (`sp+96 -> sp+88`, vector slots `0x170/0x118 -> 0x168/0x110`), grows the
   function by +112 B and raises raw to 55,554.  The missing piece is getting
   the compiler to hoist `enemies` into `r9` instead of spilling it; that is
   blocked by `r9` already holding `&Globals::status`.

3. **Separate `if` for mission 5 is the faithful outer shape** (original tail
   falls through to `0x13812a`), and with a whole-condition
   `__builtin_expect(...,1)` the deactivation block stays inline and mission 5
   lands at `0x138122` (only -8 B from the original).  But the switch guard
   moves to `0x13828c` and the function shrinks to 57,156 B, so raw rises to
   55,207.  Not landed (size is sacred).

4. **State-arm source order is not the compare order.**  Base source order
   `4,5,3` emits compares `5,4,3`; source `4,3,5` emits `3,5,4`; source
   `3,5,4` emits a different order again.  The original compare order is
   `4,3,5` (`0x1389c8`) with the state-5 body last (`0x138fd8`).  Permutation
   `3,5,4` gives the best aligned count seen (17,839) but raw 54,448.

## Conclusion

No source change beat HEAD's raw 53,812.  The next lever is the 3-ship loop's
`enemies` hoist: if `r9` can be freed (so the compiler keeps `enemies` in it
instead of `&Globals::status`), the loop becomes 66 B, the tail tag lands at
`0x137f7c`, and the whole post-tail body realigns.  Tree left at HEAD.
