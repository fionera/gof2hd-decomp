# PaintCanvas::DrawTransform
elf 0x798e4 body 206
Sig: undefined __thiscall DrawTransform(PaintCanvas * this, Transform * param_1, Matrix * param_2, Matrix * param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawTransform(AbyssEngine::Transform*, AbyssEngine::AEMath::Matrix
   const&, AbyssEngine::AEMath::Matrix&) */

void __thiscall
AbyssEngine::PaintCanvas::DrawTransform
          (PaintCanvas *this,Transform *param_1,Matrix *param_2,Matrix *param_3)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  Matrix aMStack_64 [60];
  int local_28;
  
  piVar3 = *(int **)(DAT_000899b4 + 0x898fa);
  local_28 = *piVar3;
  if ((param_1 != (Transform *)0x0) && (param_1[0xec] != (Transform)0x0)) {
    AEMath::operator*(aMStack_64,param_2);
    if (*(int *)(param_1 + 0x11c) != 0) {
      AEMath::Matrix::operator*=(aMStack_64,(Matrix *)(param_1 + 0x5c));
    }
    for (uVar2 = 0; uVar2 < *(uint *)(param_1 + 0x3c); uVar2 = uVar2 + 1) {
      DrawMesh(this,*(Mesh **)(*(int *)(param_1 + 0x40) + uVar2 * 4),aMStack_64,param_3,
               *(uint *)(param_1 + 0x48),(Matrix *)(param_1 + 0x98));
    }
    for (uVar2 = 0; uVar2 < *(uint *)(param_1 + 0x4c); uVar2 = uVar2 + 1) {
      if ((*(uint *)(this + 0x170) < *(uint *)(this + 0x164)) &&
         (iVar1 = Transform::InCameraVF
                            (*(Transform **)(*(int *)(param_1 + 0x50) + uVar2 * 4),aMStack_64,
                             *(Camera **)(*(int *)(this + 0x168) + *(uint *)(this + 0x170) * 4)),
         iVar1 != 0)) {
        DrawTransform(this,*(Transform **)(*(int *)(param_1 + 0x50) + uVar2 * 4),aMStack_64,param_3)
        ;
      }
      else {
        *(int *)(this + 4) = *(int *)(this + 4) + 1;
      }
    }
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
  000898e4: push {r4,r5,r6,r7,lr}
  000898e6: add r7,sp,#0xc
  000898e8: push {r8,r9,r10,r11}
  000898ec: sub sp,#0x4c
  000898ee: mov r6,r0
  000898f0: ldr r0,[0x000899b4]
  000898f2: mov r9,r3
  000898f4: mov r5,r1
  000898f6: add r0,pc
  000898f8: cmp r1,#0x0
  000898fa: ldr.w r10,[r0,#0x0]
  000898fe: ldr.w r0,[r10,#0x0]
  00089902: str r0,[sp,#0x48]
  00089904: beq 0x0008999c
  00089906: ldrb.w r0,[r5,#0xec]
  0008990a: cmp r0,#0x0
  0008990c: beq 0x0008999c
  0008990e: add r0,sp,#0xc
  00089910: mov r1,r2
  00089912: mov r2,r5
  00089914: blx 0x0006f82c
  00089918: ldr.w r0,[r5,#0x11c]
  0008991c: cbz r0,0x00089928
  0008991e: add.w r1,r5,#0x5c
  00089922: add r0,sp,#0xc
  00089924: blx 0x0006f4a8
  00089928: add.w r11,r5,#0x98
  0008992c: add.w r8,sp,#0xc
  00089930: movs r4,#0x0
  00089932: b 0x0008994c
  00089934: ldr r0,[r5,#0x40]
  00089936: mov r3,r9
  00089938: ldr r2,[r5,#0x48]
  0008993a: ldr.w r1,[r0,r4,lsl #0x2]
  0008993e: mov r0,r6
  00089940: strd r2,r11,[sp,#0x0]
  00089944: mov r2,r8
  00089946: blx 0x0006fb74
  0008994a: adds r4,#0x1
  0008994c: ldr r0,[r5,#0x3c]
  0008994e: cmp r4,r0
  00089950: bcc 0x00089934
  00089952: add.w r8,sp,#0xc
  00089956: movs r4,#0x0
  00089958: b 0x00089996
  0008995a: ldr.w r1,[r6,#0x164]
  0008995e: ldr.w r0,[r6,#0x170]
  00089962: cmp r0,r1
  00089964: bcs 0x0008998e
  00089966: ldr.w r1,[r6,#0x168]
  0008996a: ldr r3,[r5,#0x50]
  0008996c: ldr.w r2,[r1,r0,lsl #0x2]
  00089970: mov r1,r8
  00089972: ldr.w r0,[r3,r4,lsl #0x2]
  00089976: blx 0x0006fb44
  0008997a: cbz r0,0x0008998e
  0008997c: ldr r0,[r5,#0x50]
  0008997e: mov r2,r8
  00089980: mov r3,r9
  00089982: ldr.w r1,[r0,r4,lsl #0x2]
  00089986: mov r0,r6
  00089988: blx 0x0006fb68
  0008998c: b 0x00089994
  0008998e: ldr r0,[r6,#0x4]
  00089990: adds r0,#0x1
  00089992: str r0,[r6,#0x4]
  00089994: adds r4,#0x1
  00089996: ldr r0,[r5,#0x4c]
  00089998: cmp r4,r0
  0008999a: bcc 0x0008995a
  0008999c: ldr r0,[sp,#0x48]
  0008999e: ldr.w r1,[r10,#0x0]
  000899a2: subs r0,r1,r0
  000899a4: ittt eq
  000899a6: add.eq sp,#0x4c
  000899a8: pop.eq.w {r8,r9,r10,r11}
  000899ac: pop.eq {r4,r5,r6,r7,pc}
  000899ae: blx 0x0006e824
```
