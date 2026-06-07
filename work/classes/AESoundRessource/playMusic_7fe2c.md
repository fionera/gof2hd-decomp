# AESoundRessource::playMusic
elf 0x7fe2c body 86
Sig: undefined __thiscall playMusic(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::playMusic(int) */

void __thiscall AbyssEngine::AESoundRessource::playMusic(AESoundRessource *this,int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  AESoundInfo aAStack_2c [12];
  int local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0008fe84 + 0x8fe42);
  local_1c = *piVar3;
  getSoundInfo(this,param_1,aAStack_2c,&local_20);
  if (((local_20 != -1) &&
      (piVar1 = *(int **)(*(int *)(this + 8) + local_20 * 4), piVar1 != (int *)0x0)) &&
     (iVar2 = (**(code **)(*piVar1 + 0x20))(), iVar2 == 0)) {
    (**(code **)(**(int **)(*(int *)(this + 8) + local_20 * 4) + 8))();
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
  0008fe2c: push {r4,r5,r6,r7,lr}
  0008fe2e: add r7,sp,#0xc
  0008fe30: push.w r11
  0008fe34: sub sp,#0x18
  0008fe36: mov r4,r0
  0008fe38: ldr r0,[0x0008fe84]
  0008fe3a: add r2,sp,#0x4
  0008fe3c: add r3,sp,#0x10
  0008fe3e: add r0,pc
  0008fe40: ldr r5,[r0,#0x0]
  0008fe42: ldr r0,[r5,#0x0]
  0008fe44: str r0,[sp,#0x14]
  0008fe46: mov r0,r4
  0008fe48: blx 0x000700c0
  0008fe4c: ldr r6,[sp,#0x10]
  0008fe4e: adds r0,r6,#0x1
  0008fe50: beq 0x0008fe6e
  0008fe52: ldr r0,[r4,#0x8]
  0008fe54: ldr.w r0,[r0,r6,lsl #0x2]
  0008fe58: cbz r0,0x0008fe6e
  0008fe5a: ldr r1,[r0,#0x0]
  0008fe5c: ldr r1,[r1,#0x20]
  0008fe5e: blx r1
  0008fe60: cbnz r0,0x0008fe6e
  0008fe62: ldr r0,[r4,#0x8]
  0008fe64: ldr.w r0,[r0,r6,lsl #0x2]
  0008fe68: ldr r1,[r0,#0x0]
  0008fe6a: ldr r1,[r1,#0x8]
  0008fe6c: blx r1
  0008fe6e: ldr r0,[sp,#0x14]
  0008fe70: ldr r1,[r5,#0x0]
  0008fe72: subs r0,r1,r0
  0008fe74: ittt eq
  0008fe76: add.eq sp,#0x18
  0008fe78: pop.eq.w r11
  0008fe7c: pop.eq {r4,r5,r6,r7,pc}
  0008fe7e: blx 0x0006e824
```
