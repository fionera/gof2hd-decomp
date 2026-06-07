# KIPlayer::cargoAvailable
elf 0xa6304 body 36
Sig: undefined __stdcall cargoAvailable(void)

## decompile
```c

/* KIPlayer::cargoAvailable() */

undefined4 KIPlayer::cargoAvailable(void)

{
  int iVar1;
  int in_r0;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = *(uint **)(in_r0 + 0x50);
  if (puVar2 != (uint *)0x0) {
    uVar3 = 0;
    while (uVar3 < *puVar2) {
      iVar1 = uVar3 * 4;
      uVar3 = uVar3 + 2;
      if (0 < *(int *)(puVar2[1] + iVar1 + 4)) {
        return 1;
      }
    }
  }
  return 0;
}

```

## target disasm
```
  000b6304: ldr r0,[r0,#0x50]
  000b6306: cbz r0,0x000b6324
  000b6308: ldr r1,[r0,#0x0]
  000b630a: movs r2,#0x0
  000b630c: b 0x000b6320
  000b630e: ldr r3,[r0,#0x4]
  000b6310: add.w r3,r3,r2, lsl #0x2
  000b6314: adds r2,#0x2
  000b6316: ldr r3,[r3,#0x4]
  000b6318: cmp r3,#0x1
  000b631a: itt ge
  000b631c: mov.ge r0,#0x1
  000b631e: bx.ge lr
  000b6320: cmp r2,r1
  000b6322: bcc 0x000b630e
  000b6324: movs r0,#0x0
  000b6326: bx lr
```
