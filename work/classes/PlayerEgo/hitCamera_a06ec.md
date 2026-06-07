# PlayerEgo::hitCamera
elf 0xa06ec body 24
Sig: undefined __thiscall hitCamera(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::hitCamera() */

void __thiscall PlayerEgo::hitCamera(PlayerEgo *this)

{
  int iVar1;
  
  *(undefined1 *)&this[0x16].field_14 = 1;
  iVar1 = this[3].field_1C;
  this[0x16].field_10 = (int *)0x0;
  (*(code *)(DAT_001abaf4 + 0x1abaf8))(iVar1);
  return;
}

```

## target disasm
```
  000b06ec: movs r1,#0x1
  000b06ee: movs r2,#0x0
  000b06f0: strb.w r1,[r0,#0x32c]
  000b06f4: ldr.w r1,[r0,#0x88]
  000b06f8: str.w r2,[r0,#0x328]
  000b06fc: mov r0,r1
  000b06fe: b.w 0x001abae8
  001abae8: bx pc
```
