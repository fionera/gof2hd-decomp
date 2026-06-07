# PlayerWormHole::isShrinking
elf 0xa5324 body 18
Sig: undefined __thiscall isShrinking(PlayerWormHole * this)

## decompile
```c

/* PlayerWormHole::isShrinking() */

bool __thiscall PlayerWormHole::isShrinking(PlayerWormHole *this)

{
  return 60000 < *(int *)(this + 0x150);
}

```

## target disasm
```
  000b5324: ldr.w r1,[r0,#0x150]
  000b5328: movs r0,#0x0
  000b532a: movw r2,#0xea60
  000b532e: cmp r1,r2
  000b5330: it gt
  000b5332: mov.gt r0,#0x1
  000b5334: bx lr
```
