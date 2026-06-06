# Ship::setEquipment
elf 0x175204 body 108
Sig: undefined __stdcall setEquipment(Item * param_1, int param_2)

## decompile
```c

/* Ship::setEquipment(Item*, int) */

void Ship::setEquipment(Item *param_1,int param_2)

{
  int iVar1;
  Item *this;
  void *pvVar2;
  undefined4 *puVar3;
  uint uVar4;
  uint in_r2;
  int iVar5;
  
  for (iVar5 = 0; iVar1 = Item::getType((Item *)param_2), iVar5 < iVar1; iVar5 = iVar5 + 1) {
    in_r2 = in_r2 + *(int *)(*(int *)(param_1 + 0x68) + iVar5 * 4);
  }
  if (**(uint **)(param_1 + 0x6c) <= in_r2) {
    return;
  }
  uVar4 = (*(uint **)(param_1 + 0x6c))[1];
  this = *(Item **)(uVar4 + in_r2 * 4);
  if (this == (Item *)0x0) {
    puVar3 = (undefined4 *)(uVar4 + in_r2 * 4);
  }
  else {
    pvVar2 = (void *)Item::~Item(this);
    operator_delete(pvVar2);
    puVar3 = (undefined4 *)(*(int *)(*(int *)(param_1 + 0x6c) + 4) + in_r2 * 4);
  }
  *puVar3 = 0;
  *(int *)(*(int *)(*(int *)(param_1 + 0x6c) + 4) + in_r2 * 4) = param_2;
  (*(code *)(DAT_001abf14 + 0x1abf18))(param_1);
  return;
}

```
## target disasm
```
  00185204: push {r4,r5,r6,r7,lr}
  00185206: add r7,sp,#0xc
  00185208: push.w r8
  0018520c: mov r4,r2
  0018520e: mov r8,r1
  00185210: mov r5,r0
  00185212: movs r6,#0x0
  00185214: b 0x00185220
  00185216: ldr r0,[r5,#0x68]
  00185218: ldr.w r0,[r0,r6,lsl #0x2]
  0018521c: adds r6,#0x1
  0018521e: add r4,r0
  00185220: mov r0,r8
  00185222: blx 0x000718fc
  00185226: cmp r6,r0
  00185228: blt 0x00185216
  0018522a: ldr r0,[r5,#0x6c]
  0018522c: ldr r1,[r0,#0x0]
  0018522e: cmp r4,r1
  00185230: bcs 0x0018524c
  00185232: ldr r1,[r0,#0x4]
  00185234: ldr.w r0,[r1,r4,lsl #0x2]
  00185238: cbz r0,0x00185252
  0018523a: blx 0x00073474
  0018523e: blx 0x0006eb48
  00185242: ldr r0,[r5,#0x6c]
  00185244: ldr r0,[r0,#0x4]
  00185246: add.w r0,r0,r4, lsl #0x2
  0018524a: b 0x00185256
  0018524c: pop.w r8
  00185250: pop {r4,r5,r6,r7,pc}
  00185252: add.w r0,r1,r4, lsl #0x2
  00185256: movs r1,#0x0
  00185258: str r1,[r0,#0x0]
  0018525a: ldr r0,[r5,#0x6c]
  0018525c: ldr r0,[r0,#0x4]
  0018525e: str.w r8,[r0,r4,lsl #0x2]
  00185262: mov r0,r5
  00185264: pop.w r8
  00185268: pop.w {r4,r5,r6,r7,lr}
  0018526c: b.w 0x001abf08
```
