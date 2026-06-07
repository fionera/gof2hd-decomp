# GlowPPShader::RenderEffect
elf 0x8d0ec body 44
Sig: undefined __thiscall RenderEffect(GlowPPShader * this, FBOContainer * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::GlowPPShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::GlowPPShader::RenderEffect(GlowPPShader *this,FBOContainer *param_1,Engine *param_2)

{
  int *piVar1;
  undefined4 local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_0009d118 + 0x9d0f8);
  local_14 = *piVar1;
  local_18 = 0;
  (**(code **)(*(int *)this + 0x1c))(this,param_1,&local_18,param_2);
  if (*piVar1 == local_14) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0009d0ec: push {r2,r3,r4,r6,r7,lr}
  0009d0ee: add r7,sp,#0x10
  0009d0f0: mov r3,r2
  0009d0f2: ldr r2,[0x0009d118]
  0009d0f4: add r2,pc
  0009d0f6: ldr r4,[r2,#0x0]
  0009d0f8: ldr r2,[r4,#0x0]
  0009d0fa: str r2,[sp,#0x4]
  0009d0fc: movs r2,#0x0
  0009d0fe: str r2,[sp,#0x0]
  0009d100: ldr r2,[r0,#0x0]
  0009d102: ldr.w r12,[r2,#0x1c]
  0009d106: mov r2,sp
  0009d108: blx r12
  0009d10a: ldr r0,[sp,#0x4]
  0009d10c: ldr r1,[r4,#0x0]
  0009d10e: subs r0,r1,r0
  0009d110: it eq
  0009d112: pop.eq {r2,r3,r4,r6,r7,pc}
  0009d114: blx 0x0006e824
```
