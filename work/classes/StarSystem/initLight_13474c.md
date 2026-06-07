# StarSystem::initLight
elf 0x13474c body 1356
Sig: undefined __stdcall initLight(void)

## decompile
```c

/* StarSystem::initLight() */

void StarSystem::initLight(void)

{
  bool bVar1;
  float *in_r0;
  int iVar2;
  Engine *pEVar3;
  uint uVar4;
  uint uVar5;
  Station *this;
  char cVar6;
  int iVar7;
  float *pfVar8;
  float fVar9;
  float fVar10;
  uint *puVar11;
  undefined4 *puVar12;
  code *pcVar13;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  float extraout_s0_06;
  float extraout_s0_07;
  float extraout_s0_08;
  float extraout_s0_09;
  undefined4 uVar14;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float extraout_s1_07;
  float extraout_s1_08;
  float extraout_s1_09;
  float extraout_s1_10;
  float extraout_s1_11;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  float extraout_s2_07;
  float extraout_s2_08;
  float fVar15;
  
  puVar12 = *(undefined4 **)(DAT_00144850 + 0x144760);
  iVar2 = AbyssEngine::ApplicationManager::GetEngine();
  *(undefined4 *)(iVar2 + 0x32c) = 2;
  if (*(char *)(in_r0 + 10) == '\0') {
    Status::getSystem();
    iVar2 = SolarSystem::getTextureIndex();
    iVar2 = iVar2 * 3;
    bVar1 = *(char *)(in_r0 + 10) == '\0';
  }
  else {
    iVar2 = 0x1e;
    bVar1 = false;
  }
  fVar15 = DAT_0014485c;
  iVar7 = DAT_00144858 + 0x1447aa;
  fVar9 = *(float *)(iVar7 + (iVar2 + 1) * 4);
  fVar10 = *(float *)(iVar7 + (iVar2 + 2) * 4);
  *in_r0 = *(float *)(iVar7 + iVar2 * 4);
  in_r0[1] = fVar9;
  in_r0[2] = fVar10;
  if (bVar1) {
    Status::getSystem();
    iVar7 = SolarSystem::getTextureIndex();
    if ((iVar7 == 0xf) &&
       ((iVar7 = Status::getCurrentCampaignMission(), iVar7 == 0x59 ||
        (iVar7 = Status::getCurrentCampaignMission(), 0x9d < iVar7)))) {
      *in_r0 = *in_r0 * 0.5;
      in_r0[1] = in_r0[1] * 0.5;
      in_r0[2] = in_r0[2] * 0.5;
    }
    cVar6 = *(char *)(in_r0 + 10);
    if (cVar6 == '\0') {
      Status::getSystem();
      iVar7 = SolarSystem::getTextureIndex();
      fVar15 = DAT_0014485c;
      if ((iVar7 == 0xf) && (iVar7 = Status::getCurrentCampaignMission(), iVar7 != 0x59)) {
        iVar7 = Status::getCurrentCampaignMission();
        pfVar8 = (float *)&DAT_00144cc8;
        if (iVar7 < 0x9e) {
          pfVar8 = (float *)&DAT_00144ccc;
        }
        fVar15 = *pfVar8;
      }
      cVar6 = *(char *)(in_r0 + 10);
      if (cVar6 == '\0') {
        this = (Station *)Status::getStation();
        iVar7 = Station::getTextureIndex(this);
        cVar6 = *(char *)(in_r0 + 10);
        iVar7 = iVar7 * 3;
        goto LAB_00144868;
      }
    }
    iVar7 = 0x17;
  }
  else {
    iVar7 = 0x17;
    cVar6 = '\x01';
  }
LAB_00144868:
  pfVar8 = (float *)(DAT_00144bec + 0x144874 + iVar7 * 4);
  fVar9 = *pfVar8;
  fVar10 = pfVar8[2];
  if (cVar6 == '\0') {
    Status::getSystem();
    iVar7 = SolarSystem::getTextureIndex();
    if ((iVar7 == 0xf) && (iVar7 = Status::getCurrentCampaignMission(), iVar7 != 0x59)) {
      Status::getCurrentCampaignMission();
    }
  }
  pcVar13 = *(code **)(DAT_00144bf4 + 0x1448c2);
  (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetGlobalSceneColorAmbient
            (fVar15 * *in_r0,extraout_s1,fVar15 * in_r0[1]);
  pEVar3 = (Engine *)(*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetRimColor
            (pEVar3,*(float *)(DAT_00144cc4 + 0x14490a + (iVar2 + 2) * 4) * 3.0,extraout_s1_00,
             *(float *)(DAT_00144cc4 + 0x14490a + iVar2 * 4) * 3.0);
  (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetMaterialColorAmbient(extraout_s0,extraout_s1_01,extraout_s2);
  (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetMaterialColorDiffuse(extraout_s0_00,extraout_s1_02,extraout_s2_00);
  (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetMaterialColorSpecular(extraout_s0_01,extraout_s1_03,extraout_s2_01);
  pEVar3 = (Engine *)(*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetMaterialColorShininess(pEVar3,extraout_s0_02);
  uVar4 = (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetLightDirection(extraout_s0_03,extraout_s1_04,extraout_s2_02,uVar4);
  uVar4 = (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetLightColorAmbient(extraout_s0_04,extraout_s1_05,extraout_s2_03,uVar4)
  ;
  in_fpscr = in_fpscr & 0xfffffff;
  uVar4 = (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetLightColorDiffuse(extraout_s0_05,extraout_s1_06,extraout_s2_04,uVar4)
  ;
  uVar4 = (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetLightColorSpecular
            (extraout_s0_06,extraout_s1_07,extraout_s2_05,uVar4);
  uVar4 = (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetLightDirection(extraout_s0_07,extraout_s1_08,extraout_s2_06,uVar4);
  uVar4 = (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetLightColorDiffuse(fVar10 * 1.5,extraout_s1_09,fVar9 * 1.5,uVar4);
  uVar4 = (*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetLightColorSpecular
            (extraout_s0_08,extraout_s1_10,extraout_s2_07,uVar4);
  pEVar3 = (Engine *)(*pcVar13)(*puVar12);
  AbyssEngine::Engine::LightSetParticleAmbient(pEVar3,extraout_s0_09,extraout_s1_11,extraout_s2_08);
  puVar11 = *(uint **)(DAT_00144d08 + 0x144b38);
  AbyssEngine::PaintCanvas::FogEnable(*puVar11,0,1);
  *(undefined1 *)(in_r0 + 0x15) = 0;
  if (*(char *)(in_r0 + 10) != '\0') {
    return;
  }
  Status::getSystem();
  uVar5 = SolarSystem::getTextureIndex();
  uVar4 = DAT_00144cdc;
  switch(uVar5) {
  case 0xb:
    break;
  case 0xc:
    uVar4 = DAT_00144cf4;
    break;
  default:
    goto switchD_00144b6a_caseD_d;
  case 0xf:
    iVar2 = Status::getCurrentCampaignMission();
    uVar14 = DAT_00144ce4;
    if (iVar2 == 0x59) {
      return;
    }
    AbyssEngine::PaintCanvas::FogSetParameter
              ((PaintCanvas *)*puVar11,0x2601,0,DAT_00144ce0,0x3f800000,DAT_00144ce4);
    AbyssEngine::PaintCanvas::FogEnable(*puVar11,1,1);
    *(undefined1 *)(in_r0 + 0x15) = 1;
    for (uVar4 = 1; uVar4 < *(uint *)in_r0[7]; uVar4 = uVar4 + 1) {
      iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar11);
      *(undefined4 *)(iVar2 + 0x48) = uVar14;
    }
    return;
  case 0x10:
    uVar4 = DAT_00144cf0;
    break;
  case 0x11:
    uVar4 = DAT_00144cec;
    break;
  case 0x12:
    uVar4 = DAT_00144ce8;
  }
  puVar12 = &DAT_00144cf8;
  if ((uVar5 | 1) == 0x11) {
    puVar12 = &DAT_00144cfc;
  }
  uVar14 = *puVar12;
  if (uVar5 == 0x12) {
    uVar14 = DAT_00144d00;
  }
  AbyssEngine::PaintCanvas::FogSetParameter
            ((PaintCanvas *)*puVar11,0x2601,0,uVar14,0x3f800000,uVar4);
  uVar5 = 1;
  AbyssEngine::PaintCanvas::FogEnable(*puVar11,1,1);
  fVar15 = (float)VectorSignedToFloat((uVar4 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
  fVar9 = (float)VectorSignedToFloat(uVar4 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
  fVar10 = (float)VectorSignedToFloat((uVar4 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
  fVar15 = fVar15 * DAT_00144d04;
  fVar9 = fVar9 * DAT_00144d04;
  fVar10 = fVar10 * DAT_00144d04;
  *(undefined1 *)(in_r0 + 0x15) = 1;
  for (; uVar5 < *(uint *)in_r0[7]; uVar5 = uVar5 + 1) {
    iVar2 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar11);
    *(uint *)(iVar2 + 0x48) = (int)fVar9 << 0x18 | (int)fVar10 << 0x10 | (int)fVar15 << 8 | 0xff;
  }
switchD_00144b6a_caseD_d:
  return;
}

```

## target disasm
```
  0014474c: push {r4,r5,r6,r7,lr}
  0014474e: add r7,sp,#0xc
  00144750: push {r7,r8,r9,r10,r11}
  00144754: vpush {d7,d8,d9,d10,d11,d12,d13,d14,d15}
  00144758: mov r8,r0
  0014475a: ldr r0,[0x00144850]
  0014475c: add r0,pc
  0014475e: ldr.w r9,[r0,#0x0]
  00144762: ldr.w r0,[r9,#0x0]
  00144766: blx 0x0007258c
  0014476a: movs r1,#0x2
  0014476c: str.w r1,[r0,#0x32c]
  00144770: ldrb.w r0,[r8,#0x28]
  00144774: cbz r0,0x0014477c
  00144776: movs r4,#0x1e
  00144778: movs r0,#0x0
  0014477a: b 0x0014479a
  0014477c: ldr r0,[0x00144854]
  0014477e: add r0,pc
  00144780: ldr r0,[r0,#0x0]
  00144782: ldr r0,[r0,#0x0]
  00144784: blx 0x00071a10
  00144788: blx 0x0007390c
  0014478c: ldrb.w r1,[r8,#0x28]
  00144790: add.w r4,r0,r0, lsl #0x1
  00144794: clz r0,r1
  00144798: lsrs r0,r0,#0x5
  0014479a: ldr r1,[0x00144858]
  0014479c: add.w r11,r4,#0x1
  001447a0: adds r6,r4,#0x2
  001447a2: vldr.32 s22,[pc,#0xb8]
  001447a6: add r1,pc
  001447a8: cmp r0,#0x0
  001447aa: ldr.w r2,[r1,r11,lsl #0x2]
  001447ae: ldr.w r3,[r1,r6,lsl #0x2]
  001447b2: ldr.w r1,[r1,r4,lsl #0x2]
  001447b6: stm r8,{r1,r2,r3}
  001447ba: beq 0x00144864
  001447bc: ldr r0,[0x00144860]
  001447be: add r0,pc
  001447c0: ldr r5,[r0,#0x0]
  001447c2: ldr r0,[r5,#0x0]
  001447c4: blx 0x00071a10
  001447c8: blx 0x0007390c
  001447cc: cmp r0,#0xf
  001447ce: bne 0x0014480c
  001447d0: ldr r0,[r5,#0x0]
  001447d2: blx 0x00071770
  001447d6: cmp r0,#0x59
  001447d8: beq 0x001447e4
  001447da: ldr r0,[r5,#0x0]
  001447dc: blx 0x00071770
  001447e0: cmp r0,#0x9e
  001447e2: blt 0x0014480c
  001447e4: vmov.f32 s0,0x3f000000
  001447e8: vldr.32 s2,[r8]
  001447ec: vldr.32 s4,[r8,#0x4]
  001447f0: vldr.32 s6,[r8,#0x8]
  001447f4: vmul.f32 s2,s2,s0
  001447f8: vmul.f32 s4,s4,s0
  001447fc: vmul.f32 s0,s6,s0
  00144800: vstr.32 s2,[r8]
  00144804: vstr.32 s4,[r8,#0x4]
  00144808: vstr.32 s0,[r8,#0x8]
  0014480c: ldrb.w r1,[r8,#0x28]
  00144810: cbnz r1,0x0014484c
  00144812: ldr r0,[r5,#0x0]
  00144814: blx 0x00071a10
  00144818: blx 0x0007390c
  0014481c: vldr.32 s22,[pc,#0x3c]
  00144820: cmp r0,#0xf
  00144822: bne 0x00144842
  00144824: ldr r0,[r5,#0x0]
  00144826: blx 0x00071770
  0014482a: cmp r0,#0x59
  0014482c: beq 0x00144842
  0014482e: ldr r0,[r5,#0x0]
  00144830: blx 0x00071770
  00144834: adr.w r1,0x144cc8
  00144838: cmp r0,#0x9e
  0014483a: it lt
  0014483c: add.lt r1,#0x4
  0014483e: vldr.32 s22,[r1]
  00144842: ldrb.w r1,[r8,#0x28]
  00144846: cmp r1,#0x0
  00144848: beq.w 0x00144b7a
  0014484c: movs r0,#0x17
  0014484e: b 0x00144868
  00144864: movs r0,#0x17
  00144866: movs r1,#0x1
  00144868: ldr r2,[0x00144bec]
  0014486a: vmov.f32 s24,0x41700000
  0014486e: cmp r1,#0x0
  00144870: add r2,pc
  00144872: add.w r0,r2,r0, lsl #0x2
  00144876: vldr.32 s16,[r0]
  0014487a: vldr.32 s18,[r0,#0x4]
  0014487e: vldr.32 s20,[r0,#0x8]
  00144882: bne 0x001448b8
  00144884: ldr r0,[0x00144bf0]
  00144886: add r0,pc
  00144888: ldr r5,[r0,#0x0]
  0014488a: ldr r0,[r5,#0x0]
  0014488c: blx 0x00071a10
  00144890: blx 0x0007390c
  00144894: cmp r0,#0xf
  00144896: bne 0x001448b8
  00144898: ldr r0,[r5,#0x0]
  0014489a: blx 0x00071770
  0014489e: cmp r0,#0x59
  001448a0: beq 0x001448b8
  001448a2: ldr r0,[r5,#0x0]
  001448a4: blx 0x00071770
  001448a8: vmov.f32 s0,0x41f00000
  001448ac: cmp r0,#0x9e
  001448ae: vmov.f32 s24,0x41700000
  001448b2: it lt
  001448b4: vmov.lt.f32 s24,s0
  001448b8: ldr r1,[0x00144bf4]
  001448ba: ldr.w r0,[r9,#0x0]
  001448be: add r1,pc
  001448c0: vldr.32 s30,[r8]
  001448c4: vldr.32 s26,[r8,#0x4]
  001448c8: ldr.w r10,[r1,#0x0]
  001448cc: vldr.32 s28,[r8,#0x8]
  001448d0: blx r10
  001448d2: vldr.32 s0,[r8]
  001448d6: vldr.32 s2,[r8,#0x4]
  001448da: vldr.32 s4,[r8,#0x8]
  001448de: vmul.f32 s0,s22,s0
  001448e2: vmul.f32 s2,s22,s2
  001448e6: vmul.f32 s4,s22,s4
  001448ea: vmov r1,s0
  001448ee: vmov r2,s2
  001448f2: vmov r3,s4
  001448f6: blx 0x000769cc
  001448fa: ldr.w r0,[r9,#0x0]
  001448fe: blx r10
  00144900: ldr r1,[0x00144cc4]
  00144902: vmov.f32 s0,0x40400000
  00144906: add r1,pc
  00144908: add.w r2,r1,r6, lsl #0x2
  0014490c: add.w r3,r1,r4, lsl #0x2
  00144910: add.w r1,r1,r11, lsl #0x2
  00144914: vldr.32 s2,[r3]
  00144918: vldr.32 s4,[r1]
  0014491c: vldr.32 s6,[r2]
  00144920: vmul.f32 s2,s2,s0
  00144924: vmul.f32 s4,s4,s0
  00144928: vmul.f32 s0,s6,s0
  0014492c: vmov r1,s2
  00144930: vmov r2,s4
  00144934: vmov r3,s0
  00144938: blx 0x000769d8
  0014493c: ldr.w r0,[r9,#0x0]
  00144940: blx r10
  00144942: mov.w r1,#0x3f800000
  00144946: mov.w r2,#0x3f800000
  0014494a: mov.w r3,#0x3f800000
  0014494e: blx 0x000745cc
  00144952: ldr.w r0,[r9,#0x0]
  00144956: blx r10
  00144958: mov.w r1,#0x3f800000
  0014495c: mov.w r2,#0x3f800000
  00144960: mov.w r3,#0x3f800000
  00144964: blx 0x000745d8
  00144968: ldr.w r0,[r9,#0x0]
  0014496c: blx r10
  0014496e: mov.w r1,#0x3f800000
  00144972: mov.w r2,#0x3f800000
  00144976: mov.w r3,#0x3f800000
  0014497a: blx 0x000745e4
  0014497e: ldr.w r0,[r9,#0x0]
  00144982: blx r10
  00144984: ldr r1,[0x00144cd0]
  00144986: blx 0x000745f0
  0014498a: ldr.w r0,[r9,#0x0]
  0014498e: blx r10
  00144990: add.w r3,r8,#0x30
  00144994: mov.w r11,#0x4000
  00144998: ldmia r3,{r1,r2,r3}
  0014499a: str.w r11,[sp,#0x0]
  0014499e: blx 0x000745a8
  001449a2: ldr.w r0,[r9,#0x0]
  001449a6: blx r10
  001449a8: movs r1,#0x0
  001449aa: movs r2,#0x0
  001449ac: movs r3,#0x0
  001449ae: str.w r11,[sp,#0x0]
  001449b2: blx 0x0007087c
  001449b6: vmul.f32 s0,s24,s30
  001449ba: vldr.32 s8,[pc,#0x318]
  001449be: vmov.f32 s2,0x40000000
  001449c2: vmul.f32 s4,s24,s26
  001449c6: vmov.f32 s10,s8
  001449ca: vmul.f32 s6,s24,s28
  001449ce: vmov.f32 s12,s8
  001449d2: vmov.f32 s14,s8
  001449d6: vcmpe.f32 s0,#0
  001449da: vmrs apsr,fpscr
  001449de: vmov.f32 s1,s8
  001449e2: vcmpe.f32 s4,s2
  001449e6: it gt
  001449e8: vmov.gt.f32 s10,s0
  001449ec: vmrs apsr,fpscr
  001449f0: vcmpe.f32 s6,s2
  001449f4: it gt
  001449f6: vmov.gt.f32 s12,s2
  001449fa: vmrs apsr,fpscr
  001449fe: vcmpe.f32 s4,#0
  00144a02: it gt
  00144a04: vmov.gt.f32 s14,s2
  00144a08: vmrs apsr,fpscr
  00144a0c: vcmpe.f32 s6,#0
  00144a10: it gt
  00144a12: vmov.gt.f32 s1,s12
  00144a16: vmrs apsr,fpscr
  00144a1a: vmov.f32 s12,s8
  00144a1e: vcmpe.f32 s10,s2
  00144a22: vmov.f32 s10,s0
  00144a26: it gt
  00144a28: vmov.gt.f32 s12,s14
  00144a2c: vmrs apsr,fpscr
  00144a30: vcmpe.f32 s4,#0
  00144a34: vmov.f32 s14,s1
  00144a38: vmov.f32 s3,s12
  00144a3c: it gt
  00144a3e: vmov.gt.f32 s8,s2
  00144a42: it gt
  00144a44: vmov.gt.f32 s10,s8
  00144a48: vmrs apsr,fpscr
  00144a4c: vcmpe.f32 s6,#0
  00144a50: it gt
  00144a52: vmov.gt.f32 s14,s4
  00144a56: vmrs apsr,fpscr
  00144a5a: vcmpe.f32 s0,#0
  00144a5e: it gt
  00144a60: vmov.gt.f32 s3,s6
  00144a64: vmrs apsr,fpscr
  00144a68: vcmpe.f32 s4,s2
  00144a6c: it le
  00144a6e: vmov.le.f32 s10,s8
  00144a72: vmrs apsr,fpscr
  00144a76: vcmpe.f32 s6,s2
  00144a7a: vmov r5,s10
  00144a7e: it gt
  00144a80: vmov.gt.f32 s14,s1
  00144a84: vmrs apsr,fpscr
  00144a88: vmov r6,s14
  00144a8c: it gt
  00144a8e: vmov.gt.f32 s3,s12
  00144a92: ldr.w r0,[r9,#0x0]
  00144a96: vmov r4,s3
  00144a9a: blx r10
  00144a9c: mov r1,r5
  00144a9e: mov r2,r6
  00144aa0: mov r3,r4
  00144aa2: str.w r11,[sp,#0x0]
  00144aa6: blx 0x000745c0
  00144aaa: ldr.w r0,[r9,#0x0]
  00144aae: blx r10
  00144ab0: mov.w r1,#0x40000000
  00144ab4: mov.w r2,#0x40000000
  00144ab8: mov.w r3,#0x40000000
  00144abc: str.w r11,[sp,#0x0]
  00144ac0: blx 0x00075040
  00144ac4: ldr.w r0,[r9,#0x0]
  00144ac8: blx r10
  00144aca: ldr r3,[0x00144cd8]
  00144acc: movw r11,#0x4001
  00144ad0: movs r1,#0x0
  00144ad2: movs r2,#0x0
  00144ad4: str.w r11,[sp,#0x0]
  00144ad8: blx 0x000745a8
  00144adc: ldr.w r0,[r9,#0x0]
  00144ae0: blx r10
  00144ae2: vmov.f32 s0,0x3fc00000
  00144ae6: str.w r11,[sp,#0x0]
  00144aea: vmul.f32 s2,s16,s0
  00144aee: vmul.f32 s4,s18,s0
  00144af2: vmul.f32 s0,s20,s0
  00144af6: vmov r4,s2
  00144afa: vmov r5,s4
  00144afe: vmov r6,s0
  00144b02: mov r1,r4
  00144b04: mov r2,r5
  00144b06: mov r3,r6
  00144b08: blx 0x000745c0
  00144b0c: ldr.w r0,[r9,#0x0]
  00144b10: blx r10
  00144b12: mov r1,r4
  00144b14: mov r2,r5
  00144b16: mov r3,r6
  00144b18: str.w r11,[sp,#0x0]
  00144b1c: blx 0x00075040
  00144b20: ldr.w r0,[r9,#0x0]
  00144b24: blx r10
  00144b26: ldm.w r8,{r1,r2,r3}
  00144b2a: blx 0x000769e4
  00144b2e: ldr r0,[0x00144d08]
  00144b30: movs r1,#0x0
  00144b32: movs r2,#0x1
  00144b34: add r0,pc
  00144b36: ldr r4,[r0,#0x0]
  00144b38: ldr r0,[r4,#0x0]
  00144b3a: blx 0x00071fe0
  00144b3e: ldrb.w r0,[r8,#0x28]
  00144b42: movs r1,#0x0
  00144b44: strb.w r1,[r8,#0x54]
  00144b48: cmp r0,#0x0
  00144b4a: bne.w 0x00144cb6
  00144b4e: ldr r0,[0x00144d0c]
  00144b50: add r0,pc
  00144b52: ldr r6,[r0,#0x0]
  00144b54: ldr r0,[r6,#0x0]
  00144b56: blx 0x00071a10
  00144b5a: blx 0x0007390c
  00144b5e: sub.w r1,r0,#0xb
  00144b62: cmp r1,#0x7
  00144b64: bhi.w 0x00144cb6
  00144b68: ldr r5,[0x00144cdc]
  00144b6a: tbb [pc,r1]
  00144b76: ldr r5,[0x00144cf4]
  00144b78: b 0x00144bfa
  00144b7a: ldr r0,[r5,#0x0]
  00144b7c: blx 0x00071c14
  00144b80: blx 0x000736b4
  00144b84: ldrb.w r1,[r8,#0x28]
  00144b88: add.w r0,r0,r0, lsl #0x1
  00144b8c: b 0x00144868
  00144b8e: ldr r0,[r6,#0x0]
  00144b90: blx 0x00071770
  00144b94: cmp r0,#0x59
  00144b96: beq.w 0x00144cb6
  00144b9a: ldr r5,[0x00144ce4]
  00144b9c: mov.w r1,#0x3f800000
  00144ba0: ldr r0,[r4,#0x0]
  00144ba2: movs r2,#0x0
  00144ba4: ldr r3,[0x00144ce0]
  00144ba6: strd r1,r5,[sp,#0x0]
  00144baa: movw r1,#0x2601
  00144bae: blx 0x00071ff8
  00144bb2: ldr r0,[r4,#0x0]
  00144bb4: movs r1,#0x1
  00144bb6: movs r2,#0x1
  00144bb8: movs r6,#0x1
  00144bba: blx 0x00071fe0
  00144bbe: strb.w r6,[r8,#0x54]
  00144bc2: b 0x00144bd6
  00144bc4: ldr r0,[r0,#0x4]
  00144bc6: ldr.w r1,[r0,r6,lsl #0x2]
  00144bca: ldr r0,[r4,#0x0]
  00144bcc: ldr r1,[r1,#0xc]
  00144bce: blx 0x00072088
  00144bd2: str r5,[r0,#0x48]
  00144bd4: adds r6,#0x1
  00144bd6: ldr.w r0,[r8,#0x1c]
  00144bda: ldr r1,[r0,#0x0]
  00144bdc: cmp r6,r1
  00144bde: bcc 0x00144bc4
  00144be0: b 0x00144cb6
  00144be2: ldr r5,[0x00144cf0]
  00144be4: b 0x00144bfa
  00144be6: ldr r5,[0x00144cec]
  00144be8: b 0x00144bfa
  00144bf8: ldr r5,[0x00144ce8]
  00144bfa: orr r1,r0,#0x1
  00144bfe: adr r2,[0x144cf8]
  00144c00: cmp r1,#0x11
  00144c02: it eq
  00144c04: add.eq r2,#0x4
  00144c06: vldr.32 s0,[r2]
  00144c0a: cmp r0,#0x12
  00144c0c: vldr.32 s2,[pc,#0xf0]
  00144c10: mov.w r1,#0x3f800000
  00144c14: mov.w r2,#0x0
  00144c18: it eq
  00144c1a: vmov.eq.f32 s0,s2
  00144c1e: ldr r0,[r4,#0x0]
  00144c20: vmov r3,s0
  00144c24: strd r1,r5,[sp,#0x0]
  00144c28: movw r1,#0x2601
  00144c2c: blx 0x00071ff8
  00144c30: ldr r0,[r4,#0x0]
  00144c32: movs r1,#0x1
  00144c34: movs r2,#0x1
  00144c36: movs r6,#0x1
  00144c38: blx 0x00071fe0
  00144c3c: ubfx r0,r5,#0x8,#0x8
  00144c40: lsrs r1,r5,#0x18
  00144c42: vmov s2,r1
  00144c46: vldr.32 s6,[pc,#0xbc]
  00144c4a: vmov s0,r0
  00144c4e: vcvt.f32.s32 s0,s0
  00144c52: vcvt.f32.s32 s2,s2
  00144c56: ubfx r0,r5,#0x10,#0x8
  00144c5a: vmov s4,r0
  00144c5e: vcvt.f32.s32 s4,s4
  00144c62: vmul.f32 s0,s0,s6
  00144c66: vmul.f32 s2,s2,s6
  00144c6a: vmul.f32 s4,s4,s6
  00144c6e: vcvt.s32.f32 s0,s0
  00144c72: vcvt.s32.f32 s2,s2
  00144c76: vcvt.s32.f32 s4,s4
  00144c7a: strb.w r6,[r8,#0x54]
  00144c7e: vmov r0,s2
  00144c82: vmov r1,s4
  00144c86: lsls r0,r0,#0x18
  00144c88: orr.w r0,r0,r1, lsl #0x10
  00144c8c: vmov r1,s0
  00144c90: orr.w r0,r0,r1, lsl #0x8
  00144c94: orr r5,r0,#0xff
  00144c98: b 0x00144cac
  00144c9a: ldr r0,[r0,#0x4]
  00144c9c: ldr.w r1,[r0,r6,lsl #0x2]
  00144ca0: ldr r0,[r4,#0x0]
  00144ca2: ldr r1,[r1,#0xc]
  00144ca4: blx 0x00072088
  00144ca8: str r5,[r0,#0x48]
  00144caa: adds r6,#0x1
  00144cac: ldr.w r0,[r8,#0x1c]
  00144cb0: ldr r1,[r0,#0x0]
  00144cb2: cmp r6,r1
  00144cb4: bcc 0x00144c9a
  00144cb6: vpop {d7,d8,d9,d10,d11,d12,d13,d14,d15}
  00144cba: add sp,#0x4
  00144cbc: pop.w {r8,r9,r10,r11}
  00144cc0: pop {r4,r5,r6,r7,pc}
```
