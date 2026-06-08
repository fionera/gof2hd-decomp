# Material::Material
elf 0x7a3d8 body 212
Sig: undefined __thiscall Material(Material * this, Material * param_1)

## decompile
```c

/* AbyssEngine::Material::Material(AbyssEngine::Material*) */

void __thiscall AbyssEngine::Material::Material(Material *this,Material *param_1)

{
  Vector *this_00;
  int iVar1;
  int iVar2;
  uint uVar3;
  void *pvVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_0008a4e8 + 0x8a3ee);
  iVar1 = *piVar5;
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x2c));
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x38));
  Array<AbyssEngine::Mesh*>::Array((Array<AbyssEngine::Mesh*> *)(this + 0x44));
  Array<unsigned_int>::Array((Array<unsigned_int> *)(this + 0x50));
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x5c));
  this_00 = (Vector *)(this + 0x68);
  *(undefined4 *)this_00 = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  if (param_1 == (Material *)0x0) {
    *(undefined4 *)(this + 0x20) = 0;
    AEMath::Vector::operator=(this_00,(Vector *)&stack0xffffffcc);
    for (iVar2 = 0; iVar2 != 8; iVar2 = iVar2 + 1) {
      *(undefined4 *)(this + iVar2 * 4) = 0xffffffff;
    }
  }
  else {
    for (iVar2 = 0; iVar2 != 8; iVar2 = iVar2 + 1) {
      *(undefined4 *)(this + iVar2 * 4) = *(undefined4 *)(param_1 + iVar2 * 4);
    }
    *(undefined4 *)(this + 0x20) = *(undefined4 *)(param_1 + 0x20);
    uVar3 = *(uint *)(param_1 + 0x28);
    *(uint *)(this + 0x28) = uVar3;
    if (uVar3 == 0) {
      pvVar4 = (void *)0x0;
    }
    else {
      pvVar4 = operator_new__(uVar3 | (int)uVar3 >> 0x1f);
    }
    *(void **)(this + 0x24) = pvVar4;
    AEMath::Vector::operator=(this_00,(Vector *)(param_1 + 0x68));
  }
  iVar1 = *piVar5 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  0008a3d8: push {r4,r5,r6,r7,lr}
  0008a3da: add r7,sp,#0xc
  0008a3dc: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0008a3e0: mov r4,r0
  0008a3e2: ldr r0,[0x0008a4e8]
  0008a3e4: add.w r8,r4,#0x2c
  0008a3e8: mov r6,r1
  0008a3ea: add r0,pc
  0008a3ec: ldr.w r9,[r0,#0x0]
  0008a3f0: ldr.w r0,[r9,#0x0]
  0008a3f4: str r0,[sp,#0x18]
  0008a3f6: mov r0,r8
  0008a3f8: blx 0x0006f8e0
  0008a3fc: add.w r5,r4,#0x38
  0008a400: mov r0,r5
  0008a402: str.w r8,[sp,#0x8]
  0008a406: blx 0x0006f8e0
  0008a40a: add.w r10,r4,#0x44
  0008a40e: mov r0,r10
  0008a410: str r5,[sp,#0x4]
  0008a412: blx 0x0006f724
  0008a416: add.w r11,r4,#0x50
  0008a41a: mov r0,r11
  0008a41c: blx 0x0006f8ec
  0008a420: add.w r5,r4,#0x5c
  0008a424: mov r0,r5
  0008a426: blx 0x0006f8e0
  0008a42a: movs r0,#0x0
  0008a42c: mov r8,r4
  0008a42e: str.w r0,[r8,#0x68]!
  0008a432: cmp r6,#0x0
  0008a434: strd r0,r0,[r8,#0x4]
  0008a438: beq 0x0008a460
  0008a43a: movs r0,#0x0
  0008a43c: b 0x0008a448
  0008a43e: ldr.w r1,[r6,r0,lsl #0x2]
  0008a442: str.w r1,[r4,r0,lsl #0x2]
  0008a446: adds r0,#0x1
  0008a448: cmp r0,#0x8
  0008a44a: bne 0x0008a43e
  0008a44c: ldr r0,[r6,#0x20]
  0008a44e: str r0,[r4,#0x20]
  0008a450: ldr r0,[r6,#0x28]
  0008a452: str r0,[r4,#0x28]
  0008a454: cbz r0,0x0008a486
  0008a456: orr.w r0,r0,r0, asr #0x1f
  0008a45a: blx 0x0006ec08
  0008a45e: b 0x0008a488
  0008a460: ldr r1,[0x0008a4e4]
  0008a462: str r0,[r4,#0x20]
  0008a464: strd r1,r0,[sp,#0xc]
  0008a468: str r0,[sp,#0x14]
  0008a46a: add r1,sp,#0xc
  0008a46c: mov r0,r8
  0008a46e: blx 0x0006eb3c
  0008a472: movs r2,#0x0
  0008a474: mov.w r0,#0xffffffff
  0008a478: b 0x0008a480
  0008a47a: str.w r0,[r4,r2,lsl #0x2]
  0008a47e: adds r2,#0x1
  0008a480: cmp r2,#0x8
  0008a482: bne 0x0008a47a
  0008a484: b 0x0008a494
  0008a486: movs r0,#0x0
  0008a488: str r0,[r4,#0x24]
  0008a48a: add.w r1,r6,#0x68
  0008a48e: mov r0,r8
  0008a490: blx 0x0006eb3c
  0008a494: ldr r0,[sp,#0x18]
  0008a496: ldr.w r1,[r9,#0x0]
  0008a49a: subs r0,r1,r0
  0008a49c: itttt eq
  0008a49e: mov.eq r0,r4
  0008a4a0: add.eq sp,#0x1c
  0008a4a2: pop.eq.w {r8,r9,r10,r11}
  0008a4a6: pop.eq {r4,r5,r6,r7,pc}
  0008a4a8: blx 0x0006e824
```
