# AbyssEngine::CurveGetValue
elf 0x74194 body 398
Sig: undefined __thiscall CurveGetValue(AbyssEngine * this, ulonglong param_1, Curve * param_2)

## decompile
```c

/* AbyssEngine::CurveGetValue(unsigned long long, AbyssEngine::Curve*) */

undefined8 __thiscall AbyssEngine::CurveGetValue(AbyssEngine *this,ulonglong param_1,Curve *param_2)

{
  char cVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  uint in_r1;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  int unaff_r5;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  AbyssEngine *pAVar15;
  uint uVar16;
  int iVar17;
  char *pcVar18;
  uint uVar19;
  int *piVar20;
  uint uVar21;
  longlong lVar22;
  ulonglong uVar23;
  
  piVar20 = *(int **)((ushort *)param_1 + 2);
  iVar9 = *piVar20;
  if (*(uint *)(iVar9 + 0xc) <= in_r1 &&
      (uint)(this <= *(AbyssEngine **)(iVar9 + 8)) <= *(uint *)(iVar9 + 0xc) - in_r1) {
    uVar5 = *(ushort *)param_1 - 1;
    iVar9 = piVar20[uVar5];
    if (in_r1 <= *(uint *)(iVar9 + 0xc) &&
        (uint)(*(AbyssEngine **)(iVar9 + 8) <= this) <= in_r1 - *(uint *)(iVar9 + 0xc)) {
      do {
        uVar10 = uVar5 & 0xffff;
        uVar5 = uVar5 - 1;
        pcVar18 = (char *)piVar20[uVar10];
        pAVar15 = *(AbyssEngine **)(pcVar18 + 8);
        uVar12 = *(uint *)(pcVar18 + 0xc);
      } while (in_r1 <= uVar12 && (uint)(pAVar15 <= this) <= in_r1 - uVar12);
      cVar1 = *pcVar18;
      if (cVar1 == '\x03') {
        iVar9 = piVar20[uVar10 + 1];
        iVar17 = *(int *)(iVar9 + 0x10);
        lVar22 = __aeabi_uldivmod(((int)this - (int)pAVar15) * 0x1000,
                                  ((in_r1 - uVar12) - (uint)(this < pAVar15)) * 0x1000 |
                                  (uint)((int)this - (int)pAVar15) >> 0x14,
                                  (int)*(AbyssEngine **)(iVar9 + 8) - (int)pAVar15,
                                  (*(int *)(iVar9 + 0xc) - uVar12) -
                                  (uint)(*(AbyssEngine **)(iVar9 + 8) < pAVar15));
        uVar21 = (uint)((ulonglong)(lVar22 * lVar22) >> 0x20);
        uVar16 = (uint)(lVar22 * lVar22) >> 0xc | uVar21 * 0x100000;
        uVar19 = uVar21 >> 0xc;
        uVar12 = (uint)lVar22 + uVar16 * -2;
        lVar4 = lVar22 * CONCAT44(uVar19,uVar16);
        uVar10 = (uint)((ulonglong)lVar4 >> 0x20);
        uVar11 = (uint)lVar4 >> 0xc | uVar10 * 0x100000;
        unaff_r5 = *(int *)(pcVar18 + 0x10);
        uVar13 = uVar12 + uVar11;
        uVar5 = *(uint *)(pcVar18 + 0x14);
        uVar6 = *(uint *)(pcVar18 + 0x18);
        iVar9 = (int)uVar10 >> 0xc;
        lVar4 = (ulonglong)uVar13 * (ulonglong)uVar5;
        lVar2 = (ulonglong)(uVar11 - uVar16) * (ulonglong)uVar6;
        uVar7 = (uint)((ulonglong)uVar16 * 3);
        uVar14 = iVar17 - unaff_r5;
        uVar8 = uVar7 + uVar11 * -2;
        lVar3 = (ulonglong)uVar8 * (ulonglong)uVar14;
        iVar9 = ((uint)lVar3 >> 0xc |
                ((((uVar19 * 3 + (int)((ulonglong)uVar16 * 3 >> 0x20)) -
                  (iVar9 << 1 | (uVar10 & 0xfff) >> 0xb)) - (uint)(uVar7 < uVar11 * 2)) * uVar14 +
                uVar8 * ((int)uVar14 >> 0x1f) + (int)((ulonglong)lVar3 >> 0x20)) * 0x100000) +
                ((uint)lVar2 >> 0xc |
                (((iVar9 - (uVar21 >> 0xc)) - (uint)(uVar11 < uVar16)) * uVar6 +
                (uVar11 - uVar16) * ((int)uVar6 >> 0x1f) + (int)((ulonglong)lVar2 >> 0x20)) *
                0x100000) +
                ((uint)lVar4 >> 0xc |
                (((((int)((ulonglong)lVar22 >> 0x20) - (uVar19 << 1 | (uVar21 & 0xfff) >> 0xb)) -
                  (uint)((uint)lVar22 < uVar16 * 2)) + iVar9 + (uint)CARRY4(uVar12,uVar11)) * uVar5
                + uVar13 * ((int)uVar5 >> 0x1f) + (int)((ulonglong)lVar4 >> 0x20)) * 0x100000) +
                unaff_r5;
      }
      else if (cVar1 == '\x02') {
        iVar9 = piVar20[uVar10 + 1];
        iVar17 = *(int *)(iVar9 + 0x10);
        uVar23 = __aeabi_uldivmod(((int)this - (int)pAVar15) * 0x1000,
                                  ((in_r1 - uVar12) - (uint)(this < pAVar15)) * 0x1000 |
                                  (uint)((int)this - (int)pAVar15) >> 0x14,
                                  (int)*(AbyssEngine **)(iVar9 + 8) - (int)pAVar15,
                                  (*(int *)(iVar9 + 0xc) - uVar12) -
                                  (uint)(*(AbyssEngine **)(iVar9 + 8) < pAVar15));
        uVar5 = iVar17 - *(int *)(pcVar18 + 0x10);
        lVar4 = (uVar23 & 0xffffffff) * (ulonglong)uVar5;
        iVar9 = ((uint)lVar4 >> 0xc |
                ((int)(uVar23 >> 0x20) * uVar5 +
                (int)uVar23 * ((int)uVar5 >> 0x1f) + (int)((ulonglong)lVar4 >> 0x20)) * 0x100000) +
                *(int *)(pcVar18 + 0x10);
      }
      else if (cVar1 == '\x01') {
        iVar9 = *(int *)(pcVar18 + 0x10);
      }
      else {
        iVar9 = 0;
      }
      goto LAB_000841f2;
    }
  }
  iVar9 = *(int *)(iVar9 + 0x10);
LAB_000841f2:
  return CONCAT44(unaff_r5,iVar9);
}

```

## target disasm
```
  00084194: push {r4,r5,r6,r7,lr}
  00084196: add r7,sp,#0xc
  00084198: push {r5,r6,r7,r8,r9,r10,r11}
  0008419c: ldr.w r12,[r2,#0x4]
  000841a0: ldr.w r3,[r12,#0x0]
  000841a4: ldrd r6,r5,[r3,#0x8]
  000841a8: subs r6,r6,r0
  000841aa: sbcs.w r6,r5,r1
  000841ae: bcs 0x000841f0
  000841b0: ldrh r2,[r2,#0x0]
  000841b2: subs r2,#0x1
  000841b4: ldr.w r3,[r12,r2,lsl #0x2]
  000841b8: ldrd r6,r5,[r3,#0x8]
  000841bc: subs r6,r0,r6
  000841be: sbcs.w r6,r1,r5
  000841c2: bcs 0x000841f0
  000841c4: uxth r3,r2
  000841c6: subs r2,#0x1
  000841c8: ldr.w r9,[r12,r3,lsl #0x2]
  000841cc: ldrd r6,r5,[r9,#0x8]
  000841d0: subs r4,r0,r6
  000841d2: sbcs.w r4,r1,r5
  000841d6: bcc 0x000841c4
  000841d8: ldrsb.w r2,[r9,#0x0]
  000841dc: cmp r2,#0x3
  000841de: beq 0x000841f8
  000841e0: cmp r2,#0x2
  000841e2: beq 0x000842de
  000841e4: cmp r2,#0x1
  000841e6: ite eq
  000841e8: ldr.eq.w r0,[r9,#0x10]
  000841ec: mov.ne r0,#0x0
  000841ee: b 0x000841f2
  000841f0: ldr r0,[r3,#0x10]
  000841f2: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000841f6: pop {r4,r5,r6,r7,pc}
  000841f8: add.w r2,r12,r3, lsl #0x2
  000841fc: ldr r2,[r2,#0x4]
  000841fe: ldrd r3,r4,[r2,#0x8]
  00084202: ldr r2,[r2,#0x10]
  00084204: str r2,[sp,#0x8]
  00084206: subs r2,r3,r6
  00084208: sbc.w r3,r4,r5
  0008420c: subs r0,r0,r6
  0008420e: sbcs r1,r5
  00084210: lsls r1,r1,#0xc
  00084212: orr.w r1,r1,r0, lsr #0x14
  00084216: lsls r0,r0,#0xc
  00084218: blx 0x0006f5ec
  0008421c: umull r2,r3,r0,r0
  00084220: mla r3,r0,r1,r3
  00084224: mla r12,r0,r1,r3
  00084228: lsrs r2,r2,#0xc
  0008422a: orr.w r6,r2,r12, lsl #0x14
  0008422e: lsr.w r10,r12,#0xc
  00084232: lsl.w r5,r10,#0x1
  00084236: orr.w r5,r5,r6, lsr #0x1f
  0008423a: subs.w r2,r0,r6, lsl #0x1
  0008423e: sbc.w lr,r1,r5
  00084242: umull r4,r5,r6,r0
  00084246: mla r1,r6,r1,r5
  0008424a: mla r1,r10,r0,r1
  0008424e: lsrs r0,r4,#0xc
  00084250: orr.w r4,r0,r1, lsl #0x14
  00084254: ldr.w r0,[r9,#0x10]
  00084258: str r0,[sp,#0x0]
  0008425a: adds r5,r2,r4
  0008425c: ldrd r0,r2,[r9,#0x14]
  00084260: adc.w lr,lr,r1, asr #0xc
  00084264: asrs r1,r1,#0xc
  00084266: umull r3,r11,r5,r0
  0008426a: asr.w r8,r0, asr #0x1f
  0008426e: mla r5,r5,r8,r11
  00084272: str r3,[sp,#0x4]
  00084274: mla lr,lr,r0,r5
  00084278: subs r5,r4,r6
  0008427a: asr.w r0,r2, asr #0x1f
  0008427e: umull r8,r11,r5,r2
  00084282: sbc.w r3,r1,r12, lsr #0xc
  00084286: lsls r1,r1,#0x1
  00084288: orr.w r1,r1,r4, lsr #0x1f
  0008428c: mla r0,r5,r0,r11
  00084290: mla r11,r3,r2,r0
  00084294: movs r2,#0x3
  00084296: ldr r0,[sp,#0x8]
  00084298: umull r2,r3,r6,r2
  0008429c: ldr r6,[sp,#0x0]
  0008429e: subs r5,r0,r6
  000842a0: asr.w r9,r5, asr #0x1f
  000842a4: subs.w r2,r2,r4, lsl #0x1
  000842a8: umull r12,r0,r2,r5
  000842ac: mla r0,r2,r9,r0
  000842b0: add.w r2,r10,r10, lsl #0x1
  000842b4: add r2,r3
  000842b6: sbc.w r1,r2,r1
  000842ba: ldr r2,[sp,#0x4]
  000842bc: mla r0,r1,r5,r0
  000842c0: lsr.w r1,r8,#0xc
  000842c4: lsrs r2,r2,#0xc
  000842c6: orr.w r1,r1,r11, lsl #0x14
  000842ca: orr.w r2,r2,lr, lsl #0x14
  000842ce: add r1,r2
  000842d0: lsr.w r2,r12,#0xc
  000842d4: orr.w r0,r2,r0, lsl #0x14
  000842d8: add r0,r1
  000842da: add r0,r6
  000842dc: b 0x000841f2
  000842de: add.w r2,r12,r3, lsl #0x2
  000842e2: ldr r2,[r2,#0x4]
  000842e4: add.w r8,r2,#0x8
  000842e8: ldm.w r8,{r3,r4,r8}
  000842ec: subs r2,r3,r6
  000842ee: sbc.w r3,r4,r5
  000842f2: subs r0,r0,r6
  000842f4: sbcs r1,r5
  000842f6: lsls r1,r1,#0xc
  000842f8: orr.w r1,r1,r0, lsr #0x14
  000842fc: lsls r0,r0,#0xc
  000842fe: blx 0x0006f5ec
  00084302: ldr.w r2,[r9,#0x10]
  00084306: sub.w r3,r8,r2
  0008430a: umull r6,r5,r0,r3
  0008430e: asrs r4,r3,#0x1f
  00084310: mla r0,r0,r4,r5
  00084314: mla r0,r1,r3,r0
  00084318: lsrs r1,r6,#0xc
  0008431a: orr.w r0,r1,r0, lsl #0x14
  0008431e: add r0,r2
  00084320: b 0x000841f2
```
