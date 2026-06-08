# _JNIEnv::CallStaticIntMethod
elf 0x69dd0 body 52
Sig: undefined __stdcall CallStaticIntMethod(_jclass * param_1, _jmethodID * param_2, ...)

## decompile
```c

/* _JNIEnv::CallStaticIntMethod(_jclass*, _jmethodID*, ...) */

void _JNIEnv::CallStaticIntMethod(_jclass *param_1,_jmethodID *param_2,...)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00079e04 + 0x79ddc);
  iVar3 = *piVar4;
  uVar1 = (**(code **)(*(int *)param_1 + 0x208))();
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
  00079dd0: sub sp,#0x4
  00079dd2: push {r1,r2,r3,r4,r5,r7,lr}
  00079dd4: add r7,sp,#0x14
  00079dd6: ldr r4,[0x00079e04]
  00079dd8: add r4,pc
  00079dda: ldr r5,[r4,#0x0]
  00079ddc: ldr r4,[r5,#0x0]
  00079dde: str r3,[r7,#0x8]
  00079de0: add.w r3,r7,#0x8
  00079de4: str r4,[sp,#0x8]
  00079de6: str r3,[sp,#0x4]
  00079de8: ldr r4,[r0,#0x0]
  00079dea: ldr.w r4,[r4,#0x208]
  00079dee: blx r4
  00079df0: ldr r1,[sp,#0x8]
  00079df2: ldr r2,[r5,#0x0]
  00079df4: subs r1,r2,r1
  00079df6: ittt eq
  00079df8: pop.eq.w {r1,r2,r3,r4,r5,r7,lr}
  00079dfc: add.eq sp,#0x4
  00079dfe: bx.eq lr
  00079e00: blx 0x0006e824
```
