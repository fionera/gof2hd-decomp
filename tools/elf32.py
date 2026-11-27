#!/usr/bin/env python3
"""Minimal, pure-Python ELF32 (little-endian ARM) reader for validation."""

from __future__ import annotations

import hashlib
import struct
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ELF_MAGIC = b"\x7fELF"
ELFCLASS32 = 1
ELFDATA2LSB = 1

SHT_NULL = 0
SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_NOBITS = 8
SHT_DYNSYM = 11

SHF_ALLOC = 0x2

SHN_UNDEF = 0
SHN_LORESERVE = 0xFF00

STT_NOTYPE = 0
STT_OBJECT = 1
STT_FUNC = 2
STT_SECTION = 3
STT_FILE = 4

STB_LOCAL = 0
STB_GLOBAL = 1
STB_WEAK = 2


@dataclass(frozen=True)
class Section:
    index: int
    name: str
    sh_type: int
    sh_flags: int
    addr: int
    offset: int
    size: int
    link: int
    info: int
    addralign: int
    entsize: int
    data: bytes

    @property
    def is_nobits(self) -> bool:
        return self.sh_type == SHT_NOBITS

    @property
    def is_alloc(self) -> bool:
        return bool(self.sh_flags & SHF_ALLOC)

    @property
    def sha256(self) -> Optional[str]:
        if self.is_nobits:
            return None
        return hashlib.sha256(self.data).hexdigest()


@dataclass(frozen=True)
class Symbol:
    name: str
    value: int
    size: int
    bind: int
    sym_type: int
    other: int
    shndx: int
    section_name: str

    @property
    def is_defined(self) -> bool:
        return self.shndx != SHN_UNDEF

    @property
    def is_thumb(self) -> bool:
        return self.sym_type == STT_FUNC and bool(self.value & 1)

    @property
    def addr(self) -> int:
        return (self.value & ~1) if self.sym_type == STT_FUNC else self.value


def _read_cstr(buf: bytes, offset: int) -> str:
    if offset < 0 or offset >= len(buf):
        return ""
    end = buf.find(b"\x00", offset)
    if end == -1:
        end = len(buf)
    return buf[offset:end].decode("utf-8", errors="replace")


class Elf32:
    """Parsed ELF32 little-endian shared object."""

    def __init__(self, path: Path) -> None:
        self.path = Path(path)
        self.raw = self.path.read_bytes()
        if len(self.raw) < 52 or self.raw[:4] != ELF_MAGIC:
            raise ValueError(f"Not an ELF file: {self.path}")
        if self.raw[4] != ELFCLASS32 or self.raw[5] != ELFDATA2LSB:
            raise ValueError(f"Unsupported ELF class/endianness (need ELF32 LE): {self.path}")

        (
            self.e_type,
            self.e_machine,
            self.e_version,
            self.e_entry,
            self.e_phoff,
            self.e_shoff,
            self.e_flags,
            self.e_ehsize,
            self.e_phentsize,
            self.e_phnum,
            self.e_shentsize,
            self.e_shnum,
            self.e_shstrndx,
        ) = struct.unpack_from("<HHIIIIIHHHHHH", self.raw, 16)

        self.sections: List[Section] = []
        self.sections_by_name: Dict[str, Section] = {}
        self._parse_sections()

        self.dynsyms: List[Symbol] = self._parse_symtab(SHT_DYNSYM)
        self.functions: Dict[str, Symbol] = {}
        self.objects: Dict[str, Symbol] = {}
        self.other_defined: Dict[str, Symbol] = {}
        self.defined: Dict[str, Symbol] = {}
        self.imports: set[str] = set()
        self._index_dynsyms()

    def _parse_sections(self) -> None:
        if self.e_shoff == 0 or self.e_shnum == 0:
            return
        raw_shdrs: List[Tuple[int, ...]] = []
        for idx in range(self.e_shnum):
            off = self.e_shoff + idx * self.e_shentsize
            if off + 40 > len(self.raw):
                raise ValueError(f"Truncated section header {idx} in {self.path}")
            raw_shdrs.append(struct.unpack_from("<IIIIIIIIII", self.raw, off))

        shstrtab = b""
        if 0 <= self.e_shstrndx < len(raw_shdrs):
            _, _, _, _, str_off, str_size, _, _, _, _ = raw_shdrs[self.e_shstrndx]
            shstrtab = self.raw[str_off : str_off + str_size]

        for idx, sh in enumerate(raw_shdrs):
            (
                sh_name,
                sh_type,
                sh_flags,
                sh_addr,
                sh_offset,
                sh_size,
                sh_link,
                sh_info,
                sh_addralign,
                sh_entsize,
            ) = sh
            name = _read_cstr(shstrtab, sh_name)
            data = b"" if sh_type == SHT_NOBITS else self.raw[sh_offset : sh_offset + sh_size]
            sec = Section(
                index=idx,
                name=name,
                sh_type=sh_type,
                sh_flags=sh_flags,
                addr=sh_addr,
                offset=sh_offset,
                size=sh_size,
                link=sh_link,
                info=sh_info,
                addralign=sh_addralign,
                entsize=sh_entsize,
                data=data,
            )
            self.sections.append(sec)
            if name and name not in self.sections_by_name:
                self.sections_by_name[name] = sec

    def _parse_symtab(self, target_type: int) -> List[Symbol]:
        sym_sec = next((s for s in self.sections if s.sh_type == target_type), None)
        if sym_sec is None or sym_sec.size == 0:
            return []
        if not (0 <= sym_sec.link < len(self.sections)):
            return []
        strtab = self.sections[sym_sec.link].data
        entsize = sym_sec.entsize or 16
        count = sym_sec.size // entsize
        symbols: List[Symbol] = []
        for i in range(count):
            off = i * entsize
            if off + 16 > len(sym_sec.data):
                break
            st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(
                "<IIIBBH", sym_sec.data, off
            )
            name = _read_cstr(strtab, st_name)
            if not name:
                continue
            bind = st_info >> 4
            sym_type = st_info & 0xF
            sec_name = (
                self.sections[st_shndx].name
                if 0 < st_shndx < len(self.sections)
                else ""
            )
            symbols.append(
                Symbol(
                    name=name,
                    value=st_value,
                    size=st_size,
                    bind=bind,
                    sym_type=sym_type,
                    other=st_other,
                    shndx=st_shndx,
                    section_name=sec_name,
                )
            )
        return symbols

    def _index_dynsyms(self) -> None:
        for sym in self.dynsyms:
            if sym.sym_type in (STT_SECTION, STT_FILE):
                continue
            if not sym.is_defined:
                self.imports.add(sym.name)
                continue
            self.defined[sym.name] = sym
            if sym.sym_type == STT_FUNC:
                self.functions[sym.name] = sym
            elif sym.sym_type == STT_OBJECT:
                self.objects[sym.name] = sym
            else:
                self.other_defined[sym.name] = sym

    def read_symbol_bytes(self, sym: Symbol) -> bytes:
        """Read raw bytes for a defined symbol from its ELF section."""
        if not sym.is_defined or sym.size == 0:
            return b""
        sec: Optional[Section] = None
        if 0 < sym.shndx < len(self.sections):
            sec = self.sections[sym.shndx]
        if sec is None or sec.is_nobits:
            for cand in self.sections:
                if cand.is_alloc and not cand.is_nobits and cand.addr <= sym.addr < cand.addr + cand.size:
                    sec = cand
                    break
        if sec is None or sec.is_nobits:
            return b""
        rel = sym.addr - sec.addr
        if rel < 0 or rel >= len(sec.data):
            return b""
        return sec.data[rel : rel + sym.size]
