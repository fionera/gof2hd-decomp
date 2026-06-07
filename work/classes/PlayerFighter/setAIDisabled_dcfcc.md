# PlayerFighter::setAIDisabled
elf 0xdcfcc body 6
Sig: undefined __thiscall setAIDisabled(PlayerFighter * this, bool param_1)

## decompile
```c

/* PlayerFighter::setAIDisabled(bool) */

void __thiscall PlayerFighter::setAIDisabled(PlayerFighter *this,bool param_1)

{
  this[0x2e4] = (PlayerFighter)param_1;
  return;
}

```

## target disasm
```
  000ecfcc: strb.w r1,[r0,#0x2e4]
  000ecfd0: bx lr
```
