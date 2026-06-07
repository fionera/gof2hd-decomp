# Radar::update
elf 0x12e01c body 48
Sig: undefined __stdcall update(KIPlayer * param_1)

## decompile
```c

/* Radar::update(KIPlayer*) */

undefined8 Radar::update(KIPlayer *param_1)

{
  int *in_r1;
  undefined4 in_r2;
  int *piVar1;
  KIPlayer *local_20;
  int *piStack_1c;
  undefined4 uStack_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_0013e04c + 0x13e028);
  local_14 = *piVar1;
  local_20 = param_1;
  piStack_1c = in_r1;
  uStack_18 = in_r2;
  (**(code **)(*in_r1 + 0x28))(&local_20);
  update(param_1,local_20,piStack_1c,uStack_18);
  if (*piVar1 == local_14) {
    return CONCAT44(piStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0013e01c: push {r0,r1,r2,r3,r4,r5,r7,lr}
  0013e01e: add r7,sp,#0x18
  0013e020: mov r4,r0
  0013e022: ldr r0,[0x0013e04c]
  0013e024: add r0,pc
  0013e026: ldr r5,[r0,#0x0]
  0013e028: ldr r0,[r5,#0x0]
  0013e02a: str r0,[sp,#0xc]
  0013e02c: ldr r0,[r1,#0x0]
  0013e02e: ldr r2,[r0,#0x28]
  0013e030: mov r0,sp
  0013e032: blx r2
  0013e034: ldm.w sp,{r1,r2,r3}
  0013e038: mov r0,r4
  0013e03a: blx 0x00076780
  0013e03e: ldr r0,[sp,#0xc]
  0013e040: ldr r1,[r5,#0x0]
  0013e042: subs r0,r1,r0
  0013e044: it eq
  0013e046: pop.eq {r0,r1,r2,r3,r4,r5,r7,pc}
  0013e048: blx 0x0006e824
```
