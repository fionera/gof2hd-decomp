# PaintCanvas::TextureCreate
elf 0x79a34 body 150
Sig: undefined __thiscall TextureCreate(PaintCanvas * this, ushort param_1, _func_void_Image_ptr_void_ptr * param_2, void * param_3, uint * param_4, bool param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::TextureCreate(unsigned short, void (*)(AbyssEngine::Image*, void*),
   void*, unsigned int&, bool) */

void __thiscall
AbyssEngine::PaintCanvas::TextureCreate
          (PaintCanvas *this,ushort param_1,_func_void_Image_ptr_void_ptr *param_2,void *param_3,
          uint *param_4,bool param_5)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  int *piVar6;
  float fVar7;
  undefined3 in_stack_00000005;
  
  piVar6 = *(int **)(DAT_00089acc + 0x89a4a);
  iVar1 = *piVar6;
  iVar2 = *(int *)(this + 0x34);
  *(undefined4 *)(iVar2 + 0x7c) = 0xffffffff;
  *(undefined4 *)(iVar2 + 0x80) = 0xffffffff;
  iVar2 = FindResource(this,param_1);
  if (iVar2 != 0) {
    uVar3 = *(uint *)(iVar2 + 8);
    if (uVar3 == 0xffffffff) {
      fVar7 = (float)(*(undefined4 **)(iVar2 + 0xc))[1];
      pcVar5 = (char *)**(undefined4 **)(iVar2 + 0xc);
      if (_param_5 == 0) {
        iVar4 = TextureCreateFromFileIntern
                          (*(Engine **)(this + 0x34),pcVar5,param_2,param_3,(uint *)&stack0xffffffd8
                           ,fVar7,(AELoadedTexture *)fVar7,false);
      }
      else {
        iVar4 = TextureCreateFromFile
                          (*(Engine **)(this + 0x34),pcVar5,param_2,param_3,(uint *)&stack0xffffffd8
                           ,true,fVar7);
      }
      if (iVar4 != 1) goto LAB_00089ab6;
      uVar3 = 0;
      *(undefined4 *)(iVar2 + 8) = 0;
    }
    *param_4 = uVar3;
  }
LAB_00089ab6:
  if (*piVar6 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00089a34: push {r4,r5,r6,r7,lr}
  00089a36: add r7,sp,#0xc
  00089a38: push {r2,r3,r4,r5,r6,r7,r8,r9,r10}
  00089a3c: mov r5,r0
  00089a3e: ldr r0,[0x00089acc]
  00089a40: mov r9,r2
  00089a42: mov.w r2,#0xffffffff
  00089a46: add r0,pc
  00089a48: mov r8,r3
  00089a4a: ldr r6,[r0,#0x0]
  00089a4c: ldr r0,[r6,#0x0]
  00089a4e: str r0,[sp,#0x14]
  00089a50: ldr r0,[r5,#0x34]
  00089a52: strd r2,r2,[r0,#0x7c]
  00089a56: mov r0,r5
  00089a58: blx 0x0006fb98
  00089a5c: mov r4,r0
  00089a5e: cbz r0,0x00089ab6
  00089a60: ldr r0,[r4,#0x8]
  00089a62: ldr.w r10,[r7,#0x8]
  00089a66: adds r1,r0,#0x1
  00089a68: bne 0x00089ab2
  00089a6a: ldr r0,[r4,#0xc]
  00089a6c: movs r2,#0x0
  00089a6e: str r2,[sp,#0x10]
  00089a70: ldr r3,[r7,#0xc]
  00089a72: vldr.32 s0,[r0,#0x4]
  00089a76: ldr r1,[r0,#0x0]
  00089a78: cmp r3,#0x0
  00089a7a: ldr r0,[r5,#0x34]
  00089a7c: beq 0x00089a94
  00089a7e: movs r2,#0x1
  00089a80: add r3,sp,#0x10
  00089a82: vstr.32 s0,[sp,#0x8]
  00089a86: strd r3,r2,[sp,#0x0]
  00089a8a: mov r2,r9
  00089a8c: mov r3,r8
  00089a8e: blx 0x0006fb80
  00089a92: b 0x00089aaa
  00089a94: movs r3,#0x1
  00089a96: strd r2,r3,[sp,#0x8]
  00089a9a: add r2,sp,#0x10
  00089a9c: str r2,[sp,#0x0]
  00089a9e: mov r2,r9
  00089aa0: mov r3,r8
  00089aa2: vstr.32 s0,[sp,#0x4]
  00089aa6: blx 0x0006f220
  00089aaa: cmp r0,#0x1
  00089aac: bne 0x00089ab6
  00089aae: ldr r0,[sp,#0x10]
  00089ab0: str r0,[r4,#0x8]
  00089ab2: str.w r0,[r10,#0x0]
  00089ab6: ldr r0,[sp,#0x14]
  00089ab8: ldr r1,[r6,#0x0]
  00089aba: subs r0,r1,r0
  00089abc: ittt eq
  00089abe: add.eq sp,#0x18
  00089ac0: pop.eq.w {r8,r9,r10}
  00089ac4: pop.eq {r4,r5,r6,r7,pc}
  00089ac6: blx 0x0006e824
```
