# RepairBeam::RepairBeam
elf 0xa7100 body 254
Sig: undefined __thiscall RepairBeam(RepairBeam * this, int param_1, int param_2)

## decompile
```c

/* RepairBeam::RepairBeam(int, int) */

RepairBeam * __thiscall RepairBeam::RepairBeam(RepairBeam *this,int param_1,int param_2)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  Array<AEGeometry*> *this_00;
  AEGeometry *this_01;
  Array *pAVar4;
  uint *puVar5;
  Array<float> *this_02;
  int *piVar6;
  uint uVar7;
  int iVar8;
  undefined4 *puVar9;
  
  *(int *)(this + 0x1c) = param_2;
  *(int *)this = param_1;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  iVar2 = Status::getShip();
  iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
  uVar3 = Item::getAttribute(iVar2);
  this_00 = operator_new(0xc);
  Array<AEGeometry*>::Array(this_00);
  *(Array<AEGeometry*> **)(this + 0x10) = this_00;
  ArraySetLength<AEGeometry*>(uVar3,(Array *)this_00);
  uVar1 = 0x4a95;
  if (param_2 == 0x25) {
    uVar1 = 0x4a94;
  }
  puVar9 = *(undefined4 **)(DAT_000b7218 + 0xb7164);
  for (iVar2 = 0; iVar2 < (int)uVar3; iVar2 = iVar2 + 1) {
    this_01 = operator_new(0xc0);
    AEGeometry::AEGeometry(this_01,uVar1,(PaintCanvas *)*puVar9,false);
    *(AEGeometry **)(*(int *)(*(int *)(this + 0x10) + 4) + iVar2 * 4) = this_01;
  }
  pAVar4 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(this + 0x14) = pAVar4;
  ArraySetLength<int>(uVar3,pAVar4);
  *(undefined4 *)(this + 0x20) = 0x9c4;
  puVar5 = *(uint **)(this + 0x14);
  for (uVar7 = 0; uVar7 < *puVar5; uVar7 = uVar7 + 1) {
    *(undefined4 *)(puVar5[1] + uVar7 * 4) = 0xffffffff;
  }
  this_02 = operator_new(0xc);
  Array<float>::Array(this_02);
  *(Array<float> **)(this + 0x18) = this_02;
  ArraySetLength<float>(uVar3,(Array *)this_02);
  piVar6 = *(int **)(this + 0x18);
  iVar8 = 0;
  for (iVar2 = *piVar6; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined4 *)(piVar6[1] + iVar8) = 0;
    iVar8 = iVar8 + 4;
  }
  return this;
}

```

## target disasm
```
  000b7100: push {r4,r5,r6,r7,lr}
  000b7102: add r7,sp,#0xc
  000b7104: push {r8,r9,r10}
  000b7108: mov r8,r0
  000b710a: ldr r0,[0x000b7214]
  000b710c: mov r6,r2
  000b710e: movs r2,#0x0
  000b7110: add r0,pc
  000b7112: str.w r6,[r8,#0x1c]
  000b7116: strd r1,r2,[r8,#0x0]
  000b711a: ldr r0,[r0,#0x0]
  000b711c: strd r2,r2,[r8,#0x8]
  000b7120: ldr r0,[r0,#0x0]
  000b7122: blx 0x00071a58
  000b7126: ldr.w r1,[r8,#0x1c]
  000b712a: blx 0x0007228c
  000b712e: movs r1,#0x37
  000b7130: blx 0x000718e4
  000b7134: mov r9,r0
  000b7136: movs r0,#0xc
  000b7138: blx 0x0006eb24
  000b713c: mov r4,r0
  000b713e: blx 0x000717b8
  000b7142: mov r0,r9
  000b7144: mov r1,r4
  000b7146: str.w r4,[r8,#0x10]
  000b714a: blx 0x00073504
  000b714e: movw r0,#0x4a95
  000b7152: cmp r6,#0x25
  000b7154: it eq
  000b7156: movw.eq r0,#0x4a94
  000b715a: ldr r1,[0x000b7218]
  000b715c: movs r5,#0x0
  000b715e: uxth r6,r0
  000b7160: add r1,pc
  000b7162: ldr.w r10,[r1,#0x0]
  000b7166: b 0x000b7188
  000b7168: movs r0,#0xc0
  000b716a: blx 0x0006eb24
  000b716e: mov r4,r0
  000b7170: ldr.w r2,[r10,#0x0]
  000b7174: mov r1,r6
  000b7176: movs r3,#0x0
  000b7178: blx 0x0007207c
  000b717c: ldr.w r0,[r8,#0x10]
  000b7180: ldr r0,[r0,#0x4]
  000b7182: str.w r4,[r0,r5,lsl #0x2]
  000b7186: adds r5,#0x1
  000b7188: cmp r5,r9
  000b718a: blt 0x000b7168
  000b718c: movs r0,#0xc
  000b718e: blx 0x0006eb24
  000b7192: mov r4,r0
  000b7194: blx 0x000701f8
  000b7198: mov r0,r9
  000b719a: mov r1,r4
  000b719c: str.w r4,[r8,#0x14]
  000b71a0: blx 0x00071a4c
  000b71a4: movw r0,#0x9c4
  000b71a8: movs r1,#0x0
  000b71aa: str.w r0,[r8,#0x20]
  000b71ae: mov.w r2,#0xffffffff
  000b71b2: ldr.w r0,[r8,#0x14]
  000b71b6: b 0x000b71c0
  000b71b8: ldr r3,[r0,#0x4]
  000b71ba: str.w r2,[r3,r1,lsl #0x2]
  000b71be: adds r1,#0x1
  000b71c0: ldr r3,[r0,#0x0]
  000b71c2: cmp r1,r3
  000b71c4: bcc 0x000b71b8
  000b71c6: movs r0,#0xc
  000b71c8: blx 0x0006eb24
  000b71cc: mov r4,r0
  000b71ce: blx 0x00073510
  000b71d2: mov r0,r9
  000b71d4: mov r1,r4
  000b71d6: str.w r4,[r8,#0x18]
  000b71da: blx 0x0007351c
  000b71de: ldr.w r0,[r8,#0x18]
  000b71e2: movs r2,#0x0
  000b71e4: movs r3,#0x0
  000b71e6: ldr r1,[r0,#0x0]
  000b71e8: b 0x000b71f2
  000b71ea: ldr r6,[r0,#0x4]
  000b71ec: subs r1,#0x1
  000b71ee: str r2,[r6,r3]
  000b71f0: adds r3,#0x4
  000b71f2: cmp r1,#0x0
  000b71f4: bne 0x000b71ea
  000b71f6: mov r0,r8
  000b71f8: pop.w {r8,r9,r10}
  000b71fc: pop {r4,r5,r6,r7,pc}
```
