# _JNIEnv::CallVoidMethod
elf 0x6e558 body 52
Sig: undefined __stdcall CallVoidMethod(_jobject * param_1, _jmethodID * param_2, ...)

## decompile
```c

/* _JNIEnv::CallVoidMethod(_jobject*, _jmethodID*, ...) */

void _JNIEnv::CallVoidMethod(_jobject *param_1,_jmethodID *param_2,...)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_0007e58c + 0x7e564);
  iVar2 = *piVar3;
  (**(code **)(*(int *)param_1 + 0xf8))();
  iVar1 = *piVar3;
  iVar2 = iVar1 - iVar2;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2,iVar1);
  }
  return;
}

```

## target disasm
```
  0007e558: sub sp,#0x4
  0007e55a: push {r1,r2,r3,r4,r5,r7,lr}
  0007e55c: add r7,sp,#0x14
  0007e55e: ldr r4,[0x0007e58c]
  0007e560: add r4,pc
  0007e562: ldr r5,[r4,#0x0]
  0007e564: ldr r4,[r5,#0x0]
  0007e566: str r3,[r7,#0x8]
  0007e568: add.w r3,r7,#0x8
  0007e56c: str r4,[sp,#0x8]
  0007e56e: str r3,[sp,#0x4]
  0007e570: ldr r4,[r0,#0x0]
  0007e572: ldr.w r4,[r4,#0xf8]
  0007e576: blx r4
  0007e578: ldr r0,[sp,#0x8]
  0007e57a: ldr r1,[r5,#0x0]
  0007e57c: subs r0,r1,r0
  0007e57e: ittt eq
  0007e580: pop.eq.w {r1,r2,r3,r4,r5,r7,lr}
  0007e584: add.eq sp,#0x4
  0007e586: bx.eq lr
  0007e588: blx 0x0006e824
```
