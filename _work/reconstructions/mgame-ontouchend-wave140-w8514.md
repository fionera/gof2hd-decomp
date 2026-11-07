# MGame::OnTouchEnd — wave 140 (w8514): the wingman tail joins the `flags == 0` test

Status: source change landed, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8514`,
base `d4857f174`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8514`. Full gate green before and after (byte 4507, linked 4507, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | base | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4363 | **4367** |
| .text differing bytes | 84,651 | **84,655** |
| wingman tail | `b.n 17b976` (epilogue) | **`b.w 17aeb4` (join test)** |

The raw diff is 4 B worse; the change is a *structural* match, not a raw-byte win.
It is landed because it reproduces the original's MIR block order for the
wingman-body -> `flags == 0` edge, which the previous waves had not achieved.

## 1. Hypothesis

Wave 139 (w8414) established that the original's wingman body ends with
`b.w 17afe2` — a branch to the **join test** (`ldr [fp,#248]; cbz`), not to the
epilogue. The base instead ends the wingman body with `b.n 17b976` (epilogue).
Wave 139's variant H tried a nested shape and got 8068 B; it did not reproduce the
MIR edge. This wave asked: what natural source shape makes the wingman tail's
successor the join-test block while keeping 8080 B?

## 2. Ground truth (original)

```
17bf2e: ldr r0,[fp,#116]      ; wingman body tail
17bf32: movs r4,#0
17bf34: strb.w r4,[fp,#214]   ; hudMenuOpen = 0
17bf38: blx Hud::closeHudMenu
17bf3c: mov r0,fp
17bf3e: strb.w r4,[fp,#93]    ; pauseOpen = 0
17bf42: blx MGame::resumeSounds
17bf46: ldr.w r8,[sp,#28]
17bf4a: b.w 17afe2            ; -> join test
...
17afe2: ldr.w r0,[fp,#248]    ; join test (flags == 0)
17afe6: cbz r0, 17b01a        ; flags body
17afe8: ... epilogue ...
```

## 3. The source shape

The base source is an `else if` chain:

```cpp
if (weaponSlot >= 0) { ... }
else if (wingmanCommand >= 0) { ... }
else if (this->hudTouchFlags == 0) { ... }
```

Clang emits the wingman tail as `br label %epilogue`, because the `else if
(flags == 0)` clause is only reachable when `wingmanCommand < 0`. Nesting the
flags test inside the `else` of the weapon-slot case is semantically identical
(after the wingman body `flags != 0`, so the test is always false) and changes
the wingman tail's successor to the join-test block:

```cpp
if (weaponSlot >= 0) {
    ...
} else {
    if (wingmanCommand >= 0) {
        ...
        this->hudMenuOpen = 0;
        this->hud->closeHudMenu();
        this->pauseOpen = 0;
        resumeSounds();
    }
    if (this->hudTouchFlags == 0) {
        ...
    }
}
```

The MIR now has `%wingman_tail: br label %join_test` (the join test is the very
next IR block, `%2552 -> %2561`), exactly the original's edge. The final
MachineBlockPlacement layout still places the join test after the 0x800 body
(`0x17aeb4`), so the branch displacement differs from the original's
`0x17afe2`; the instruction is the same `b.w join_test`.

## 4. Variants measured (all 8080 B unless noted)

| variant | raw diff | join test | note |
|---|---:|---|---|
| base (`else if`) | **4363** | 0x17aeb4 | wingman tail -> epilogue |
| **nested `else { if (wingman){} if (flags==0){} }`** | **4367** | 0x17aeb4 | **landed**; wingman tail -> join test |
| nested with `goto afterMenu` + label (same CFG) | 4367 | 0x17aeb4 | identical bytes to nested |
| `goto afterMenu` in 0x800 body + label | 4370 | **0x17adcc** | join test moves after shared tail, but the 0x200/0x400 `initHudMenu` tails stop merging (two `blx`) |
| 0x800 goto + nested wingman join | 4374 | 0x17adcc | same two-`blx` effect |
| swap 0x200/0x400 cases | 4366 | 0x17aeb4 | survivor 0x400, test order wrong |
| swap + 0x800 goto | 4373 | 0x17adcc | survivor 0x400, join test after shared tail |
| swap + nested | 4370 | 0x17aeb4 | survivor 0x400, test order wrong |
| nested + `hudMenuOpen = 1` in 0x200 body | 4723 (8084 B) | 0x17aeb4 | estimate bump does not flip survivor |
| nested + `__builtin_expect(flags==0,0)` | 6092 (8076 B) | 0x17adcc | wrong size |

Key finding: the wingman-tail -> join-test edge is a **pure source-shape** win
(the nested `else`), independent of the BranchFolder survivor. It keeps the
function at 8080 B. The +4 raw bytes are entirely the branch displacement:
the join test is still emitted at 0x17aeb4 rather than the original 0x17afe2.

## 5. Why the join test still does not move

The `0x800 goto` variant proves the join test *can* be moved after the shared
tail (0x17adcc), but it breaks the `initHudMenu` tail merge: the 0x200 and 0x400
bodies each keep their own `blx` (common tail length 2 < `MinCommonTailLength`
3). The original has a single shared `blx` and the join test after it. Getting
both still needs the 0x400 body to be the tail-merge survivor **and** the join
test to be its layout successor, i.e. the wave-138/139 decoupling problem.

## 6. Open question for the next wave

The join test must be emitted at `0x17afe2` (after the 0x400 body's shared tail).
The 0x800-goto experiment shows the move is possible, but only at the cost of
the tail merge. Candidate next ideas:

1. A `do { ... } while (0)` or `switch` wrapper around the dispatch chain that
   keeps the shared `blx` but changes the join test's layout predecessor.
2. Force the 0x400 body to be the tail-merge survivor while keeping the test
   order `0x200, 0x800, 0x400` (the decoupling problem).
3. Accept the +4 B and look for a size-neutral companion change that removes
   4 B elsewhere without re-rolling the layout.

## 7. Reproduction

```
cp /dev/shm/w8514/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8514/analyze.py        # diff 4363 / 8080, join 0x17aeb4
python3 /dev/shm/w8514/final_nested.py   # landed nested shape: diff 4367 / 8080
python3 /dev/shm/w8514/var800.py         # 0x800 goto: join moves to 0x17adcc
python3 /dev/shm/w8514/var800swap.py     # swap + 0x800 goto: survivor 0x400
```

Scratch: `/dev/shm/w8514/{analyze.py,runs.py,region.py,tryvar.py,final_nested.py,var800.py,var800swap.py,var800nested.py,var_comb.py,pa_*.txt,cfo_*.txt,mbp_*.txt}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; original
wingman tail `0x17bf4a` (`b.w 0x17afe2`), join test `0x17afe2`.
