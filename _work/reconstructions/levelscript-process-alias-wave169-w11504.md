# LevelScript::process — wave 169 / w11504: remaining block-scope aliases

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11504`, branch
`wave169-w11504`, base master `00c403d5a` (wave 169).  Scratch `/dev/shm/w11504/`.

Follow-up to `levelscript-process-mplevel-alias-wave169-w11404.md` (three
narrow-scope `m_pLevel` aliases, 54,015 -> 54,002 on the wave-168 base; the
wave-169 base is 53,970).

Result: **five measured raw wins landed**, size-exact (57,232 B), `.rel.plt`
byte-identical (ratchet PASS):

```
base (wave-169 HEAD)   raw 53,970  aligned 17,818  insn 20,722/20,830
final                  raw 53,933  aligned 17,916  insn 20,731/20,830
.text diffbytes        81,642 -> 81,605
.ARM.extab             1,292 -> 1,291
.rel.plt               0 differing bytes
gate                   byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. `m_pLevel` block sweep is exhausted on this base

Re-ran the full innermost-block sweep (56 leaf blocks with >=2 `m_pLevel`
uses) **and** the 98 non-leaf blocks with >=2 uses (the wave-169 note only
swept leaf blocks).  Every size-exact leaf candidate is raw >= 53,970.  Three
non-leaf blocks win and are landed below.

## 2. Landed wins (each on top of the previous)

| # | block | change | raw | aligned |
|---|---|---|---:|---:|
| 1 | mission-0 `else if (m_nState == 1)` (1676) | `Level *self` for the 3 uses (`forceUpdate` x2, `resetCamera`) | 53,970 -> 53,968 | 17,818 -> 17,832 |
| 2 | case-21 `for (i=1..3)` loop body (1764) | `Level *self` for the 2 `getEnemies()` uses | (stacked) | (stacked) |
| 3 | state-7 dock block (3074) | move existing `Level *self` to the outer block; `resetCamera(self)` | -> 53,958 | -> 17,846 |
| 4 | mission-1 `timer >= 6001` block (1147) | `AEGeometry *geom = m_pGeometry5` for the 3 uses | 53,958 -> 53,956 | 17,846 -> 17,855 |
| 5 | mission-70 state-1 block (4770) | `AEGeometry *geom = m_pGeometry4` for the 2 uses | 53,956 -> 53,951 | 17,855 -> 17,876 |
| 6 | mission-80 state-0 block (2603) | `AEGeometry *geom = m_pGeometry1` for the 2 uses | 53,951 -> 53,948 | 17,876 -> 17,887 |
| 7 | mission-1 state-7 particle block (2159) | `AEGeometry *pg1 = m_pParticleGeom1` for the 3 uses | 53,948 -> 53,939 | 17,887 -> 17,892 |
| 8 | mission-1 msg-3 block (1629) + marker-create (3834) | `TargetFollowCamera *cam = m_pCamera` for 3+2 uses | 53,939 -> 53,933 | 17,892 -> 17,916 |

## 3. Negative results

* Function-scope `Level *&self = m_pLevel;` / `Level *const &self`: 55,127 raw,
  57,192 B (-40) — size-changing.
* `Status *status` removal (all 95 uses direct `Globals::status->`): 53,971 raw,
  17,814 aligned, size exact (+1).  Declaring `status` first/last is
  size-changing (+88 / identical).
* `LevelScript *const lsc = this;` with every member access via `lsc->`:
  codegen-identical (53,958 / 17,846).
* `m_pRadar` block aliases (6 leaf blocks): best is a tie.
* `m_pGeometry6` (5 blocks), `m_pExplosion` (4), `m_pGeometry2/3` (2 each):
  no raw win.
* `m_pGeometry5` block at 853 (2 uses) was -6 on the base but +6 after the
  other aliases landed (interaction), so not landed.
* `m_pCamera` block at 2685 (2 uses) is a raw tie; the other 66 leaf blocks
  are worse.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53933 aligned= 17916 size= 57232/57232 insn= 20731/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
