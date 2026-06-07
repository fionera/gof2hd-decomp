# BoundingVolume::staticProjectCollisionOnSurface
elf 0x11f8b0 body 144
Sig: undefined __thiscall staticProjectCollisionOnSurface(BoundingVolume * this, Vector * param_1, Array * param_2)

## decompile
```c

/* BoundingVolume::staticProjectCollisionOnSurface(AbyssEngine::AEMath::Vector const&,
   Array<BoundingVolume*>*) */

void __thiscall
BoundingVolume::staticProjectCollisionOnSurface(BoundingVolume *this,Vector *param_1,Array *param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  int iVar6;
  undefined8 uVar7;
  
  piVar5 = *(int **)(DAT_0012f940 + 0x12f8c4);
  iVar1 = *piVar5;
  uVar7 = *(undefined8 *)param_1;
  *(undefined4 *)(this + 8) = *(undefined4 *)(param_1 + 8);
  *(undefined8 *)this = uVar7;
  if (param_2 != (Array *)0x0) {
    for (iVar6 = 0; iVar6 != 2; iVar6 = iVar6 + 1) {
      for (uVar4 = 0; uVar4 < *(uint *)param_2; uVar4 = uVar4 + 1) {
        piVar2 = *(int **)(*(int *)(param_2 + 4) + uVar4 * 4);
        iVar3 = (**(code **)(*piVar2 + 0xc))
                          (piVar2,*(undefined4 *)this,*(undefined4 *)(this + 4),
                           *(undefined4 *)(this + 8));
        if (iVar3 != 0) {
          piVar2 = *(int **)(*(int *)(param_2 + 4) + uVar4 * 4);
          (**(code **)(*piVar2 + 0x10))((Vector *)&stack0xffffffcc,piVar2,this);
          AbyssEngine::AEMath::Vector::operator=((Vector *)this,(Vector *)&stack0xffffffcc);
        }
      }
    }
  }
  if (*piVar5 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012f8b0: push {r4,r5,r6,r7,lr}
  0012f8b2: add r7,sp,#0xc
  0012f8b4: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0012f8b8: mov r5,r0
  0012f8ba: ldr r0,[0x0012f940]
  0012f8bc: mov r11,r2
  0012f8be: cmp r2,#0x0
  0012f8c0: add r0,pc
  0012f8c2: ldr.w r8,[r0,#0x0]
  0012f8c6: ldr.w r0,[r8,#0x0]
  0012f8ca: str r0,[sp,#0x10]
  0012f8cc: vldr.64 d16,[r1]
  0012f8d0: ldr r0,[r1,#0x8]
  0012f8d2: str r0,[r5,#0x8]
  0012f8d4: vstr.64 d16,[r5]
  0012f8d8: beq 0x0012f92a
  0012f8da: add.w r10,sp,#0x4
  0012f8de: mov.w r9,#0x0
  0012f8e2: b 0x0012f924
  0012f8e4: movs r6,#0x0
  0012f8e6: b 0x0012f918
  0012f8e8: ldr.w r0,[r11,#0x4]
  0012f8ec: ldr.w r0,[r0,r6,lsl #0x2]
  0012f8f0: ldr r4,[r0,#0x0]
  0012f8f2: ldm.w r5,{r1,r2,r3}
  0012f8f6: ldr r4,[r4,#0xc]
  0012f8f8: blx r4
  0012f8fa: cbz r0,0x0012f916
  0012f8fc: ldr.w r0,[r11,#0x4]
  0012f900: mov r2,r5
  0012f902: ldr.w r1,[r0,r6,lsl #0x2]
  0012f906: ldr r0,[r1,#0x0]
  0012f908: ldr r3,[r0,#0x10]
  0012f90a: mov r0,r10
  0012f90c: blx r3
  0012f90e: mov r0,r5
  0012f910: mov r1,r10
  0012f912: blx 0x0006eb3c
  0012f916: adds r6,#0x1
  0012f918: ldr.w r0,[r11,#0x0]
  0012f91c: cmp r6,r0
  0012f91e: bcc 0x0012f8e8
  0012f920: add.w r9,r9,#0x1
  0012f924: cmp.w r9,#0x2
  0012f928: bne 0x0012f8e4
  0012f92a: ldr r0,[sp,#0x10]
  0012f92c: ldr.w r1,[r8,#0x0]
  0012f930: subs r0,r1,r0
  0012f932: ittt eq
  0012f934: add.eq sp,#0x14
  0012f936: pop.eq.w {r8,r9,r10,r11}
  0012f93a: pop.eq {r4,r5,r6,r7,pc}
  0012f93c: blx 0x0006e824
```
