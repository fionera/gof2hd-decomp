# PaintCanvas::TransformGetLocal
elf 0x7b3cc body 64
Sig: undefined __stdcall TransformGetLocal(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformGetLocal(unsigned int) */

void AbyssEngine::PaintCanvas::TransformGetLocal(uint param_1)

{
  int iVar1;
  uint in_r1;
  int *piVar2;
  AEMath aAStack_50 [60];
  int local_14;
  
  piVar2 = *(int **)(DAT_0008b40c + 0x8b3d8);
  local_14 = *piVar2;
  if (*(uint *)(param_1 + 0x158) <= in_r1) {
    AEMath::MatrixIdentity(aAStack_50,(Matrix *)(param_1 + 0xf8));
  }
  iVar1 = *piVar2 - local_14;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  0008b3cc: push {r4,r5,r7,lr}
  0008b3ce: add r7,sp,#0x8
  0008b3d0: sub sp,#0x40
  0008b3d2: ldr r2,[0x0008b40c]
  0008b3d4: add r2,pc
  0008b3d6: ldr r5,[r2,#0x0]
  0008b3d8: ldr r2,[r5,#0x0]
  0008b3da: str r2,[sp,#0x3c]
  0008b3dc: ldr.w r2,[r0,#0x158]
  0008b3e0: cmp r2,r1
  0008b3e2: bls 0x0008b3ee
  0008b3e4: ldr.w r0,[r0,#0x15c]
  0008b3e8: ldr.w r4,[r0,r1,lsl #0x2]
  0008b3ec: b 0x0008b3fa
  0008b3ee: add.w r4,r0,#0xf8
  0008b3f2: mov r0,sp
  0008b3f4: mov r1,r4
  0008b3f6: blx 0x0006fb2c
  0008b3fa: ldr r0,[sp,#0x3c]
  0008b3fc: ldr r1,[r5,#0x0]
  0008b3fe: subs r0,r1,r0
  0008b400: ittt eq
  0008b402: mov.eq r0,r4
  0008b404: add.eq sp,#0x40
  0008b406: pop.eq {r4,r5,r7,pc}
  0008b408: blx 0x0006e824
```
