# KIPlayer::getNearestDockingPoint
elf 0xa5c54 body 226
Sig: undefined __stdcall getNearestDockingPoint(Vector * param_1)

## decompile
```c

/* KIPlayer::getNearestDockingPoint(AbyssEngine::AEMath::Vector const&) */

void KIPlayer::getNearestDockingPoint(Vector *param_1)

{
  Vector *pVVar1;
  float fVar2;
  int *piVar3;
  uint uVar4;
  float fVar5;
  undefined8 in_d1;
  float fVar6;
  undefined8 local_68;
  Vector aVStack_58 [12];
  Vector aVStack_4c [12];
  Vector aVStack_40 [12];
  int local_34;
  
  piVar3 = *(int **)(DAT_000b5d3c + 0xb5c6a);
  local_34 = *piVar3;
  local_68 = in_d1;
  if (*(int *)(param_1 + 0xcc) != 0) {
    local_68 = CONCAT44((int)((ulonglong)in_d1 >> 0x20),piVar3);
    (**(code **)(*(int *)param_1 + 0x28))(aVStack_40,param_1);
    fVar6 = DAT_000b5d38;
    for (uVar4 = 0; uVar4 < **(uint **)(param_1 + 0xcc); uVar4 = uVar4 + 1) {
      if (*(int *)(*(int *)((*(uint **)(param_1 + 0xcc))[1] + uVar4 * 4) + 0x18) == 2) {
        pVVar1 = (Vector *)AEGeometry::getMatrix();
        AbyssEngine::AEMath::MatrixRotateVector((Matrix *)((int)&local_68 + 4),pVVar1);
        AbyssEngine::AEMath::operator+(aVStack_58,aVStack_40);
        AbyssEngine::AEMath::operator-(aVStack_4c,aVStack_58);
        fVar2 = (float)AbyssEngine::AEMath::VectorLength(aVStack_4c);
        fVar5 = -fVar2;
        if (0.0 < fVar2) {
          fVar5 = fVar2;
        }
        if ((int)((uint)(fVar5 < fVar6) << 0x1f) < 0) {
          fVar6 = fVar5;
        }
      }
    }
    piVar3 = (int *)local_68;
  }
  if (*piVar3 - local_34 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(*piVar3 - local_34);
}

```

## target disasm
```
  000b5c54: push {r4,r5,r6,r7,lr}
  000b5c56: add r7,sp,#0xc
  000b5c58: push {r7,r8,r9,r10,r11}
  000b5c5c: vpush {d1,d2,d3,d4,d5,d6,d7,d8}
  000b5c60: mov r5,r0
  000b5c62: ldr r0,[0x000b5d3c]
  000b5c64: mov r10,r1
  000b5c66: add r0,pc
  000b5c68: ldr r1,[r0,#0x0]
  000b5c6a: ldr r0,[r1,#0x0]
  000b5c6c: str r0,[sp,#0x34]
  000b5c6e: ldr.w r0,[r5,#0xcc]
  000b5c72: cmp r0,#0x0
  000b5c74: beq 0x000b5d16
  000b5c76: ldr r0,[r5,#0x0]
  000b5c78: str r1,[sp,#0x0]
  000b5c7a: mov r1,r5
  000b5c7c: ldr r2,[r0,#0x28]
  000b5c7e: add r0,sp,#0x28
  000b5c80: blx r2
  000b5c82: vldr.32 s16,[pc,#0xb4]
  000b5c86: add.w r11,sp,#0x4
  000b5c8a: add r4,sp,#0x10
  000b5c8c: add r6,sp,#0x1c
  000b5c8e: mov.w r9,#0x0
  000b5c92: mov.w r8,#0x0
  000b5c96: b 0x000b5d08
  000b5c98: ldr r0,[r0,#0x4]
  000b5c9a: ldr.w r0,[r0,r8,lsl #0x2]
  000b5c9e: ldr r0,[r0,#0x18]
  000b5ca0: cmp r0,#0x2
  000b5ca2: bne 0x000b5d04
  000b5ca4: ldr r0,[r5,#0x8]
  000b5ca6: blx 0x000721cc
  000b5caa: mov r1,r0
  000b5cac: ldr.w r0,[r5,#0xcc]
  000b5cb0: ldr r0,[r0,#0x4]
  000b5cb2: ldr.w r2,[r0,r8,lsl #0x2]
  000b5cb6: mov r0,r11
  000b5cb8: blx 0x0006f694
  000b5cbc: add r1,sp,#0x28
  000b5cbe: mov r0,r4
  000b5cc0: mov r2,r11
  000b5cc2: blx 0x0006f1cc
  000b5cc6: mov r0,r6
  000b5cc8: mov r1,r4
  000b5cca: mov r2,r10
  000b5ccc: blx 0x0006ec38
  000b5cd0: mov r0,r6
  000b5cd2: blx 0x0006ec44
  000b5cd6: vmov s2,r0
  000b5cda: vcmpe.f32 s2,#0
  000b5cde: vmrs apsr,fpscr
  000b5ce2: vneg.f32 s0,s2
  000b5ce6: it gt
  000b5ce8: vmov.gt.f32 s0,s2
  000b5cec: vcmpe.f32 s0,s16
  000b5cf0: vmrs apsr,fpscr
  000b5cf4: bpl 0x000b5d04
  000b5cf6: ldr.w r0,[r5,#0xcc]
  000b5cfa: vmov.f32 s16,s0
  000b5cfe: ldr r0,[r0,#0x4]
  000b5d00: ldr.w r9,[r0,r8,lsl #0x2]
  000b5d04: add.w r8,r8,#0x1
  000b5d08: ldr.w r0,[r5,#0xcc]
  000b5d0c: ldr r1,[r0,#0x0]
  000b5d0e: cmp r8,r1
  000b5d10: bcc 0x000b5c98
  000b5d12: ldr r1,[sp,#0x0]
  000b5d14: b 0x000b5d1a
  000b5d16: mov.w r9,#0x0
  000b5d1a: ldr r0,[sp,#0x34]
  000b5d1c: ldr r1,[r1,#0x0]
  000b5d1e: subs r0,r1,r0
  000b5d20: itttt eq
  000b5d22: mov.eq r0,r9
  000b5d24: vpop.eq {d1,d2,d3,d4,d5,d6,d7,d8}
  000b5d28: add.eq sp,#0x4
  000b5d2a: pop.eq.w {r8,r9,r10,r11}
  000b5d2e: it eq
  000b5d30: pop.eq {r4,r5,r6,r7,pc}
  000b5d32: blx 0x0006e824
```
