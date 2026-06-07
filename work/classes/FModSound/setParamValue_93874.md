# FModSound::setParamValue
elf 0x93874 body 62
Sig: undefined __thiscall setParamValue(FModSound * this, Event * param_1, int param_2, float param_3)

## decompile
```c

/* FModSound::setParamValue(FMOD::Event*, int, float) */

void __thiscall FModSound::setParamValue(FModSound *this,Event *param_1,int param_2,float param_3)

{
  int iVar1;
  int *piVar2;
  float fVar3;
  
  piVar2 = *(int **)(DAT_000a38b4 + 0xa3882);
  iVar1 = *piVar2;
  if ((param_1 != (Event *)0x0) && (*(int *)(this + 0x23fc) != 0)) {
    fVar3 = (float)FMOD::Event::getParameterByIndex((int)param_1,(EventParameter **)param_2);
    param_3 = (float)FMOD::EventParameter::setValue(fVar3);
  }
  if (*piVar2 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_3);
}

```

## target disasm
```
  000a3874: push {r2,r3,r4,r5,r7,lr}
  000a3876: add r7,sp,#0x10
  000a3878: mov r4,r3
  000a387a: ldr r3,[0x000a38b4]
  000a387c: cmp r1,#0x0
  000a387e: add r3,pc
  000a3880: ldr r5,[r3,#0x0]
  000a3882: ldr r3,[r5,#0x0]
  000a3884: str r3,[sp,#0x4]
  000a3886: beq 0x000a38a4
  000a3888: movw r3,#0x23fc
  000a388c: ldr r0,[r0,r3]
  000a388e: cbz r0,0x000a38a4
  000a3890: mov r3,sp
  000a3892: mov r0,r1
  000a3894: mov r1,r2
  000a3896: mov r2,r3
  000a3898: blx 0x00071500
  000a389c: ldr r0,[sp,#0x0]
  000a389e: mov r1,r4
  000a38a0: blx 0x0007150c
  000a38a4: ldr r0,[sp,#0x4]
  000a38a6: ldr r1,[r5,#0x0]
  000a38a8: subs r0,r1,r0
  000a38aa: it eq
  000a38ac: pop.eq {r2,r3,r4,r5,r7,pc}
  000a38ae: blx 0x0006e824
```
