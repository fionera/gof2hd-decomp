#!/usr/bin/env python3
"""Fast A/B harness: recompile only the 16 TUs owning the 24 residual functions,
relink with the canonical gold+strip+shstrtab chain, then score those functions.

Usage:
  python3 harness.py --variant-file variants.json --out results.json [--jobs 6]
  python3 harness.py --flags "-mllvm -foo" --name foo   # single variant
"""
import argparse, json, os, shlex, subprocess, sys, tempfile, hashlib
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, as_completed

ROOT = Path("/home/debian/GalaxyOnFire2/wt/w5424")
BUILD = ROOT / "cmake-build-match"
ORIGINAL = Path("/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so")
SCRATCH = Path("/dev/shm/w5424")
sys.path.insert(0, str(ROOT / "tools/verify"))
sys.path.insert(0, str(ROOT / "tools"))
import verify
from elf import Elf32
from flag_fingerprint import function_blobs

TARGET_TUS = [
 "src/game/core/Globals.cpp","src/game/menu/MGame.cpp","src/game/menu/ModStation.cpp",
 "src/game/menu/SpaceLounge.cpp","src/game/mission/Generator.cpp","src/game/mission/Status.cpp",
 "src/game/ship/PlayerEgo.cpp","src/game/ui/HangarWindow.cpp","src/game/ui/Hud.cpp",
 "src/game/ui/Layout.cpp","src/game/ui/MenuTouchWindow.cpp","src/game/weapons/Gun.cpp",
 "src/game/weapons/Radar.cpp","src/game/weapons/RocketGun.cpp","src/game/world/Level.cpp",
 "src/game/world/LevelScript.cpp"]

TARGET_FUNCS = [
 "_ZN6LayoutC1Ev","_ZN6LayoutC2Ev","_ZN11LevelScript7processEi",
 "_ZN6Status16missionCompletedEbbx","_ZN5Level10createShipEiiiP8Waypointbb",
 "_ZN5Radar4drawEP6PlayerP3Hudi","_ZN5MGame10OnTouchEndEiiPv","_ZN7GlobalsC1Ev",
 "_ZN7GlobalsC2Ev","_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE",
 "_ZN12HangarWindow6renderEv","_ZN9RocketGun6updateEi",
 "_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE","_ZN5Level11createSceneEv",
 "_ZN3Gun22calcCharacterCollisionEv","_ZN5Level13createMissionEv",
 "_ZN3Hud4drawExxP9PlayerEgobjj","_ZN15MenuTouchWindowC1Ei","_ZN15MenuTouchWindowC2Ei",
 "_ZN11SpaceLounge10drawLoungeEv","_ZN15MenuTouchWindow12OnTouchBeginEiiPv",
 "_ZN10ModStationC1Ev","_ZN10ModStationC2Ev","_ZN5MGame8OnUpdateEv"]

def load_cc():
    cc = json.loads((BUILD/"compile_commands.json").read_text())
    byfile = {e["file"]: e for e in cc}
    out = []
    for t in TARGET_TUS:
        e = byfile[str(ROOT/t)]
        out.append(e)
    return out

def link_chain():
    p = subprocess.run(["ninja","-t","commands","link_so"], cwd=BUILD,
                       capture_output=True, text=True, check=True)
    lines=[l for l in p.stdout.splitlines() if "ld.gold" in l and "-shared" in l]
    assert len(lines)==1, len(lines)
    return lines[0]

def obj_abs(entry):
    return str((BUILD/entry["output"]).resolve())

def compile_tu(entry, extra, out):
    cmd = shlex.split(entry["command"])
    i = cmd.index("-o")
    cmd[i+1] = str(out)
    if extra:
        cmd += shlex.split(extra)
    p = subprocess.run(cmd, cwd=entry["directory"], capture_output=True, text=True)
    if p.returncode != 0:
        raise RuntimeError(f"compile failed: {entry['file']}\n{(p.stderr or '')[-2000:]}")

def build_variant(tag, extra, cc_entries, chain, scratch):
    objdir = scratch/tag/"obj"
    objdir.mkdir(parents=True, exist_ok=True)
    subs = {}
    for e in cc_entries:
        out = objdir/(Path(e["output"]).name)
        compile_tu(e, extra, out)
        subs[obj_abs(e)] = str(out)
    so = scratch/tag/"libgof2hdaa.so"
    unstripped = scratch/tag/"libgof2hdaa.unstripped.so"
    c = chain.replace("/./", "/")
    for a,b in subs.items():
        c = c.replace(a,b)
    c = c.replace(str(BUILD/"libgof2hdaa.unstripped.so"), str(unstripped))
    c = c.replace(str(BUILD/"libgof2hdaa.so"), str(so))
    p = subprocess.run(["bash","-c",c], cwd=BUILD, capture_output=True, text=True)
    if p.returncode != 0 or not so.exists():
        raise RuntimeError(f"link failed: {tag}\n{(p.stderr or '')[-3000:]}")
    return so

# ---- scoring ----
_orig_elf=None; _orig=None; _orig_res=None
def _init_orig(objdump):
    global _orig_elf,_orig,_orig_res
    _orig_elf=Elf32(ORIGINAL); _orig=_orig_elf.functions()
    def addr_range(elf):
        mapped=[s for s in elf.sections if s["addr"]]
        lo=next((s["addr"] for s in mapped if s["name"]==".plt"),min(s["addr"] for s in mapped))
        return lo,max(s["addr"]+s["size"] for s in mapped)
    _orig_res=verify.Resolver(_orig,set(_orig), _orig_elf.plt_map(), addr_range(_orig_elf))
    _orig_res.attach_data(_orig_elf,{o[2] for o in _orig_elf.objects()})

def score_so(so, objdump):
    global _orig_elf,_orig,_orig_res
    ours_elf=Elf32(so); ours=ours_elf.functions()
    common=set(_orig)&set(ours)
    # rebuild resolvers with the real common set for fair normalization
    def addr_range(elf):
        mapped=[s for s in elf.sections if s["addr"]]
        lo=next((s["addr"] for s in mapped if s["name"]==".plt"),min(s["addr"] for s in mapped))
        return lo,max(s["addr"]+s["size"] for s in mapped)
    ores=verify.Resolver(_orig,common,_orig_elf.plt_map(),addr_range(_orig_elf))
    ures=verify.Resolver(ours,common,ours_elf.plt_map(),addr_range(ours_elf))
    common_objects={o[2] for o in _orig_elf.objects()}&{o[2] for o in ours_elf.objects()}
    for res,e in ((ores,_orig_elf),(ures,ours_elf)):
        res.attach_data(e,common_objects)
    imports=({v for k,v in _orig_elf.got_map().values() if k=="sym"}
             &{v for k,v in ours_elf.got_map().values() if k=="sym"})
    ores.imports=ures.imports=imports
    # byte_exact + size-exact over all common functions
    byte_exact=0; size_exact=0; compared=0
    ob=function_blobs(ORIGINAL); vb=function_blobs(so)
    for n in common:
        o=_orig[n]; u=ours[n]
        if o["size"]>0 and u["size"]==0: continue
        if o["size"]==0 and u["size"]>0: continue
        compared+=1
        if o["size"]==u["size"]: size_exact+=1
        if ob[n][1]==vb[n][1]: byte_exact+=1
    # fuzzy for the 24
    res={}
    for n in TARGET_FUNCS:
        o=_orig[n]; u=ours[n]
        obb=_orig_elf.bytes_at(o["value"],o["size"])
        ubb=ours_elf.bytes_at(u["value"],u["size"])
        be=obb==ubb
        if be:
            pct=100.0
        else:
            _,on=verify.disassemble(objdump,obb,o["thumb"],o["value"],ores)
            _,un=verify.disassemble(objdump,ubb,u["thumb"],u["value"],ures)
            on,un=verify.reconcile(on,un)
            pct=verify.score(on,un)
        res[n]=dict(pct=pct,byte=be,osize=o["size"],usize=u["size"])
    return dict(byte_exact=byte_exact,size_exact=size_exact,compared=compared,targets=res)

def run_one(spec, cc_entries, chain, objdump, base_blobs):
    tag=spec["tag"]; extra=spec.get("flags","")
    try:
        so=build_variant(tag,extra,cc_entries,chain,SCRATCH/"screen")
        sc=score_so(so,objdump)
        return dict(tag=tag,flags=extra,ok=True,**sc)
    except Exception as e:
        return dict(tag=tag,flags=extra,ok=False,error=str(e)[-2000:])

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--variant-file")
    ap.add_argument("--flags")
    ap.add_argument("--name",default="single")
    ap.add_argument("--out",default=str(SCRATCH/"results.json"))
    ap.add_argument("--jobs",type=int,default=6)
    args=ap.parse_args()
    if args.variant_file:
        specs=json.loads(Path(args.variant_file).read_text())
    else:
        specs=[dict(tag=args.name,flags=args.flags or "")]
    cc_entries=load_cc(); chain=link_chain()
    objdump=str(BUILD/"CMakeCache.txt")
    import re
    m=re.search(r"GOF2_NDK_OBJDUMP:\w+=(.*)",Path(BUILD/"CMakeCache.txt").read_text())
    objdump=m.group(1).strip()
    _init_orig(objdump)
    base_blobs=function_blobs(ORIGINAL)
    results=[]
    if args.jobs<=1:
        for s in specs:
            r=run_one(s,cc_entries,chain,objdump,base_blobs); results.append(r)
            print(json.dumps({k:v for k,v in r.items() if k!='targets'}),flush=True)
    else:
        with ProcessPoolExecutor(max_workers=args.jobs) as ex:
            futs={ex.submit(run_one,s,cc_entries,chain,objdump,base_blobs):s for s in specs}
            for f in as_completed(futs):
                r=f.result(); results.append(r)
                print(json.dumps({k:v for k,v in r.items() if k!='targets'}),flush=True)
    results.sort(key=lambda r:r["tag"])
    Path(args.out).write_text(json.dumps(results,indent=1))

if __name__=="__main__":
    main()
