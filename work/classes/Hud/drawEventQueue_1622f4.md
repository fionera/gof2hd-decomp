# Hud::drawEventQueue
elf 0x1622f4 body 374
Sig: undefined __thiscall drawEventQueue(Hud * this)

## decompile
```c

/* Hud::drawEventQueue() */

void __thiscall Hud::drawEventQueue(Hud *this)

{
  char cVar1;
  char cVar2;
  uchar uVar3;
  uchar uVar4;
  int iVar5;
  uchar uVar6;
  uchar uVar7;
  int iVar8;
  String *pSVar9;
  uint uVar10;
  uint *puVar11;
  int iVar12;
  undefined4 uVar13;
  uint in_fpscr;
  float fVar14;
  float fVar15;
  
  cVar1 = **(char **)(DAT_00172474 + 0x17230e);
  if (cVar1 == '\0') {
    cVar2 = '\0';
  }
  else {
    cVar2 = (char)*(undefined2 *)(this + 0x3e2);
  }
  VectorSignedToFloat(*(undefined4 *)(this + 0x268),(byte)(in_fpscr >> 0x16) & 3);
  puVar11 = *(uint **)(DAT_0017247c + 0x172334);
  uVar13 = *(undefined4 *)(**(int **)(DAT_00172478 + 0x172332) + 0x1e4);
  fVar15 = *(float *)(**(int **)(DAT_00172478 + 0x172332) + 0x1e0);
  AbyssEngine::PaintCanvas::SetColor((uchar)*puVar11,0xff,0xff,0xff);
  fVar14 = -1.0;
  if (cVar1 == '\0') {
    fVar14 = -2.0;
  }
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar11,*(int *)(this + 0x354),(uint)*(ushort *)(this + 0x3e0));
  iVar8 = *(int *)(*(int *)(*(int *)(this + 0x264) + 4) + 4);
  if (iVar8 == 0) goto LAB_00172444;
  iVar5 = *(int *)(iVar8 + 0x30);
  if (iVar5 == 2) {
    uVar3 = (uchar)*puVar11;
    uVar4 = '\0';
    uVar6 = 0xed;
LAB_001723fc:
    uVar7 = '\0';
  }
  else {
    if (iVar5 == 1) {
      uVar3 = (uchar)*puVar11;
      uVar4 = 0xff;
      uVar6 = '*';
      goto LAB_001723fc;
    }
    uVar3 = (uchar)*puVar11;
    if (iVar5 == 3) {
      uVar4 = 0xff;
      uVar6 = 0x80;
      goto LAB_001723fc;
    }
    uVar4 = 0xff;
    uVar6 = 0xff;
    uVar7 = 0xff;
  }
  AbyssEngine::PaintCanvas::SetColor(uVar3,uVar4,uVar6,uVar7);
  iVar5 = *(int *)(iVar8 + 0x1c);
  uVar10 = *puVar11;
  iVar12 = **(int **)(DAT_00172480 + 0x17240a);
  pSVar9 = (String *)**(undefined4 **)(DAT_00172484 + 0x17240e);
  iVar8 = AbyssEngine::PaintCanvas::GetTextWidth(uVar10,pSVar9);
  AbyssEngine::PaintCanvas::DrawString
            (uVar10,pSVar9,iVar5,(iVar12 >> 1) - iVar8 / 2,
             (bool)((char)(int)(fVar14 * fVar15) + (char)uVar13 + cVar2));
LAB_00172444:
  (*(code *)(DAT_001ac094 + 0x1ac098))(*puVar11,0xffffffff);
  return;
}

```

## target disasm
```
  001722f4: push {r4,r5,r6,r7,lr}
  001722f6: add r7,sp,#0xc
  001722f8: push {r7,r8,r9,r10,r11}
  001722fc: vpush {d6,d7,d8}
  00172300: mov r4,r0
  00172302: ldr r0,[0x00172474]
  00172304: vldr.32 s2,[pc,#0x164]
  00172308: movs r2,#0xff
  0017230a: add r0,pc
  0017230c: movs r3,#0xff
  0017230e: ldr r6,[r0,#0x0]
  00172310: ldrb.w r10,[r6,#0x0]
  00172314: cmp.w r10,#0x0
  00172318: ite ne
  0017231a: ldrh.w.ne r0,[r4,#0x3e2]
  0017231e: mov.eq r0,#0x0
  00172320: vldr.32 s0,[r4,#0x268]
  00172324: str r0,[sp,#0xc]
  00172326: vcvt.f32.s32 s0,s0
  0017232a: ldr r0,[0x00172478]
  0017232c: ldr r1,[0x0017247c]
  0017232e: add r0,pc
  00172330: add r1,pc
  00172332: ldr r5,[r0,#0x0]
  00172334: ldr.w r8,[r1,#0x0]
  00172338: vdiv.f32 s0,s0,s2
  0017233c: ldr r1,[r5,#0x0]
  0017233e: ldr.w r0,[r8,#0x0]
  00172342: ldr.w r11,[r1,#0x1e4]
  00172346: vldr.32 s2,[pc,#0x128]
  0017234a: vmul.f32 s0,s0,s2
  0017234e: vcvt.s32.f32 s0,s0
  00172352: vldr.32 s16,[r1,#0x1e0]
  00172356: vmov r1,s0
  0017235a: cmp r1,#0xff
  0017235c: it gt
  0017235e: rsb.gt.w r1,r1,#0xfe
  00172362: uxtb.w r9,r1
  00172366: movs r1,#0xff
  00172368: str.w r9,[sp,#0x0]
  0017236c: blx 0x0006ff28
  00172370: vmov.f32 s2,0xc0000000
  00172374: cmp.w r10,#0x0
  00172378: vmov.f32 s0,0xbf800000
  0017237c: it eq
  0017237e: vmov.eq.f32 s0,s2
  00172382: vmul.f32 s0,s0,s16
  00172386: vcvt.s32.f32 s16,s0
  0017238a: ldrb r3,[r6,#0x0]
  0017238c: ldrh.w r2,[r4,#0x3e0]
  00172390: ldr.w r1,[r4,#0x354]
  00172394: cmp r3,#0x0
  00172396: ldr.w r0,[r8,#0x0]
  0017239a: ite ne
  0017239c: ldrh.w.ne r3,[r4,#0x3e2]
  001723a0: mov.eq r3,#0x0
  001723a2: ldr r6,[r5,#0x0]
  001723a4: ldr.w r6,[r6,#0x1e4]
  001723a8: subs r3,r3,r6
  001723aa: blx 0x00072dc0
  001723ae: ldr.w r0,[r4,#0x264]
  001723b2: ldr r0,[r0,#0x4]
  001723b4: ldr r4,[r0,#0x4]
  001723b6: cmp r4,#0x0
  001723b8: beq 0x00172444
  001723ba: vmov r0,s16
  001723be: ldr r1,[r4,#0x30]
  001723c0: cmp r1,#0x2
  001723c2: str r0,[sp,#0x8]
  001723c4: ldr r0,[sp,#0xc]
  001723c6: add.w r10,r11,r0
  001723ca: beq 0x001723de
  001723cc: cmp r1,#0x1
  001723ce: bne 0x001723ec
  001723d0: ldr.w r0,[r8,#0x0]
  001723d4: movs r1,#0xff
  001723d6: str.w r9,[sp,#0x0]
  001723da: movs r2,#0x2a
  001723dc: b 0x001723fc
  001723de: ldr.w r0,[r8,#0x0]
  001723e2: movs r1,#0x0
  001723e4: str.w r9,[sp,#0x0]
  001723e8: movs r2,#0xed
  001723ea: b 0x001723fc
  001723ec: ldr.w r0,[r8,#0x0]
  001723f0: cmp r1,#0x3
  001723f2: bne 0x0017245e
  001723f4: movs r1,#0xff
  001723f6: movs r2,#0x80
  001723f8: str.w r9,[sp,#0x0]
  001723fc: movs r3,#0x0
  001723fe: blx 0x0006ff28
  00172402: ldr r0,[0x00172480]
  00172404: ldr r1,[0x00172484]
  00172406: add r0,pc
  00172408: ldr r4,[r4,#0x1c]
  0017240a: add r1,pc
  0017240c: ldr.w r6,[r8,#0x0]
  00172410: ldr r0,[r0,#0x0]
  00172412: ldr r1,[r1,#0x0]
  00172414: mov r2,r4
  00172416: ldr.w r9,[r0,#0x0]
  0017241a: mov r0,r6
  0017241c: ldr r5,[r1,#0x0]
  0017241e: mov r1,r5
  00172420: blx 0x0006faa8
  00172424: ldr r2,[sp,#0x8]
  00172426: add.w r0,r0,r0, lsr #0x1f
  0017242a: movs r1,#0x0
  0017242c: add r2,r10
  0017242e: strd r2,r1,[sp,#0x0]
  00172432: asr.w r1,r9, asr #0x1
  00172436: sub.w r3,r1,r0, asr #0x1
  0017243a: mov r0,r6
  0017243c: mov r1,r5
  0017243e: mov r2,r4
  00172440: blx 0x0006fe14
  00172444: ldr.w r0,[r8,#0x0]
  00172448: mov.w r1,#0xffffffff
  0017244c: vpop {d6,d7,d8}
  00172450: add sp,#0x4
  00172452: pop.w {r8,r9,r10,r11}
  00172456: pop.w {r4,r5,r6,r7,lr}
  0017245a: b.w 0x001ac088
  0017245e: movs r1,#0xff
  00172460: movs r2,#0xff
  00172462: movs r3,#0xff
  00172464: str.w r9,[sp,#0x0]
  00172468: b 0x001723fe
```
