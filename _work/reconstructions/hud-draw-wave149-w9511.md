# Hud::draw — wave 149 (w9511): the fp/r9 swap is caused by the `dividerOffset` pivotY anchor

**Status:** analysis-only, 2026-11-08 (wave 149). Worktree `wt/w9511`,
base `master fece77a64` (wave 148). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **1325** (unchanged, no source landed) |
| aligned (instruction-index) diff | **249** | **249** |
| size | 9332 | 9332 |
| `.ARM.extab` | 0x26298 (156,312) | 0x26298 (156,312) |

Entry gate: `check` green at base (`byte_exact 4507`, `linked_exact 4507`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

---

## 0. TL;DR

1. **The fp/r9 swap is not a dock2-local RA decision — it is caused by the
   `dividerOffset` pivotY anchor in the `hasArmorRegen` block.** That anchor
   (`DrawRegion2D(..., 0.0f, 0, dividerOffset, ...)`, src line ~1191) keeps
   `dividerOffset` live in `sl` through the gamma block. Because `sl` is busy,
   `armorY` (unsigned short) is coloured into `r8` instead of the original's
   `sl`; `r8` is then unavailable for the `DrawImage2D` function pointer, which
   moves to `fp`; that evicts `ego` from `fp` and it ends up in `r9` (and
   `Canvas` in `fp`) — exactly the 9368-shape swap.
2. **The original does NOT have that anchor**: original disasm at
   `0x163ff4` is `strd r4, r4, [sp, #12]` (pivotY = 0), while our base is
   `strd r4, sl, [sp, #12]` (pivotY = dividerOffset). The original's `sl` is
   free after `0x163e54` and is reused for `armorY` at `0x163f0a`
   (`ldrh.w sl, [r1]`), so `r8` keeps the function pointer and `fp` keeps `ego`.
3. **But removing the anchor alone is not enough**: base + pivotY=0 is only
   **9312 B** (20 B short) and its dock2 still has `egoslot=108` (frame
   permutation), raw 8256. The anchor is currently buying the 20 bytes. The
   original gets those 20 bytes from a *different* (still unidentified) source
   difference, not from the anchor.
4. **All 24 dock2 declaration orders and every filled/y/top placement tried
   still give `egoreg=r9` when `filled` is inline**; only `filled` late gives
   `fp`, and that also re-splits the dock2 LSDA (tlen 459/rows 104 vs original
   454/103). The base (order `barYOffset,barH,text,halfW`, `top` late,
   `filled` late, anchor) remains the best raw/align variant.

## 1. Root-cause chain (verified by disasm)

Original (`0x163f0a`): `ldrh.w sl, [r1]` -> `sl = armorY`; `r8` holds the
DrawImage2D fn-ptr (`blx r8`), `fp = ego` (`mov r0, fp` at `0x163f5a`).
Our base (`0x163f0a`): `ldrh.w r8, [r1]` -> `r8 = armorY`; `sl` still holds
`dividerOffset`; the fn-ptr goes to `fp` (`blx fp`), forcing `mov sl, fp` and
`mov r0, sl` for `getHullDamageRate`, and later the dock2 `mov r0, r9` for
`ego` (r9) with `Canvas` in `fp`.

The single instruction that proves the anchor is the cause is
`0x163ff4`: original `strd r4,r4` (pivotY=0) vs base `strd r4,sl`
(pivotY=dividerOffset).

## 2. Measured variants (full build; base = 1325 / 249 / 9332)

| variant | size | raw | align | egoslot | egoreg |
|---|---:|---:|---:|---:|---|
| **base (landed)** | **9332** | **1325** | **249** | **116** | **fp** |
| base order, filled late, top late (= base) | 9332 | 1325 | 249 | 116 | fp |
| faithful order `text,halfW,barH,barYOffset`, filled late, top late | 9332 | 1352 | **230** | 116 | fp |
| faithful order, filled late, top after decls | 9332 | 1429 | 228 | 116 | fp |
| 9368 shape (`THBY`, y before SetColor, filled inline) | 9368 | 8001 | 895 | 120 | r9 |
| 9368 shape + all 24 decl orders | 9368-9372 | 7991-8028 | 895-958 | 120 | r9 |
| 9368 shape + top first | 9368 | 7998 | 901 | 120 | r9 |
| 9368 shape + ego local/ref, +expect(show/prog/fade) | 9368-9376 | 8001-8074 | 895-928 | 120 | r9 |
| 9368 shape + pivotY=0 | 9368 | 8001 | 894 | 120 | r9 |
| 9368 shape + pivotY=armorY | 9336 | 4433 | 1096 | 108 | fp |
| base + pivotY=0 | 9312 | 8256 | 981 | 108 | fp |
| base + pivotX=dividerOffset (pivotY=0) | 9336 | 7312 | 454 | 116 | fp |
| base + `__builtin_expect(show,1)` | 9332 | 1325 | 249 | 116 | fp |

`expect(show,1)` is metric-neutral but changes the first dock2 LSDA pad from
`2432` to the original's `2434` (LSDA then `205e 4/2434, 20e4 2a/2440,
2130 7e/2444`). It cannot be landed without a lint waiver: `goal_lint`
counts `__builtin_expect` as `arm_builtin` (Hud.cpp baseline 0), so it is a
lint regression unless a same-line `// lint: arm_builtin` waiver is added.

## 3. Open questions for the next wave

1. **Find the original's other 20 bytes.** The faithful source has pivotY=0
   and 9332 B; our pivotY=0 build is 9312 B. The missing 20 B must be a
   different source difference (likely in dock1 or the `y` handling), not the
   pivotY anchor. If found, the faithful shape should get `fp=ego` for free.
2. **`armorY` register is the pivot**: the original colours `armorY` into `sl`
   and the fn-ptr into `r8`. Any source change that frees `sl` after
   `0x163e54` *and* keeps the 20 bytes will fix the swap.
3. `__builtin_expect(show,1)` is a free LSDA-pad improvement if a lint waiver
   is acceptable.

## 4. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9511
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9511/measure.py            # size/raw/align/egoslot/egoreg
python3 /dev/shm/w9511/lsda.py <so> _ZN3Hud4drawExxP9PlayerEgobjj
```
Scratch harness in `/dev/shm/w9511`: `measure.py`, `lsda.py`, `dis.py`,
`gen.py` (dock2 shape generator: declaration order, `top` position, `y`
position, `filled` mode), `sweep*.py`.
