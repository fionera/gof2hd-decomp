# FModSound::stopAll
elf 0x93b4c body 52
Sig: undefined __stdcall stopAll(void)

## decompile
```c

/* FModSound::stopAll() */

void FModSound::stopAll(void)

{
  int in_r0;
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)(in_r0 + 0x18);
  for (iVar2 = 0x8f5; iVar2 != 0; iVar2 = iVar2 + -1) {
    if ((*(int *)(in_r0 + 0x23fc) != 0) && (*piVar1 != 0)) {
      FMOD::Event::stop(SUB41(*piVar1,0));
    }
    piVar1 = piVar1 + 1;
  }
  return;
}

```

## target disasm
```
  000a3b4c: push {r4,r5,r6,r7,lr}
  000a3b4e: add r7,sp,#0xc
  000a3b50: push.w r11
  000a3b54: movw r1,#0x23fc
  000a3b58: adds r4,r0,r1
  000a3b5a: add.w r5,r0,#0x18
  000a3b5e: movw r6,#0x8f5
  000a3b62: b 0x000a3b76
  000a3b64: ldr r0,[r4,#0x0]
  000a3b66: cbz r0,0x000a3b72
  000a3b68: ldr r0,[r5,#0x0]
  000a3b6a: cbz r0,0x000a3b72
  000a3b6c: movs r1,#0x1
  000a3b6e: blx 0x000715a8
  000a3b72: adds r5,#0x4
  000a3b74: subs r6,#0x1
  000a3b76: cmp r6,#0x0
  000a3b78: bne 0x000a3b64
  000a3b7a: pop.w r11
  000a3b7e: pop {r4,r5,r6,r7,pc}
```
