# Engine::SetEyePosition
elf 0x850c0 body 42
Sig: undefined __thiscall SetEyePosition(Engine * this, float param_1, float param_2, float param_3)

## decompile
```c

/* AbyssEngine::Engine::SetEyePosition(float, float, float) */

undefined8 __thiscall
AbyssEngine::Engine::SetEyePosition(Engine *this,float param_1,float param_2,float param_3)

{
  undefined4 in_r1;
  undefined4 in_r2;
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  int local_14;
  
  piVar1 = *(int **)(DAT_000950ec + 0x950ce);
  local_14 = *piVar1;
  local_20 = in_r1;
  uStack_1c = in_r2;
  AEMath::Vector::operator=((Vector *)(this + 0x3fc),(Vector *)&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000950c0: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000950c2: add r7,sp,#0x18
  000950c4: ldr r4,[0x000950ec]
  000950c6: add.w r0,r0,#0x3fc
  000950ca: add r4,pc
  000950cc: ldr r5,[r4,#0x0]
  000950ce: ldr r4,[r5,#0x0]
  000950d0: str r4,[sp,#0xc]
  000950d2: stm sp,{r1,r2,r3}
  000950d6: mov r1,sp
  000950d8: blx 0x0006eb3c
  000950dc: ldr r0,[sp,#0xc]
  000950de: ldr r1,[r5,#0x0]
  000950e0: subs r0,r1,r0
  000950e2: it eq
  000950e4: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  000950e6: blx 0x0006e824
```
