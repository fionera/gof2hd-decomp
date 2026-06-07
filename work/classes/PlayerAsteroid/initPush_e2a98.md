# PlayerAsteroid::initPush
elf 0xe2a98 body 300
Sig: undefined __thiscall initPush(PlayerAsteroid * this, Vector * param_1, int param_2)

## decompile
```c

/* PlayerAsteroid::initPush(AbyssEngine::AEMath::Vector const&, int) */

void __thiscall PlayerAsteroid::initPush(PlayerAsteroid *this,Vector *param_1,int param_2)

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
  piVar4 = *(int **)(DAT_000f2bcc + 0xf2ab0);
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
  iVar6 = (int)((1.0 - fVar8) * DAT_000f2bc4);
  *(int *)(this + 0x104) = iVar6;
  *(int *)(this + 0x108) = iVar6;
  (**(code **)(*(int *)this + 0x28))(&local_58,this);
  AbyssEngine::AEMath::operator-((Vector *)auStack_4c,(Vector *)&local_58);
  AbyssEngine::AEMath::VectorNormalize((AEMath *)aVStack_40,(Vector *)auStack_4c);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x10c),aVStack_40);
  pcVar2 = *(code **)(DAT_000f2bd0 + 0xf2b48);
  puVar3 = *(undefined4 **)(DAT_000f2bd4 + 0xf2b4a);
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
  000f2a98: push {r4,r5,r6,r7,lr}
  000f2a9a: add r7,sp,#0xc
  000f2a9c: push {r8,r9,r10}
  000f2aa0: vpush {d3,d4,d5,d6,d7,d8,d9}
  000f2aa4: mov r4,r0
  000f2aa6: ldr r0,[0x000f2bcc]
  000f2aa8: mov r9,r2
  000f2aaa: add r5,sp,#0xc
  000f2aac: add r0,pc
  000f2aae: mov r8,r1
  000f2ab0: mov r1,r4
  000f2ab2: ldr.w r10,[r0,#0x0]
  000f2ab6: ldr.w r0,[r10,#0x0]
  000f2aba: str r0,[sp,#0x24]
  000f2abc: ldr r0,[r4,#0x0]
  000f2abe: ldr r2,[r0,#0x28]
  000f2ac0: mov r0,r5
  000f2ac2: blx r2
  000f2ac4: add r6,sp,#0x18
  000f2ac6: mov r1,r8
  000f2ac8: mov r2,r5
  000f2aca: mov r0,r6
  000f2acc: blx 0x0006ec38
  000f2ad0: mov r0,r6
  000f2ad2: blx 0x0006ec44
  000f2ad6: vmov s0,r9
  000f2ada: mov r5,sp
  000f2adc: vmov s2,r0
  000f2ae0: mov r1,r4
  000f2ae2: vcvt.f32.s32 s0,s0
  000f2ae6: vdiv.f32 s0,s2,s0
  000f2aea: vmov.f32 s2,0x3f800000
  000f2aee: vcmpe.f32 s0,s2
  000f2af2: vmrs apsr,fpscr
  000f2af6: vmov.f32 s4,s2
  000f2afa: it mi
  000f2afc: vmov.mi.f32 s4,s0
  000f2b00: vsub.f32 s0,s2,s4
  000f2b04: vldr.32 s2,[pc,#0xbc]
  000f2b08: vmul.f32 s0,s0,s2
  000f2b0c: vcvt.s32.f32 s0,s0
  000f2b10: vstr.32 s0,[r4,#0x104]
  000f2b14: vstr.32 s0,[r4,#0x108]
  000f2b18: ldr r0,[r4,#0x0]
  000f2b1a: ldr r2,[r0,#0x28]
  000f2b1c: mov r0,r5
  000f2b1e: blx r2
  000f2b20: add r6,sp,#0xc
  000f2b22: mov r1,r5
  000f2b24: mov r2,r8
  000f2b26: mov r0,r6
  000f2b28: blx 0x0006ec38
  000f2b2c: add r5,sp,#0x18
  000f2b2e: mov r1,r6
  000f2b30: mov r0,r5
  000f2b32: blx 0x0006ec80
  000f2b36: add.w r0,r4,#0x10c
  000f2b3a: mov r1,r5
  000f2b3c: blx 0x0006eb3c
  000f2b40: ldr r0,[0x000f2bd0]
  000f2b42: ldr r1,[0x000f2bd4]
  000f2b44: add r0,pc
  000f2b46: add r1,pc
  000f2b48: ldr r5,[r0,#0x0]
  000f2b4a: ldr r6,[r1,#0x0]
  000f2b4c: movs r1,#0xc8
  000f2b4e: ldr r0,[r6,#0x0]
  000f2b50: blx r5
  000f2b52: sub.w r1,r0,#0x64
  000f2b56: ldr r0,[r6,#0x0]
  000f2b58: vmov s0,r1
  000f2b5c: movs r1,#0xc8
  000f2b5e: vcvt.f32.s32 s16,s0
  000f2b62: blx r5
  000f2b64: sub.w r1,r0,#0x64
  000f2b68: ldr r0,[r6,#0x0]
  000f2b6a: vmov s0,r1
  000f2b6e: movs r1,#0xc8
  000f2b70: vcvt.f32.s32 s18,s0
  000f2b74: blx r5
  000f2b76: subs r0,#0x64
  000f2b78: add r5,sp,#0xc
  000f2b7a: mov r1,sp
  000f2b7c: vmov s0,r0
  000f2b80: mov r0,r5
  000f2b82: vcvt.f32.s32 s0,s0
  000f2b86: vstr.32 s16,[sp]
  000f2b8a: vstr.32 s18,[sp,#0x4]
  000f2b8e: vstr.32 s0,[sp,#0x8]
  000f2b92: blx 0x0006ec80
  000f2b96: add r6,sp,#0x18
  000f2b98: ldr r1,[0x000f2bc8]
  000f2b9a: mov r2,r5
  000f2b9c: mov r0,r6
  000f2b9e: blx 0x00072658
  000f2ba2: add.w r0,r4,#0x118
  000f2ba6: mov r1,r6
  000f2ba8: blx 0x0006eb3c
  000f2bac: ldr r0,[sp,#0x24]
  000f2bae: ldr.w r1,[r10,#0x0]
  000f2bb2: subs r0,r1,r0
  000f2bb4: ittt eq
  000f2bb6: vpop.eq {d3,d4,d5,d6,d7,d8,d9}
  000f2bba: pop.eq.w {r8,r9,r10}
  000f2bbe: pop.eq {r4,r5,r6,r7,pc}
  000f2bc0: blx 0x0006e824
```
