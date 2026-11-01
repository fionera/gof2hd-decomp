#!/bin/bash
# usage: launch_research.sh <wid> <title> <prompt-file>
MAIN=/home/debian/GalaxyOnFire2/gof2-decomp
w=$1; title=$2; pf=$3
d=/home/debian/GalaxyOnFire2/wt/$w
if [ -d "$d" ]; then echo "exists $d"; exit 1; fi
git -C $MAIN worktree add -q "$d" -b wave87-$w master || exit 1
ln -s $MAIN/.cache "$d/.cache"
cat > /tmp/$w.prompt <<PROMPT
You are research worker $w in a parallel byte-matching decompilation fleet for Galaxy on Fire 2 HD
(Android armeabi-v7a, original libgof2hdaa.so 2.0.16, NDK r18b clang 7.0.2 -Oz).

Topic: $title

Repo: $d (branch wave87-$w, based on current master). Read AGENTS.md first, then the existing
research notes _work/reconstructions/build_flags_research.md, link_flags_research.md,
plt-dynsym-order.md, literal_pool_audit.md and docs/IDENTICALITY.md.

Confinement: work ONLY inside $d. Do not modify /home/debian/GalaxyOnFire2/gof2-decomp or any
other worktree. Scratch files go under /tmp. Leave the worktree clean when you finish; the
coordinator removes it after harvesting. Do not push. Do not harvest other workers.

Ground truth: ../_work/bins/android_2.0.16_libgof2hdaa.so. Ghidra project
android_2.0.16_libgof2hdaa.so, image base 0x10000 (real = ghidra - 0x10000). Source-layout dumps
in ../source file list dump/ and _work/original_layout/ are authoritative for TU membership.

Goal: produce evidence, not guesses. Every claim must be backed by a command run against the
original binary or by a controlled A/B rebuild of this tree with the gate counts recorded.
No inline asm, no __attribute__, no extern shortcuts, no manual jump tables, no manually linked
data. Do not invent linker tricks. Do not add padding anchors: every ELF section is already
size- and base-exact.

Required work method:
1. If cmake-build-match is missing: cmake --preset default
2. Establish the current baseline with: cmake --build cmake-build-match --target check -j8
   (record byte_exact / linked_exact / size / lint / drift; baseline is byte 4115, linked 4495).
3. Run controlled experiments. For compile-flag A/B use a scratch copy of the build dir or edit
   cmake/toolchain.cmake, rebuild, and record the full gate numbers. Never --update-baseline.
4. Write a self-contained note in _work/reconstructions/ (new file or a clearly-marked section)
   with the exact commands, the measured table, and the conclusion. Commit it.
5. If (and only if) an experiment is a strict improvement, commit the flag/source change too,
   with honest before/after gate counts. Otherwise commit only the research note.
6. Leave the worktree clean.

Report: exact commands, measured before/after numbers, what is proven vs still hypothesis.
PROMPT
setsid bash -c "codex exec -p dsv41 --dangerously-bypass-approvals-and-sandbox -C $d -o /tmp/$w.last - < /tmp/$w.prompt > /tmp/$w.log 2>&1" >/dev/null 2>&1 < /dev/null &
echo "launched $w $title"
