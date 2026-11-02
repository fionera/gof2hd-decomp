# Generator::createMission — the real 4 bytes behind the `0.5f` size anchor (w3591)

Status: research note (2026-11-02). Every number below was produced by a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or by a controlled rebuild of this tree. No source
change was landed: none of the experiments below is a strict improvement over the current gate.

## 1. Baseline

```
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8
```

Result (HEAD `76364fa7`, no uncommitted source change):

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
RATCHET PASS
```

`readelf -S cmake-build-match/libgof2hdaa.so`:

```
.text   0x1337c8
.rodata 0x41b67
```

`report.json` row:

```
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
  osize 1996  usize 1996  pct 73.6
```

## 2. The `0.5f` site is not the size difference

The source currently has a deliberate hack at `src/game/mission/Generator.cpp:957`:

```cpp
// 0.5f + 1 ulp: same result after truncation, keeps the original code size.
costs = (int) ((float) costs * 0.5000001f);
```

Disassemble the original and the current build at the `type == 8` cost adjustment:

```
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x977c0 --stop-address=0x977e4 ../_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x977b8 --stop-address=0x977dc cmake-build-match/libgof2hdaa.so
```

Original:

```
977c8: ee10 5a10  vmov  r5, s0
977cc: d10a       bne.n 977e4
977ce: eeb6 1a00  vmov.f32 s2, #96      ; 0x3f000000 = 0.5f
977d2: eeb8 0ac0  vcvt.f32.s32 s0, s0
977d6: ee20 0a01  vmul.f32 s0, s0, s2
977da: eebd 0ac0  vcvt.s32.f32 s0, s0
977de: ee10 5a10  vmov  r5, s0
```

Current hack build:

```
977c2: d10a       bne.n 977da
977c4: eeb8 0ac0  vcvt.f32.s32 s0, s0
977c8: ed9f 1a6a  vldr  s2, [pc, #424]   ; literal pool word 0x3f000002
977cc: ee20 0a01  vmul.f32 s0, s0, s2
977d0: eebd 0ac0  vcvt.s32.f32 s0, s0
977d4: ee10 5a10  vmov  r5, s0
...
97974: 0002 3f00  .word 0x3f000002        ; the extra 4 bytes
```

The original uses the 4-byte immediate `vmov.f32 s2, #96` (true `0.5f`). The hack uses a 4-byte
`vldr` **plus** a 4-byte literal-pool word. The instruction streams at this site are otherwise the
same length, so the hack is exactly a +4-byte size anchor at the end of the function, not a
reconstruction of the original instruction.

Replacing the hack with true `0.5f`:

```
sed -i 's/0\.5000001f/0.5f/' src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
```

Result:

```
verify: byte 2615  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
  FAIL  byte_exact 4496 -> 2615
```

`report.json` row: `osize 1996  usize 1992  pct 73.8`.

So true `0.5f` is *better* for the function's fuzzy match (73.8 vs 73.6) but makes the function
4 bytes shorter. `.text` stays `0x1337c8` (the section end is padded), but every later function
shifts down by 4, which is why raw-byte `byte_exact` collapses from 4496 to 2615. The hack is
currently holding the whole downstream layout in place.

## 3. Where the real 4 bytes are: the `pick == 12` block

The 4-byte function-size delta is accounted for by the loop's `pick == 12` handling, not by the
`0.5f` site.

Original (`../_work/bins/android_2.0.16_libgof2hdaa.so`, `0x972b0..0x972d4`, 36 bytes):

```
972b0: 2e0c        cmp   r6, #12
972b2: d10c        bne.n 972ce
972b4: 9808        ldr   r0, [sp, #32]
972b6: 6800        ldr   r0, [r0]
972b8: f7ca ebaa   blx   Status::getSystem()
972bc: f7ca ebde   blx   SolarSystem::getIndex()
972c0: 220c        movs  r2, #12
972c2: 2819        cmp   r0, #25
972c4: f04f 0b01   mov.w fp, #1
972c8: bf18        it    ne
972ca: 4632        movne r2, r6
972cc: e002        b.n   972d4
972ce: f04f 0b01   mov.w fp, #1
972d2: 4632        mov   r2, r6
```

True-`0.5f` build (`cmake-build-match/libgof2hdaa.so`, `0x972b4..0x972d4`, 32 bytes):

```
972b4: f5b1 2f40   cmp.w r1, #0xc0000     ; (short)pick << 16 == 12 << 16
972b8: d109        bne.n 972ce
972ba: 9807        ldr   r0, [sp, #28]
972bc: 6800        ldr   r0, [r0]
972be: f7ca eba8   blx   Status::getSystem()
972c2: f7ca ebdc   blx   SolarSystem::getIndex()
972c6: f04f 0b01   mov.w fp, #1
972ca: 260c        movs  r6, #12           ; ternary constant-folded
972cc: e001        b.n   972d2
972ce: f04f 0b01   mov.w fp, #1
972d2: 3d01        subs  r5, #1
```

The source expression is:

```cpp
if (pick == 12) {
    selected = true;
    type = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() != 25 ? pick : 12;
}
```

The original does **not** constant-fold the ternary: it emits
`movs r2,#12; cmp r0,#25; it ne; movne r2,r6` (8 bytes). Our clang 7.0.2 does fold it, to
`movs r6,#12` (2 bytes). The reason for the different value tracking is not yet proven. That accounts for +6 bytes in the original block.
The original also compares `pick` directly (`cmp r6,#12`, 2 bytes), while our `short pick` is
sign-extended and compared as `(short)pick << 16` (`cmp.w r1,#0xc0000`, 4 bytes), which gives back
2 bytes. Net: original block = 36 bytes, ours = 32 bytes, exactly the missing 4 bytes.

This is the same 4-byte difference as the total function size (1996 vs 1992). The hack's extra
literal-pool word is therefore a coincidental size substitute for the missing non-folded ternary,
not a reconstruction of it.

## 4. Measured experiment matrix

All runs are against the original `.so`, with the full `check` target. `pct`/`size` are the
`Generator::createMission` row in `cmake-build-match/verify/report.json`.

| variant | createMission size | pct | gate byte/linked | verdict |
|---|---:|---:|---|---|
| HEAD: `0.5000001f` | 1996 | 73.6 | 4496 / 4496 | baseline, PASS |
| `0.5f` | 1992 | 73.8 | 2615 / 4496 | FAIL (`byte_exact` -1881) |
| `0.5f` + inverted `getRoutes()` branch | 1992 | 73.8 | 2615 / 4496 | FAIL; branch reorder alone is neutral |
| `0.5000001f` + inverted branch | 1996 | 73.6 | 4496 / 4496 | PASS; no improvement |
| `0.5f` + swap `amount`/`itemId` declarations | 1996 | 64.6 | 4496 / 4496 | PASS; size anchor but much worse pct |
| `0.5f` + `int pick` | 1956 | 49.1 | 2045 / 4496 | FAIL; removes the `sxth`/shifted compare but changes too much |

The `int pick` row is useful evidence for the next pass: the original has no `sxth` for `pick`
in this region and its compare is `cmp r6,#12`, which suggests the original `pick` may be `int`.
But changing only the declaration shrinks the function to 1956 bytes and drops the match to
49.1%, so it is not a standalone fix.

## 5. Conclusion

Proven:

* The original's `type == 8` cost adjustment uses a true 4-byte `vmov.f32 s2, #96` (`0.5f`),
  not a literal load.
* The current `0.5000001f` hack compiles to `vldr` + a 4-byte pool word `0x3f000002`; it adds
  exactly 4 bytes to the function and is the only reason the current build's `usize` is 1996.
* The 4-byte function-size delta is accounted for by the `pick == 12` block: original 36 bytes vs
  our 32 bytes.
  The original emits the non-folded ternary (`movs r2,#12; cmp r0,#25; it ne; movne r2,r6`), while
  our build folds it to `movs r6,#12`.
* True `0.5f` alone improves the function's fuzzy match (73.6 -> 73.8) but fails the gate because
  the function shrinks by 4 and shifts every later function.

Still hypothesis:

* The exact source spelling that prevents clang 7.0.2 from folding the ternary while keeping the
  rest of the function unchanged. `short pick` vs `int pick`, casts, `+0`, `^0`, `|0`, `*1`, and
  a volatile local were all tried; none produced a 1996-byte true-`0.5f` build with a better pct
  than the current hack.
* The original `pick` type. The absent `sxth` and the `cmp r6,#12` form point at `int`, but the
  `int pick` build is 40 bytes shorter, so other source-shape work is needed before that can be
  landed.

No source or flag change is committed with this note: the current hack is still the only variant
that passes the gate, and the true-`0.5f` variants are not strict improvements until the
`pick == 12` block is reconstructed.

## 6. Reproduction commands

```sh
# baseline
git checkout -- src/game/mission/Generator.cpp report.json
cmake --build cmake-build-match --target check -j8

# true 0.5f
sed -i 's/0\.5000001f/0.5f/' src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8

# disassembly (NDK objdump)
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x972b0 --stop-address=0x972d4 ../_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x972b4 --stop-address=0x972d4 cmake-build-match/libgof2hdaa.so
$OD -d --start-address=0x977c0 --stop-address=0x977e4 ../_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x977b8 --stop-address=0x977dc cmake-build-match/libgof2hdaa.so
```
