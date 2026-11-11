# LevelScript::process — wave 170 / w11604: remaining block-scope aliases

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11604`, branch
`wave170-w11604`, base master `50716a763` (wave 170).  Scratch `/dev/shm/w11604/`.

Follow-up to `levelscript-process-alias-wave169-w11504.md` (five `m_pLevel` /
`m_pGeometry*` / `m_pCamera` aliases, 53,970 -> 53,933 on the wave-169 base; the
wave-170 base is 53,932 after the state-7 `m_pLevel` block-scope fix).

Result: **ten measured raw wins landed** in four commits, all size-exact
(57,232 B) and `.rel.plt` byte-identical (ratchet PASS after every commit):

```
base (wave-170 HEAD)   raw 53,932  aligned 17,900  insn 20,722/20,830
final                  raw 53,829  aligned 18,339  insn 20,725/20,830
size                   57,232 / 57,232
gate                   byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
symbol parity          5370 / 5370 (0 extra / 0 missing)
```

The aligned count rises while raw falls: the landed aliases change register
allocation (fewer `m_pX` reloads, different callee-saved assignment) which
shifts branch/instruction alignment, but the same-address byte diff — the
byte-identicality ground truth — drops monotonically.  Instruction count is
+3 on 20,830 (not a structural regression).

## 1. Method

A brace-block parser over the function body (only statement blocks whose `{`
ends the line) enumerated every block with >=2 uses of a member, then applied
`Type *alias = m_pX;` at the block head and rewrote every `m_pX` in the block
to `alias`, rebuilt (`-j2`, ccache incremental ~3 s), and recorded
`fn_diff.py` raw/aligned/size.  Each candidate was measured alone against the
then-current base; only size-exact raw wins were kept, and every kept win was
re-measured on the new base before the next candidate.  This is much cheaper
than the hand sweeps of earlier waves (~3 s per candidate).

## 2. Landed wins (each on top of the previous)

| # | commit | block | change | raw | aligned |
|---|---|---|---:|---:|---:|
| 1 | `9d8cb1217` | case-29 intro (`m_nState==0`, 1845-1890) | `TargetFollowCamera *cam` for `setLookAtCam` + 2x `setPosition` (3 of 6 uses; first `enableFirstPersonCam` and the trailing `setTarget`/`getPosition` stay direct) | 53,932 -> 53,889 | 17,900 -> 17,964 |
| 2 | `0feab4984` | mission-70 `m_nState==11` (5091-5113) | `m_pCamera` for `translate` + `setTarget` | -> 53,884 | -> 17,994 |
| 3 | | mission-? `m_nState==4` block (3482-3511) | `m_pCamera` | -> 53,880 | -> 18,109 |
| 4 | | mission-? `m_nState==3` block (3398-3452) | `m_pCamera` | -> 53,877 | -> 18,183 |
| 5 | | `m_pGeometry6 == nullptr` block (3836-3847) | `m_pCamera` | -> 53,874 | -> 18,194 |
| 6 | | mission-? `m_nState==4` block (3016-3041) | `m_pCamera` | -> 53,873 | -> 18,184 |
| 7 | | mission-1 `m_nState==1` block (649-657) | `m_pCamera` | -> 53,872 | -> 18,186 |
| 8 | `5ea1ddb91` | mission-? `m_nState==6` block (3082-3092) | `m_pRadar` | -> 53,860 | -> 18,270 |
| 9 | | mission-1 state-7 particle block (2159-2205) | `m_pParticleGeom0` | -> 53,849 | -> 18,271 |
| 10 | | mission-1 `m_nState==3` block (736-755) | `m_pRadar` | -> 53,847 | -> 18,270 |
| 11 | `740265646` | mission-? state-1 block (4780-4798) | `m_pGeometry4` | -> 53,842 | -> 18,291 |
| 12 | | mission-80 state-0 block (2608-2628) | `m_pGeometry1` | -> 53,839 | -> 18,302 |
| 13 | | mission-1 `timer>=6001` block (1151-1162) | `m_pGeometry5` | -> 53,837 | -> 18,311 |
| 14 | `82e7d864a` | mission-? `m_nState==9` block (1633-1645) | `m_pCamera` | -> 53,832 | -> 18,331 |
| 15 | | mission-? `m_nState==9` block (2691-2718) | `m_pCamera` | -> 53,830 | -> 18,338 |
| 16 | | `case 8` block (806-824) | `m_pCamera` | -> 53,829 | -> 18,339 |

(Line numbers are from the post-commit tree; commits 2-7 are one commit, 8-10
one commit, 11-13 one commit, 14-16 one commit.)

Notable shape detail (win #1): aliasing all six `m_pCamera` uses at the block
head is only -34; aliasing the three middle uses (`setLookAtCam` + the two
`setPosition` calls) is -43.  Declaring the alias after the first use
(`enableFirstPersonCam`) and before `setLookAtCam` is the best placement;
declaring it before the `setPosition` run is size-changing (+8).

## 3. Negative results (re-measured on this base)

* `m_pLevel` block sweep: all 140 statement blocks with >=2 uses, no raw win
  (best is +42).  Wave-169's "exhausted" conclusion still holds after the new
  aliases.
* `m_pHud` blocks: no raw win.
* `m_pGeometry0/2/3/6`, `m_pExplosion`, `m_pParticleGeom1/2`: no raw win on
  this base (the wave-169 `m_pGeometry6`/`m_pExplosion` results reproduce).
* Function-scope declaration shapes: `Status *&status` +1; `status` first
  +1,633 (size +96); `lvl = m_pLevel` alias +1,242 (size +8); `player` first
  +12; `enemies` first +14.  The callee-saved map (original `this=sl`,
  `canary=fp`, `&Globals::status=r4`; ours swapped) is still not reachable
  from declaration shapes without a size change, confirming wave-168.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53829 aligned= 18339 size= 57232/57232 insn= 20725/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
