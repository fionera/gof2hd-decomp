# BlurShader::RenderEffect
elf 0x8a760 body 68
Sig: undefined __stdcall RenderEffect(FBOContainer * param_1, Engine * param_2, float param_3, Vector param_4)

## decompile
```c

/* AbyssEngine::BlurShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*, float,
   AbyssEngine::AEMath::Vector) */

void AbyssEngine::BlurShader::RenderEffect
               (int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  int *piVar1;
  undefined4 local_20;
  int local_1c;
  
  piVar1 = *(int **)(DAT_0009a7a4 + 0x9a772);
  local_1c = *piVar1;
  local_20 = 0;
  (**(code **)(*param_1 + 0x20))(param_1,param_2,&local_20,param_3,param_4,param_5,param_6,param_7);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0009a760: push {r4,r5,r6,r7,lr}
  0009a762: add r7,sp,#0xc
  0009a764: push.w r11
  0009a768: sub sp,#0x18
  0009a76a: mov r12,r2
  0009a76c: ldr r2,[0x0009a7a4]
  0009a76e: add r2,pc
  0009a770: ldr r4,[r2,#0x0]
  0009a772: ldr r2,[r4,#0x0]
  0009a774: str r2,[sp,#0x14]
  0009a776: movs r2,#0x0
  0009a778: str r2,[sp,#0x10]
  0009a77a: ldr r2,[r0,#0x0]
  0009a77c: ldrd r5,lr,[r7,#0x8]
  0009a780: ldr r6,[r2,#0x20]
  0009a782: ldr r2,[r7,#0x10]
  0009a784: stm sp,{r3,r5,lr}
  0009a788: mov r3,r12
  0009a78a: str r2,[sp,#0xc]
  0009a78c: add r2,sp,#0x10
  0009a78e: blx r6
  0009a790: ldr r0,[sp,#0x14]
  0009a792: ldr r1,[r4,#0x0]
  0009a794: subs r0,r1,r0
  0009a796: ittt eq
  0009a798: add.eq sp,#0x18
  0009a79a: pop.eq.w r11
  0009a79e: pop.eq {r4,r5,r6,r7,pc}
  0009a7a0: blx 0x0006e824
```
