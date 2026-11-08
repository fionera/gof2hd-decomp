# MGame::OnTouchEnd — wave 158 (w10412): fold + w10311 AND form; the pre-dock join is a tail-merge, not a branch-weight, signature

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10412`,
base `9e4795206` (wave 157 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10412`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 157) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4391 | **4375** |
| aligned (align_diff) | 903 | **956** |
| `initHudMenu` owner | (1) @0x17adca | **(1) @0x17adca** |
| pre-dock join | 0x17aeb2 | **0x17aebc** (not fixed) |

## 1. Landed shape (three independent edits)

```cpp
// starMap tap: cold hint (restores dockToStream's PLT slot, w10311)
if (__builtin_expect(!this->starMap->OnTouchEnd(x, y), 0)) { // lint: arm_builtin
    return;
}

// won guard: the (B & 1) AND exception to calcZeroHeuristics (w10311)
bool won = Globals::status->getMission()->hasWon() ||
           (((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() & 1) != 0;

// fold the hudTouchFlags == 0 block into the wingman else-if chain (w10312)
} else if (this->hudTouchFlags == 0) {
```

The fold is semantically identical: `wingmanCommand >= 0` implies one of the wingman bits
(`0x20000/0x40000/0x80000/0x100000`) is set, so `hudTouchFlags != 0` and the block is dead
after the wingman arm.

## 2. Region diff

| region | base | this wave |
|---|---:|---:|
| `pre 0x17a144..0x17adc0` | 114 | **115** |
| `pre-dock 0x17adc0..0x17bb3a` | 3277 | **3264** |
| `dock 0x17bb3a..0x17bf7e` | 986 | **982** |
| `post 0x17bf7e..end` | 14 | **14** |

The `pre-dock` improvement (-13) is entirely the `won` AND form (`andform` alone: 3257);
the `dock` improvement (-4) is the fold. The join did **not** move: the test is still at
`0x17aebc` (`ldr r0,[fp,#248]; cmp r0,#0; bne.w 0x17b9fc`), not the original's
`0x17afe2` (`ldr; cbz r0, 0x17b01a`).

## 3. The new finding: the pre-dock join is a tail-merge signature

w10014/w10312 attributed the 3268-byte run to a PGO branch weight on
`hudTouchFlags == 0`.  This wave found a **source-only** lever that does move the join:
break the `initHudMenu` tail merge.  Adding a semantically-null store *after* the
`initHudMenu(2)` call stops `BranchFolder` from merging the two call blocks, so arm3
(the `0x400` arm) owns its own `blx`, falls through to the join, and the original layout
appears:

```
17af62  arm3: ldrd r0,r2,[fp,#116] ; movs r1,#2
17af68  blx  Hud::initHudMenu       ; arm3 falls through
17af6c  movs r0,#1 ; strb r0,[fp,#214]   ; the null store
17af72  join: ldr r0,[fp,#248]
17af76  cbz  r0, 17afc4             ; body cold
17af78  (autopilot epilogue)        ; fall-through (hot)
```

This is structurally the original (`0x17afd8..0x17b01a`), and `pre-dock` drops
3277 -> 2917.  But the epilogue/return block moves ~0x70 bytes **earlier** (return
`0x17b004` -> `0x17af94`), so every `b.w 0x17b004` in the `pre` region retargets and
`pre` explodes 114 -> 1463.  Net raw is worse.

| variant | size | raw | aligned | pre | pre-dock | dock | post |
|---|---:|---:|---:|---:|---:|---:|---:|
| base | 8080 | 4391 | 903 | 114 | 3277 | 986 | 14 |
| **landed: fold + andform + star** | **8080** | **4375** | **956** | **115** | **3264** | **982** | **14** |
| fold alone | 8080 | 4387 | 901 | 114 | 3277 | 982 | 14 |
| andform alone | 8080 | 4371 | 904 | 114 | 3257 | 986 | 14 |
| andform + star (w10311) | 8080 | 4379 | 949 | 115 | 3264 | 986 | 14 |
| fold + andform (no star) | 8080 | 4367 | 902 | 114 | 3257 | 982 | 14 |
| `arm3_store_after` (join fixed) | 8072 | 5714 | 1024 | 1463 | 2917 | 1054 | 280 |
| both arms store-after (join fixed) | 8080 | 5893 | 1043 | 1463 | 3259 | 1005 | 166 |
| `__builtin_expect(flags==0,0)` | 8068 | 5777 | 1035 | 1463 | 3007 | 1017 | 290 |

`fold + andform` (no star) is the best raw (4367) but puts `resetStartSequenceOver`'s
relocation before `dockToStream`'s, so `MGame::dockEvent`'s `blx dockToStream` moves
`0x683a0 -> 0x683ac` (byte_exact 4507 -> 4506).  The starMap cold hint restores the PLT
slot at a cost of +8 raw / +54 aligned.

## 4. Merge lesson (both orders)

`fold` and `andform` are independent source lines, so the two orders are identical:

| shape | size | raw | aligned |
|---|---:|---:|---:|
| fold alone | 8080 | 4387 | 901 |
| andform+star alone (w10311) | 8080 | 4379 | 949 |
| **combined** | **8080** | **4375** | **956** |

The combination is raw-best (-4 vs w10311 alone) but aligned-worst (+7 vs w10311 alone,
+55 vs fold alone).  The aligned regression is the starMap hint's block re-lay, not the
join.

## 5. Honest assessment

The pre-dock join is **not fixed** at 8080 B by any branch-weight spelling:
`__builtin_expect(flags==0,0)` (2000:1) moves the join but re-lays the hot region and
shrinks to 8068.  The join is controlled by `BranchFolder`'s common-tail owner, not by
`calcZeroHeuristics`: the `0x200` arm owns the shared `blx` in our build, the `0x400` arm
owns it in the original.  Breaking the merge with a null store *does* reproduce the
original join (test after arm3, cold body), but the return block then lands ~0x70 early
and the `pre` region retargets.  Closing that last gap needs the `won` chain placed just
before arm3 (as in the original) — a block-order problem, not a branch-weight one.

## 6. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10412/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4375 / 8080
python3 /dev/shm/w10412/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 956
python3 /dev/shm/w10412/regions.py _ZN5MGame10OnTouchEndEiiPv
python3 /dev/shm/w10412/owner.py
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w10412/{fdiff.py,align_diff.py,regions.py,owner.py,aligndetail.py,
run.sh,gen.py..gen8.py,v_*.cpp}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
