# BluePrint::BluePrint
elf 0x176f20 body 158
Sig: undefined __thiscall BluePrint(BluePrint * this, int param_1)

## decompile
```c

/* BluePrint::BluePrint(int) */

BluePrint * __thiscall BluePrint::BluePrint(BluePrint *this,int param_1)

{
  int iVar1;
  int iVar2;
  void *pvVar3;
  uint *puVar4;
  undefined4 uVar5;
  uint uVar6;
  
  AbyssEngine::String::String((String *)(this + 0x14));
  iVar1 = DAT_00186fd8;
  *(int *)(this + 0x20) = param_1;
  iVar1 = Item::getType(*(Item **)(*(int *)(**(int **)(iVar1 + 0x186f3e) + 4) + param_1 * 4));
  *(undefined4 *)(this + 0x10) = 0xffffffff;
  uVar5 = 1;
  if (iVar1 == 1) {
    uVar5 = 10;
  }
  *(undefined4 *)(this + 0x24) = uVar5;
  iVar1 = Item::getQuantities();
  *(undefined4 *)this = 0;
  iVar2 = Item::getIngredients();
  if (iVar2 != 0) {
    pvVar3 = operator_new(0xc);
    Array<int>::Array();
    *(void **)this = pvVar3;
    puVar4 = (uint *)Item::getIngredients();
    ArraySetLength<int>(*puVar4,*(Array **)this);
    puVar4 = *(uint **)this;
    for (uVar6 = 0; uVar6 < *puVar4; uVar6 = uVar6 + 1) {
      *(undefined4 *)(puVar4[1] + uVar6 * 4) = *(undefined4 *)(*(int *)(iVar1 + 4) + uVar6 * 4);
    }
  }
  *(undefined4 *)(this + 0xc) = 0;
  this[8] = (BluePrint)0x0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 0x28) = *(undefined4 *)(this + 0x24);
  return this;
}

```

## target disasm
```
  00186f20: push {r4,r5,r6,r7,lr}
  00186f22: add r7,sp,#0xc
  00186f24: push {r8,r9,r11}
  00186f28: add.w r8,r0,#0x14
  00186f2c: mov r4,r0
  00186f2e: mov r5,r1
  00186f30: mov r0,r8
  00186f32: blx 0x0006efbc
  00186f36: ldr r0,[0x00186fd8]
  00186f38: str r5,[r4,#0x20]
  00186f3a: add r0,pc
  00186f3c: ldr r0,[r0,#0x0]
  00186f3e: ldr r0,[r0,#0x0]
  00186f40: ldr r0,[r0,#0x4]
  00186f42: ldr.w r5,[r0,r5,lsl #0x2]
  00186f46: mov r0,r5
  00186f48: blx 0x000718fc
  00186f4c: mov.w r1,#0xffffffff
  00186f50: cmp r0,#0x1
  00186f52: str r1,[r4,#0x10]
  00186f54: mov.w r1,#0x1
  00186f58: it eq
  00186f5a: mov.eq r1,#0xa
  00186f5c: str r1,[r4,#0x24]
  00186f5e: mov r0,r5
  00186f60: blx 0x0007801c
  00186f64: mov r6,r0
  00186f66: movs r0,#0x0
  00186f68: str r0,[r4,#0x0]
  00186f6a: mov r0,r5
  00186f6c: blx 0x00071914
  00186f70: cbz r0,0x00186faa
  00186f72: movs r0,#0xc
  00186f74: blx 0x0006eb24
  00186f78: mov r9,r0
  00186f7a: blx 0x000701f8
  00186f7e: str.w r9,[r4,#0x0]
  00186f82: mov r0,r5
  00186f84: blx 0x00071914
  00186f88: ldr r1,[r4,#0x0]
  00186f8a: ldr r0,[r0,#0x0]
  00186f8c: blx 0x00071a4c
  00186f90: ldr r0,[r4,#0x0]
  00186f92: movs r2,#0x0
  00186f94: b 0x00186fa4
  00186f96: ldr r1,[r6,#0x4]
  00186f98: ldr r3,[r0,#0x4]
  00186f9a: ldr.w r1,[r1,r2,lsl #0x2]
  00186f9e: str.w r1,[r3,r2,lsl #0x2]
  00186fa2: adds r2,#0x1
  00186fa4: ldr r1,[r0,#0x0]
  00186fa6: cmp r2,r1
  00186fa8: bcc 0x00186f96
  00186faa: ldr r0,[r4,#0x24]
  00186fac: movs r1,#0x0
  00186fae: str r1,[r4,#0xc]
  00186fb0: strb r1,[r4,#0x8]
  00186fb2: str r1,[r4,#0x4]
  00186fb4: str r0,[r4,#0x28]
  00186fb6: mov r0,r4
  00186fb8: pop.w {r8,r9,r11}
  00186fbc: pop {r4,r5,r6,r7,pc}
```
