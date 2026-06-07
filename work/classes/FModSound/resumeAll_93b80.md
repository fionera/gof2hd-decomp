# FModSound::resumeAll
elf 0x93b80 body 52
Sig: undefined __thiscall resumeAll(FModSound * this)

## decompile
```c

/* FModSound::resumeAll() */

void __thiscall FModSound::resumeAll(FModSound *this)

{
  FModSound *pFVar1;
  int iVar2;
  
  pFVar1 = this + 0x18;
  for (iVar2 = 0x8f5; iVar2 != 0; iVar2 = iVar2 + -1) {
    if ((*(int *)(this + 0x23fc) != 0) && (*(int *)pFVar1 != 0)) {
      FMOD::Event::setPaused(SUB41(*(int *)pFVar1,0));
    }
    pFVar1 = pFVar1 + 4;
  }
  return;
}

```

## target disasm
```
  000a3b80: push {r4,r5,r6,r7,lr}
  000a3b82: add r7,sp,#0xc
  000a3b84: push.w r11
  000a3b88: movw r1,#0x23fc
  000a3b8c: adds r4,r0,r1
  000a3b8e: add.w r5,r0,#0x18
  000a3b92: movw r6,#0x8f5
  000a3b96: b 0x000a3baa
  000a3b98: ldr r0,[r4,#0x0]
  000a3b9a: cbz r0,0x000a3ba6
  000a3b9c: ldr r0,[r5,#0x0]
  000a3b9e: cbz r0,0x000a3ba6
  000a3ba0: movs r1,#0x0
  000a3ba2: blx 0x000715c0
  000a3ba6: adds r5,#0x4
  000a3ba8: subs r6,#0x1
  000a3baa: cmp r6,#0x0
  000a3bac: bne 0x000a3b98
  000a3bae: pop.w r11
  000a3bb2: pop {r4,r5,r6,r7,pc}
```
