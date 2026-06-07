# FModSound::pauseAllPlayingSoundFXEvents
elf 0x93bfc body 134
Sig: undefined __thiscall pauseAllPlayingSoundFXEvents(FModSound * this)

## decompile
```c

/* FModSound::pauseAllPlayingSoundFXEvents() */

void __thiscall FModSound::pauseAllPlayingSoundFXEvents(FModSound *this)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int unaff_r4;
  int iVar4;
  undefined4 *unaff_r6;
  
  piVar1 = *(int **)(DAT_000a3c84 + 0xa3c14);
  iVar2 = *piVar1;
  for (iVar4 = 0; iVar4 != 0x8f5; iVar4 = iVar4 + 1) {
    if (((*(int *)(this + 0x23fc) != 0) && (*(int *)(this + iVar4 * 4 + 0x18) != 0)) &&
       (iVar3 = isPlaying((int)this), iVar3 != 0)) {
      FMOD::Event::getCategory(*(EventCategory ***)(this + iVar4 * 4 + 0x18));
      (**(code **)*unaff_r6)(unaff_r6,&stack0xffffffcc,&stack0xffffffd0);
      if (unaff_r4 == 1) {
        FMOD::Event::setPaused(SUB41(*(undefined4 *)(this + iVar4 * 4 + 0x18),0));
      }
    }
  }
  if (*piVar1 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000a3bfc: push {r4,r5,r6,r7,lr}
  000a3bfe: add r7,sp,#0xc
  000a3c00: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000a3c04: mov r4,r0
  000a3c06: ldr r0,[0x000a3c84]
  000a3c08: add.w r9,sp,#0x4
  000a3c0c: add.w r11,sp,#0x8
  000a3c10: add r0,pc
  000a3c12: movs r5,#0x0
  000a3c14: movw r10,#0x8f5
  000a3c18: ldr r0,[r0,#0x0]
  000a3c1a: str r0,[sp,#0x0]
  000a3c1c: ldr r0,[r0,#0x0]
  000a3c1e: str r0,[sp,#0x10]
  000a3c20: movw r0,#0x23fc
  000a3c24: adds r6,r4,r0
  000a3c26: b 0x000a3c68
  000a3c28: ldr r0,[r6,#0x0]
  000a3c2a: cbz r0,0x000a3c66
  000a3c2c: add.w r8,r4,r5, lsl #0x2
  000a3c30: ldr.w r0,[r8,#0x18]
  000a3c34: cbz r0,0x000a3c66
  000a3c36: mov r0,r4
  000a3c38: mov r1,r5
  000a3c3a: blx 0x0007153c
  000a3c3e: cbz r0,0x000a3c66
  000a3c40: ldr.w r0,[r8,#0x18]
  000a3c44: add r1,sp,#0xc
  000a3c46: blx 0x0007156c
  000a3c4a: ldr r0,[sp,#0xc]
  000a3c4c: mov r2,r11
  000a3c4e: ldr r1,[r0,#0x0]
  000a3c50: ldr r3,[r1,#0x0]
  000a3c52: mov r1,r9
  000a3c54: blx r3
  000a3c56: ldr r0,[sp,#0x4]
  000a3c58: cmp r0,#0x1
  000a3c5a: bne 0x000a3c66
  000a3c5c: ldr.w r0,[r8,#0x18]
  000a3c60: movs r1,#0x1
  000a3c62: blx 0x000715c0
  000a3c66: adds r5,#0x1
  000a3c68: cmp r5,r10
  000a3c6a: bne 0x000a3c28
  000a3c6c: ldr r0,[sp,#0x10]
  000a3c6e: ldr r1,[sp,#0x0]
  000a3c70: ldr r1,[r1,#0x0]
  000a3c72: subs r0,r1,r0
  000a3c74: ittt eq
  000a3c76: add.eq sp,#0x14
  000a3c78: pop.eq.w {r8,r9,r10,r11}
  000a3c7c: pop.eq {r4,r5,r6,r7,pc}
  000a3c7e: blx 0x0006e824
```
