# KIPlayer::getNearestNavigationPoint
elf 0xa5b4c body 256
Sig: undefined __stdcall getNearestNavigationPoint(Vector * param_1, SpacePoint * param_2)

## decompile
```c

/* KIPlayer::getNearestNavigationPoint(AbyssEngine::AEMath::Vector const&, SpacePoint*) */

void KIPlayer::getNearestNavigationPoint(Vector *param_1,SpacePoint *param_2)

{
  Vector *pVVar1;
  float fVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int in_r2;
  uint uVar8;
  ulonglong in_d1;
  float fVar9;
  float fVar10;
  undefined8 local_70;
  Vector aVStack_60 [12];
  Vector aVStack_54 [12];
  Vector aVStack_48 [12];
  int local_3c;
  
  piVar5 = *(int **)(DAT_000b5c50 + 0xb5b64);
  local_3c = *piVar5;
  local_70 = in_d1;
  if (*(int *)(param_1 + 0xcc) == 0) {
    uVar4 = 0;
  }
  else {
    (**(code **)(*(int *)param_1 + 0x28))(aVStack_48,param_1);
    local_70 = local_70 & 0xffffffff00000000;
    fVar9 = DAT_000b5c4c;
    for (uVar8 = 0; uVar8 < **(uint **)(param_1 + 0xcc); uVar8 = uVar8 + 1) {
      if (*(int *)(*(int *)((*(uint **)(param_1 + 0xcc))[1] + uVar8 * 4) + 0x18) == 1) {
        pVVar1 = (Vector *)AEGeometry::getMatrix();
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)((int)&local_70 + 4),pVVar1);
        AbyssEngine::AEMath::operator+(aVStack_60,aVStack_48);
        AbyssEngine::AEMath::operator-(aVStack_54,aVStack_60);
        fVar2 = (float)AbyssEngine::AEMath::VectorLength(aVStack_54);
        fVar10 = -fVar2;
        if (0.0 < fVar2) {
          fVar10 = fVar2;
        }
        if ((int)((uint)(fVar10 < fVar9) << 0x1f) < 0) {
          iVar3 = SpacePoint::isFree();
          iVar6 = *(int *)(*(int *)(*(int *)(param_1 + 0xcc) + 4) + uVar8 * 4);
          iVar7 = iVar6;
          if ((iVar3 != 0) || (iVar7 = in_r2, iVar6 == in_r2)) {
            local_70 = CONCAT44(local_70._4_4_,iVar7);
            fVar9 = fVar10;
          }
        }
      }
    }
    uVar4 = (undefined4)local_70;
  }
  if (*piVar5 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}

```

## target disasm
```
  000b5b4c: push {r4,r5,r6,r7,lr}
  000b5b4e: add r7,sp,#0xc
  000b5b50: push {r7,r8,r9,r10,r11}
  000b5b54: vpush {d0,d1,d2,d3,d4,d5,d6,d7,d8,d9}
  000b5b58: str r2,[sp,#0x4]
  000b5b5a: mov r6,r0
  000b5b5c: ldr r0,[0x000b5c50]
  000b5b5e: mov r11,r1
  000b5b60: add r0,pc
  000b5b62: ldr r1,[r0,#0x0]
  000b5b64: ldr r0,[r1,#0x0]
  000b5b66: str r0,[sp,#0x3c]
  000b5b68: ldr.w r0,[r6,#0xcc]
  000b5b6c: cmp r0,#0x0
  000b5b6e: beq 0x000b5c32
  000b5b70: ldr r0,[r6,#0x0]
  000b5b72: add.w r10,sp,#0x30
  000b5b76: str r1,[sp,#0x0]
  000b5b78: mov r1,r6
  000b5b7a: ldr r2,[r0,#0x28]
  000b5b7c: mov r0,r10
  000b5b7e: blx r2
  000b5b80: vldr.32 s16,[pc,#0xc8]
  000b5b84: add r4,sp,#0xc
  000b5b86: add r5,sp,#0x18
  000b5b88: add.w r8,sp,#0x24
  000b5b8c: movs r0,#0x0
  000b5b8e: mov.w r9,#0x0
  000b5b92: str r0,[sp,#0x8]
  000b5b94: b 0x000b5c22
  000b5b96: ldr r0,[r0,#0x4]
  000b5b98: ldr.w r0,[r0,r9,lsl #0x2]
  000b5b9c: ldr r0,[r0,#0x18]
  000b5b9e: cmp r0,#0x1
  000b5ba0: bne 0x000b5c1e
  000b5ba2: ldr r0,[r6,#0x8]
  000b5ba4: blx 0x000721cc
  000b5ba8: mov r1,r0
  000b5baa: ldr.w r0,[r6,#0xcc]
  000b5bae: ldr r0,[r0,#0x4]
  000b5bb0: ldr.w r2,[r0,r9,lsl #0x2]
  000b5bb4: mov r0,r4
  000b5bb6: blx 0x0006f694
  000b5bba: mov r0,r5
  000b5bbc: mov r1,r10
  000b5bbe: mov r2,r4
  000b5bc0: blx 0x0006f1cc
  000b5bc4: mov r0,r8
  000b5bc6: mov r1,r5
  000b5bc8: mov r2,r11
  000b5bca: blx 0x0006ec38
  000b5bce: mov r0,r8
  000b5bd0: blx 0x0006ec44
  000b5bd4: vmov s0,r0
  000b5bd8: vcmpe.f32 s0,#0
  000b5bdc: vmrs apsr,fpscr
  000b5be0: vneg.f32 s18,s0
  000b5be4: it gt
  000b5be6: vmov.gt.f32 s18,s0
  000b5bea: vcmpe.f32 s18,s16
  000b5bee: vmrs apsr,fpscr
  000b5bf2: bpl 0x000b5c1e
  000b5bf4: ldr.w r0,[r6,#0xcc]
  000b5bf8: ldr r0,[r0,#0x4]
  000b5bfa: ldr.w r0,[r0,r9,lsl #0x2]
  000b5bfe: blx 0x000732c4
  000b5c02: ldr.w r1,[r6,#0xcc]
  000b5c06: cmp r0,#0x0
  000b5c08: ldr r1,[r1,#0x4]
  000b5c0a: ldr.w r1,[r1,r9,lsl #0x2]
  000b5c0e: bne 0x000b5c18
  000b5c10: ldr r0,[sp,#0x4]
  000b5c12: cmp r1,r0
  000b5c14: mov r1,r0
  000b5c16: bne 0x000b5c1e
  000b5c18: vmov.f32 s16,s18
  000b5c1c: str r1,[sp,#0x8]
  000b5c1e: add.w r9,r9,#0x1
  000b5c22: ldr.w r0,[r6,#0xcc]
  000b5c26: ldr r1,[r0,#0x0]
  000b5c28: cmp r9,r1
  000b5c2a: bcc 0x000b5b96
  000b5c2c: ldr r1,[sp,#0x0]
  000b5c2e: ldr r0,[sp,#0x8]
  000b5c30: b 0x000b5c34
  000b5c32: movs r0,#0x0
  000b5c34: ldr r2,[sp,#0x3c]
  000b5c36: ldr r1,[r1,#0x0]
  000b5c38: subs r1,r1,r2
  000b5c3a: itttt eq
  000b5c3c: vpop.eq {d0,d1,d2,d3,d4,d5,d6,d7,d8,d9}
  000b5c40: add.eq sp,#0x4
  000b5c42: pop.eq.w {r8,r9,r10,r11}
  000b5c46: pop.eq {r4,r5,r6,r7,pc}
  000b5c48: blx 0x0006e824
```
