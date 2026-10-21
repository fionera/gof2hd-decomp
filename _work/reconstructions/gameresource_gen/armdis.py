"""Thumb-2 disassembly + symbolic interpretation of the original GameResource.cpp builders.

Used by gen_*.py in this directory to recover the exact entry sequence of the huge
straight-line resource-table builders (BuildResourceList, loadLowTexturesAndMaterials,
loadPortraits) from the ORIGINAL libgof2hdaa.so (android 2.0.16).

  - decode(): linear Thumb decode that skips the inline literal-pool islands clang emits
    (data words found from every pc-relative `ldr` target).
  - Interp: tracks constants / string addresses / heap objects / stack slots through the
    straight-line code and emits an event list (calls with symbolic args, stores of
    objects into stack slots, branches).
"""
import struct
import subprocess
import re
import capstone
from capstone import arm as A

SO = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OBJDUMP = ('.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/'
           'linux-x86_64/bin/arm-linux-androideabi-objdump')

_data = open(SO, 'rb').read()
_bias = 0  # file offset of address 0 (non-zero only for a relocatable .o section)


def use_binary(path):
    """Switch the module to another .so (e.g. our build) for decode()/func_range()."""
    global SO, _data, _bias
    _bias = 0
    SO = path
    _data = open(path, 'rb').read()


def u32(a):
    return struct.unpack_from('<I', _data, a)[0]


def cstr(a):
    e = _data.index(b'\0', a)
    return _data[a:e].decode('latin1')


def plt_map():
    out = subprocess.run([OBJDUMP, '-d', '-j', '.plt', SO], capture_output=True, text=True).stdout
    m = {}
    for line in out.splitlines():
        mm = re.match(r'([0-9a-f]+) <(.*)@plt>:', line)
        if mm:
            m[int(mm.group(1), 16)] = mm.group(2)
    return m


def got_map():
    out = subprocess.run(['readelf', '-rW', '--dyn-syms', SO], capture_output=True, text=True).stdout
    m = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 5 and p[2] in ('R_ARM_GLOB_DAT', 'R_ARM_ABS32'):
            m[int(p[0], 16)] = p[4].split('@')[0]
    return m


def func_range(name):
    global _bias
    if SO.endswith('.o') or SO.endswith('.obj'):
        # relocatable object: function lives in its own .text.<name> section at offset 0
        out = subprocess.run(['readelf', '-SW', SO], capture_output=True, text=True).stdout
        for line in out.splitlines():
            m = re.search(r'\]\s+\.text\.(\S+)\s+PROGBITS\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)', line)
            if m and m.group(1) == name:
                _bias = int(m.group(3), 16)
                return 0, int(m.group(4), 16)
        raise KeyError(name)
    out = subprocess.run(['readelf', '-sW', SO], capture_output=True, text=True).stdout
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 8 and p[7].split('@')[0] == name:
            return int(p[1], 16) & ~1, int(p[2], 0)
    raise KeyError(name)


_md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
_md.detail = True


def decode(start, size):
    end = start + size
    datas = set()
    insns = []
    a = start
    while a < end:
        if a in datas:
            a += 4
            continue
        if (a + 2) in datas and a % 4 == 2:  # should not happen
            a += 2
            continue
        code = _data[a + _bias:min(a + 4, end) + _bias]
        il = list(_md.disasm(code, a, 1))
        if not il:
            a += 2
            continue
        i = il[0]
        for op in i.operands:
            if op.type == A.ARM_OP_MEM and op.mem.base == A.ARM_REG_PC:
                t = ((i.address + 4) & ~3) + op.mem.disp
                datas.add(t)
                if i.mnemonic.startswith('ldrd'):
                    datas.add(t + 4)
        insns.append(i)
        a += i.size
    return insns, datas


class V:
    """Symbolic value."""
    __slots__ = ('k', 'v')

    def __init__(self, k, v=None):
        self.k = k
        self.v = v

    def __repr__(self):
        if self.k == 'c':
            return '#%d' % self.v
        return '%s(%r)' % (self.k, self.v)

    def __eq__(self, o):
        return isinstance(o, V) and self.k == o.k and self.v == o.v

    def __hash__(self):
        return hash((self.k, self.v))


def C(x):
    return V('c', x & 0xffffffff)


REGN = {}
for n in range(16):
    pass


def rname(r):
    return _md.reg_name(r)


class Interp:
    def __init__(self, insns, datas, plt, got):
        self.insns = insns
        self.datas = datas
        self.plt = plt
        self.got = got
        self.regs = {}
        self.stack = {}
        self.heap = {}  # (objid, off) -> V
        self.objs = []  # sizes
        self.events = []
        self.unknown = set()

    def g(self, r):
        n = rname(r)
        return self.regs.get(n, V('u', n))

    def s(self, r, v):
        self.regs[rname(r)] = v

    def opval(self, i, op):
        if op.type == A.ARM_OP_IMM:
            return C(op.imm)
        if op.type == A.ARM_OP_REG:
            v = self.g(op.reg)
            if op.shift.type == A.ARM_SFT_LSL and op.shift.value and v.k == 'c':
                return C(v.v << op.shift.value)
            if op.shift.type:
                return V('u', 'shift')
            return v
        return V('u', 'op')

    def addr_of(self, i, op):
        """returns ('sp', off) / ('obj', id, off) / ('pc', addr) / ('got', name) / None"""
        m = op.mem
        if m.base == A.ARM_REG_PC:
            return ('pc', ((i.address + 4) & ~3) + m.disp)
        base = self.g(m.base) if rname(m.base) != 'sp' else V('sp', 0)
        disp = m.disp
        if m.index:
            iv = self.g(m.index)
            if iv.k != 'c':
                return None
            disp += iv.v << (op.shift.value if op.shift.type == A.ARM_SFT_LSL else 0)
        if base.k == 'sp':
            return ('sp', base.v + disp)
        if base.k == 'obj':
            return ('obj', base.v, disp)
        if base.k == 'gotslot':
            return ('gotslot', base.v, disp)
        if base.k == 'gotptr':
            return ('gotptr', base.v, disp)
        if base.k == 'spa':
            return ('sp', base.v + disp)
        return ('other', repr(base), disp)

    def load(self, ad):
        if ad is None:
            return V('u', 'load')
        if ad[0] == 'pc':
            return C(u32(ad[1]))
        if ad[0] == 'sp':
            return self.stack.get(ad[1], V('stk', ad[1]))
        if ad[0] == 'obj':
            return self.heap.get((ad[1], ad[2]), V('u', 'heap'))
        if ad[0] == 'gotslot':
            # address of GOT entry -> value is &symbol
            return V('gsym', self.got.get(ad[1] + ad[2], hex(ad[1] + ad[2])))
        if ad[0] == 'other':
            b = ad[1]
            return V('deref', (b, ad[2]))
        return V('u', 'load')

    def store(self, ad, v, i):
        if ad is None:
            return
        if ad[0] == 'sp':
            self.stack[ad[1]] = v
            if v.k == 'obj':
                self.events.append(('slot', ad[1], v.v, i.address))
        elif ad[0] == 'obj':
            self.heap[(ad[1], ad[2])] = v
            self.events.append(('field', ad[1], ad[2], v, i.address))
        else:
            self.events.append(('store', ad, v, i.address))

    def run(self):
        for i in self.insns:
            try:
                self.step(i)
            except Exception as e:  # pragma: no cover
                raise RuntimeError('%x %s %s: %s' % (i.address, i.mnemonic, i.op_str, e))
        return self.events

    def step(self, i):
        mn = i.mnemonic.split('.')[0]
        ops = i.operands
        if mn in ('mov', 'movs', 'movw'):
            if rname(ops[1].reg) == 'sp' if ops[1].type == A.ARM_OP_REG else False:
                self.s(ops[0].reg, V('spa', 0))
            else:
                self.s(ops[0].reg, self.opval(i, ops[1]))
        elif mn == 'movt':
            v = self.g(ops[0].reg)
            if v.k == 'c':
                self.s(ops[0].reg, C((v.v & 0xffff) | (ops[1].imm << 16)))
            else:
                self.s(ops[0].reg, V('u', 'movt'))
        elif mn in ('mvn', 'mvns'):
            v = self.opval(i, ops[1])
            self.s(ops[0].reg, C(~v.v) if v.k == 'c' else V('u', 'mvn'))
        elif mn in ('add', 'adds', 'addw', 'sub', 'subs', 'subw'):
            if len(ops) == 2:
                a, b = self.g(ops[0].reg), self.opval(i, ops[1])
                srcreg = ops[1]
            else:
                if ops[1].type == A.ARM_OP_REG and rname(ops[1].reg) == 'sp':
                    a = V('spa', 0)
                elif ops[1].type == A.ARM_OP_REG and rname(ops[1].reg) == 'pc':
                    a = V('pcv', (i.address + 4) & ~3)
                else:
                    a = self.g(ops[1].reg)
                b = self.opval(i, ops[2])
                srcreg = ops[2]
            if srcreg.type == A.ARM_OP_REG and rname(srcreg.reg) == 'pc':
                # add rX, pc
                if a.k == 'c':
                    self.s(ops[0].reg, V('addr', (a.v + i.address + 4) & 0xffffffff))
                else:
                    self.s(ops[0].reg, V('u', 'addpc'))
                return
            sub = mn.startswith('sub')
            if a.k == 'c' and b.k == 'c':
                r = C(a.v - b.v if sub else a.v + b.v)
            elif a.k == 'spa' and b.k == 'c':
                r = V('spa', a.v - b.v if sub else a.v + b.v)
            elif a.k == 'pcv' and b.k == 'c':
                r = V('addr', a.v + b.v)
            elif a.k == 'c' and b.k == 'spa' and not sub:
                r = V('spa', a.v + b.v)
            elif a.k == 'obj' and b.k == 'c' and not sub:
                r = V('objp', (a.v, b.v))
            else:
                r = V('u', 'arith')
            if rname(ops[0].reg) == 'sp':
                return
            self.s(ops[0].reg, r)
        elif mn in ('uxth', 'uxtb', 'sxth'):
            v = self.opval(i, ops[1])
            self.s(ops[0].reg, C(v.v & (0xff if mn == 'uxtb' else 0xffff)) if v.k == 'c' else V('u', mn))
        elif mn == 'adr':
            self.s(ops[0].reg, V('addr', ((i.address + 4) & ~3) + ops[1].imm))
        elif mn in ('ldr', 'ldrh', 'ldrb', 'ldrsh', 'ldrsb'):
            ad = self.addr_of(i, ops[1])
            v = self.load(ad)
            if mn != 'ldr' and v.k == 'c':
                v = C(v.v & (0xffff if 'h' in mn else 0xff))
            # GOT idiom: ldr rX,[pc]; add rX,pc; ldr rX,[rX]
            base = self.g(ops[1].mem.base) if ops[1].mem.base and rname(ops[1].mem.base) not in ('pc', 'sp') else None
            if base is not None and base.k == 'addr' and ops[1].mem.disp == 0 and not ops[1].mem.index:
                v = V('gsym', self.got.get(base.v, hex(base.v)))
            elif base is not None and base.k == 'gsym':
                v = V('gval', (base.v, ops[1].mem.disp))
            self.s(ops[0].reg, v)
            if len(ops) > 2 or i.writeback:
                pass
        elif mn == 'ldrd':
            ad = self.addr_of(i, ops[2])
            self.s(ops[0].reg, self.load(ad))
            if ad and ad[0] == 'sp':
                self.s(ops[1].reg, self.load(('sp', ad[1] + 4)))
            else:
                self.s(ops[1].reg, V('u', 'ldrd'))
        elif mn in ('str', 'strh', 'strb'):
            ad = self.addr_of(i, ops[1])
            v = self.g(ops[0].reg)
            if mn != 'str' and v.k == 'c':
                v = C(v.v & (0xffff if mn == 'strh' else 0xff))
            if ad and ad[0] == 'obj':
                v = V(v.k, v.v)
                self.store(ad, v, i)
                if mn != 'str':
                    self.events[-1] = self.events[-1] + (mn,)
            else:
                self.store(ad, v, i)
        elif mn == 'strd':
            ad = self.addr_of(i, ops[2])
            self.store(ad, self.g(ops[0].reg), i)
            if ad and ad[0] == 'sp':
                self.store(('sp', ad[1] + 4), self.g(ops[1].reg), i)
            elif ad and ad[0] == 'obj':
                self.store(('obj', ad[1], ad[2] + 4), self.g(ops[1].reg), i)
        elif mn == 'stm' or mn == 'stmia':
            b = self.g(ops[0].reg)
            if b.k == 'objp':
                oid, off = b.v
                for k, op in enumerate(ops[1:]):
                    self.store(('obj', oid, off + 4 * k), self.g(op.reg), i)
            elif b.k == 'spa':
                for k, op in enumerate(ops[1:]):
                    self.stack[b.v + 4 * k] = self.g(op.reg)
            else:
                self.events.append(('stm?', repr(b), i.address))
            if i.writeback:
                self.s(ops[0].reg, V('u', 'wb'))
        elif mn in ('bl', 'blx'):
            t = ops[0].imm
            name = self.plt.get(t, 'local_%x' % t)
            args = [self.regs.get(r, V('u', r)) for r in ('r0', 'r1', 'r2', 'r3')]
            sargs = [self.stack.get(o, None) for o in (0, 4, 8)]
            if name == '_Znwj':
                oid = len(self.objs)
                self.objs.append(args[0].v if args[0].k == 'c' else None)
                res = V('obj', oid)
            else:
                res = V('ret', (name, i.address))
            self.events.append(('call', name, args, sargs, i.address, res))
            for r in ('r0', 'r1', 'r2', 'r3', 'r12', 'lr'):
                self.regs.pop(r, None)
            self.regs['r0'] = res
        elif mn in ('b', 'cbz', 'cbnz') or (mn.startswith('b') and len(mn) == 3 and mn not in ('bic',)):
            self.events.append(('branch', i.mnemonic, i.op_str, i.address))
        elif mn in ('cmp', 'cmn', 'tst', 'it', 'nop', 'push', 'pop'):
            if mn == 'cmp':
                self.events.append(('cmp', repr(self.g(ops[0].reg)), repr(self.opval(i, ops[1])), i.address))
        elif mn == 'stmdb':
            pass
        elif mn in ('vldr', 'vstr', 'vmov'):
            self.events.append(('vfp', i.mnemonic, i.op_str, i.address))
        else:
            self.unknown.add(mn)
            if ops and ops[0].type == A.ARM_OP_REG:
                self.s(ops[0].reg, V('u', mn))
            self.events.append(('unk', i.mnemonic, i.op_str, i.address))
