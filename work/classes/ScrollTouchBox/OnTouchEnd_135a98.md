# ScrollTouchBox::OnTouchEnd
elf 0x135a98 body 58
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* ScrollTouchBox::OnTouchEnd(int, int) */

void ScrollTouchBox::OnTouchEnd(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  uint in_fpscr;
  undefined4 uVar3;
  undefined4 uVar4;
  
  if (*(char *)(param_1 + 0x30) != '\0') {
    iVar1 = *(int *)(param_1 + 0x1c);
    uVar4 = VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    iVar2 = iVar1;
    if (iVar1 < 0) {
      iVar2 = -iVar1;
    }
    uVar3 = DAT_00145ad4;
    if (3 < iVar2) {
      uVar3 = uVar4;
    }
    *(undefined4 *)(param_1 + 0x20) = DAT_00145ad8;
    *(undefined1 *)(param_1 + 0x30) = 0;
    *(int *)(param_1 + 0x34) = iVar1 + *(int *)(param_1 + 0x34);
    *(undefined4 *)(param_1 + 0x24) = uVar3;
  }
  return;
}

```

## target disasm
```
  00145a98: ldrb.w r1,[r0,#0x30]
  00145a9c: cbz r1,0x00145ad0
  00145a9e: ldr r1,[r0,#0x1c]
  00145aa0: ldr r2,[r0,#0x34]
  00145aa2: vldr.32 s0,[pc,#0x30]
  00145aa6: cmp r1,#0x0
  00145aa8: vmov s2,r1
  00145aac: mov r3,r1
  00145aae: vcvt.f32.s32 s2,s2
  00145ab2: it mi
  00145ab4: rsb.mi r3,r1
  00145ab6: cmp r3,#0x3
  00145ab8: ldr r3,[0x00145ad8]
  00145aba: add r1,r2
  00145abc: it gt
  00145abe: vmov.gt.f32 s0,s2
  00145ac2: str r3,[r0,#0x20]
  00145ac4: movs r3,#0x0
  00145ac6: strb.w r3,[r0,#0x30]
  00145aca: str r1,[r0,#0x34]
  00145acc: vstr.32 s0,[r0,#0x24]
  00145ad0: bx lr
```
