#!/usr/bin/env bash
# run_codex_class.sh <Class> — ONE codex worker authoring a whole NEW class directly (no nested
# fan-out, to conserve codex usage). Builds the shared header, authors each method in methods.tsv,
# verifies each at -Oz via try.sh. Output -> work/classes/<Class>/results/_class.out
Class="$1"
REPO=/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
dir="work/classes/$Class"; mkdir -p "$REPO/$dir/results" "$REPO/$dir/src"
PROMPT="HARD RULES (an integrity gate enforces these; violations are auto-reverted and your work rejected):
- ONLY create/edit files under $dir/src/. Do NOT modify tools/, Makefile, matches.tsv, include/, the target binary, or any other class.
- GENUINE C++ ONLY. NEVER use inline asm / __asm__ / asm volatile / __attribute__((naked)) to force a match — these are rejected.

You OWN class $Class for the Galaxy on Fire 2 matching decompilation. The target Android .so was compiled with clang **-Oz -fstack-protector** (size-opt + stack canaries); tools/try.sh already uses both. Author natural, idiomatic C++ that the compiler reproduces byte-for-byte. For functions with on-stack buffers the COMPILER emits the canary automatically — write natural local variables, NEVER hand-write __stack_chk_guard cookies.

Inputs (in $dir): methods.tsv (lines: key<TAB>method<TAB>elf_vaddr_hex<TAB>nbytes) and one <key>.md work-item per method (Ghidra decompile + the EXACT target disassembly = your source of truth).

Do this:
1) DETERMINE THE NAMESPACE from the work-items' 'Sig:'/decompile lines: if they show AbyssEngine::$Class::… use 'namespace AbyssEngine { struct $Class; }' and define methods in that namespace; if they show top-level $Class::… use 'struct $Class;' (no namespace). The compiled symbol must demangle to the same qualified name the target uses (gofdiff matches the method name as a substring). Create $dir/src/class.h ONCE with that forward-declared struct + small helper types. To access fields, DO NOT guess a full layout — use byte-offset casts read straight from each work-item, e.g.  *(int*)((char*)self + 0xNN)  /  *(float*)((char*)self + 0xNN). This avoids full-struct RE and matches the target's loads/stores exactly.
2) Author each method (process SMALLEST nbytes first — accessors build momentum). For each line in methods.tsv write $dir/src/<key>.cpp: #include \"class.h\", implement the method as natural C++ matching the work-item's decompile + disasm.
   - The receiver is the first arg (this) in r0; write methods as free functions 'extern \"C\"'-style or as the right signature so this stays in r0.
   - The differ normalizes call/branch TARGETS, so for tail calls just write  return ext(args);  with 'extern \"C\"' declarations of the callee.
3) Verify EACH:  bash tools/try.sh $dir/src/<key>.cpp '<method>' <vaddr> <n> </dev/null   — iterate the SOURCE until it prints '>>> EXACT MATCH'.
   -Oz match tips: comparison operand order ('i<n' vs 'n>i' flips cmp); hoist/sink an assignment for scheduling; reuse vs reload a value for register allocation; field access order; simplest expression of the logic.
4) If a method won't reach exact after honest effort, keep your best version and move on (note it).

Report a table: method -> final match% -> EXACT(yes/no), and the count that reached exact."
exec codex exec --dangerously-bypass-approvals-and-sandbox --skip-git-repo-check \
     -C "$REPO" -o "$dir/results/_class.out" "$PROMPT"
