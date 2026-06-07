# PaintCanvas::ReloadTextures
elf 0x7e8e4 body 140
Sig: undefined __thiscall ReloadTextures(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::ReloadTextures() */

void __thiscall AbyssEngine::PaintCanvas::ReloadTextures(PaintCanvas *this)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  
  piVar5 = *(int **)(DAT_0008e970 + 0x8e8fc);
  iVar1 = *piVar5;
  for (uVar4 = 0; uVar4 < *(uint *)(this + 0x10); uVar4 = uVar4 + 1) {
    if (**(int **)(*(int *)(this + 0x14) + uVar4 * 4) == -1) {
      pcVar2 = (char *)String::GetAEChar();
      iVar3 = TextureCreateFromFile
                        (*(Engine **)(this + 0x34),pcVar2,(_func_void_Image_ptr_void_ptr *)0x0,
                         (void *)0x0,(uint *)&stack0xffffffd8,false,
                         *(float *)(*(int *)(*(int *)(this + 0x14) + uVar4 * 4) + 0x10));
      if (iVar3 == 1) {
        **(undefined4 **)(*(int *)(this + 0x14) + uVar4 * 4) = 0;
      }
      operator_delete__(pcVar2);
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
  0008e8e4: push {r4,r5,r6,r7,lr}
  0008e8e6: add r7,sp,#0xc
  0008e8e8: push {r2,r3,r4,r5,r6,r7,r8,r9,r10}
  0008e8ec: mov r4,r0
  0008e8ee: ldr r0,[0x0008e970]
  0008e8f0: add.w r9,sp,#0x10
  0008e8f4: mov.w r10,#0x0
  0008e8f8: add r0,pc
  0008e8fa: movs r6,#0x0
  0008e8fc: ldr.w r8,[r0,#0x0]
  0008e900: ldr.w r0,[r8,#0x0]
  0008e904: str r0,[sp,#0x14]
  0008e906: b 0x0008e954
  0008e908: ldr r0,[r4,#0x14]
  0008e90a: ldr.w r0,[r0,r6,lsl #0x2]
  0008e90e: ldr r1,[r0,#0x0]
  0008e910: adds r1,#0x1
  0008e912: bne 0x0008e952
  0008e914: adds r0,#0x4
  0008e916: str.w r10,[sp,#0x10]
  0008e91a: blx 0x0006ef68
  0008e91e: ldr r1,[r4,#0x14]
  0008e920: mov r5,r0
  0008e922: ldr r0,[r4,#0x34]
  0008e924: movs r2,#0x0
  0008e926: movs r3,#0x0
  0008e928: ldr.w r1,[r1,r6,lsl #0x2]
  0008e92c: vldr.32 s0,[r1,#0x10]
  0008e930: mov r1,r5
  0008e932: vstr.32 s0,[sp,#0x8]
  0008e936: strd r9,r10,[sp,#0x0]
  0008e93a: blx 0x0006fb80
  0008e93e: cmp r0,#0x1
  0008e940: bne 0x0008e94c
  0008e942: ldr r0,[r4,#0x14]
  0008e944: ldr r1,[sp,#0x10]
  0008e946: ldr.w r0,[r0,r6,lsl #0x2]
  0008e94a: str r1,[r0,#0x0]
  0008e94c: mov r0,r5
  0008e94e: blx 0x0006ebfc
  0008e952: adds r6,#0x1
  0008e954: ldr r0,[r4,#0x10]
  0008e956: cmp r6,r0
  0008e958: bcc 0x0008e908
  0008e95a: ldr r0,[sp,#0x14]
  0008e95c: ldr.w r1,[r8,#0x0]
  0008e960: subs r0,r1,r0
  0008e962: ittt eq
  0008e964: add.eq sp,#0x18
  0008e966: pop.eq.w {r8,r9,r10}
  0008e96a: pop.eq {r4,r5,r6,r7,pc}
  0008e96c: blx 0x0006e824
```
