# AESoundRessource::playLoop
elf 0x7ff54 body 86
Sig: undefined __thiscall playLoop(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::playLoop(int) */

void __thiscall AbyssEngine::AESoundRessource::playLoop(AESoundRessource *this,int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  AESoundInfo aAStack_2c [12];
  int local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0008ffac + 0x8ff6a);
  local_1c = *piVar3;
  getSoundInfo(this,param_1,aAStack_2c,&local_20);
  if (((local_20 != -1) &&
      (piVar1 = *(int **)(*(int *)(this + 8) + local_20 * 4), piVar1 != (int *)0x0)) &&
     (iVar2 = (**(code **)(*piVar1 + 0x20))(), iVar2 == 0)) {
    (**(code **)(**(int **)(*(int *)(this + 8) + local_20 * 4) + 0x10))();
  }
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008ff54: push {r4,r5,r6,r7,lr}
  0008ff56: add r7,sp,#0xc
  0008ff58: push.w r11
  0008ff5c: sub sp,#0x18
  0008ff5e: mov r4,r0
  0008ff60: ldr r0,[0x0008ffac]
  0008ff62: add r2,sp,#0x4
  0008ff64: add r3,sp,#0x10
  0008ff66: add r0,pc
  0008ff68: ldr r5,[r0,#0x0]
  0008ff6a: ldr r0,[r5,#0x0]
  0008ff6c: str r0,[sp,#0x14]
  0008ff6e: mov r0,r4
  0008ff70: blx 0x000700c0
  0008ff74: ldr r6,[sp,#0x10]
  0008ff76: adds r0,r6,#0x1
  0008ff78: beq 0x0008ff96
  0008ff7a: ldr r0,[r4,#0x8]
  0008ff7c: ldr.w r0,[r0,r6,lsl #0x2]
  0008ff80: cbz r0,0x0008ff96
  0008ff82: ldr r1,[r0,#0x0]
  0008ff84: ldr r1,[r1,#0x20]
  0008ff86: blx r1
  0008ff88: cbnz r0,0x0008ff96
  0008ff8a: ldr r0,[r4,#0x8]
  0008ff8c: ldr.w r0,[r0,r6,lsl #0x2]
  0008ff90: ldr r1,[r0,#0x0]
  0008ff92: ldr r1,[r1,#0x10]
  0008ff94: blx r1
  0008ff96: ldr r0,[sp,#0x14]
  0008ff98: ldr r1,[r5,#0x0]
  0008ff9a: subs r0,r1,r0
  0008ff9c: ittt eq
  0008ff9e: add.eq sp,#0x18
  0008ffa0: pop.eq.w r11
  0008ffa4: pop.eq {r4,r5,r6,r7,pc}
  0008ffa6: blx 0x0006e824
```
