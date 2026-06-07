# Gun::setLevelCollision
elf 0x1522dc body 6
Sig: undefined __thiscall setLevelCollision(Gun * this, bool param_1)

## decompile
```c

/* Gun::setLevelCollision(bool) */

void __thiscall Gun::setLevelCollision(Gun *this,bool param_1)

{
  this[0xf8] = (Gun)param_1;
  return;
}

```

## target disasm
```
  001622dc: strb.w r1,[r0,#0xf8]
  001622e0: bx lr
```
