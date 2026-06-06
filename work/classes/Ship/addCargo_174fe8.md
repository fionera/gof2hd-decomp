# Ship::addCargo
elf 0x174fe8 body 26
Sig: undefined __stdcall addCargo(Array * param_1)

## decompile
```c

/* Ship::addCargo(Array<Item*>*) */

void Ship::addCargo(Array *param_1)

{
  undefined4 uVar1;
  Array *in_r1;
  
  uVar1 = Item::combineItems(*(Array **)(param_1 + 0x70),in_r1);
  (*(code *)(DAT_001ac754 + 0x1ac758))(param_1,uVar1);
  return;
}

```
## target disasm
```
  00184fe8: push {r4,r6,r7,lr}
  00184fea: add r7,sp,#0x8
  00184fec: mov r4,r0
  00184fee: ldr r0,[r0,#0x70]
  00184ff0: blx 0x00075a30
  00184ff4: mov r1,r0
  00184ff6: mov r0,r4
  00184ff8: pop.w {r4,r6,r7,lr}
  00184ffc: b.w 0x001ac748
  001ac748: bx pc
```
