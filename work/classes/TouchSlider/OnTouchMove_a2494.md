# TouchSlider::OnTouchMove
elf 0xa2494 body 72
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* TouchSlider::OnTouchMove(int, int) */

bool TouchSlider::OnTouchMove(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  if (*(char *)(param_1 + 0x35) != '\0') {
    return false;
  }
  if (*(char *)(param_1 + 0x34) != '\0') {
    iVar1 = *(int *)(param_1 + 0x14) / 2;
    iVar2 = *(int *)param_1 + iVar1;
    iVar1 = (*(int *)param_1 + *(int *)(param_1 + 0x1c)) - iVar1;
    if (param_2 < iVar1) {
      iVar1 = param_2;
    }
    if (iVar1 <= iVar2) {
      iVar1 = iVar2;
    }
    *(int *)(param_1 + 8) = iVar1;
  }
  return *(char *)(param_1 + 0x34) != '\0';
}

```

## target disasm
```
  000b2494: mov r2,r0
  000b2496: ldrb.w r0,[r0,#0x35]
  000b249a: cbz r0,0x000b24a0
  000b249c: movs r0,#0x0
  000b249e: bx lr
  000b24a0: ldrb.w r0,[r2,#0x34]
  000b24a4: cbz r0,0x000b24d4
  000b24a6: push {r4,r6,r7,lr}
  000b24a8: add r7,sp,#0x8
  000b24aa: ldr.w r12,[r2,#0x14]
  000b24ae: ldr r4,[r2,#0x0]
  000b24b0: ldr.w lr,[r2,#0x1c]
  000b24b4: add.w r3,r12,r12, lsr #0x1f
  000b24b8: add.w r12,r4,r3, asr #0x1
  000b24bc: add r4,lr
  000b24be: sub.w r3,r4,r3, asr #0x1
  000b24c2: cmp r3,r1
  000b24c4: it gt
  000b24c6: mov.gt r3,r1
  000b24c8: cmp r3,r12
  000b24ca: it le
  000b24cc: mov.le r3,r12
  000b24ce: str r3,[r2,#0x8]
  000b24d0: pop.w {r4,r6,r7,lr}
  000b24d4: cmp r0,#0x0
  000b24d6: it ne
  000b24d8: mov.ne r0,#0x1
  000b24da: bx lr
```
