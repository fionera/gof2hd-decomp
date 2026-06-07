# PaintCanvas::DrawImage2D
elf 0x78b68 body 200
Sig: undefined __stdcall DrawImage2D(uint param_1, int param_2, int param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int) */

void AbyssEngine::PaintCanvas::DrawImage2D(uint param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  Image2D *pIVar2;
  undefined4 in_r3;
  int *piVar3;
  uint in_fpscr;
  undefined4 local_60 [3];
  undefined4 local_54;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined4 local_28;
  int local_24;
  
  piVar3 = *(int **)(DAT_00088c40 + 0x88b7e);
  local_24 = *piVar3;
  if ((uint)param_2 < *(uint *)(param_1 + 0x14c)) {
    pIVar2 = *(Image2D **)(*(int *)(param_1 + 0x150) + param_2 * 4);
    if ((PaintCanvas *)(uint)(byte)pIVar2[0x14] != (PaintCanvas *)0x0) {
      RestoreImage2D((PaintCanvas *)(uint)(byte)pIVar2[0x14],pIVar2);
      pIVar2 = *(Image2D **)(*(int *)(param_1 + 0x150) + param_2 * 4);
    }
    SetTexture(param_1,*(uint *)(pIVar2 + 4));
    uStack_30 = DAT_00088c38;
    uStack_38 = DAT_00088c30;
    uStack_40 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_3c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    local_54 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
    uStack_44 = VectorSignedToFloat(in_r3,(byte)(in_fpscr >> 0x16) & 3);
    local_48 = 0;
    puVar1 = (undefined4 *)((uint)local_60 | 4);
    *puVar1 = 0;
    puVar1[1] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    puVar1[2] = uStack_40;
    puVar1[3] = uStack_3c;
    local_28 = 0x3f800000;
    local_60[0] = 0x3f800000;
    local_4c = 0x3f800000;
    SetWorldViewMatrix((Matrix *)param_1);
    glDisable(0xb44);
    MeshDraw(*(Engine **)(param_1 + 0x34),
             (Mesh *)**(undefined4 **)(*(int *)(param_1 + 0x150) + param_2 * 4));
    glEnable(0xb44);
  }
  if (*piVar3 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00088b68: push {r4,r5,r6,r7,lr}
  00088b6a: add r7,sp,#0xc
  00088b6c: push {r8,r9,r11}
  00088b70: sub sp,#0x40
  00088b72: mov r5,r0
  00088b74: ldr r0,[0x00088c40]
  00088b76: mov r8,r3
  00088b78: mov r9,r2
  00088b7a: add r0,pc
  00088b7c: mov r4,r1
  00088b7e: ldr r6,[r0,#0x0]
  00088b80: ldr r0,[r6,#0x0]
  00088b82: str r0,[sp,#0x3c]
  00088b84: ldr.w r0,[r5,#0x14c]
  00088b88: cmp r0,r1
  00088b8a: bls 0x00088c1c
  00088b8c: ldr.w r0,[r5,#0x150]
  00088b90: ldr.w r1,[r0,r4,lsl #0x2]
  00088b94: ldrb r0,[r1,#0x14]
  00088b96: cbz r0,0x00088ba4
  00088b98: blx 0x0006faf0
  00088b9c: ldr.w r0,[r5,#0x150]
  00088ba0: ldr.w r1,[r0,r4,lsl #0x2]
  00088ba4: ldr r1,[r1,#0x4]
  00088ba6: mov r0,r5
  00088ba8: mov.w r2,#0xffffffff
  00088bac: blx 0x0006fa6c
  00088bb0: vmov s0,r9
  00088bb4: adr r0,[0x88c30]
  00088bb6: vmov.i32 q8,#0x0
  00088bba: vld1.64 {d18,d19},[r0]
  00088bbe: vmov s2,r8
  00088bc2: mov r1,sp
  00088bc4: add.w r0,r1,#0x18
  00088bc8: vcvt.f32.s32 s0,s0
  00088bcc: vcvt.f32.s32 s2,s2
  00088bd0: vst1.64 {d16,d17},[r0]
  00088bd4: orr r0,r1,#0x4
  00088bd8: vst1.32 {d16,d17},[r0]
  00088bdc: add.w r0,r1,#0x28
  00088be0: vst1.64 {d18,d19},[r0]
  00088be4: mov.w r0,#0x3f800000
  00088be8: str r0,[sp,#0x38]
  00088bea: str r0,[sp,#0x0]
  00088bec: str r0,[sp,#0x14]
  00088bee: mov r0,r5
  00088bf0: vstr.32 s2,[sp,#0x1c]
  00088bf4: vstr.32 s0,[sp,#0xc]
  00088bf8: blx 0x0006f55c
  00088bfc: movw r0,#0xb44
  00088c00: blx 0x0006edc4
  00088c04: ldr.w r1,[r5,#0x150]
  00088c08: ldr r0,[r5,#0x34]
  00088c0a: ldr.w r1,[r1,r4,lsl #0x2]
  00088c0e: ldr r1,[r1,#0x0]
  00088c10: blx 0x0006f568
  00088c14: movw r0,#0xb44
  00088c18: blx 0x0006edb8
  00088c1c: ldr r0,[sp,#0x3c]
  00088c1e: ldr r1,[r6,#0x0]
  00088c20: subs r0,r1,r0
  00088c22: ittt eq
  00088c24: add.eq sp,#0x40
  00088c26: pop.eq.w {r8,r9,r11}
  00088c2a: pop.eq {r4,r5,r6,r7,pc}
  00088c2c: blx 0x0006e824
```
