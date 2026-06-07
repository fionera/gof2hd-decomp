# PlayerEgo::startJumpDrive
elf 0x9a6c4 body 62
Sig: undefined __stdcall startJumpDrive(void)

## decompile
```c

/* PlayerEgo::startJumpDrive() */

void PlayerEgo::startJumpDrive(void)

{
  int in_r0;
  float in_s0;
  
  if (*(char *)(in_r0 + 0x204) != '\0') {
    return;
  }
  FModSound::play(**(int **)(DAT_000aa704 + 0xaa6de),(Vector *)0x21,(Vector *)0x0,in_s0);
  Hud::hudEvent(*(int *)(in_r0 + 0x220),(PlayerEgo *)0x19,in_r0);
  *(undefined4 *)(in_r0 + 0x1fc) = 0;
  *(undefined1 *)(in_r0 + 0x204) = 1;
  return;
}

```

## target disasm
```
  000aa6c4: push {r2,r3,r4,r5,r7,lr}
  000aa6c6: add r7,sp,#0x10
  000aa6c8: mov r4,r0
  000aa6ca: ldrb.w r0,[r0,#0x204]
  000aa6ce: cbz r0,0x000aa6d2
  000aa6d0: pop {r2,r3,r4,r5,r7,pc}
  000aa6d2: ldr r0,[0x000aa704]
  000aa6d4: movs r5,#0x0
  000aa6d6: movs r1,#0x21
  000aa6d8: movs r2,#0x0
  000aa6da: add r0,pc
  000aa6dc: movs r3,#0x0
  000aa6de: str r5,[sp,#0x0]
  000aa6e0: ldr r0,[r0,#0x0]
  000aa6e2: ldr r0,[r0,#0x0]
  000aa6e4: blx 0x00071548
  000aa6e8: ldr.w r0,[r4,#0x220]
  000aa6ec: movs r1,#0x19
  000aa6ee: mov r2,r4
  000aa6f0: movs r3,#0x0
  000aa6f2: blx 0x00072418
  000aa6f6: movs r0,#0x1
  000aa6f8: str.w r5,[r4,#0x1fc]
  000aa6fc: strb.w r0,[r4,#0x204]
  000aa700: pop {r2,r3,r4,r5,r7,pc}
```
