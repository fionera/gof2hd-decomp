# ListItemWindow::OnTouchEnd
elf 0x133b04 body 88
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* ListItemWindow::OnTouchEnd(int, int) */

void ListItemWindow::OnTouchEnd(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  uint in_fpscr;
  undefined4 uVar3;
  undefined4 uVar4;
  
  ScrollTouchWindow::OnTouchEnd(*(int *)(param_1 + 0x18),param_2);
  if ((*(char *)(param_1 + 0x54) != '\0') && (*(char *)(param_1 + 0x138) != '\0')) {
    iVar2 = *(int *)(param_1 + 0x128);
    iVar1 = *(int *)(param_1 + 0x11c) + iVar2;
    uVar4 = VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    if (iVar2 < 0) {
      iVar2 = -iVar2;
    }
    uVar3 = DAT_00143b5c;
    if (3 < iVar2) {
      uVar3 = uVar4;
    }
    *(undefined4 *)(param_1 + 300) = DAT_00143b60;
    *(undefined1 *)(param_1 + 0x138) = 0;
    *(int *)(param_1 + 0x11c) = iVar1;
    *(int *)(param_1 + 0x124) = iVar1;
    *(undefined4 *)(param_1 + 0x130) = uVar3;
  }
  return;
}

```

## target disasm
```
  00143b04: push {r4,r6,r7,lr}
  00143b06: add r7,sp,#0x8
  00143b08: mov r4,r0
  00143b0a: ldr r0,[r0,#0x18]
  00143b0c: blx 0x00075bc8
  00143b10: ldrb.w r0,[r4,#0x54]
  00143b14: cbz r0,0x00143b5a
  00143b16: ldrb.w r0,[r4,#0x138]
  00143b1a: cbz r0,0x00143b5a
  00143b1c: ldr.w r1,[r4,#0x128]
  00143b20: ldr.w r0,[r4,#0x11c]
  00143b24: vldr.32 s0,[pc,#0x34]
  00143b28: cmp r1,#0x0
  00143b2a: vmov s2,r1
  00143b2e: mov r2,r1
  00143b30: add r0,r1
  00143b32: vcvt.f32.s32 s2,s2
  00143b36: it mi
  00143b38: rsb.mi r2,r1
  00143b3a: cmp r2,#0x3
  00143b3c: ldr r2,[0x00143b60]
  00143b3e: it gt
  00143b40: vmov.gt.f32 s0,s2
  00143b44: str.w r2,[r4,#0x12c]
  00143b48: movs r2,#0x0
  00143b4a: strb.w r2,[r4,#0x138]
  00143b4e: str.w r0,[r4,#0x11c]
  00143b52: str.w r0,[r4,#0x124]
  00143b56: vstr.32 s0,[r4,#0x130]
  00143b5a: pop {r4,r6,r7,pc}
```
