# MGame::OnTouchEnd — wave 156 (w10212): `hudTouchFlags == 0` joins the wingman `else if` chain

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10212`,
base `cf74f4ee6` (wave 155 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10212`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 155) | w10014/w10108 shape | this wave |
|---|---:|---:|---:|
| size | 8080 | 8080 | **8080** |
| raw same-address diff | 4391 | 4435 | **4415** |
| aligned (align_diff) | 903 | 929 | **926** |
| shared `blx dockToAsteroid` owner | stream | asteroid (right) | **asteroid (right)** |
| shared `blx hudEvent(6)` owner | stream | autopilot (right) | **autopilot (right)** |
| shared `blx initHudMenu` owner | (1) @0x17adca | (2) @0x17adca | **(2) @0x17adca** |

(The w10014/w10108 notes quote 4427/926 on the wave-153/154 base.  On wave 155 the same
source shape measures 4435/929: the wave-155 `MGame::OnUpdate` change moves MGame.cpp's
literal pool, worth +8 raw in the `post` region.  The shape itself is unchanged.)

## 1. Landed shape

Three source changes on the wave-155 base:

1. **Faithful dock tail-merge owners** (w9914 shape): named `asteroidTarget` local in the
   asteroid arm and `__builtin_expect(isDockingToDockingPoint(), 1)` on the middle arm.
2. **initHudMenu tail-merge owner**: reorder the three source arms from
   `0x200, 0x800, 0x400` to `0x400, 0x800, 0x200` (mutually exclusive bits).  The `0x400`
   arm becomes the lower-numbered MBB and owns the shared `blx`, as in the original.
3. **NEW: fold the `if (this->hudTouchFlags == 0)` block into the wingman `else if`
   chain.**  This is semantically identical: `wingmanCommand >= 0` implies one of the
   wingman bits (`0x20000/0x40000/0x80000/0x100000`) is set, so `hudTouchFlags != 0`
   and the block is dead after the wingman arm.  The flat chain is also the more natural
   source.  It does not move the pre-dock run (`pre-dock` stays 3268) but tightens the
   dock chain: `dock` 1014 -> 994, raw 4435 -> 4415, aligned 929 -> 926.

```cpp
        if (this->hudTouchFlags & 0x400) {
            this->hud->initHudMenu(2, this->level);
        } else if (this->hudTouchFlags & 0x800) {
            ...
        } else if (this->hudTouchFlags & 0x200) {
            this->hud->initHudMenu(1, this->level);
        } else if (this->hudTouchFlags & 0x2000) {
            ...
        }
        ...
        } else {
            if (wingmanCommand >= 0) {
                ...
            } else if (this->hudTouchFlags == 0) {
                this->hud->closeHudMenu();
                ...
            }
        }
```

Owner detection (`/dev/shm/w10212/owner.py`):

```
orig  initHudMenu blx@17afde arg=2 ; dockToAsteroid ast@17bcf4 ; hudEvent auto@17bb5c
ours  initHudMenu blx@17adca arg=2 ; dockToAsteroid ast@17bc66 ; hudEvent auto@17baec
```

## 2. Region diff

| region | base | w10014 shape | this wave |
|---|---:|---:|---:|
| `pre 0x17a144..0x17adc0` | 114 | 124 | **124** |
| `pre-dock 0x17adc0..0x17bb3a` | 3277 | 3268 | **3268** |
| `dock 0x17bb3a..0x17bf7e` | 986 | 1014 | **994** |
| `post 0x17bf7e..end` | 14 | 29 | **29** |

The 3268-byte `pre-dock` run is still the cold-block placement signature identified in
w10014 §2 (the original's `hudTouchFlags == 0` join is at `0x17afe2` with the epilogue
inline; ours is at `0x17aeb2` with the body inline).  It is unchanged by this wave.

## 3. New hypotheses measured (all 8080 B unless noted)

All on top of the faithful owners + `400,800,200` arm order.

| variant | raw | aligned | note |
|---|---:|---:|---|
| w10014/w10108 shape | 4435 | 929 | separate `if (hudTouchFlags == 0)` |
| **landed: fold into wingman `else if`** | **4415** | **926** | dock 994 |
| fold + `wingmanCommand < 0 &&` | 4415 | 926 | folded by InstCombine |
| fold + nested `if (hudTouchFlags == 0)` | 4415 | 926 | folded |
| flatten `weaponSlot`/wingman/hud into one chain | 4415 | 926 | folded |
| `__builtin_expect(hudTouchFlags == 0, 0)` | 5757 | 1068 | size 8068, re-lay |
| move block out of the `else`, after the if/else | 4467 | 938 | 8080 |
| move block before the wingman block | 5580 | 1156 | size 8036 |
| `__builtin_expect(hudMenuOpen, 1/0)` | 5952/6179 | 1205/1346 | size 8064/8044 |
| `won` guard `(hasWon() & 1) != 0` | **4394** | 927 | **breaks dockEvent by 1 B** |
| `won` guard truthiness | 4384 | 1797 | block re-lay |
| `expect(hudTouchFlags == 0, 0) && hudMenuOpen` | 5778 | 1071 | size 8076 |
| `hudMenuOpen && expect(hudTouchFlags == 0, 0)` | 4481 | 953 | size 8088 |
| `hudTouchFlags == 0 && expect(player != nullptr, 1)` | 4481 | 953 | size 8088 |
| `expect(hudTouchFlags != 0, 1)` inverted | 5757 | 1068 | size 8068 |

The `won & 1` spelling was also tried with the operands swapped, `== 1`, `<< 31`,
`& 0x1`, and `& 1` as a bool: all give 4394/927 and the same 1-byte dockEvent
break.  `& 0xff` keeps dockEvent exact but re-lays to aligned 1797.

The `won & 1` variant is a genuine 21-byte raw win, but it reorders MGame.o's relocations:
`resetStartSequenceOver` moves ahead of `dockToStream` in the PLT, so `dockEvent`'s
`blx dockToStream` byte changes (0x17fc18: orig `c4`, ours `ca`).  It is not landable
without restoring the import order; `check` drops byte_exact 4507 -> 4506.

## 4. Honest assessment

The mission's two constraints are met: faithful dock owners **and** the initHudMenu `(2)`
owner, at 8080 B.  The new `else if` fold is worth **-20 raw / -3 aligned** over the
w10014/w10108 shape while leaving the `pre`/`pre-dock` regions byte-identical and the
`post` region unchanged.  The 3268-byte pre-dock run remains the PGO join-placement
signature; no source spelling of the `hudTouchFlags == 0` cold branch was found that
flips it without a 12-byte body re-lay (the `__builtin_expect` 2000:1 weight is too
strong, and the static `X == 0` zero heuristic keeps the body hot).

Next lever (not this wave): a mild branch weight on the `hudTouchFlags == 0` join.  The
`won & 1` raw win is available if the MGame.o import order can be restored.

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10212/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4415 / 8080
python3 /dev/shm/w10212/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 926
python3 /dev/shm/w10212/regions.py _ZN5MGame10OnTouchEndEiiPv
python3 /dev/shm/w10212/owner.py
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w10212/{fdiff.py,align_diff.py,regions.py,owner.py,compile_ir.sh,
variant*.py,MGame.*.cpp}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
