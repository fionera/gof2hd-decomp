# AESoundRessource::playMusicLoop
elf 0x7fe88 body 86
Sig: undefined __thiscall playMusicLoop(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::playMusicLoop(int) */

void __thiscall AbyssEngine::AESoundRessource::playMusicLoop(AESoundRessource *this,int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  AESoundInfo aAStack_2c [12];
  int local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0008fee0 + 0x8fe9e);
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
  0008fe88: push {r4,r5,r6,r7,lr}
  0008fe8a: add r7,sp,#0xc
  0008fe8c: push.w r11
  0008fe90: sub sp,#0x18
  0008fe92: mov r4,r0
  0008fe94: ldr r0,[0x0008fee0]
  0008fe96: add r2,sp,#0x4
  0008fe98: add r3,sp,#0x10
  0008fe9a: add r0,pc
  0008fe9c: ldr r5,[r0,#0x0]
  0008fe9e: ldr r0,[r5,#0x0]
  0008fea0: str r0,[sp,#0x14]
  0008fea2: mov r0,r4
  0008fea4: blx 0x000700c0
  0008fea8: ldr r6,[sp,#0x10]
  0008feaa: adds r0,r6,#0x1
  0008feac: beq 0x0008feca
  0008feae: ldr r0,[r4,#0x8]
  0008feb0: ldr.w r0,[r0,r6,lsl #0x2]
  0008feb4: cbz r0,0x0008feca
  0008feb6: ldr r1,[r0,#0x0]
  0008feb8: ldr r1,[r1,#0x20]
  0008feba: blx r1
  0008febc: cbnz r0,0x0008feca
  0008febe: ldr r0,[r4,#0x8]
  0008fec0: ldr.w r0,[r0,r6,lsl #0x2]
  0008fec4: ldr r1,[r0,#0x0]
  0008fec6: ldr r1,[r1,#0x10]
  0008fec8: blx r1
  0008feca: ldr r0,[sp,#0x14]
  0008fecc: ldr r1,[r5,#0x0]
  0008fece: subs r0,r1,r0
  0008fed0: ittt eq
  0008fed2: add.eq sp,#0x18
  0008fed4: pop.eq.w r11
  0008fed8: pop.eq {r4,r5,r6,r7,pc}
  0008feda: blx 0x0006e824
```
