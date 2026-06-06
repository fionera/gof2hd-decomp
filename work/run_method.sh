#!/usr/bin/env bash
# run_method.sh <Class> <key> <method> <vaddr> <n> — codex sub-worker for one method of a class.
# Writes work/classes/<Class>/src/<key>.cpp (which #includes the class's shared header).
Class="$1"; key="$2"; method="$3"; vaddr="$4"; n="$5"
REPO=/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
dir="work/classes/$Class"
PROMPT="HARD RULE: ONLY edit your assigned source file(s) under work/. Do NOT modify tools/, Makefile, matches.tsv, the target binary, or any other file — they are integrity-checked and changes will be reverted and your work rejected.\nDecompile ONE method of class $Class to byte-matching, GENUINE C++ (NO inline asm / __attribute__((naked))).
- Work item: $dir/$key.md  (Ghidra decompile + the EXACT target disassembly to reproduce).
- The shared class layout is in $dir/src/class.h — #include \"class.h\" and use its field offsets; do NOT redefine the struct.
- Write your C++ to: $dir/src/$key.cpp
- Verify:  bash tools/try.sh $dir/src/$key.cpp $method $vaddr $n   — iterate until it prints '>>> EXACT MATCH'.
- The differ normalizes call/branch TARGETS, so for tail calls write  return ext(args);  with extern \"C\" decls.
- If you cannot reach an exact GENUINE match after several tries, report 'INFEASIBLE: <reason>' (do NOT use asm to fake it).
Print the final MATCH line (or INFEASIBLE) and the path."
exec codex exec --dangerously-bypass-approvals-and-sandbox --skip-git-repo-check \
     -C "$REPO" -o "$dir/results/$key.out" "$PROMPT"
