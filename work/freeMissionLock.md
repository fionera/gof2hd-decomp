# PlayerWormHole::freeMissionLock

ELF vaddr 0xa5378, body 8 bytes
Signature: undefined __thiscall freeMissionLock(PlayerWormHole * this)

## Ghidra decompile
```c

/* PlayerWormHole::freeMissionLock() */

void __thiscall PlayerWormHole::freeMissionLock(PlayerWormHole *this)

{
  this[0x15c] = (PlayerWormHole)0x0;
  return;
}

```

## Target disassembly (must match)
```
  000b5378: movs r1,#0x0
  000b537a: strb.w r1,[r0,#0x15c]
  000b537e: bx lr
```

Verify: tools/try.sh work/src/freeMissionLock.cpp freeMissionLock a5378 32
