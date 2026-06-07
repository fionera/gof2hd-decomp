# PlayerWormHole::open
elf 0xa5314 body 10
Sig: int __thiscall open(PlayerWormHole * this, char * __file, int __oflag, ...)

## decompile
```c

/* PlayerWormHole::open() */

int __thiscall PlayerWormHole::open(PlayerWormHole *this,char *__file,int __oflag,...)

{
  *(undefined4 *)(this + 0x150) = DAT_000b5320;
  *(undefined4 *)(this + 0x154) = 0;
  return (int)this;
}

```

## target disasm
```
  000b5314: ldr r1,[0x000b5320]
  000b5316: movs r2,#0x0
  000b5318: strd r1,r2,[r0,#0x150]
  000b531c: bx lr
```
