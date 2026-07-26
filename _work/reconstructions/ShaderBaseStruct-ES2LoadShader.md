# ShaderBaseStruct::ES2LoadShader

Symbol: `_ZN11AbyssEngine16ShaderBaseStruct13ES2LoadShaderEjPKc`

Status: deferred after the initial attempt, two fresh retries, and one
independent escalation review. The remaining difference is an LLVM
MachineSink/register-allocation artifact, not a known semantic mismatch.

## Verified facts

- Original entry: file address `0x8e4f0` (Ghidra image address `0x9e4f0`).
- Both original and current functions are 148 bytes.
- Passing `&source` directly to `glShaderSource` is authentic. It produces the
  original formal spills (`source` at `sp+8`) and makes the prologue exact
  through the `glCreateShader` call.
- The original sequence after `glCreateShader` is:

```text
mov r4, r0
cbz r0, failure
add r2, sp, #8
mov r0, r4
movs r1, #1
movs r3, #0
movs r5, #0
blx glShaderSource
```

- The current sequence is:

```text
cbz r0, failure
add r2, sp, #8
movs r1, #1
movs r3, #0
mov r4, r0
movs r5, #0
blx glShaderSource
```

- The subsequent CFG, branches, callees, compile-failure handling, return
  value, and stack-canary epilogue are semantically identical. Current output
  has a trailing alignment `nop`, keeping the total size equal.

## Attempts and blocker

The zero successor proves that the shader handle is zero and executes none of
the shader operations. Consequently, normal C++14 spellings using a split
declaration, assignment in the condition, aliases, references, copied locals,
an early return, an inverted guard, a `goto`, redundant self-use, or a
cast-to-void use reduce to the same optimized SSA graph. LLVM remains free to
sink the only required `r0`-to-`r4` copy into the nonzero successor.

Keeping the copy before the branch would require a genuine observable
use/escape on the zero edge, extra instructions, `volatile`, assembly, an
attribute, or undefined-behavior type punning. Those options either change the
function or violate `AGENTS.md`.

Revisit only if a legitimate reconstruction or TU-pressure change elsewhere
naturally changes register allocation. Do not add artificial source
contortions solely to pin the copy.
