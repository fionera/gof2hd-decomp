# Recompile + byte-match harness (Android target)

Goal: rebuild recovered C++ with the **exact original toolchain** and diff the produced
ARM/Thumb code against `libgof2hdaa.so` until it matches, function by function.

## Why Android is the first matching target
The `.comment` of `libgof2hdaa.so` (2.0.16) identifies the toolchain precisely:
`Android (4751641 based on r328903) clang version 7.0.2 ... (LLVM 7.0.2svn)` — i.e. **NDK r18b**,
**armeabi-v7a**, **libc++**. NDK r18b is a frozen, downloadable release, so the compiler is
exactly reproducible (unlike the iOS 2012 Apple toolchain or the MSVC Windows build).

## Environment
- Host: OrbStack Ubuntu (arm64). The NDK r18b toolchain is x86_64 — it runs under OrbStack's
  Rosetta/qemu x86_64 binfmt (`/proc/sys/fs/binfmt_misc/rosetta`), so x86_64 clang runs on arm64.
- Toolchain: `/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64` (extracted from the
  official `android-ndk-r18b-linux-x86_64.zip`).

## Scripts
- `flags.sh`      — toolchain paths + best-effort `armeabi-v7a` release flags (`GOF2_CXXFLAGS`).
- `build_fn.sh`   — `NDK=/opt/android-ndk-r18b ./build_fn.sh src.cpp [symbol]` → compiles a TU and
                    prints its `.comment` + Thumb disassembly.
- `extract_target.sh` — `NDK=... ./extract_target.sh <elf_vaddr_hex> <nbytes>` → disassembles the
                    same region from the target `.so` (Thumb triple).
- `sample_arrayadd.cpp` — PoC: reproduces `ArrayAdd<T*>` (logic validated in task 7).

## Address mapping
Ghidra rebased the ELF by **+0x10000**. So: `elf_vaddr = ghidra_addr − 0x10000`.
Example: `ArrayAdd` is Ghidra `0x000805b4` → ELF `0x000705b4`.

## Target reference — `ArrayAdd<AEPakFileEntry*>` (Ghidra 0x805b4)
```
push {r4,r5,r7,lr}; add r7,sp,#8; mov r4,r1; ldr r1,[r1]; mov r5,r0
ldr r0,[r4,#4]; adds r1,#1; str r1,[r4,#8]; lsls r1,r1,#2; blx realloc
ldr r1,[r4]; str.w r5,[r0,r1,lsl #2]; ldr r1,[r4,#8]; strd r1,r0,[r4]; pop {r4,r5,r7,pc}
```

## Matching workflow (iterative, per function)
1. Recover/author the C++ source for a function (from the Ghidra decompile + recovered types).
2. `build_fn.sh src.cpp Sym` → compiled Thumb.
3. `extract_target.sh <elf_vaddr> <n>` → target Thumb.
4. Diff. Mismatches are driven by: optimization level, `-mfpu`, `-fno-exceptions/-frtti`,
   inlining, struct layout, and ABI/stack-protector. Adjust flags/source and repeat.
5. Lock the flag set that reproduces a representative sample, then scale across TUs.

> Full byte-identical relinking of the whole `.so` also needs the original link order, version
> script, and build-id handling — a later milestone. Per-function disassembly equivalence is the
> tractable unit of progress and is what this harness measures.
