# LodMeshMerger::update
elf 0x18174c body 454
Sig: undefined __thiscall update(LodMeshMerger * this)

## decompile
```c

/* LodMeshMerger::update() */

void __thiscall LodMeshMerger::update(LodMeshMerger *this)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  short *psVar5;
  short *psVar6;
  byte *pbVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  float in_s0;
  float extraout_s0;
  
  for (iVar8 = 0; iVar3 = *(int *)this, iVar8 < iVar3; iVar8 = iVar8 + 1) {
    uVar4 = AbyssEngine::PaintCanvas::CameraIsSphereinViewFrustum
                      (*(PaintCanvas **)(this + 0x14),
                       (Vector *)(*(int *)(*(int *)(this + 0x24) + iVar8 * 4) + 0x3c),in_s0);
    if ((uVar4 != *(byte *)(*(int *)(this + 0x34) + iVar8)) &&
       (*(char *)(*(int *)(this + 0x34) + iVar8) = (char)uVar4,
       *(char *)(*(int *)(this + 0x30) + iVar8) != '\0')) {
      this[0x3c] = (LodMeshMerger)0x1;
    }
    in_s0 = extraout_s0;
  }
  if (this[0x3c] != (LodMeshMerger)0x0) {
    iVar9 = 0;
    for (iVar8 = 0; iVar8 < iVar3; iVar8 = iVar8 + 1) {
      if ((*(char *)(*(int *)(this + 0x30) + iVar8) != '\0') &&
         (*(char *)(*(int *)(this + 0x34) + iVar8) != '\0')) {
        iVar9 = iVar9 + (uint)*(ushort *)
                               (*(int *)(*(int *)(this + 0x24) +
                                        (iVar3 * *(char *)(*(int *)(this + 0x2c) + iVar8) + iVar8) *
                                        4) + 0x28);
      }
    }
    for (iVar8 = 0; (0xffff < iVar9 && (iVar8 < iVar3)); iVar8 = iVar8 + 1) {
      if ((*(char *)(*(int *)(this + 0x30) + iVar8) != '\0') &&
         (*(char *)(*(int *)(this + 0x34) + iVar8) != '\0')) {
        cVar2 = *(char *)(*(int *)(this + 0x2c) + iVar8);
        if ((int)cVar2 < *(int *)(this + 0x38) + -1) {
          iVar10 = *(int *)(*(int *)(this + 0x24) + (iVar3 * cVar2 + iVar8) * 4);
          setLod(this,iVar8,(int)(char)(cVar2 + '\x01'));
          iVar3 = *(int *)this;
          iVar9 = (iVar9 - (uint)*(ushort *)(iVar10 + 0x28)) +
                  (uint)*(ushort *)
                         (*(int *)(*(int *)(this + 0x24) +
                                  (iVar3 * *(char *)(*(int *)(this + 0x2c) + iVar8) + iVar8) * 4) +
                         0x28);
        }
      }
    }
    iVar8 = 0;
    iVar10 = 0;
    for (iVar9 = 0; iVar9 < iVar3; iVar9 = iVar9 + 1) {
      if ((*(char *)(*(int *)(this + 0x30) + iVar9) != '\0') &&
         (*(char *)(*(int *)(this + 0x34) + iVar9) != '\0')) {
        pbVar7 = *(byte **)(this + 0x20);
        bVar1 = *pbVar7;
        iVar11 = *(int *)(*(int *)(this + 0x24) +
                         (iVar3 * *(char *)(*(int *)(this + 0x2c) + iVar9) + iVar9) * 4);
        if ((bVar1 & 1) != 0) {
          __aeabi_memcpy4(*(int *)(pbVar7 + 4) + iVar8 * 0xc,*(undefined4 *)(iVar11 + 4),
                          (uint)*(ushort *)(iVar11 + 2) * 0xc);
          pbVar7 = *(byte **)(this + 0x20);
          bVar1 = *pbVar7;
        }
        uVar4 = (uint)bVar1;
        if ((int)(uVar4 << 0x1d) < 0) {
          __aeabi_memcpy4(*(int *)(pbVar7 + 0x10) + iVar8 * 0xc,*(undefined4 *)(iVar11 + 0x10),
                          (uint)*(ushort *)(iVar11 + 2) * 0xc);
          pbVar7 = *(byte **)(this + 0x20);
          uVar4 = (uint)*pbVar7;
        }
        if ((int)(uVar4 << 0x1c) < 0) {
          __aeabi_memcpy4(*(int *)(pbVar7 + 0xc) + iVar8 * 0x10,*(undefined4 *)(iVar11 + 0xc),
                          (uint)*(ushort *)(iVar11 + 2) << 4);
          pbVar7 = *(byte **)(this + 0x20);
          uVar4 = (uint)*pbVar7;
        }
        if ((int)(uVar4 << 0x1e) < 0) {
          __aeabi_memcpy4(*(int *)(pbVar7 + 8) + iVar8 * 8,*(undefined4 *)(iVar11 + 8),
                          (uint)*(ushort *)(iVar11 + 2) << 3);
          pbVar7 = *(byte **)(this + 0x20);
          uVar4 = (uint)*pbVar7;
        }
        if ((int)(uVar4 << 0x1b) < 0) {
          psVar5 = *(short **)(iVar11 + 0x2c);
          psVar6 = (short *)(*(int *)(pbVar7 + 0x2c) + iVar10 * 2);
          for (iVar3 = -(uint)*(ushort *)(iVar11 + 0x28); iVar3 != 0; iVar3 = iVar3 + 1) {
            *psVar6 = *psVar5 + (short)iVar8;
            psVar5 = psVar5 + 1;
            psVar6 = psVar6 + 1;
          }
        }
        iVar3 = *(int *)this;
        iVar10 = iVar10 + (uint)*(ushort *)(iVar11 + 0x28);
        iVar8 = iVar8 + (uint)*(ushort *)(iVar11 + 2);
      }
    }
    iVar3 = *(int *)(this + 0x20);
    *(short *)(iVar3 + 0x28) = (short)iVar10;
    *(short *)(iVar3 + 2) = (short)iVar8;
    this[0x3c] = (LodMeshMerger)0x0;
  }
  return;
}

```

## target disasm
```
  0019174c: push {r4,r5,r6,r7,lr}
  0019174e: add r7,sp,#0xc
  00191750: push {r8,r9,r11}
  00191754: mov r4,r0
  00191756: movs r5,#0x0
  00191758: movs r6,#0x1
  0019175a: b 0x00191782
  0019175c: ldr r1,[r4,#0x24]
  0019175e: ldr r0,[r4,#0x14]
  00191760: ldr.w r1,[r1,r5,lsl #0x2]
  00191764: ldr r2,[r1,#0x48]
  00191766: adds r1,#0x3c
  00191768: blx 0x0006ff94
  0019176c: ldr r1,[r4,#0x34]
  0019176e: ldrb r2,[r1,r5]
  00191770: cmp r0,r2
  00191772: beq 0x00191780
  00191774: strb r0,[r1,r5]
  00191776: ldr r0,[r4,#0x30]
  00191778: ldrb r0,[r0,r5]
  0019177a: cbz r0,0x00191780
  0019177c: strb.w r6,[r4,#0x3c]
  00191780: adds r5,#0x1
  00191782: ldr r0,[r4,#0x0]
  00191784: cmp r5,r0
  00191786: blt 0x0019175c
  00191788: ldrb.w r1,[r4,#0x3c]
  0019178c: cmp r1,#0x0
  0019178e: beq.w 0x0019190c
  00191792: movs r1,#0x0
  00191794: movs r6,#0x0
  00191796: b 0x001917b8
  00191798: ldr r2,[r4,#0x30]
  0019179a: ldrb r2,[r2,r1]
  0019179c: cbz r2,0x001917b6
  0019179e: ldr r2,[r4,#0x34]
  001917a0: ldrb r2,[r2,r1]
  001917a2: cbz r2,0x001917b6
  001917a4: ldr r3,[r4,#0x2c]
  001917a6: ldr r2,[r4,#0x24]
  001917a8: ldrsb r3,[r3,r1]
  001917aa: mla r3,r0,r3,r1
  001917ae: ldr.w r2,[r2,r3,lsl #0x2]
  001917b2: ldrh r2,[r2,#0x28]
  001917b4: add r6,r2
  001917b6: adds r1,#0x1
  001917b8: cmp r1,r0
  001917ba: blt 0x00191798
  001917bc: movs r5,#0x0
  001917be: b 0x00191810
  001917c0: cmp r5,r0
  001917c2: bge 0x00191816
  001917c4: ldr r1,[r4,#0x30]
  001917c6: ldrb r1,[r1,r5]
  001917c8: cbz r1,0x0019180e
  001917ca: ldr r1,[r4,#0x34]
  001917cc: ldrb r1,[r1,r5]
  001917ce: cbz r1,0x0019180e
  001917d0: ldr r1,[r4,#0x2c]
  001917d2: ldr r2,[r4,#0x38]
  001917d4: ldrsb r1,[r1,r5]
  001917d6: subs r2,#0x1
  001917d8: cmp r2,r1
  001917da: ble 0x0019180e
  001917dc: mla r0,r0,r1,r5
  001917e0: uxtb r2,r1
  001917e2: ldr r1,[r4,#0x24]
  001917e4: ldr.w r8,[r1,r0,lsl #0x2]
  001917e8: adds r0,r2,#0x1
  001917ea: mov r1,r5
  001917ec: sxtb r2,r0
  001917ee: mov r0,r4
  001917f0: blx 0x0007309c
  001917f4: ldr r2,[r4,#0x2c]
  001917f6: ldr r0,[r4,#0x0]
  001917f8: ldr r1,[r4,#0x24]
  001917fa: ldrsb r2,[r2,r5]
  001917fc: mla r2,r0,r2,r5
  00191800: ldr.w r1,[r1,r2,lsl #0x2]
  00191804: ldrh.w r2,[r8,#0x28]
  00191808: subs r2,r6,r2
  0019180a: ldrh r1,[r1,#0x28]
  0019180c: adds r6,r2,r1
  0019180e: adds r5,#0x1
  00191810: cmp.w r6,#0x10000
  00191814: bge 0x001917c0
  00191816: movs r5,#0x0
  00191818: mov.w r8,#0x0
  0019181c: movs r6,#0x0
  0019181e: b 0x001918fa
  00191820: ldr r1,[r4,#0x30]
  00191822: ldrb r1,[r1,r6]
  00191824: cmp r1,#0x0
  00191826: beq 0x001918f8
  00191828: ldr r1,[r4,#0x34]
  0019182a: ldrb r1,[r1,r6]
  0019182c: cmp r1,#0x0
  0019182e: beq 0x001918f8
  00191830: ldrd r2,r1,[r4,#0x20]
  00191834: ldr r3,[r4,#0x2c]
  00191836: ldrsb r3,[r3,r6]
  00191838: mla r3,r0,r3,r6
  0019183c: ldrb r0,[r2,#0x0]
  0019183e: tst r0,#0x1
  00191842: ldr.w r9,[r1,r3,lsl #0x2]
  00191846: beq 0x00191868
  00191848: ldrh.w r3,[r9,#0x2]
  0019184c: ldr r0,[r2,#0x4]
  0019184e: add.w r2,r5,r5, lsl #0x1
  00191852: ldr.w r1,[r9,#0x4]
  00191856: add.w r0,r0,r2, lsl #0x2
  0019185a: add.w r2,r3,r3, lsl #0x1
  0019185e: lsls r2,r2,#0x2
  00191860: blx 0x0006f6a0
  00191864: ldr r2,[r4,#0x20]
  00191866: ldrb r0,[r2,#0x0]
  00191868: lsls r1,r0,#0x1d
  0019186a: bpl 0x0019188c
  0019186c: ldrh.w r3,[r9,#0x2]
  00191870: ldr r0,[r2,#0x10]
  00191872: add.w r2,r5,r5, lsl #0x1
  00191876: ldr.w r1,[r9,#0x10]
  0019187a: add.w r0,r0,r2, lsl #0x2
  0019187e: add.w r2,r3,r3, lsl #0x1
  00191882: lsls r2,r2,#0x2
  00191884: blx 0x0006f6a0
  00191888: ldr r2,[r4,#0x20]
  0019188a: ldrb r0,[r2,#0x0]
  0019188c: lsls r1,r0,#0x1c
  0019188e: bpl 0x001918a8
  00191890: ldr r0,[r2,#0xc]
  00191892: ldrh.w r3,[r9,#0x2]
  00191896: ldr.w r1,[r9,#0xc]
  0019189a: add.w r0,r0,r5, lsl #0x4
  0019189e: lsls r2,r3,#0x4
  001918a0: blx 0x0006f6a0
  001918a4: ldr r2,[r4,#0x20]
  001918a6: ldrb r0,[r2,#0x0]
  001918a8: lsls r1,r0,#0x1e
  001918aa: bpl 0x001918c4
  001918ac: ldr r0,[r2,#0x8]
  001918ae: ldrh.w r3,[r9,#0x2]
  001918b2: ldr.w r1,[r9,#0x8]
  001918b6: add.w r0,r0,r5, lsl #0x3
  001918ba: lsls r2,r3,#0x3
  001918bc: blx 0x0006f6a0
  001918c0: ldr r2,[r4,#0x20]
  001918c2: ldrb r0,[r2,#0x0]
  001918c4: lsls r0,r0,#0x1b
  001918c6: bpl 0x001918ea
  001918c8: ldr r1,[r2,#0x2c]
  001918ca: ldrh.w r2,[r9,#0x28]
  001918ce: ldr.w r0,[r9,#0x2c]
  001918d2: add.w r1,r1,r8, lsl #0x1
  001918d6: rsbs r2,r2
  001918d8: b 0x001918e6
  001918da: ldrh.w r3,[r0],#0x2
  001918de: adds r2,#0x1
  001918e0: add r3,r5
  001918e2: strh r3,[r1],#0x2
  001918e6: cmp r2,#0x0
  001918e8: bne 0x001918da
  001918ea: ldrh.w r1,[r9,#0x28]
  001918ee: ldrh.w r2,[r9,#0x2]
  001918f2: ldr r0,[r4,#0x0]
  001918f4: add r8,r1
  001918f6: add r5,r2
  001918f8: adds r6,#0x1
  001918fa: cmp r6,r0
  001918fc: blt 0x00191820
  001918fe: ldr r0,[r4,#0x20]
  00191900: strh.w r8,[r0,#0x28]
  00191904: strh r5,[r0,#0x2]
  00191906: movs r0,#0x0
  00191908: strb.w r0,[r4,#0x3c]
  0019190c: pop.w {r8,r9,r11}
  00191910: pop {r4,r5,r6,r7,pc}
```
