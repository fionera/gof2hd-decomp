# Drift fix rules — make every field_0xNN land at its named offset 0xNN

A `field_0xNN` (or byte_0xNN/flag_0xNN/image_0xNN/pad_0xNN) name encodes the field's TRUE offset in
the original binary (Ghidra-derived). If our struct puts it elsewhere, our layout differs from the
original and accesses hit the wrong byte. Fix the layout so each named field sits at its name.

## Method
1. Get TRUE offsets under the REAL target (ARM AAPCS) with the PROJECT toolchain.
   **NEVER use the host compiler.** Host `clang++ -m32` targets i386, whose ABI aligns
   `long long` / `double` / `uint64_t` to 4 bytes where ARM AAPCS aligns them to 8 — any class
   containing an 8-byte scalar gets wrong offsets (false drift on correct layouts, false clean
   on broken ones; e.g. `{char; long long; double; unsigned}` puts the last field at 0x14 on
   i386 but 0x18 on ARM).
   Easiest: `python3 tools/layout.py ClassName` dumps the resolved layout with the right
   compiler and marks drift inline. For a manual probe, use the template trick
   (`template<int N> struct S; S<(int)__builtin_offsetof(Class, field_0xNN)> a;` — the `S<NNN>`
   in the error is the actual decimal offset) with the NDK clang++:
   ```
   NDK=.cache/ndk/android-ndk-r18b
   $NDK/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ \
     -target armv7-none-linux-androideabi21 --sysroot=$NDK/sysroot \
     -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
     -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
     -isystem $NDK/sources/android/support/include \
     -isystem $NDK/sysroot/usr/include -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
     -D__ANDROID_API__=21 -Isrc -Ithird_party/fmod/inc -Ithird_party/gl -Ithird_party/jni \
     -Ithird_party/libzip -Ithird_party/crypto -include cstdint -fsyntax-only x.cpp 2>&1
   ```
   (Same flags the real build uses — steal them from `cmake-build-match/compile_commands.json`
   if in doubt. `tools/layout.py` / `tools/drift_scan.py` resolve them automatically and refuse
   to run without the NDK toolchain.)
2. Walk the struct top-down. For each named field, compare actual vs named:
   - actual < named  -> insert `uint8_t _pad_0xXX[named-actual];` immediately BEFORE it (missing bytes
     upstream). Often one pad fixes a whole run of trailing fields (they're all short by the same N).
   - actual > named  -> a PRIOR field is over-sized. Infer the correct size from the gap to the NEXT
     named field (consecutive named offsets give field sizes), and shrink that field's type
     (e.g. int->uint8_t) OR split it. Do NOT just delete bytes blindly.
   - For matrix-embedding structs: our AEMath::Matrix is 0x3c but the original often reserves 0x40 for
     a matrix region; add 4 bytes padding AFTER the matrix member (local fix; never change Matrix.h).
3. PRESERVE total size where a trailing/region size is known; do not shift fields that are already
   correct. Verify with the template trick that EVERY named field now matches AND that fields you
   didn't intend to move didn't move.
4. Keep code compiling: if you rename/resize a field that is USED, update use sites (verify receiver
   types; field_0xNN names are reused across classes). Prefer inserting padding (no use-site changes)
   over retyping where possible.
5. Add a guarded static_assert block after the class:
   `#if __SIZEOF_POINTER__ == 4` ... `static_assert(__builtin_offsetof(Class, field_0xNN)==0xNN,"");`
   for every former-drift field plus the next field, then `#endif`.

## Verify (self-check before reporting)
- Re-run the probe (NDK clang++, per Method step 1): all named fields match their names.
- `python3 tools/layout.py | grep YourClass` -> no rows (authoritative, record-layout dump).
- `python3 tools/drift_scan.py 2>/dev/null | grep YourClass` -> no rows.
- Do NOT run the full build (orchestrator does that + parity + ASM no-degradation centrally).
Report per field: actual->named and how you fixed it (pad N bytes / retyped X).
