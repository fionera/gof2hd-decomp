# PaintCanvas::TransformGet2DPickedTextureRegion
elf 0x7b590 body 218
Sig: undefined __stdcall TransformGet2DPickedTextureRegion(uint param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformGet2DPickedTextureRegion(unsigned int, int, int, int, int) */

void AbyssEngine::PaintCanvas::TransformGet2DPickedTextureRegion
               (uint param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  uint in_fpscr;
  undefined4 in_stack_00000004;
  Matrix aMStack_80 [12];
  float local_74;
  float local_70;
  undefined4 local_6c;
  Vector aVStack_68 [64];
  int local_28;
  
  uVar1 = DAT_0008b66c;
  piVar3 = *(int **)(DAT_0008b670 + 0x8b5a6);
  local_28 = *piVar3;
  if ((uint)param_3 < *(uint *)(param_2 + 0x158)) {
    iVar4 = *(int *)(*(int *)(param_2 + 0x15c) + param_3 * 4);
    __aeabi_memcpy(aVStack_68,iVar4,0x3c);
    local_70 = (float)VectorSignedToFloat(param_5,(byte)(in_fpscr >> 0x16) & 3);
    local_74 = (float)VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
    local_6c = VectorSignedToFloat(in_stack_00000004,(byte)(in_fpscr >> 0x16) & 3);
    AEMath::MatrixInverseTransformVector(aMStack_80,aVStack_68);
    AEMath::Vector::operator=((Vector *)&local_74,(Vector *)aMStack_80);
    TransformGet2DPickedTextureRegion
              ((Transform *)param_1,param_2,iVar4,(int)local_74,(int)local_70);
  }
  else {
    iVar2 = DAT_0008b678 + 0x8b64c;
    iVar4 = *(int *)(DAT_0008b674 + 0x8b648);
    *(undefined4 *)param_1 = DAT_0008b66c;
    *(undefined4 *)(param_1 + 4) = uVar1;
    fprintf(iVar4 + 0xa8,iVar2);
  }
  if (*piVar3 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008b590: push {r4,r5,r6,r7,lr}
  0008b592: add r7,sp,#0xc
  0008b594: push {r8,r9,r10,r11}
  0008b598: sub sp,#0x6c
  0008b59a: mov r4,r0
  0008b59c: ldr r0,[0x0008b670]
  0008b59e: mov r10,r3
  0008b5a0: mov r5,r1
  0008b5a2: add r0,pc
  0008b5a4: ldr r6,[r0,#0x0]
  0008b5a6: ldr r0,[r6,#0x0]
  0008b5a8: str r0,[sp,#0x68]
  0008b5aa: ldr.w r3,[r1,#0x158]
  0008b5ae: cmp r3,r2
  0008b5b0: bls 0x0008b640
  0008b5b2: ldr.w r0,[r5,#0x15c]
  0008b5b6: add.w r9,sp,#0x28
  0008b5ba: ldr.w r11,[r7,#0x8]
  0008b5be: ldr.w r8,[r0,r2,lsl #0x2]
  0008b5c2: mov r0,r9
  0008b5c4: movs r2,#0x3c
  0008b5c6: mov r1,r8
  0008b5c8: blx 0x0006f1e4
  0008b5cc: ldr r0,[r7,#0xc]
  0008b5ce: vmov s0,r11
  0008b5d2: vmov s2,r10
  0008b5d6: add.w r11,sp,#0x1c
  0008b5da: vcvt.f32.s32 s0,s0
  0008b5de: add.w r10,sp,#0x10
  0008b5e2: vmov s4,r0
  0008b5e6: mov r1,r9
  0008b5e8: vcvt.f32.s32 s2,s2
  0008b5ec: mov r0,r10
  0008b5ee: vcvt.f32.s32 s4,s4
  0008b5f2: mov r2,r11
  0008b5f4: vstr.32 s0,[sp,#0x20]
  0008b5f8: vstr.32 s2,[sp,#0x1c]
  0008b5fc: vstr.32 s4,[sp,#0x24]
  0008b600: blx 0x0006f4b4
  0008b604: mov r0,r11
  0008b606: mov r1,r10
  0008b608: blx 0x0006eb3c
  0008b60c: vldr.32 s0,[sp,#0x1c]
  0008b610: mov r1,r5
  0008b612: vldr.32 s2,[sp,#0x20]
  0008b616: mov r2,r8
  0008b618: vldr.32 s4,[sp,#0x24]
  0008b61c: ldr r0,[r7,#0x10]
  0008b61e: vcvt.s32.f32 s4,s4
  0008b622: vcvt.s32.f32 s2,s2
  0008b626: vcvt.s32.f32 s0,s0
  0008b62a: str r0,[sp,#0x8]
  0008b62c: mov r0,r4
  0008b62e: vstr.32 s4,[sp,#0x4]
  0008b632: vstr.32 s2,[sp]
  0008b636: vmov r3,s0
  0008b63a: blx 0x0006fd60
  0008b63e: b 0x0008b656
  0008b640: ldr r0,[0x0008b674]
  0008b642: ldr r1,[0x0008b678]
  0008b644: add r0,pc
  0008b646: ldr r5,[0x0008b66c]
  0008b648: add r1,pc
  0008b64a: ldr r0,[r0,#0x0]
  0008b64c: strd r5,r5,[r4,#0x0]
  0008b650: adds r0,#0xa8
  0008b652: blx 0x0006ef74
  0008b656: ldr r0,[sp,#0x68]
  0008b658: ldr r1,[r6,#0x0]
  0008b65a: subs r0,r1,r0
  0008b65c: ittt eq
  0008b65e: add.eq sp,#0x6c
  0008b660: pop.eq.w {r8,r9,r10,r11}
  0008b664: pop.eq {r4,r5,r6,r7,pc}
  0008b666: blx 0x0006e824
```
