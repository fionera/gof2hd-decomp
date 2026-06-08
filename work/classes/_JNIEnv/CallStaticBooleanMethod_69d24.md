# _JNIEnv::CallStaticBooleanMethod
elf 0x69d24 body 52
Sig: undefined __stdcall CallStaticBooleanMethod(_jclass * param_1, _jmethodID * param_2, ...)

## decompile
```c

/* _JNIEnv::CallStaticBooleanMethod(_jclass*, _jmethodID*, ...) */

void _JNIEnv::CallStaticBooleanMethod(_jclass *param_1,_jmethodID *param_2,...)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079d58 + 0x79d30);
  iVar3 = *piVar4;
  uVar1 = (**(code **)(*(int *)param_1 + 0x1d8))();
  iVar2 = *piVar4;
  iVar3 = iVar2 - iVar3;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar1,iVar3,iVar2);
  }
  return;
}

```

## target disasm
```
  00079d24: sub sp,#0x4
  00079d26: push {r1,r2,r3,r4,r5,r7,lr}
  00079d28: add r7,sp,#0x14
  00079d2a: ldr r4,[0x00079d58]
  00079d2c: add r4,pc
  00079d2e: ldr r5,[r4,#0x0]
  00079d30: ldr r4,[r5,#0x0]
  00079d32: str r3,[r7,#0x8]
  00079d34: add.w r3,r7,#0x8
  00079d38: str r4,[sp,#0x8]
  00079d3a: str r3,[sp,#0x4]
  00079d3c: ldr r4,[r0,#0x0]
  00079d3e: ldr.w r4,[r4,#0x1d8]
  00079d42: blx r4
  00079d44: ldr r1,[sp,#0x8]
  00079d46: ldr r2,[r5,#0x0]
  00079d48: subs r1,r2,r1
  00079d4a: ittt eq
  00079d4c: pop.eq.w {r1,r2,r3,r4,r5,r7,lr}
  00079d50: add.eq sp,#0x4
  00079d52: bx.eq lr
  00079d54: blx 0x0006e824
```
