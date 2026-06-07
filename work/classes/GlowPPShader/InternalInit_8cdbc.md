# GlowPPShader::InternalInit
elf 0x8cdbc body 286
Sig: undefined __stdcall InternalInit(Engine * param_1)

## decompile
```c

/* AbyssEngine::GlowPPShader::InternalInit(AbyssEngine::Engine*) */

void AbyssEngine::GlowPPShader::InternalInit(Engine *param_1)

{
  FBOContainer *pFVar1;
  int *piVar2;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_0009cf0c + 0x9cdd0);
  local_1c = *piVar2;
  pFVar1 = operator_new(0x38);
  String::String(aSStack_28,(char *)(DAT_0009cf10 + 0x9cde6),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0x3c) = pFVar1;
  String::~String(aSStack_28);
  FBOContainer::Create(*(int *)(param_1 + 0x3c),0x200,false,true);
  pFVar1 = operator_new(0x38);
  String::String(aSStack_34,(char *)(DAT_0009cf14 + 0x9ce20),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0x58) = pFVar1;
  String::~String(aSStack_34);
  FBOContainer::Create(*(int *)(param_1 + 0x58),0x200,false,true);
  pFVar1 = operator_new(0x38);
  String::String(aSStack_40,(char *)(DAT_0009cf18 + 0x9ce5a),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0x74) = pFVar1;
  String::~String(aSStack_40);
  FBOContainer::Create(*(int *)(param_1 + 0x74),0x200,false,true);
  pFVar1 = operator_new(0x38);
  String::String(aSStack_4c,(char *)(DAT_0009cf1c + 0x9ce94),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0xa0) = pFVar1;
  String::~String(aSStack_4c);
  FBOContainer::Create(*(int *)(param_1 + 0xa0),0x200,false,true);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0009cdbc: push {r4,r5,r6,r7,lr}
  0009cdbe: add r7,sp,#0xc
  0009cdc0: push.w r8
  0009cdc4: sub sp,#0x38
  0009cdc6: mov r4,r0
  0009cdc8: ldr r0,[0x0009cf0c]
  0009cdca: mov r5,r1
  0009cdcc: add r0,pc
  0009cdce: ldr.w r8,[r0,#0x0]
  0009cdd2: ldr.w r0,[r8,#0x0]
  0009cdd6: str r0,[sp,#0x34]
  0009cdd8: movs r0,#0x38
  0009cdda: blx 0x0006eb24
  0009cdde: ldr r1,[0x0009cf10]
  0009cde0: mov r6,r0
  0009cde2: add r1,pc
  0009cde4: add r0,sp,#0x28
  0009cde6: movs r2,#0x0
  0009cde8: blx 0x0006ee18
  0009cdec: add r2,sp,#0x28
  0009cdee: mov r0,r6
  0009cdf0: mov r1,r5
  0009cdf2: blx 0x0006ee24
  0009cdf6: add r0,sp,#0x28
  0009cdf8: str r6,[r4,#0x3c]
  0009cdfa: blx 0x0006ee30
  0009cdfe: ldr r0,[r4,#0x3c]
  0009ce00: movs r1,#0x0
  0009ce02: str r1,[sp,#0x0]
  0009ce04: mov.w r1,#0x200
  0009ce08: mov.w r2,#0x200
  0009ce0c: movs r3,#0x1
  0009ce0e: blx 0x0006ee3c
  0009ce12: movs r0,#0x38
  0009ce14: blx 0x0006eb24
  0009ce18: ldr r1,[0x0009cf14]
  0009ce1a: mov r6,r0
  0009ce1c: add r1,pc
  0009ce1e: add r0,sp,#0x1c
  0009ce20: movs r2,#0x0
  0009ce22: blx 0x0006ee18
  0009ce26: add r2,sp,#0x1c
  0009ce28: mov r0,r6
  0009ce2a: mov r1,r5
  0009ce2c: blx 0x0006ee24
  0009ce30: add r0,sp,#0x1c
  0009ce32: str r6,[r4,#0x58]
  0009ce34: blx 0x0006ee30
  0009ce38: ldr r0,[r4,#0x58]
  0009ce3a: movs r1,#0x0
  0009ce3c: str r1,[sp,#0x0]
  0009ce3e: mov.w r1,#0x200
  0009ce42: mov.w r2,#0x200
  0009ce46: movs r3,#0x1
  0009ce48: blx 0x0006ee3c
  0009ce4c: movs r0,#0x38
  0009ce4e: blx 0x0006eb24
  0009ce52: ldr r1,[0x0009cf18]
  0009ce54: mov r6,r0
  0009ce56: add r1,pc
  0009ce58: add r0,sp,#0x10
  0009ce5a: movs r2,#0x0
  0009ce5c: blx 0x0006ee18
  0009ce60: add r2,sp,#0x10
  0009ce62: mov r0,r6
  0009ce64: mov r1,r5
  0009ce66: blx 0x0006ee24
  0009ce6a: add r0,sp,#0x10
  0009ce6c: str r6,[r4,#0x74]
  0009ce6e: blx 0x0006ee30
  0009ce72: ldr r0,[r4,#0x74]
  0009ce74: movs r1,#0x0
  0009ce76: str r1,[sp,#0x0]
  0009ce78: mov.w r1,#0x200
  0009ce7c: mov.w r2,#0x200
  0009ce80: movs r3,#0x1
  0009ce82: blx 0x0006ee3c
  0009ce86: movs r0,#0x38
  0009ce88: blx 0x0006eb24
  0009ce8c: ldr r1,[0x0009cf1c]
  0009ce8e: mov r6,r0
  0009ce90: add r1,pc
  0009ce92: add r0,sp,#0x4
  0009ce94: movs r2,#0x0
  0009ce96: blx 0x0006ee18
  0009ce9a: add r2,sp,#0x4
  0009ce9c: mov r0,r6
  0009ce9e: mov r1,r5
  0009cea0: blx 0x0006ee24
  0009cea4: add r0,sp,#0x4
  0009cea6: str.w r6,[r4,#0xa0]
  0009ceaa: blx 0x0006ee30
  0009ceae: ldr.w r0,[r4,#0xa0]
  0009ceb2: movs r1,#0x0
  0009ceb4: str r1,[sp,#0x0]
  0009ceb6: mov.w r1,#0x200
  0009ceba: mov.w r2,#0x200
  0009cebe: movs r3,#0x1
  0009cec0: blx 0x0006ee3c
  0009cec4: ldr r0,[sp,#0x34]
  0009cec6: ldr.w r1,[r8,#0x0]
  0009ceca: subs r0,r1,r0
  0009cecc: ittt eq
  0009cece: add.eq sp,#0x38
  0009ced0: pop.eq.w r8
  0009ced4: pop.eq {r4,r5,r6,r7,pc}
  0009ced6: blx 0x0006e824
```
