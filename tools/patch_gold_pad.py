#!/usr/bin/env python3
"""Patch the ARM gold stub-table auto-padding cap.

Android's gold carries a ``stub_group_auto_padding`` patch (gold/arm.cc).  When the stub table
grows by ``delta`` with ``0x500 <= delta < 0x10000``, the padding is::

    padding_ = std::min((unsigned int)(delta * 0.02), (unsigned int)0x40);

The 0x40 cap is a hard-coded immediate -- gold exposes no command-line switch for it.  This script
rewrites the two ``movl $0x40,-0x44(%rbp)`` sites in the NDK r18b ``ld.gold`` (the big- and
little-endian instantiations of ``Stub_table::update_data_size_and_addralign``) to the requested
cap, so a link can reproduce a different tail pad without rebuilding gold.

Usage: patch_gold_pad.py <ld.gold> <hex-cap>
"""
import sys

# movl $0x40,-0x44(%rbp); the immediate is the fourth byte.
PATTERN = bytes.fromhex("c745bc40000000")


def main():
    path, cap = sys.argv[1], int(sys.argv[2], 16)
    if not 0 <= cap <= 0xFF:
        sys.exit(f"cap out of range: {cap:#x}")
    data = bytearray(open(path, "rb").read())
    count = 0
    pos = 0
    while True:
        pos = data.find(PATTERN, pos)
        if pos < 0:
            break
        data[pos + 3] = cap
        count += 1
        pos += len(PATTERN)
    if count != 2:
        sys.exit(f"expected 2 gold padding-cap sites, found {count}")
    open(path, "wb").write(data)
    print(f"patched {count} gold padding-cap sites to {cap:#04x}")


if __name__ == "__main__":
    main()
