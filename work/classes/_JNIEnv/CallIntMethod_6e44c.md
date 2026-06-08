# _JNIEnv::CallIntMethod
elf 0x6e44c body 52
Sig: undefined __stdcall CallIntMethod(_jobject * param_1, _jmethodID * param_2, ...)

## decompile
```c

/* _JNIEnv::CallIntMethod(_jobject*, _jmethodID*, ...) */

void _JNIEnv::CallIntMethod(_jobject *param_1,_jmethodID *param_2,...)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_0007e480 + 0x7e458);
  iVar3 = *piVar4;
  uVar1 = (**(code **)(*(int *)param_1 + 200))();
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
  0007e44c: sub sp,#0x4
  0007e44e: push {r1,r2,r3,r4,r5,r7,lr}
  0007e450: add r7,sp,#0x14
  0007e452: ldr r4,[0x0007e480]
  0007e454: add r4,pc
  0007e456: ldr r5,[r4,#0x0]
  0007e458: ldr r4,[r5,#0x0]
  0007e45a: str r3,[r7,#0x8]
  0007e45c: add.w r3,r7,#0x8
  0007e460: str r4,[sp,#0x8]
  0007e462: str r3,[sp,#0x4]
  0007e464: ldr r4,[r0,#0x0]
  0007e466: ldr.w r4,[r4,#0xc8]
  0007e46a: blx r4
  0007e46c: ldr r1,[sp,#0x8]
  0007e46e: ldr r2,[r5,#0x0]
  0007e470: subs r1,r2,r1
  0007e472: ittt eq
  0007e474: pop.eq.w {r1,r2,r3,r4,r5,r7,lr}
  0007e478: add.eq sp,#0x4
  0007e47a: bx.eq lr
  0007e47c: blx 0x0006e824
```
