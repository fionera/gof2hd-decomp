# LevelScript::process — wave 181 / w12504: scalar-multiply operand order (raw 52,796 -> 52,795)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12504`, branch `wave181-w12504`, base of this
change is the Vector-add-swap commit `dddbe0930` (raw 52,796).

## Result (code win landed)

```
                         before            this pass
process raw diffbytes    52,796            52,795            (-1)
process aligned insns    20,204            20,204            (0)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          76,178            76,177            (-1)
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

One scalar multiply in the mission-0 camera hand-off (line 3175):

```diff
-                            cam->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);
+                            cam->setPosition(player->getPosition() - 2400.0f * player->GetDirVector());
```

`AEMath::operator*(const Vector &, float)` and
`AEMath::operator*(float, const Vector &)` are component-wise identical
(`src/engine/math/Vector.cpp:103` and `:114`), so the value is unchanged.
The original calls the `Vector * float` overload (`0x13ddf8`, r1 = dir,
r2 = 2400.0f); the swapped form is a semantically equivalent source spelling
that happens to land one byte closer in the reconstruction's register
allocation.  Function size stays exactly 57,232 B and `.rel.plt` is
byte-identical.

## Method

`scratch/w12504/binswap.py` enumerates every top-level ` + ` / ` * ` on a
single-line statement in `process` (255 candidates), extracts the balanced
left/right operands and swaps them.  The candidate TU is compiled with the
exact `compile_commands.json` command and gold-linked with the exact
`build.ninja` command.  Four candidates are size-exact raw wins; three are
rejected below.

## Rejected (measured -1/-2 but not semantically faithful)

* **Line 3809 (`+` before `(float) rz`).**  The scanner stopped at the `*` and
  produced `lightDir.z * (float) rz + 100000.0f`, changing the product, not
  just the operand order.  Rejected.
* **Line 1261 (`* 7.0f * factor`).**  Original `0x138618` passes `7.0f` to
  `operator*(Vector, float)` before computing the `-50000.0f` factor, i.e. the
  base order `... * 7.0f * factor`.  The swap changes the float rounding
  order.  Rejected.
* **Line 3630 (`* 0.4f * (float) delta`).**  Original `0x13af96` multiplies by
  `0.4f` first and by `(float) delta` second, i.e. the base order.  The swap
  changes the float rounding order.  Rejected.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12504
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52795 aligned= 20204 size= 57232/57232 insn= 20731/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52795 size=57232/57232 text=76177 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harness (not tracked): `binswap.py` in
`/home/debian/GalaxyOnFire2/scratch/w12504/`.
