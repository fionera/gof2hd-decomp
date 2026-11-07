# MGame::OnTouchEnd — wave 140 (w8513): faithful 96||127 split landed at 8080 B

Status: source change landed, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8513`,
base `d4857f174`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8513`. Full gate green: byte 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, RATCHET PASS.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

## 1. Next-largest hunk after the PredBB family: the merged `96 || 127` case

Instruction-level sequence alignment (`difflib` on the objdump mnemonics, not the
same-address byte diff) confirms the wave-139 classification. The same-address
diff hunks 1-7 are almost all pure address shifts (escort `new Mission(183)`,
`enableParticleEffects` block, campaign-chain case bodies). The one genuine
content block is the base's merged clause:

```
base source:  else if (isCampaignMission() &&
                       (getCurrentCampaignMission() == 96 ||
                        getCurrentCampaignMission() == 127)) { <body> }
base asm:     cmp #96; beq body; cmp #127; bne next   (one call to getCurrentCampaignMission)
```

The original has **two separate `else if` clauses** at the source positions
`96, 100, 110, 120, 126, 127, 134`:

```
17b71c  cmp #96;  bne.w 17b93c     ; 96 test, body at 17b722
...
17bc34  cmp #127; beq.w 17b722     ; 127 test, branches back to the 96 body
```

The two identical bodies are tail-merged by clang into the single shared block
at `17b722`. So the faithful source is a duplicated body in a `== 127` clause
placed after `== 126`.

## 2. The split is intrinsically +8 B; paired with three content fixes it lands at 8080

The split alone (`S`) is **8088 B** (raw diff 4674): the 96 test's false edge and
the 127 test's backward branch change encoding widths. The base is a strong local
optimum, so the split must be paired with other *faithful* content fixes that
also match the original and remove the 8 B. Four changes landed:

| change | source | original asm |
|---|---|---|
| `S` split 96/127 | `== 96` clause + separate `== 127` clause after 126 | 96/127 separate tests, shared body |
| `F` `hasFailed()` | drop `== true` | `cmp r4,#0; beq` (not `cmp #1`) |
| `M` `menuWindow->OnTouchEnd(...)` | drop `== true` | `cmp r0,#0; beq` (not `cmp #1`) |
| `C` mission selection | re-call `getCampaignMission()` instead of reusing a `campaign` temp | 3rd `blx getCampaignMission` |

The 64-combination sweep (split × {barewon, barefailed, baremenu, mining, camp})
shows only two size-8080 combinations; `S+F+M+C` has the lower raw diff.

## 3. Measured result

```
base:  8080 B  raw same-address diff 4363
S+F+M+C: 8080 B  raw same-address diff 5816
```

The raw diff rises because the content fixes shift the whole campaign chain by
several bytes (the same-address metric penalises shifts). The **instruction
multiset is now strictly closer to the original**:

| count | orig | base | landed |
|---|---:|---:|---:|
| `Mission::isCampaignMission` calls | 23 | 22 | **23** |
| `Status::getCampaignMission` calls | 3 | 2 | **3** |
| `cmp r4,#1` (hasFailed `== true`) | 2 | 3 | **2** |
| `cmp r0,#1` (hasWon/menu `== true`) | 24 | 26 | **25** |

The one remaining `cmp r0,#1` is the `won` expression's `hasWon() == true`
(residual #2, wave-139 note §1); fixing it alone is -16 B and re-rolls the failed
region, so it was left for the next pass.

## 4. Reproduction

```
cp /dev/shm/w8513/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
# apply S+F+M+C (see /dev/shm/w8513/variants.py FUNCS)
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8513/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 5816 / 8080
cmake --build cmake-build-match --target check -j6            # RATCHET PASS
```

Scratch: `/dev/shm/w8513/{fdiff.py,variants.py,variants2.py,seqdiff.py,widthdiff.py,orig.dis,sfmc.dis}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
