# Item::isInList
elf 0xe02a8 body 8
Sig: undefined __stdcall isInList(Item * param_1, Array * param_2)

## decompile
```c

/* Item::isInList(Item*, Array<Item*>*) */

void Item::isInList(Item *param_1,Array *param_2)

{
  (*(code *)(DAT_001ac244 + 0x1ac248))(*(undefined4 *)param_1);
  return;
}

```
## target disasm
```
  000f02a8: ldr r0,[r0,#0x0]
  000f02aa: b.w 0x001ac238
  001ac238: bx pc
```
