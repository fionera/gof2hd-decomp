#!/usr/bin/env python3
import os, sys, subprocess, shutil, json, re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
B=ROOT/'cmake-build-match'
NDK=ROOT/'.cache/ndk/android-ndk-r18b'
CLANG=NDK/'toolchains/llvm/prebuilt/linux-x86_64/bin/clang++'
OBJCOPY=NDK/'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/objcopy'
GOLD=NDK/'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold'
STRIP=NDK/'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip'
OBJDUMP=NDK/'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
FLAGS=('-target','armv7-none-linux-androideabi16','-march=armv7-a','-mthumb','-mfpu=neon','-mfloat-abi=softfp','-std=c++11','-fPIC','-frtti','-fstack-protector-strong','-Oz','-g','-stdlib=libc++',
 '-isystem',str(NDK/'sources/cxx-stl/llvm-libc++/include'),'-isystem',str(NDK/'sources/cxx-stl/llvm-libc++abi/include'),'-isystem',str(NDK/'sources/android/support/include'),
 '--sysroot='+str(NDK/'sysroot'),'-isystem',str(NDK/'sysroot/usr/include'),'-isystem',str(NDK/'sysroot/usr/include/arm-linux-androideabi'),
 '-D__ANDROID_API__=16','-Wno-int-to-pointer-cast','-Wno-int-to-void-pointer-cast',
 '-I'+str(ROOT/'src'),'-I'+str(ROOT/'third_party/fmod/inc'),'-I'+str(ROOT/'third_party/gl'),'-I'+str(ROOT/'third_party/jni'),'-I'+str(ROOT/'third_party/libzip'),'-I'+str(ROOT/'third_party/crypto'),
 '-ffunction-sections','-funwind-tables')
MARKER='.text.tuend.o107'
def sh(cmd, **kw):
    p=subprocess.run(cmd, text=True, capture_output=True, **kw)
    if p.returncode:
        print('CMD FAILED:', ' '.join(map(str,cmd)))
        print(p.stdout); print(p.stderr)
        raise SystemExit(p.returncode)
    return p
def compile_obj(src, out):
    sh([str(CLANG),*FLAGS,'-MD','-MT',str(out),'-MF',str(out)+'.d','-o',str(out),'-c',str(src)])
def make_marker(obj, out):
    sh([str(OBJCOPY),'--rename-section','.text='+MARKER,str(obj),str(out)])
def link(obj, out):
    rsp=Path('/tmp/link_order_variant.rsp')
    text=(B/'exidx_markers/link_order.rsp').read_text()
    old=str(B/'exidx_markers/objects/o107_Layout.cpp.obj')
    text=text.replace(old, str(obj))
    rsp.write_text(text)
    cmd=[str(GOLD),'-shared','-soname','libgof2hdaa.so','--build-id','-z','now','--exclude-libs=libgcc.a',
         '--section-ordering-file='+str(B/'section_order.txt'),'-o',str(out),
         str(B/'crtbegin_so_renamed.o'),'-L',str(NDK/'platforms/android-16/arch-arm/usr/lib'),
         '@'+str(rsp),
         '--whole-archive',str(B/'exidx_markers/archives/libz_gof2.a'),str(B/'exidx_markers/archives/libzip_gof2.a'),'--no-whole-archive',
         '-l','log','-l','GLESv2','-l','GLESv1_CM','-l','EGL','-l','android',
         str(ROOT/'../_work/bins/apk_libs/libfmodex.so'),str(ROOT/'../_work/bins/apk_libs/libfmodevent.so'),
         str(B/'exidx_markers/archives/libcrypto_gof2.a'),'-l','m',str(B/'exidx_markers/archives/libc++abi.a'),
         str(B/'exidx_markers/archives/libandroid_support.a'),str(B/'exidx_markers/archives/libgcc.a'),'-l','dl','-l','c',
         str(NDK/'platforms/android-16/arch-arm/usr/lib/crtend_so.o')]
    sh(cmd)
    sh([str(STRIP),'--strip-all',str(out)])
def score(so):
    sys.path.insert(0,str(ROOT/'tools/verify'))
    import verify
    of=verify.Elf32(str(ROOT/'../_work/bins/android_2.0.16_libgof2hdaa.so'))
    ue=verify.Elf32(str(so))
    orig=of.functions(); ours=ue.functions(); common=set(orig)&set(ours)
    def ar(elf):
        mapped=[s for s in elf.sections if s['addr']]
        lo=next((s['addr'] for s in mapped if s['name']=='.plt'),min(s['addr'] for s in mapped))
        return lo,max(s['addr']+s['size'] for s in mapped)
    ores=verify.Resolver(orig,common,of.plt_map(),ar(of)); ures=verify.Resolver(ours,common,ue.plt_map(),ar(ue))
    co={o[2] for o in of.objects()}&{o[2] for o in ue.objects()}
    for r,e in ((ores,of),(ures,ue)): r.attach_data(e,co)
    imports=({v for k,v in of.got_map().values() if k=='sym'}&{v for k,v in ue.got_map().values() if k=='sym'})
    ores.imports=ures.imports=imports
    out={}
    for name in ('_ZN6LayoutC1Ev','_ZN6LayoutC2Ev'):
        o,u=orig[name],ours[name]
        ob=of.bytes_at(o['value'],o['size']); ub=ue.bytes_at(u['value'],u['size'])
        _,on=verify.disassemble(str(OBJDUMP),ob,o['thumb'],o['value'],ores)
        _,un=verify.disassemble(str(OBJDUMP),ub,u['thumb'],u['value'],ures)
        on,un=verify.reconcile(on,un)
        out[name]=dict(osize=o['size'],usize=u['size'],byte=ob==ub,linked=(on==un and o['size']==u['size']),pct=100.0 if ob==ub else verify.score(on,un))
    return out
if __name__=='__main__':
    src=Path(sys.argv[1]); tag=sys.argv[2] if len(sys.argv)>2 else 'v'
    obj=Path('/tmp')/f'layout_{tag}.o'; marker=Path('/tmp')/f'layout_{tag}_marker.o'; so=Path('/tmp')/f'layout_{tag}.so'
    compile_obj(src,obj); make_marker(obj,marker); link(marker,so)
    res=score(so)
    print(json.dumps(res,indent=2))
