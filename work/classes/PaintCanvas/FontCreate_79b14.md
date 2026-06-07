# PaintCanvas::FontCreate
elf 0x79b14 body 220
Sig: undefined __stdcall FontCreate(ushort param_1, uint * param_2, bool param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::FontCreate(unsigned short, unsigned int&, bool) */

void AbyssEngine::PaintCanvas::FontCreate(ushort param_1,uint *param_2,bool param_3)

{
  PaintCanvas *this;
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  ImageFont *pIVar5;
  int *piVar6;
  ushort *puVar7;
  int *piVar8;
  undefined8 uVar9;
  
  piVar6 = (int *)(uint)param_3;
  this = (PaintCanvas *)(uint)param_1;
  piVar8 = *(int **)(DAT_00089bf0 + 0x89b26);
  iVar1 = *piVar8;
  iVar2 = FindResource(this,(ushort)param_2);
  if (iVar2 != 0) {
    puVar7 = *(ushort **)(iVar2 + 0xc);
    iVar3 = FindResource(this,*puVar7);
    if (iVar3 != 0) {
      if (*(int *)(iVar3 + 8) == -1) {
        TextureCreate(param_1,(uint *)(uint)*puVar7,(bool)((char)&stack0xffffffd0 + '\b'));
      }
      if (*(int *)(iVar2 + 8) == -1) {
        iVar4 = ImageCreateFontFromFile
                          (*(Engine **)(this + 0x34),(char *)**(undefined4 **)(iVar3 + 0xc),
                           puVar7[1],(ImageFont **)&stack0xffffffd4);
        if (iVar4 == 1) {
          if (*(int *)(iVar3 + 8) != -1) {
            iRam00000008 = *(int *)(iVar3 + 8);
          }
          ArrayAdd<AbyssEngine::ImageFont*>((ImageFont *)0x0,(Array *)(this + 0x140));
          iVar3 = *(int *)(this + 0x140) + -1;
          *(int *)(iVar2 + 8) = iVar3;
          *piVar6 = iVar3;
          iVar2 = *(int *)(*(int *)(this + 0x34) + 0x78);
          if (iVar2 == -1) {
            *(int *)(*(int *)(this + 0x34) + 0x78) = iVar3;
          }
          else {
            pIVar5 = *(ImageFont **)(*(int *)(this + 0x144) + iVar2 * 4);
            if (*(ushort *)pIVar5 <= uRam00000000) {
              iVar2 = ImageFontGetHeight(pIVar5);
              uVar9 = ImageFontGetHeight((ImageFont *)0x0);
              iVar3 = (int)uVar9;
              if (iVar3 < iVar2) {
                uVar9 = CONCAT44(*piVar6,*(undefined4 *)(this + 0x34));
              }
              if (iVar3 < iVar2) {
                *(int *)((int)uVar9 + 0x78) = (int)((ulonglong)uVar9 >> 0x20);
              }
            }
          }
        }
      }
      else {
        *piVar6 = *(int *)(iVar2 + 8);
      }
    }
  }
  iVar2 = *piVar8;
  iVar1 = iVar2 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1,iVar2);
}

```

## target disasm
```
  00089b14: push {r4,r5,r6,r7,lr}
  00089b16: add r7,sp,#0xc
  00089b18: push {r4,r5,r6,r7,r8,r9,r10}
  00089b1c: mov r10,r0
  00089b1e: ldr r0,[0x00089bf0]
  00089b20: mov r8,r2
  00089b22: add r0,pc
  00089b24: ldr.w r9,[r0,#0x0]
  00089b28: ldr.w r0,[r9,#0x0]
  00089b2c: str r0,[sp,#0xc]
  00089b2e: mov r0,r10
  00089b30: blx 0x0006fb98
  00089b34: mov r6,r0
  00089b36: cmp r0,#0x0
  00089b38: beq 0x00089bdc
  00089b3a: ldr r5,[r6,#0xc]
  00089b3c: mov r0,r10
  00089b3e: ldrh r1,[r5,#0x0]
  00089b40: blx 0x0006fb98
  00089b44: mov r4,r0
  00089b46: cmp r0,#0x0
  00089b48: beq 0x00089bdc
  00089b4a: ldr r0,[r4,#0x8]
  00089b4c: adds r0,#0x1
  00089b4e: bne 0x00089b60
  00089b50: movs r0,#0x0
  00089b52: add r2,sp,#0x8
  00089b54: str r0,[sp,#0x8]
  00089b56: mov r0,r10
  00089b58: ldrh r1,[r5,#0x0]
  00089b5a: movs r3,#0x0
  00089b5c: blx 0x0006fbb0
  00089b60: ldr r0,[r6,#0x8]
  00089b62: adds r1,r0,#0x1
  00089b64: beq 0x00089b6c
  00089b66: str.w r0,[r8,#0x0]
  00089b6a: b 0x00089bdc
  00089b6c: movs r0,#0x0
  00089b6e: add r3,sp,#0x4
  00089b70: str r0,[sp,#0x4]
  00089b72: ldr r1,[r4,#0xc]
  00089b74: ldrh r2,[r5,#0x2]
  00089b76: ldr.w r0,[r10,#0x34]
  00089b7a: ldr r1,[r1,#0x0]
  00089b7c: blx 0x0006fbbc
  00089b80: cmp r0,#0x1
  00089b82: bne 0x00089bdc
  00089b84: ldr r0,[sp,#0x4]
  00089b86: ldr r1,[r4,#0x8]
  00089b88: adds r2,r1,#0x1
  00089b8a: it ne
  00089b8c: str.ne r1,[r0,#0x8]
  00089b8e: add.w r1,r10,#0x140
  00089b92: blx 0x0006fbc8
  00089b96: ldr.w r0,[r10,#0x140]
  00089b9a: subs r0,#0x1
  00089b9c: str r0,[r6,#0x8]
  00089b9e: str.w r0,[r8,#0x0]
  00089ba2: ldr.w r1,[r10,#0x34]
  00089ba6: ldr r2,[r1,#0x78]
  00089ba8: adds r3,r2,#0x1
  00089baa: beq 0x00089bda
  00089bac: ldr.w r0,[r10,#0x144]
  00089bb0: ldr r1,[sp,#0x4]
  00089bb2: ldr.w r0,[r0,r2,lsl #0x2]
  00089bb6: ldrh r1,[r1,#0x0]
  00089bb8: ldrh r2,[r0,#0x0]
  00089bba: cmp r2,r1
  00089bbc: bhi 0x00089bdc
  00089bbe: blx 0x0006f538
  00089bc2: mov r4,r0
  00089bc4: ldr r0,[sp,#0x4]
  00089bc6: blx 0x0006f538
  00089bca: cmp r4,r0
  00089bcc: ittt gt
  00089bce: ldr.gt.w r0,[r10,#0x34]
  00089bd2: ldr.gt.w r1,[r8,#0x0]
  00089bd6: str.gt r1,[r0,#0x78]
  00089bd8: b 0x00089bdc
  00089bda: str r0,[r1,#0x78]
  00089bdc: ldr r0,[sp,#0xc]
  00089bde: ldr.w r1,[r9,#0x0]
  00089be2: subs r0,r1,r0
  00089be4: itt eq
  00089be6: pop.eq.w {r0,r1,r2,r3,r8,r9,r10}
  00089bea: pop.eq {r4,r5,r6,r7,pc}
  00089bec: blx 0x0006e824
```
