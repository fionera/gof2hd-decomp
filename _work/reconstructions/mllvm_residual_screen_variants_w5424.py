#!/usr/bin/env python3
"""Generate the -mllvm candidate payloads screened in
_work/reconstructions/mllvm_residual_screen-wave106-w5424.md.

Usage:
  python3 mllvm_residual_screen_variants_w5424.py <outdir>

Writes targeted.json (406), extra.json (157), sched.json (13), combos.json (10)
and allbool.json (643).  The option list is parsed from the NDK r18b clang
7.0.2 `-mllvm -help-hidden` output; this script only needs the static option
names below, so it is self-contained.
"""
import json, re, sys
from pathlib import Path

OUT = Path(sys.argv[1] if len(sys.argv) > 1 else "/dev/shm/w5424")
OUT.mkdir(parents=True, exist_ok=True)

def tag(flags):
    return re.sub(r'[^A-Za-z0-9_.=-]+', '_', flags).strip('_')

def mk(pairs):
    seen, out = set(), []
    for flags in pairs:
        if flags in seen:
            continue
        seen.add(flags)
        out.append(dict(tag=tag(flags), flags=flags))
    return out

def bools(names):
    for b in names:
        yield f"-mllvm -{b}"
        yield f"-mllvm -{b}=false"

def vals(opt, values):
    for v in values:
        yield f"-mllvm -{opt}={v}"

# ---------------------------------------------------------------- targeted (406)
T = []
def add(flags): T.append(flags)
for v in ["greedy","basic","fast","pbqp","default"]:
    add(f"-mllvm -regalloc={v}")
for v in ["default","size","speed"]:
    add(f"-mllvm -split-spill-mode={v}")
for b in ["enable-deferred-spilling","disable-spill-fusing","disable-spill-hoist",
          "optimize-regalloc","condsider-local-interval-cost","remat-pic-stub-load",
          "early-live-intervals","enable-subreg-liveness","join-liveintervals",
          "disable-adv-copy-opt","disable-non-allocatable-phys-copy-opt",
          "twoaddr-reschedule","disable-2addr-hack","enable-arm-3-addr-conv",
          "align-neon-spills","reverse-csr-restore-seq"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
for v in [0,1,2,4,8,16,32]:
    add(f"-mllvm -regalloc-csr-first-time-cost={v}")
for v in [0,1,2,4,8,16,32,64]:
    add(f"-mllvm -spp-rematerialization-threshold={v}")
for v in [4,8,16]:
    add(f"-mllvm -stress-regalloc={v}")
add("-mllvm -disable-block-placement"); add("-mllvm -disable-block-placement=false")
add("-mllvm -enable-block-placement-stats")
for v in [0,1,2,5,10,20,50,100,200]:
    add(f"-mllvm -block-placement-exit-block-bias={v}")
for v in [0,1,2,4,8,16,32,64]:
    add(f"-mllvm -misfetch-cost={v}")
for b in ["disable-tail-duplicate","disable-early-taildup","tail-dup-placement",
          "tail-dup-placement-aggressive-threshold","tail-dup-verify"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
for opt in ["tail-dup-size","tail-dup-limit","tail-dup-indirect-size",
            "tail-dup-placement-threshold","tail-dup-placement-aggressive-threshold",
            "tail-dup-placement-penalty"]:
    for v in [0,1,2,4,8,16,32,64,128]:
        add(f"-mllvm -{opt}={v}")
for b in ["disable-early-ifcvt","disable-ifcvt-diamond","disable-ifcvt-forked-diamond",
          "disable-ifcvt-simple","disable-ifcvt-simple-false","disable-ifcvt-triangle",
          "disable-ifcvt-triangle-false","disable-ifcvt-triangle-false-rev",
          "disable-ifcvt-triangle-rev","ifcvt-branch-fold","old-thumb2-ifcvt",
          "stress-early-ifcvt","enable-if-conversion"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
for opt in ["early-ifcvt-limit","ifcvt-limit","ifcvt-fn-start","ifcvt-fn-stop"]:
    for v in [0,1,2,4,8,16,32,64,100,1000]:
        add(f"-mllvm -{opt}={v}")
for b in ["enable-shrink-wrap","disable-libcalls-shrinkwrap"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
for v in [2,3,4,8,16,32]:
    add(f"-mllvm -arm-constant-island-max-iteration={v}")
for b in ["arm-promote-constant","arm-adjust-jump-tables","arm-synthesize-thumb-1-tbb",
          "arm-load-store-opt","arm-assume-misaligned-load-store","arm-global-merge",
          "arm-force-fast-isel","arm-use-mulops","arm-atomic-cfg-tidy",
          "arm-add-build-attributes","arm-interworking"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
for opt in ["arm-promote-constant-max-size","arm-promote-constant-max-total"]:
    for v in [0,1,2,4,8,16,32,64,128,256]:
        add(f"-mllvm -{opt}={v}")
for v in ["always","never","arm","thumb"]:
    add(f"-mllvm -arm-implicit-it={v}")
for v in [0,1,2,4,8,16,32]:
    add(f"-mllvm -min-jump-table-entries={v}")
for v in [0,1,2,4,8,16,32,64,100]:
    add(f"-mllvm -jump-table-density={v}")
for opt in ["t2-reduce-limit","t2-reduce-limit2","t2-reduce-limit3"]:
    for v in [0,1,2,4,8,16,32,64,100]:
        add(f"-mllvm -{opt}={v}")
for b in ["disable-cleanups","disable-demotion","disable-nounwind-inference"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
for v in ["default","converge","ilpmax","ilpmin"]:
    add(f"-mllvm -misched={v}")
for v in ["default","list-burr","source","list-hybrid","list-ilp"]:
    add(f"-mllvm -pre-RA-sched={v}")
for b in ["misched-bottomup","misched-topdown","misched-regpressure","misched-cluster",
          "misched-cyclicpath","misched-fusion","enable-misched","misched-postra",
          "enable-post-misched","post-RA-scheduler","disable-post-ra",
          "disable-postra-machine-sink","disable-postra-machine-licm",
          "disable-dfa-sched","enable-aa-sched-mi","use-tbaa-in-sched-mi",
          "sink-insts-to-avoid-spills"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
for v in [0,1,2,4,8,16,32,64]:
    add(f"-mllvm -misched-limit={v}")
    add(f"-mllvm -max-sched-reorder={v}")
    add(f"-mllvm -sched-avg-ipc={v}")
    add(f"-mllvm -dfa-sched-reg-pressure-threshold={v}")
for b in ["disable-sched-critical-path","disable-sched-live-uses","disable-sched-stalls",
          "disable-sched-reg-pressure","disable-sched-height","disable-sched-vrcycle",
          "disable-sched-cycles","disable-sched-hazard","disable-sched-physreg-join"]:
    add(f"-mllvm -{b}"); add(f"-mllvm -{b}=false")
(OUT/"targeted.json").write_text(json.dumps(mk(T), indent=1))

# ---------------------------------------------------------------- extra (157)
E = []
def add2(flags): E.append(flags)
for b in ["machine-sink-split","force-loop-cold-block","phi-elim-split-all-critical-edges",
          "join-splitedges","disable-phi-elim-edge-splitting","no-phi-elim-live-out-early-exit",
          "addr-sink-new-phis"]:
    add2(f"-mllvm -{b}"); add2(f"-mllvm -{b}=false")
for v in [0,1,25,50,64,75,90,100]:
    add2(f"-mllvm -machine-sink-split-probability-threshold={v}")
for v in [0,1,2,4,8,16,32,64]:
    add2(f"-mllvm -loop-to-cold-block-ratio={v}")
    add2(f"-mllvm -callsite-splitting-duplication-threshold={v}")
    add2(f"-mllvm -tail-merge-size={v}")
    add2(f"-mllvm -tail-merge-threshold={v}")
add2("-mllvm -enable-tail-merge"); add2("-mllvm -enable-tail-merge=false")
for opt in ["align-all-blocks","align-all-nofallthru-blocks","align-all-functions"]:
    for v in [0,2,4,8,16]:
        add2(f"-mllvm -{opt}={v}")
for b in ["consthoist-with-block-frequency","disable-constant-hoisting","global-merge-on-const",
          "enable-global-merge","global-merge-group-by-use","combiner-split-load-index",
          "disable-separate-const-offset-from-gep","mul-constant-optimization"]:
    add2(f"-mllvm -{b}"); add2(f"-mllvm -{b}=false")
for v in [0,1,2,4,8,16,32,64,128]:
    add2(f"-mllvm -global-merge-max-offset={v}")
for v in [0,1,2,4,8,16,32,64,100,1000]:
    add2(f"-mllvm -memdep-block-number-limit={v}")
for v in [0,1,2,4,8,16,32,64,100]:
    add2(f"-mllvm -memdep-block-scan-limit={v}")
for b in ["extra-vectorizer-passes","disable-machine-dce","disable-machine-licm",
          "disable-postra-machine-licm","disable-copyprop","disable-cgp","disable-peephole",
          "disable-machine-cse","disable-branch-fold","disable-lftr","disable-lsr",
          "disable-licm-promotion","disable-complex-addr-modes","disable-basicaa",
          "combiner-global-alias-analysis","enable-aa-sched-mi","use-tbaa-in-sched-mi",
          "enable-double-float-shrink","pbqp-coalescing","enable-patchpoint-liveness",
          "disable-preheader-prot"]:
    add2(f"-mllvm -{b}"); add2(f"-mllvm -{b}=false")
(OUT/"extra.json").write_text(json.dumps(mk(E), indent=1))

# ---------------------------------------------------------------- sched (13)
S = []
for b in ["scheditins","schedmodel","misched-dcpl"]:
    S += [f"-mllvm -{b}", f"-mllvm -{b}=false"]
for v in [0,1,2,4,8,16,32]:
    S.append(f"-mllvm -dfa-instr-limit={v}")
(OUT/"sched.json").write_text(json.dumps(mk(S), indent=1))

# ---------------------------------------------------------------- combos (10)
C = [
"-mllvm -disable-ifcvt-simple-false -mllvm -sink-insts-to-avoid-spills",
"-mllvm -disable-ifcvt-simple-false -mllvm -disable-ifcvt-triangle-rev",
"-mllvm -disable-ifcvt-simple-false -mllvm -disable-peephole",
"-mllvm -sink-insts-to-avoid-spills -mllvm -machine-sink-split-probability-threshold=64",
"-mllvm -disable-ifcvt-triangle-rev -mllvm -ifcvt-limit=0",
"-mllvm -disable-ifcvt-simple-false -mllvm -machine-sink-split-probability-threshold=64 -mllvm -disable-peephole",
"-mllvm -sink-insts-to-avoid-spills -mllvm -disable-peephole",
"-mllvm -disable-ifcvt-simple-false -mllvm -disable-postra-machine-sink",
"-mllvm -disable-ifcvt-triangle-rev -mllvm -machine-sink-split-probability-threshold=64",
"-mllvm -disable-ifcvt-simple-false -mllvm -disable-ifcvt-triangle-rev -mllvm -sink-insts-to-avoid-spills -mllvm -machine-sink-split-probability-threshold=64 -mllvm -disable-peephole",
]
(OUT/"combos.json").write_text(json.dumps(mk(C), indent=1))

# ---------------------------------------------------------------- misc (71)
M = []
def add3(flags): M.append(flags)
for v in [0,1,2,4,8,16,32,64,128,256,1024]:
    add3(f"-mllvm -max-jump-table-size={v}")
for v in [0,1,2,4,8,16,32,64,100]:
    add3(f"-mllvm -optsize-jump-table-density={v}")
for v in [0,1,2,4,8,16,32]:
    add3(f"-mllvm -sched-high-latency-cycles={v}")
for v in [0,1,2,4,8,16,32,64]:
    add3(f"-mllvm -slp-schedule-budget={v}")
    add3(f"-mllvm -min-block-execution={v}")
    add3(f"-mllvm -max-num-inline-blocks={v}")
for v in [0,1,2,4,8,16,32,64,100,225]:
    add3(f"-mllvm -inlinehint-threshold={v}")
for b in ["use-segment-set-for-physregs","no-stack-coloring",
          "stackcoloring-lifetime-start-on-first-use","loop-vectorize-with-block-frequency",
          "pi-force-live-exit-outline"]:
    add3(f"-mllvm -{b}"); add3(f"-mllvm -{b}=false")
(OUT/"misc.json").write_text(json.dumps(mk(M), indent=1))

# ---------------------------------------------------------------- all bool (643)
# Parsed from the committed NDK r18b clang 7.0.2 -mllvm -help-hidden output.
HELP = Path(__file__).with_name("mllvm_help_hidden_clang702_r18b.txt")
lines = HELP.read_text().splitlines()
BOOL = []
for i, ln in enumerate(lines):
    m = re.match(r'^  -([A-Za-z0-9_.-]+)(=<[^>]+>)?\s{2,}(.*)$', ln)
    if not m:
        continue
    name, typ = m.group(1), m.group(2)
    if typ:
        continue                      # value-taking option
    nxt = lines[i + 1] if i + 1 < len(lines) else ""
    if re.match(r'^    =\S+\s{2,}', nxt):
        continue                      # enum option
    BOOL.append(name)
B = []
for b in BOOL:
    B.append(f"-mllvm -{b}")
    if b.startswith("enable-") or b.startswith("disable-"):
        B.append(f"-mllvm -{b}=false")
(OUT/"allbool.json").write_text(json.dumps(mk(B), indent=1))

for n in ["targeted","extra","sched","misc","combos","allbool"]:
    print(n, len(json.loads((OUT/f"{n}.json").read_text())))
