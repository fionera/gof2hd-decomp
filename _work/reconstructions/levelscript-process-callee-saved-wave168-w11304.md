# LevelScript::process — wave 168 / w11304: cached `Status` alias placement

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11304`, base master
`631c26f0b` (wave 167).  Scratch `/dev/shm/w11304/`.

Follow-up to `levelscript-process-alias-wave167-w11204.md` and
`levelscript-process-callee-saved-wave166-w11104.md`.

Result: **four measured raw wins landed**, all size-exact (57,232 B) and with
`.rel.plt` byte-identical:

```
base (wave-167 HEAD)   raw 54,031  aligned 17,764
final                  raw 54,018  aligned 17,764   (57,232 B)
.text diffbytes        82,526 -> 82,513
.ARM.extab             670 (unchanged)
.rel.plt               0 differing bytes
```

## 1. The callee-saved map is not reachable from declaration shapes

The wave-166/167 conclusion is reconfirmed on the wave-167 base:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base `Status *status = Globals::status;` | **54,031** | **17,764** | 57,232 |
| direct `Globals::status->` (no local) | 54,032 | 17,763 | 57,232 |
| `Status *&status` / `Status *const &status` / `Status **status` | 54,032 | 17,763 | 57,232 |
| `Status &status = *Globals::status;` + `.` uses | 54,031 | 17,764 | 57,232 |
| `Status *const status` / `const` on messages/player/enemies | 54,031 | 17,764 | 57,232 |
| `Level *&self` / `Level *const &self` (reference aliases) | 54,031 | 17,764 | 57,232 |

Every form that actually caches the `Status*` value is still `this=fp`,
`canary=sl`, `&status=r9`, `Status*=r4`; the original map (`this=sl`,
`canary=fp`, `&status=r4`, value reloaded) is decided by the unmatched body
(the original spills/reloads `this` at `0x138878`, ours keeps it in `fp`).
All 24 declaration permutations were re-measured; the base order remains the
best raw (best alternative `enemies player messages status` = 54,035).
`__builtin_expect` on either dispatch is still size-changing and much worse.

A base-vs-direct byte compare shows the two builds differ in exactly one
29-byte run at `0x137d61`; the rest of the function is identical.  So the
whole map question is worth at most ~6 bytes of the prologue and cannot be
won without a size change (the original prologue is 6 B shorter).

## 2. The lever: consecutive `Globals::status->` write runs

The original reloads `[r4]` at *every* `Globals::status->` use.  Our
reconstruction has a few cached `Status *st` locals in runs of consecutive
status writes; moving which write uses the cached value changes the
instruction mix and the raw same-address diff without changing the size.

Landed (each on top of the previous):

| # | block | change | raw |
|---|---|---|---:|
| 1 | mission 106, `field_60/68` | `st` used for gamma (`field_68`) instead of armor (`field_60`) | 54,031 -> 54,025 |
| 2 | mission 113, 4 HP writes | new `Status *st` before the run, used for shield (`field_5c`) | 54,025 -> 54,022 |
| 3 | mission 5, station-attacked run | new `Status *st` before the run, used for `getStation()->setAttackedFriends(false)` | 54,022 -> 54,020 |
| 4 | mission 113, 4 HP writes | same `st` also used for gamma (`field_68`) | 54,020 -> 54,018 |

Every other position in those runs (and the mission-106 block with a second
cached use) is worse; the mission-113 block with `st` at uses 2+3 is
size-changing (+8).  The mission-106 block at 2180 and the mission-113
`st` at use 1 are worse (`+1..+2`).

## 3. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 54018 aligned= 17764 size= 57232/57232 insn= 20714/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
