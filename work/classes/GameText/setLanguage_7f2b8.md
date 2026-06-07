# GameText::setLanguage
elf 0x7f2b8 body 10
Sig: undefined __thiscall setLanguage(GameText * this, int param_1)

## decompile
```c

/* GameText::setLanguage(int) */

void __thiscall GameText::setLanguage(GameText *this,int param_1)

{
  (*(code *)(DAT_001ab444 + 0x1ab448))(this,0,param_1);
  return;
}

```

## target disasm
```
  0008f2b8: mov r2,r1
  0008f2ba: movs r1,#0x0
  0008f2bc: b.w 0x001ab438
  001ab438: bx pc
```
