# PaintCanvas::CameraIsSphereinViewFrustum
elf 0x7bbb8 body 272
Sig: undefined __thiscall CameraIsSphereinViewFrustum(PaintCanvas * this, Vector * param_1, float param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraIsSphereinViewFrustum(AbyssEngine::AEMath::Vector const&, float)
    */

void __thiscall
AbyssEngine::PaintCanvas::CameraIsSphereinViewFrustum
          (PaintCanvas *this,Vector *param_1,float param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  int iVar4;
  uint uVar5;
  undefined4 uVar6;
  Matrix *in_r2;
  int *piVar7;
  float extraout_s0;
  float extraout_s0_00;
  AEMath aAStack_a4 [60];
  undefined4 local_68;
  uint local_64;
  uint local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined4 local_30;
  int local_28;
  
  uVar2 = DAT_0008bcd0;
  uVar1 = DAT_0008bcc8;
  piVar7 = *(int **)(DAT_0008bce0 + 0x8bbd0);
  local_28 = *piVar7;
  if (((float)in_r2 == 0.0) || (*(uint *)(this + 0x164) <= *(uint *)(this + 0x170))) {
    uVar6 = 1;
  }
  else {
    if (*this == (PaintCanvas)0x0) {
      if (*piVar7 - local_28 != 0) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(*piVar7 - local_28);
      }
      (*(code *)(DAT_001ab304 + 0x1ab308))();
      return;
    }
    uStack_4c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_48 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_44 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar3 = (undefined4 *)((uint)&local_68 | 4);
    *puVar3 = 0;
    puVar3[1] = uStack_4c;
    puVar3[2] = uStack_48;
    puVar3[3] = uStack_44;
    local_50 = 0;
    uStack_40 = uVar1;
    uStack_38 = uVar2;
    local_68 = 0x3f800000;
    local_54 = 0x3f800000;
    local_30 = 0x3f800000;
    AEMath::MatrixIdentity(aAStack_a4,(Matrix *)&local_68);
    iVar4 = Engine::GetGravValue(*(Engine **)(this + 0x34));
    uVar5 = AEMath::Sinf((float)(*(double *)(iVar4 + 8) * DAT_0008bcd8));
    local_68 = AEMath::Cosf(extraout_s0);
    local_64 = uVar5 ^ 0x80000000;
    local_58 = uVar5;
    local_54 = local_68;
    uVar6 = AbyssEngine::CameraIsSphereinViewFrustum
                      (param_1,extraout_s0_00,in_r2,(Camera *)&local_68);
  }
  if (*piVar7 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

```

## target disasm
```
  0008bbb8: push {r4,r5,r6,r7,lr}
  0008bbba: add r7,sp,#0xc
  0008bbbc: push {r8,r9,r10,r11}
  0008bbc0: sub sp,#0x84
  0008bbc2: mov r6,r0
  0008bbc4: ldr r0,[0x0008bce0]
  0008bbc6: vmov s0,r2
  0008bbca: mov r4,r2
  0008bbcc: add r0,pc
  0008bbce: mov r9,r1
  0008bbd0: vcmpe.f32 s0,#0
  0008bbd4: ldr r5,[r0,#0x0]
  0008bbd6: vmrs apsr,fpscr
  0008bbda: ldr r0,[r5,#0x0]
  0008bbdc: str r0,[sp,#0x80]
  0008bbde: beq 0x0008bc86
  0008bbe0: ldr.w r1,[r6,#0x164]
  0008bbe4: ldr.w r0,[r6,#0x170]
  0008bbe8: cmp r0,r1
  0008bbea: bcs 0x0008bc86
  0008bbec: ldrb r1,[r6,#0x0]
  0008bbee: cmp r1,#0x0
  0008bbf0: beq 0x0008bc9c
  0008bbf2: adr r0,[0x8bcc8]
  0008bbf4: add.w r8,sp,#0x40
  0008bbf8: vmov.i32 q8,#0x0
  0008bbfc: vld1.64 {d18,d19},[r0]
  0008bc00: orr r0,r8,#0x4
  0008bc04: mov r1,r8
  0008bc06: vst1.32 {d16,d17},[r0]
  0008bc0a: add.w r0,r8,#0x18
  0008bc0e: vst1.64 {d16,d17},[r0]
  0008bc12: add.w r0,r8,#0x28
  0008bc16: vst1.64 {d18,d19},[r0]
  0008bc1a: mov.w r0,#0x3f800000
  0008bc1e: str r0,[sp,#0x40]
  0008bc20: str r0,[sp,#0x54]
  0008bc22: str r0,[sp,#0x78]
  0008bc24: add r0,sp,#0x4
  0008bc26: blx 0x0006fb2c
  0008bc2a: ldr r0,[r6,#0x34]
  0008bc2c: blx 0x0006fb38
  0008bc30: vldr.64 d17,[r0,#0x8]
  0008bc34: vldr.64 d16,[pc,#0xa0]
  0008bc38: ldr r0,[r6,#0x30]
  0008bc3a: vmul.f64 d16,d17,d16
  0008bc3e: cmp r0,#0x1
  0008bc40: vcvt.f32.f64 s0,d16
  0008bc44: vneg.f32 s2,s0
  0008bc48: it eq
  0008bc4a: vmov.eq.f32 s2,s0
  0008bc4e: vmov r11,s2
  0008bc52: mov r0,r11
  0008bc54: blx 0x0006f1a8
  0008bc58: mov r10,r0
  0008bc5a: mov r0,r11
  0008bc5c: blx 0x0006f49c
  0008bc60: str r0,[sp,#0x40]
  0008bc62: mov r2,r8
  0008bc64: str r0,[sp,#0x54]
  0008bc66: eor r0,r10,#0x80000000
  0008bc6a: str r0,[sp,#0x44]
  0008bc6c: str.w r10,[sp,#0x50]
  0008bc70: ldr.w r0,[r6,#0x168]
  0008bc74: ldr.w r1,[r6,#0x170]
  0008bc78: ldr.w r3,[r0,r1,lsl #0x2]
  0008bc7c: mov r0,r9
  0008bc7e: mov r1,r4
  0008bc80: blx 0x0006f874
  0008bc84: b 0x0008bc88
  0008bc86: movs r0,#0x1
  0008bc88: ldr r1,[sp,#0x80]
  0008bc8a: ldr r2,[r5,#0x0]
  0008bc8c: subs r1,r2,r1
  0008bc8e: ittt eq
  0008bc90: add.eq sp,#0x84
  0008bc92: pop.eq.w {r8,r9,r10,r11}
  0008bc96: pop.eq {r4,r5,r6,r7,pc}
  0008bc98: blx 0x0006e824
  0008bc9c: ldr.w r1,[r6,#0x168]
  0008bca0: ldr.w r3,[r1,r0,lsl #0x2]
  0008bca4: ldr r0,[sp,#0x80]
  0008bca6: ldr r1,[r5,#0x0]
  0008bca8: subs r0,r1,r0
  0008bcaa: itttt eq
  0008bcac: mov.eq r0,r9
  0008bcae: mov.eq r1,r4
  0008bcb0: mov.eq r2,#0x0
  0008bcb2: add.eq sp,#0x84
  0008bcb4: ittt eq
  0008bcb6: pop.eq.w {r8,r9,r10,r11}
  0008bcba: pop.eq.w {r4,r5,r6,r7,lr}
  0008bcbe: b.eq.w 0x001ab2f8
  0008bcc2: blx 0x0006e824
  001ab2f8: bx pc
```
