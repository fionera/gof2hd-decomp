# Status::addPendingProduct
elf 0xa8ef4 body 144
Sig: undefined __thiscall addPendingProduct(Status * this, BluePrint * param_1)

## decompile
```c

/* Status::addPendingProduct(BluePrint*) */

void __thiscall Status::addPendingProduct(Status *this,BluePrint *param_1)

{
  uint *puVar1;
  int iVar2;
  Array<PendingProduct*> *this_00;
  PendingProduct *this_01;
  int iVar3;
  uint uVar4;
  
  puVar1 = *(uint **)(this + 0x1c);
  if (puVar1 == (uint *)0x0) {
    this_00 = operator_new(0xc);
    Array<PendingProduct*>::Array(this_00);
    *(Array<PendingProduct*> **)(this + 0x1c) = this_00;
  }
  else {
    for (uVar4 = 0; uVar4 < *puVar1; uVar4 = uVar4 + 1) {
      iVar2 = *(int *)(puVar1[1] + uVar4 * 4);
      if (((iVar2 != 0) &&
          (iVar3 = *(int *)(iVar2 + 0x14), iVar2 = BluePrint::getIndex(param_1), iVar3 == iVar2)) &&
         (iVar3 = *(int *)(*(int *)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar4 * 4) + 0xc),
         iVar2 = BluePrint::getStationIndex(param_1), iVar3 == iVar2)) {
        iVar2 = BluePrint::getQuantity(param_1);
        iVar3 = *(int *)(*(int *)(*(int *)(this + 0x1c) + 4) + uVar4 * 4);
        *(int *)(iVar3 + 0x10) = iVar2 + *(int *)(iVar3 + 0x10);
        return;
      }
      puVar1 = *(uint **)(this + 0x1c);
    }
  }
  this_01 = operator_new(0x18);
  PendingProduct::PendingProduct(this_01,param_1);
  (*(code *)(DAT_001abf04 + 0x1abf08))(this_01,*(undefined4 *)(this + 0x1c));
  return;
}

```
## target disasm
```
  000b8ef4: push {r4,r5,r6,r7,lr}
  000b8ef6: add r7,sp,#0xc
  000b8ef8: push.w r8
  000b8efc: mov r4,r0
  000b8efe: ldr r0,[r0,#0x1c]
  000b8f00: mov r8,r1
  000b8f02: cbz r0,0x000b8f3c
  000b8f04: movs r6,#0x0
  000b8f06: b 0x000b8f34
  000b8f08: ldr r0,[r0,#0x4]
  000b8f0a: ldr.w r0,[r0,r6,lsl #0x2]
  000b8f0e: cbz r0,0x000b8f30
  000b8f10: ldr r5,[r0,#0x14]
  000b8f12: mov r0,r8
  000b8f14: blx 0x000733fc
  000b8f18: cmp r5,r0
  000b8f1a: bne 0x000b8f30
  000b8f1c: ldr r0,[r4,#0x1c]
  000b8f1e: ldr r0,[r0,#0x4]
  000b8f20: ldr.w r0,[r0,r6,lsl #0x2]
  000b8f24: ldr r5,[r0,#0xc]
  000b8f26: mov r0,r8
  000b8f28: blx 0x00073624
  000b8f2c: cmp r5,r0
  000b8f2e: beq 0x000b8f68
  000b8f30: ldr r0,[r4,#0x1c]
  000b8f32: adds r6,#0x1
  000b8f34: ldr r1,[r0,#0x0]
  000b8f36: cmp r6,r1
  000b8f38: bcc 0x000b8f08
  000b8f3a: b 0x000b8f4a
  000b8f3c: movs r0,#0xc
  000b8f3e: blx 0x0006eb24
  000b8f42: mov r6,r0
  000b8f44: blx 0x00073630
  000b8f48: str r6,[r4,#0x1c]
  000b8f4a: movs r0,#0x18
  000b8f4c: blx 0x0006eb24
  000b8f50: mov r6,r0
  000b8f52: mov r1,r8
  000b8f54: blx 0x0007363c
  000b8f58: ldr r1,[r4,#0x1c]
  000b8f5a: mov r0,r6
  000b8f5c: pop.w r8
  000b8f60: pop.w {r4,r5,r6,r7,lr}
  000b8f64: b.w 0x001abef8
  000b8f68: mov r0,r8
  000b8f6a: blx 0x00073654
  000b8f6e: ldr r1,[r4,#0x1c]
  000b8f70: ldr r1,[r1,#0x4]
  000b8f72: ldr.w r1,[r1,r6,lsl #0x2]
  000b8f76: ldr r2,[r1,#0x10]
  000b8f78: add r0,r2
  000b8f7a: str r0,[r1,#0x10]
  000b8f7c: pop.w r8
  000b8f80: pop {r4,r5,r6,r7,pc}
  001abef8: bx pc
```
