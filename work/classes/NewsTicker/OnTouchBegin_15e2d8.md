# NewsTicker::OnTouchBegin
elf 0x15e2d8 body 70
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* NewsTicker::OnTouchBegin(int, int) */

undefined1 NewsTicker::OnTouchBegin(int param_1,int param_2)

{
  int in_r2;
  
  if ((((*(int *)(param_1 + 4) <= param_2) &&
       (param_2 <= *(int *)(param_1 + 0xc) + *(int *)(param_1 + 4))) &&
      (*(int *)(param_1 + 8) <= in_r2)) &&
     (in_r2 <= (**(int **)(DAT_0016e324 + 0x16e2fc) + 2) -
               *(int *)(**(int **)(DAT_0016e320 + 0x16e2fa) + 0x10))) {
    *(int *)(param_1 + 0x2c) = param_2;
    *(undefined1 *)(param_1 + 0x28) = 1;
  }
  return *(undefined1 *)(param_1 + 0x28);
}

```

## target disasm
```
  0016e2d8: ldr.w r12,[r0,#0x4]
  0016e2dc: cmp r12,r1
  0016e2de: bgt 0x0016e318
  0016e2e0: ldr r3,[r0,#0xc]
  0016e2e2: add r3,r12
  0016e2e4: cmp r3,r1
  0016e2e6: blt 0x0016e318
  0016e2e8: ldr r3,[r0,#0x8]
  0016e2ea: cmp r3,r2
  0016e2ec: bgt 0x0016e318
  0016e2ee: push {r4,r6,r7,lr}
  0016e2f0: add r7,sp,#0x8
  0016e2f2: ldr r3,[0x0016e320]
  0016e2f4: ldr r4,[0x0016e324]
  0016e2f6: add r3,pc
  0016e2f8: add r4,pc
  0016e2fa: ldr r3,[r3,#0x0]
  0016e2fc: ldr r4,[r4,#0x0]
  0016e2fe: ldr r3,[r3,#0x0]
  0016e300: ldr r4,[r4,#0x0]
  0016e302: ldr r3,[r3,#0x10]
  0016e304: adds r4,#0x2
  0016e306: subs r3,r4,r3
  0016e308: cmp r3,r2
  0016e30a: pop.w {r4,r6,r7,lr}
  0016e30e: ittt ge
  0016e310: str.ge r1,[r0,#0x2c]
  0016e312: mov.ge r1,#0x1
  0016e314: strb.ge.w r1,[r0,#0x28]
  0016e318: ldrb.w r0,[r0,#0x28]
  0016e31c: bx lr
```
