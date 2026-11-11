# LevelScript::process — wave 181 / w12504: one-use `playerPos` inline (raw 52,799 -> 52,798)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12504`, branch `wave181-w12504`, base of this
change is the declaration-order commit `a2de1d116` (raw 52,799).

## Result (code win landed)

```
                         before            this pass
process raw diffbytes    52,799            52,798            (-1)
process aligned insns    20,178            20,176            (-2)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          76,181            76,180            (-1)
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

In the mission-130/131 group-8 escort revive loop, the one-use `Vector`
temporary is folded into its only consumer:

```diff
                         (*enemies)[i]->revive();          // vtable+0x18
-                        Vector playerPos = player->getPosition();
-                        (*enemies)[i]->setPosition(playerPos + Vector{(float) xOff, 5000.0f, 25000.0f}); // vtable+0x44
+                        (*enemies)[i]->setPosition(player->getPosition() + Vector{(float) xOff, 5000.0f, 25000.0f}); // vtable+0x44
```

`PlayerEgo::getPosition()` is a pure getter returning a `Vector` by value; the
temporary lives to the end of the full expression, so the value and lifetime
are unchanged.  The fold changes the virtual-register numbering and lets clang
reuse the getPosition return registers directly for the `Vector` add, dropping
one byte of same-address diff.  Function size stays exactly 57,232 B and
`.rel.plt` is byte-identical.

## Method

`scratch/w12504/oneuse.py` parses the clang AST of `process` and finds every
`VarDecl` with exactly one `DeclRefExpr` use in the function (157 declarations).
`inline_oneuse.py` keeps single-line `Type name = expr;` declarations whose
initializer is pure (no `++`/`--`/assignment/`new`/`delete`/RNG and only known
pure getters), whose single use is on a simple statement in the same straight
line of code, and folds the initializer into the use.  60 candidates were
measured; 48 were size-exact, and the `playerPos` fold at line 4148 is the only
raw win (52,798).  Every other size-exact fold is neutral at 52,799.

## Rejected

* Adjacent-statement swap sweep on the 52,799 tree (1,210 pairs): two
  size-exact -1 cells, both rejected on semantics.  `setPosition(m_tempVec)` /
  `forceUpdate(30, false)` (case 11, line 1240) is `setPosition` then
  `forceUpdate` in the original (`0x13be72` / `0x13be82`); the swap is a
  scheduler artefact.  `frame->setPosition` / `CameraSetLocal` (line 2671) is
  the read-after-write reorder already rejected by w12403.
* Declaration-move sweep on the 52,799 tree (993 candidates): the only
  size-exact win is `float lift` moved above `m_nScriptTimerA += delta`
  (52,796), rejected on semantics — the original at `0x141932`-`0x141976`
  converts `m_nScriptTimerA + delta` and stores the sum afterwards.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12504
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52798 aligned= 20176 size= 57232/57232 insn= 20731/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52798 size=57232/57232 text=76180 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (not tracked): `oneuse.py`, `inline_oneuse.py`,
`stmt_pairs.py`, `sweep.py` in `/home/debian/GalaxyOnFire2/scratch/w12504/`.
