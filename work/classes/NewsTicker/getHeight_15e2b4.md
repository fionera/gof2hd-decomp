# NewsTicker::getHeight
elf 0x15e2b4 body 28
Sig: undefined __thiscall getHeight(NewsTicker * this)

## decompile
```c

/* NewsTicker::getHeight() */

int __thiscall NewsTicker::getHeight(NewsTicker *this)

{
  return ((**(int **)(DAT_0016e2d4 + 0x16e2c0) + 2) -
         *(int *)(**(int **)(DAT_0016e2d0 + 0x16e2bc) + 0x10)) - *(int *)(this + 8);
}

```

## target disasm
```
  0016e2b4: ldr r1,[0x0016e2d0]
  0016e2b6: ldr r2,[0x0016e2d4]
  0016e2b8: add r1,pc
  0016e2ba: ldr r0,[r0,#0x8]
  0016e2bc: add r2,pc
  0016e2be: ldr r1,[r1,#0x0]
  0016e2c0: ldr r2,[r2,#0x0]
  0016e2c2: ldr r1,[r1,#0x0]
  0016e2c4: ldr r2,[r2,#0x0]
  0016e2c6: ldr r1,[r1,#0x10]
  0016e2c8: adds r2,#0x2
  0016e2ca: subs r1,r2,r1
  0016e2cc: subs r0,r1,r0
  0016e2ce: bx lr
```
