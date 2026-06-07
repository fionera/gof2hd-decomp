# KIPlayer::setPosition
elf 0xa6394 body 40
Sig: undefined __thiscall setPosition(KIPlayer * this, float param_1, float param_2, float param_3)

## decompile
```c

/* KIPlayer::setPosition(float, float, float) */

undefined8 __thiscall
KIPlayer::setPosition(KIPlayer *this,float param_1,float param_2,float param_3)

{
  undefined4 in_r1;
  undefined4 in_r2;
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  int local_14;
  
  piVar1 = *(int **)(DAT_000b63bc + 0xb639e);
  local_14 = *piVar1;
  local_20 = in_r1;
  uStack_1c = in_r2;
  (**(code **)(*(int *)this + 0x44))(this,&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000b6394: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000b6396: add r7,sp,#0x18
  000b6398: ldr r4,[0x000b63bc]
  000b639a: add r4,pc
  000b639c: ldr r5,[r4,#0x0]
  000b639e: ldr r4,[r5,#0x0]
  000b63a0: str r4,[sp,#0xc]
  000b63a2: stm sp,{r1,r2,r3}
  000b63a6: ldr r1,[r0,#0x0]
  000b63a8: ldr r2,[r1,#0x44]
  000b63aa: mov r1,sp
  000b63ac: blx r2
  000b63ae: ldr r0,[sp,#0xc]
  000b63b0: ldr r1,[r5,#0x0]
  000b63b2: subs r0,r1,r0
  000b63b4: it eq
  000b63b6: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  000b63b8: blx 0x0006e824
```
