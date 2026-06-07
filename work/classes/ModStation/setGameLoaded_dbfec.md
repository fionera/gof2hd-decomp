# ModStation::setGameLoaded
elf 0xdbfec body 12
Sig: undefined __thiscall setGameLoaded(ModStation * this)

## decompile
```c

/* ModStation::setGameLoaded() */

void __thiscall ModStation::setGameLoaded(ModStation *this)

{
  *(undefined1 *)((int)&this[1].field_20 + 3) = 1;
  *(undefined1 *)&this[1].field_20 = 1;
  return;
}

```

## target disasm
```
  000ebfec: movs r1,#0x1
  000ebfee: strb.w r1,[r0,#0xb3]
  000ebff2: strb.w r1,[r0,#0xb0]
  000ebff6: bx lr
```
