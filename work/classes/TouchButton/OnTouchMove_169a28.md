# TouchButton::OnTouchMove
elf 0x169a28 body 44
Sig: undefined __stdcall OnTouchMove(int param_1, int param_2)

## decompile
```c

/* TouchButton::OnTouchMove(int, int) */

undefined4 TouchButton::OnTouchMove(int param_1,int param_2)

{
  undefined4 uVar1;
  int in_r2;
  
  if ((*(char *)(param_1 + 0xb2) != '\0') && (*(char *)(param_1 + 0xb3) == '\0')) {
    if (*(char *)(param_1 + 0xb0) == '\0') {
      uVar1 = 0;
    }
    else {
      uVar1 = touchedInside((TouchButton *)param_1,param_2,in_r2);
    }
    *(char *)(param_1 + 0xb0) = (char)uVar1;
    return uVar1;
  }
  return 0;
}

```

## target disasm
```
  00179a28: push {r4,r6,r7,lr}
  00179a2a: add r7,sp,#0x8
  00179a2c: mov r4,r0
  00179a2e: ldrb.w r0,[r0,#0xb2]
  00179a32: cbz r0,0x00179a3a
  00179a34: ldrb.w r0,[r4,#0xb3]
  00179a38: cbz r0,0x00179a3e
  00179a3a: movs r0,#0x0
  00179a3c: pop {r4,r6,r7,pc}
  00179a3e: ldrb.w r0,[r4,#0xb0]
  00179a42: cbz r0,0x00179a4c
  00179a44: mov r0,r4
  00179a46: blx 0x00077c8c
  00179a4a: b 0x00179a4e
  00179a4c: movs r0,#0x0
  00179a4e: strb.w r0,[r4,#0xb0]
  00179a52: pop {r4,r6,r7,pc}
```
