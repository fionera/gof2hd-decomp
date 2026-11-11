# `MGame::OnTouchEnd` — wave 179 (w12315): `MenuTouchWindow::OnTouchEnd` returns `bool`

Status: **code win landed**, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w12315`, branch `wave179-w12315`, base
`64014ce6f` (wave-178 merge). Target `_ZN5MGame10OnTouchEndEiiPv`, original
8,080 B @ `0x17a144`. Scratch `/home/debian/GalaxyOnFire2/scratch/w12315`.

## Result

| | raw | aligned | size | `.rel.plt` | `.text` diff |
|---|---:|---:|---:|---:|---:|
| base | 3,261 | 1,185 | 8,080/8,080 | 0 | 77,936 |
| **landed** | **3,259** | **1,183** | **8,080/8,080** | **0** | **77,934** |

Gate after the change (full `check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact **843/843**, imports **207**,
lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
`.rel.plt` is byte-identical (29,392 B). `MenuTouchWindow::OnTouchEnd` itself
stays byte-exact (`raw=0`, 8,132/8,132). The whole `.text` delta is exactly
the 2 bytes below; no other function changed.

## The landed change

`MenuTouchWindow::OnTouchEnd` is declared/defined as returning `int`, but the
function only ever returns 0 or 1 (every `return` in the body is `return 0;`
except two `return 1;` sites). Change the return type to `bool` in
`src/game/ui/MenuTouchWindow.h` and `src/game/ui/MenuTouchWindow.cpp`:

```cpp
bool OnTouchEnd(int x, int y, void *touchId); // lint: void_ptr ...
```

The mangled symbol is unchanged (`_ZN15MenuTouchWindow10OnTouchEndEiiPv`;
return type is not part of the Itanium mangling), and the ABI is unchanged
(0/1 in `r0`).

## Why it works

At the `MGame::OnTouchEnd` call site the source is

```cpp
if (this->menuWindow->OnTouchEnd(x, y, touchId) == 1) {
```

With an `int` return type clang canonicalises `x == 1` to `x != 0` and emits
`cmp r0,#0; beq.w` (ours). With a `bool` return type the value range is known
to be `[0,1]`, so `x == 1` is kept as a one-bit test and clang emits
`cmp r0,#0; beq.w` on the *canonical* bool value, matching the original
instruction pair at `0x17aecc`:

```
orig  17aecc: 2800  cmp r0, #0
      17aece: f000 80bf  beq.w 17b050
ours  17aecc: 2800  cmp r0, #0
      17aece: f000 80bf  beq.w 17b050
```

Before the change ours was `cmp r0,#1; bne.w` at the same address (the
`0xd88..0xd8a` run in the wave-178 residual list). The two-byte opcode fix
removes that run; the rest of the cold-region block shift is unchanged.

## Sweeps that did not win

All size 8,080 / `.rel.plt` 0 unless noted; measured on the landed base.

* `count >= 0` spellings: `count > -1`, `0 <= count`, `amount <= 0`,
  `!(count < 0)` are codegen-identical to the base (3,259). `count > 0` /
  `amount < 0` give 3,261; `count != 0` / `count == 0`-else / `count` give
  6,588 (size 8,084). `count >= 0u` 5,324 (size 8,088).
* Sound-arm condition spellings: `!(cat || muted)`, `cat == 0 && muted == 0`,
  `!cat && muted == 0`, `(cat & 1) == 0 && !muted` are all codegen-identical
  (3,259). `muted == 0 && !cat` / `!muted && !cat` reorder the arms and cost
  size (8,088, raw 5,978). `else if (muted != 0)` is codegen-identical;
  `else if (muted == 1)` 3,992 (size 8,084); `expect(muted,1)` 4,378
  (size 8,096).
* `MenuTouchWindow::OnTouchEnd` return type: `uint8_t` gives 3,261 (no win);
  `bool` is the only spelling that produces the original `cmp #0; beq`.
* `case 15` explicit `goto finishMission;` 4,027 (size 8,108).

## Remaining residual (unchanged)

The dominant cold-region residual is still the sound Play block placement:
ours emits the `soundsMuted` Play block at `0x17b0bc` (before the
freelance/reward chain), the original at `0x17b1ee` (after it), shifting the
shared tail by `0x68` and the campaign chain by `0x3c`. This is an MBP
chain-worklist/readiness effect, not a source-order or branch-weight one; the
sound-arm source permutations above do not move it. The hot-region residuals
(`count` `bgt`/`beq` at `0x048b`, the 22-byte `String`-ctor scheduling at
`0x04a4`, the `.rodata` `"t "` pointer at `0x06b4`, and the deliberate
campaign/instant dispatch swap at `0x0e3a..0x0e7c`) are unchanged.

## Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w12315
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
# raw=3259 aligned=1183 size=8080/8080
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

Scratch helpers (all under `/home/debian/GalaxyOnFire2/scratch/w12315/`):
`measure.sh` (build + `fn_diff`), `tryvar.py` (apply a source variant, build,
measure, revert), `dump.py` (same-address diff runs / side-by-side), `mbp7.cpp`
(LLVM 7 `MachineBlockPlacement.cpp` for the chain-worklist analysis),
`mbp_ontouchend.txt` / `blockorder.txt` (MBP MIR and layout order).
