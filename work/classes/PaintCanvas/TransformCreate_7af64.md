# PaintCanvas::TransformCreate
elf 0x7af64 body 250
Sig: undefined __thiscall TransformCreate(PaintCanvas * this, ushort param_1, uint * param_2)

## decompile
```c

/* WARNING: Removing unreachable block (ram,0x0008aff2) */
/* WARNING: Removing unreachable block (ram,0x0008b030) */
/* AbyssEngine::PaintCanvas::TransformCreate(unsigned short, unsigned int&) */

void __thiscall
AbyssEngine::PaintCanvas::TransformCreate(PaintCanvas *this,ushort param_1,uint *param_2)

{
  int iVar1;
  int iVar2;
  Transform *this_00;
  int iVar3;
  Matrix *pMVar4;
  uint uVar5;
  int *piVar6;
  
  piVar6 = *(int **)(DAT_0008b06c + 0x8af76);
  iVar1 = *piVar6;
  iVar2 = FindResource(this,param_1);
  if (iVar2 != 0) {
    if (*(uint *)(iVar2 + 8) == 0xffffffff) {
      pMVar4 = *(Matrix **)(iVar2 + 0xc);
      this_00 = operator_new(0x180);
      Transform::Transform(this_00);
      ArrayAdd<AbyssEngine::Transform*>(this_00,(Array *)(this + 0x158));
      iVar3 = *(int *)(this + 0x158);
      *(uint *)(iVar2 + 8) = iVar3 - 1U;
      *param_2 = iVar3 - 1U;
      AEMath::Matrix::operator=((Matrix *)this_00,pMVar4);
      for (uVar5 = 0; uVar5 < *(ushort *)(pMVar4 + 0x3c); uVar5 = uVar5 + 1) {
        MeshCreate(this,*(ushort *)(*(int *)(pMVar4 + 0x40) + uVar5 * 2),(uint *)&stack0xffffffd4,
                   false);
      }
      for (uVar5 = 0; uVar5 < *(ushort *)(pMVar4 + 0x44); uVar5 = uVar5 + 1) {
        TransformCreate(this,*(ushort *)(*(int *)(pMVar4 + 0x48) + uVar5 * 2),
                        (uint *)&stack0xffffffd0);
      }
    }
    else {
      *param_2 = *(uint *)(iVar2 + 8);
    }
  }
  if (*piVar6 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0008af64: push {r4,r5,r6,r7,lr}
  0008af66: add r7,sp,#0xc
  0008af68: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0008af6c: mov r4,r0
  0008af6e: ldr r0,[0x0008b06c]
  0008af70: mov r9,r2
  0008af72: add r0,pc
  0008af74: ldr.w r10,[r0,#0x0]
  0008af78: ldr.w r0,[r10,#0x0]
  0008af7c: str r0,[sp,#0x10]
  0008af7e: mov r0,r4
  0008af80: blx 0x0006fb98
  0008af84: mov r6,r0
  0008af86: cmp r0,#0x0
  0008af88: beq 0x0008b048
  0008af8a: ldr r0,[r6,#0x8]
  0008af8c: adds r1,r0,#0x1
  0008af8e: beq 0x0008af96
  0008af90: str.w r0,[r9,#0x0]
  0008af94: b 0x0008b048
  0008af96: mov.w r0,#0x180
  0008af9a: ldr r5,[r6,#0xc]
  0008af9c: blx 0x0006eb24
  0008afa0: mov r8,r0
  0008afa2: blx 0x0006eb84
  0008afa6: add.w r1,r4,#0x158
  0008afaa: mov r0,r8
  0008afac: blx 0x0006f790
  0008afb0: ldr.w r0,[r4,#0x158]
  0008afb4: mov r1,r5
  0008afb6: subs r0,#0x1
  0008afb8: str r0,[r6,#0x8]
  0008afba: str.w r0,[r9,#0x0]
  0008afbe: mov r0,r8
  0008afc0: blx 0x0006f148
  0008afc4: add.w r11,r8,#0x3c
  0008afc8: add.w r9,sp,#0xc
  0008afcc: str.w r8,[sp,#0x4]
  0008afd0: movs r6,#0x0
  0008afd2: mov.w r8,#0xffffffff
  0008afd6: b 0x0008b000
  0008afd8: str.w r8,[sp,#0xc]
  0008afdc: mov r2,r9
  0008afde: ldr r0,[r5,#0x40]
  0008afe0: movs r3,#0x0
  0008afe2: ldrh.w r1,[r0,r6,lsl #0x1]
  0008afe6: mov r0,r4
  0008afe8: blx 0x0006fcf4
  0008afec: ldr r0,[sp,#0xc]
  0008afee: adds r1,r0,#0x1
  0008aff0: beq 0x0008affe
  0008aff2: ldr r1,[r4,#0x28]
  0008aff4: ldr.w r0,[r1,r0,lsl #0x2]
  0008aff8: mov r1,r11
  0008affa: blx 0x0006ecbc
  0008affe: adds r6,#0x1
  0008b000: ldrh r0,[r5,#0x3c]
  0008b002: cmp r6,r0
  0008b004: bcc 0x0008afd8
  0008b006: ldr r0,[sp,#0x4]
  0008b008: add.w r8,sp,#0x8
  0008b00c: movs r6,#0x0
  0008b00e: mov.w r11,#0xffffffff
  0008b012: add.w r9,r0,#0x4c
  0008b016: b 0x0008b040
  0008b018: str.w r11,[sp,#0x8]
  0008b01c: mov r2,r8
  0008b01e: ldr r0,[r5,#0x48]
  0008b020: ldrh.w r1,[r0,r6,lsl #0x1]
  0008b024: mov r0,r4
  0008b026: blx 0x0006fd00
  0008b02a: ldr r0,[sp,#0x8]
  0008b02c: adds r1,r0,#0x1
  0008b02e: beq 0x0008b03e
  0008b030: ldr.w r1,[r4,#0x15c]
  0008b034: ldr.w r0,[r1,r0,lsl #0x2]
  0008b038: mov r1,r9
  0008b03a: blx 0x0006f790
  0008b03e: adds r6,#0x1
  0008b040: ldrh.w r0,[r5,#0x44]
  0008b044: cmp r6,r0
  0008b046: bcc 0x0008b018
  0008b048: ldr r0,[sp,#0x10]
  0008b04a: ldr.w r1,[r10,#0x0]
  0008b04e: subs r0,r1,r0
  0008b050: ittt eq
  0008b052: add.eq sp,#0x14
  0008b054: pop.eq.w {r8,r9,r10,r11}
  0008b058: pop.eq {r4,r5,r6,r7,pc}
  0008b05a: blx 0x0006e824
```
