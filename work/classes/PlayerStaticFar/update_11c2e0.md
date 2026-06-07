# PlayerStaticFar::update
elf 0x11c2e0 body 354
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* PlayerStaticFar::update(int) */

void PlayerStaticFar::update(int param_1)

{
  float fVar1;
  uint uVar2;
  code *pcVar3;
  Vector *this;
  int *piVar4;
  uint in_fpscr;
  undefined4 uVar5;
  float fVar6;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  undefined4 uVar7;
  float extraout_s2;
  undefined8 in_d6;
  undefined4 in_s14;
  undefined8 local_38;
  undefined4 local_30;
  int local_2c;
  
  piVar4 = *(int **)(DAT_0012c454 + 0x12c2f4);
  local_2c = *piVar4;
  local_38 = in_d6;
  local_30 = in_s14;
  if (*(int *)(param_1 + 8) != 0) {
    uVar2 = **(uint **)(DAT_0012c458 + 0x12c30a);
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraGetLocal(uVar2);
    AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_38);
    pcVar3 = *(code **)(DAT_0012c45c + 0x12c330);
    (*pcVar3)((Vector *)(param_1 + 0x90),&local_38);
    uVar7 = VectorSignedToFloat(*(undefined4 *)(param_1 + 0x128),(byte)(in_fpscr >> 0x16) & 3);
    uVar5 = VectorSignedToFloat(*(undefined4 *)(param_1 + 0x124),(byte)(in_fpscr >> 0x16) & 3);
    local_30 = VectorSignedToFloat(*(undefined4 *)(param_1 + 300),(byte)(in_fpscr >> 0x16) & 3);
    local_38 = CONCAT44(uVar7,uVar5);
    (*pcVar3)((Vector *)(param_1 + 0x9c),&local_38);
    AbyssEngine::AEMath::operator-((Vector *)&local_38,(Vector *)(param_1 + 0x9c));
    this = (Vector *)(param_1 + 0x134);
    (*pcVar3)(this,&local_38);
    fVar1 = (float)AbyssEngine::AEMath::VectorLength(this);
    if ((int)fVar1 < DAT_0012c444) {
      AEGeometry::setScaling(fVar1,extraout_s1,extraout_s2);
      fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x124),
                                         (byte)(in_fpscr >> 0x16) & 3);
      fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x128),
                                         (byte)(in_fpscr >> 0x16) & 3);
      VectorSignedToFloat(*(undefined4 *)(param_1 + 300),(byte)(in_fpscr >> 0x16) & 3);
      AEGeometry::setPosition(fVar1,extraout_s1_01,fVar6);
    }
    else {
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_38,this);
      fVar6 = (float)AbyssEngine::AEMath::Vector::operator=(this,(Vector *)&local_38);
      AbyssEngine::AEMath::Vector::operator*=(this,fVar6);
      AbyssEngine::AEMath::Vector::operator+=(this,(Vector *)(param_1 + 0x90));
      AEGeometry::setPosition(*(AEGeometry **)(param_1 + 8),this);
      fVar1 = (float)VectorSignedToFloat((int)fVar1,(byte)(in_fpscr >> 0x16) & 3);
      fVar1 = (float)VectorSignedToFloat((int)((DAT_0012c448 / fVar1) * DAT_0012c44c),
                                         (byte)(in_fpscr >> 0x16) & 3);
      AEGeometry::setScaling(fVar1 * DAT_0012c450,extraout_s1_00,DAT_0012c450);
    }
  }
  if (*piVar4 != local_2c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012c2e0: push {r4,r5,r6,r7,lr}
  0012c2e2: add r7,sp,#0xc
  0012c2e4: push {r8,r9,r10}
  0012c2e8: vpush {d6,d7,d8}
  0012c2ec: mov r4,r0
  0012c2ee: ldr r0,[0x0012c454]
  0012c2f0: add r0,pc
  0012c2f2: ldr.w r9,[r0,#0x0]
  0012c2f6: ldr.w r0,[r9,#0x0]
  0012c2fa: str r0,[sp,#0xc]
  0012c2fc: ldr r0,[r4,#0x8]
  0012c2fe: cmp r0,#0x0
  0012c300: beq.w 0x0012c42a
  0012c304: ldr r0,[0x0012c458]
  0012c306: add r0,pc
  0012c308: ldr r0,[r0,#0x0]
  0012c30a: ldr r5,[r0,#0x0]
  0012c30c: mov r0,r5
  0012c30e: blx 0x000717f4
  0012c312: mov r1,r0
  0012c314: mov r0,r5
  0012c316: blx 0x0006ff1c
  0012c31a: mov r6,sp
  0012c31c: mov r1,r0
  0012c31e: mov r0,r6
  0012c320: blx 0x0006f16c
  0012c324: ldr r0,[0x0012c45c]
  0012c326: add.w r8,r4,#0x90
  0012c32a: mov r1,r6
  0012c32c: add r0,pc
  0012c32e: ldr r5,[r0,#0x0]
  0012c330: mov r0,r8
  0012c332: blx r5
  0012c334: vldr.32 s0,[r4,#0x124]
  0012c338: add.w r6,r4,#0x9c
  0012c33c: vldr.32 s2,[r4,#0x128]
  0012c340: mov r1,sp
  0012c342: vldr.32 s4,[r4,#0x12c]
  0012c346: mov r0,r6
  0012c348: vcvt.f32.s32 s2,s2
  0012c34c: vcvt.f32.s32 s0,s0
  0012c350: vcvt.f32.s32 s4,s4
  0012c354: vstr.32 s2,[sp,#0x4]
  0012c358: vstr.32 s0,[sp]
  0012c35c: vstr.32 s4,[sp,#0x8]
  0012c360: blx r5
  0012c362: mov r10,sp
  0012c364: mov r1,r6
  0012c366: mov r0,r10
  0012c368: mov r2,r8
  0012c36a: blx 0x0006ec38
  0012c36e: add.w r6,r4,#0x134
  0012c372: mov r1,r10
  0012c374: mov r0,r6
  0012c376: blx r5
  0012c378: mov r0,r6
  0012c37a: blx 0x0006ec44
  0012c37e: vmov s0,r0
  0012c382: ldr r0,[0x0012c444]
  0012c384: vcvt.s32.f32 s16,s0
  0012c388: vmov r1,s16
  0012c38c: cmp r1,r0
  0012c38e: blt 0x0012c3ee
  0012c390: mov r5,sp
  0012c392: mov r1,r6
  0012c394: mov r0,r5
  0012c396: blx 0x0006ec80
  0012c39a: mov r0,r6
  0012c39c: mov r1,r5
  0012c39e: blx 0x0006eb3c
  0012c3a2: ldr r1,[0x0012c448]
  0012c3a4: mov r0,r6
  0012c3a6: blx 0x00072628
  0012c3aa: mov r0,r6
  0012c3ac: mov r1,r8
  0012c3ae: blx 0x00073534
  0012c3b2: ldr r0,[r4,#0x8]
  0012c3b4: mov r1,r6
  0012c3b6: blx 0x00072148
  0012c3ba: vcvt.f32.s32 s0,s16
  0012c3be: vldr.32 s2,[pc,#0x88]
  0012c3c2: vdiv.f32 s0,s2,s0
  0012c3c6: vldr.32 s2,[pc,#0x84]
  0012c3ca: vmul.f32 s0,s0,s2
  0012c3ce: vldr.32 s2,[pc,#0x80]
  0012c3d2: vcvt.s32.f32 s0,s0
  0012c3d6: vcvt.f32.s32 s0,s0
  0012c3da: ldr r0,[r4,#0x8]
  0012c3dc: vmul.f32 s0,s0,s2
  0012c3e0: vmov r1,s0
  0012c3e4: mov r2,r1
  0012c3e6: mov r3,r1
  0012c3e8: blx 0x000727b4
  0012c3ec: b 0x0012c42a
  0012c3ee: ldr r0,[r4,#0x8]
  0012c3f0: mov.w r1,#0x3f800000
  0012c3f4: mov.w r2,#0x3f800000
  0012c3f8: mov.w r3,#0x3f800000
  0012c3fc: blx 0x000727b4
  0012c400: vldr.32 s0,[r4,#0x124]
  0012c404: vldr.32 s2,[r4,#0x128]
  0012c408: vldr.32 s4,[r4,#0x12c]
  0012c40c: vcvt.f32.s32 s0,s0
  0012c410: vcvt.f32.s32 s2,s2
  0012c414: vcvt.f32.s32 s4,s4
  0012c418: ldr r0,[r4,#0x8]
  0012c41a: vmov r1,s0
  0012c41e: vmov r2,s2
  0012c422: vmov r3,s4
  0012c426: blx 0x00073048
  0012c42a: ldr r0,[sp,#0xc]
  0012c42c: ldr.w r1,[r9,#0x0]
  0012c430: subs r0,r1,r0
  0012c432: ittt eq
  0012c434: vpop.eq {d6,d7,d8}
  0012c438: pop.eq.w {r8,r9,r10}
  0012c43c: pop.eq {r4,r5,r6,r7,pc}
  0012c43e: blx 0x0006e824
```
