# PaintCanvas::CameraIsPointinViewFrustum
elf 0x7ba98 body 258
Sig: undefined __thiscall CameraIsPointinViewFrustum(PaintCanvas * this, Vector * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraIsPointinViewFrustum(AbyssEngine::AEMath::Vector const&) */

void __thiscall
AbyssEngine::PaintCanvas::CameraIsPointinViewFrustum(PaintCanvas *this,Vector *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  int iVar4;
  uint uVar5;
  undefined4 uVar6;
  int *piVar7;
  float extraout_s0;
  AEMath aAStack_9c [60];
  undefined4 local_60;
  uint local_5c;
  uint local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined4 local_28;
  int local_24;
  
  uVar2 = DAT_0008bba0;
  uVar1 = DAT_0008bb98;
  piVar7 = *(int **)(DAT_0008bbb0 + 0x8baac);
  local_24 = *piVar7;
  if (*(uint *)(this + 0x170) < *(uint *)(this + 0x164)) {
    if (*this == (PaintCanvas)0x0) {
      iVar4 = *piVar7 - local_24;
      if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(iVar4,*piVar7,
                         *(undefined4 *)(*(int *)(this + 0x168) + *(uint *)(this + 0x170) * 4));
      }
      (*(code *)(DAT_001ab2f4 + 0x1ab2f8))();
      return;
    }
    uStack_44 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_40 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_3c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar3 = (undefined4 *)((uint)&local_60 | 4);
    *puVar3 = 0;
    puVar3[1] = uStack_44;
    puVar3[2] = uStack_40;
    puVar3[3] = uStack_3c;
    local_48 = 0;
    uStack_38 = uVar1;
    uStack_30 = uVar2;
    local_60 = 0x3f800000;
    local_4c = 0x3f800000;
    local_28 = 0x3f800000;
    AEMath::MatrixIdentity(aAStack_9c,(Matrix *)&local_60);
    iVar4 = Engine::GetGravValue(*(Engine **)(this + 0x34));
    uVar5 = AEMath::Sinf((float)(*(double *)(iVar4 + 8) * DAT_0008bba8));
    local_60 = AEMath::Cosf(extraout_s0);
    local_5c = uVar5 ^ 0x80000000;
    local_50 = uVar5;
    local_4c = local_60;
    uVar6 = AbyssEngine::CameraIsPointinViewFrustum
                      (param_1,(Matrix *)&local_60,
                       *(Camera **)(*(int *)(this + 0x168) + *(int *)(this + 0x170) * 4));
  }
  else {
    uVar6 = 1;
  }
  if (*piVar7 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

```

## target disasm
```
  0008ba98: push {r4,r5,r6,r7,lr}
  0008ba9a: add r7,sp,#0xc
  0008ba9c: push {r8,r9,r10}
  0008baa0: sub sp,#0x80
  0008baa2: mov r5,r0
  0008baa4: ldr r0,[0x0008bbb0]
  0008baa6: mov r9,r1
  0008baa8: add r0,pc
  0008baaa: ldr.w r10,[r0,#0x0]
  0008baae: ldr.w r0,[r10,#0x0]
  0008bab2: str r0,[sp,#0x7c]
  0008bab4: ldr.w r1,[r5,#0x164]
  0008bab8: ldr.w r0,[r5,#0x170]
  0008babc: cmp r0,r1
  0008babe: bcs 0x0008bb56
  0008bac0: ldrb r1,[r5,#0x0]
  0008bac2: cmp r1,#0x0
  0008bac4: beq 0x0008bb6e
  0008bac6: adr r0,[0x8bb98]
  0008bac8: add.w r8,sp,#0x40
  0008bacc: vmov.i32 q8,#0x0
  0008bad0: vld1.64 {d18,d19},[r0]
  0008bad4: orr r0,r8,#0x4
  0008bad8: mov r1,r8
  0008bada: vst1.32 {d16,d17},[r0]
  0008bade: add.w r0,r8,#0x18
  0008bae2: vst1.64 {d16,d17},[r0]
  0008bae6: add.w r0,r8,#0x28
  0008baea: vst1.64 {d18,d19},[r0]
  0008baee: mov.w r0,#0x3f800000
  0008baf2: str r0,[sp,#0x40]
  0008baf4: str r0,[sp,#0x54]
  0008baf6: str r0,[sp,#0x78]
  0008baf8: add r0,sp,#0x4
  0008bafa: blx 0x0006fb2c
  0008bafe: ldr r0,[r5,#0x34]
  0008bb00: blx 0x0006fb38
  0008bb04: vldr.64 d17,[r0,#0x8]
  0008bb08: vldr.64 d16,[pc,#0x9c]
  0008bb0c: ldr r0,[r5,#0x30]
  0008bb0e: vmul.f64 d16,d17,d16
  0008bb12: cmp r0,#0x1
  0008bb14: vcvt.f32.f64 s0,d16
  0008bb18: vneg.f32 s2,s0
  0008bb1c: it eq
  0008bb1e: vmov.eq.f32 s2,s0
  0008bb22: vmov r6,s2
  0008bb26: mov r0,r6
  0008bb28: blx 0x0006f1a8
  0008bb2c: mov r4,r0
  0008bb2e: mov r0,r6
  0008bb30: blx 0x0006f49c
  0008bb34: str r0,[sp,#0x40]
  0008bb36: str r0,[sp,#0x54]
  0008bb38: eor r0,r4,#0x80000000
  0008bb3c: str r0,[sp,#0x44]
  0008bb3e: str r4,[sp,#0x50]
  0008bb40: ldr.w r0,[r5,#0x168]
  0008bb44: ldr.w r1,[r5,#0x170]
  0008bb48: ldr.w r2,[r0,r1,lsl #0x2]
  0008bb4c: mov r0,r9
  0008bb4e: mov r1,r8
  0008bb50: blx 0x0006fdb4
  0008bb54: b 0x0008bb58
  0008bb56: movs r0,#0x1
  0008bb58: ldr r1,[sp,#0x7c]
  0008bb5a: ldr.w r2,[r10,#0x0]
  0008bb5e: subs r1,r2,r1
  0008bb60: ittt eq
  0008bb62: add.eq sp,#0x80
  0008bb64: pop.eq.w {r8,r9,r10}
  0008bb68: pop.eq {r4,r5,r6,r7,pc}
  0008bb6a: blx 0x0006e824
  0008bb6e: ldr.w r1,[r5,#0x168]
  0008bb72: ldr.w r2,[r1,r0,lsl #0x2]
  0008bb76: ldr r0,[sp,#0x7c]
  0008bb78: ldr.w r1,[r10,#0x0]
  0008bb7c: subs r0,r1,r0
  0008bb7e: itttt eq
  0008bb80: mov.eq r0,r9
  0008bb82: mov.eq r1,#0x0
  0008bb84: add.eq sp,#0x80
  0008bb86: pop.eq.w {r8,r9,r10}
  0008bb8a: itt eq
  0008bb8c: pop.eq.w {r4,r5,r6,r7,lr}
  0008bb90: b.eq.w 0x001ab2e8
  0008bb94: blx 0x0006e824
  001ab2e8: bx pc
```
