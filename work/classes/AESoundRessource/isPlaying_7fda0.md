# AESoundRessource::isPlaying
elf 0x7fda0 body 66
Sig: undefined __thiscall isPlaying(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::isPlaying(int) */

void __thiscall AbyssEngine::AESoundRessource::isPlaying(AESoundRessource *this,int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  AESoundInfo aAStack_24 [12];
  int local_18;
  int local_14;
  
  piVar2 = *(int **)(DAT_0008fde4 + 0x8fdb2);
  local_14 = *piVar2;
  getSoundInfo(this,param_1,aAStack_24,&local_18);
  if (local_18 == -1) {
    uVar1 = 0;
  }
  else {
    uVar1 = (**(code **)(**(int **)(*(int *)(this + 8) + local_18 * 4) + 0x20))();
  }
  if (*piVar2 != local_14) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar1);
  }
  return;
}

```

## target disasm
```
  0008fda0: push {r4,r5,r7,lr}
  0008fda2: add r7,sp,#0x8
  0008fda4: sub sp,#0x18
  0008fda6: mov r4,r0
  0008fda8: ldr r0,[0x0008fde4]
  0008fdaa: add r2,sp,#0x4
  0008fdac: add r3,sp,#0x10
  0008fdae: add r0,pc
  0008fdb0: ldr r5,[r0,#0x0]
  0008fdb2: ldr r0,[r5,#0x0]
  0008fdb4: str r0,[sp,#0x14]
  0008fdb6: mov r0,r4
  0008fdb8: blx 0x000700c0
  0008fdbc: ldr r0,[sp,#0x10]
  0008fdbe: adds r1,r0,#0x1
  0008fdc0: beq 0x0008fdd0
  0008fdc2: ldr r1,[r4,#0x8]
  0008fdc4: ldr.w r0,[r1,r0,lsl #0x2]
  0008fdc8: ldr r1,[r0,#0x0]
  0008fdca: ldr r1,[r1,#0x20]
  0008fdcc: blx r1
  0008fdce: b 0x0008fdd2
  0008fdd0: movs r0,#0x0
  0008fdd2: ldr r1,[sp,#0x14]
  0008fdd4: ldr r2,[r5,#0x0]
  0008fdd6: subs r1,r2,r1
  0008fdd8: itt eq
  0008fdda: add.eq sp,#0x18
  0008fddc: pop.eq {r4,r5,r7,pc}
  0008fdde: blx 0x0006e824
```
