# `Gun::calcCharacterCollision` pass 14: `dmg` is not a source local, and the shipped
# `F P H L K` tail carries the *high-pressure* `F H K L P` allocation (wf112, wave 101)

Worker: wf112. Branch: `wave101-wf112`, base master `3856949ab`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 instructions.
Scratch: `/dev/shm/wf112/`.

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail, §2) is still the best measured
shape: **90.0 %**, 1984/1984 B, 650/650 normalized instructions (official verifier).

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `3856949ab`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0, `RATCHET PASS`.

This pass contributes one new *source fact* and one new *measurement*:

1. **The original has no `int dmg = this->damage;` local.** The shipped binary reloads
   `this->damage` (`ldr.w r1,[r9,#96]` / `vldr s0,[r9,#96]`) at **all three** damage sites
   (`0x1532b8`, `0x1532d4`, `0x153308`), whereas the tree body caches it in `sl`
   (`ldr.w sl,[r9,#96]` at `0x153282`, then `mov r1,sl` at `0x1532fe`). The faithful source is therefore
   `enemy->damage(this->damage, …)` in the two `else` arms.
2. **The shipped tail is `F P H L K` but the shipped register map is the tree's `F H K L P`
   map.** Re-measured against the full disassembly: the shipped keeps the tail constant `1` in
   the *caller-saved* `r1` (`movs r1,#1` at `0x153316`, dead before the `setHitVector` call),
   yet `this=r9`, `enemy=r8`, `i3=r4`, `td=r6`, `explosive=[sp,#20]`, `td=[sp,#24]`. The tree
   body has `this=r9`, `enemy=r8`, `i3=r4` but `fp=1` (constant live across the calls),
   `i=[sp,#40]`, `td=[sp,#20]`, `explosive=[sp,#24]`.

So the residual is *not* just the tail statement order: the shipped is a **hybrid** — the
`F P H L K` instruction order with the higher-pressure `F H K L P` register assignment.

## 1. The two builds, re-verified

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`, `H` = `setHitVector`,
`L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

| body | tail order | `dmg` local | pct | size | insns | `this` | `enemy` | `i` | `i3` | `fp` |
|---|---|---|---:|---:|---:|---|---|---|---|---|
| tree (HEAD) | `F H K L P` | yes | **90.0** | **1984** | 650 | r9 | r8 | [sp,#40] | r4 | 1 |
| tree, faithful | `F H K L P` | no | 84.0 | 1996 | 652 | r9 | r8 | [sp,#40] | r4 | 1 |
| shipped order | `F P H L K` | yes | 64.8 | 1912 | 638 | r4 | r6 | r8 | r5 | — |
| shipped order, faithful | `F P H L K` | no | 64.1 | 1920 | 641 | r4 | r6 | r8 | r5 | — |
| shipped binary | `F P H L K` | **no** | — | **1984** | **650** | **r9** | **r8** | **[sp,#40]** | **r4** | **i** |

The shipped's `fp` holds `i` in the tail (`str.w r1,[r0,fp,lsl#2]` at `0x153346`) and is
reloaded from `[sp,#40]` at `0x153198`/`0x153262`; the constant `1` is rematerialised in `r1`.
The tree body's `fp` holds the constant `1` across the `H`/`K` calls and `i` is read from
`[sp,#40]` into `r2` (`ldr r2,[sp,#40]` at `0x153330`).

## 2. Why the two `F P H L K` builds are 12 instructions short

`F P H L K` keeps `1` live only across the `P` branch, so the greedy allocator has no reason to
put it in a callee-saved register; the whole allocation then collapses to the low-pressure map
(`this=r4`, `enemy=r6`, `i=r8`, no `[sp,#40]` spill, 638/641 insns). The shipped instead has the
*high-pressure* map — i.e. the `F H K L P` allocation — which is 12 instructions longer.

LLVM-7.0.2 `-debug-only=regalloc` (whole-module IR, `-relocation-model=pic`):

| body | `$r11` split line | outcome |
|---|---|---|
| `F H K L P` | `static = 21.32312512, v=69, total = 110.4083111` (19 bundles) | isolates `%bb.92`, creates `%458 [16024r,16128r)` weight `2.818017e-01`, evicts `this` from `$r4` → `this=r9` |
| `F P H L K` (no `dmg`) | `static = 21.32312512, v=69, total = 98.31180978` (24 bundles) | `%bb.92 … intf invalid-invalid, straight through`, no eviction → `this=r4` |

`Cost of isolating all blocks = 105.0344503` in both. The `total` is the same `static` plus the
through-bundle weights; the extra bundles `EB#34..#38` (the damage / `isAsteroid` / `explosive`
region) are *cheap* in the `F P H L K` body, so its total falls below the isolation cost and the
high-weight `%458` copy is never materialised.

## 3. New source-shape probes measured this pass

All measured with the official verifier-equivalent scorer
(`/dev/shm/wf112/fastscore.py`, exact copy of `tools/verify/verify.py` scoring) after
`cmake --build cmake-build-match --target link_so -j8`.

### 3.1 `dmg` handling

| variant | pct | size |
|---|---:|---:|
| `F H K L P` + `int dmg` (HEAD) | **90.0** | 1984 |
| `F H K L P`, `this->damage` at all 3 sites | 84.0 | 1996 |
| `F H K L P`, `dmg` used at all 4 sites | 84.6 | 1984 |
| `F P H L K` + `int dmg` | 64.8 | 1912 |
| `F P H L K`, `this->damage` at all 3 sites | 64.1 | 1920 |
| `F P H L K`, `dmg` at all 4 sites | 64.7 | 1916 |

### 3.2 `__builtin_expect` grid on `F P H L K` + no `dmg` (best per site)

| site | best value | pct | size |
|---|---|---:|---:|
| `mine` (radius `*= 5`) | 0 | **80.4** | 1996 |
| `scatter` (radius block) | 1 | 79.1 | 1992 |
| `playergun` | 1 | 64.1 | 1920 |
| `impact` | 1 | 64.1 | 1920 |
| `owner` | 1 | 63.2 | 1920 |
| `distance` | 1 | 62.5 | 1920 |
| `eoc` | 0 | 61.9 | 1920 |
| `hitpoints` | 0 | 61.4 | 1920 |
| `active` | 0 | 61.1 | 1920 |
| `geometries` | 1 | 59.6 | 1908 |
| `mine_outer` | 1 | 59.5 | 1920 |
| `explosive` | 1 | 58.5 | 1992 |
| `isAsteroid` | 0 | 57.1 | 1928 |

### 3.3 `td` / declaration / hoist / scope probes (all on `F P H L K`)

| variant | pct | size |
|---|---:|---:|
| `td` direct (`this->targetDir = …`) | 64.1 | 1920 |
| `Vector &td = this->targetDir` | 64.1 | 1920 |
| `td` inside the scatter block | 64.1 | 1920 |
| `td` declared before `lastHitKIPlayer` | 64.1 | 1920 |
| `td` declared at function top | 64.8 | 1912 |
| `explosive` declared at first use | 64.8 | 1912 |
| `explodeOnContact` as `bool` | 64.8 | 1912 |
| `explodeOnContact` inlined (`if (bomb || mine)`) | 64.8 | 1912 |
| `const bool mine/scatter` | 64.8 | 1912 |
| `Matrix view` hoisted to function scope | 64.0 | 2032 |
| `Matrix view` copy-init at function scope | 52.7 | 1956 |
| `Matrix view` declared first in the geometry block | 63.1 | 1920 |
| `view` after the two `SetAnimationState` calls (HEAD spelling) | 64.8 | 1912 |
| `int dmg` hoisted to function top | 64.8 | 1912 |
| `position/velocity/enemyPosition` hoisted to function scope | 61.3 | 1996 |
| `radius` / `outer` / `distance` / `enemy` / `ki` hoisted | 64.1 | 1920 |
| `idx` / `ki2` / `pg` / `lvl` locals in the tail | 64.8 | 1912 |
| `if (!outer || !outerCollide)` instead of `!(outer && …)` | 64.8 | 1912 |
| `impact` single check / `Sparks *imp` local | 64.9 / 61.3 | 1912 / 1940 |
| `signed i` / `signed e` / `signed k` / `size_t i` | 64.8 | 1912 |
| `unsigned radius` | 63.8 | 1912 |
| `explosive`/`mine`/`scatter`/`bomb` as `int` | 64.8 | 1912 |
| `explosive` assigned after the `bomb||mine` block | 60.2 | 1904 |

### 3.4 `F H K L P` (tree) probes

| variant | pct | size |
|---|---:|---:|
| `eoc_direct` / `eoc_ternary` | **90.0** | 1984 |
| `const int dmg` | **90.0** | 1984 |
| `td` direct | **90.0** | 1984 |
| `if (!outer || !outerCollide)` | **90.0** | 1984 |
| `view` after the animation calls | **90.0** | 1984 |
| `idx`/`lvl` locals in the tail | **90.0** | 1984 |
| `td` declared at function top | 66.2 | 1964 |
| `int dmg` at all 4 sites | 84.6 | 1984 |
| no `dmg` local | 84.0 | 1996 |
| `view` first in the geometry block | 73.5 | 1968 |
| `explosive` assigned after `bomb||mine` | 69.6 | 1952 |
| `impact` single check | 84.4 | 1980 |

### 3.5 Randomised multi-knob search

120 samples over {10 tail orders} × {`dmg` on/off} × {`td` top} × {`eoc` first/direct} ×
{`explosive` after} × {`Matrix` hoist} × {one `expect` site}: best **89.7 / 1984**
(`K F H L P`), nothing above the HEAD 90.0. (`/dev/shm/wf112/search_out.json`.)

## 4. Conclusion

The shipped body is `F P H L K` with **no `dmg` local**, yet it carries the *high-pressure*
`F H K L P` register map (`this=r9`, `enemy=r8`, `i=fp`, `i3=r4`, `td=r6`, `explosive=[sp,#20]`,
`td=[sp,#24]`). Every faithful spelling of `F P H L K` compiles to the low-pressure map
(`this=r4`, `enemy=r6`, `i=r8`) and loses 12 instructions. The residual is therefore a single
LLVM-7.0.2 greedy-allocator region-split decision (`$r11` total `98.31` vs the isolation cost
`105.03`) that no source-level shape tried in passes 1–14 (w3805 … w5013, this pass) can flip
without also changing the emitted instruction order. The tree body remains a strong local
optimum at **90.0 % / 1984 B**.

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

Scratch harness (`/dev/shm/wf112/`):

| file | purpose |
|---|---|
| `Gun.cpp.orig` | pristine `src/game/weapons/Gun.cpp` at HEAD |
| `body_head.txt` | HEAD body (`F H K L P`) |
| `body_tail.txt` | shipped-order body (`F P H L K`) |
| `try.sh` / `try2.sh` … `try16.sh` | splice a body, `link_so`, score |
| `gen2.py` … `gen16.py` | the knob generators in §3 |
| `fastscore.py` | official-verifier-exact scorer |
| `search.py` / `search_out.json` | the 120-sample randomised search in §3.5 |
| `dump.py` | print the verifier-normalised disassembly of either side |
| `fnd.ra`, `head_702.ra` | llc-7.0.2 `-debug-only=regalloc` traces |
| `orig.dis`, `head.dis`, `hnd.dis`, `fnd.dis` | normalised disassembly of the shipped / HEAD / HEAD-no-dmg / FPHLK-no-dmg builds |

## 6. Files changed

None under `src/` (note only). Gate: byte 4500 / linked 4500, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
lint CLEAN, drift 0.
