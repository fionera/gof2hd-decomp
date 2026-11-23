2026-11-23 17:28:10 w14200 hunk3 process
baseline raw=54870 aligned=17438 size=57232 insn=20709/20830
next: locate first instruction divergence in 0x3e..
exp1 inline (*enemies)[i] twice in intro loop: raw=55532 aligned=18559 size=57424 (+192) -> worse, reverted
exp2 cache Status* status=Globals::status once, 98 uses: raw=55351 aligned=19638 size=57184 (-48, size-fatal) -> reverted
final: hunk3 first divergence 0x137d56 is a callee-saved regalloc permutation (orig this->sl/Globals->r4; ours this->fp/Globals->r9), global cascade from +121 insn body-size delta; not locally source-steerable within timebox
no win committed; tree restored pristine (LevelScript.cpp.bak), gate unchanged raw=54870 aligned=17438 size=57232
git status clean
