#!/usr/bin/env bash
# run_class.sh <Class> — launch ONE codex class-worker that owns a whole class: it defines the
# shared header, then FANS OUT its own method sub-workers (run_method.sh) in parallel, and collects.
Class="$1"
REPO=/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
dir="work/classes/$Class"; mkdir -p "$REPO/$dir/results"
PROMPT="You OWN class $Class for the GOF2 decompilation. Decompile ALL its methods to byte-exact, GENUINE C++.
Inputs: $dir/class.md (overview + method list) and $dir/methods.tsv (lines: key<TAB>method<TAB>elf_vaddr<TAB>nbytes),
and one $dir/<key>.md work-item per method (Ghidra decompile + exact target disassembly).

Do this:
1) Study the method work-items and define the shared class layout ONCE in $dir/src/quaternion.h
   (the struct with correct field offsets, consistent across every method). Method .cpp files will
   #include \"quaternion.h\".
2) PARALLELIZE across methods by fanning out sub-workers. For each line in $dir/methods.tsv run:
     bash work/run_method.sh $Class <key> <method> <vaddr> <n> </dev/null >$dir/results/<key>.log 2>&1 &
   Keep at most 5 running at once (wait when at the cap; use: while [ \$(jobs -rp | wc -l) -ge 5 ]; do sleep 2; done).
   Each sub-worker authors+verifies one method against the target using your shared header.
3) After all finish, run for each method:  bash tools/try.sh $dir/src/<key>.cpp <method> <vaddr> <n> </dev/null
   Ensure each reached '>>> EXACT MATCH' with GENUINE C++ (reject any src containing asm/naked — redo it yourself).
4) Report a table: method -> match% -> genuine? Stop when all are exact or you've exhausted reasonable effort.
Constraints: do NOT modify tools/ or build flags. Do NOT use inline asm to fake matches."
exec codex exec --dangerously-bypass-approvals-and-sandbox --skip-git-repo-check \
     -C "$REPO" -o "$dir/results/_class.out" "$PROMPT"
