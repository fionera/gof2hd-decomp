# PaintCanvas::Suspend
elf 0x7e7c8 body 92
Sig: undefined __thiscall Suspend(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::Suspend() */

void __thiscall AbyssEngine::PaintCanvas::Suspend(PaintCanvas *this)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_0008e824 + 0x8e7de);
  iVar1 = *piVar4;
  for (uVar3 = 0; uVar3 < *(uint *)(this + 0x10); uVar3 = uVar3 + 1) {
    piVar2 = *(int **)(*(int *)(this + 0x14) + uVar3 * 4);
    if (*piVar2 != -1) {
      glDeleteTextures(1,&stack0xffffffd8);
      piVar2 = *(int **)(*(int *)(this + 0x14) + uVar3 * 4);
    }
    *piVar2 = -1;
  }
  if (*piVar4 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008e7c8: push {r4,r5,r6,r7,lr}
  0008e7ca: add r7,sp,#0xc
  0008e7cc: push {r6,r7,r8,r9,r11}
  0008e7d0: mov r4,r0
  0008e7d2: ldr r0,[0x0008e824]
  0008e7d4: movs r6,#0x0
  0008e7d6: mov.w r5,#0xffffffff
  0008e7da: add r0,pc
  0008e7dc: mov r9,sp
  0008e7de: ldr.w r8,[r0,#0x0]
  0008e7e2: ldr.w r0,[r8,#0x0]
  0008e7e6: str r0,[sp,#0x4]
  0008e7e8: b 0x0008e80a
  0008e7ea: ldr r0,[r4,#0x14]
  0008e7ec: ldr.w r0,[r0,r6,lsl #0x2]
  0008e7f0: ldr r1,[r0,#0x0]
  0008e7f2: str r1,[sp,#0x0]
  0008e7f4: adds r1,#0x1
  0008e7f6: beq 0x0008e806
  0008e7f8: movs r0,#0x1
  0008e7fa: mov r1,r9
  0008e7fc: blx 0x0006f2a4
  0008e800: ldr r0,[r4,#0x14]
  0008e802: ldr.w r0,[r0,r6,lsl #0x2]
  0008e806: str r5,[r0,#0x0]
  0008e808: adds r6,#0x1
  0008e80a: ldr r0,[r4,#0x10]
  0008e80c: cmp r6,r0
  0008e80e: bcc 0x0008e7ea
  0008e810: ldr r0,[sp,#0x4]
  0008e812: ldr.w r1,[r8,#0x0]
  0008e816: subs r0,r1,r0
  0008e818: itt eq
  0008e81a: pop.eq.w {r2,r3,r8,r9,r11}
  0008e81e: pop.eq {r4,r5,r6,r7,pc}
  0008e820: blx 0x0006e824
```
