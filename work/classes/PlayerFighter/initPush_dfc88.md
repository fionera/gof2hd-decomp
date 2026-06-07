# PlayerFighter::initPush
elf 0xdfc88 body 300
Sig: undefined __thiscall initPush(PlayerFighter * this, Vector * param_1, int param_2)

## decompile
```c

/* PlayerFighter::initPush(AbyssEngine::AEMath::Vector const&, int) */

void __thiscall PlayerFighter::initPush(PlayerFighter *this,Vector *param_1,int param_2)

{
  float fVar1;
  code *pcVar2;
  undefined4 *puVar3;
  int *piVar4;
  uint in_fpscr;
  float fVar5;
  int iVar6;
  undefined4 uVar7;
  float fVar8;
  undefined8 in_d4;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 auStack_4c [3];
  Vector aVStack_40 [12];
  int local_34;
  
  auStack_4c[0] = (undefined4)((ulonglong)in_d4 >> 0x20);
  piVar4 = *(int **)(DAT_000efdbc + 0xefca0);
  local_34 = *piVar4;
  (**(code **)(*(int *)this + 0x28))(auStack_4c,this);
  AbyssEngine::AEMath::operator-(aVStack_40,param_1);
  fVar1 = (float)AbyssEngine::AEMath::VectorLength(aVStack_40);
  fVar5 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  in_fpscr = in_fpscr & 0xfffffff;
  fVar8 = 1.0;
  if (fVar1 / fVar5 < 1.0) {
    fVar8 = fVar1 / fVar5;
  }
  iVar6 = (int)((1.0 - fVar8) * DAT_000efdb4);
  *(int *)(this + 0x104) = iVar6;
  *(int *)(this + 0x108) = iVar6;
  (**(code **)(*(int *)this + 0x28))(&local_58,this);
  AbyssEngine::AEMath::operator-((Vector *)auStack_4c,(Vector *)&local_58);
  AbyssEngine::AEMath::VectorNormalize((AEMath *)aVStack_40,(Vector *)auStack_4c);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x10c),aVStack_40);
  pcVar2 = *(code **)(DAT_000efdc0 + 0xefd38);
  puVar3 = *(undefined4 **)(DAT_000efdc4 + 0xefd3a);
  iVar6 = (*pcVar2)(*puVar3,200);
  uVar9 = VectorSignedToFloat(iVar6 + -100,(byte)(in_fpscr >> 0x16) & 3);
  iVar6 = (*pcVar2)(*puVar3,200);
  uVar10 = VectorSignedToFloat(iVar6 + -100,(byte)(in_fpscr >> 0x16) & 3);
  iVar6 = (*pcVar2)(*puVar3,200);
  uVar7 = VectorSignedToFloat(iVar6 + -100,(byte)(in_fpscr >> 0x16) & 3);
  _local_58 = CONCAT44(uVar10,uVar9);
  _local_50 = CONCAT44(auStack_4c[0],uVar7);
  fVar8 = (float)AbyssEngine::AEMath::VectorNormalize((AEMath *)auStack_4c,(Vector *)&local_58);
  AbyssEngine::AEMath::operator*(fVar8,aVStack_40);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x118),aVStack_40);
  if (*piVar4 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000efc88: push {r4,r5,r6,r7,lr}
  000efc8a: add r7,sp,#0xc
  000efc8c: push {r8,r9,r10}
  000efc90: vpush {d3,d4,d5,d6,d7,d8,d9}
  000efc94: mov r4,r0
  000efc96: ldr r0,[0x000efdbc]
  000efc98: mov r9,r2
  000efc9a: add r5,sp,#0xc
  000efc9c: add r0,pc
  000efc9e: mov r8,r1
  000efca0: mov r1,r4
  000efca2: ldr.w r10,[r0,#0x0]
  000efca6: ldr.w r0,[r10,#0x0]
  000efcaa: str r0,[sp,#0x24]
  000efcac: ldr r0,[r4,#0x0]
  000efcae: ldr r2,[r0,#0x28]
  000efcb0: mov r0,r5
  000efcb2: blx r2
  000efcb4: add r6,sp,#0x18
  000efcb6: mov r1,r8
  000efcb8: mov r2,r5
  000efcba: mov r0,r6
  000efcbc: blx 0x0006ec38
  000efcc0: mov r0,r6
  000efcc2: blx 0x0006ec44
  000efcc6: vmov s0,r9
  000efcca: mov r5,sp
  000efccc: vmov s2,r0
  000efcd0: mov r1,r4
  000efcd2: vcvt.f32.s32 s0,s0
  000efcd6: vdiv.f32 s0,s2,s0
  000efcda: vmov.f32 s2,0x3f800000
  000efcde: vcmpe.f32 s0,s2
  000efce2: vmrs apsr,fpscr
  000efce6: vmov.f32 s4,s2
  000efcea: it mi
  000efcec: vmov.mi.f32 s4,s0
  000efcf0: vsub.f32 s0,s2,s4
  000efcf4: vldr.32 s2,[pc,#0xbc]
  000efcf8: vmul.f32 s0,s0,s2
  000efcfc: vcvt.s32.f32 s0,s0
  000efd00: vstr.32 s0,[r4,#0x104]
  000efd04: vstr.32 s0,[r4,#0x108]
  000efd08: ldr r0,[r4,#0x0]
  000efd0a: ldr r2,[r0,#0x28]
  000efd0c: mov r0,r5
  000efd0e: blx r2
  000efd10: add r6,sp,#0xc
  000efd12: mov r1,r5
  000efd14: mov r2,r8
  000efd16: mov r0,r6
  000efd18: blx 0x0006ec38
  000efd1c: add r5,sp,#0x18
  000efd1e: mov r1,r6
  000efd20: mov r0,r5
  000efd22: blx 0x0006ec80
  000efd26: add.w r0,r4,#0x10c
  000efd2a: mov r1,r5
  000efd2c: blx 0x0006eb3c
  000efd30: ldr r0,[0x000efdc0]
  000efd32: ldr r1,[0x000efdc4]
  000efd34: add r0,pc
  000efd36: add r1,pc
  000efd38: ldr r5,[r0,#0x0]
  000efd3a: ldr r6,[r1,#0x0]
  000efd3c: movs r1,#0xc8
  000efd3e: ldr r0,[r6,#0x0]
  000efd40: blx r5
  000efd42: sub.w r1,r0,#0x64
  000efd46: ldr r0,[r6,#0x0]
  000efd48: vmov s0,r1
  000efd4c: movs r1,#0xc8
  000efd4e: vcvt.f32.s32 s16,s0
  000efd52: blx r5
  000efd54: sub.w r1,r0,#0x64
  000efd58: ldr r0,[r6,#0x0]
  000efd5a: vmov s0,r1
  000efd5e: movs r1,#0xc8
  000efd60: vcvt.f32.s32 s18,s0
  000efd64: blx r5
  000efd66: subs r0,#0x64
  000efd68: add r5,sp,#0xc
  000efd6a: mov r1,sp
  000efd6c: vmov s0,r0
  000efd70: mov r0,r5
  000efd72: vcvt.f32.s32 s0,s0
  000efd76: vstr.32 s16,[sp]
  000efd7a: vstr.32 s18,[sp,#0x4]
  000efd7e: vstr.32 s0,[sp,#0x8]
  000efd82: blx 0x0006ec80
  000efd86: add r6,sp,#0x18
  000efd88: ldr r1,[0x000efdb8]
  000efd8a: mov r2,r5
  000efd8c: mov r0,r6
  000efd8e: blx 0x00072658
  000efd92: add.w r0,r4,#0x118
  000efd96: mov r1,r6
  000efd98: blx 0x0006eb3c
  000efd9c: ldr r0,[sp,#0x24]
  000efd9e: ldr.w r1,[r10,#0x0]
  000efda2: subs r0,r1,r0
  000efda4: ittt eq
  000efda6: vpop.eq {d3,d4,d5,d6,d7,d8,d9}
  000efdaa: pop.eq.w {r8,r9,r10}
  000efdae: pop.eq {r4,r5,r6,r7,pc}
  000efdb0: blx 0x0006e824
```
