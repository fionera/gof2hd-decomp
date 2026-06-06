# Level::removeObjectives

ELF vaddr 0xc626a, body 8 bytes
Signature: undefined __thiscall removeObjectives(Level * this)

## Ghidra decompile
```c

/* Level::removeObjectives() */

void __thiscall Level::removeObjectives(Level *this)

{
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  return;
}

```

## Target disassembly (must match)
```
  000d626a: movs r1,#0x0
  000d626c: strd r1,r1,[r0,#0x28]
  000d6270: bx lr
```

Verify: tools/try.sh work/src/removeObjectives.cpp removeObjectives c626a 32
