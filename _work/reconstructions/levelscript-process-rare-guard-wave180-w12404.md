# LevelScript::process — wave 180 / w12404: case-102 timer-guard expect

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
original `0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`wt/w12404`, branch `wave180-w12404`, base master `99e341e9b` (wave-179
merge).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12404/`.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,926           52,925     (-1)
process aligned insns    20,010           20,010     (0)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,731
.text diffbytes          76,870           76,869     (-1)
.ARM.extab diffbytes     709              709        (size/base exact)
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
included), from `tools/fn_diff.py`; it is the primary metric.  The
`.note.gnu.build-id` is 20 differing bytes before and after; total differing
section bytes fall 77,599 -> 77,598.  `.ARM.extab` content is unchanged
(709), so the -1 `.text` byte is a net win (no extab growth).

## The change

One `__builtin_expect` hint in the mission-102 block (`case 102`, the
Valkyrie escort battle), at source line 3461:

```cpp
// before
if (m_nScriptTimerA >= 10001) {
// after
if (__builtin_expect(m_nScriptTimerA >= 10001, 1)) { // lint: arm_builtin
```

This is the post-fly-by timer that releases the group-10 escorts and hands
control back to the player.  The original lays the timer-expiry body out as
the fall-through/hot path; the hint reproduces that branch layout.  The
function size stays exactly 57,232 B and `.rel.plt` stays byte-identical
(md5 `289b2a069f8504d1e8a3919e43915e04`).

## Sweeps run on the wave-179 base

All cells were built with the real gold link (`ninja -C cmake-build-match
libgof2hdaa.so -j2`) and measured from the candidate ELF: raw same-address
diff, function size, whole-`.text` diff and `.rel.plt` md5.

| sweep | cells | size-exact | wins (< 52,926) | size-changing |
|---|---:|---:|---:|---:|
| sub-expression `&&`/`||` operands + simple whole conditions | 1,686 | 851 | **1** (line 3461:1) | 835 |
| compound whole conditions (`&&`/`||` as a whole) | 228 | 85 | 0 | 143 |
| flip/strip every existing `__builtin_expect` | 268 | 140 | 0 | 128 |
| equivalent condition-form rewrites (`==0` ↔ `!x`, `<` ↔ `<=`, operand swap) | 313 | 313 | 0 | 0 |
| `switch (__builtin_expect(...))` on all 9 switches | 18 | 0 | 0 | 18 |
| top-level declaration permutations (`messages,player,enemies,status`) | 24 | 12 | 0 | 12 |
| case-chain reorders (case 56, 131, 158) | 3 | 0 | 0 | 3 |
| pre-switch mission-chain permutations (`5,26,50,51`) | 24 | 4 | 0 | 20 |
| add each size-exact cell on top of the landed hint | 451 | 451 | 0 | 0 |
| size-changing pair search on top of the landed hint | 200 | 200 | 0 | 0 |
| case-102 size-exact pair search on top of the landed hint | 231 | 231 | 0 | 0 |
| block-scope alias sweep over all `case` bodies | 197 | 193 | 0 | 0 (4 type-mismatch build failures) |

The sub-expression sweep is the wave-176
`levelscript-process-subs-sweep-w12004.py` cell generator run unchanged on
the wave-179 base (it wraps every `&&`/`||` operand, the inner expression of
every existing `__builtin_expect`, and the inner expression of every
`!(...)`).  The compound sweep uses the wave-176 whole-condition parser
restricted to conditions containing `&&`/`||` in `process` only.

The only size-exact cell below 52,926 is line 3461 variant 1.  Every other
size-exact cell is neutral at 52,926 or worse.  The best size-changing pair
(`5105:1` + `5319:1`) measures 52,929 at size 57,232; no pair beats the
landed hint.

The rare-block alias sweep inserts `Type *name = expr;` at the head of every `case` body and
rewrites the block-local occurrences of each candidate expression (the wave-179 w12302 family).
The best cell is neutral at 52,925 (`case 4`, `m_pLevel`); every other alias is neutral or worse.

## Rejected near misses (size-changing unless noted)

| cell | raw | size | note |
|---|---:|---:|---|
| 1446 whole condition expect 0 | 52,929 | 57,232 | size-exact, +4 |
| 3159 outer expect 0/1 | 52,975 | 57,228 | -4 B |
| 1480 deactivation whole condition expect 1 | 53,023 | 57,232 | size-exact, +98 |
| 4293 case-142 `getSparks()` expect 0 | 52,926 | 57,232 | w12304 hint; neutral on this base |
| 4289 `m_nState == 1` expect 0 | 54,181 | 57,220 | -12 B |
| 4290 `getGasClouds() != nullptr` expect 1 | 53,783 | 57,224 | -8 B |
| 4097 case-135 `m_nState == 0` expect 0 | 54,682 | 57,236 | +4 B |
| 5310 tail `field_0xa9 != 0` expect 1 | 55,350 | 57,276 | +44 B |
| 5311 tail `m_nState == 0` expect 0 | 54,673 | 57,252 | +20 B |
| 5328 tail `m_nState <= 1` expect 1 | 55,420 | 57,296 | +64 B |

Case-chain reorders are all much worse and size-changing: case 56 `0,1 -> 1,0`
= 54,742 / 57,240; case 131 `1,2 -> 2,1` = 55,134 / 57,248; case 158
`1,2,3 -> 3,2,1` = 55,494 / 57,256.  The source order is already the best
shape for all three on this base.

Pre-switch mission-chain permutations: the base order `5,26,50,51` is best
(52,925 with the landed hint).  The w12306 order `26,5,50,51` measures
53,305 on this base (size 57,232), consistent with its wave-179 regression;
no permutation beats the base.

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing),
stub_zero_size **0**, missing **0**, wrong_type **0**, extra **0**,
`RATCHET PASS`.  `process` is raw **52,925** / aligned **20,010** / size
**57,232**; `.text` **76,869**; `.ARM.extab` **709** (size- and base-exact
156,312 B @ `0x1a3b78`); `.rel.plt` byte-identical
(md5 `289b2a069f8504d1e8a3919e43915e04`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12404
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52925 aligned= 20010 size= 57232/57232 insn= 20731/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52925 size=57232/57232 text=76869 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Scratch harnesses (not tracked): `compound.py`, `forms.py`, `switches.py`,
`perm.py`, `reorder.py`, `chainperm.py`, `pairs.py`, `pairs102.py`,
`addone.py`, `secdiff.py`, all in
`/home/debian/GalaxyOnFire2/scratch/w12404/`.
