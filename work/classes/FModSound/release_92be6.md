# FModSound::release
elf 0x92be6 body 74
Sig: undefined __thiscall release(FModSound * this)

## decompile
```c

/* FModSound::release() */

void __thiscall FModSound::release(FModSound *this)

{
  FModSound *pFVar1;
  int iVar2;
  
  if (*(int *)(this + 0x23fc) != 0) {
    FMOD::EventSystem::unload();
    FMOD::EventSystem::release();
    *(undefined4 *)(this + 0x23fc) = 0;
  }
  pFVar1 = this + 0x18;
  for (iVar2 = 0x8f5; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined4 *)pFVar1 = 0;
    pFVar1 = pFVar1 + 4;
  }
  for (iVar2 = 0; iVar2 != 4; iVar2 = iVar2 + 1) {
    *(undefined4 *)(this + iVar2 * 4 + 0x23ec) = 0;
  }
  return;
}

```

## target disasm
```
  000a2be6: push {r4,r5,r7,lr}
  000a2be8: add r7,sp,#0x8
  000a2bea: movw r1,#0x23fc
  000a2bee: mov r4,r0
  000a2bf0: ldr r0,[r0,r1]
  000a2bf2: cbz r0,0x000a2c04
  000a2bf4: adds r5,r4,r1
  000a2bf6: blx 0x00071458
  000a2bfa: ldr r0,[r5,#0x0]
  000a2bfc: blx 0x00071464
  000a2c00: movs r0,#0x0
  000a2c02: str r0,[r5,#0x0]
  000a2c04: add.w r0,r4,#0x18
  000a2c08: movw r1,#0x8f5
  000a2c0c: movs r2,#0x0
  000a2c0e: b 0x000a2c14
  000a2c10: stmia r0!,{r2}
  000a2c12: subs r1,#0x1
  000a2c14: cmp r1,#0x0
  000a2c16: bne 0x000a2c10
  000a2c18: movw r0,#0x23ec
  000a2c1c: movs r1,#0x0
  000a2c1e: add r0,r4
  000a2c20: movs r2,#0x0
  000a2c22: b 0x000a2c2a
  000a2c24: str.w r1,[r0,r2,lsl #0x2]
  000a2c28: adds r2,#0x1
  000a2c2a: cmp r2,#0x4
  000a2c2c: bne 0x000a2c24
  000a2c2e: pop {r4,r5,r7,pc}
```
