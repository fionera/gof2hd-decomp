# LevelScript::process — wave 155 / w10101: why the prologue `&Globals::status`
# is r9 and not r4 (the addr vreg is whole-function live; r4 needs a body-local)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10101`, base master
`f4e0ea40b` (wave 154, w10007 landed).  Scratch in `/dev/shm/w10101/`.

## Result (no landed source change)

```
                         HEAD (w10007)     this wave
process raw diffbytes    55,327            55,327
process aligned insns    17,176            17,176
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
```

No size-exact source shape found that moves the prologue status register to
r4.  The variants that DO put `&Globals::status` in r4 all change the size
(see §3) and worsen the aligned metric.

## 1. The prologue register map (target vs HEAD)

The value in the original's r4 is **`&Globals::status`** (the GOT-loaded
address of the global pointer variable), *not* the `Status*` value.  The
pointer value is loaded into r0 twice (once per dispatch test).

```
original (0x137d18)                       HEAD (w10007)
137d26 mov  sl, r0      this   -> r10     137d26 mov  fp, r0      this   -> r11
137d28 ldr  r0,[pc]     canary GOT        137d28 ldr  r0,[pc]
137d2a mov  r4, sl      &m_pLevel -> r4   137d2a mov  r4, fp      &m_pLevel -> r4
137d2c mov  r6, r1      delta  -> r6      137d2c mov  r6, r1      delta  -> r6
137d2e add  r0, pc                        137d2e add  r0, pc
137d30 ldr.w fp,[r0]    guard ptr -> r11  137d30 ldr.w sl,[r0]    guard ptr -> r10
137d34 ldr.w r0,[fp]                      137d34 ldr.w r0,[sl]
137d38 str  r0,[sp,#428]                  137d38 str  r0,[sp,#428]
137d3a ldr.w r0,[r4,#24]!  m_pLevel       137d3a ldr.w r0,[r4,#24]!
137d3e blx  getMessages                   137d3e blx  getMessages
137d42 mov  r5, r0      messages -> r5    137d42 mov  r5, r0      messages -> r5
137d44 ldr  r0,[r4]                       137d44 ldr  r0,[r4]
137d46 blx  getPlayer                     137d46 blx  getPlayer
137d4a strd r4, r0,[sp,#80]  player direct 137d4a strd r4, r0,[sp,#80]
137d4e ldr  r0,[r4]                       137d4e ldr  r0,[r4]
137d50 blx  getEnemies                    137d50 blx  getEnemies
137d54 mov  r8, r0      enemies -> r8     137d54 mov  r8, r0      enemies -> r8
137d56 ldr  r0,[pc]     status GOT        137d56 ldr  r0,[pc]     status GOT
137d58 add  r0, pc                        137d58 add  r0, pc
137d5a ldr  r4,[r0]     &status -> r4     137d5a ldr.w r9,[r0]    &status -> r9
137d5c ldr  r0,[r4]     ptr -> r0         137d5e ldr.w r4,[r9]    ptr -> r4 (CSE)
137d5e blx  getCurrentCampaignMission     137d62 mov  r0, r4
                                          137d64 blx  getCurrentCampaignMission
137d62 cmp  r0,#0                         137d68 cmp  r0,#0
137d64 str.w sl,[sp,#88]  this            137d6a str.w fp,[sp,#88]
137d68 strd r4,r8,[sp,#92]                137d6e strd r9,r8,[sp,#92]
137d6c mov  r0,r8                         137d72 mov  r0,r8
137d6e strd r5,r6,[sp,#72]                137d74 strd r5,r6,[sp,#72]
137d72 beq.w 137e84                       137d78 beq.w 137e8a
137d76 ldr  r0,[r4]     ptr -> r0 (again) 137d7c mov  r0, r4     (CSE'd)
137d78 blx  getCurrentCampaignMission     137d7e blx  getCurrentCampaignMission
```

The +6 B shift of the whole function (`137e8a` vs `137e84`) is exactly the
extra `ldr.w r9,[r0]` (4 B) + `mov r0,r4` (2 B) vs the original's
`ldr r4,[r0]` + `ldr r0,[r4]`.  The mission-0 block is otherwise
**instruction-for-instruction identical** modulo the `this`/canary swap
(`fp`<->`sl`): compare original `137e84..137f02` with HEAD
`137e8a..137f08`.  So matching the prologue allocation would also remove
the +6 shift and align the whole mission-0 block.

## 2. Root cause: the `&Globals::status` vreg is whole-function live

Post-RA MIR (`-mllvm -print-machineinstrs=greedy` / `-print-after-all`,
`Virtual Register Rewriter` dump) for the base source:

```
%11652:gpr = t2LDRi12 %804, 0   ; GOT entry  == &Globals::status
%806:gpr   = t2LDRi12 %11652,0  ; status ptr
...
t2STRi12 %11652, %stack.678     ; [sp+92] = &Globals::status
```

`%11652` is **one vreg used by every `Globals::status` access in the whole
function** (reloaded from `%stack.678` at 2596B, 15320B, 17240B, ... out to
69744B).  Because it is live across the entire body, the greedy allocator
picks r9 (a register free for that whole span) and keeps it live to
`0x13878e` (`ldr.w r0,[r9]`); the original spills it to `[sp+92]` right
after the prologue and reloads it 49 times (HEAD: 25).  r4 is free at
`0x137d5a` in both builds (`&m_pLevel`'s range ends at `0x137d4e`), but the
allocator only reuses r4 when the vreg is **short** (first range only).

## 3. The only shape that yields r4 is a body-local `Status*` (size changes)

Making the whole body use a local `Status *status` shortens the
`&Globals::status` vreg to the init + the two dispatch tests, and the
allocator then does put `&Globals::status` in **r4** with the pointer in r0
— exactly the original's structure.  Measured (all built + linked):

| shape | size | raw | aligned | r4 |
|---|---:|---:|---:|---|
| HEAD (local in dispatch only) | 57232 | 55327 | 17176 | ptr |
| base, no local | 57232 | 55328 | 17185 | r9 |
| body-local + dispatch via `Globals::status` (both) | 57172 | 55365 | 18368 | **addr** |
| body-local + dispatch via `Globals::status` (first only) | 57156 | 55324 | 18705 | **addr** |
| body-local, dispatch via local (all uses) | 57032 | — | 18435 | r2 |

The body-local variants are 60-200 B short and the aligned metric is much
worse (all `Globals::status` GOT loads become stack reloads).  The -60
variant is the closest to size-exact but still 60 B off, so it cannot land
(byte_exact would collapse).

## 4. Sweep of the mission's proposed shapes (all 57,232 B, all r9)

Every shape below is size-exact and byte-identical to HEAD's prologue
(`r9 = &Globals::status`, `r4 = ptr`):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `Status *status` (HEAD) | 57232 | 55327 | 17176 |
| `Status *const status` | 57232 | 55327 | 17176 |
| `auto status` | 57232 | 55327 | 17176 |
| `Status &status = *Globals::status` | 57232 | 55327 | 17176 |
| `Status **` + `(*p)->...` | 57232 | 55328 | 17185 |
| two-statement `Status *status; status = ...` | 57232 | 55327 | 17176 |
| Yoda `0 == ...` | 57232 | 55327 | 17176 |
| inverted `!(... != 0)` | 57232 | 55327 | 17176 |
| `inline getStatusPtr()` helper | 57232 | 55327 | 17176 |
| `do { } while (0)` around mission-0 | 57232 | 55327 | 17176 |
| `__builtin_expect(introMsg->isOver() && ..., 1)` | 57232 | 55327 | 17176 |
| local before accessors | 57292 | 54755 | 17041 |
| local after messages | 57292 | 54756 | 17035 |
| local after player | 57248 | 55375 | 17398 |
| accessors enemies,messages,player | 57208 | 55187 | 17868 |
| `expect(mission==0,0)` | 57424 | 55514 | 22017 |
| `expect(mission==0,1)` | 57400 | 55685 | 22148 |
| `expect(mission==1,1)` | 57476 | 55539 | 22859 |
| `expect(mission==1,0)` | 57408 | 55633 | 21818 |

Note the "local before/after messages" variants (57292) give the best
aligned numbers seen (17,035/17,041) but are +60 B.  A -60 B companion
would make them landable; none was found in this pass.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10101/fdiff.py _ZN11LevelScript7processEi        # 55,327 / 57,232
python3 /dev/shm/w10101/align_diff.py _ZN11LevelScript7processEi  # 17,176
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## 6. Open questions

1. The body-local shape proves r4 is reachable from source, but it needs a
   -60 B companion (or +60 B of missing body code) to be size-exact.  The
   w9905 faithful-loop lever is +112 on this base, so `-60 + 112 = +52`;
   the state-0 `forceUpdate` removal is -124.  No combination hit 0 this
   pass.
2. The `this`/canary r10/r11 swap is correlated: the original allocates
   `this` before the canary pointer, HEAD the reverse.  A shape that
   reorders those two intervals would also be needed for a byte-identical
   prologue even after r4 is fixed.
3. The "local after messages" 57,292 variant (aligned 17,035) is the most
   promising base for a future -60 companion search.
