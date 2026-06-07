# PaintCanvas::Image2DCreate
elf 0x79c78 body 186
Sig: undefined __thiscall Image2DCreate(PaintCanvas * this, ushort param_1, uint * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::Image2DCreate(unsigned short, unsigned int&) */

void __thiscall
AbyssEngine::PaintCanvas::Image2DCreate(PaintCanvas *this,ushort param_1,uint *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  Image2D *pIVar5;
  int iVar6;
  int *piVar7;
  ushort *puVar8;
  
  piVar7 = *(int **)(DAT_00089d34 + 0x89c8a);
  iVar1 = *piVar7;
  iVar2 = FindResource(this,param_1);
  if (iVar2 != 0) {
    puVar8 = *(ushort **)(iVar2 + 0xc);
    iVar3 = FindResource(this,*puVar8);
    if (iVar3 != 0) {
      if (*(int *)(iVar3 + 8) == -1) {
        TextureCreate((ushort)this,(uint *)(uint)*puVar8,true);
      }
      uVar4 = *(uint *)(iVar2 + 8);
      if (uVar4 == 0xffffffff) {
        pIVar5 = operator_new(0x18);
        *(undefined4 *)(pIVar5 + 0xd) = 0;
        *(undefined4 *)pIVar5 = 0;
        *(undefined4 *)(pIVar5 + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4)
        ;
        *(undefined4 *)(pIVar5 + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8)
        ;
        *(undefined4 *)(pIVar5 + 0xc) =
             *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        *(undefined4 *)(pIVar5 + 0x11) = 0;
        iVar6 = ImageCreateRegionFromFile
                          (*(Engine **)(this + 0x34),(char *)**(undefined4 **)(iVar3 + 0xc),
                           puVar8[1],pIVar5);
        if (iVar6 != 1) goto LAB_00089d20;
        if (*(int *)(iVar3 + 8) != -1) {
          *(int *)(pIVar5 + 4) = *(int *)(iVar3 + 8);
        }
        ArrayAdd<AbyssEngine::Image2D*>(pIVar5,(Array *)(this + 0x14c));
        uVar4 = *(int *)(this + 0x14c) - 1;
        *(uint *)(iVar2 + 8) = uVar4;
      }
      *param_2 = uVar4;
    }
  }
LAB_00089d20:
  iVar2 = *piVar7;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  00089c78: push {r4,r5,r6,r7,lr}
  00089c7a: add r7,sp,#0xc
  00089c7c: push {r5,r6,r7,r8,r9,r10,r11}
  00089c80: mov r5,r0
  00089c82: ldr r0,[0x00089d34]
  00089c84: mov r8,r2
  00089c86: add r0,pc
  00089c88: ldr r4,[r0,#0x0]
  00089c8a: ldr r0,[r4,#0x0]
  00089c8c: str r0,[sp,#0x8]
  00089c8e: mov r0,r5
  00089c90: blx 0x0006fb98
  00089c94: mov r6,r0
  00089c96: cmp r0,#0x0
  00089c98: beq 0x00089d20
  00089c9a: ldr.w r11,[r6,#0xc]
  00089c9e: mov r0,r5
  00089ca0: ldrh.w r1,[r11,#0x0]
  00089ca4: blx 0x0006fb98
  00089ca8: mov r10,r0
  00089caa: cbz r0,0x00089d20
  00089cac: ldr.w r0,[r10,#0x8]
  00089cb0: adds r0,#0x1
  00089cb2: bne 0x00089cc6
  00089cb4: movs r0,#0x0
  00089cb6: add r2,sp,#0x4
  00089cb8: str r0,[sp,#0x4]
  00089cba: mov r0,r5
  00089cbc: ldrh.w r1,[r11,#0x0]
  00089cc0: movs r3,#0x0
  00089cc2: blx 0x0006fbb0
  00089cc6: ldr r0,[r6,#0x8]
  00089cc8: adds r1,r0,#0x1
  00089cca: bne 0x00089d1c
  00089ccc: movs r0,#0x18
  00089cce: blx 0x0006eb24
  00089cd2: mov r9,r0
  00089cd4: movs r0,#0x0
  00089cd6: vmov.i32 q8,#0x0
  00089cda: movs r1,#0x11
  00089cdc: mov r2,r9
  00089cde: str.w r0,[r9,#0xd]
  00089ce2: vst1.32 {d16,d17},[r2],r1
  00089ce6: mov r3,r9
  00089ce8: str r0,[r2,#0x0]
  00089cea: ldr.w r1,[r10,#0xc]
  00089cee: ldr r0,[r5,#0x34]
  00089cf0: ldrh.w r2,[r11,#0x2]
  00089cf4: ldr r1,[r1,#0x0]
  00089cf6: blx 0x0006fc04
  00089cfa: cmp r0,#0x1
  00089cfc: bne 0x00089d20
  00089cfe: ldr.w r0,[r10,#0x8]
  00089d02: adds r1,r0,#0x1
  00089d04: it ne
  00089d06: str.w.ne r0,[r9,#0x4]
  00089d0a: add.w r1,r5,#0x14c
  00089d0e: mov r0,r9
  00089d10: blx 0x0006fc10
  00089d14: ldr.w r0,[r5,#0x14c]
  00089d18: subs r0,#0x1
  00089d1a: str r0,[r6,#0x8]
  00089d1c: str.w r0,[r8,#0x0]
  00089d20: ldr r0,[sp,#0x8]
  00089d22: ldr r1,[r4,#0x0]
  00089d24: subs r0,r1,r0
  00089d26: itt eq
  00089d28: pop.eq.w {r1,r2,r3,r8,r9,r10,r11}
  00089d2c: pop.eq {r4,r5,r6,r7,pc}
  00089d2e: blx 0x0006e824
```
