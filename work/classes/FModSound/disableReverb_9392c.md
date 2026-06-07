# FModSound::disableReverb
elf 0x9392c body 82
Sig: undefined __stdcall disableReverb(void)

## decompile
```c

/* FModSound::disableReverb() */

void FModSound::disableReverb(void)

{
  int in_r0;
  FMOD_REVERB_PROPERTIES *pFVar1;
  int *piVar2;
  undefined1 local_70 [84];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000a3980 + 0xa393e);
  local_1c = *piVar2;
  pFVar1 = *(FMOD_REVERB_PROPERTIES **)(in_r0 + 0x23fc);
  if (pFVar1 != (FMOD_REVERB_PROPERTIES *)0x0) {
    __aeabi_memcpy8(local_70,DAT_000a3984 + 0xa3956,0x50);
    FMOD::EventSystem::setReverbProperties(pFVar1);
  }
  *(undefined4 *)(in_r0 + 0x2408) = 0xffffffff;
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000a392c: push {r4,r5,r6,r7,lr}
  000a392e: add r7,sp,#0xc
  000a3930: push.w r8
  000a3934: sub sp,#0x58
  000a3936: mov r4,r0
  000a3938: ldr r0,[0x000a3980]
  000a393a: add r0,pc
  000a393c: ldr r6,[r0,#0x0]
  000a393e: ldr r0,[r6,#0x0]
  000a3940: str r0,[sp,#0x54]
  000a3942: movw r0,#0x23fc
  000a3946: ldr r5,[r4,r0]
  000a3948: cbz r5,0x000a3960
  000a394a: ldr r1,[0x000a3984]
  000a394c: mov r8,sp
  000a394e: mov r0,r8
  000a3950: movs r2,#0x50
  000a3952: add r1,pc
  000a3954: blx 0x000715f0
  000a3958: mov r0,r5
  000a395a: mov r1,r8
  000a395c: blx 0x000715e4
  000a3960: movw r0,#0x2408
  000a3964: mov.w r1,#0xffffffff
  000a3968: str r1,[r4,r0]
  000a396a: ldr r0,[sp,#0x54]
  000a396c: ldr r1,[r6,#0x0]
  000a396e: subs r0,r1,r0
  000a3970: ittt eq
  000a3972: add.eq sp,#0x58
  000a3974: pop.eq.w r8
  000a3978: pop.eq {r4,r5,r6,r7,pc}
  000a397a: blx 0x0006e824
```
