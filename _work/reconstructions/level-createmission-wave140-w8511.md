# w8511 (wave-140): `Level::createMission` — alien-block `bool` pad experiments,
# tail-loop variants, and the mission-object store order

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8511`, based on master
`d4857f174` (wave 139). Continues `level-createmission-wave139-w8411.md`,
`…-wave138-w8311.md`, `…-wave137-w8213.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; the NDK `objdump` disassembly is the authority. Raw same-address
byte diff is the primary metric (`/dev/shm/w8511/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5763
.text diff 85,473   .ARM.extab 1,395   .rel.plt 0   .note.gnu.build-id 20
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## Where the 5,763 diff bytes live

The diff is dominated by **three phase-shift regions**; each starts with one
small semantic difference and every following instruction is then compared at a
2-byte-shifted address, so hundreds of bytes count as different:

| region | size | first real difference |
|---|---:|---|
| `0x1400`–`0x1a00` | ~1,420 | case-183 switch join branch (+2 B, MBP tie-break) |
| `0x2e00`–`0x3500` | ~1,660 | alien-wave guard: `eor` combined test vs two compares |
| `0x3700`–`0x4200` | ~2,000 | raid block hoists `guns+friendCount+field_164+field_168` before the raid guard |

The remaining ~700 bytes are 1-byte register/literal-pool/spill-slot diffs.

## 1. `bool alienWave` reproduces the `eor` but is 16 B short

The original alien guard (`b2e04`) is a combined test:

```text
subs.w r0, fp, r3        ; i - loopStart
mov    r1, r0
it     ne
movne  r1, #1            ; r1 = (i != loopStart)
ldr    r2, [sp, #60]     ; alienWave
eor.w  r2, r2, #1        ; !alienWave
orrs   r1, r2
bne.w  else
```

`eor #1` requires an i1, i.e. a `bool`. Changing `int alienWave` to `bool`
reproduces it, but clang then drops the redundant re-initialisation that the
original keeps after `nextInt(100)`:

```text
original b0f16..b0f26:            bool variant:
  mov.w r0,#0                       movs r1,#0
  mov.w r1,#0                       str  r1,[sp,#56]
  str   r0,[sp,#60]                 (only pirateFortress re-init)
  mov.w r0,#0
  str   r1,[sp,#56]
  str   r0,[sp,#64]
```

Measured (all size-exact targets are 17,172 B):

| variant | size | raw diff |
|---|---:|---:|
| int alienWave (HEAD) | 17172 | **5763** |
| bool alienWave | 17156 | 12394 |
| bool alienWave + `else alienWave=false` | 17156 | 12394 |
| bool alienWave + 8 volatile pad stores | 17172 | 9802 |
| bool bigAlienWave+alienWave | 17164 | 12378 |
| bool all three | 17304 | 16543 |
| bool all three, reordered decls | 17304 | 16554 |
| bool all three, `bigAlienWave` (no `==1`) | 17304 | 16543 |
| bool alienWave + explicit re-init in else | 17304 | 16543 |

The 8-volatile-pad variant is interesting: it restores 17,172 B and lifts the
alien/tail regions out of the phase shift, but the pad instructions themselves
do not match the original's re-init, so the total is still worse than HEAD
(9,802 vs 5,763). No source spelling found that both keeps 17,172 B and emits
the original's 6-instruction redundant re-init.

## 2. Tail loop: the `volatile unsigned n` latch is still the best formulation

The original tail condition re-reads `this->enemies->size()` every iteration
(`ldr r0,[r8,#248]; ldr r0,[r0,#0]; cmp r9,r0`). Measured:

| variant | size | raw diff |
|---|---:|---:|
| `volatile unsigned n = size(); for (i<n)` (HEAD) | 17172 | **5763** |
| `for (i < this->enemies->size())` (faithful) | 17164 | 5896 |
| `unsigned n = size(); for (i<n)` (non-volatile) | 17172 | 5769 |
| `volatile n` + faithful condition | 17176 | 6458 |
| `volatile n; (void)n;` + faithful condition | 17180 | 6462 |
| `int i` faithful | 17164 | 5896 |
| `Level *self`, `Globals *g`, faithful | 17168 | 5882 |

The faithful loop puts `this` in `sl` (original: `r8`) and is 8 B short; the
volatile latch keeps `this` in `r8` but loads the bound from `[sp,#312]`
instead of re-reading the array. No formulation keeps both.

## 3. Mission-object store order: tail-merge is a register-map artifact

The case-1 `objectivesA = new Objective(7, attackerCount, this)` store is
inline in the original (`b20f6 str r5,[r4,#40]; b.w exit`) but tail-merged in
ours (`b20f6 b.w b41e4`). The tail store is `str.w r5,[r8,#40]` in the original
(`this`=r8) and `str r5,[r4,#40]` in ours (`this`=r4), so the two stores are
not mergeable in the original. Folding the temporary back into a single
statement, or introducing `Objective *objective` / `Level *self` aliases, does
not change the code (measured 5763 in all cases).

## 4. `shipRace` refactor: same total, different distribution

The original computes the alien ship race as two values:
`race = alienRace ? localRace : 2; shipRace = alienRace ? race : stationRace;
if (localRace==0) shipRace = race;` (`b30ba..b30d4`). Writing this form
instead of the current `if (!alienRace && localRace != 0) race = stationRace;`
gives the original's `movgt r1,r0` (instead of `movgt r1,r2`) but the total
raw diff is unchanged at 5,763 (the 1-byte register win is offset elsewhere).
Kept out of the tree to avoid churn.

## 5. Raid-block hoist

`int raidStart = guns + friendCount + field_164 + field_168;` before the raid
loop still hoists the computation above the raid guard (17,180 B, raw 5,887).
The original computes it at `b37e2` after the raid-specific spawn logic; this
is a scheduling/register-pressure decision, not statement order.

## Conclusion

No source change this wave beats HEAD. The three phase-shift roots remain:
(1) the case-183 switch join branch (MBP tie-break, unreachable from a plain
switch/if-chain per wave 138), (2) the alien guard's `bool` `eor` form, which
costs 16 B of redundant re-init, and (3) the raid-loop bound hoist. The
baseline (raw 5,763, 17,172 B) is retained.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8511/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8511/buckets.py _ZN5Level13createMissionEv 0x400
python3 /dev/shm/w8511/regions.py _ZN5Level13createMissionEv
python3 /dev/shm/w8511/align.py
```

Scratch: `/dev/shm/w8511/{Level.base.cpp,fdiff.py,buckets.py,regions.py,
align.py,cmp.py,sizedelta.py,orig_full.asm,ours_int.asm,ours_bool.asm}`.
