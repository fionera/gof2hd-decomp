# ListItemWindow::OnTouchBegin
elf 0x133a74 body 90
Sig: undefined __thiscall OnTouchBegin(ListItemWindow * this, int param_1, int param_2)

## decompile
```c

/* ListItemWindow::OnTouchBegin(int, int) */

void __thiscall ListItemWindow::OnTouchBegin(ListItemWindow *this,int param_1,int param_2)

{
  ScrollTouchWindow::OnTouchBegin(*(int *)(this + 0x18),param_1);
  if (((this[0x54] != (ListItemWindow)0x0) &&
      (*(int *)(this + 100) + (*(int *)(this + 0x6c) >> 1) < param_1)) &&
     (param_2 < *(int *)(**(int **)(DAT_00143ad0 + 0x143aa2) + 0x20) +
                *(int *)(this + 0x68) + *(int *)(**(int **)(DAT_00143ad0 + 0x143aa2) + 0xc) +
                *(int *)(this + 0x20))) {
    *(int *)(this + 0x134) = param_1;
    *(int *)(this + 0x120) = param_1;
    *(undefined4 *)(this + 0x128) = 0;
    this[0x138] = (ListItemWindow)0x1;
  }
  return;
}

```

## target disasm
```
  00143a74: push {r4,r5,r6,r7,lr}
  00143a76: add r7,sp,#0xc
  00143a78: push.w r11
  00143a7c: mov r4,r0
  00143a7e: ldr r0,[r0,#0x18]
  00143a80: mov r6,r2
  00143a82: mov r5,r1
  00143a84: blx 0x00075ba4
  00143a88: ldrb.w r0,[r4,#0x54]
  00143a8c: cbz r0,0x00143ac8
  00143a8e: ldr r1,[r4,#0x6c]
  00143a90: ldr r0,[r4,#0x64]
  00143a92: add.w r0,r0,r1, asr #0x1
  00143a96: cmp r0,r5
  00143a98: bge 0x00143ac8
  00143a9a: ldr r0,[0x00143ad0]
  00143a9c: ldr r2,[r4,#0x68]
  00143a9e: add r0,pc
  00143aa0: ldr r1,[r4,#0x20]
  00143aa2: ldr r0,[r0,#0x0]
  00143aa4: ldr r0,[r0,#0x0]
  00143aa6: ldr r3,[r0,#0xc]
  00143aa8: ldr r0,[r0,#0x20]
  00143aaa: add r2,r3
  00143aac: add r0,r2
  00143aae: add r0,r1
  00143ab0: cmp r0,r6
  00143ab2: ble 0x00143ac8
  00143ab4: movs r0,#0x0
  00143ab6: str.w r5,[r4,#0x134]
  00143aba: str.w r5,[r4,#0x120]
  00143abe: str.w r0,[r4,#0x128]
  00143ac2: movs r0,#0x1
  00143ac4: strb.w r0,[r4,#0x138]
  00143ac8: pop.w r11
  00143acc: pop {r4,r5,r6,r7,pc}
```
