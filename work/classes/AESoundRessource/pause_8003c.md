# AESoundRessource::pause
elf 0x8003c body 64
Sig: int __thiscall pause(AESoundRessource * this)

## decompile
```c

/* AbyssEngine::AESoundRessource::pause(int) */

int __thiscall AbyssEngine::AESoundRessource::pause(AESoundRessource *this)

{
  int *piVar1;
  int in_r1;
  int *piVar2;
  AESoundInfo aAStack_24 [12];
  int local_18;
  int local_14;
  
  piVar2 = *(int **)(DAT_0009007c + 0x9004e);
  local_14 = *piVar2;
  getSoundInfo(this,in_r1,aAStack_24,&local_18);
  if ((local_18 != -1) &&
     (piVar1 = *(int **)(*(int *)(this + 8) + local_18 * 4), piVar1 != (int *)0x0)) {
    (**(code **)(*piVar1 + 0x14))();
  }
  if (*piVar2 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}

```

## target disasm
```
  0009003c: push {r4,r5,r7,lr}
  0009003e: add r7,sp,#0x8
  00090040: sub sp,#0x18
  00090042: mov r4,r0
  00090044: ldr r0,[0x0009007c]
  00090046: add r2,sp,#0x4
  00090048: add r3,sp,#0x10
  0009004a: add r0,pc
  0009004c: ldr r5,[r0,#0x0]
  0009004e: ldr r0,[r5,#0x0]
  00090050: str r0,[sp,#0x14]
  00090052: mov r0,r4
  00090054: blx 0x000700c0
  00090058: ldr r0,[sp,#0x10]
  0009005a: adds r1,r0,#0x1
  0009005c: beq 0x0009006c
  0009005e: ldr r1,[r4,#0x8]
  00090060: ldr.w r0,[r1,r0,lsl #0x2]
  00090064: cbz r0,0x0009006c
  00090066: ldr r1,[r0,#0x0]
  00090068: ldr r1,[r1,#0x14]
  0009006a: blx r1
  0009006c: ldr r0,[sp,#0x14]
  0009006e: ldr r1,[r5,#0x0]
  00090070: subs r0,r1,r0
  00090072: itt eq
  00090074: add.eq sp,#0x18
  00090076: pop.eq {r4,r5,r7,pc}
  00090078: blx 0x0006e824
```
