# PaintCanvas::DrawLine
elf 0x794d8 body 408
Sig: undefined __thiscall DrawLine(PaintCanvas * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawLine(int, int, int, int) */

void __thiscall
AbyssEngine::PaintCanvas::DrawLine
          (PaintCanvas *this,int param_1,int param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  code *pcVar2;
  int *piVar3;
  uint in_fpscr;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 local_68 [5];
  undefined4 local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined4 local_30;
  int local_28;
  
  uStack_38 = DAT_00089678;
  uStack_40 = DAT_00089670;
  piVar3 = *(int **)(DAT_00089684 + 0x894f4);
  local_28 = *piVar3;
  if (**(char **)(DAT_00089680 + 0x894ee) == '\0') {
    glLineWidth(0x3f800000);
    Engine::GlEnable(*(uint *)(this + 0x34),true);
    uStack_38 = DAT_00089678;
    uStack_40 = DAT_00089670;
    uVar4 = VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
    uVar5 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    uVar6 = VectorSignedToFloat(param_1 + 1,(byte)(in_fpscr >> 0x16) & 3);
    puVar1 = (undefined4 *)((uint)local_68 | 4);
    uVar7 = VectorSignedToFloat(param_3 + 1,(byte)(in_fpscr >> 0x16) & 3);
    uStack_4c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_48 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_44 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    *(undefined4 *)(this + 0x1d0) = uVar6;
    *(undefined4 *)(this + 0x1d4) = uVar5;
    *(undefined4 *)(this + 0x1d8) = uVar7;
    *(undefined4 *)(this + 0x1dc) = uVar4;
    *puVar1 = 0;
    puVar1[1] = uStack_4c;
    puVar1[2] = uStack_48;
    puVar1[3] = uStack_44;
    local_50 = 0;
    local_68[0] = 0x3f800000;
    local_54 = 0x3f800000;
    local_30 = 0x3f800000;
    SetWorldViewMatrix((Matrix *)this);
    glVertexPointer(2,0x1406,0,this + 0x1d0);
    pcVar2 = *(code **)(DAT_00089688 + 0x89620);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8074,1);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8078,0);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8075,0);
    (*pcVar2)(*(undefined4 *)(this + 0x34),0x8076,0);
    glDrawArrays(1,0,2);
    Engine::GlEnable(*(uint *)(this + 0x34),true);
  }
  else {
    uStack_4c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_48 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_44 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar1 = (undefined4 *)((uint)local_68 | 4);
    *puVar1 = 0;
    puVar1[1] = uStack_4c;
    puVar1[2] = uStack_48;
    puVar1[3] = uStack_44;
    local_50 = 0;
    local_68[0] = 0x3f800000;
    local_54 = 0x3f800000;
    local_30 = 0x3f800000;
    SetWorldViewMatrix((Matrix *)this);
    uVar4 = VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
    uVar5 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    uVar6 = VectorSignedToFloat(param_1 + 1,(byte)(in_fpscr >> 0x16) & 3);
    uVar7 = VectorSignedToFloat(param_3 + 1,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined4 *)(this + 0x1d0) = uVar6;
    *(undefined4 *)(this + 0x1d4) = uVar5;
    *(undefined4 *)(this + 0x1d8) = uVar7;
    *(undefined4 *)(this + 0x1dc) = uVar4;
    Engine::DrawLine2D(*(float **)(this + 0x34),(int)(this + 0x1d0),true);
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
  000894d8: push {r4,r5,r6,r7,lr}
  000894da: add r7,sp,#0xc
  000894dc: push {r8,r9,r10,r11}
  000894e0: sub sp,#0x44
  000894e2: mov r4,r0
  000894e4: ldr r0,[0x00089680]
  000894e6: mov r6,r1
  000894e8: ldr r1,[0x00089684]
  000894ea: add r0,pc
  000894ec: ldr.w r10,[r7,#0x8]
  000894f0: add r1,pc
  000894f2: mov r8,r3
  000894f4: ldr r0,[r0,#0x0]
  000894f6: mov r5,r2
  000894f8: ldr.w r9,[r1,#0x0]
  000894fc: ldrb r0,[r0,#0x0]
  000894fe: ldr.w r1,[r9,#0x0]
  00089502: cmp r0,#0x0
  00089504: str r1,[sp,#0x40]
  00089506: beq 0x00089582
  00089508: adr r0,[0x89670]
  0008950a: vmov.i32 q8,#0x0
  0008950e: mov r1,sp
  00089510: vld1.64 {d18,d19},[r0]
  00089514: orr r0,r1,#0x4
  00089518: vst1.32 {d16,d17},[r0]
  0008951c: add.w r0,r1,#0x18
  00089520: vst1.64 {d16,d17},[r0]
  00089524: add.w r0,r1,#0x28
  00089528: vst1.64 {d18,d19},[r0]
  0008952c: mov.w r0,#0x3f800000
  00089530: str r0,[sp,#0x0]
  00089532: str r0,[sp,#0x14]
  00089534: str r0,[sp,#0x38]
  00089536: mov r0,r4
  00089538: blx 0x0006f55c
  0008953c: adds r0,r6,#0x1
  0008953e: vmov s0,r10
  00089542: vmov s2,r5
  00089546: add.w r1,r4,#0x1d0
  0008954a: vmov s4,r0
  0008954e: add.w r0,r8,#0x1
  00089552: vcvt.f32.s32 s0,s0
  00089556: movs r2,#0x2
  00089558: vmov s6,r0
  0008955c: movs r3,#0x0
  0008955e: vcvt.f32.s32 s2,s2
  00089562: vcvt.f32.s32 s4,s4
  00089566: vcvt.f32.s32 s6,s6
  0008956a: ldr r0,[r4,#0x34]
  0008956c: vstr.32 s4,[r4,#0x1d0]
  00089570: vstr.32 s2,[r4,#0x1d4]
  00089574: vstr.32 s6,[r4,#0x1d8]
  00089578: vstr.32 s0,[r4,#0x1dc]
  0008957c: blx 0x0006fb14
  00089580: b 0x0008965a
  00089582: mov.w r0,#0x3f800000
  00089586: mov.w r11,#0x3f800000
  0008958a: blx 0x0006edd0
  0008958e: ldr r0,[r4,#0x34]
  00089590: movw r1,#0xde1
  00089594: movs r2,#0x0
  00089596: blx 0x0006eddc
  0008959a: adds r0,r6,#0x1
  0008959c: vmov s0,r10
  000895a0: vmov s2,r5
  000895a4: mov r1,sp
  000895a6: vmov s4,r0
  000895aa: add.w r0,r8,#0x1
  000895ae: vcvt.f32.s32 s0,s0
  000895b2: vmov s6,r0
  000895b6: adr r0,[0x89670]
  000895b8: vcvt.f32.s32 s2,s2
  000895bc: vld1.64 {d16,d17},[r0]
  000895c0: vcvt.f32.s32 s4,s4
  000895c4: orr r0,r1,#0x4
  000895c8: vcvt.f32.s32 s6,s6
  000895cc: vmov.i32 q9,#0x0
  000895d0: vstr.32 s4,[r4,#0x1d0]
  000895d4: vstr.32 s2,[r4,#0x1d4]
  000895d8: vstr.32 s6,[r4,#0x1d8]
  000895dc: vstr.32 s0,[r4,#0x1dc]
  000895e0: vst1.32 {d18,d19},[r0]
  000895e4: add.w r0,r1,#0x18
  000895e8: vst1.64 {d18,d19},[r0]
  000895ec: add.w r0,r1,#0x28
  000895f0: vst1.64 {d16,d17},[r0]
  000895f4: mov r0,r4
  000895f6: str.w r11,[sp,#0x0]
  000895fa: str.w r11,[sp,#0x14]
  000895fe: str.w r11,[sp,#0x38]
  00089602: blx 0x0006f55c
  00089606: add.w r3,r4,#0x1d0
  0008960a: movs r0,#0x2
  0008960c: movw r1,#0x1406
  00089610: movs r2,#0x0
  00089612: blx 0x0006ed1c
  00089616: ldr r1,[0x00089688]
  00089618: movs r2,#0x1
  0008961a: ldr r0,[r4,#0x34]
  0008961c: add r1,pc
  0008961e: ldr r5,[r1,#0x0]
  00089620: movw r1,#0x8074
  00089624: blx r5
  00089626: ldr r0,[r4,#0x34]
  00089628: movw r1,#0x8078
  0008962c: movs r2,#0x0
  0008962e: blx r5
  00089630: ldr r0,[r4,#0x34]
  00089632: movw r1,#0x8075
  00089636: movs r2,#0x0
  00089638: blx r5
  0008963a: ldr r0,[r4,#0x34]
  0008963c: movw r1,#0x8076
  00089640: movs r2,#0x0
  00089642: blx r5
  00089644: movs r0,#0x1
  00089646: movs r1,#0x0
  00089648: movs r2,#0x2
  0008964a: blx 0x0006fb20
  0008964e: ldr r0,[r4,#0x34]
  00089650: movw r1,#0xde1
  00089654: movs r2,#0x1
  00089656: blx 0x0006eddc
  0008965a: ldr r0,[sp,#0x40]
  0008965c: ldr.w r1,[r9,#0x0]
  00089660: subs r0,r1,r0
  00089662: ittt eq
  00089664: add.eq sp,#0x44
  00089666: pop.eq.w {r8,r9,r10,r11}
  0008966a: pop.eq {r4,r5,r6,r7,pc}
  0008966c: blx 0x0006e824
```
