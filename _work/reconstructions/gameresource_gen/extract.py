"""Compress the Interp event stream into resource-entry records.

Each record: dict(kind='slot'|'add'|'branch'|'cmp'|'call', ...). A resource record has
  id, rkind, payload (tuple describing the payload ctor), slot (sp offset) or 'add'.
"""
import sys
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from armdis import *  # noqa


def fval(v):
    if v.k == 'c':
        return struct.unpack('<f', struct.pack('<I', v.v))[0]
    return None


def describe_payload(oid, ctor, fields, objs):
    if ctor is not None:
        name, args = ctor
        if 'ResourceTexture' in name:
            s = args[1]
            assert s.k == 'addr', args
            return ('tex', cstr(s.v), fval(args[2]))
        if 'ResourceMesh' in name:
            assert args[1].k == 'addr', args
            return ('mesh', cstr(args[1].v), args[2].v, args[3].v)
        if 'ResourceMaterialC2Ett' in name or 'ResourceMaterialC1Ett' in name:
            return ('mat2', args[1].v, args[2].v, args[3].v)
        if 'ResourceMaterialC2Et' in name or 'ResourceMaterialC1Et' in name:
            return ('mat1', args[1].v, args[2].v)
        return ('ctor', name, args)
    f = fields.get(oid, {})
    if objs[oid] == 4:
        lo = f.get(0)
        hi = f.get(2)
        if lo is not None and lo.k == 'c' and hi is None and lo.v > 0xffff:
            lo, hi = C(lo.v & 0xffff), C(lo.v >> 16)
        elif lo is not None and lo.k == 'c' and hi is None:
            hi = C(0)
        return ('image', lo.v if lo is not None and lo.k == 'c' else repr(lo),
                hi.v if hi is not None and hi.k == 'c' else repr(hi))
    return ('raw', objs[oid], {k: repr(v) for k, v in f.items()})


def extract(name):
    st, sz = func_range(name)
    insns, datas = decode(st, sz)
    it = Interp(insns, datas, plt_map(), got_map())
    ev = it.run()
    islands = set()
    for a, b in zip(insns, insns[1:]):
        if a.mnemonic in ('b', 'b.w') and a.operands[0].imm == b.address and b.address != a.address + a.size:
            islands.add(a.address)
    fields = {}
    ctors = {}
    out = []
    for e in ev:
        if e[0] == 'field':
            fields.setdefault(e[1], {})[e[2]] = e[3]
        elif e[0] == 'call':
            n = e[1]
            a = e[2]
            if n.startswith('_ZN11AbyssEngine') and ('C2E' in n or 'C1E' in n) and a[0].k == 'obj':
                ctors[a[0].v] = (n, a)
            elif n == '_ZN11AbyssEngine11PaintCanvas11AddResourceEPNS_8ResourceE':
                out.append(dict(t='add', res=a[1].v if a[1].k == 'obj' else None, addr=e[4], canvas=a[0]))
            elif n == '_Znwj':
                pass
            else:
                out.append(dict(t='call', name=n, args=a, addr=e[4]))
        elif e[0] == 'slot':
            if it.objs[e[2]] == 16:
                out.append(dict(t='slot', off=e[1], res=e[2], addr=e[3]))
        elif e[0] == 'branch' and e[3] in islands:
            pass
        elif e[0] in ('branch', 'cmp'):
            out.append(dict(t=e[0], a=e[1], b=e[2], addr=e[3]))
        elif e[0] in ('unk', 'stm?', 'store', 'vfp'):
            out.append(dict(t='other', e=e, addr=e[-1]))
    # resolve resources
    for r in out:
        if r['t'] in ('slot', 'add') and r.get('res') is not None:
            f = fields.get(r['res'], {})
            pid = f.get(12)
            r['id'] = f.get(0)
            r['rkind'] = f.get(4)
            r['unused'] = f.get(8)
            if pid is not None and pid.k == 'obj':
                r['payload'] = describe_payload(pid.v, ctors.get(pid.v), fields, it.objs)
            else:
                r['payload'] = ('val', pid)
    return out, it


def fmt(r):
    if r['t'] in ('slot', 'add'):
        idv = r['id']
        ids = str(idv.v) if idv is not None and idv.k == 'c' else repr(idv)
        k = r['rkind'].v if r['rkind'] is not None and r['rkind'].k == 'c' else repr(r['rkind'])
        where = ('slot %d' % r['off']) if r['t'] == 'slot' else 'ADD'
        return '%x %s id=%s kind=%s unused=%r %r' % (r['addr'], where, ids, k, r['unused'], r['payload'])
    if r['t'] == 'call':
        return '%x CALL %s %r' % (r['addr'], r['name'], r['args'])
    if r['t'] in ('branch', 'cmp'):
        return '%x %s %s %s' % (r['addr'], r['t'].upper(), r['a'], r['b'])
    return '%x OTHER %r' % (r['addr'], r['e'])


if __name__ == '__main__':
    out, it = extract(sys.argv[1])
    for r in out:
        if r['t'] == 'branch' and r['a'] == 'b.w' and len(sys.argv) < 3:
            pass
        print(fmt(r))
