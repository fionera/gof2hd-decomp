# AESoundRessource::resume
elf 0x800a4 body 64
Sig: undefined __thiscall resume(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::resume(int) */

void __thiscall AbyssEngine::AESoundRessource::resume(AESoundRessource *this,int param_1)

{
  int *piVar1;
  int *piVar2;
  AESoundInfo aAStack_24 [12];
  int local_18;
  int local_14;
  
  piVar2 = *(int **)(DAT_000900e4 + 0x900b6);
  local_14 = *piVar2;
  getSoundInfo(this,param_1,aAStack_24,&local_18);
  if ((local_18 != -1) &&
     (piVar1 = *(int **)(*(int *)(this + 8) + local_18 * 4), piVar1 != (int *)0x0)) {
    (**(code **)(*piVar1 + 0x18))();
  }
  if (*piVar2 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000900a4: push {r4,r5,r7,lr}
  000900a6: add r7,sp,#0x8
  000900a8: sub sp,#0x18
  000900aa: mov r4,r0
  000900ac: ldr r0,[0x000900e4]
  000900ae: add r2,sp,#0x4
  000900b0: add r3,sp,#0x10
  000900b2: add r0,pc
  000900b4: ldr r5,[r0,#0x0]
  000900b6: ldr r0,[r5,#0x0]
  000900b8: str r0,[sp,#0x14]
  000900ba: mov r0,r4
  000900bc: blx 0x000700c0
  000900c0: ldr r0,[sp,#0x10]
  000900c2: adds r1,r0,#0x1
  000900c4: beq 0x000900d4
  000900c6: ldr r1,[r4,#0x8]
  000900c8: ldr.w r0,[r1,r0,lsl #0x2]
  000900cc: cbz r0,0x000900d4
  000900ce: ldr r1,[r0,#0x0]
  000900d0: ldr r1,[r1,#0x18]
  000900d2: blx r1
  000900d4: ldr r0,[sp,#0x14]
  000900d6: ldr r1,[r5,#0x0]
  000900d8: subs r0,r1,r0
  000900da: itt eq
  000900dc: add.eq sp,#0x18
  000900de: pop.eq {r4,r5,r7,pc}
  000900e0: blx 0x0006e824
```
