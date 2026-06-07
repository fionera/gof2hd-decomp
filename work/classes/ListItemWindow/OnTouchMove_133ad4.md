# ListItemWindow::OnTouchMove
elf 0x133ad4 body 48
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* ListItemWindow::OnTouchMove(int, int) */

void ListItemWindow::OnTouchMove(int param_1,int param_2)

{
  int iVar1;
  
  ScrollTouchWindow::OnTouchMove(*(int *)(param_1 + 0x18),param_2);
  if ((*(char *)(param_1 + 0x54) != '\0') && (*(char *)(param_1 + 0x138) != '\0')) {
    iVar1 = param_2 - *(int *)(param_1 + 0x120);
    *(int *)(param_1 + 0x128) = iVar1;
    *(undefined4 *)(param_1 + 300) = 0x3f800000;
    *(int *)(param_1 + 0x11c) = *(int *)(param_1 + 0x11c) + iVar1;
    *(int *)(param_1 + 0x120) = param_2;
  }
  return;
}

```

## target disasm
```
  00143ad4: push {r4,r5,r7,lr}
  00143ad6: add r7,sp,#0x8
  00143ad8: mov r5,r0
  00143ada: ldr r0,[r0,#0x18]
  00143adc: mov r4,r1
  00143ade: blx 0x00075bbc
  00143ae2: ldrb.w r0,[r5,#0x54]
  00143ae6: cbz r0,0x00143b02
  00143ae8: ldrb.w r0,[r5,#0x138]
  00143aec: cbz r0,0x00143b02
  00143aee: ldrd r0,r1,[r5,#0x11c]
  00143af2: mov.w r2,#0x3f800000
  00143af6: subs r1,r4,r1
  00143af8: strd r1,r2,[r5,#0x128]
  00143afc: add r0,r1
  00143afe: strd r0,r4,[r5,#0x11c]
  00143b02: pop {r4,r5,r7,pc}
```
