# BloomShader::InternalInit
elf 0x8948c body 282
Sig: undefined __stdcall InternalInit(Engine * param_1)

## decompile
```c

/* AbyssEngine::BloomShader::InternalInit(AbyssEngine::Engine*) */

void AbyssEngine::BloomShader::InternalInit(Engine *param_1)

{
  FBOContainer *pFVar1;
  int *piVar2;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000995d8 + 0x994a0);
  local_1c = *piVar2;
  pFVar1 = operator_new(0x38);
  String::String(aSStack_28,(char *)(DAT_000995dc + 0x994b6),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0x34) = pFVar1;
  String::~String(aSStack_28);
  FBOContainer::Create(*(int *)(param_1 + 0x34),0x100,false,true);
  pFVar1 = operator_new(0x38);
  String::String(aSStack_34,(char *)(DAT_000995e0 + 0x994f0),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0x50) = pFVar1;
  String::~String(aSStack_34);
  FBOContainer::Create(*(int *)(param_1 + 0x50),0x100,false,true);
  pFVar1 = operator_new(0x38);
  String::String(aSStack_40,(char *)(DAT_000995e4 + 0x9952a),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0x6c) = pFVar1;
  String::~String(aSStack_40);
  FBOContainer::Create(*(int *)(param_1 + 0x6c),0x100,false,true);
  pFVar1 = operator_new(0x38);
  String::String(aSStack_4c,(char *)(DAT_000995e8 + 0x99564),false);
  FBOContainer::FBOContainer(pFVar1);
  *(FBOContainer **)(param_1 + 0x70) = pFVar1;
  String::~String(aSStack_4c);
  FBOContainer::Create(*(int *)(param_1 + 0x70),0x100,false,true);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0009948c: push {r4,r5,r6,r7,lr}
  0009948e: add r7,sp,#0xc
  00099490: push.w r8
  00099494: sub sp,#0x38
  00099496: mov r4,r0
  00099498: ldr r0,[0x000995d8]
  0009949a: mov r5,r1
  0009949c: add r0,pc
  0009949e: ldr.w r8,[r0,#0x0]
  000994a2: ldr.w r0,[r8,#0x0]
  000994a6: str r0,[sp,#0x34]
  000994a8: movs r0,#0x38
  000994aa: blx 0x0006eb24
  000994ae: ldr r1,[0x000995dc]
  000994b0: mov r6,r0
  000994b2: add r1,pc
  000994b4: add r0,sp,#0x28
  000994b6: movs r2,#0x0
  000994b8: blx 0x0006ee18
  000994bc: add r2,sp,#0x28
  000994be: mov r0,r6
  000994c0: mov r1,r5
  000994c2: blx 0x0006ee24
  000994c6: add r0,sp,#0x28
  000994c8: str r6,[r4,#0x34]
  000994ca: blx 0x0006ee30
  000994ce: ldr r0,[r4,#0x34]
  000994d0: movs r1,#0x0
  000994d2: str r1,[sp,#0x0]
  000994d4: mov.w r1,#0x100
  000994d8: mov.w r2,#0x100
  000994dc: movs r3,#0x1
  000994de: blx 0x0006ee3c
  000994e2: movs r0,#0x38
  000994e4: blx 0x0006eb24
  000994e8: ldr r1,[0x000995e0]
  000994ea: mov r6,r0
  000994ec: add r1,pc
  000994ee: add r0,sp,#0x1c
  000994f0: movs r2,#0x0
  000994f2: blx 0x0006ee18
  000994f6: add r2,sp,#0x1c
  000994f8: mov r0,r6
  000994fa: mov r1,r5
  000994fc: blx 0x0006ee24
  00099500: add r0,sp,#0x1c
  00099502: str r6,[r4,#0x50]
  00099504: blx 0x0006ee30
  00099508: ldr r0,[r4,#0x50]
  0009950a: movs r1,#0x0
  0009950c: str r1,[sp,#0x0]
  0009950e: mov.w r1,#0x100
  00099512: mov.w r2,#0x100
  00099516: movs r3,#0x1
  00099518: blx 0x0006ee3c
  0009951c: movs r0,#0x38
  0009951e: blx 0x0006eb24
  00099522: ldr r1,[0x000995e4]
  00099524: mov r6,r0
  00099526: add r1,pc
  00099528: add r0,sp,#0x10
  0009952a: movs r2,#0x0
  0009952c: blx 0x0006ee18
  00099530: add r2,sp,#0x10
  00099532: mov r0,r6
  00099534: mov r1,r5
  00099536: blx 0x0006ee24
  0009953a: add r0,sp,#0x10
  0009953c: str r6,[r4,#0x6c]
  0009953e: blx 0x0006ee30
  00099542: ldr r0,[r4,#0x6c]
  00099544: movs r1,#0x0
  00099546: str r1,[sp,#0x0]
  00099548: mov.w r1,#0x100
  0009954c: mov.w r2,#0x100
  00099550: movs r3,#0x1
  00099552: blx 0x0006ee3c
  00099556: movs r0,#0x38
  00099558: blx 0x0006eb24
  0009955c: ldr r1,[0x000995e8]
  0009955e: mov r6,r0
  00099560: add r1,pc
  00099562: add r0,sp,#0x4
  00099564: movs r2,#0x0
  00099566: blx 0x0006ee18
  0009956a: add r2,sp,#0x4
  0009956c: mov r0,r6
  0009956e: mov r1,r5
  00099570: blx 0x0006ee24
  00099574: add r0,sp,#0x4
  00099576: str r6,[r4,#0x70]
  00099578: blx 0x0006ee30
  0009957c: ldr r0,[r4,#0x70]
  0009957e: movs r1,#0x0
  00099580: str r1,[sp,#0x0]
  00099582: mov.w r1,#0x100
  00099586: mov.w r2,#0x100
  0009958a: movs r3,#0x1
  0009958c: blx 0x0006ee3c
  00099590: ldr r0,[sp,#0x34]
  00099592: ldr.w r1,[r8,#0x0]
  00099596: subs r0,r1,r0
  00099598: ittt eq
  0009959a: add.eq sp,#0x38
  0009959c: pop.eq.w r8
  000995a0: pop.eq {r4,r5,r6,r7,pc}
  000995a2: blx 0x0006e824
```
