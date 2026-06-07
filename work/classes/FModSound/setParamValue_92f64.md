# FModSound::setParamValue
elf 0x92f64 body 64
Sig: undefined __thiscall setParamValue(FModSound * this, char * param_1, int param_2, float param_3)

## decompile
```c

/* FModSound::setParamValue(char const*, int, float) */

void __thiscall FModSound::setParamValue(FModSound *this,char *param_1,int param_2,float param_3)

{
  int iVar1;
  int *piVar2;
  float fVar3;
  
  piVar2 = *(int **)(DAT_000a2fa4 + 0xa2f72);
  iVar1 = *piVar2;
  if (((-1 < param_2) && (*(int *)(this + 0x23fc) != 0)) &&
     (*(char **)(this + param_2 * 4 + 0x18) != (char *)0x0)) {
    fVar3 = (float)FMOD::Event::getParameter
                             (*(char **)(this + param_2 * 4 + 0x18),(EventParameter **)param_1);
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
  000a2f64: push {r2,r3,r4,r5,r7,lr}
  000a2f66: add r7,sp,#0x10
  000a2f68: mov r4,r3
  000a2f6a: ldr r3,[0x000a2fa4]
  000a2f6c: cmp r2,#0x0
  000a2f6e: add r3,pc
  000a2f70: ldr r5,[r3,#0x0]
  000a2f72: ldr r3,[r5,#0x0]
  000a2f74: str r3,[sp,#0x4]
  000a2f76: blt 0x000a2f96
  000a2f78: movw r3,#0x23fc
  000a2f7c: ldr r3,[r0,r3]
  000a2f7e: cbz r3,0x000a2f96
  000a2f80: add.w r0,r0,r2, lsl #0x2
  000a2f84: ldr r0,[r0,#0x18]
  000a2f86: cbz r0,0x000a2f96
  000a2f88: mov r2,sp
  000a2f8a: blx 0x000714f4
  000a2f8e: ldr r0,[sp,#0x0]
  000a2f90: mov r1,r4
  000a2f92: blx 0x0007150c
  000a2f96: ldr r0,[sp,#0x4]
  000a2f98: ldr r1,[r5,#0x0]
  000a2f9a: subs r0,r1,r0
  000a2f9c: it eq
  000a2f9e: pop.eq {r2,r3,r4,r5,r7,pc}
  000a2fa0: blx 0x0006e824
```
