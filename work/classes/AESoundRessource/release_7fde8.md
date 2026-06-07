# AESoundRessource::release
elf 0x7fde8 body 62
Sig: undefined __thiscall release(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::release(int) */

void __thiscall AbyssEngine::AESoundRessource::release(AESoundRessource *this,int param_1)

{
  int *piVar1;
  AESoundInfo aAStack_24 [12];
  int local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_0008fe28 + 0x8fdfa);
  local_14 = *piVar1;
  getSoundInfo(this,param_1,aAStack_24,&local_18);
  if (local_18 != -1) {
    (**(code **)(**(int **)(*(int *)(this + 8) + local_18 * 4) + 0x34))();
  }
  if (*piVar1 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008fde8: push {r4,r5,r7,lr}
  0008fdea: add r7,sp,#0x8
  0008fdec: sub sp,#0x18
  0008fdee: mov r4,r0
  0008fdf0: ldr r0,[0x0008fe28]
  0008fdf2: add r2,sp,#0x4
  0008fdf4: add r3,sp,#0x10
  0008fdf6: add r0,pc
  0008fdf8: ldr r5,[r0,#0x0]
  0008fdfa: ldr r0,[r5,#0x0]
  0008fdfc: str r0,[sp,#0x14]
  0008fdfe: mov r0,r4
  0008fe00: blx 0x000700c0
  0008fe04: ldr r0,[sp,#0x10]
  0008fe06: adds r1,r0,#0x1
  0008fe08: beq 0x0008fe16
  0008fe0a: ldr r1,[r4,#0x8]
  0008fe0c: ldr.w r0,[r1,r0,lsl #0x2]
  0008fe10: ldr r1,[r0,#0x0]
  0008fe12: ldr r1,[r1,#0x34]
  0008fe14: blx r1
  0008fe16: ldr r0,[sp,#0x14]
  0008fe18: ldr r1,[r5,#0x0]
  0008fe1a: subs r0,r1,r0
  0008fe1c: itt eq
  0008fe1e: add.eq sp,#0x18
  0008fe20: pop.eq {r4,r5,r7,pc}
  0008fe22: blx 0x0006e824
```
