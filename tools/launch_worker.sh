#!/bin/bash
# usage: tools/launch_worker.sh <wid> <target> <mangled> <file> <pct> <osize> <usize> <hint>
set -u
MAIN=/home/debian/GalaxyOnFire2/gof2-decomp
w=$1; target=$2; mangled=$3; file=$4; pct=$5; osize=$6; usize=$7; hint=$8
d=/home/debian/GalaxyOnFire2/wt/$w
if [ -d "$d" ]; then echo "exists $d"; exit 1; fi
git -C "$MAIN" worktree add -q "$d" -b wave193-$w master || exit 1
ln -s "$MAIN/.cache" "$d/.cache"
cat > /tmp/$w.prompt <<PROMPT
You are worker $w in a parallel byte-matching decompilation fleet for Galaxy on Fire 2 HD (Android armeabi-v7a, original libgof2hdaa.so 2.0.16, NDK r18b clang 7.0.2 -Oz).

Target: $target ($mangled)
Source file: $file
Current report: $pct%, $usize/$osize B. $hint

Repo: $d (branch wave193-$w, based on current master). Read AGENTS.md first.
Confinement: work ONLY inside $d. Do not modify /home/debian/GalaxyOnFire2/gof2-decomp or any
other worktree. Put scratch files under /tmp. Leave the worktree clean when you finish; the
coordinator removes it after harvesting. Ground truth is the original binary ../_work/bins/android_2.0.16_libgof2hdaa.so loaded in Ghidra as android_2.0.16_libgof2hdaa.so (image base 0x10000; real address = Ghidra address - 0x10000). Use get_struct_layout, decompile_function, and manual Thumb disassembly. The source-layout dumps in ../source file list dump/ and _work/original_layout/ are authoritative for TU membership. DeepOpen is only a logic oracle; v2 constants differ.

Baseline gate (do not regress): byte_exact 4508/4524, linked_exact 4508/4524, size_exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0. The 16 residual functions are
all size-exact. Current section residual: .text 78,772 differing bytes; .ARM.extab 122,327 and
ours is 4 B short (orig 0x26298, ours 0x26294); .ARM.exidx 1,307; .rel.plt 4; build-id 20.
All sections are size/base-exact; .rodata, .data, .data.rel.ro, .dynsym, .dynstr, .hash and
.rel.dyn are byte-identical. Do not assume only .text differs.

Context: this is the "worst-match" wave. These functions are the lowest fuzzy percentages in the tree, so a root-cause fix here (wrong control-flow shape, wrong field offsets/struct layout, wrong call order, wrong constant, wrong prologue register allocation) often also fixes neighbouring functions that share the same struct or helper. Prefer fixing the real source shape over local hill-climbing, but keep every change measured. The fuzzy metric is dominated by register allocation and literal-pool placement, so a faithful source shape can still score lower than a tuned one; report both raw and aligned scores.

Goal: improve this function's match (prefer size-exact + linked/byte exact, otherwise fuzzy
percentage) with real human-style C++ only. No inline asm, no __attribute__, no __declspec, no
extern shortcuts, no manual jump tables, no manually linked data, no decompiler shims. One class
per header; void*/union only with documented same-line waivers. Do not invent linker tricks. If
the function has an artificial anchor, only remove it after reconstructing the real body.
Keep the target function's size exactly $osize B before committing unless an explicit,
documented gate-improving reason exists.

Work method:
1. If cmake-build-match is missing run: cmake --preset default
2. Inspect the current source and the original disassembly side by side. Use:
   FN=$mangled cmake --build cmake-build-match --target verify-fn
   python3 tools/verify/verify.py --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours cmake-build-match/libgof2hdaa.so --show $mangled
   python3 tools/hunks.py $mangled --ours cmake-build-match/libgof2hdaa.so
   Dump the original with the NDK arm-linux-androideabi-objdump and read the literal pool.
3. Identify the real source shape (control flow, field offsets, constants, call order, temporaries) from Ghidra/Thumb, not from guesswork.
4. Make a focused change, rebuild and measure. Keep only a real improvement.
5. Before committing run: cmake --build cmake-build-match --target check -j4
   Required: RATCHET PASS, byte_exact >= 4508, linked_exact >= 4508, size 4524/4524,
   data 843/843, imports 207, lint CLEAN, drift 0. Do not commit report.json.
   Do not run tools/ratchet.py --update-baseline unless you have a real documented gain; if you
   do, update the inline counts in docs/VALIDATION.md and docs/IDENTICALITY.md in the same commit.
6. Commit with message: "<Area> pass 193: <what> (<before> -> <after> raw/aligned, <size>B; gate byte 4508 linked 4508)".
   Leave the worktree clean.
7. If no real improvement is found, leave the worktree clean and report the best measured variant and why it did not win, including hunks and raw/aligned scores.

Do not push. Do not harvest other workers. Focus only on the target function unless a shared struct/layout fix is the clear root cause. Report exact before/after counts.
PROMPT
setsid bash -c "codex exec -p dsv41 --dangerously-bypass-approvals-and-sandbox -C $d -o /tmp/$w.last - < /tmp/$w.prompt > /tmp/$w.log 2>&1" >/dev/null 2>&1 < /dev/null &
echo "launched $w $target"
