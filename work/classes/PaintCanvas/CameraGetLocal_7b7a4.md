# PaintCanvas::CameraGetLocal
elf 0x7b7a4 body 68
Sig: undefined __stdcall CameraGetLocal(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraGetLocal(unsigned int) */

void AbyssEngine::PaintCanvas::CameraGetLocal(uint param_1)

{
  int iVar1;
  uint in_r1;
  int *piVar2;
  AEMath aAStack_50 [60];
  int local_14;
  
  piVar2 = *(int **)(DAT_0008b7e8 + 0x8b7b0);
  local_14 = *piVar2;
  if (*(uint *)(param_1 + 0x164) <= in_r1) {
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
  0008b7a4: push {r4,r5,r7,lr}
  0008b7a6: add r7,sp,#0x8
  0008b7a8: sub sp,#0x40
  0008b7aa: ldr r2,[0x0008b7e8]
  0008b7ac: add r2,pc
  0008b7ae: ldr r5,[r2,#0x0]
  0008b7b0: ldr r2,[r5,#0x0]
  0008b7b2: str r2,[sp,#0x3c]
  0008b7b4: ldr.w r2,[r0,#0x164]
  0008b7b8: cmp r2,r1
  0008b7ba: bls 0x0008b7ca
  0008b7bc: ldr.w r0,[r0,#0x168]
  0008b7c0: ldr.w r0,[r0,r1,lsl #0x2]
  0008b7c4: add.w r4,r0,#0xc
  0008b7c8: b 0x0008b7d6
  0008b7ca: add.w r4,r0,#0xf8
  0008b7ce: mov r0,sp
  0008b7d0: mov r1,r4
  0008b7d2: blx 0x0006fb2c
  0008b7d6: ldr r0,[sp,#0x3c]
  0008b7d8: ldr r1,[r5,#0x0]
  0008b7da: subs r0,r1,r0
  0008b7dc: ittt eq
  0008b7de: mov.eq r0,r4
  0008b7e0: add.eq sp,#0x40
  0008b7e2: pop.eq {r4,r5,r7,pc}
  0008b7e4: blx 0x0006e824
```
