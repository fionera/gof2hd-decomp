# MovingStars::update
elf 0x1461c8 body 1290
Sig: undefined __stdcall update(int param_1, Matrix param_2, bool param_3, float param_4)

## decompile
```c

/* MovingStars::update(int, AbyssEngine::AEMath::Matrix, bool, float) */

void MovingStars::update(int *param_1,int param_2,undefined4 param_3,undefined4 param_4,
                        undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
                        undefined4 param_9,undefined4 param_10,undefined4 param_11,
                        undefined4 param_12,undefined4 param_13,undefined4 param_14,
                        undefined4 param_15,undefined4 param_16,undefined4 param_17,int param_18,
                        float param_19)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  code *pcVar6;
  byte bVar7;
  int iVar8;
  uint *puVar9;
  uint in_fpscr;
  float extraout_s1;
  float extraout_s2;
  float fVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  Matrix aMStack_d8 [8];
  float local_d0;
  float local_cc;
  float local_c0;
  float local_bc;
  float local_b0;
  float local_ac;
  undefined4 local_9c;
  undefined4 local_98;
  float local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined4 uStack_60;
  undefined4 local_5c;
  undefined4 uStack_58;
  int local_54;
  
  piVar1 = *(int **)(DAT_0015655c + 0x1561e6);
  local_54 = *piVar1;
  local_88 = param_5;
  local_84 = param_6;
  local_80 = param_7;
  local_7c = param_8;
  local_78 = param_9;
  local_74 = param_10;
  local_70 = param_11;
  uStack_6c = param_12;
  uStack_68 = param_13;
  local_64 = param_14;
  uStack_60 = param_15;
  local_5c = param_16;
  uStack_58 = param_17;
  param_1[6] = param_1[6] + param_2;
  local_90 = param_3;
  local_8c = param_4;
  if (param_18 == 0) {
    *(undefined1 *)(param_1 + 5) = 0;
    if (*(char *)((int)param_1 + 0x15) != '\0') {
      *(undefined1 *)((int)param_1 + 0x15) = 0;
      puVar5 = *(undefined4 **)(DAT_0015670c + 0x156408);
      pcVar6 = *(code **)(DAT_00156710 + 0x156410);
      for (iVar8 = 0; uVar12 = DAT_001566f0, uVar11 = DAT_001566ec, iVar8 != 200; iVar8 = iVar8 + 4)
      {
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),0,DAT_001566f0,0,DAT_001566ec);
        uVar13 = DAT_001566f4;
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),1,uVar12,0,DAT_001566f4);
        uVar12 = DAT_001566f8;
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),2,DAT_001566f8,0,uVar11);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),3,uVar12,0,uVar13);
        uVar11 = DAT_001566f0;
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),4,0,DAT_001566f0,DAT_001566ec);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),5,0,uVar11,uVar13);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),6,0,uVar12,DAT_001566ec);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),7,0,uVar12,uVar13);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),8,uVar11,uVar12,0);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),9,uVar11,uVar11,0);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),10,uVar12,uVar12,0);
        (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),0xb,uVar12,uVar11,0);
      }
    }
  }
  else {
    iVar8 = 0;
    fVar10 = param_19 * DAT_00156568;
    uVar11 = VectorSignedToFloat(DAT_00156564 - (int)(param_19 * DAT_00156560),
                                 (byte)(in_fpscr >> 0x16) & 3);
    uVar12 = VectorSignedToFloat((int)(param_19 * DAT_00156560) + 500,(byte)(in_fpscr >> 0x16) & 3);
    uVar13 = VectorSignedToFloat((int)(param_19 * -20.0) + 0x46,(byte)(in_fpscr >> 0x16) & 3);
    uVar14 = VectorSignedToFloat(-0x46 - (int)(param_19 * -20.0),(byte)(in_fpscr >> 0x16) & 3);
    puVar5 = *(undefined4 **)(DAT_0015656c + 0x1562c4);
    pcVar6 = *(code **)(DAT_00156570 + 0x1562c6);
    for (; iVar8 != 200; iVar8 = iVar8 + 4) {
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),0,uVar14,0,uVar12);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),1,uVar14,0,uVar11);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),2,uVar13,0,uVar12);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),3,uVar13,0,uVar11);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),4,0,uVar14,uVar12);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),5,0,uVar14,uVar11);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),6,0,uVar13,uVar12);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),7,0,uVar13,uVar11);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),8,uVar14,uVar13,0);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),9,uVar14,uVar14,0);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),10,uVar13,uVar13,0);
      (*pcVar6)(*puVar5,*(undefined4 *)(*param_1 + iVar8),0xb,uVar13,uVar14,0);
      *(int *)(param_1[4] + iVar8) = (int)fVar10 + 1000;
    }
    *(undefined2 *)(param_1 + 5) = 0x101;
  }
  param_19 = param_19 * DAT_001566fc;
  bVar7 = 0;
  puVar9 = *(uint **)(DAT_00156704 + 0x15654e);
  piVar2 = *(int **)(DAT_00156708 + 0x156556);
  for (iVar8 = 0; iVar8 != 0x32; iVar8 = iVar8 + 1) {
    iVar4 = *(int *)(param_1[3] + iVar8 * 4);
    if (((bool)(0 < iVar4 | bVar7)) || ((param_18 == 0 && (param_1[6] < 0x29)))) {
      *(int *)(param_1[3] + iVar8 * 4) = iVar4 - param_2;
      uVar11 = AbyssEngine::PaintCanvas::TransformGetLocal(*puVar9);
      __aeabi_memcpy(aMStack_d8,uVar11,0x3c);
      fVar10 = (float)VectorSignedToFloat(*(undefined4 *)(param_1[4] + iVar8 * 4),
                                          (byte)(in_fpscr >> 0x16) & 3);
      local_ac = local_ac - local_b0 * fVar10;
      local_bc = local_bc - local_c0 * fVar10;
      local_cc = local_cc - local_d0 * fVar10;
      AbyssEngine::PaintCanvas::TransformSetLocal(*puVar9,*(Matrix **)(param_1[1] + iVar8 * 4));
    }
    else {
      param_1[6] = 0;
      iVar4 = AbyssEngine::AERandom::nextInt(*piVar2);
      iVar3 = AbyssEngine::AERandom::nextInt(*piVar2);
      local_9c = VectorSignedToFloat(iVar4 + -10000,(byte)(in_fpscr >> 0x16) & 3);
      local_98 = VectorSignedToFloat(iVar3 + -9000,(byte)(in_fpscr >> 0x16) & 3);
      local_94 = DAT_00156700;
      AbyssEngine::AEMath::MatrixTransformVector(aMStack_d8,(Vector *)&local_90);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_9c,(Vector *)aMStack_d8);
      AbyssEngine::PaintCanvas::TransformSetLocal(*puVar9,*(Matrix **)(param_1[1] + iVar8 * 4));
      AbyssEngine::PaintCanvas::TransformGetLocal(*puVar9);
      AbyssEngine::AEMath::MatrixSetTranslation(aMStack_d8,local_94,extraout_s1,extraout_s2);
      if (param_18 == 0) {
        iVar4 = AbyssEngine::AERandom::nextInt(*piVar2);
        iVar4 = iVar4 + 500;
        uVar11 = 2000;
      }
      else {
        uVar11 = 500;
        iVar4 = (int)param_19 + 1000;
      }
      bVar7 = 1;
      *(int *)(param_1[4] + iVar8 * 4) = iVar4;
      *(undefined4 *)(param_1[3] + iVar8 * 4) = uVar11;
    }
  }
  if (*piVar1 != local_54) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001561c8: push {r4,r5,r6,r7,lr}
  001561ca: add r7,sp,#0xc
  001561cc: push {r7,r8,r9,r10,r11}
  001561d0: vpush {d8,d9,d10,d11,d12}
  001561d4: sub sp,#0xa0
  001561d6: mov r5,r0
  001561d8: ldr r0,[0x0015655c]
  001561da: add.w lr,r7,#0x10
  001561de: ldr.w r9,[r7,#0x24]
  001561e2: add r0,pc
  001561e4: ldr.w r12,[r7,#0x1c]
  001561e8: ldm.w lr,{r6,r10,lr}
  001561ec: ldr r0,[r0,#0x0]
  001561ee: ldrd r4,r11,[r7,#0x8]
  001561f2: str r2,[sp,#0x60]
  001561f4: str r0,[sp,#0x8]
  001561f6: ldr r2,[r0,#0x0]
  001561f8: str r3,[sp,#0x64]
  001561fa: str r4,[sp,#0x68]
  001561fc: str.w r11,[sp,#0x6c]
  00156200: str r6,[sp,#0x70]
  00156202: ldr.w r11,[r7,#0x28]
  00156206: ldr r6,[r7,#0x30]
  00156208: ldr r4,[r7,#0x38]
  0015620a: ldr.w r8,[r7,#0x20]
  0015620e: str.w r10,[sp,#0x74]
  00156212: str.w lr,[sp,#0x78]
  00156216: str.w r12,[sp,#0x7c]
  0015621a: add.w r12,sp,#0x80
  0015621e: ldr r0,[r7,#0x2c]
  00156220: ldr r3,[r7,#0x34]
  00156222: stm r12,{r8,r9,r11}
  00156226: strd r0,r6,[sp,#0x8c]
  0015622a: strd r3,r4,[sp,#0x94]
  0015622e: str r2,[sp,#0x9c]
  00156230: ldr r2,[r7,#0x3c]
  00156232: ldr r0,[r5,#0x18]
  00156234: vldr.32 s16,[r7,#0x40]
  00156238: cmp r2,#0x0
  0015623a: add r0,r1
  0015623c: str r1,[sp,#0x14]
  0015623e: str r0,[r5,#0x18]
  00156240: beq.w 0x001563f0
  00156244: vldr.32 s2,[pc,#0x318]
  00156248: vmov.f32 s0,0xc1a00000
  0015624c: ldr r0,[0x00156564]
  0015624e: mov.w r10,#0x0
  00156252: vmul.f32 s2,s16,s2
  00156256: vldr.32 s4,[pc,#0x310]
  0015625a: mov.w r9,#0x0
  0015625e: vmul.f32 s4,s16,s4
  00156262: vmul.f32 s0,s16,s0
  00156266: vcvt.s32.f32 s2,s2
  0015626a: vmov r1,s2
  0015626e: subs r0,r0,r1
  00156270: add.w r2,r1,#0x1f4
  00156274: vmov s2,r0
  00156278: mvn r0,#0x45
  0015627c: vcvt.f32.s32 s18,s2
  00156280: vcvt.s32.f32 s0,s0
  00156284: vmov s2,r2
  00156288: vcvt.f32.s32 s20,s2
  0015628c: vmov r1,s0
  00156290: add.w r2,r1,#0x46
  00156294: subs r0,r0,r1
  00156296: vmov s0,r2
  0015629a: vcvt.f32.s32 s22,s0
  0015629e: vmov s0,r0
  001562a2: vcvt.f32.s32 s24,s0
  001562a6: vcvt.s32.f32 s0,s4
  001562aa: vmov r6,s22
  001562ae: vmov r11,s24
  001562b2: vmov r0,s0
  001562b6: add.w r0,r0,#0x3e8
  001562ba: str r0,[sp,#0x10]
  001562bc: ldr r0,[0x0015656c]
  001562be: ldr r1,[0x00156570]
  001562c0: add r0,pc
  001562c2: add r1,pc
  001562c4: ldr r4,[r0,#0x0]
  001562c6: ldr.w r8,[r1,#0x0]
  001562ca: b 0x001563e0
  001562cc: ldr r0,[r5,#0x0]
  001562ce: movs r2,#0x0
  001562d0: mov r3,r11
  001562d2: ldr.w r1,[r0,r9]
  001562d6: ldr r0,[r4,#0x0]
  001562d8: vstr.32 s20,[sp,#0x4]
  001562dc: str.w r10,[sp,#0x0]
  001562e0: blx r8
  001562e2: ldr r0,[r5,#0x0]
  001562e4: movs r2,#0x1
  001562e6: mov r3,r11
  001562e8: ldr.w r1,[r0,r9]
  001562ec: ldr r0,[r4,#0x0]
  001562ee: vstr.32 s18,[sp,#0x4]
  001562f2: str.w r10,[sp,#0x0]
  001562f6: blx r8
  001562f8: ldr r0,[r5,#0x0]
  001562fa: movs r2,#0x2
  001562fc: mov r3,r6
  001562fe: ldr.w r1,[r0,r9]
  00156302: ldr r0,[r4,#0x0]
  00156304: vstr.32 s20,[sp,#0x4]
  00156308: str.w r10,[sp,#0x0]
  0015630c: blx r8
  0015630e: ldr r0,[r5,#0x0]
  00156310: movs r2,#0x3
  00156312: mov r3,r6
  00156314: ldr.w r1,[r0,r9]
  00156318: ldr r0,[r4,#0x0]
  0015631a: vstr.32 s18,[sp,#0x4]
  0015631e: str.w r10,[sp,#0x0]
  00156322: blx r8
  00156324: ldr r0,[r5,#0x0]
  00156326: movs r2,#0x4
  00156328: movs r3,#0x0
  0015632a: ldr.w r1,[r0,r9]
  0015632e: ldr r0,[r4,#0x0]
  00156330: vstr.32 s24,[sp]
  00156334: vstr.32 s20,[sp,#0x4]
  00156338: blx r8
  0015633a: ldr r0,[r5,#0x0]
  0015633c: movs r2,#0x5
  0015633e: movs r3,#0x0
  00156340: ldr.w r1,[r0,r9]
  00156344: ldr r0,[r4,#0x0]
  00156346: vstr.32 s24,[sp]
  0015634a: vstr.32 s18,[sp,#0x4]
  0015634e: blx r8
  00156350: ldr r0,[r5,#0x0]
  00156352: movs r2,#0x6
  00156354: movs r3,#0x0
  00156356: ldr.w r1,[r0,r9]
  0015635a: ldr r0,[r4,#0x0]
  0015635c: vstr.32 s22,[sp]
  00156360: vstr.32 s20,[sp,#0x4]
  00156364: blx r8
  00156366: ldr r0,[r5,#0x0]
  00156368: movs r2,#0x7
  0015636a: movs r3,#0x0
  0015636c: ldr.w r1,[r0,r9]
  00156370: ldr r0,[r4,#0x0]
  00156372: vstr.32 s22,[sp]
  00156376: vstr.32 s18,[sp,#0x4]
  0015637a: blx r8
  0015637c: ldr r0,[r5,#0x0]
  0015637e: movs r2,#0x8
  00156380: mov r3,r11
  00156382: ldr.w r1,[r0,r9]
  00156386: ldr r0,[r4,#0x0]
  00156388: str.w r10,[sp,#0x4]
  0015638c: vstr.32 s22,[sp]
  00156390: blx r8
  00156392: ldr r0,[r5,#0x0]
  00156394: movs r2,#0x9
  00156396: mov r3,r11
  00156398: ldr.w r1,[r0,r9]
  0015639c: ldr r0,[r4,#0x0]
  0015639e: str.w r10,[sp,#0x4]
  001563a2: vstr.32 s24,[sp]
  001563a6: blx r8
  001563a8: ldr r0,[r5,#0x0]
  001563aa: movs r2,#0xa
  001563ac: mov r3,r6
  001563ae: ldr.w r1,[r0,r9]
  001563b2: ldr r0,[r4,#0x0]
  001563b4: str.w r10,[sp,#0x4]
  001563b8: vstr.32 s22,[sp]
  001563bc: blx r8
  001563be: ldr r1,[r5,#0x0]
  001563c0: movs r2,#0xb
  001563c2: ldr r0,[r4,#0x0]
  001563c4: mov r3,r6
  001563c6: ldr.w r1,[r1,r9]
  001563ca: str.w r10,[sp,#0x4]
  001563ce: vstr.32 s24,[sp]
  001563d2: blx r8
  001563d4: ldr r0,[r5,#0x10]
  001563d6: ldr r1,[sp,#0x10]
  001563d8: str.w r1,[r0,r9]
  001563dc: add.w r9,r9,#0x4
  001563e0: cmp.w r9,#0xc8
  001563e4: bne.w 0x001562cc
  001563e8: movw r0,#0x101
  001563ec: strh r0,[r5,#0x14]
  001563ee: b 0x00156526
  001563f0: ldrb r0,[r5,#0x15]
  001563f2: movs r2,#0x0
  001563f4: movs r1,#0x0
  001563f6: strb r2,[r5,#0x14]
  001563f8: cmp r0,#0x0
  001563fa: beq.w 0x00156526
  001563fe: strb r1,[r5,#0x15]
  00156400: movs r4,#0x0
  00156402: ldr r0,[0x0015670c]
  00156404: add r0,pc
  00156406: ldr.w r8,[r0,#0x0]
  0015640a: ldr r0,[0x00156710]
  0015640c: add r0,pc
  0015640e: ldr r6,[r0,#0x0]
  00156410: b 0x00156520
  00156412: ldr r0,[r5,#0x0]
  00156414: movs r2,#0x0
  00156416: ldr r3,[0x001566f0]
  00156418: mov.w r9,#0x0
  0015641c: ldr r1,[r0,r4]
  0015641e: str r2,[sp,#0x0]
  00156420: ldr r2,[0x001566ec]
  00156422: mov r10,r3
  00156424: ldr.w r0,[r8,#0x0]
  00156428: str r2,[sp,#0x4]
  0015642a: mov r11,r2
  0015642c: movs r2,#0x0
  0015642e: blx r6
  00156430: ldr r0,[r5,#0x0]
  00156432: mov r3,r10
  00156434: ldr r2,[0x001566f4]
  00156436: ldr r1,[r0,r4]
  00156438: ldr.w r0,[r8,#0x0]
  0015643c: str.w r9,[sp,#0x0]
  00156440: mov r9,r2
  00156442: str r2,[sp,#0x4]
  00156444: movs r2,#0x1
  00156446: blx r6
  00156448: ldr r0,[r5,#0x0]
  0015644a: movs r2,#0x0
  0015644c: ldr r3,[0x001566f8]
  0015644e: mov.w r10,#0x0
  00156452: ldr r1,[r0,r4]
  00156454: ldr.w r0,[r8,#0x0]
  00156458: str r2,[sp,#0x0]
  0015645a: movs r2,#0x2
  0015645c: str.w r11,[sp,#0x4]
  00156460: mov r11,r3
  00156462: blx r6
  00156464: ldr r0,[r5,#0x0]
  00156466: movs r2,#0x3
  00156468: mov r3,r11
  0015646a: ldr r1,[r0,r4]
  0015646c: ldr.w r0,[r8,#0x0]
  00156470: strd r10,r9,[sp,#0x0]
  00156474: blx r6
  00156476: ldr r0,[r5,#0x0]
  00156478: movs r3,#0x0
  0015647a: ldr r2,[0x001566f0]
  0015647c: ldr r1,[r0,r4]
  0015647e: ldr.w r0,[r8,#0x0]
  00156482: mov r10,r2
  00156484: str r2,[sp,#0x0]
  00156486: ldr r2,[0x001566ec]
  00156488: str r2,[sp,#0x4]
  0015648a: movs r2,#0x4
  0015648c: blx r6
  0015648e: ldr r0,[r5,#0x0]
  00156490: movs r2,#0x5
  00156492: movs r3,#0x0
  00156494: ldr r1,[r0,r4]
  00156496: ldr.w r0,[r8,#0x0]
  0015649a: strd r10,r9,[sp,#0x0]
  0015649e: blx r6
  001564a0: ldr r0,[r5,#0x0]
  001564a2: movs r3,#0x0
  001564a4: ldr r2,[0x001566ec]
  001564a6: ldr r1,[r0,r4]
  001564a8: ldr.w r0,[r8,#0x0]
  001564ac: str r2,[sp,#0x4]
  001564ae: movs r2,#0x6
  001564b0: str.w r11,[sp,#0x0]
  001564b4: blx r6
  001564b6: ldr r0,[r5,#0x0]
  001564b8: movs r2,#0x7
  001564ba: movs r3,#0x0
  001564bc: ldr r1,[r0,r4]
  001564be: ldr.w r0,[r8,#0x0]
  001564c2: strd r11,r9,[sp,#0x0]
  001564c6: blx r6
  001564c8: ldr r0,[r5,#0x0]
  001564ca: mov.w r9,#0x0
  001564ce: movs r2,#0x8
  001564d0: mov r3,r10
  001564d2: ldr r1,[r0,r4]
  001564d4: ldr.w r0,[r8,#0x0]
  001564d8: str.w r11,[sp,#0x0]
  001564dc: str.w r9,[sp,#0x4]
  001564e0: blx r6
  001564e2: ldr r0,[r5,#0x0]
  001564e4: mov r3,r10
  001564e6: movs r2,#0x9
  001564e8: ldr r1,[r0,r4]
  001564ea: ldr.w r0,[r8,#0x0]
  001564ee: strd r10,r9,[sp,#0x0]
  001564f2: mov.w r10,#0x0
  001564f6: mov r9,r3
  001564f8: blx r6
  001564fa: ldr r0,[r5,#0x0]
  001564fc: movs r2,#0xa
  001564fe: mov r3,r11
  00156500: ldr r1,[r0,r4]
  00156502: ldr.w r0,[r8,#0x0]
  00156506: strd r11,r10,[sp,#0x0]
  0015650a: blx r6
  0015650c: ldr r1,[r5,#0x0]
  0015650e: movs r2,#0xb
  00156510: ldr.w r0,[r8,#0x0]
  00156514: mov r3,r11
  00156516: ldr r1,[r1,r4]
  00156518: strd r9,r10,[sp,#0x0]
  0015651c: blx r6
  0015651e: adds r4,#0x4
  00156520: cmp r4,#0xc8
  00156522: bne.w 0x00156412
  00156526: vldr.32 s0,[pc,#0x1d4]
  0015652a: add.w r10,sp,#0x18
  0015652e: ldr r6,[r7,#0x3c]
  00156530: movs r4,#0x0
  00156532: vmul.f32 s0,s16,s0
  00156536: mov.w r9,#0x0
  0015653a: vcvt.s32.f32 s0,s0
  0015653e: vmov r0,s0
  00156542: add.w r0,r0,#0x3e8
  00156546: str r0,[sp,#0xc]
  00156548: ldr r0,[0x00156704]
  0015654a: add r0,pc
  0015654c: ldr.w r11,[r0,#0x0]
  00156550: ldr r0,[0x00156708]
  00156552: add r0,pc
  00156554: ldr r0,[r0,#0x0]
  00156556: str r0,[sp,#0x10]
  00156558: b 0x001566c8
  00156574: ldr r0,[r5,#0xc]
  00156576: movs r2,#0x0
  00156578: ldr.w r1,[r0,r4,lsl #0x2]
  0015657c: cmp r1,#0x0
  0015657e: it gt
  00156580: mov.gt r2,#0x1
  00156582: orr.w r2,r2,r9
  00156586: lsls r2,r2,#0x1f
  00156588: bne 0x00156632
  0015658a: cbnz r6,0x00156592
  0015658c: ldr r2,[r5,#0x18]
  0015658e: cmp r2,#0x29
  00156590: blt 0x00156632
  00156592: ldr.w r9,[sp,#0x10]
  00156596: movs r0,#0x0
  00156598: str r0,[r5,#0x18]
  0015659a: movw r1,#0x4e20
  0015659e: ldr.w r0,[r9,#0x0]
  001565a2: blx 0x00071848
  001565a6: ldr.w r1,[r9,#0x0]
  001565aa: movw r2,#0x2710
  001565ae: subs r6,r0,r2
  001565b0: mov r0,r1
  001565b2: movw r1,#0x4650
  001565b6: blx 0x00071848
  001565ba: movw r1,#0x2328
  001565be: subs r0,r0,r1
  001565c0: vmov s0,r6
  001565c4: add r6,sp,#0x60
  001565c6: vmov s2,r0
  001565ca: add.w r8,sp,#0x54
  001565ce: vcvt.f32.s32 s0,s0
  001565d2: ldr r0,[0x00156700]
  001565d4: vcvt.f32.s32 s2,s2
  001565d8: str r0,[sp,#0x5c]
  001565da: mov r0,r10
  001565dc: mov r1,r6
  001565de: mov r2,r8
  001565e0: vstr.32 s0,[sp,#0x54]
  001565e4: vstr.32 s2,[sp,#0x58]
  001565e8: blx 0x0006f688
  001565ec: mov r0,r8
  001565ee: mov r1,r10
  001565f0: blx 0x0006eb3c
  001565f4: ldr r1,[r5,#0x4]
  001565f6: mov r2,r6
  001565f8: ldr.w r0,[r11,#0x0]
  001565fc: ldr r6,[r7,#0x3c]
  001565fe: ldr.w r1,[r1,r4,lsl #0x2]
  00156602: blx 0x000721c0
  00156606: ldr r1,[r5,#0x4]
  00156608: ldr.w r0,[r11,#0x0]
  0015660c: ldr.w r1,[r1,r4,lsl #0x2]
  00156610: blx 0x000720c4
  00156614: ldrd r2,r3,[sp,#0x54]
  00156618: mov r1,r0
  0015661a: vldr.32 s0,[sp,#0x5c]
  0015661e: mov r0,r10
  00156620: vstr.32 s0,[sp]
  00156624: blx 0x0006f820
  00156628: cbz r6,0x001566a2
  0015662a: mov.w r0,#0x1f4
  0015662e: ldr r1,[sp,#0xc]
  00156630: b 0x001566b6
  00156632: ldr r2,[sp,#0x14]
  00156634: subs r1,r1,r2
  00156636: str.w r1,[r0,r4,lsl #0x2]
  0015663a: ldr r0,[r5,#0x4]
  0015663c: ldr.w r1,[r0,r4,lsl #0x2]
  00156640: ldr.w r0,[r11,#0x0]
  00156644: blx 0x000720c4
  00156648: mov r1,r0
  0015664a: mov r0,r10
  0015664c: movs r2,#0x3c
  0015664e: blx 0x0006f1e4
  00156652: ldr r0,[r5,#0x10]
  00156654: mov r2,r10
  00156656: add.w r0,r0,r4, lsl #0x2
  0015665a: vldr.32 s0,[r0]
  0015665e: vcvt.f32.s32 s0,s0
  00156662: vldr.32 s2,[sp,#0x40]
  00156666: vldr.32 s4,[sp,#0x20]
  0015666a: vldr.32 s8,[sp,#0x30]
  0015666e: vldr.32 s6,[sp,#0x24]
  00156672: vldr.32 s10,[sp,#0x34]
  00156676: vldr.32 s12,[sp,#0x44]
  0015667a: ldr.w r0,[r11,#0x0]
  0015667e: vmls.f32 s12,s2,s0
  00156682: vmls.f32 s10,s8,s0
  00156686: vmls.f32 s6,s4,s0
  0015668a: vstr.32 s12,[sp,#0x44]
  0015668e: vstr.32 s10,[sp,#0x34]
  00156692: vstr.32 s6,[sp,#0x24]
  00156696: ldr r1,[r5,#0x4]
  00156698: ldr.w r1,[r1,r4,lsl #0x2]
  0015669c: blx 0x000721c0
  001566a0: b 0x001566c6
  001566a2: ldr.w r0,[r9,#0x0]
  001566a6: mov.w r1,#0x1f4
  001566aa: blx 0x00071848
  001566ae: add.w r1,r0,#0x1f4
  001566b2: mov.w r0,#0x7d0
  001566b6: ldr r2,[r5,#0x10]
  001566b8: mov.w r9,#0x1
  001566bc: str.w r1,[r2,r4,lsl #0x2]
  001566c0: ldr r1,[r5,#0xc]
  001566c2: str.w r0,[r1,r4,lsl #0x2]
  001566c6: adds r4,#0x1
  001566c8: cmp r4,#0x32
  001566ca: bne.w 0x00156574
  001566ce: ldr r0,[sp,#0x9c]
  001566d0: ldr r1,[sp,#0x8]
  001566d2: ldr r1,[r1,#0x0]
  001566d4: subs r0,r1,r0
  001566d6: itttt eq
  001566d8: add.eq sp,#0xa0
  001566da: vpop.eq {d8,d9,d10,d11,d12}
  001566de: add.eq sp,#0x4
  001566e0: pop.eq.w {r8,r9,r10,r11}
  001566e4: it eq
  001566e6: pop.eq {r4,r5,r6,r7,pc}
  001566e8: blx 0x0006e824
```
