# PaintCanvas::FillRectangle
elf 0x79288 body 200
Sig: undefined __stdcall FillRectangle(int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::FillRectangle(int, int, int, int) */

void AbyssEngine::PaintCanvas::FillRectangle(int param_1,int param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  int *piVar2;
  uint in_fpscr;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int in_stack_00000000;
  undefined4 local_50 [5];
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined4 local_18;
  int local_14;
  
  uStack_20 = DAT_00089358;
  uStack_28 = DAT_00089350;
  piVar2 = *(int **)(DAT_00089360 + 0x8929c);
  uVar3 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  uStack_34 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_30 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_2c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  local_14 = *piVar2;
  uVar4 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
  uVar5 = VectorSignedToFloat(param_2 + param_4,(byte)(in_fpscr >> 0x16) & 3);
  puVar1 = *(undefined4 **)(*(int *)(param_1 + 0x1c8) + 4);
  uVar6 = VectorSignedToFloat(in_stack_00000000 + param_3,(byte)(in_fpscr >> 0x16) & 3);
  *puVar1 = uVar3;
  puVar1[1] = uVar4;
  puVar1[3] = uVar5;
  puVar1[4] = uVar4;
  puVar1[6] = uVar5;
  puVar1[7] = uVar6;
  puVar1[9] = uVar3;
  puVar1[10] = uVar6;
  puVar1 = (undefined4 *)((uint)local_50 | 4);
  *puVar1 = 0;
  puVar1[1] = uStack_34;
  puVar1[2] = uStack_30;
  puVar1[3] = uStack_2c;
  local_38 = 0;
  local_50[0] = 0x3f800000;
  local_3c = 0x3f800000;
  local_18 = 0x3f800000;
  SetWorldViewMatrix((Matrix *)param_1);
  Engine::GlEnable(*(uint *)(param_1 + 0x34),true);
  MeshDraw(*(Engine **)(param_1 + 0x34),*(Mesh **)(param_1 + 0x1c8));
  Engine::GlEnable(*(uint *)(param_1 + 0x34),true);
  if (*piVar2 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00089288: push {r4,r5,r7,lr}
  0008928a: add r7,sp,#0x8
  0008928c: sub sp,#0x40
  0008928e: mov r4,r0
  00089290: ldr r0,[0x00089360]
  00089292: vmov s0,r1
  00089296: add r1,r3
  00089298: add r0,pc
  0008929a: ldr.w r12,[r7,#0x8]
  0008929e: vmov s2,r2
  000892a2: ldr r5,[r0,#0x0]
  000892a4: vcvt.f32.s32 s0,s0
  000892a8: vmov s4,r1
  000892ac: add.w r1,r12,r2
  000892b0: vmov.i32 q9,#0x0
  000892b4: adr r2,[0x89350]
  000892b6: ldr r0,[r5,#0x0]
  000892b8: vcvt.f32.s32 s2,s2
  000892bc: str r0,[sp,#0x3c]
  000892be: vmov s6,r1
  000892c2: ldr.w r0,[r4,#0x1c8]
  000892c6: vcvt.f32.s32 s4,s4
  000892ca: mov r1,sp
  000892cc: vld1.64 {d16,d17},[r2]
  000892d0: ldr r0,[r0,#0x4]
  000892d2: vcvt.f32.s32 s6,s6
  000892d6: vstr.32 s0,[r0]
  000892da: vstr.32 s2,[r0,#0x4]
  000892de: vstr.32 s4,[r0,#0xc]
  000892e2: vstr.32 s2,[r0,#0x10]
  000892e6: vstr.32 s4,[r0,#0x18]
  000892ea: vstr.32 s6,[r0,#0x1c]
  000892ee: vstr.32 s0,[r0,#0x24]
  000892f2: vstr.32 s6,[r0,#0x28]
  000892f6: orr r0,r1,#0x4
  000892fa: vst1.32 {d18,d19},[r0]
  000892fe: add.w r0,r1,#0x18
  00089302: vst1.64 {d18,d19},[r0]
  00089306: add.w r0,r1,#0x28
  0008930a: vst1.64 {d16,d17},[r0]
  0008930e: mov.w r0,#0x3f800000
  00089312: str r0,[sp,#0x0]
  00089314: str r0,[sp,#0x14]
  00089316: str r0,[sp,#0x38]
  00089318: mov r0,r4
  0008931a: blx 0x0006f55c
  0008931e: ldr r0,[r4,#0x34]
  00089320: movw r1,#0xde1
  00089324: movs r2,#0x0
  00089326: blx 0x0006eddc
  0008932a: ldr r0,[r4,#0x34]
  0008932c: ldr.w r1,[r4,#0x1c8]
  00089330: blx 0x0006f568
  00089334: ldr r0,[r4,#0x34]
  00089336: movw r1,#0xde1
  0008933a: movs r2,#0x1
  0008933c: blx 0x0006eddc
  00089340: ldr r0,[sp,#0x3c]
  00089342: ldr r1,[r5,#0x0]
  00089344: subs r0,r1,r0
  00089346: itt eq
  00089348: add.eq sp,#0x40
  0008934a: pop.eq {r4,r5,r7,pc}
  0008934c: blx 0x0006e824
```
