# `-fpic` vs `-fPIC` on armeabi-v7a: GOT codegen A/B

Status: research note (2026-11-01). Worktree `wt/w3422`, branch `wave87-w3422`,
HEAD `d00e0030`. All numbers were measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b clang 7.0.2.

## TL;DR

For this tree and this target, **`-fpic` and `-fPIC` are codegen-identical**. The two
whole-`.so` links differ in exactly the 20-byte GNU build-id; `.text`, `.got`, `.plt`,
`.rel.dyn`, `.rel.plt`, `.rodata`, `.data` and `.data.rel.ro` are byte-identical, and every
gate count is unchanged.

The original binary therefore **cannot distinguish** the two flags from GOT codegen,
relocations, or ELF attributes. Both flags produce GOT-indirect code and neither produces a
dynamic `R_ARM_REL32` or `DT_TEXTREL`. The rationale in `cmake/toolchain.cmake` that
"`-fpic` emits `R_ARM_REL32` the original lacks" is not supported by this compiler/target:
both flags emit the same object-level `R_ARM_REL32` for local statics, and the linker resolves
it in both cases.

No strict improvement was found, so this commit contains only this note. The existing
`-fPIC` choice is harmless and conventional, but it is not uniquely proven by the binary.

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
python3 tools/goal_lint.py src
```

Measured gate (`check`):

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Additional gate fields from `cmake-build-match/verify/report.json`:

```
size_match 4523 of 4524
size_mismatch: _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
               original 1996, ours 1992
```

`goal_lint.py` printed `CLEAN ✓ (no new soft violations; hard invariants 0)`. `drift` printed
`=== DRIFT: 0 field(s) across 0 class(es) ===`.

## 2. Controlled `-fpic` A/B

The repo toolchain sets `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS` with `CACHE ... FORCE`, so the A/B
used a scratch toolchain wrapper that includes the real toolchain and replaces only the app
`-fPIC` token. Vendored archives were held fixed.

```sh
cat > /tmp/w3422-toolchain-fpic.cmake <<'WRAPPER_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3422/cmake/toolchain.cmake")
string(REPLACE "-fPIC" "-fpic" _gof2_cxx_flags "${CMAKE_CXX_FLAGS}")
string(REPLACE "-fPIC" "-fpic" _gof2_c_flags "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${_gof2_cxx_flags}" CACHE STRING "matching ARM compile flags (-fpic A/B)" FORCE)
set(CMAKE_C_FLAGS "${_gof2_c_flags}" CACHE STRING "matching ARM compile flags (-fpic A/B)" FORCE)
WRAPPER_EOF

cmake -S . -B /tmp/w3422-fpic \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3422-toolchain-fpic.cmake
cmake --build /tmp/w3422-fpic --target check -j8
cmake --build /tmp/w3422-fpic --target drift -j8
```

The scratch cache confirms the only app-flag change:

```
CMAKE_CXX_FLAGS: ... -std=c++11 -fpic -frtti -fstack-protector-strong -Oz -g ...
CMAKE_C_FLAGS:   ... -fpic -fstack-protector-strong -Oz -g ...
```

Measured table:

| variant | app PIC flag | byte_exact | linked_exact | size_match | lint | drift | data | imports |
|---|---|---:|---:|---:|---|---:|---|---:|
| baseline | `-fPIC` | 4182 | 4495 | 4523/4524 | CLEAN | 0 | 843/843 | 207 |
| A/B | `-fpic` | 4182 | 4495 | 4523/4524 | CLEAN | 0 | 843/843 | 207 |

Both `check` runs ended with `RATCHET PASS`; `drift` remained 0.

## 3. Whole-binary and section evidence

The two linked `.so` files differ in exactly 20 bytes, all inside the GNU build-id note:

```sh
sha256sum cmake-build-match/libgof2hdaa.so /tmp/w3422-fpic/libgof2hdaa.so
cmp -l cmake-build-match/libgof2hdaa.so /tmp/w3422-fpic/libgof2hdaa.so | wc -l
cmp -l cmake-build-match/libgof2hdaa.so /tmp/w3422-fpic/libgof2hdaa.so
```

Result:

```
differing bytes=20 first=477(0x1dc) last=496(0x1ef)
```

`0x1dc..0x1ef` is exactly the 20-byte build-id payload (`.note.gnu.build-id` starts at file
offset `0x1cc`; its 12-byte note header plus 4-byte `GNU\0` name put the descriptor at `0x1dc`).
`readelf -nW` confirms the two build IDs differ and nothing else in the note does.

Section dumps are identical:

```sh
OBJCOPY=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
for sec in .text .got .plt .rel.dyn .rel.plt .rodata .data .data.rel.ro; do
  $OBJCOPY --dump-section "$sec=/tmp/fPIC${sec//\//_}.bin" cmake-build-match/libgof2hdaa.so
  $OBJCOPY --dump-section "$sec=/tmp/fpic${sec//\//_}.bin" /tmp/w3422-fpic/libgof2hdaa.so
  cmp -s /tmp/fPIC${sec//\//_}.bin /tmp/fpic${sec//\//_}.bin && echo "$sec SAME"
done
```

All eight sections printed `SAME`. Representative hashes:

| section | SHA-256 (both variants) |
|---|---|
| `.text` | `db2f97442b0092103f03b725f673de061d11ab29986840dfe0bd6ba78aeca567` |
| `.got` | `87e6b167273f1d99b6f7c40b1619909275a0ab8ed249f9349da6928da2f57576` |
| `.rel.dyn` | `8200dba9518c55f85ce70db52cb404b072bf629b04964844c6a678d2f0aa2da5` |
| `.rel.plt` | `fae682d52bb58a12e2ba6fe73a4c17d8d7a3b50818697c9486925b25ca4d1f95` |

## 4. Relocation and ELF-attribute evidence

Command:

```sh
for f in ../_work/bins/android_2.0.16_libgof2hdaa.so \
         cmake-build-match/libgof2hdaa.so \
         /tmp/w3422-fpic/libgof2hdaa.so; do
  echo "$f"
  readelf -rW "$f" | awk '/^Relocation section/{next} /^[0-9a-f]/{print $3}' | sort | uniq -c
  readelf -dW "$f" | grep -Ei 'TEXTREL|FLAGS'
  readelf -AW "$f" | grep 'Tag_ABI_PCS_GOT_use'
done
```

Result:

| binary | `R_ARM_ABS32` | `R_ARM_GLOB_DAT` | `R_ARM_JUMP_SLOT` | `R_ARM_RELATIVE` | dynamic `R_ARM_REL32` | `DT_TEXTREL` | GOT attribute |
|---|---:|---:|---:|---:|---:|---|---|
| original | 1817 | 614 | 3674 | 1162 | 0 | absent | `GOT-indirect` |
| baseline `-fPIC` | 1818 | 614 | 3674 | 1161 | 0 | absent | `GOT-indirect` |
| A/B `-fpic` | 1818 | 614 | 3674 | 1161 | 0 | absent | `GOT-indirect` |

The one-relocation delta from the original (`ABS32 +1`, `RELATIVE -1`) is **flag-invariant**:
both PIC variants have exactly the same counts. It is therefore a source/data-layout residue,
not evidence about `-fpic` vs `-fPIC`.

A concrete GOT-indirect access in the original is `GetEngine()` at `0x93c88`:

```
93c88: ldr  r0, [pc, #8]   ; PC-relative literal
93c8a: add  r0, pc         ; r0 = GOT slot address
93c8c: ldr  r0, [r0, #0]   ; load gEngine
93c8e: ldr  r0, [r0, #0]
```

Our baseline has the same instruction bytes at the same address, and the `-fpic` build has
the same `.text` section bytes.

## 5. Object-level probe: the flags are actually applied but produce the same code

A small probe with external globals, file-static globals, string literals and arrays was
compiled with the exact target flags under `-fpic`, `-fPIC` and `-fno-pic`:

```sh
CLANG=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
for flag in -fpic -fPIC -fno-pic; do
  $CLANG -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb \
    -mfpu=neon -mfloat-abi=softfp -std=c++11 "$flag" -Oz -g \
    -c /tmp/pic_probe.cpp -o /tmp/pic_probe${flag}.o
done
sha256sum /tmp/pic_probe-fpic.o /tmp/pic_probe-fPIC.o /tmp/pic_probe-fno-pic.o
```

Result:

```
fpic  = adbaf5630e24b356f008fcae99de35ba0bf87af4ef3524dd672123345acb1736
fPIC  = adbaf5630e24b356f008fcae99de35ba0bf87af4ef3524dd672123345acb1736
fno-pic = ab2bd15a6433b11eb7224ae379db57c946c4eb385146e9488a70684ba562e961
```

`-fpic` and `-fPIC` are byte-identical; `-fno-pic` is different, so the flag is being
applied. A second probe with 5,000 external globals and a 200-case switch also produced
identical `-fpic`/`-fPIC` objects (`sha256 e0a1e84a...` for both; 5,200 `R_ARM_GOT_PREL`
relocations).

The driver does pass different PIC levels:

```
-fpic -> "-pic-level" "1"
-fPIC -> "-pic-level" "2"
```

but the ARM codegen in clang 7.0.2 is the same for both levels on this target.

Both flags also emit object-level `R_ARM_REL32` for local statics; for example the probe has
`R_ARM_REL32 .L_MergedGlobals` under both `-fpic` and `-fPIC`. The final link resolves those
to `R_ARM_RELATIVE`, which is why the original's dynamic relocation table has no
`R_ARM_REL32` under either flag. The original's lack of a dynamic `R_ARM_REL32` is therefore
not evidence for `-fPIC`.

## 6. Conclusion

Proven by controlled A/B:

* `-fpic` and `-fPIC` give identical gate counts on this tree
  (`byte 4182`, `linked 4495`, `size 4523/4524`, lint CLEAN, drift 0, data 843/843,
  imports 207).
* The linked `.so` files differ only in the 20-byte build-id.
* `.text`, `.got`, `.plt`, `.rel.dyn`, `.rel.plt`, `.rodata`, `.data` and `.data.rel.ro`
  are byte-identical between the two variants.
* Both flags produce GOT-indirect code, no dynamic `R_ARM_REL32`, and no `DT_TEXTREL`.
* The original's relocation counts and GOT attribute are consistent with both flags.

Not proven / still hypothesis:

* Which of `-fpic` or `-fPIC` the original build actually used. The binary cannot distinguish
  them on this target.
* That LLVM 7.0.2's ARM backend ignores PIC level 1 vs 2 for every possible construct. The
  whole tree and the two probes support it, but this note only establishes it for this tree
  and the tested constructs.

Recommendation: keep `-fPIC` (harmless, conventional), but do not cite `R_ARM_REL32` as the
reason. If the toolchain comment is cleaned up later, it should say that `-fpic` and `-fPIC`
are indistinguishable here and that `-fPIC` is the conventional choice, not a binary-proven
one.
