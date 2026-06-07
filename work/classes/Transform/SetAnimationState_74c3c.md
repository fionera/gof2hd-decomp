# Transform::SetAnimationState
elf 0x74c3c body 8
Sig: undefined __stdcall SetAnimationState(AnimationMode param_1, void * param_2)

## decompile
```c

/* AbyssEngine::Transform::SetAnimationState(AbyssEngine::AnimationMode, void*) */

void AbyssEngine::Transform::SetAnimationState
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined4 unaff_r6;
  undefined4 unaff_r8;
  undefined4 in_r12;
  bool in_CY;
  
  if (in_CY) {
    *(undefined **)(param_4 + -4) = &UNK_00084c44;
    *(BADSPACEBASE **)(param_4 + -8) = register0x00000054;
    *(undefined4 *)(param_4 + -0xc) = in_r12;
    *(undefined4 *)(param_4 + -0x10) = unaff_r8;
    *(undefined4 *)(param_4 + -0x14) = unaff_r6;
    *(int *)(param_4 + -0x18) = param_4;
    *(undefined4 *)(param_4 + -0x1c) = param_1;
  }
  return;
}

```

## target disasm
```
  00084c3c: stmdbcs r3,{r0,r3,r6,r8,r12,sp,pc}
  00084c40: ldmib r0,{r1,r3,r8,r12,lr,pc}^
```
