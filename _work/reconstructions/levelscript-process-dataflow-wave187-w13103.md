# LevelScript::process — DATA-FLOW pass (wave 187, w13103)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13103`, branch
`wave187-w13103`, base master `8b5d4281c` (wave-186 harvest 6, raw **51,968**,
`.text` 74,274).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13103`.

Continuation of the wave-182/183/185/186 data-flow passes (w12604, w12703,
w12903, w13003): bool vs int, `float` vs `int` casts, `Vector`
construction/assignment forms, `Array` indexing, multiply-vs-branch.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,968           51,966     (-2)
process aligned insns    20,856           20,856
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,727
.text diffbytes          74,274           74,272     (-2)
.ARM.extab diffbytes     647              647        (size/base exact)
.note.gnu.build-id       20               20
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
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

`raw` is the same-address byte diff over the function body (literal pools
included) from `tools/fn_diff.py`; `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after (byte-identical to the
original).

## Landed change (source line 2504, `case 70` state-0 camera seat)

```cpp
// before
camPos += dir * 3000.0f;
// after
camPos += 3000.0f * dir;
```

This is the `Vector operator*(const Vector&, float)` call site.  The original
emits the scalar as the first operand of the multiply (`vmul.f32 sN, sConst,
sDir`) while our left-to-right spelling `dir * 3000.0f` put the vector first
and forced a different register/operand order in the surrounding
`Vector::operator+=` inline.  `Vector` scalar multiply is commutative, so the
swap is semantics-preserving.  Measured alone on the wave-186 body: raw
**51,968 -> 51,966** (-2 B), size 57,232/57,232 exact, `.text` 74,274 ->
74,272, `.rel.plt` unchanged.

## Method

`scratch/w13103/harness.py` compiles one `LevelScript.cpp` variant with the
exact `compile_commands.json` command, runs the exact `build.ninja` gold link
(and keeps an unstripped copy for `-g` line attribution), then measures with
`_work/reconstructions/measure_w11901.py` / `tools/fn_diff.py`; ~1.5 s per
candidate.  `mnalign2.py` / `mnsummary.py` align the original and our
instruction streams on mnemonics after removing literal-pool words, and
`callseg.py` aligns on PLT call targets; both were used to locate candidate
sites before measuring.

The win was found by a mechanical sweep of every simple `IDENT * CONSTf`
multiply in `process` (60 sites) with the operands swapped.  Only this site
improves; the rest are neutral or worse.

## Re-probed and rejected on the current body

All size-exact and `.rel.plt`-identical unless noted; base 51,968.

* **Mission==1 prologue statement order** (`rotate`, `setToSleep`,
  `getPosition`, `forceUpdate`): all 24 permutations measured; current
  `rotate, setToSleep, getPosition, forceUpdate` is the local optimum
  (original's emitted order `forceUpdate, rotate, getPosition, setToSleep`
  is 51,971, +3).
* **`(float)(N*delta)` family** (all 19 distinct forms, 2-3 spellings each):
  every alternative is worse or neutral; `-18*delta` alternatives neutral.
* **`-(float)delta` <-> `(float)-delta`**: the single `-(float)delta` site is
  worse (54,170); the 30 `(float)-delta` sites uniformly swapped are
  size-changing (57,016).
* **Escort route cross-product forms** (lines 3611-3619): difference forms
  `(r.x-d.x)*9e5`, `(r.z-d.z)*9e5`, `(d.y-r.y)*-9e5` are +8/+12/+8; all
  combinations worse.  Current product forms are the local optimum.
* **`bool` locals**: `bool fading` -> `int` neutral; direct
  `if (Globals::layout->isFading())` +8; `bool docked` -> `int` neutral.
* **`Array` indexing**: `(*messages)[i]` -> `messages->data_[i]` / `->at(i)`
  neutral (identical code); `(*enemies)[i]` -> `data_[i]` size-changing
  (+12); `enemies->count` -> `size()` neutral.
* **`Vector` zero forms**: `Vector zero{0,0,0}` -> `Vector zero;` /
  `Vector zero{}` / `= Vector()` all neutral.  Hoisting inline
  `Vector{0,0,0}` arguments to a named local is worse (+1..+599).
* **`(float)(delta<<1)`** -> `(float)(2*delta)` neutral; -> `(float)delta*2.0f`
  size-changing (57,208).  `(delta<<1)&-16` -> `(delta*2)&-16` neutral.
* **`Vector / 1.7f`** -> `* (1.0f/1.7f)` neutral.
* **`(float)(m_nScriptTimerA*delta)*0.001f`** associativity variants neutral.
* **`m_nScriptTimerA=0; m_nScriptTimerB=0;` merge moves** (lines 3736/3738,
  5251/5253, 5277/5279): moving the second store next to the first is
  neutral or worse (+2..+88); the original has two more NEON zero-vector
  stores (`vmov.i32 q8,#0; vst1.64`) than ours, but they are `Vector zero`
  materialisations whose `strd`/`str` vs `vst1.64` choice is driven by stack
  slot alignment/RA, not by the zero-init spelling.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13103
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51966 aligned=20856 size=57232/57232 insn=20727/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51966 size=57232/57232 text=74272 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Harnesses (`harness.py`, `sweep.py`, `mnalign2.py`, `mnsummary.py`,
`callseg.py`, `mapzero.py`) live in `/home/debian/GalaxyOnFire2/scratch/w13103/`;
they are scratch, not tracked.
