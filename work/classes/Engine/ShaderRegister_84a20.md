# Engine::ShaderRegister
elf 0x84a20 body 108
Sig: undefined __thiscall ShaderRegister(Engine * this, ShaderBaseStruct * param_1)

## decompile
```c

/* AbyssEngine::Engine::ShaderRegister(AbyssEngine::ShaderBaseStruct*) */

void __thiscall AbyssEngine::Engine::ShaderRegister(Engine *this,ShaderBaseStruct *param_1)

{
  void *pvVar1;
  int *piVar2;
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_00094a9c + 0x94a36);
  local_1c = *piVar2;
  if (param_1 != (ShaderBaseStruct *)0x0) {
    ShaderBaseStruct::GetShaderName();
    pvVar1 = (void *)String::GetAEChar();
    String::~String(aSStack_28);
    (**(code **)(*(int *)param_1 + 8))(param_1,this);
    ArrayAdd<AbyssEngine::ShaderBaseStruct*>(param_1,(Array *)(this + 0x510));
    ArrayAdd<int>(0,(Array *)(this + 0x3d8));
    operator_delete__(pvVar1);
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00094a20: push {r4,r5,r6,r7,lr}
  00094a22: add r7,sp,#0xc
  00094a24: push.w r8
  00094a28: sub sp,#0x10
  00094a2a: mov r4,r0
  00094a2c: ldr r0,[0x00094a9c]
  00094a2e: mov r5,r1
  00094a30: cmp r1,#0x0
  00094a32: add r0,pc
  00094a34: ldr r6,[r0,#0x0]
  00094a36: ldr r0,[r6,#0x0]
  00094a38: str r0,[sp,#0xc]
  00094a3a: beq 0x00094a78
  00094a3c: mov r8,sp
  00094a3e: mov r1,r5
  00094a40: mov r0,r8
  00094a42: blx 0x0007048c
  00094a46: mov r0,r8
  00094a48: blx 0x0006ef68
  00094a4c: mov r8,r0
  00094a4e: mov r0,sp
  00094a50: blx 0x0006ee30
  00094a54: ldr r0,[r5,#0x0]
  00094a56: mov r1,r4
  00094a58: ldr r2,[r0,#0x8]
  00094a5a: mov r0,r5
  00094a5c: blx r2
  00094a5e: add.w r1,r4,#0x510
  00094a62: mov r0,r5
  00094a64: blx 0x00070738
  00094a68: add.w r1,r4,#0x3d8
  00094a6c: movs r0,#0x0
  00094a6e: blx 0x0007021c
  00094a72: mov r0,r8
  00094a74: blx 0x0006ebfc
  00094a78: ldr r0,[sp,#0xc]
  00094a7a: ldr r1,[r6,#0x0]
  00094a7c: subs r0,r1,r0
  00094a7e: ittt eq
  00094a80: add.eq sp,#0x10
  00094a82: pop.eq.w r8
  00094a86: pop.eq {r4,r5,r6,r7,pc}
  00094a88: blx 0x0006e824
```
