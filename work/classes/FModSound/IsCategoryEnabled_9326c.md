# FModSound::IsCategoryEnabled
elf 0x9326c body 28
Sig: undefined __stdcall IsCategoryEnabled(int param_1)

## decompile
```c

/* FModSound::IsCategoryEnabled(int) */

undefined1 FModSound::IsCategoryEnabled(int param_1)

{
  undefined1 uVar1;
  int in_r1;
  
  uVar1 = 0;
  if (((in_r1 < 4) && (*(int *)(param_1 + 0x23fc) != 0)) &&
     (uVar1 = 0, *(char *)(param_1 + in_r1 + 0x11) != '\0')) {
    uVar1 = 1;
  }
  return uVar1;
}

```

## target disasm
```
  000a326c: mov r2,r0
  000a326e: movs r0,#0x0
  000a3270: cmp r1,#0x3
  000a3272: bgt 0x000a3286
  000a3274: movw r3,#0x23fc
  000a3278: ldr r3,[r2,r3]
  000a327a: cbz r3,0x000a3286
  000a327c: adds r0,r2,r1
  000a327e: ldrb r0,[r0,#0x11]
  000a3280: cmp r0,#0x0
  000a3282: it ne
  000a3284: mov.ne r0,#0x1
  000a3286: bx lr
```
