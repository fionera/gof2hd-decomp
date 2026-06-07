# PlayerEgo::hackingWon
elf 0xa1118 body 16
Sig: undefined __thiscall hackingWon(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::hackingWon() */

undefined4 __thiscall PlayerEgo::hackingWon(PlayerEgo *this)

{
  undefined4 uVar1;
  
  if (this[0xd].field_14 == (int *)0x0) {
    return 0;
  }
  uVar1 = (*(code *)(DAT_001abb54 + 0x1abb58))();
  return uVar1;
}

```

## target disasm
```
  000b1118: ldr.w r0,[r0,#0x1e8]
  000b111c: cbz r0,0x000b1122
  000b111e: b.w 0x001abb48
  000b1122: movs r0,#0x0
  000b1124: bx lr
  001abb48: bx pc
```
