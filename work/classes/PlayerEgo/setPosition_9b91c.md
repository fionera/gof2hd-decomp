# PlayerEgo::setPosition
elf 0x9b91c body 40
Sig: undefined __stdcall setPosition(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerEgo::setPosition(float, float, float) */

undefined8 PlayerEgo::setPosition(float param_1,float param_2,float param_3)

{
  int in_r0;
  undefined4 in_r1;
  undefined4 in_r2;
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  int local_14;
  
  piVar1 = *(int **)(DAT_000ab944 + 0xab926);
  local_14 = *piVar1;
  local_20 = in_r1;
  uStack_1c = in_r2;
  AEGeometry::setPosition(*(AEGeometry **)(in_r0 + 8),(Vector *)&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000ab91c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  000ab91e: add r7,sp,#0x18
  000ab920: ldr r4,[0x000ab944]
  000ab922: add r4,pc
  000ab924: ldr r5,[r4,#0x0]
  000ab926: ldr r4,[r5,#0x0]
  000ab928: str r4,[sp,#0xc]
  000ab92a: ldr r0,[r0,#0x8]
  000ab92c: stm sp,{r1,r2,r3}
  000ab930: mov r1,sp
  000ab932: blx 0x00072148
  000ab936: ldr r0,[sp,#0xc]
  000ab938: ldr r1,[r5,#0x0]
  000ab93a: subs r0,r1,r0
  000ab93c: it eq
  000ab93e: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  000ab940: blx 0x0006e824
```
