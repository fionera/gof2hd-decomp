# PlayerEgo::hideShipForFirstPersonCameraView
elf 0xa1c56 body 50
Sig: undefined __stdcall hideShipForFirstPersonCameraView(bool param_1)

## decompile
```c

/* PlayerEgo::hideShipForFirstPersonCameraView(bool) */

void PlayerEgo::hideShipForFirstPersonCameraView(bool param_1)

{
  uint uVar1;
  byte in_r1;
  
  uVar1 = (uint)param_1;
  *(byte *)(uVar1 + 0x32d) = in_r1;
  *(byte *)(uVar1 + 0x309) = *(char *)(uVar1 + 0x32e) != '\0' & (in_r1 ^ 1);
  **(byte **)(*(int *)(uVar1 + 0xc) + 0x80) = (in_r1 ^ 1) & *(char *)(uVar1 + 0x32f) != '\0';
  return;
}

```

## target disasm
```
  000b1c56: ldrb.w r3,[r0,#0x32e]
  000b1c5a: ldrb.w r2,[r0,#0x32f]
  000b1c5e: strb.w r1,[r0,#0x32d]
  000b1c62: eor r1,r1,#0x1
  000b1c66: ldr.w r12,[r0,#0xc]
  000b1c6a: cmp r3,#0x0
  000b1c6c: it ne
  000b1c6e: mov.ne r3,#0x1
  000b1c70: cmp r2,#0x0
  000b1c72: and.w r3,r3,r1
  000b1c76: strb.w r3,[r0,#0x309]
  000b1c7a: ldr.w r0,[r12,#0x80]
  000b1c7e: it ne
  000b1c80: mov.ne r2,#0x1
  000b1c82: ands r1,r2
  000b1c84: strb r1,[r0,#0x0]
  000b1c86: bx lr
```
