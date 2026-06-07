# FModSound::enableReverb
elf 0x938b8 body 110
Sig: undefined __stdcall enableReverb(int param_1)

## decompile
```c

/* FModSound::enableReverb(int) */

void FModSound::enableReverb(int param_1)

{
  int iVar1;
  FMOD_REVERB_PROPERTIES *in_r1;
  int *piVar2;
  char *apcStack_70 [20];
  int local_20;
  int local_1c;
  
  piVar2 = *(int **)(DAT_000a3928 + 0xa38d0);
  local_1c = *piVar2;
  if ((((*(int **)(param_1 + 0x23fc) != (int *)0x0) &&
       (iVar1 = FMOD::EventSystem::getNumReverbPresets(*(int **)(param_1 + 0x23fc)), iVar1 == 0)) &&
      ((int)in_r1 < local_20)) && (*(FMOD_REVERB_PROPERTIES **)(param_1 + 0x2408) != in_r1)) {
    *(FMOD_REVERB_PROPERTIES **)(param_1 + 0x2408) = in_r1;
    iVar1 = FMOD::EventSystem::getReverbPresetByIndex(*(int *)(param_1 + 0x23fc),in_r1,apcStack_70);
    if (iVar1 == 0) {
      FMOD::EventSystem::setReverbProperties(*(FMOD_REVERB_PROPERTIES **)(param_1 + 0x23fc));
    }
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
  000a38b8: push {r4,r5,r6,r7,lr}
  000a38ba: add r7,sp,#0xc
  000a38bc: push.w r8
  000a38c0: sub sp,#0x58
  000a38c2: mov r5,r0
  000a38c4: ldr r0,[0x000a3928]
  000a38c6: movw r8,#0x23fc
  000a38ca: mov r4,r1
  000a38cc: add r0,pc
  000a38ce: ldr r6,[r0,#0x0]
  000a38d0: ldr r0,[r6,#0x0]
  000a38d2: str r0,[sp,#0x54]
  000a38d4: ldr.w r0,[r5,r8]
  000a38d8: cbz r0,0x000a3912
  000a38da: add r1,sp,#0x50
  000a38dc: blx 0x000715cc
  000a38e0: cbnz r0,0x000a3912
  000a38e2: ldr r0,[sp,#0x50]
  000a38e4: cmp r0,r4
  000a38e6: ble 0x000a3912
  000a38e8: movw r0,#0x2408
  000a38ec: ldr r1,[r5,r0]
  000a38ee: cmp r1,r4
  000a38f0: beq 0x000a3912
  000a38f2: add r0,r5
  000a38f4: add r8,r5
  000a38f6: mov r2,sp
  000a38f8: mov r1,r4
  000a38fa: str r4,[r0,#0x0]
  000a38fc: movs r3,#0x0
  000a38fe: ldr.w r0,[r8,#0x0]
  000a3902: blx 0x000715d8
  000a3906: cbnz r0,0x000a3912
  000a3908: ldr.w r0,[r8,#0x0]
  000a390c: mov r1,sp
  000a390e: blx 0x000715e4
  000a3912: ldr r0,[sp,#0x54]
  000a3914: ldr r1,[r6,#0x0]
  000a3916: subs r0,r1,r0
  000a3918: ittt eq
  000a391a: add.eq sp,#0x58
  000a391c: pop.eq.w r8
  000a3920: pop.eq {r4,r5,r6,r7,pc}
  000a3922: blx 0x0006e824
```
