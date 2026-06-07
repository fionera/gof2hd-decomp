# AESoundRessource::setVolume
elf 0x7fcc4 body 76
Sig: undefined __thiscall setVolume(AESoundRessource * this, int param_1, int param_2)

## decompile
```c

/* AbyssEngine::AESoundRessource::setVolume(int, int) */

void __thiscall
AbyssEngine::AESoundRessource::setVolume(AESoundRessource *this,int param_1,int param_2)

{
  int *piVar1;
  int *piVar2;
  AESoundInfo aAStack_2c [12];
  int local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_0008fd10 + 0x8fcda);
  local_1c = *piVar2;
  getSoundInfo(this,param_1,aAStack_2c,&local_20);
  if ((local_20 != -1) &&
     (piVar1 = *(int **)(*(int *)(this + 8) + local_20 * 4), piVar1 != (int *)0x0)) {
    (**(code **)(*piVar1 + 0x28))(piVar1,param_2);
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008fcc4: push {r4,r5,r6,r7,lr}
  0008fcc6: add r7,sp,#0xc
  0008fcc8: push.w r11
  0008fccc: sub sp,#0x18
  0008fcce: mov r5,r0
  0008fcd0: ldr r0,[0x0008fd10]
  0008fcd2: mov r4,r2
  0008fcd4: add r2,sp,#0x4
  0008fcd6: add r0,pc
  0008fcd8: add r3,sp,#0x10
  0008fcda: ldr r6,[r0,#0x0]
  0008fcdc: ldr r0,[r6,#0x0]
  0008fcde: str r0,[sp,#0x14]
  0008fce0: mov r0,r5
  0008fce2: blx 0x000700c0
  0008fce6: ldr r0,[sp,#0x10]
  0008fce8: adds r1,r0,#0x1
  0008fcea: beq 0x0008fcfc
  0008fcec: ldr r1,[r5,#0x8]
  0008fcee: ldr.w r0,[r1,r0,lsl #0x2]
  0008fcf2: cbz r0,0x0008fcfc
  0008fcf4: ldr r1,[r0,#0x0]
  0008fcf6: ldr r2,[r1,#0x28]
  0008fcf8: mov r1,r4
  0008fcfa: blx r2
  0008fcfc: ldr r0,[sp,#0x14]
  0008fcfe: ldr r1,[r6,#0x0]
  0008fd00: subs r0,r1,r0
  0008fd02: ittt eq
  0008fd04: add.eq sp,#0x18
  0008fd06: pop.eq.w r11
  0008fd0a: pop.eq {r4,r5,r6,r7,pc}
  0008fd0c: blx 0x0006e824
```
