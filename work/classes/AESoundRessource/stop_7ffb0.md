# AESoundRessource::stop
elf 0x7ffb0 body 64
Sig: undefined __thiscall stop(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::stop(int) */

void __thiscall AbyssEngine::AESoundRessource::stop(AESoundRessource *this,int param_1)

{
  int *piVar1;
  int *piVar2;
  AESoundInfo aAStack_24 [12];
  int local_18;
  int local_14;
  
  piVar2 = *(int **)(DAT_0008fff0 + 0x8ffc2);
  local_14 = *piVar2;
  getSoundInfo(this,param_1,aAStack_24,&local_18);
  if ((local_18 != -1) &&
     (piVar1 = *(int **)(*(int *)(this + 8) + local_18 * 4), piVar1 != (int *)0x0)) {
    (**(code **)(*piVar1 + 0x1c))();
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
  0008ffb0: push {r4,r5,r7,lr}
  0008ffb2: add r7,sp,#0x8
  0008ffb4: sub sp,#0x18
  0008ffb6: mov r4,r0
  0008ffb8: ldr r0,[0x0008fff0]
  0008ffba: add r2,sp,#0x4
  0008ffbc: add r3,sp,#0x10
  0008ffbe: add r0,pc
  0008ffc0: ldr r5,[r0,#0x0]
  0008ffc2: ldr r0,[r5,#0x0]
  0008ffc4: str r0,[sp,#0x14]
  0008ffc6: mov r0,r4
  0008ffc8: blx 0x000700c0
  0008ffcc: ldr r0,[sp,#0x10]
  0008ffce: adds r1,r0,#0x1
  0008ffd0: beq 0x0008ffe0
  0008ffd2: ldr r1,[r4,#0x8]
  0008ffd4: ldr.w r0,[r1,r0,lsl #0x2]
  0008ffd8: cbz r0,0x0008ffe0
  0008ffda: ldr r1,[r0,#0x0]
  0008ffdc: ldr r1,[r1,#0x1c]
  0008ffde: blx r1
  0008ffe0: ldr r0,[sp,#0x14]
  0008ffe2: ldr r1,[r5,#0x0]
  0008ffe4: subs r0,r1,r0
  0008ffe6: itt eq
  0008ffe8: add.eq sp,#0x18
  0008ffea: pop.eq {r4,r5,r7,pc}
  0008ffec: blx 0x0006e824
```
