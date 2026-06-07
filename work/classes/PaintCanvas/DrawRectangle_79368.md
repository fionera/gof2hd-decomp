# PaintCanvas::DrawRectangle
elf 0x79368 body 336
Sig: undefined __thiscall DrawRectangle(PaintCanvas * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawRectangle(int, int, int, int) */

void __thiscall
AbyssEngine::PaintCanvas::DrawRectangle
          (PaintCanvas *this,int param_1,int param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  code *pcVar2;
  int *piVar3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  undefined4 local_58 [5];
  undefined4 local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined4 local_20;
  int local_1c;
  
  uStack_28 = DAT_000894c0;
  uStack_30 = DAT_000894b8;
  dVar6 = (double)VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
  dVar7 = (double)VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
  dVar8 = (double)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  dVar9 = (double)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  piVar3 = *(int **)(DAT_000894c8 + 0x893a8);
  fVar4 = (float)(dVar6 + -0.5 + dVar9);
  fVar5 = (float)(dVar7 + -0.5 + dVar8);
  uStack_3c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_38 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_34 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  local_1c = *piVar3;
  puVar1 = (undefined4 *)((uint)local_58 | 4);
  *(float *)(this + 0x1d0) = (float)(dVar9 + 0.5);
  *(float *)(this + 0x1d4) = (float)(dVar8 + 0.5);
  *(float *)(this + 0x1d8) = fVar4;
  *(float *)(this + 0x1dc) = (float)(dVar8 + 0.5);
  *(float *)(this + 0x1e0) = fVar4;
  *(float *)(this + 0x1e4) = fVar5;
  *(float *)(this + 0x1e8) = (float)(dVar9 + 0.5);
  *(float *)(this + 0x1ec) = fVar5;
  *puVar1 = 0;
  puVar1[1] = uStack_3c;
  puVar1[2] = uStack_38;
  puVar1[3] = uStack_34;
  local_40 = 0;
  local_58[0] = 0x3f800000;
  local_44 = 0x3f800000;
  local_20 = 0x3f800000;
  SetWorldViewMatrix((Matrix *)this);
  if (**(char **)(DAT_000894cc + 0x8942c) == '\0') {
    glLineWidth(0x3f800000);
    Engine::GlEnable(*(uint *)(this + 0x34),true);
    glVertexPointer(2,0x1406,0,this + 0x1d0);
    pcVar2 = *(code **)(DAT_000894d0 + 0x8946a);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8074,1);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8078,0);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8075,0);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8076,0);
    glDrawArrays(2,0,4);
    Engine::GlEnable(*(uint *)(this + 0x34),true);
  }
  else {
    Engine::DrawLine2D(*(float **)(this + 0x34),(int)(this + 0x1d0),true);
  }
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00089368: push {r4,r5,r6,r7,lr}
  0008936a: add r7,sp,#0xc
  0008936c: push.w r11
  00089370: sub sp,#0x40
  00089372: vmov s2,r3
  00089376: vldr.32 s0,[r7,#0x8]
  0008937a: vmov.f64 d17,-0x4020000000000000
  0008937e: mov r4,r0
  00089380: vcvt.f64.s32 d16,s2
  00089384: vcvt.f64.s32 d18,s0
  00089388: vmov s0,r2
  0008938c: vadd.f64 d16,d16,d17
  00089390: vcvt.f64.s32 d19,s0
  00089394: vmov s0,r1
  00089398: mov r1,sp
  0008939a: vadd.f64 d17,d18,d17
  0008939e: vcvt.f64.s32 d21,s0
  000893a2: ldr r0,[0x000894c8]
  000893a4: add r0,pc
  000893a6: ldr r6,[r0,#0x0]
  000893a8: adr r0,[0x894b8]
  000893aa: vmov.f64 d20,0x3fe0000000000000
  000893ae: vadd.f64 d18,d19,d20
  000893b2: vadd.f64 d20,d21,d20
  000893b6: vadd.f64 d16,d16,d21
  000893ba: vadd.f64 d17,d17,d19
  000893be: vcvt.f32.f64 s0,d18
  000893c2: vcvt.f32.f64 s2,d20
  000893c6: vcvt.f32.f64 s4,d16
  000893ca: vcvt.f32.f64 s6,d17
  000893ce: vld1.64 {d16,d17},[r0]
  000893d2: vmov.i32 q9,#0x0
  000893d6: ldr r0,[r6,#0x0]
  000893d8: str r0,[sp,#0x3c]
  000893da: orr r0,r1,#0x4
  000893de: vstr.32 s2,[r4,#0x1d0]
  000893e2: vstr.32 s0,[r4,#0x1d4]
  000893e6: vstr.32 s4,[r4,#0x1d8]
  000893ea: vstr.32 s0,[r4,#0x1dc]
  000893ee: vstr.32 s4,[r4,#0x1e0]
  000893f2: vstr.32 s6,[r4,#0x1e4]
  000893f6: vstr.32 s2,[r4,#0x1e8]
  000893fa: vstr.32 s6,[r4,#0x1ec]
  000893fe: vst1.32 {d18,d19},[r0]
  00089402: add.w r0,r1,#0x18
  00089406: vst1.64 {d18,d19},[r0]
  0008940a: add.w r0,r1,#0x28
  0008940e: vst1.64 {d16,d17},[r0]
  00089412: mov.w r0,#0x3f800000
  00089416: str r0,[sp,#0x0]
  00089418: str r0,[sp,#0x14]
  0008941a: str r0,[sp,#0x38]
  0008941c: mov r0,r4
  0008941e: blx 0x0006f55c
  00089422: ldr r0,[0x000894cc]
  00089424: add.w r5,r4,#0x1d0
  00089428: add r0,pc
  0008942a: ldr r0,[r0,#0x0]
  0008942c: ldrb r0,[r0,#0x0]
  0008942e: cbz r0,0x0008943e
  00089430: ldr r0,[r4,#0x34]
  00089432: mov r1,r5
  00089434: movs r2,#0x4
  00089436: movs r3,#0x1
  00089438: blx 0x0006fb14
  0008943c: b 0x000894a4
  0008943e: mov.w r0,#0x3f800000
  00089442: blx 0x0006edd0
  00089446: ldr r0,[r4,#0x34]
  00089448: movw r1,#0xde1
  0008944c: movs r2,#0x0
  0008944e: blx 0x0006eddc
  00089452: movs r0,#0x2
  00089454: movw r1,#0x1406
  00089458: movs r2,#0x0
  0008945a: mov r3,r5
  0008945c: blx 0x0006ed1c
  00089460: ldr r1,[0x000894d0]
  00089462: movs r2,#0x1
  00089464: ldr r0,[r4,#0x34]
  00089466: add r1,pc
  00089468: ldr r5,[r1,#0x0]
  0008946a: movw r1,#0x8074
  0008946e: blx r5
  00089470: ldr r0,[r4,#0x34]
  00089472: movw r1,#0x8078
  00089476: movs r2,#0x0
  00089478: blx r5
  0008947a: ldr r0,[r4,#0x34]
  0008947c: movw r1,#0x8075
  00089480: movs r2,#0x0
  00089482: blx r5
  00089484: ldr r0,[r4,#0x34]
  00089486: movw r1,#0x8076
  0008948a: movs r2,#0x0
  0008948c: blx r5
  0008948e: movs r0,#0x2
  00089490: movs r1,#0x0
  00089492: movs r2,#0x4
  00089494: blx 0x0006fb20
  00089498: ldr r0,[r4,#0x34]
  0008949a: movw r1,#0xde1
  0008949e: movs r2,#0x1
  000894a0: blx 0x0006eddc
  000894a4: ldr r0,[sp,#0x3c]
  000894a6: ldr r1,[r6,#0x0]
  000894a8: subs r0,r1,r0
  000894aa: ittt eq
  000894ac: add.eq sp,#0x40
  000894ae: pop.eq.w r11
  000894b2: pop.eq {r4,r5,r6,r7,pc}
  000894b4: blx 0x0006e824
```
