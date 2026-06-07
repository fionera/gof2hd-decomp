# PlayerWormHole::reset
elf 0xa52ec body 18
Sig: undefined __thiscall reset(PlayerWormHole * this, bool param_1)

## decompile
```c

/* PlayerWormHole::reset(bool) */

void __thiscall PlayerWormHole::reset(PlayerWormHole *this,bool param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (param_1) {
    uVar1 = 59000;
  }
  *(undefined4 *)(this + 0x150) = uVar1;
  *(undefined4 *)(this + 0x154) = 0x1000;
  return;
}

```

## target disasm
```
  000b52ec: cmp r1,#0x0
  000b52ee: mov.w r2,#0x1000
  000b52f2: it ne
  000b52f4: movw.ne r1,#0xe678
  000b52f8: strd r1,r2,[r0,#0x150]
  000b52fc: bx lr
```
