# FModSound::pauseAllPlaying
elf 0x93bb4 body 70
Sig: undefined __stdcall pauseAllPlaying(void)

## decompile
```c

/* FModSound::pauseAllPlaying() */

void FModSound::pauseAllPlaying(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  int iVar3;
  
  for (iVar2 = 0; iVar2 != 0x8f5; iVar2 = iVar2 + 1) {
    if (((*(int *)(in_r0 + 0x23fc) != 0) && (iVar3 = in_r0 + iVar2 * 4, *(int *)(iVar3 + 0x18) != 0)
        ) && (iVar1 = isPlaying(in_r0), iVar1 != 0)) {
      FMOD::Event::setPaused(SUB41(*(undefined4 *)(iVar3 + 0x18),0));
    }
  }
  return;
}

```

## target disasm
```
  000a3bb4: push {r4,r5,r6,r7,lr}
  000a3bb6: add r7,sp,#0xc
  000a3bb8: push {r8,r9,r11}
  000a3bbc: mov r4,r0
  000a3bbe: movw r0,#0x23fc
  000a3bc2: add.w r9,r4,r0
  000a3bc6: movs r5,#0x0
  000a3bc8: movw r8,#0x8f5
  000a3bcc: b 0x000a3bf0
  000a3bce: ldr.w r0,[r9,#0x0]
  000a3bd2: cbz r0,0x000a3bee
  000a3bd4: add.w r6,r4,r5, lsl #0x2
  000a3bd8: ldr r0,[r6,#0x18]
  000a3bda: cbz r0,0x000a3bee
  000a3bdc: mov r0,r4
  000a3bde: mov r1,r5
  000a3be0: blx 0x0007153c
  000a3be4: cbz r0,0x000a3bee
  000a3be6: ldr r0,[r6,#0x18]
  000a3be8: movs r1,#0x1
  000a3bea: blx 0x000715c0
  000a3bee: adds r5,#0x1
  000a3bf0: cmp r5,r8
  000a3bf2: bne 0x000a3bce
  000a3bf4: pop.w {r8,r9,r11}
  000a3bf8: pop {r4,r5,r6,r7,pc}
```
