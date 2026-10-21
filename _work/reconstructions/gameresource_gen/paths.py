"""Path-sensitive execution of a region of an original builder for every assignment of
the global flags it tests (Globals::iPadHD etc.), printing the AddResource sequence each
assignment produces. Used to recover the source-level if/else nesting of the tail of
BuildResourceList (whose machine code is heavily tail-merged).

usage: paths.py <mangled> <start-hex> <stop-hex> flag1,flag2,...
"""
import sys
import copy
import itertools
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from extract import *  # noqa


def run_paths(name, start, stop, flags, pre_stop=None):
    st, sz = func_range(name)
    insns, datas = decode(st, sz)
    idx = {i.address: k for k, i in enumerate(insns)}
    base = Interp(insns, datas, plt_map(), got_map())
    for i in insns:
        if i.address >= start:
            break
        base.step(i)
    results = {}
    for vals in itertools.product([0, 1], repeat=len(flags)):
        fl = dict(zip(flags, vals))
        it = copy.copy(base)
        it.regs = dict(base.regs)
        it.stack = dict(base.stack)
        it.heap = dict(base.heap)
        it.objs = list(base.objs)
        it.events = []
        k = idx[start]
        last_cmp = None
        steps = 0
        while True:
            i = insns[k]
            if i.address == stop:
                break
            steps += 1
            assert steps < 200000
            mn = i.mnemonic
            if mn == 'cmp':
                last_cmp = (it.g(i.operands[0].reg), it.opval(i, i.operands[1]))
                k += 1
                continue
            if mn in ('b', 'b.w'):
                k = idx[i.operands[0].imm]
                continue
            if mn.split('.')[0] in ('beq', 'bne'):
                lhs, rhs = last_cmp
                assert lhs.k == 'gval' and rhs == C(0), (hex(i.address), lhs, rhs)
                v = fl[lhs.v[0]]
                taken = (v == 0) if mn.startswith('beq') else (v != 0)
                k = idx[i.operands[0].imm] if taken else k + 1
                continue
            if mn in ('cbz', 'cbnz') or (mn.startswith('b') and mn.split('.')[0] in
                                          ('blo', 'bhs', 'bhi', 'bls', 'blt', 'bge', 'bgt', 'ble')):
                raise RuntimeError('unhandled branch %x %s' % (i.address, mn))
            it.step(i)
            k += 1
        # resources added
        fields = {}
        ctors = {}
        seq = []
        for e in it.events:
            if e[0] == 'field':
                fields.setdefault(e[1], {})[e[2]] = e[3]
            elif e[0] == 'call':
                n, a = e[1], e[2]
                if ('C2E' in n or 'C1E' in n) and a[0].k == 'obj':
                    ctors[a[0].v] = (n, a)
                elif n.endswith('AddResourceEPNS_8ResourceE'):
                    seq.append(('add', a[1].v))
                elif n != '_Znwj':
                    seq.append(('call', n))
        recs = []
        for s in seq:
            if s[0] == 'add':
                f = fields.get(s[1], {})
                pid = f.get(12)
                pay = describe_payload(pid.v, ctors.get(pid.v), fields, it.objs) if pid is not None and pid.k == 'obj' else ('val', pid)
                recs.append((f.get(0).v, f.get(4).v, f.get(8).v, pay))
            else:
                recs.append(s)
        results[vals] = recs
    return results


if __name__ == '__main__':
    name = sys.argv[1]
    start, stop = int(sys.argv[2], 16), int(sys.argv[3], 16)
    flags = sys.argv[4].split(',')
    res = run_paths(name, start, stop, flags)
    for vals, recs in res.items():
        print('#### ' + ' '.join('%s=%d' % (f, v) for f, v in zip(flags, vals)))
        for r in recs:
            print('   ', r)
