#!/usr/bin/env bash
# run_worker.sh <name> <elf_vaddr_hex> <nbytes> — launch a codex worker to byte-match one function.
name="$1"; vaddr="$2"; n="$3"
REPO=/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
PROMPT="Decompile ONE ARM/Thumb function to byte-matching C++.
- Read the work item: work/$name.md  (Ghidra decompile + the EXACT target disassembly to reproduce).
- Write your C++ to: work/src/$name.cpp
- Verify by running:  bash tools/try.sh work/src/$name.cpp $name $vaddr $n
  It prints 'MATCH <pct>%'; iterate your source until it prints '>>> EXACT MATCH'.
Tips:
- The differ normalizes call/branch TARGETS (bl/blx -> 'call'; b/b.w -> 'b @'), so the exact callee
  identity does NOT matter. For a tail call write  return ext(args);  and declare unknown callees
  as  extern \"C\"  with plausible signatures.
- Reproduce field offsets from the disassembly (e.g. ldr r0,[r0,#0x34] => a struct field at 0x34).
- Build flags are FIXED (NDK r18 clang, armv7 thumb, -O2). Do not change tools/ or the flags.
- If after several attempts you cannot exceed ~80% (e.g. the target disasm is garbage/mis-decoded),
  stop and report 'INFEASIBLE: <reason>'.
Finish by printing the final 'MATCH ...' line (or INFEASIBLE) and the path work/src/$name.cpp."
exec codex exec --dangerously-bypass-approvals-and-sandbox --skip-git-repo-check \
     -C "$REPO" -o "work/results/$name.out" "$PROMPT"
