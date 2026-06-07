# PlayerFighter::handleCloaking
elf 0xdcdbc body 498
Sig: undefined __stdcall handleCloaking(void)

## decompile
```c

/* PlayerFighter::handleCloaking() */

void PlayerFighter::handleCloaking(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  undefined4 *puVar6;
  PaintCanvas *this;
  bool bVar7;
  uint in_fpscr;
  float fVar8;
  float fVar9;
  
  if ((((*(int *)(in_r0 + 0x28) == 10) && (*(int *)(in_r0 + 8) != 0)) &&
      (iVar1 = KIPlayer::isDead(), iVar1 == 0)) &&
     ((*(char *)(*(int *)(in_r0 + 4) + 0x68) == '\0' && (*(char *)(in_r0 + 0x2d8) != '\0')))) {
    if (*(char *)(in_r0 + 0x2d0) != '\0') {
      iVar1 = *(int *)(in_r0 + 0x2c8);
      if (iVar1 == 0) {
        uVar4 = *(uint *)(in_r0 + 0x2dc);
        *(undefined1 *)(in_r0 + 0x13c) = 1;
        if (uVar4 == 0xffffffff) {
          puVar6 = *(undefined4 **)(DAT_000ecfb8 + 0xece0e);
          AbyssEngine::PaintCanvas::MeshCloneMaterial
                    ((PaintCanvas *)*puVar6,*(uint *)(*(int *)(in_r0 + 0xc) + 0x1c),
                     (uint *)(in_r0 + 0x2dc));
          iVar1 = AbyssEngine::PaintCanvas::MeshGetPointer
                            ((PaintCanvas *)*puVar6,*(uint *)(*(int *)(in_r0 + 0xc) + 0x1c));
          uVar4 = *(uint *)(in_r0 + 0x2dc);
          *(undefined4 *)(in_r0 + 0x2e0) = *(undefined4 *)(*(int *)(iVar1 + 0x30) + 0x20);
        }
        puVar6 = *(undefined4 **)(DAT_000ecfbc + 0xece32);
        iVar1 = AbyssEngine::PaintCanvas::MaterialGetMaterial((PaintCanvas *)*puVar6,uVar4);
        *(undefined4 *)(iVar1 + 0x20) = 0xe;
        AbyssEngine::PaintCanvas::MeshChangeMaterial
                  ((PaintCanvas *)*puVar6,*(uint *)(*(int *)(in_r0 + 0xc) + 0x1c),
                   *(ushort *)(in_r0 + 0x2dc));
        if (*(char *)(in_r0 + 0x2d9) == '\0') {
          iVar1 = *(int *)(in_r0 + 0x2c8);
        }
        else {
          *(undefined1 *)(in_r0 + 0x2d9) = 0;
          iVar1 = 2000;
          *(undefined4 *)(in_r0 + 0x2c8) = 2000;
        }
      }
      uVar5 = *(uint *)(in_r0 + 0x1d0);
      uVar4 = iVar1 + uVar5;
      *(uint *)(in_r0 + 0x2c8) = uVar4;
      iVar1 = (((int)uVar4 >> 0x1f) - *(int *)(in_r0 + 0x1d4)) - (uint)(uVar4 < uVar5);
      bVar7 = 2000 < uVar4 - uVar5;
      if ((int)(-(uint)bVar7 - iVar1) < 0 == (SBORROW4(0,iVar1) != SBORROW4(-iVar1,(uint)bVar7))) {
        if (1999 < (int)uVar4) {
          setExhaustVisible(SUB41(in_r0,0));
          *(undefined1 *)(in_r0 + 0x74) = 1;
        }
        this = (PaintCanvas *)**(undefined4 **)(DAT_000ecfc0 + 0xecef6);
        uVar3 = AbyssEngine::PaintCanvas::MeshGetPointer
                          (this,*(uint *)(*(int *)(in_r0 + 0xc) + 0x1c));
        fVar8 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x2c8),
                                           (byte)(in_fpscr >> 0x16) & 3);
        fVar8 = fVar8 / DAT_000ecfac;
      }
      else {
        if (*(int *)(in_r0 + 0x2cc) < (int)uVar4) {
          uVar3 = *(undefined4 *)(in_r0 + 0x2e0);
          puVar6 = (undefined4 *)(DAT_000ecfc8 + 0xecf28);
          *(undefined4 *)(in_r0 + 0x2c8) = 0;
          *(undefined1 *)(in_r0 + 0x2d0) = 0;
          puVar6 = (undefined4 *)*puVar6;
          *(undefined1 *)(in_r0 + 0x13c) = 0;
          iVar1 = AbyssEngine::PaintCanvas::MaterialGetMaterial
                            ((PaintCanvas *)*puVar6,*(uint *)(in_r0 + 0x2dc));
          *(undefined4 *)(iVar1 + 0x20) = uVar3;
          (*(code *)(DAT_001ac204 + 0x1ac208))();
          return;
        }
        if ((int)uVar4 <= *(int *)(in_r0 + 0x2cc) + -2000) {
          return;
        }
        puVar6 = (undefined4 *)(DAT_000ecfc4 + 0xecf5c);
        *(undefined1 *)(in_r0 + 0x74) = 0;
        this = *(PaintCanvas **)*puVar6;
        uVar3 = AbyssEngine::PaintCanvas::MeshGetPointer
                          (this,*(uint *)(*(int *)(in_r0 + 0xc) + 0x1c));
        fVar8 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x2c8),
                                           (byte)(in_fpscr >> 0x16) & 3);
        fVar9 = (float)VectorSignedToFloat(*(int *)(in_r0 + 0x2cc) + -2000,
                                           (byte)(in_fpscr >> 0x16) & 3);
        fVar8 = (fVar8 - fVar9) / DAT_000ecfa8 + 1.0;
      }
      (*(code *)(DAT_001ac214 + 0x1ac218))(this,uVar3,fVar8,1);
      return;
    }
    if ((*(char *)(in_r0 + 0x1e0) != '\0') &&
       (iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000ecfb0 + 0xece6e)), iVar1 < 0x32)) {
LAB_000ecea4:
      (*(code *)(DAT_001ac1f4 + 0x1ac1f8))();
      return;
    }
    iVar2 = *(int *)(in_r0 + 0x1d0) + *(int *)(in_r0 + 0x2d4);
    *(int *)(in_r0 + 0x2d4) = iVar2;
    iVar1 = DAT_000ecfb4;
    if (8000 < iVar2) {
      *(undefined4 *)(in_r0 + 0x2d4) = 0;
      iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(iVar1 + 0xece9a));
      if (iVar1 < 0x1e) goto LAB_000ecea4;
    }
  }
  return;
}

```

## target disasm
```
  000ecdbc: push {r4,r5,r7,lr}
  000ecdbe: add r7,sp,#0x8
  000ecdc0: mov r4,r0
  000ecdc2: ldr r0,[r0,#0x28]
  000ecdc4: cmp r0,#0xa
  000ecdc6: bne 0x000ecddc
  000ecdc8: ldr r0,[r4,#0x8]
  000ecdca: cbz r0,0x000ecddc
  000ecdcc: mov r0,r4
  000ecdce: blx 0x00071ec0
  000ecdd2: cbnz r0,0x000ecddc
  000ecdd4: ldr r0,[r4,#0x4]
  000ecdd6: ldrb.w r0,[r0,#0x68]
  000ecdda: cbz r0,0x000ecdde
  000ecddc: pop {r4,r5,r7,pc}
  000ecdde: ldrb.w r0,[r4,#0x2d8]
  000ecde2: cmp r0,#0x0
  000ecde4: beq 0x000ecddc
  000ecde6: ldrb.w r0,[r4,#0x2d0]
  000ecdea: cbz r0,0x000ece60
  000ecdec: ldr.w r0,[r4,#0x2c8]
  000ecdf0: cmp r0,#0x0
  000ecdf2: bne 0x000eceb8
  000ecdf4: ldr.w r1,[r4,#0x2dc]
  000ecdf8: movs r0,#0x1
  000ecdfa: strb.w r0,[r4,#0x13c]
  000ecdfe: adds r0,r1,#0x1
  000ece00: bne 0x000ece2c
  000ece02: ldr r0,[0x000ecfb8]
  000ece04: add.w r2,r4,#0x2dc
  000ece08: ldr r1,[r4,#0xc]
  000ece0a: add r0,pc
  000ece0c: ldr r5,[r0,#0x0]
  000ece0e: ldr r1,[r1,#0x1c]
  000ece10: ldr r0,[r5,#0x0]
  000ece12: blx 0x000723b8
  000ece16: ldr r1,[r4,#0xc]
  000ece18: ldr r0,[r5,#0x0]
  000ece1a: ldr r1,[r1,#0x1c]
  000ece1c: blx 0x00072370
  000ece20: ldr r0,[r0,#0x30]
  000ece22: ldr.w r1,[r4,#0x2dc]
  000ece26: ldr r0,[r0,#0x20]
  000ece28: str.w r0,[r4,#0x2e0]
  000ece2c: ldr r0,[0x000ecfbc]
  000ece2e: add r0,pc
  000ece30: ldr r5,[r0,#0x0]
  000ece32: ldr r0,[r5,#0x0]
  000ece34: blx 0x00072430
  000ece38: movs r1,#0xe
  000ece3a: str r1,[r0,#0x20]
  000ece3c: ldr r1,[r4,#0xc]
  000ece3e: ldr r0,[r5,#0x0]
  000ece40: ldrh.w r2,[r4,#0x2dc]
  000ece44: ldr r1,[r1,#0x1c]
  000ece46: blx 0x0007243c
  000ece4a: ldrb.w r0,[r4,#0x2d9]
  000ece4e: cbz r0,0x000eceb4
  000ece50: movs r0,#0x0
  000ece52: strb.w r0,[r4,#0x2d9]
  000ece56: mov.w r0,#0x7d0
  000ece5a: str.w r0,[r4,#0x2c8]
  000ece5e: b 0x000eceb8
  000ece60: ldrb.w r0,[r4,#0x1e0]
  000ece64: cbz r0,0x000ece78
  000ece66: ldr r0,[0x000ecfb0]
  000ece68: movs r1,#0x64
  000ece6a: add r0,pc
  000ece6c: ldr r0,[r0,#0x0]
  000ece6e: ldr r0,[r0,#0x0]
  000ece70: blx 0x00071848
  000ece74: cmp r0,#0x31
  000ece76: ble 0x000ecea4
  000ece78: ldr.w r0,[r4,#0x1d0]
  000ece7c: ldr.w r1,[r4,#0x2d4]
  000ece80: add r0,r1
  000ece82: str.w r0,[r4,#0x2d4]
  000ece86: cmp.w r0,#0x1f40
  000ece8a: ble 0x000ecddc
  000ece8c: ldr r0,[0x000ecfb4]
  000ece8e: movs r1,#0x0
  000ece90: str.w r1,[r4,#0x2d4]
  000ece94: movs r1,#0x64
  000ece96: add r0,pc
  000ece98: ldr r0,[r0,#0x0]
  000ece9a: ldr r0,[r0,#0x0]
  000ece9c: blx 0x00071848
  000ecea0: cmp r0,#0x1d
  000ecea2: bgt 0x000ecddc
  000ecea4: mov r0,r4
  000ecea6: mov.w r1,#0xffffffff
  000eceaa: movs r2,#0x0
  000eceac: pop.w {r4,r5,r7,lr}
  000eceb0: b.w 0x001ac1e8
  000eceb4: ldr.w r0,[r4,#0x2c8]
  000eceb8: ldrd r1,r2,[r4,#0x1d0]
  000ecebc: add r0,r1
  000ecebe: str.w r0,[r4,#0x2c8]
  000ecec2: subs r1,r0,r1
  000ecec4: asr.w r3,r0, asr #0x1f
  000ecec8: sbc.w r2,r3,r2
  000ececc: rsbs.w r1,r1,#0x7d0
  000eced0: mov.w r3,#0x0
  000eced4: sbcs.w r1,r3,r2
  000eced8: blt 0x000ecf14
  000eceda: cmp.w r0,#0x7d0
  000ecede: blt 0x000eceee
  000ecee0: mov r0,r4
  000ecee2: movs r1,#0x0
  000ecee4: blx 0x000721f0
  000ecee8: movs r0,#0x1
  000eceea: strb.w r0,[r4,#0x74]
  000eceee: ldr r0,[0x000ecfc0]
  000ecef0: ldr r1,[r4,#0xc]
  000ecef2: add r0,pc
  000ecef4: ldr r0,[r0,#0x0]
  000ecef6: ldr r1,[r1,#0x1c]
  000ecef8: ldr r5,[r0,#0x0]
  000ecefa: mov r0,r5
  000ecefc: blx 0x00072370
  000ecf00: vldr.32 s0,[r4,#0x2c8]
  000ecf04: mov r1,r0
  000ecf06: vldr.32 s2,[pc,#0xa4]
  000ecf0a: vcvt.f32.s32 s0,s0
  000ecf0e: vdiv.f32 s0,s0,s2
  000ecf12: b 0x000ecf98
  000ecf14: ldr.w r1,[r4,#0x2cc]
  000ecf18: cmp r0,r1
  000ecf1a: ble 0x000ecf48
  000ecf1c: ldr r0,[0x000ecfc8]
  000ecf1e: movs r2,#0x0
  000ecf20: ldrd r1,r5,[r4,#0x2dc]
  000ecf24: add r0,pc
  000ecf26: str.w r2,[r4,#0x2c8]
  000ecf2a: strb.w r2,[r4,#0x2d0]
  000ecf2e: ldr r0,[r0,#0x0]
  000ecf30: strb.w r2,[r4,#0x13c]
  000ecf34: ldr r0,[r0,#0x0]
  000ecf36: blx 0x00072430
  000ecf3a: str r5,[r0,#0x20]
  000ecf3c: mov r0,r4
  000ecf3e: movs r1,#0x1
  000ecf40: pop.w {r4,r5,r7,lr}
  000ecf44: b.w 0x001ac1f8
  000ecf48: sub.w r1,r1,#0x7d0
  000ecf4c: cmp r0,r1
  000ecf4e: it le
  000ecf50: pop.le {r4,r5,r7,pc}
  000ecf52: ldr r0,[0x000ecfc4]
  000ecf54: movs r2,#0x0
  000ecf56: ldr r1,[r4,#0xc]
  000ecf58: add r0,pc
  000ecf5a: strb.w r2,[r4,#0x74]
  000ecf5e: ldr r0,[r0,#0x0]
  000ecf60: ldr r1,[r1,#0x1c]
  000ecf62: ldr r5,[r0,#0x0]
  000ecf64: mov r0,r5
  000ecf66: blx 0x00072370
  000ecf6a: mov r1,r0
  000ecf6c: ldr.w r0,[r4,#0x2cc]
  000ecf70: vldr.32 s0,[r4,#0x2c8]
  000ecf74: sub.w r0,r0,#0x7d0
  000ecf78: vcvt.f32.s32 s0,s0
  000ecf7c: vmov s2,r0
  000ecf80: vcvt.f32.s32 s2,s2
  000ecf84: vsub.f32 s0,s0,s2
  000ecf88: vldr.32 s2,[pc,#0x1c]
  000ecf8c: vdiv.f32 s0,s0,s2
  000ecf90: vmov.f32 s2,0x3f800000
  000ecf94: vadd.f32 s0,s0,s2
  000ecf98: vmov r2,s0
  000ecf9c: mov r0,r5
  000ecf9e: movs r3,#0x1
  000ecfa0: pop.w {r4,r5,r7,lr}
  000ecfa4: b.w 0x001ac208
  001ac1e8: bx pc
  001ac1f8: bx pc
  001ac208: bx pc
```
