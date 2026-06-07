# Hud::touchedElement
elf 0x166574 body 1146
Sig: undefined __thiscall touchedElement(Hud * this, uint param_1, uint param_2)

## decompile
```c

/* Hud::touchedElement(unsigned int, unsigned int) */

undefined4 __thiscall Hud::touchedElement(Hud *this,uint param_1,uint param_2)

{
  int iVar1;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  bool bVar5;
  bool bVar6;
  
  if ((this[0x282] == (Hud)0x0) || (puVar2 = *(uint **)(this + 0x18), puVar2 == (uint *)0x0)) {
    if (**(char **)(DAT_00176908 + 0x1765b8) == '\0') {
      if (this[0x528] != (Hud)0x0) {
        if ((((*(ushort *)(this + 0x456) <= param_2) &&
             (param_2 <= (uint)*(ushort *)(this + 0x456) + *(int *)(this + 0x4d8))) &&
            (*(ushort *)(this + 0x454) <= param_1)) &&
           (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x454))) {
          return 0x200;
        }
        if (((*(ushort *)(this + 0x45a) <= param_2) &&
            (param_2 <= (uint)*(ushort *)(this + 0x45a) + *(int *)(this + 0x4d8))) &&
           ((*(ushort *)(this + 0x458) <= param_1 &&
            (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x458))))) {
          return 0x400;
        }
        if (((*(ushort *)(this + 0x460) <= param_2) &&
            (param_2 <= (uint)*(ushort *)(this + 0x460) + *(int *)(this + 0x4d8))) &&
           ((*(ushort *)(this + 0x45e) <= param_1 &&
            (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x45e))))) {
          return 0x800;
        }
      }
      if (param_2 < (uint)(**(int **)(DAT_001769f4 + 0x17669c) >> 2)) {
        if (((*(ushort *)(this + 0x40c) <= param_2) &&
            (param_2 <= (uint)*(ushort *)(this + 0x40c) + *(int *)(this + 0x4d8))) &&
           ((*(ushort *)(this + 0x40a) <= param_1 &&
            (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x40a))))) {
          return 1;
        }
      }
      else if (param_1 < (uint)(**(int **)(DAT_001769f8 + 0x17675e) >> 1)) {
        if ((((this[0x21e] != (Hud)0x0) && (*(ushort *)(this + 0x412) <= param_2)) &&
            (iVar3 = *(int *)(this + 0x4d8), param_2 <= (uint)*(ushort *)(this + 0x412) + iVar3)) &&
           (((uint)*(ushort *)(this + 0x410) - iVar3 <= param_1 &&
            (param_1 <= iVar3 + (uint)*(ushort *)(this + 0x410))))) {
          return 2;
        }
        if (((*(ushort *)(this + 0x3fa) <= param_2) &&
            (param_2 <= (uint)*(ushort *)(this + 0x3fa) + *(int *)(this + 0x4d8))) &&
           ((*(ushort *)(this + 0x3f8) <= param_1 &&
            (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x3f8))))) {
          return 0x40;
        }
        iVar3 = *(int *)(this + 0x4d8);
        if ((((*(ushort *)(this + 0x406) <= param_2) &&
             (param_2 <= (uint)*(ushort *)(this + 0x406) + iVar3)) &&
            (*(ushort *)(this + 0x404) <= param_1)) &&
           (param_1 <= (uint)*(ushort *)(this + 0x404) + iVar3)) {
          return 0x100;
        }
        if (((uint)*(ushort *)(this + 0x424) - iVar3 <= param_1) &&
           (param_1 <= iVar3 + (uint)*(ushort *)(this + 0x424))) {
          if (((uint)*(ushort *)(this + 0x426) - *(int *)(this + 0x4dc) <= param_2) &&
             (param_2 <= (uint)*(ushort *)(this + 0x426) + *(int *)(this + 0x4dc))) {
            return 0x20;
          }
        }
      }
      else {
        iVar3 = *(int *)(this + 0x4d8);
        if ((((*(ushort *)(this + 0x3f2) <= param_1) &&
             (param_1 <= (uint)*(ushort *)(this + 0x3f2) + iVar3)) &&
            (*(ushort *)(this + 0x3f4) <= param_2)) &&
           (param_2 <= (uint)*(ushort *)(this + 0x3f4) + iVar3)) goto LAB_001768fc;
        if (param_1 <= iVar3 + (uint)*(ushort *)(this + 0x3ec)) {
          uVar4 = (uint)*(ushort *)(this + 0x3ec) - (iVar3 >> 1);
          bVar6 = param_1 <= uVar4;
          bVar5 = uVar4 == param_1;
          if (!bVar6 || bVar5) {
            uVar4 = (uint)*(ushort *)(this + 0x3ee);
            bVar6 = param_2 <= uVar4;
            bVar5 = uVar4 == param_2;
          }
          if ((!bVar6 || bVar5) && (param_2 <= uVar4 + iVar3)) {
            return 8;
          }
        }
        if (((*(ushort *)(this + 0x3e4) <= param_1) &&
            (param_1 <= (uint)*(ushort *)(this + 0x3e4) + *(int *)(this + 0x4dc))) &&
           ((*(ushort *)(this + 0x3e6) <= param_2 &&
            (param_2 <= *(int *)(this + 0x4dc) + (uint)*(ushort *)(this + 0x3e6))))) {
          return 0x10;
        }
        if ((((this[0x283] == (Hud)0x0) && (*(ushort *)(this + 0x416) <= param_1)) &&
            (param_1 <= (uint)*(ushort *)(this + 0x416) + (uint)*(ushort *)(this + 0x41a))) &&
           ((*(ushort *)(this + 0x418) <= param_2 &&
            (param_2 <= (uint)*(ushort *)(this + 0x418) + (uint)*(ushort *)(this + 0x41c))))) {
          return 4;
        }
        if (((*(ushort *)(this + 0x400) <= param_2) &&
            (param_2 <= (uint)*(ushort *)(this + 0x400) + iVar3)) &&
           ((*(ushort *)(this + 0x3fe) <= param_1 &&
            (param_1 <= iVar3 + (uint)*(ushort *)(this + 0x3fe))))) {
          return 0x20000000;
        }
      }
    }
    else {
      if ((((*(ushort *)(this + 0x40c) <= param_2) &&
           (param_2 <= (uint)*(ushort *)(this + 0x40c) + *(int *)(this + 0x4d8))) &&
          (*(ushort *)(this + 0x40a) <= param_1)) &&
         (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x40a))) {
        return 1;
      }
      if (((this[0x21e] != (Hud)0x0) && (*(ushort *)(this + 0x412) <= param_2)) &&
         ((param_2 <= (uint)*(ushort *)(this + 0x412) + *(int *)(this + 0x4d8) &&
          ((*(ushort *)(this + 0x410) <= param_1 &&
           (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x410))))))) {
        return 2;
      }
      if ((((*(ushort *)(this + 0x3fa) <= param_2) &&
           (param_2 <= (uint)*(ushort *)(this + 0x3fa) + *(int *)(this + 0x4d8))) &&
          (*(ushort *)(this + 0x3f8) <= param_1)) &&
         (param_1 <= *(int *)(this + 0x4d8) + (uint)*(ushort *)(this + 0x3f8))) {
        return 0x40;
      }
      iVar3 = *(int *)(this + 0x4d8);
      if (((*(ushort *)(this + 0x406) <= param_2) &&
          (param_2 <= (uint)*(ushort *)(this + 0x406) + iVar3)) &&
         ((*(ushort *)(this + 0x404) <= param_1 &&
          (param_1 <= (uint)*(ushort *)(this + 0x404) + iVar3)))) {
        return 0x100;
      }
      if (((uint)*(ushort *)(this + 0x424) - iVar3 <= param_1) &&
         (param_1 <= (uint)*(ushort *)(this + 0x424) + iVar3)) {
        if (((uint)*(ushort *)(this + 0x426) - *(int *)(this + 0x4dc) <= param_2) &&
           (param_2 <= (uint)*(ushort *)(this + 0x426) + *(int *)(this + 0x4dc))) {
          return 0x20;
        }
      }
      if ((((*(ushort *)(this + 0x3f2) <= param_1) &&
           (param_1 <= (uint)*(ushort *)(this + 0x3f2) + iVar3)) &&
          (*(ushort *)(this + 0x3f4) <= param_2)) &&
         (param_2 <= (uint)*(ushort *)(this + 0x3f4) + iVar3)) {
LAB_001768fc:
        *(undefined4 *)(this + 0x470) = 1000;
        return 0x80;
      }
      if ((((uint)*(ushort *)(this + 0x3ec) - (iVar3 >> 1) <= param_1) &&
          (param_1 <= (uint)*(ushort *)(this + 0x3ec) + iVar3)) &&
         ((*(ushort *)(this + 0x3ee) <= param_2 &&
          (param_2 <= (uint)*(ushort *)(this + 0x3ee) + iVar3)))) {
        return 8;
      }
      iVar1 = *(int *)(this + 0x4dc) >> 1;
      if ((((uint)*(ushort *)(this + 0x3e4) - iVar1 <= param_1) &&
          (param_1 <= (uint)*(ushort *)(this + 0x3e4) + iVar1)) &&
         (((uint)*(ushort *)(this + 0x3e6) - iVar1 <= param_2 &&
          (param_2 <= iVar1 + (uint)*(ushort *)(this + 0x3e6))))) {
        return 0x10;
      }
      if ((((this[0x283] == (Hud)0x0) && (*(ushort *)(this + 0x416) <= param_1)) &&
          (param_1 <= (uint)*(ushort *)(this + 0x416) + (uint)*(ushort *)(this + 0x41a))) &&
         ((*(ushort *)(this + 0x418) <= param_2 &&
          (param_2 <= (uint)*(ushort *)(this + 0x418) + (uint)*(ushort *)(this + 0x41c))))) {
        return 4;
      }
      if (((*(ushort *)(this + 0x400) <= param_2) &&
          (param_2 <= (uint)*(ushort *)(this + 0x400) + iVar3)) &&
         ((*(ushort *)(this + 0x3fe) <= param_1 &&
          (param_1 <= (uint)*(ushort *)(this + 0x3fe) + iVar3)))) {
        return 0x20000000;
      }
      if (this[0x528] != (Hud)0x0) {
        if ((((*(ushort *)(this + 0x456) <= param_2) &&
             (param_2 <= (uint)*(ushort *)(this + 0x456) + iVar3)) &&
            (*(ushort *)(this + 0x454) <= param_1)) &&
           (param_1 <= (uint)*(ushort *)(this + 0x454) + iVar3)) {
          return 0x200;
        }
        if (((*(ushort *)(this + 0x45a) <= param_2) &&
            (param_2 <= (uint)*(ushort *)(this + 0x45a) + iVar3)) &&
           ((*(ushort *)(this + 0x458) <= param_1 &&
            (param_1 <= (uint)*(ushort *)(this + 0x458) + iVar3)))) {
          return 0x400;
        }
        if (((*(ushort *)(this + 0x460) <= param_2) &&
            (param_2 <= (uint)*(ushort *)(this + 0x460) + iVar3)) &&
           ((*(ushort *)(this + 0x45e) <= param_1 &&
            (param_1 <= iVar3 + (uint)*(ushort *)(this + 0x45e))))) {
          return 0x800;
        }
      }
    }
  }
  else {
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      iVar3 = TouchButton::OnTouchBegin(*(int *)(puVar2[1] + uVar4 * 4),param_1);
      if (iVar3 != 0) {
        return **(undefined4 **)(*(int *)(*(int *)(this + 0x18) + 4) + uVar4 * 4);
      }
      puVar2 = *(uint **)(this + 0x18);
    }
  }
  return 0;
}

```

## target disasm
```
  00176574: push {r4,r5,r6,r7,lr}
  00176576: add r7,sp,#0xc
  00176578: push.w r8
  0017657c: mov r5,r0
  0017657e: ldrb.w r0,[r0,#0x282]
  00176582: mov r6,r2
  00176584: mov r8,r1
  00176586: cbz r0,0x001765b2
  00176588: ldr r0,[r5,#0x18]
  0017658a: cbz r0,0x001765b2
  0017658c: movs r4,#0x0
  0017658e: b 0x001765a8
  00176590: ldr r0,[r0,#0x4]
  00176592: mov r1,r8
  00176594: mov r2,r6
  00176596: ldr.w r0,[r0,r4,lsl #0x2]
  0017659a: blx 0x00074734
  0017659e: cmp r0,#0x0
  001765a0: bne.w 0x001766f4
  001765a4: ldr r0,[r5,#0x18]
  001765a6: adds r4,#0x1
  001765a8: ldr r1,[r0,#0x0]
  001765aa: cmp r4,r1
  001765ac: bcc 0x00176590
  001765ae: movs r0,#0x0
  001765b0: b 0x001769ca
  001765b2: ldr r0,[0x00176908]
  001765b4: add r0,pc
  001765b6: ldr r0,[r0,#0x0]
  001765b8: ldrb r0,[r0,#0x0]
  001765ba: cmp r0,#0x0
  001765bc: beq 0x00176646
  001765be: ldrh.w r1,[r5,#0x40c]
  001765c2: cmp r1,r6
  001765c4: bhi 0x001765d2
  001765c6: ldr.w r0,[r5,#0x4d8]
  001765ca: add r1,r0
  001765cc: cmp r1,r6
  001765ce: bcs.w 0x00176742
  001765d2: ldrb.w r0,[r5,#0x21e]
  001765d6: cbz r0,0x001765fc
  001765d8: ldrh.w r1,[r5,#0x412]
  001765dc: cmp r1,r6
  001765de: bhi 0x001765fc
  001765e0: ldr.w r0,[r5,#0x4d8]
  001765e4: add r1,r0
  001765e6: cmp r1,r6
  001765e8: bcc 0x001765fc
  001765ea: ldrh.w r1,[r5,#0x410]
  001765ee: cmp r1,r8
  001765f0: bhi 0x001765fc
  001765f2: add r0,r1
  001765f4: cmp r0,r8
  001765f6: bcc 0x001765fc
  001765f8: movs r0,#0x2
  001765fa: b 0x001769ca
  001765fc: ldrh.w r1,[r5,#0x3fa]
  00176600: cmp r1,r6
  00176602: bhi 0x00176620
  00176604: ldr.w r0,[r5,#0x4d8]
  00176608: add r1,r0
  0017660a: cmp r1,r6
  0017660c: bcc 0x00176620
  0017660e: ldrh.w r1,[r5,#0x3f8]
  00176612: cmp r1,r8
  00176614: bhi 0x00176620
  00176616: add r0,r1
  00176618: cmp r0,r8
  0017661a: bcc 0x00176620
  0017661c: movs r0,#0x40
  0017661e: b 0x001769ca
  00176620: ldrh.w r1,[r5,#0x406]
  00176624: ldr.w r0,[r5,#0x4d8]
  00176628: cmp r1,r6
  0017662a: bhi 0x001766cc
  0017662c: add r1,r0
  0017662e: cmp r1,r6
  00176630: bcc 0x001766cc
  00176632: ldrh.w r1,[r5,#0x404]
  00176636: cmp r1,r8
  00176638: bhi 0x001766cc
  0017663a: add r1,r0
  0017663c: cmp r1,r8
  0017663e: bcc 0x001766cc
  00176640: mov.w r0,#0x100
  00176644: b 0x001769ca
  00176646: ldrb.w r0,[r5,#0x528]
  0017664a: cbz r0,0x00176696
  0017664c: ldrh.w r1,[r5,#0x456]
  00176650: cmp r1,r6
  00176652: bhi 0x00176660
  00176654: ldr.w r0,[r5,#0x4d8]
  00176658: add r1,r0
  0017665a: cmp r1,r6
  0017665c: bcs.w 0x0017690c
  00176660: ldrh.w r1,[r5,#0x45a]
  00176664: cmp r1,r6
  00176666: bhi 0x00176674
  00176668: ldr.w r0,[r5,#0x4d8]
  0017666c: add r1,r0
  0017666e: cmp r1,r6
  00176670: bcs.w 0x001769b4
  00176674: ldrh.w r1,[r5,#0x460]
  00176678: cmp r1,r6
  0017667a: bhi 0x00176696
  0017667c: ldr.w r0,[r5,#0x4d8]
  00176680: add r1,r0
  00176682: cmp r1,r6
  00176684: bcc 0x00176696
  00176686: ldrh.w r1,[r5,#0x45e]
  0017668a: cmp r1,r8
  0017668c: bhi 0x00176696
  0017668e: add r0,r1
  00176690: cmp r0,r8
  00176692: bcs.w 0x001768d6
  00176696: ldr r0,[0x001769f4]
  00176698: add r0,pc
  0017669a: ldr r0,[r0,#0x0]
  0017669c: ldr r0,[r0,#0x0]
  0017669e: cmp.w r6,r0, asr #0x2
  001766a2: bcs 0x00176758
  001766a4: ldrh.w r1,[r5,#0x40c]
  001766a8: cmp r1,r6
  001766aa: bhi.w 0x001765ae
  001766ae: ldr.w r0,[r5,#0x4d8]
  001766b2: add r1,r0
  001766b4: cmp r1,r6
  001766b6: bcc.w 0x001765ae
  001766ba: ldrh.w r1,[r5,#0x40a]
  001766be: cmp r1,r8
  001766c0: bhi.w 0x001765ae
  001766c4: add r0,r1
  001766c6: cmp r0,r8
  001766c8: bcs 0x00176754
  001766ca: b 0x001765ae
  001766cc: ldrh.w r1,[r5,#0x424]
  001766d0: subs r2,r1,r0
  001766d2: cmp r2,r8
  001766d4: bhi 0x00176700
  001766d6: add r1,r0
  001766d8: cmp r1,r8
  001766da: bcc 0x00176700
  001766dc: ldrh.w r1,[r5,#0x426]
  001766e0: ldr.w r2,[r5,#0x4dc]
  001766e4: subs r3,r1,r2
  001766e6: cmp r3,r6
  001766e8: bhi 0x00176700
  001766ea: add r1,r2
  001766ec: cmp r1,r6
  001766ee: bcc 0x00176700
  001766f0: movs r0,#0x20
  001766f2: b 0x001769ca
  001766f4: ldr r0,[r5,#0x18]
  001766f6: ldr r0,[r0,#0x4]
  001766f8: ldr.w r0,[r0,r4,lsl #0x2]
  001766fc: ldr r0,[r0,#0x0]
  001766fe: b 0x001769ca
  00176700: ldrh.w r1,[r5,#0x3f2]
  00176704: cmp r1,r8
  00176706: bhi 0x0017671e
  00176708: add r1,r0
  0017670a: cmp r1,r8
  0017670c: bcc 0x0017671e
  0017670e: ldrh.w r1,[r5,#0x3f4]
  00176712: cmp r1,r6
  00176714: bhi 0x0017671e
  00176716: add r1,r0
  00176718: cmp r1,r6
  0017671a: bcs.w 0x001768fc
  0017671e: ldrh.w r1,[r5,#0x3ec]
  00176722: sub.w r2,r1,r0, asr #0x1
  00176726: cmp r2,r8
  00176728: bhi 0x001767f6
  0017672a: add r1,r0
  0017672c: cmp r1,r8
  0017672e: bcc 0x001767f6
  00176730: ldrh.w r1,[r5,#0x3ee]
  00176734: cmp r1,r6
  00176736: bhi 0x001767f6
  00176738: add r1,r0
  0017673a: cmp r1,r6
  0017673c: bcc 0x001767f6
  0017673e: movs r0,#0x8
  00176740: b 0x001769ca
  00176742: ldrh.w r1,[r5,#0x40a]
  00176746: cmp r1,r8
  00176748: bhi.w 0x001765d2
  0017674c: add r0,r1
  0017674e: cmp r0,r8
  00176750: bcc.w 0x001765d2
  00176754: movs r0,#0x1
  00176756: b 0x001769ca
  00176758: ldr r0,[0x001769f8]
  0017675a: add r0,pc
  0017675c: ldr r0,[r0,#0x0]
  0017675e: ldr r0,[r0,#0x0]
  00176760: cmp.w r8,r0, asr #0x1
  00176764: bcs.w 0x001768dc
  00176768: ldrb.w r0,[r5,#0x21e]
  0017676c: cbz r0,0x00176792
  0017676e: ldrh.w r1,[r5,#0x412]
  00176772: cmp r1,r6
  00176774: bhi 0x00176792
  00176776: ldr.w r0,[r5,#0x4d8]
  0017677a: add r1,r0
  0017677c: cmp r1,r6
  0017677e: bcc 0x00176792
  00176780: ldrh.w r1,[r5,#0x410]
  00176784: subs r2,r1,r0
  00176786: cmp r2,r8
  00176788: bhi 0x00176792
  0017678a: add r0,r1
  0017678c: cmp r0,r8
  0017678e: bcs.w 0x001765f8
  00176792: ldrh.w r1,[r5,#0x3fa]
  00176796: cmp r1,r6
  00176798: bhi 0x001767a6
  0017679a: ldr.w r0,[r5,#0x4d8]
  0017679e: add r1,r0
  001767a0: cmp r1,r6
  001767a2: bcs.w 0x001769d0
  001767a6: ldrh.w r1,[r5,#0x406]
  001767aa: ldr.w r0,[r5,#0x4d8]
  001767ae: cmp r1,r6
  001767b0: bhi 0x001767c8
  001767b2: add r1,r0
  001767b4: cmp r1,r6
  001767b6: bcc 0x001767c8
  001767b8: ldrh.w r1,[r5,#0x404]
  001767bc: cmp r1,r8
  001767be: bhi 0x001767c8
  001767c0: add r1,r0
  001767c2: cmp r1,r8
  001767c4: bcs.w 0x00176640
  001767c8: ldrh.w r1,[r5,#0x424]
  001767cc: subs r2,r1,r0
  001767ce: cmp r2,r8
  001767d0: bhi.w 0x001765ae
  001767d4: add r0,r1
  001767d6: cmp r0,r8
  001767d8: bcc.w 0x001765ae
  001767dc: ldrh.w r0,[r5,#0x426]
  001767e0: ldr.w r1,[r5,#0x4dc]
  001767e4: subs r2,r0,r1
  001767e6: cmp r2,r6
  001767e8: bhi.w 0x001765ae
  001767ec: add r0,r1
  001767ee: cmp r0,r6
  001767f0: bcs.w 0x001766f0
  001767f4: b 0x001765ae
  001767f6: ldr.w r1,[r5,#0x4dc]
  001767fa: ldrh.w r2,[r5,#0x3e4]
  001767fe: sub.w r3,r2,r1, asr #0x1
  00176802: cmp r3,r8
  00176804: bhi 0x00176824
  00176806: add.w r2,r2,r1, asr #0x1
  0017680a: cmp r2,r8
  0017680c: bcc 0x00176824
  0017680e: ldrh.w r2,[r5,#0x3e6]
  00176812: asrs r1,r1,#0x1
  00176814: subs r3,r2,r1
  00176816: cmp r3,r6
  00176818: bhi 0x00176824
  0017681a: add r1,r2
  0017681c: cmp r1,r6
  0017681e: bcc 0x00176824
  00176820: movs r0,#0x10
  00176822: b 0x001769ca
  00176824: ldrb.w r1,[r5,#0x283]
  00176828: cbnz r1,0x00176850
  0017682a: ldrh.w r1,[r5,#0x416]
  0017682e: cmp r1,r8
  00176830: bhi 0x00176850
  00176832: ldrh.w r2,[r5,#0x41a]
  00176836: add r1,r2
  00176838: cmp r1,r8
  0017683a: bcc 0x00176850
  0017683c: ldrh.w r1,[r5,#0x418]
  00176840: cmp r1,r6
  00176842: bhi 0x00176850
  00176844: ldrh.w r2,[r5,#0x41c]
  00176848: add r1,r2
  0017684a: cmp r1,r6
  0017684c: bcs.w 0x001769ee
  00176850: ldrh.w r1,[r5,#0x400]
  00176854: cmp r1,r6
  00176856: bhi 0x0017686e
  00176858: add r1,r0
  0017685a: cmp r1,r6
  0017685c: bcc 0x0017686e
  0017685e: ldrh.w r1,[r5,#0x3fe]
  00176862: cmp r1,r8
  00176864: bhi 0x0017686e
  00176866: add r1,r0
  00176868: cmp r1,r8
  0017686a: bcs.w 0x001769ae
  0017686e: ldrb.w r1,[r5,#0x528]
  00176872: cmp r1,#0x0
  00176874: beq.w 0x001765ae
  00176878: ldrh.w r1,[r5,#0x456]
  0017687c: cmp r1,r6
  0017687e: bhi 0x00176894
  00176880: add r1,r0
  00176882: cmp r1,r6
  00176884: bcc 0x00176894
  00176886: ldrh.w r1,[r5,#0x454]
  0017688a: cmp r1,r8
  0017688c: bhi 0x00176894
  0017688e: add r1,r0
  00176890: cmp r1,r8
  00176892: bcs 0x0017691e
  00176894: ldrh.w r1,[r5,#0x45a]
  00176898: cmp r1,r6
  0017689a: bhi 0x001768b2
  0017689c: add r1,r0
  0017689e: cmp r1,r6
  001768a0: bcc 0x001768b2
  001768a2: ldrh.w r1,[r5,#0x458]
  001768a6: cmp r1,r8
  001768a8: bhi 0x001768b2
  001768aa: add r1,r0
  001768ac: cmp r1,r8
  001768ae: bcs.w 0x001769c6
  001768b2: ldrh.w r1,[r5,#0x460]
  001768b6: cmp r1,r6
  001768b8: bhi.w 0x001765ae
  001768bc: add r1,r0
  001768be: cmp r1,r6
  001768c0: bcc.w 0x001765ae
  001768c4: ldrh.w r1,[r5,#0x45e]
  001768c8: cmp r1,r8
  001768ca: bhi.w 0x001765ae
  001768ce: add r0,r1
  001768d0: cmp r0,r8
  001768d2: bcc.w 0x001765ae
  001768d6: mov.w r0,#0x800
  001768da: b 0x001769ca
  001768dc: ldrh.w r1,[r5,#0x3f2]
  001768e0: ldr.w r0,[r5,#0x4d8]
  001768e4: cmp r1,r8
  001768e6: bhi 0x00176924
  001768e8: add r1,r0
  001768ea: cmp r1,r8
  001768ec: bcc 0x00176924
  001768ee: ldrh.w r1,[r5,#0x3f4]
  001768f2: cmp r1,r6
  001768f4: bhi 0x00176924
  001768f6: add r1,r0
  001768f8: cmp r1,r6
  001768fa: bcc 0x00176924
  001768fc: mov.w r0,#0x3e8
  00176900: str.w r0,[r5,#0x470]
  00176904: movs r0,#0x80
  00176906: b 0x001769ca
  0017690c: ldrh.w r1,[r5,#0x454]
  00176910: cmp r1,r8
  00176912: bhi.w 0x00176660
  00176916: add r0,r1
  00176918: cmp r0,r8
  0017691a: bcc.w 0x00176660
  0017691e: mov.w r0,#0x200
  00176922: b 0x001769ca
  00176924: ldrh.w r1,[r5,#0x3ec]
  00176928: adds r2,r0,r1
  0017692a: cmp r2,r8
  0017692c: bcc 0x0017693e
  0017692e: sub.w r1,r1,r0, asr #0x1
  00176932: cmp r1,r8
  00176934: itt ls
  00176936: ldrh.w.ls r1,[r5,#0x3ee]
  0017693a: cmp.ls r1,r6
  0017693c: bls 0x001769e4
  0017693e: ldrh.w r2,[r5,#0x3e4]
  00176942: cmp r2,r8
  00176944: bhi 0x00176960
  00176946: ldr.w r1,[r5,#0x4dc]
  0017694a: add r2,r1
  0017694c: cmp r2,r8
  0017694e: bcc 0x00176960
  00176950: ldrh.w r2,[r5,#0x3e6]
  00176954: cmp r2,r6
  00176956: bhi 0x00176960
  00176958: add r1,r2
  0017695a: cmp r1,r6
  0017695c: bcs.w 0x00176820
  00176960: ldrb.w r1,[r5,#0x283]
  00176964: cbnz r1,0x0017698a
  00176966: ldrh.w r1,[r5,#0x416]
  0017696a: cmp r1,r8
  0017696c: bhi 0x0017698a
  0017696e: ldrh.w r2,[r5,#0x41a]
  00176972: add r1,r2
  00176974: cmp r1,r8
  00176976: bcc 0x0017698a
  00176978: ldrh.w r1,[r5,#0x418]
  0017697c: cmp r1,r6
  0017697e: bhi 0x0017698a
  00176980: ldrh.w r2,[r5,#0x41c]
  00176984: add r1,r2
  00176986: cmp r1,r6
  00176988: bcs 0x001769ee
  0017698a: ldrh.w r1,[r5,#0x400]
  0017698e: cmp r1,r6
  00176990: bhi.w 0x001765ae
  00176994: add r1,r0
  00176996: cmp r1,r6
  00176998: bcc.w 0x001765ae
  0017699c: ldrh.w r1,[r5,#0x3fe]
  001769a0: cmp r1,r8
  001769a2: bhi.w 0x001765ae
  001769a6: add r0,r1
  001769a8: cmp r0,r8
  001769aa: bcc.w 0x001765ae
  001769ae: mov.w r0,#0x20000000
  001769b2: b 0x001769ca
  001769b4: ldrh.w r1,[r5,#0x458]
  001769b8: cmp r1,r8
  001769ba: bhi.w 0x00176674
  001769be: add r0,r1
  001769c0: cmp r0,r8
  001769c2: bcc.w 0x00176674
  001769c6: mov.w r0,#0x400
  001769ca: pop.w r8
  001769ce: pop {r4,r5,r6,r7,pc}
  001769d0: ldrh.w r1,[r5,#0x3f8]
  001769d4: cmp r1,r8
  001769d6: bhi.w 0x001767a6
  001769da: add r0,r1
  001769dc: cmp r0,r8
  001769de: bcs.w 0x0017661c
  001769e2: b 0x001767a6
  001769e4: add r1,r0
  001769e6: cmp r1,r6
  001769e8: bcs.w 0x0017673e
  001769ec: b 0x0017693e
  001769ee: movs r0,#0x4
  001769f0: b 0x001769ca
```
