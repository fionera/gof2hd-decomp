# Item::isInList
elf 0xe02a0 body 10
Sig: undefined __stdcall isInList(int param_1, Array * param_2)

## decompile
```c

/* Item::isInList(int, Array<Item*>*) */

void Item::isInList(int param_1,Array *param_2)

{
  (*(code *)(DAT_001ac234 + 0x1ac238))(param_1,1,param_2);
  return;
}

```
## target disasm
```
  000f02a0: mov r2,r1
  000f02a2: movs r1,#0x1
  000f02a4: b.w 0x001ac228
  001ac228: bx pc
```
