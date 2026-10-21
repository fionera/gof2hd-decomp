"""Regenerate the entry list of loadPortraits (src/engine/render/ResourceMaterial.cpp) from
the ORIGINAL binary. Each entry is one full-expression
    canvas->AddResource(new Resource(id, RESOURCE_TEXTURE,
        new ResourceTexture(String(<base>, false) + suffix + String(".aei", false), 0.0f)));
The device suffix prologue is hand-written (kept verbatim in the target file); this script
rewrites everything between the BEGIN/END GENERATED markers.

  python3 _work/reconstructions/gameresource_gen/gen_portraits.py src/engine/render/ResourceMaterial.cpp
"""
import sys
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from armdis import *  # noqa
from emit import cstr_lit, flt, KIND  # noqa

FN = '_Z13loadPortraitsPN11AbyssEngine6EngineE'
BEGIN = '    // BEGIN GENERATED (gen_portraits.py)\n'
END = '    // END GENERATED\n'


def entries():
    st, sz = func_range(FN)
    insns, datas = decode(st, sz)
    it = Interp(insns, datas, plt_map(), got_map())
    ev = it.run()
    fields = {}
    strs = []
    tex = None
    out = []
    for e in ev:
        if e[0] == 'field':
            fields.setdefault(e[1], {})[e[2]] = e[3]
        elif e[0] == 'call':
            n, a = e[1], e[2]
            if n == '_ZN11AbyssEngine6StringC1EPKcb':
                assert a[1].k == 'addr' and a[2] == C(0), a
                strs.append(cstr(a[1].v))
            elif n == '_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf':
                tex = struct.unpack('<f', struct.pack('<I', a[2].v))[0]
            elif n.endswith('AddResourceEPNS_8ResourceE'):
                f = fields[a[1].v]
                out.append((f[0].v, f[4].v, f[8].v, list(strs), tex))
                strs = []
    return out


def main(path):
    src = open(path).read()
    a = src.index(BEGIN) + len(BEGIN)
    b = src.index(END)
    lines = []
    first = True
    for rid, kind, unused, strs, tex in entries():
        if first:
            # the suffix String is built once before the first entry
            assert strs[0] in ('_ipad_large', '_ipad_1440', '_ipad', ''), strs
            strs = strs[1:]
            first = False
        assert unused == 0xffffffff and len(strs) == 2 and strs[1] == '.aei', (rid, strs)
        lines.append('    canvas->AddResource(new Resource(%d, %s, new ResourceTexture(\n'
                     '        String(%s, false) + suffix + String(".aei", false), %s)));\n'
                     % (rid, KIND[kind], cstr_lit(strs[0]), flt(tex)))
    open(path, 'w').write(src[:a] + ''.join(lines) + src[b:])


if __name__ == '__main__':
    main(sys.argv[1])
