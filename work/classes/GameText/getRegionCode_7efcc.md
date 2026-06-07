# GameText::getRegionCode
elf 0x7efcc body 16
Sig: undefined __thiscall getRegionCode(GameText * this)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* GameText::getRegionCode() */

void __thiscall GameText::getRegionCode(GameText *this)

{
  AbyssEngine::String::String((String *)this,(char *)(_FUN_0008efdc + 0x8efd8),false);
  return;
}

```

## target disasm
```
  0008efcc: push {r7,lr}
  0008efce: mov r7,sp
  0008efd0: ldr r1,[0x0008efdc]
  0008efd2: movs r2,#0x0
  0008efd4: add r1,pc
  0008efd6: blx 0x0006ee18
  0008efda: pop {r7,pc}
```
