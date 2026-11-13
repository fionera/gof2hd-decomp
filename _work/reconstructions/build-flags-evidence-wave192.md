# Build-flag evidence (wave 192, 2026-11-13)

Consolidated binary evidence about how the original `libgof2hdaa.so` was built.
Everything below is measured against `_work/bins/android_2.0.16_libgof2hdaa.so`
with the current `cmake-build-match/libgof2hdaa.so`.

## Proven identical (so these flags are already right)

| evidence | result |
|---|---|
| `.comment` | **byte-identical**: `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123 (prerelease)` |
| `.ARM.attributes` | **byte-identical**, all 18 tags |
| `.rodata` | byte-identical (269,159 B) |
| `.data` | byte-identical (10,604 B) |
| `.rel.plt` | byte-identical (29,392 B) |
| `.ARM.exidx` | byte-identical (28,528 B) |
| `.ARM.extab` | size/base exact (156,312 B @ `0x1a3b78`); content differs only in the 9 residual LSDA records |
| `.dynsym`/`.dynstr`/`.hash`/`.rel.dyn` | size- and base-exact; symbol parity 5370/5370, imports 207 |

`.ARM.attributes` (identical on both sides) pins the ABI flags:

```
Tag_CPU_name "ARM v7"   Tag_CPU_arch v7   Tag_CPU_arch_profile Application
Tag_ARM_ISA_use Yes     Tag_THUMB_ISA_use Thumb-2
Tag_FP_arch VFPv3       Tag_Advanced_SIMD_arch NEONv1
Tag_ABI_PCS_GOT_use GOT-indirect          Tag_ABI_PCS_wchar_t 4
Tag_ABI_FP_denormal Needed                Tag_ABI_FP_exceptions Needed
Tag_ABI_FP_number_model IEEE 754          Tag_ABI_align_needed 8-byte
Tag_ABI_enum_size int                     Tag_ABI_HardFP_use Deprecated
Tag_CPU_unaligned_access v6               Tag_ABI_FP_16bit_format IEEE 754
```

Consequences:
- `Tag_CPU_unaligned_access: v6` means **unaligned access is enabled** — `-mno-unaligned-access`
  is ruled out (it would flip this tag to "None").
- `Tag_ABI_HardFP_use: Deprecated` + `Tag_FP_arch VFPv3` = **softfp** (`-mfloat-abi=softfp`),
  as configured.
- `Tag_ABI_enum_size: int`, `wchar_t 4`, `8-byte` alignment all match the canonical flags.

## What is left

Because every data section, the attribute section, the comment, the exidx and the
relocation tables are already byte-identical, the residual is **pure codegen** in the
15 residual functions (`.text` 76,877 B, `.ARM.extab` 597 B). A flag that changed
data layout or the ABI would have shown up above; only flags that change *instruction
selection / scheduling / register allocation* can still be wrong, and they must be
applied per TU (a global change would break the 4508 byte-exact functions).

Per-TU sweep harness: `tools/per_tu_flags.cmake` + `tools/per_tu_opt_sweep.py`
(the `-O1/-O2/-Os/-O3` sweep is already done). Candidate non-O flags for w13913:
`-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`,
`-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fvisibility=hidden`,
`-fomit-frame-pointer`/`-fno-omit-frame-pointer`, `-fno-math-errno`,
`-fno-exceptions`, `-fno-rtti`, `-fno-inline-functions`, `-fno-vectorize`,
`-fno-slp-vectorize`, `-fno-jump-tables`, `-fno-common`, `-fno-builtin`.
