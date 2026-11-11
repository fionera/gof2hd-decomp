# LevelScript::process — wave 169 / w11404: size-neutral `m_pLevel` alias scopes

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11404`, branch
`wave169-w11404`, base master `ec3ceb07c` (wave 168).  Scratch `/dev/shm/w11404/`.

Follow-up to `levelscript-process-callee-saved-wave166-w11104.md` §4 ("only the
stack-spilled `m_pLevel` alias is a lever") and
`levelscript-process-alias-wave167-w11204.md`.

Result: **three measured raw wins landed**, all size-exact (57,232 B) and with
`.rel.plt` byte-identical (ratchet PASS):

```
base (wave-168 HEAD)   raw 54,015  aligned 17,788  insn 20,715/20,830
final                  raw 54,002  aligned 17,810  insn 20,722/20,830
.text diffbytes        (wave-168) -> -13 raw bytes
.rel.plt               0 differing bytes
gate                   byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. The alias at function scope is still size-changing

Re-confirmed on the wave-168 base.  `Level *self = m_pLevel;` declared at the
top of the body (line 614) and used for a range of occurrences:

| replacement range (occurrence indices) | raw | aligned | size | map |
|---|---:|---:|---:|---|
| none (base) | 54,015 | 17,788 | 57,232 | this=fp |
| {36} | 55,540 | 18,306 | 57,352 (+120) | this=sl |
| {3,36} | 55,546 | 18,317 | 57,336 (+104) | this=sl |
| 3..36 | 55,359 | 18,395 | 57,288 (+56) | this=sl |
| 3..95 | 55,164 | 24,280 | 57,232 (exact) | this=r9 |
| 3..346 | 55,419 | 19,095 | 57,048 (-184) | this=r9 |

The map does flip to `this=sl` (the original's register), but the alias value
occupies a callee-saved register (`r8` in the `{36}` build) and pushes
`enemies`/`&status` around; the extra live range costs 100+ bytes.  Only
`3..95` is size-exact, and it lands in a different map (`this=r9`) with raw
+1,149.  `Level *const self`, `Level *&self`, `Level *const &self` are
codegen-identical to the plain pointer for small ranges (`R_3_36` is exactly
the base); `Level &self = *m_pLevel;` cannot express the pointer-argument
uses (`resetCamera(m_pLevel)`).

## 2. The lever that works: a *narrow block scope*

Declaring the alias at the start of the innermost block that contains a small
run of `m_pLevel` uses keeps the live range short, so the frame/RA is
unchanged (`this=fp`) and the size stays 57,232.  The alias then saves the
second load of the stack-spilled member (`ldr rX,[sp,#80]; ldr rX,[rX]` ->
one `mov`) and matches the original's instruction mix in that run.

Sweep: for every innermost block with >=2 `m_pLevel` occurrences (68 blocks),
insert `Level *self = m_pLevel;` at the block start and replace all
`m_pLevel` uses in that block.  Size-exact, raw-positive blocks on the
wave-168 base (raw < 54,015):

| block (start line) | uses | raw | aligned |
|---|---:|---:|---:|
| `if (m_nScriptTimerA >= 3001)` (2853) | 5 | **54,012** | 17,784 |
| `if (player->isDockedToDockingPoint())` (3074) | 3 | **54,009** | 17,787 |
| mission-0 msg-10 `if (...isOver())` (686) | 2 | **54,010** | 17,783 |
| mission-1 `case 1:` (4337) | 2 | **54,002** | 17,810 |
| `case 4:` (1322) | 2 | 54,012 | 17,785 |
| `case 1:` s1 (1061) | 2 | 54,013 | 17,786 |

## 3. Landed (each on top of the previous)

| # | block | change | raw | aligned |
|---|---|---|---:|---:|
| 1 | state-5 timer `>= 3001` | `Level *self` for 5 uses (incl. `resetCamera(self)`) | 54,015 -> 54,012 | 17,788 -> 17,784 |
| 2 | mission-0 msg-10 + state-7 dock | two block-scoped aliases | 54,012 -> 54,007 | 17,784 -> 17,786 |
| 3 | mission-1 `case 1:` | block-scoped alias for `resetCamera` + `getEnemies` | 54,007 -> 54,002 | 17,786 -> 17,810 |

The `case 1:` win is raw-positive but aligned +24; it is kept because raw is
the byte-identicality ground truth and the shape is a faithful block-local
alias (the aligned delta is one extra instruction run, not a structural
change).

## 4. Negative results

* `Level *self = m_pLevel;` + direct `Globals::status->` (no `Status *status`
  local): 54,255 raw, +12 B, `this=sl` — still size-changing.
* Direct `Globals::status->` alone: 54,016 / 17,787 / 57,232 (+1 raw,
  aligned -1) — the known wave-166 result; not landed.
* Alias used for the three prologue accessors only: +8 B (57,240).
* Stacking every other size-exact block on top of the final state does not
  lower raw further (`case 4:` ties at 54,002 with aligned +1; `messages[9]`
  else-if is +1).

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=54002 aligned=17810 size=57232/57232 insn=20722/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
