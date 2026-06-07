# TouchButton::OnTouchEnd
elf 0x169a54 body 76
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* TouchButton::OnTouchEnd(int, int) */

undefined4 TouchButton::OnTouchEnd(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int in_r2;
  float extraout_s0;
  
  if ((*(char *)(param_1 + 0xb2) == '\0') || (*(char *)(param_1 + 0xb3) != '\0')) {
    uVar2 = 0;
  }
  else if ((*(char *)(param_1 + 0xb0) == '\0') ||
          (iVar3 = touchedInside((TouchButton *)param_1,param_2,in_r2), iVar1 = DAT_00179aa0,
          iVar3 == 0)) {
    uVar2 = 0;
    *(undefined1 *)(param_1 + 0xb0) = 0;
  }
  else {
    *(undefined1 *)(param_1 + 0xb0) = 0;
    FModSound::play(**(int **)(iVar1 + 0x179a88),(Vector *)0x7b,(Vector *)0x0,extraout_s0);
    uVar2 = 1;
  }
  return uVar2;
}

```

## target disasm
```
  00179a54: push {r2,r3,r4,r6,r7,lr}
  00179a56: add r7,sp,#0x10
  00179a58: mov r4,r0
  00179a5a: ldrb.w r0,[r0,#0xb2]
  00179a5e: cbz r0,0x00179a66
  00179a60: ldrb.w r0,[r4,#0xb3]
  00179a64: cbz r0,0x00179a6c
  00179a66: movs r0,#0x0
  00179a68: add sp,#0x8
  00179a6a: pop {r4,r6,r7,pc}
  00179a6c: ldrb.w r0,[r4,#0xb0]
  00179a70: cbz r0,0x00179a98
  00179a72: mov r0,r4
  00179a74: blx 0x00077c8c
  00179a78: cbz r0,0x00179a98
  00179a7a: ldr r0,[0x00179aa0]
  00179a7c: movs r1,#0x0
  00179a7e: strb.w r1,[r4,#0xb0]
  00179a82: movs r2,#0x0
  00179a84: add r0,pc
  00179a86: str r1,[sp,#0x0]
  00179a88: movs r1,#0x7b
  00179a8a: movs r3,#0x0
  00179a8c: ldr r0,[r0,#0x0]
  00179a8e: ldr r0,[r0,#0x0]
  00179a90: blx 0x00071548
  00179a94: movs r0,#0x1
  00179a96: b 0x00179a68
  00179a98: movs r0,#0x0
  00179a9a: strb.w r0,[r4,#0xb0]
  00179a9e: b 0x00179a68
```
