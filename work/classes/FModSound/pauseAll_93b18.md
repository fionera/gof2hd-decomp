# FModSound::pauseAll
elf 0x93b18 body 52
Sig: undefined __thiscall pauseAll(FModSound * this)

## decompile
```c

/* FModSound::pauseAll() */

void __thiscall FModSound::pauseAll(FModSound *this)

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
  000a3b18: push {r4,r5,r6,r7,lr}
  000a3b1a: add r7,sp,#0xc
  000a3b1c: push.w r11
  000a3b20: movw r1,#0x23fc
  000a3b24: adds r4,r0,r1
  000a3b26: add.w r5,r0,#0x18
  000a3b2a: movw r6,#0x8f5
  000a3b2e: b 0x000a3b42
  000a3b30: ldr r0,[r4,#0x0]
  000a3b32: cbz r0,0x000a3b3e
  000a3b34: ldr r0,[r5,#0x0]
  000a3b36: cbz r0,0x000a3b3e
  000a3b38: movs r1,#0x1
  000a3b3a: blx 0x000715c0
  000a3b3e: adds r5,#0x4
  000a3b40: subs r6,#0x1
  000a3b42: cmp r6,#0x0
  000a3b44: bne 0x000a3b30
  000a3b46: pop.w r11
  000a3b4a: pop {r4,r5,r6,r7,pc}
```
