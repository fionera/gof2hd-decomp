#!/usr/bin/env python3
"""Extract ARM Thumb TBH/TBB jump tables from a linked .so and report the
physical order of the case blocks (sorted by target address) versus numeric
case order.  Also maps each table to our function symbols (unstripped .so)."""
import struct, re, subprocess, sys, os, bisect

import os
REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OBJDUMP = os.path.join(REPO, ".cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump")

class ELF:
    def __init__(self, path):
        self.data = open(path,'rb').read()
        d = self.data
        assert d[:4] == b'\x7fELF'
        self.is64 = d[4] == 2
        if self.is64:
            e_shoff, = struct.unpack_from('<Q', d, 0x28)
            e_shentsize, e_shnum, e_shstrndx = struct.unpack_from('<HHH', d, 0x3a)
        else:
            e_shoff, = struct.unpack_from('<I', d, 0x20)
            e_shentsize, e_shnum, e_shstrndx = struct.unpack_from('<HHH', d, 0x2e)
        self.sections = []
        for i in range(e_shnum):
            off = e_shoff + i*e_shentsize
            if self.is64:
                name, typ, flags, addr, offset, size = struct.unpack_from('<IIQQQQ', d, off)
                link, info, align, entsize = struct.unpack_from('<IIQQ', d, off+0x28)
            else:
                name, typ, flags, addr, offset, size = struct.unpack_from('<IIIIII', d, off)
                link, info, align, entsize = struct.unpack_from('<IIII', d, off+0x18)
            self.sections.append(dict(name_off=name, type=typ, flags=flags, addr=addr,
                                      offset=offset, size=size, link=link, info=info,
                                      align=align, entsize=entsize))
        shstr = self.sections[e_shstrndx]
        strtab = d[shstr['offset']:shstr['offset']+shstr['size']]
        for s in self.sections:
            end = strtab.find(b'\0', s['name_off'])
            s['name'] = strtab[s['name_off']:end].decode('latin1')
    def sec(self, name):
        for s in self.sections:
            if s['name'] == name: return s
        return None
    def read_vaddr(self, addr, n):
        for s in self.sections:
            if s['type'] != 8 and s['addr'] <= addr < s['addr']+s['size']:  # not NOBITS
                off = s['offset'] + (addr - s['addr'])
                return self.data[off:off+n]
        return None

def disasm(path, start=None, stop=None):
    cmd = [OBJDUMP, '-d', '--no-show-raw-insn']
    if start is not None:
        cmd += [f'--start-address={start}', f'--stop-address={stop}']
    cmd.append(path)
    out = subprocess.run(cmd, capture_output=True, text=True).stdout
    rows = []
    sec = None
    for ln in out.split('\n'):
        sm = re.match(r'^Disassembly of section (\S+):', ln)
        if sm:
            sec = sm.group(1)
            continue
        m = re.match(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$', ln)
        if m:
            rows.append((int(m.group(1),16), m.group(2), m.group(3).strip(), sec))
    return rows

def find_tables(path):
    elf = ELF(path)
    rows = disasm(path)
    tables = []
    for i,(addr,mn,ops,sec) in enumerate(rows):
        if mn not in ('tbh','tbb'): continue
        is_tbh = (mn == 'tbh')
        if is_tbh:
            m = re.search(r'\[pc,\s*(r\d+),\s*lsl\s*#1\]', ops)
        else:
            m = re.search(r'\[pc,\s*(r\d+)\]', ops)
        if not m: continue
        reg = m.group(1)
        # look back up to 8 instructions for cmp reg, #N and sub reg, rX, #base
        N = None; base = None; default_addr = None
        for j in range(i-1, max(-1,i-9), -1):
            a2,mn2,ops2,sec2 = rows[j]
            if mn2 == 'cmp':
                mm = re.match(rf'{reg},\s*#(\d+)', ops2)
                if mm: N = int(mm.group(1)); continue
            if mn2 in ('sub','sub.w'):
                mm = re.match(rf'{reg},\s*r\d+,\s*#(\d+)', ops2)
                if mm: base = int(mm.group(1)); continue
        if N is None or base is None: continue
        tbl = addr + 4  # TBH/TBB base is PC (instr+4), NOT aligned
        # For objects, all function sections have addr 0; read from the
        # section that contains the tbh.
        srec = elf.sec(sec) if sec else None
        def rd(off, n):
            if srec is not None:
                if srec['addr'] <= off < srec['addr']+srec['size']:
                    o = srec['offset'] + (off - srec['addr'])
                    return elf.data[o:o+n]
                if srec['addr'] == 0:
                    o = srec['offset'] + off
                    return elf.data[o:o+n]
            return elf.read_vaddr(off, n)
        if is_tbh:
            raw = rd(tbl, (N+1)*2)
            if raw is None or len(raw) < (N+1)*2: continue
            entries = struct.unpack('<%dh' % (N+1), raw)
        else:
            raw = rd(tbl, (N+1))
            if raw is None or len(raw) < (N+1): continue
            entries = struct.unpack('<%dB' % (N+1), raw)
        targets = [tbl + 2*e for e in entries]
        # physical order: group by target, sorted by address
        order = sorted(range(N+1), key=lambda k: (targets[k], k))
        vals = [base+k for k in order]
        tables.append(dict(tbh=addr, kind=mn, reg=reg, base=base, N=N, tbl=tbl,
                           targets=targets, phys_vals=vals))
    return elf, tables

def main():
    path = sys.argv[1]
    elf, tables = find_tables(path)
    print(f"{len(tables)} tbh tables")
    for t in tables:
        vals = t['phys_vals']
        numeric = vals == sorted(vals)
        print(f"tbh {t['tbh']:#x} base={t['base']} N={t['N']} numeric={numeric}")
        if not numeric:
            print(f"   phys: {vals}")

if __name__ == '__main__':
    main()
