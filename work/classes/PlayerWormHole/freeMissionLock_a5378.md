# PlayerWormHole::freeMissionLock
elf 0xa5378 body 8
Sig: undefined __thiscall freeMissionLock(PlayerWormHole * this)

## decompile
```c

/* PlayerWormHole::freeMissionLock() */

void __thiscall PlayerWormHole::freeMissionLock(PlayerWormHole *this)

{
  this[0x15c] = (PlayerWormHole)0x0;
  return;
}

```

## target disasm
```
  000b5378: movs r1,#0x0
  000b537a: strb.w r1,[r0,#0x15c]
  000b537e: bx lr
```
