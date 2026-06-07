# FModSound::setParamValue
elf 0x92f20 body 64
Sig: undefined __thiscall setParamValue(FModSound * this, int param_1, int param_2, float param_3)

## decompile
```c

/* FModSound::setParamValue(int, int, float) */

void __thiscall FModSound::setParamValue(FModSound *this,int param_1,int param_2,float param_3)

{
  int iVar1;
  int *piVar2;
  float fVar3;
  
  piVar2 = *(int **)(DAT_000a2f60 + 0xa2f2e);
  iVar1 = *piVar2;
  if (((-1 < param_2) && (*(int *)(this + 0x23fc) != 0)) &&
     (*(int *)(this + param_2 * 4 + 0x18) != 0)) {
    fVar3 = (float)FMOD::Event::getParameterByIndex
                             (*(int *)(this + param_2 * 4 + 0x18),(EventParameter **)param_1);
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
  000a2f20: push {r2,r3,r4,r5,r7,lr}
  000a2f22: add r7,sp,#0x10
  000a2f24: mov r4,r3
  000a2f26: ldr r3,[0x000a2f60]
  000a2f28: cmp r2,#0x0
  000a2f2a: add r3,pc
  000a2f2c: ldr r5,[r3,#0x0]
  000a2f2e: ldr r3,[r5,#0x0]
  000a2f30: str r3,[sp,#0x4]
  000a2f32: blt 0x000a2f52
  000a2f34: movw r3,#0x23fc
  000a2f38: ldr r3,[r0,r3]
  000a2f3a: cbz r3,0x000a2f52
  000a2f3c: add.w r0,r0,r2, lsl #0x2
  000a2f40: ldr r0,[r0,#0x18]
  000a2f42: cbz r0,0x000a2f52
  000a2f44: mov r2,sp
  000a2f46: blx 0x00071500
  000a2f4a: ldr r0,[sp,#0x0]
  000a2f4c: mov r1,r4
  000a2f4e: blx 0x0007150c
  000a2f52: ldr r0,[sp,#0x4]
  000a2f54: ldr r1,[r5,#0x0]
  000a2f56: subs r0,r1,r0
  000a2f58: it eq
  000a2f5a: pop.eq {r2,r3,r4,r5,r7,pc}
  000a2f5c: blx 0x0006e824
```
