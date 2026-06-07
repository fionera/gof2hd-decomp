# PlayerFighter::push
elf 0xdfdc8 body 322
Sig: undefined __stdcall push(int param_1)

## decompile
```c

/* PlayerFighter::push(int) */

void PlayerFighter::push(int param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int iVar3;
  undefined4 *puVar4;
  Matrix *pMVar5;
  undefined4 uVar6;
  int in_r1;
  int iVar7;
  Matrix *pMVar8;
  code *pcVar9;
  int *piVar10;
  Vector *pVVar11;
  uint in_fpscr;
  float fVar12;
  float in_s1;
  float fVar13;
  undefined1 auStack_c4 [12];
  undefined1 auStack_b8 [12];
  Matrix aMStack_ac [60];
  undefined4 local_70 [5];
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined4 local_38;
  int local_34;
  
  uVar2 = DAT_000eff18;
  uVar1 = DAT_000eff10;
  piVar10 = *(int **)(DAT_000eff24 + 0xefdde);
  local_34 = *piVar10;
  if (0 < *(int *)(param_1 + 0x104)) {
    iVar3 = *(int *)(param_1 + 0x104) - in_r1;
    uStack_54 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_50 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_4c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    fVar13 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    fVar12 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x108),
                                        (byte)(in_fpscr >> 0x16) & 3);
    *(int *)(param_1 + 0x104) = iVar3;
    puVar4 = (undefined4 *)((uint)local_70 | 4);
    *puVar4 = 0;
    puVar4[1] = uStack_54;
    puVar4[2] = uStack_50;
    puVar4[3] = uStack_4c;
    local_58 = 0;
    fVar13 = fVar13 / fVar12;
    uStack_48 = uVar1;
    uStack_40 = uVar2;
    local_70[0] = 0x3f800000;
    local_5c = 0x3f800000;
    local_38 = 0x3f800000;
    AbyssEngine::AEMath::MatrixSetRotation
              (aMStack_ac,fVar13 * *(float *)(param_1 + 0x120),in_s1,
               fVar13 * *(float *)(param_1 + 0x11c));
    iVar3 = *(int *)(param_1 + 0x1d0);
    iVar7 = *(int *)(param_1 + 0x1d4);
    if ((int)(uint)(iVar3 == 0) <= iVar7) {
      pMVar8 = *(Matrix **)(param_1 + 8);
      pMVar5 = (Matrix *)AEGeometry::getMatrix();
      AbyssEngine::AEMath::operator*(aMStack_ac,pMVar5);
      AEGeometry::setMatrix(pMVar8);
      iVar3 = *(int *)(param_1 + 0x1d0);
      iVar7 = *(int *)(param_1 + 0x1d4);
    }
    uVar6 = __aeabi_l2f(iVar3,iVar7);
    pVVar11 = *(Vector **)(param_1 + 8);
    fVar12 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x108),
                                        (byte)(in_fpscr >> 0x16) & 3);
    pcVar9 = *(code **)(DAT_000eff28 + 0xefea4);
    (*pcVar9)(auStack_c4,param_1 + 0x10c,uVar6);
    (*pcVar9)(auStack_b8,auStack_c4,*(undefined4 *)(param_1 + 0x1e8));
    (*pcVar9)(aMStack_ac,auStack_b8,(2.0 - fVar13) * 3.0 * (fVar12 / DAT_000eff20));
    AEGeometry::translate(pVVar11);
  }
  if (*piVar10 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000efdc8: push {r4,r5,r6,r7,lr}
  000efdca: add r7,sp,#0xc
  000efdcc: push {r8,r9,r11}
  000efdd0: vpush {d8,d9}
  000efdd4: sub sp,#0x98
  000efdd6: mov r4,r0
  000efdd8: ldr r0,[0x000eff24]
  000efdda: add r0,pc
  000efddc: ldr.w r8,[r0,#0x0]
  000efde0: ldr.w r0,[r8,#0x0]
  000efde4: str r0,[sp,#0x94]
  000efde6: ldr.w r0,[r4,#0x104]
  000efdea: cmp r0,#0x1
  000efdec: blt.w 0x000efef0
  000efdf0: subs r0,r0,r1
  000efdf2: vldr.32 s0,[r4,#0x108]
  000efdf6: adr r1,[0xeff10]
  000efdf8: vmov.i32 q9,#0x0
  000efdfc: vmov s2,r0
  000efe00: vld1.64 {d16,d17},[r1]
  000efe04: add r1,sp,#0x58
  000efe06: vcvt.f32.s32 s2,s2
  000efe0a: vcvt.f32.s32 s0,s0
  000efe0e: str.w r0,[r4,#0x104]
  000efe12: orr r0,r1,#0x4
  000efe16: vst1.32 {d18,d19},[r0]
  000efe1a: add.w r0,r1,#0x18
  000efe1e: vst1.64 {d18,d19},[r0]
  000efe22: add.w r0,r1,#0x28
  000efe26: vdiv.f32 s16,s2,s0
  000efe2a: vst1.64 {d16,d17},[r0]
  000efe2e: mov.w r0,#0x3f800000
  000efe32: str r0,[sp,#0x58]
  000efe34: str r0,[sp,#0x6c]
  000efe36: str r0,[sp,#0x90]
  000efe38: add r0,sp,#0x1c
  000efe3a: vldr.32 s0,[r4,#0x118]
  000efe3e: vldr.32 s2,[r4,#0x11c]
  000efe42: vmul.f32 s0,s16,s0
  000efe46: vldr.32 s4,[r4,#0x120]
  000efe4a: vmul.f32 s2,s16,s2
  000efe4e: vmov r2,s0
  000efe52: vmov r3,s2
  000efe56: vmul.f32 s0,s16,s4
  000efe5a: vstr.32 s0,[sp]
  000efe5e: blx 0x00072094
  000efe62: ldrd r0,r1,[r4,#0x1d0]
  000efe66: subs r2,r0,#0x1
  000efe68: sbcs r2,r1,#0x0
  000efe6c: blt 0x000efe92
  000efe6e: ldr r5,[r4,#0x8]
  000efe70: add.w r9,r4,#0x1d0
  000efe74: mov r0,r5
  000efe76: blx 0x000721cc
  000efe7a: add r6,sp,#0x1c
  000efe7c: add r2,sp,#0x58
  000efe7e: mov r1,r0
  000efe80: mov r0,r6
  000efe82: blx 0x0006f82c
  000efe86: mov r0,r5
  000efe88: mov r1,r6
  000efe8a: blx 0x000721e4
  000efe8e: ldrd r0,r1,[r9,#0x0]
  000efe92: blx 0x0006f7d8
  000efe96: mov r2,r0
  000efe98: ldr r0,[0x000eff28]
  000efe9a: add r6,sp,#0x4
  000efe9c: vldr.32 s0,[r4,#0x108]
  000efea0: add r0,pc
  000efea2: add.w r1,r4,#0x10c
  000efea6: ldr.w r9,[r4,#0x8]
  000efeaa: vcvt.f32.s32 s18,s0
  000efeae: ldr r5,[r0,#0x0]
  000efeb0: mov r0,r6
  000efeb2: blx r5
  000efeb4: ldr.w r2,[r4,#0x1e8]
  000efeb8: add r4,sp,#0x10
  000efeba: mov r1,r6
  000efebc: mov r0,r4
  000efebe: blx r5
  000efec0: vldr.32 s0,[pc,#0x5c]
  000efec4: vmov.f32 s2,0x40000000
  000efec8: vmov.f32 s4,0x40400000
  000efecc: add r6,sp,#0x1c
  000efece: vdiv.f32 s0,s18,s0
  000efed2: mov r1,r4
  000efed4: mov r0,r6
  000efed6: vsub.f32 s2,s2,s16
  000efeda: vmul.f32 s2,s2,s4
  000efede: vmul.f32 s0,s2,s0
  000efee2: vmov r2,s0
  000efee6: blx r5
  000efee8: mov r0,r9
  000efeea: mov r1,r6
  000efeec: blx 0x00072460
  000efef0: ldr r0,[sp,#0x94]
  000efef2: ldr.w r1,[r8,#0x0]
  000efef6: subs r0,r1,r0
  000efef8: itttt eq
  000efefa: add.eq sp,#0x98
  000efefc: vpop.eq {d8,d9}
  000eff00: pop.eq.w {r8,r9,r11}
  000eff04: pop.eq {r4,r5,r6,r7,pc}
  000eff06: blx 0x0006e824
```
