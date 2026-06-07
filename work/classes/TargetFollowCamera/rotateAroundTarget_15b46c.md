# TargetFollowCamera::rotateAroundTarget
elf 0x15b46c body 40
Sig: undefined __thiscall rotateAroundTarget(TargetFollowCamera * this, float param_1, float param_2, float param_3)

## decompile
```c

/* TargetFollowCamera::rotateAroundTarget(float, float, float) */

undefined8 __thiscall
TargetFollowCamera::rotateAroundTarget
          (TargetFollowCamera *this,float param_1,float param_2,float param_3)

{
  undefined4 in_r1;
  undefined4 in_r2;
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  int local_14;
  
  piVar1 = *(int **)(DAT_0016b494 + 0x16b478);
  local_14 = *piVar1;
  local_20 = in_r1;
  uStack_1c = in_r2;
  AbyssEngine::AEMath::Vector::operator=((Vector *)&this->field_50,(Vector *)&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0016b46c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0016b46e: add r7,sp,#0x18
  0016b470: ldr r4,[0x0016b494]
  0016b472: adds r0,#0x50
  0016b474: add r4,pc
  0016b476: ldr r5,[r4,#0x0]
  0016b478: ldr r4,[r5,#0x0]
  0016b47a: str r4,[sp,#0xc]
  0016b47c: stm sp,{r1,r2,r3}
  0016b480: mov r1,sp
  0016b482: blx 0x0006eb3c
  0016b486: ldr r0,[sp,#0xc]
  0016b488: ldr r1,[r5,#0x0]
  0016b48a: subs r0,r1,r0
  0016b48c: it eq
  0016b48e: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  0016b490: blx 0x0006e824
```
