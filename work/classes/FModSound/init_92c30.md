# FModSound::init
elf 0x92c30 body 316
Sig: int __thiscall init(FModSound * this, EVP_PKEY_CTX * ctx)

## decompile
```c

/* FModSound::init() */

int __thiscall FModSound::init(FModSound *this,EVP_PKEY_CTX *ctx)

{
  FModSound FVar1;
  FMOD_EVENT_LOADINFO *pFVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  FModSound *pFVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  FMOD_EVENT_LOADINFO aFStack_428 [6];
  undefined1 auStack_422 [3];
  undefined8 auStack_41f [126];
  int iStack_28;
  
  piVar5 = *(int **)(DAT_000a2d6c + 0xa2c4c);
  iStack_28 = *piVar5;
  for (iVar3 = 0; iVar3 != 5; iVar3 = iVar3 + 1) {
    *(undefined4 *)(this + iVar3 * 4 + 0x2410) = 0xffffffff;
  }
  pFVar6 = this + 0x23fc;
  FMOD_EventSystem_Create(pFVar6);
  FMOD::EventSystem::init(*(int *)(this + 0x23fc),0x20,(void *)0x82,0);
  iVar3 = GameText::getLanguage();
  setAudioLanguage(this,iVar3);
  __aeabi_memclr8(aFStack_428,0x400);
  strcpy(aFStack_428,*(undefined4 *)(this + 0xc));
  FVar1 = this[0x10];
  iVar3 = strlen(aFStack_428);
  pFVar2 = aFStack_428 + iVar3;
  if (FVar1 == (FModSound)0x0) {
    uVar7 = *(undefined8 *)(DAT_000a2d70 + 0xa2ce2);
    uVar8 = *(undefined8 *)(DAT_000a2d70 + 0xa2ce3);
    uVar9 = *(undefined8 *)(DAT_000a2d70 + 0xa2ceb);
    *(undefined8 *)pFVar2 = *(undefined8 *)(DAT_000a2d70 + 0xa2cda);
    *(undefined8 *)(auStack_422 + iVar3 + 2) = uVar7;
    *(undefined8 *)(auStack_422 + iVar3 + 3) = uVar8;
    *(undefined8 *)((int)auStack_41f + iVar3 + 8) = uVar9;
  }
  else {
    *(undefined8 *)pFVar2 = *(undefined8 *)(DAT_000a2d74 + 0xa2cbe);
    *(undefined8 *)(auStack_422 + iVar3) = *(undefined8 *)(DAT_000a2d74 + 0xa2cc4);
  }
  FMOD::EventSystem::load(*(char **)pFVar6,aFStack_428,(EventProject **)0x0);
  iVar3 = DAT_000a2d78 + 0xa2d0a;
  for (iVar4 = 0; iVar4 != 0x10; iVar4 = iVar4 + 4) {
    *(undefined4 *)(this + iVar4 + 0x23ec) = 0;
    FMOD::EventSystem::getCategory(*(char **)pFVar6,*(EventCategory ***)(iVar3 + iVar4));
  }
  FMOD::EventSystem::getProjectByIndex(*(int *)pFVar6,(EventProject **)0x0);
  *(undefined4 *)(this + 0x240c) = 0;
  *(undefined4 *)(this + 0x2408) = 0xffffffff;
  *(undefined4 *)this = 0xffffffff;
  *(undefined4 *)(this + 4) = 0xffffffff;
  this[8] = (FModSound)0x0;
  if (*piVar5 != iStack_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}

```

## target disasm
```
  000a2c30: push {r4,r5,r6,r7,lr}
  000a2c32: add r7,sp,#0xc
  000a2c34: push {r8,r9,r10,r11}
  000a2c38: sub.w sp,sp,#0x408
  000a2c3c: sub sp,#0x4
  000a2c3e: mov r9,r0
  000a2c40: ldr r0,[0x000a2d6c]
  000a2c42: movs r1,#0x0
  000a2c44: mov.w r2,#0xffffffff
  000a2c48: add r0,pc
  000a2c4a: ldr.w r8,[r0,#0x0]
  000a2c4e: ldr.w r0,[r8,#0x0]
  000a2c52: str.w r0,[r7,#-0x20]
  000a2c56: movw r0,#0x2410
  000a2c5a: add r0,r9
  000a2c5c: b 0x000a2c64
  000a2c5e: str.w r2,[r0,r1,lsl #0x2]
  000a2c62: adds r1,#0x1
  000a2c64: cmp r1,#0x5
  000a2c66: bne 0x000a2c5e
  000a2c68: movw r4,#0x23fc
  000a2c6c: add.w r10,r9,r4
  000a2c70: mov r0,r10
  000a2c72: blx 0x00071470
  000a2c76: ldr.w r0,[r9,r4]
  000a2c7a: movs r1,#0x0
  000a2c7c: str r1,[sp,#0x0]
  000a2c7e: movs r1,#0x20
  000a2c80: movs r2,#0x82
  000a2c82: movs r3,#0x0
  000a2c84: blx 0x0007147c
  000a2c88: blx 0x0006f544
  000a2c8c: mov r1,r0
  000a2c8e: mov r0,r9
  000a2c90: blx 0x00071488
  000a2c94: add r6,sp,#0x8
  000a2c96: mov.w r1,#0x400
  000a2c9a: mov r0,r6
  000a2c9c: blx 0x00071494
  000a2ca0: ldr.w r1,[r9,#0xc]
  000a2ca4: mov r0,r6
  000a2ca6: blx 0x0006e9b0
  000a2caa: mov r0,r6
  000a2cac: ldrb.w r4,[r9,#0x10]
  000a2cb0: blx 0x0006e998
  000a2cb4: add r0,r6
  000a2cb6: cbz r4,0x000a2cd2
  000a2cb8: ldr r1,[0x000a2d74]
  000a2cba: add r1,pc
  000a2cbc: vldr.64 d16,[r1]
  000a2cc0: adds r1,#0x6
  000a2cc2: vst1.8 {d16},[r0]
  000a2cc6: adds r0,#0x6
  000a2cc8: vld1.16 {d17},[r1]
  000a2ccc: vst1.8 {d17},[r0]
  000a2cd0: b 0x000a2ce8
  000a2cd2: ldr r1,[0x000a2d70]
  000a2cd4: movs r2,#0x9
  000a2cd6: add r1,pc
  000a2cd8: vld1.8 {d16,d17},[r1],r2
  000a2cdc: vld1.8 {d18,d19},[r1]
  000a2ce0: vst1.8 {d16,d17},[r0],r2
  000a2ce4: vst1.8 {d18,d19},[r0]
  000a2ce8: ldr.w r0,[r10,#0x0]
  000a2cec: add r1,sp,#0x8
  000a2cee: movs r2,#0x0
  000a2cf0: movs r3,#0x0
  000a2cf2: mov.w r11,#0x0
  000a2cf6: blx 0x000714a0
  000a2cfa: ldr r5,[0x000a2d78]
  000a2cfc: movw r0,#0x23ec
  000a2d00: add.w r4,r9,r0
  000a2d04: movs r6,#0x0
  000a2d06: add r5,pc
  000a2d08: b 0x000a2d1c
  000a2d0a: str.w r11,[r4,r6]
  000a2d0e: adds r2,r4,r6
  000a2d10: ldr r1,[r5,r6]
  000a2d12: ldr.w r0,[r10,#0x0]
  000a2d16: blx 0x000714ac
  000a2d1a: adds r6,#0x4
  000a2d1c: cmp r6,#0x10
  000a2d1e: bne 0x000a2d0a
  000a2d20: movw r0,#0x2404
  000a2d24: add.w r2,r9,r0
  000a2d28: ldr.w r0,[r10,#0x0]
  000a2d2c: movs r1,#0x0
  000a2d2e: movs r4,#0x0
  000a2d30: blx 0x000714b8
  000a2d34: movw r0,#0x240c
  000a2d38: mov.w r1,#0xffffffff
  000a2d3c: str.w r4,[r9,r0]
  000a2d40: movw r0,#0x2408
  000a2d44: str.w r1,[r9,r0]
  000a2d48: strd r1,r1,[r9,#0x0]
  000a2d4c: strb.w r4,[r9,#0x8]
  000a2d50: ldr.w r0,[r7,#-0x20]
  000a2d54: ldr.w r1,[r8,#0x0]
  000a2d58: subs r0,r1,r0
  000a2d5a: itttt eq
  000a2d5c: add.eq.w sp,sp,#0x408
  000a2d60: add.eq sp,#0x4
  000a2d62: pop.eq.w {r8,r9,r10,r11}
  000a2d66: pop.eq {r4,r5,r6,r7,pc}
  000a2d68: blx 0x0006e824
```
