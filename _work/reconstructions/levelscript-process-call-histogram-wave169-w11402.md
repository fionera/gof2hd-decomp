# LevelScript::process — wave 169 / w11402: call-histogram re-sweep

Target `_ZN11LevelScript7processEi` (0x137d18, 57,232 B).  Worktree `wt/w11402`,
branch `wave169-w11402`, base master `ec3ceb07c`.  Scratch `/dev/shm/w11402/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    54,015
process aligned insns    17,788
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## The wave-166 delta table is stale on this base

`callseq.py` (bl/blx target-name diff over the whole function) gives
**orig 2,900 / ours 2,904 calls (+4 net)**.  Per-callee deltas now:

| callee | orig | ours | delta |
|---|---:|---:|---:|
| `AEGeometry::setPosition(const Vector&)` | 23 | 24 | +1 |
| `Level::getStarSystem()` | 15 | 16 | +1 |
| `Level::getPlayer()` | 23 | 24 | +1 |
| `PlayerEgo::getPosition()` | 41 | 42 | +1 |
| `LODManager::forceUpdate(int,bool)` | 66 | 65 | -1 |
| `Status::getStanding()` | 1 | 2 | +1 (PLT anchor) |
| `Standing::setStanding(int,int)` | 1 | 2 | +1 (PLT anchor) |
| `PlayerEgo::setAutoTurret(bool)` | 2 | 3 | +1 (PLT anchor) |
| `PlayerEgo::autoTurretIsEnabled()` | 1 | 2 | +1 (PLT anchor) |
| indirect `blx reg` (target `None`) | 544 | 541 | -3 |

`AEGeometry::moveForward` (40/40) and both `TargetFollowCamera::setPosition`
overloads (8/8, 59/59) are now **equal** — the wave-166 +1s there no longer
exist on this base, so the "six genuine duplicates" premise is obsolete.
Accounting: +4 genuine, -1 forceUpdate, -3 indirect, +4 anchors = +4 net.

## Line mapping

The linked `.so` is stripped, and every `.o` function section has VMA 0, so
`objdump -dl` on the `.o` mis-attributes lines (constructor lines leak into
process).  Fix: relink once without the strip step into
`/dev/shm/w11402/libgof2hdaa.unstripped.so` (same gold command, `-o` redirected)
and use `objdump -dl` on that.  `lines2.py` then gives correct source lines for
every call site.

## Measured experiments (all full link + `tools/fn_diff.py`)

HEAD = raw 54,015 / aligned 17,788 / 57,232 B.

| change | raw | aligned | size |
|---|---:|---:|---:|
| `m_pLevel->getPlayer()` → `player` @1479 | 54,046 | 17,921 | 57,232 |
| @1481 | 54,047 | 17,900 | 57,232 |
| @4009 / @4010 / @4014 | 54,048 | 17,783 | 57,232 |
| @4015 | 54,049 | 17,783 | 57,232 |
| @5204 | 54,043 | 17,798 | 57,232 |
| **@2855 (best)** | **54,021** | 17,782 | 57,232 |
| @2874 | 54,023 | 17,790 | 57,232 |
| @2880 | 54,024 | 17,779 | 57,232 |
| @4828 | 54,028 | 17,787 | 57,232 |
| @695 (setCollide(false)) | 54,332 | 17,791 | 57,232 |
| @745 (setCollide(true)) | 54,491 | 17,768 | 57,224 |
| @1471 (`!= 0` → `player != 0`) | 54,853 | 17,897 | 57,240 |
| pairs 4009+4010 / 4014+4015 / all four | 54,438 / 54,437 / 54,666 | — | 57,224/57,216 |
| pairs 2855+2874+2880 | 54,626 | 17,779 | 57,216 |
| pairs 1479+1481 | 54,835 | 17,902 | 57,224 |
| `getStarSystem` cache at case-4 (:1323/1324) | 54,164 | 17,792 | 57,232 |
| `player->getPosition()` cache at :2130/2132 | 54,530 | 17,735 | 57,224 |
| `m_pGeometry5->setPosition(player->getPosition())` → fff @769/812/830/845 | 54,500 / 54,433 / 54,501 / 54,700 | — | 57,224/57,216 |

Every candidate is a raw regression; the best (`:2855`) is still +6 B.  Note the
`:2855` site is inside the mission-113 `setAutoPilot(nullptr)` block, where the
original genuinely re-fetches `m_pLevel->getPlayer()` rather than reusing the
prologue `player` local — confirming the wave-166 conclusion that the +1s are
not literal duplicate calls.

## Landed

Nothing (source reverted; tree clean, baseline raw 54,015 / 17,788 / 57,232).

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# unstripped relink for correct line info:
sed -n '1705p' cmake-build-match/build.ninja | sed 's/^  COMMAND = cd [^&]*&& //' | awk -F' && ' '{print $1}' > link.sh
# redirect -o to /dev/shm/w11402/libgof2hdaa.unstripped.so, then run it
python3 /dev/shm/w11402/lines2.py '_ZN5Level9getPlayerEv'
```
