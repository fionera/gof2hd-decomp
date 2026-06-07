# Gun::setPlayerGun
elf 0x15241c body 6
Sig: undefined __thiscall setPlayerGun(Gun * this, bool param_1)

## decompile
```c

/* Gun::setPlayerGun(bool) */

void __thiscall Gun::setPlayerGun(Gun *this,bool param_1)

{
  this[0xf0] = (Gun)param_1;
  return;
}

```

## target disasm
```
  0016241c: strb.w r1,[r0,#0xf0]
  00162420: bx lr
```
