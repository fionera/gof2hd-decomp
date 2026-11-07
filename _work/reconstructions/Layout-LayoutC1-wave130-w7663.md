# `Layout::Layout` C1/C2 — wave 130 / worker w7663

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-129 master `003a9623a` (7072 B / 48.943802311678 % unrounded).

## Result — size-exact source change, gate green

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 48.943802311678 % | **48.864089278597 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.

This wave replaces three field-major ternary groups in the non-HD common1
block with the original's nested `if/else` branch trees decoded from the
Ghidra decompile (`/tmp/decomp_layout.txt`, function `0xe1028`):

1. `0x154/0x158/0x15c` — four-arm tree (`iPadLarge` / `iPad` / `!n9` / `n9`)
   with the shared `v158` temporary and the original's `(int)(float)` round
   trips on `0x158` and `0x15c` (w7643's "clearest example").
2. `0x164/0x168` — shared `f168` temporary with the `goto` join at the
   original's `LAB_000e1e38`; this reproduces the duplicated `0x164` stores.
3. `0x16c/0x170` — shared `v170` temporary, four arms, with the original's
   `(int)(float)` round trip on `0x170`.
4. `0x174/0x178/0x17c` — four-arm tree (`iPadLarge` / `iPad` / `!n9` / `n9`).
5. `0x19c` and `0x1a0` — five-arm trees (`iPadHD` / `iPadLarge` / `iPad` /
   `!n9` / `n9`).

The `0x19c/0x1a0` pair is the size anchor: it adds the 48 B needed to keep
the body at exactly 7072 B after groups 1–4 shrank it to 7024 B.  The
`0x16c/0x170` tree alone was the anchor in the earlier 7064 B state (it
adds 8 B); the final landed combination uses both.

## 1. New measurements (real `link_so` builds, exact unrounded ratio)

All variants were built on top of the wave-129 source. `score.py` is the
exact single-function scorer (verify.py `Resolver` + `reconcile` + `difflib`).

| variant | size | ratio % |
|---|---:|---:|
| HEAD baseline | **7072** | **48.943802** |
| + groups 1+2 (`0x154`, `0x164`) | 7064 | 48.973081 |
| + group 3 (`0x16c/0x170`) | **7072** | **48.864089** |
| groups 1+2 + `0x174/0x178/0x17c` flat if/else | 7016 | **49.200000** |
| groups 1+2 + `0x174` + `0x16c` | 7024 | 49.050570 |
| groups 1+2 + `0x174` + `0x2d4/0x2d8` if/else | 7000 | 49.179015 |
| groups 1+2 + `0x174` + `0x7c/0x80/0x84/0x88` if/else | 7040 | 47.833899 |
| groups 1+2 + `0x174` + `0x184/0x188/0x18c` if/else | 7024 | 47.862565 |
| groups 1+2+3+4 + `0x19c/0x1a0` if/else (landed) | **7072** | **48.864089** |
| full common1 tail tree (incl. `0x1b0..0x1e0`, HD block) | 7152 | 38.796516 |
| full common1 tail tree, ballast removed | 7104 | 42.071840 |

The **`0x174/0x178/0x17c` flat tree is the strongest ratio win** (+0.256 %),
but it shrinks the body by 56 B. The `0x16c/0x170` tree is the only
reconstruction tested that restores size exactness without the `int v`
ballast being touched (the ballast is still present in the landed source).

## 2. Why the `0x174` win cannot be landed alone

The `0x174` group is a four-arm tree in the original (`iPadLarge`, `iPad`,
`!n9`, `n9`) that shares `bVar42` with the following `0x184..0x1ac` groups.
Writing it as a standalone flat `if/else` lets clang merge the arms and
removes 56 B. Re-integrating it with the later groups via the original's
`goto` joins (`LAB_000e1f4e`, `LAB_000e1f84`, `LAB_000e1f64`,
`LAB_000e1f9e`) grew the body to 7152 B and dropped the ratio to 38.8 % —
the HD block and the `0x1b0..0x1e0` SLP block were both displaced from the
HD arm, so the instruction stream diverged.

## 3. Open questions

* The `0x174` group needs 48 B of real duplicated stores from the
  `0x184..0x1ac` tree to become size-exact. Every flat transcription of that
  tree measured here either shrank (7024 B) or diverged (6904 B). The next
  pass should transcribe the `0x184..0x1ac` tree **with the original's
  `bVar42` live range and `goto` joins**, but keep the HD values in the HD
  arm instead of an explicit common1 `if (iPadHD)` block.
* The `int v` ballast is still present. Removing it (`0xa8` int-typed form)
  costs 40 B; the `0x174` tree supplies 56 B of real code, so the two
  together would be 16 B over. The correct combination is likely
  `0x174` + a partial `0x184` tree + ballast removal.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7663
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 /dev/shm/w7663/score.py                        # 7072 / 48.864089
```

Scratch harnesses (not committed, `/dev/shm/w7663/`): `score.py` (exact
single-function scorer), `parse.py` (capstone disassembly with literal-pool
skipping), `emu.py` (the `layout_ctor/emu.py` flag emulator retargeted at
this worktree's `tools/verify`), `Layout.base.cpp`, `Layout.16c.cpp`
(landed), `Layout.174.cpp` (7016 / 49.2), `Layout.7024.cpp`
(7024 / 49.05), `tree.cpp` (full common1 tail tree).
