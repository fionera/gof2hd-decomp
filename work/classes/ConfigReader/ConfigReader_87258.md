# ConfigReader::ConfigReader
elf 0x87258 body 150
Sig: undefined __thiscall ConfigReader(ConfigReader * this, Engine * param_1)

## decompile
```c

/* AbyssEngine::ConfigReader::ConfigReader(AbyssEngine::Engine*) */

void __thiscall AbyssEngine::ConfigReader::ConfigReader(ConfigReader *this,Engine *param_1)

{
  int iVar1;
  int *piVar2;
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_00097314 + 0x9726c);
  local_1c = *piVar2;
  Array<AbyssEngine::TokenStruct*>::Array((Array<AbyssEngine::TokenStruct*> *)this);
  iVar1 = DAT_00097318;
  *(Engine **)(this + 0xc) = param_1;
  String::String(aSStack_28,(char *)(iVar1 + 0x9727e),false);
  RegisterTokenReadFunction(this,aSStack_28,DAT_0009731c + 0x9728a,param_1);
  String::~String(aSStack_28);
  String::String(aSStack_34,(char *)(DAT_00097320 + 0x9729e),false);
  RegisterTokenReadFunction(this,aSStack_34,DAT_00097324 + 0x972aa,param_1);
  String::~String(aSStack_34);
  String::String(aSStack_40,(char *)(DAT_00097328 + 0x972be),false);
  RegisterTokenReadFunction(this,aSStack_40,DAT_0009732c + 0x972ca,param_1);
  String::~String(aSStack_40);
  iVar1 = *piVar2 - local_1c;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  00097258: push {r4,r5,r6,r7,lr}
  0009725a: add r7,sp,#0xc
  0009725c: push.w r11
  00097260: sub sp,#0x28
  00097262: mov r4,r0
  00097264: ldr r0,[0x00097314]
  00097266: mov r5,r1
  00097268: add r0,pc
  0009726a: ldr r6,[r0,#0x0]
  0009726c: ldr r0,[r6,#0x0]
  0009726e: str r0,[sp,#0x24]
  00097270: mov r0,r4
  00097272: blx 0x00070888
  00097276: ldr r1,[0x00097318]
  00097278: str r5,[r4,#0xc]
  0009727a: add r1,pc
  0009727c: add r0,sp,#0x18
  0009727e: movs r2,#0x0
  00097280: blx 0x0006ee18
  00097284: ldr r2,[0x0009731c]
  00097286: add r2,pc
  00097288: add r1,sp,#0x18
  0009728a: mov r0,r4
  0009728c: mov r3,r5
  0009728e: blx 0x00070414
  00097292: add r0,sp,#0x18
  00097294: blx 0x0006ee30
  00097298: ldr r1,[0x00097320]
  0009729a: add r1,pc
  0009729c: add r0,sp,#0xc
  0009729e: movs r2,#0x0
  000972a0: blx 0x0006ee18
  000972a4: ldr r2,[0x00097324]
  000972a6: add r2,pc
  000972a8: add r1,sp,#0xc
  000972aa: mov r0,r4
  000972ac: mov r3,r5
  000972ae: blx 0x00070414
  000972b2: add r0,sp,#0xc
  000972b4: blx 0x0006ee30
  000972b8: ldr r1,[0x00097328]
  000972ba: add r1,pc
  000972bc: mov r0,sp
  000972be: movs r2,#0x0
  000972c0: blx 0x0006ee18
  000972c4: ldr r2,[0x0009732c]
  000972c6: add r2,pc
  000972c8: mov r1,sp
  000972ca: mov r0,r4
  000972cc: mov r3,r5
  000972ce: blx 0x00070414
  000972d2: mov r0,sp
  000972d4: blx 0x0006ee30
  000972d8: ldr r0,[sp,#0x24]
  000972da: ldr r1,[r6,#0x0]
  000972dc: subs r0,r1,r0
  000972de: itttt eq
  000972e0: mov.eq r0,r4
  000972e2: add.eq sp,#0x28
  000972e4: pop.eq.w r11
  000972e8: pop.eq {r4,r5,r6,r7,pc}
  000972ea: blx 0x0006e824
```
