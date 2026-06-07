# MovingStars::MovingStars
elf 0x146050 body 280
Sig: undefined __thiscall MovingStars(MovingStars * this)

## decompile
```c

/* MovingStars::MovingStars() */

void __thiscall MovingStars::MovingStars(MovingStars *this)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  code *pcVar5;
  int *piVar6;
  uint *puVar7;
  int *piVar8;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s3;
  Matrix aMStack_64 [60];
  int local_28;
  
  piVar1 = *(int **)(DAT_0015617c + 0x156068);
  pcVar5 = *(code **)(DAT_00156180 + 0x15606c);
  local_28 = *piVar1;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x10) = 0;
  uVar2 = (*pcVar5)(200);
  *(undefined4 *)this = uVar2;
  uVar2 = (*pcVar5)(200);
  *(undefined4 *)(this + 4) = uVar2;
  uVar2 = (*pcVar5)(200);
  *(undefined4 *)(this + 0x10) = uVar2;
  iVar3 = (*pcVar5)(200);
  *(int *)(this + 0xc) = iVar3;
  for (iVar4 = 0; iVar4 != 0x32; iVar4 = iVar4 + 1) {
    *(undefined4 *)(iVar3 + iVar4 * 4) = 0xffffffff;
  }
  piVar8 = *(int **)(DAT_00156184 + 0x1560ba);
  piVar6 = *(int **)(DAT_00156188 + 0x1560c2);
  puVar7 = *(uint **)(DAT_0015618c + 0x1560ca);
  for (iVar3 = 0; iVar3 != 200; iVar3 = iVar3 + 4) {
    AbyssEngine::AERandom::nextInt(*piVar8);
    uVar2 = Globals::createBillBoard
                      (*piVar6,0x46,extraout_s0,extraout_s1,extraout_s2,extraout_s3,500);
    *(undefined4 *)(*(int *)this + iVar3) = uVar2;
    AbyssEngine::PaintCanvas::TransformCreate
              ((PaintCanvas *)*puVar7,(uint *)(*(int *)(this + 4) + iVar3));
    AbyssEngine::PaintCanvas::TransformAddMeshId
              ((PaintCanvas *)*puVar7,*(uint *)(*(int *)(this + 4) + iVar3),
               *(uint *)(*(int *)this + iVar3));
    AbyssEngine::PaintCanvas::TransformGetLocal(*puVar7);
    AbyssEngine::AEMath::MatrixSetTranslation
              (aMStack_64,extraout_s0_00,extraout_s1_00,extraout_s2_00);
  }
  AbyssEngine::PaintCanvas::TextureCreate((ushort)*puVar7,(uint *)0x2711,(bool)((char)this + '\b'));
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined2 *)(this + 0x14) = 0;
  iVar3 = *piVar1 - local_28;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  00156050: push {r4,r5,r6,r7,lr}
  00156052: add r7,sp,#0xc
  00156054: push {r8,r9,r10,r11}
  00156058: sub sp,#0x5c
  0015605a: mov r4,r0
  0015605c: ldr r0,[0x0015617c]
  0015605e: ldr r1,[0x00156180]
  00156060: vmov.i32 q8,#0x0
  00156064: add r0,pc
  00156066: mov r6,r4
  00156068: add r1,pc
  0015606a: movs r5,#0x0
  0015606c: ldr r0,[r0,#0x0]
  0015606e: str r0,[sp,#0x18]
  00156070: ldr.w r8,[r1,#0x0]
  00156074: ldr r0,[r0,#0x0]
  00156076: str r0,[sp,#0x58]
  00156078: movs r0,#0xc8
  0015607a: vst1.32 {d16,d17},[r6]!
  0015607e: str r5,[r6,#0x0]
  00156080: blx r8
  00156082: str r0,[r4,#0x0]
  00156084: movs r0,#0xc8
  00156086: blx r8
  00156088: str r0,[r4,#0x4]
  0015608a: movs r0,#0xc8
  0015608c: blx r8
  0015608e: str r0,[r6,#0x0]
  00156090: movs r0,#0xc8
  00156092: blx r8
  00156094: add.w r1,r4,#0x8
  00156098: str r1,[sp,#0x14]
  0015609a: mov.w r1,#0xffffffff
  0015609e: str r0,[r4,#0xc]
  001560a0: b 0x001560a8
  001560a2: str.w r1,[r0,r5,lsl #0x2]
  001560a6: adds r5,#0x1
  001560a8: cmp r5,#0x32
  001560aa: bne 0x001560a2
  001560ac: ldr r0,[0x00156184]
  001560ae: add.w r10,sp,#0x1c
  001560b2: ldr r5,[0x00156178]
  001560b4: movs r6,#0x0
  001560b6: add r0,pc
  001560b8: ldr.w r11,[r0,#0x0]
  001560bc: ldr r0,[0x00156188]
  001560be: add r0,pc
  001560c0: ldr.w r8,[r0,#0x0]
  001560c4: ldr r0,[0x0015618c]
  001560c6: add r0,pc
  001560c8: ldr.w r9,[r0,#0x0]
  001560cc: b 0x00156136
  001560ce: ldr.w r0,[r11,#0x0]
  001560d2: movs r1,#0x4
  001560d4: blx 0x00071848
  001560d8: ldr r1,[0x0015616c]
  001560da: mov.w r2,#0x1f4
  001560de: str r1,[sp,#0x0]
  001560e0: ldr r1,[0x00156168]
  001560e2: str r1,[sp,#0x4]
  001560e4: ldr r1,[0x00156174]
  001560e6: ldr.w r0,[r8,#0x0]
  001560ea: ldr r3,[0x00156170]
  001560ec: str r1,[sp,#0x8]
  001560ee: movw r1,#0x4e54
  001560f2: str r1,[sp,#0xc]
  001560f4: movs r1,#0x46
  001560f6: blx 0x00076d74
  001560fa: ldr r1,[r4,#0x0]
  001560fc: str r0,[r1,r6]
  001560fe: ldr r1,[r4,#0x4]
  00156100: ldr.w r0,[r9,#0x0]
  00156104: add r1,r6
  00156106: blx 0x000720ac
  0015610a: ldrd r1,r3,[r4,#0x0]
  0015610e: ldr.w r0,[r9,#0x0]
  00156112: ldr r2,[r1,r6]
  00156114: ldr r1,[r3,r6]
  00156116: blx 0x00073030
  0015611a: ldr r1,[r4,#0x4]
  0015611c: ldr.w r0,[r9,#0x0]
  00156120: ldr r1,[r1,r6]
  00156122: blx 0x000720c4
  00156126: mov r1,r0
  00156128: mov r0,r10
  0015612a: mov r2,r5
  0015612c: mov r3,r5
  0015612e: str r5,[sp,#0x0]
  00156130: blx 0x0006f820
  00156134: adds r6,#0x4
  00156136: cmp r6,#0xc8
  00156138: bne 0x001560ce
  0015613a: ldr.w r0,[r9,#0x0]
  0015613e: movw r1,#0x2711
  00156142: ldr r2,[sp,#0x14]
  00156144: movs r3,#0x0
  00156146: movs r5,#0x0
  00156148: blx 0x0006fbb0
  0015614c: str r5,[r4,#0x18]
  0015614e: strh r5,[r4,#0x14]
  00156150: ldr r0,[sp,#0x58]
  00156152: ldr r1,[sp,#0x18]
  00156154: ldr r1,[r1,#0x0]
  00156156: subs r0,r1,r0
  00156158: itttt eq
  0015615a: mov.eq r0,r4
  0015615c: add.eq sp,#0x5c
  0015615e: pop.eq.w {r8,r9,r10,r11}
  00156162: pop.eq {r4,r5,r6,r7,pc}
  00156164: blx 0x0006e824
```
