# PlayerFixedObject::setWreckedMeshId
elf 0x15407c body 156
Sig: undefined __thiscall setWreckedMeshId(PlayerFixedObject * this, int param_1)

## decompile
```c

/* PlayerFixedObject::setWreckedMeshId(int) */

void __thiscall PlayerFixedObject::setWreckedMeshId(PlayerFixedObject *this,int param_1)

{
  AEGeometry *pAVar1;
  int iVar2;
  undefined4 uVar3;
  uint *puVar4;
  
  *(ushort *)(this + 0x184) = (ushort)param_1;
  pAVar1 = operator_new(0xc0);
  puVar4 = *(uint **)(DAT_0016412c + 0x16409a);
  AEGeometry::AEGeometry(pAVar1,(ushort)param_1,(PaintCanvas *)*puVar4,true);
  *(AEGeometry **)(this + 0x124) = pAVar1;
  iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar4);
  *(undefined4 *)(iVar2 + 0xe0) = DAT_00164128;
  iVar2 = *(int *)(this + 0xac);
  if (iVar2 == 0xd) {
    pAVar1 = (AEGeometry *)&DAT_00000004;
  }
  else if (iVar2 == 0xe) {
    pAVar1 = (AEGeometry *)0x0;
  }
  else if (iVar2 == 0xf) {
    if (*(int *)(this + 0x28) == 3) {
      pAVar1 = (AEGeometry *)0x1;
    }
    else if (*(int *)(this + 0x28) == 2) {
      pAVar1 = (AEGeometry *)0x2;
    }
    else {
      pAVar1 = (AEGeometry *)0x3;
    }
  }
  else {
    if (iVar2 != 0x37a3) {
      pAVar1 = *(AEGeometry **)(this + 0x194);
      if ((int)pAVar1 < 0) {
        return;
      }
      goto LAB_001640fe;
    }
    pAVar1 = (AEGeometry *)&DAT_00000005;
  }
  *(AEGeometry **)(this + 0x194) = pAVar1;
LAB_001640fe:
  uVar3 = Globals::getWreckCollision(**(int **)(DAT_00164130 + 0x164108),pAVar1);
  *(undefined4 *)(this + 300) = uVar3;
  return;
}

```

## target disasm
```
  0016407c: push {r4,r5,r6,r7,lr}
  0016407e: add r7,sp,#0xc
  00164080: push.w r8
  00164084: mov r8,r0
  00164086: strh.w r1,[r0,#0x184]
  0016408a: movs r0,#0xc0
  0016408c: mov r6,r1
  0016408e: blx 0x0006eb24
  00164092: mov r5,r0
  00164094: ldr r0,[0x0016412c]
  00164096: add r0,pc
  00164098: ldr r4,[r0,#0x0]
  0016409a: ldr r2,[r4,#0x0]
  0016409c: uxth r1,r6
  0016409e: mov r0,r5
  001640a0: movs r3,#0x1
  001640a2: blx 0x0007207c
  001640a6: str.w r5,[r8,#0x124]
  001640aa: ldr r1,[r5,#0xc]
  001640ac: ldr r0,[r4,#0x0]
  001640ae: blx 0x00072088
  001640b2: ldr r1,[0x00164128]
  001640b4: str.w r1,[r0,#0xe0]
  001640b8: ldr.w r0,[r8,#0xac]
  001640bc: cmp r0,#0xd
  001640be: beq 0x001640d4
  001640c0: cmp r0,#0xe
  001640c2: beq 0x001640d8
  001640c4: cmp r0,#0xf
  001640c6: beq 0x001640dc
  001640c8: movw r1,#0x37a3
  001640cc: cmp r0,r1
  001640ce: bne 0x001640e8
  001640d0: movs r1,#0x5
  001640d2: b 0x001640fa
  001640d4: movs r1,#0x4
  001640d6: b 0x001640fa
  001640d8: movs r1,#0x0
  001640da: b 0x001640fa
  001640dc: ldr.w r0,[r8,#0x28]
  001640e0: cmp r0,#0x3
  001640e2: bne 0x001640f2
  001640e4: movs r1,#0x1
  001640e6: b 0x001640fa
  001640e8: ldr.w r1,[r8,#0x194]
  001640ec: cmp r1,#0x0
  001640ee: bge 0x001640fe
  001640f0: b 0x00164112
  001640f2: cmp r0,#0x2
  001640f4: ite ne
  001640f6: mov.ne r1,#0x3
  001640f8: mov.eq r1,#0x2
  001640fa: str.w r1,[r8,#0x194]
  001640fe: ldr r0,[0x00164130]
  00164100: ldr.w r2,[r8,#0x124]
  00164104: add r0,pc
  00164106: ldr r0,[r0,#0x0]
  00164108: ldr r0,[r0,#0x0]
  0016410a: blx 0x00077404
  0016410e: str.w r0,[r8,#0x12c]
  00164112: pop.w r8
  00164116: pop {r4,r5,r6,r7,pc}
```
