"""Normalized diff of original vs ours for one function (literal pools skipped, addresses,
pool offsets, branch targets and sp offsets stripped) so stack-layout noise is separated
from real codegen differences.
usage: normdiff.py <mangled> <ours.so> [--keep-sp]"""
import sys, re, difflib, os
sys.path.insert(0, __file__.rsplit('/', 1)[0])
import armdis


def norm(so, name, keep_sp):
    armdis.use_binary(so)
    st, sz = armdis.func_range(name)
    insns, _ = armdis.decode(st, sz)
    out = []
    for i in insns:
        mn, ops = i.mnemonic, i.op_str
        if mn == 'bl':
            mn = 'blx'
        if mn.startswith('b') or mn.startswith('cb'):
            ops = re.sub(r'#0x[0-9a-f]+', 'T', ops)
        if 'pc' in ops:
            ops = re.sub(r'#-?(0x)?[0-9a-f]+', '#P', ops)
        if not keep_sp:
            ops = re.sub(r'(sp|lr), #(0x)?[0-9a-f]+', r'\1, #S', ops)
        if os.environ.get('NOREG'):
            ops = re.sub(r'\b(r\d+|sb|sl|fp|ip|lr)\b', 'R', ops)
        out.append((i.address, (mn.replace('.w', '') + ' ' + ops).strip()))
    return out


name, ours = sys.argv[1], sys.argv[2]
keep = '--keep-sp' in sys.argv
a = norm(armdis.SO, name, keep)
b = norm(ours, name, keep)
sm = difflib.SequenceMatcher(None, [x[1] for x in a], [x[1] for x in b], autojunk=False)
n = 0
mx = int(os.environ.get('MAXH', '40'))
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        continue
    n += 1
    if n <= mx:
        print('--- %s orig %x(+%d) ours %x(+%d)' % (tag, a[min(i1, len(a) - 1)][0], i2 - i1, b[min(j1, len(b) - 1)][0], j2 - j1))
        for x in a[i1:min(i2, i1 + 10)]:
            print('   - ' + x[1])
        for x in b[j1:min(j2, j1 + 10)]:
            print('   + ' + x[1])
print('hunks', n, 'ratio %.4f' % sm.ratio(), 'orig', len(a), 'ours', len(b))
