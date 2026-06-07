# ModStation::showMapWindow
elf 0xdc0c8 body 14
Sig: undefined __thiscall showMapWindow(ModStation * this)

## decompile
```c

/* ModStation::showMapWindow() */

void __thiscall ModStation::showMapWindow(ModStation *this)

{
  *(undefined1 *)((int)&this->field_64 + 3) = 1;
  *(undefined1 *)&this->field_64 = 0;
  return;
}

```

## target disasm
```
  000ec0c8: movs r1,#0x1
  000ec0ca: strb.w r1,[r0,#0x67]
  000ec0ce: movs r1,#0x0
  000ec0d0: strb.w r1,[r0,#0x64]
  000ec0d4: bx lr
```
