# PlayerEgo::getCurrentMiningAmount
elf 0x9af24 body 16
Sig: undefined __thiscall getCurrentMiningAmount(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getCurrentMiningAmount() */

undefined4 __thiscall PlayerEgo::getCurrentMiningAmount(PlayerEgo *this)

{
  undefined4 uVar1;
  
  if (this[0xd].field_10 == (int *)0x0) {
    return 0;
  }
  uVar1 = (*(code *)(DAT_001aba74 + 0x1aba78))();
  return uVar1;
}

```

## target disasm
```
  000aaf24: ldr.w r0,[r0,#0x1e4]
  000aaf28: cbz r0,0x000aaf2e
  000aaf2a: b.w 0x001aba68
  000aaf2e: movs r0,#0x0
  000aaf30: bx lr
  001aba68: bx pc
```
