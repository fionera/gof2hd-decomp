# Level::stealFriendCargo

ELF vaddr 0xc625c, body 8 bytes
Signature: undefined __thiscall stealFriendCargo(Level * this)

## Ghidra decompile
```c

/* Level::stealFriendCargo() */

void __thiscall Level::stealFriendCargo(Level *this)

{
  this[0x13c] = (Level)0x1;
  return;
}

```

## Target disassembly (must match)
```
  000d625c: movs r1,#0x1
  000d625e: strb.w r1,[r0,#0x13c]
  000d6262: bx lr
```

Verify: tools/try.sh work/src/stealFriendCargo.cpp stealFriendCargo c625c 32
