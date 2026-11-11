# LevelScript::process — wave 181 / w12504: Vector-add operand swaps (raw 52,798 -> 52,796)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12504`, branch `wave181-w12504`, base of this
change is the one-use-inline commit `463fdae4f` (raw 52,798).

## Result (code win landed)

```
                         before            this pass
process raw diffbytes    52,798            52,796            (-2)
process aligned insns    20,176            20,204            (+28)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          76,180            76,178            (-2)
.ARM.extab diffbytes     713               713               (size/base exact)
.rel.plt                 md5 289b2a06…     md5 289b2a06…     (byte-identical)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
symbol parity            5370/5370         5370/5370
RATCHET                  PASS              PASS
```

## The landed source change

Two `Vector` additions have their operands swapped so the `Vector` literal is
evaluated first.  `operator+` is a free commutative function taking
`const Vector &`, and both operands are side-effect-free, so the value is
unchanged; only the argument-evaluation order changes.

```diff
@@ mission-125 escort scatter loop (line 3277) @@
-                                ship->setPosition(player->getPosition() + Vector{(float) x, 3000.0f, 20000.0f});
+                                ship->setPosition(Vector{(float) x, 3000.0f, 20000.0f} + player->getPosition());

@@ mission-130/131 group-8 revive loop (line 4148) @@
-                        (*enemies)[i]->setPosition(player->getPosition() + Vector{(float) xOff, 5000.0f, 25000.0f}); // vtable+0x44
+                        (*enemies)[i]->setPosition(Vector{(float) xOff, 5000.0f, 25000.0f} + player->getPosition()); // vtable+0x44
```

Each swap is a -1 raw win individually (52,797) and they are additive
(52,796).  Function size stays exactly 57,232 B and `.rel.plt` is
byte-identical.

## Method

`scratch/w12504/opswap.py` finds every single-line `<expr> + Vector{...}` in
`process` (26 candidates), extracts the balanced left operand and the full
`Vector{...}` literal, and rewrites it as `Vector{...} + <expr>`.  The
candidate TU is compiled with the exact `compile_commands.json` command and
gold-linked with the exact `build.ninja` command; raw/size/`.rel.plt` are
measured from the resulting ELF.  Two candidates are size-exact raw wins; the
rest are neutral (52,798) or build failures from the deliberately crude
operand scanner.

## Rejected

* The same sweep over the 52,796 tree finds no further `+ Vector{...}` win.
* `*` operand swaps and non-literal `+` operand swaps are not part of this
  pass.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12504
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52796 aligned= 20204 size= 57232/57232 insn= 20731/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52796 size=57232/57232 text=76178 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harness (not tracked): `opswap.py` in
`/home/debian/GalaxyOnFire2/scratch/w12504/`.
