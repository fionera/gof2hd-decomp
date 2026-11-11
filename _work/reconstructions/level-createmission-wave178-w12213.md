# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 178 / w12213

Target: `src/game/world/Level.cpp`, function at `0xaffd8`, size **17172 B** (`0x4314`).
Branch `wave178-w12213`, base master `1b48f990f`.

## Result

No size-exact raw win landed. `src/` is left at the base. The single best lead is the
`extraAliens` `__builtin_expect` removal, which cuts raw **5143 → 4485 (−658)** but grows the
function **+4 B** (17176). Every compensation tried in this pass either missed the size target
or raised raw above the base.

Base metrics (unchanged, verified after restore):
```
raw=5143 aligned=952 size=17172/17172 insn=6208/6197
```

## The best lead: drop the `extraAliens` expect

`src/game/world/Level.cpp:2052`

```cpp
-    if (__builtin_expect(extraAliens >= 1, 1)) { // lint: arm_builtin
+    if (extraAliens >= 1) {
```

Measured (real build, `tools/fn_diff.py`):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 5143 | 952 | 17172/17172 |
| **no `extraAliens` expect** | **4485** | 975 | **17176/17172 (+4)** |
| no-ea + `case 0` store order 0,1,2 | **4483** | **968** | 17176/17172 (+4) |
| no-ea + no tail hoists | 4419 | 959 | 17176/17172 (+4) |

Why it helps: removing the expect changes clang's global block placement. It fixes the
`raid = raidRoll < (security==0?90:…)` block order at `0xb04be` and, more importantly, collapses
the 249-byte raw run at `0xb2f24` (see below). Why it is size-fatal: the resulting function uses
**7 literal-pool words** where the original uses **6** (pool at `0xb42d4`; orig `b42d4..b42ec`,
ours `b42d4..b42f0`). Base also uses 7 words but compensates with 4 fewer code bytes (base code
ends `0xb42d0`, orig `0xb42d4`); no-ea keeps orig's code length, so the extra word is exposed.

## The 249-byte run at `0xb2f24` (bigAlienWave cargo tail)

```
ORIG b2f22: add r0, r8
     b2f24: add.w r3, r1, #80      ; r3 = &enemies[i]->cargo
     b2f28: b.n b3194
OURS b2f22: add r0, r8
     b2f24: adds r1, #80           ; r1 = &enemies[i]->cargo
     b2f26: b.n b3194
```

`add.w r3` is 4 B, `adds r1` is 2 B, so every byte after it shifts by 2 until the next
re-sync (a `nop` at `0xb3176` in base pads the literal pool back to 4-byte alignment). This is a
pure register-allocation choice (r3 vs r1 for `&cargo`). Source:

```cpp
delete (*this->enemies)[i]->cargo;
(*this->enemies)[i]->cargo = nullptr;
(*this->enemies)[i]->field_0x70 = 0;      // bigAlienWave branch, ~line 1915
```

Tried and failed to force `add.w r3`: local `KIPlayer *e`, `Array<int> *&cargo`, `Array<int> **cargo`,
`= 0`, reordering `field_0x70` first — all either 5143/17172 or much worse (16200–16300 / 17200+).

## The +4 literal: `Globals::rnd`

The extra pool word is the GOT slot for `Globals::rnd` (`0x2101f6`), loaded at `0xb40ec` into r9 in
the `case4Tail` hunter loop. The original instead reuses a stack copy: `ldr r0,[sp,#120]; ldr r6,[r0]`
at `0xb417a`. The source hoist

```cpp
AERandom *rnd = Globals::rnd;   // line 2119, top of case4Tail
```

is what materialises the extra GOT literal. Removing it (inline `Globals::rnd`) gave 4664/17184
(+8, worse); moving it after `ArraySetLength` gave 4489/17176; moving it to the function top gave
16493/17288. None removed the literal.

## Size-exact compensations tried on top of no-ea

`raid` `bool`→`int` is the only −4 change found, but it costs raw:

```
noea + raid int            raw=5815 aligned=992 size=17172/17172   (base is 5143)
noea + pirateFortress int  raw=12258                 size=17172
noea + c184 getDifficulty() removed  raw=8035        size=17168 (−8, not −4)
base + raid int            raw=5914                  size=17168
base + getDifficulty() removed       raw=8042        size=17164
```

`raid int` shaves 4 B because `if ((security==3) & raid)` compiles to `tst r1,r0; beq` (4 B) instead
of `ands r0,r1; cmp r0,#1; bne` (6 B), plus a second −2 elsewhere. The original uses the
`ands+cmp` form, so `raid` really is `bool`; the −4 is wrong codegen that happens to compensate.

Other size-exact variants measured (all raw ≥ 5143):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| no `comingFromAlienWorld` expect | 5162 | 971 | 17172 |
| `bool blueprint` | 6805 | | 17172 |
| `x` unsigned in else branch | 5143 | 953 | 17172 |
| `Coords6`→`int[6]`, `RaidCoords3`→`int[3]` | 5143 | 952 | 17172 |
| `cargo`/`crateCaptured` swap | 5144 | 952 | 17172 |

## Flag / stack-slot mismatches (dominant raw contributors)

Stack slots still differ from the original and drive most of the 5143:

- Flags: orig `pirateFortress=sp+56, alienWave=sp+60, bigAlienWave=sp+64`
  (reverse declaration order). Ours: `bigAlienWave=sp+56, pirateFortress=sp+60, alienWave=sp+64`.
  Declaration order `BAP` is best; `BPA/ABP/APB/PBA/PAB` give raw 7392/10102/10416/12268/12139.
- `extraAliens` vs `stationPirates`: orig `extraAliens=sp+44, stationPirates=sp+48`;
  ours swapped (`extraAliens=sp+48, stationPirates=sp+44`). Moving the `extraAliens` declaration
  earlier did not change the assignment (still 5143).
- `raid` is at `sp+84` in both, and every access is a word `str`/`ldr` (clang uses word slots for
  local `bool` at `-Oz`), so the word stores are not evidence of `int`.

## Other measured variants (all size 17172 unless noted)

```
no `blueprint` expect            12762 / 1047 / 17156
`blueprint` expect(...,0)        11519 / 1960 / 17160
no `extraAliens` expect(...,0)   13869 / 2638 / 17180
flags `int` / `char` / `unsigned` 5219 / 5937 / 5219  (17176 / 17184 / 17176)
no rnd hoist / globals / enemies / all   5241 / 5168 / 5151 / 5260  (17180 / 17172 / 17172 / 17172)
all-`102` switch store order     11352 / 1029 / 17164
`case 0` store orders 012/021/102/120/201/210  → 4483/4504/4488/4504/4502/4497 (all +4)
swap alienGun/pirateFortress blocks      6149 / 1212 / 17156
delete-cargo `local2`            6216 / 788 / 17164
delete-cargo `if != nullptr`     16298 / 1896 / 17208
alienChance/extraAliens moved early      13711 / 1032 / 17184
no-local in else (station->setPosition)  6205 / 992 / 17164
```

## EH / LSDA

LSDA is already the right shape: cleanup-only, 148 call-site rows, `cs_len` 933 vs orig 934.
`.ARM.extab` is size- and base-exact. The landing-pad table at `0xb41f0..0xb42ec` differs by
±2/±4/±6 in the cleanup ordering; the LSDA landing-pad offsets are all +4 vs orig, consistent with
the extra code byte. `try/catch` is size-fatal (17184/17196) per wave-176 notes.

## Reproduce

Scratch tooling in `/home/debian/GalaxyOnFire2/scratch/w12213`: `v.py`/`batch.py` (apply source
replacement to a pristine copy, rebuild `link_so -j2`, print `fn_diff`), `cumdelta2.py` (cumulative
instruction-size delta between two `.so`), `rawruns.py`, `diff2.py`, `sizediff.py`.
Pristine source copy: `Level.base.cpp`; base build `base.so`; no-ea build `noea.so`.

Build recipe: `ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2`.
Metric: `python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv`.
