# PaintCanvas::Resume
elf 0x7e828 body 178
Sig: undefined __thiscall Resume(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::Resume() */

void __thiscall AbyssEngine::PaintCanvas::Resume(PaintCanvas *this)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int *piVar6;
  
  piVar6 = *(int **)(DAT_0008e8dc + 0x8e840);
  iVar1 = *piVar6;
  for (uVar5 = 0; uVar5 < *(uint *)(this + 0x10); uVar5 = uVar5 + 1) {
    pcVar2 = (char *)String::GetAEChar();
    iVar3 = TextureCreateFromFile
                      (*(Engine **)(this + 0x34),pcVar2,(_func_void_Image_ptr_void_ptr *)0x0,
                       (void *)0x0,(uint *)&stack0xffffffd8,false,
                       *(float *)(*(int *)(*(int *)(this + 0x14) + uVar5 * 4) + 0x10));
    if (iVar3 == 1) {
      **(undefined4 **)(*(int *)(this + 0x14) + uVar5 * 4) = 0;
    }
    operator_delete__(pcVar2);
  }
  piVar4 = *(int **)(DAT_0008e8e0 + 0x8e89e);
  if (*piVar4 != 0) {
    glActiveTexture(0x84c7);
    glBindTexture(0x8513,**(undefined4 **)(*(int *)(this + 0x14) + *piVar4 * 4));
    glActiveTexture(0x84c0);
  }
  if (*piVar6 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008e828: push {r4,r5,r6,r7,lr}
  0008e82a: add r7,sp,#0xc
  0008e82c: push {r2,r3,r4,r5,r6,r7,r8,r9,r10}
  0008e830: mov r4,r0
  0008e832: ldr r0,[0x0008e8dc]
  0008e834: add.w r9,sp,#0x10
  0008e838: mov.w r10,#0x0
  0008e83c: add r0,pc
  0008e83e: movs r6,#0x0
  0008e840: ldr.w r8,[r0,#0x0]
  0008e844: ldr.w r0,[r8,#0x0]
  0008e848: str r0,[sp,#0x14]
  0008e84a: b 0x0008e892
  0008e84c: str.w r10,[sp,#0x10]
  0008e850: ldr r0,[r4,#0x14]
  0008e852: ldr.w r0,[r0,r6,lsl #0x2]
  0008e856: adds r0,#0x4
  0008e858: blx 0x0006ef68
  0008e85c: ldr r1,[r4,#0x14]
  0008e85e: mov r5,r0
  0008e860: ldr r0,[r4,#0x34]
  0008e862: movs r2,#0x0
  0008e864: movs r3,#0x0
  0008e866: ldr.w r1,[r1,r6,lsl #0x2]
  0008e86a: vldr.32 s0,[r1,#0x10]
  0008e86e: mov r1,r5
  0008e870: vstr.32 s0,[sp,#0x8]
  0008e874: strd r9,r10,[sp,#0x0]
  0008e878: blx 0x0006fb80
  0008e87c: cmp r0,#0x1
  0008e87e: bne 0x0008e88a
  0008e880: ldr r0,[r4,#0x14]
  0008e882: ldr r1,[sp,#0x10]
  0008e884: ldr.w r0,[r0,r6,lsl #0x2]
  0008e888: str r1,[r0,#0x0]
  0008e88a: mov r0,r5
  0008e88c: blx 0x0006ebfc
  0008e890: adds r6,#0x1
  0008e892: ldr r0,[r4,#0x10]
  0008e894: cmp r6,r0
  0008e896: bcc 0x0008e84c
  0008e898: ldr r0,[0x0008e8e0]
  0008e89a: add r0,pc
  0008e89c: ldr r5,[r0,#0x0]
  0008e89e: ldr r0,[r5,#0x0]
  0008e8a0: cbz r0,0x0008e8c4
  0008e8a2: movw r0,#0x84c7
  0008e8a6: blx 0x0006fb8c
  0008e8aa: ldr r0,[r4,#0x14]
  0008e8ac: ldr r1,[r5,#0x0]
  0008e8ae: ldr.w r0,[r0,r1,lsl #0x2]
  0008e8b2: ldr r1,[r0,#0x0]
  0008e8b4: movw r0,#0x8513
  0008e8b8: blx 0x0006f244
  0008e8bc: movw r0,#0x84c0
  0008e8c0: blx 0x0006fb8c
  0008e8c4: ldr r0,[sp,#0x14]
  0008e8c6: ldr.w r1,[r8,#0x0]
  0008e8ca: subs r0,r1,r0
  0008e8cc: ittt eq
  0008e8ce: add.eq sp,#0x18
  0008e8d0: pop.eq.w {r8,r9,r10}
  0008e8d4: pop.eq {r4,r5,r6,r7,pc}
  0008e8d6: blx 0x0006e824
```
