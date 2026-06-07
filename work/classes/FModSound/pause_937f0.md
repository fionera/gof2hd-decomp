# FModSound::pause
elf 0x937f0 body 86
Sig: int __stdcall pause(void)

## decompile
```c

/* FModSound::pause(FMOD::Event*) */

int FModSound::pause(void)

{
  int in_r0;
  int iVar1;
  uint uVar2;
  int iVar3;
  uint *in_r1;
  int in_r2;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000a3848 + 0xa3800);
  iVar1 = *piVar4;
  uVar2 = 0;
  if ((in_r1 != (uint *)0x0) && (*(int *)(in_r0 + 0x23fc) != 0)) {
    iVar3 = FMOD::Event::getState(in_r1);
    if ((iVar3 == 0) && (in_r2 << 0x1c < 0)) {
      iVar3 = FMOD::Event::setPaused(SUB41(in_r1,0));
      uVar2 = (uint)(iVar3 == 0);
    }
    else {
      uVar2 = 0;
    }
  }
  if (*piVar4 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar2;
}

```

## target disasm
```
  000a37f0: push {r2,r3,r4,r5,r7,lr}
  000a37f2: add r7,sp,#0x10
  000a37f4: mov r4,r1
  000a37f6: mov r1,r0
  000a37f8: ldr r0,[0x000a3848]
  000a37fa: cmp r4,#0x0
  000a37fc: add r0,pc
  000a37fe: ldr r5,[r0,#0x0]
  000a3800: ldr r0,[r5,#0x0]
  000a3802: str r0,[sp,#0x4]
  000a3804: mov.w r0,#0x0
  000a3808: beq 0x000a3836
  000a380a: movw r2,#0x23fc
  000a380e: ldr r1,[r1,r2]
  000a3810: cbz r1,0x000a3836
  000a3812: mov r1,sp
  000a3814: mov r0,r4
  000a3816: blx 0x000714e8
  000a381a: cbnz r0,0x000a3824
  000a381c: ldrb.w r0,[sp,#0x0]
  000a3820: lsls r0,r0,#0x1c
  000a3822: bmi 0x000a3828
  000a3824: movs r0,#0x0
  000a3826: b 0x000a3836
  000a3828: mov r0,r4
  000a382a: movs r1,#0x1
  000a382c: blx 0x000715c0
  000a3830: clz r0,r0
  000a3834: lsrs r0,r0,#0x5
  000a3836: ldr r1,[sp,#0x4]
  000a3838: ldr r2,[r5,#0x0]
  000a383a: subs r1,r2,r1
  000a383c: itt eq
  000a383e: add.eq sp,#0x8
  000a3840: pop.eq {r4,r5,r7,pc}
  000a3842: blx 0x0006e824
```
