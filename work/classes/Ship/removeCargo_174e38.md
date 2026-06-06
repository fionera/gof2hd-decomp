# Ship::removeCargo
elf 0x174e38 body 44
Sig: undefined __stdcall removeCargo(Item * param_1)

## decompile
```c

/* Ship::removeCargo(Item*) */

void Ship::removeCargo(Item *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  Item *in_r1;
  
  uVar1 = Item::getIndex(in_r1);
  uVar2 = Item::getAmount();
  (*(code *)(DAT_001ac734 + 0x1ac738))(param_1,uVar1,uVar2);
  return;
}

```
## target disasm
```
  00184e38: push {r4,r5,r6,r7,lr}
  00184e3a: add r7,sp,#0xc
  00184e3c: push.w r11
  00184e40: mov r5,r0
  00184e42: mov r0,r1
  00184e44: mov r4,r1
  00184e46: blx 0x000718d8
  00184e4a: mov r6,r0
  00184e4c: mov r0,r4
  00184e4e: blx 0x0007183c
  00184e52: mov r2,r0
  00184e54: mov r0,r5
  00184e56: mov r1,r6
  00184e58: pop.w r11
  00184e5c: pop.w {r4,r5,r6,r7,lr}
  00184e60: b.w 0x001ac728
```
