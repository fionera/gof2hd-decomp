# ApplicationManager::ConfigRegisterTokenReadFunction
elf 0x825a4 body 72
Sig: undefined __thiscall ConfigRegisterTokenReadFunction(ApplicationManager * this, String param_1, _func_void_ConfigReader_ptr_void_ptr * param_2, void * param_3)

## decompile
```c

/* AbyssEngine::ApplicationManager::ConfigRegisterTokenReadFunction(AbyssEngine::String, void
   (*)(AbyssEngine::ConfigReader*, void*), void*) */

void __thiscall
AbyssEngine::ApplicationManager::ConfigRegisterTokenReadFunction
          (ApplicationManager *this,String *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  ConfigReader *pCVar4;
  
  piVar3 = *(int **)(DAT_000925fc + 0x925b6);
  iVar2 = *piVar3;
  pCVar4 = *(ConfigReader **)(this + 0x38);
  if (pCVar4 != (ConfigReader *)0x0) {
    String::String((String *)&stack0xffffffd0,param_2,false);
    ConfigReader::RegisterTokenReadFunction(pCVar4,(String *)&stack0xffffffd0,param_3,param_4);
    String::~String((String *)&stack0xffffffd0);
  }
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
  000925a4: push {r4,r5,r6,r7,lr}
  000925a6: add r7,sp,#0xc
  000925a8: push {r4,r5,r6,r7,r8,r9,r11}
  000925ac: mov r5,r2
  000925ae: ldr r2,[0x000925fc]
  000925b0: mov r9,r3
  000925b2: add r2,pc
  000925b4: ldr r4,[r2,#0x0]
  000925b6: ldr r2,[r4,#0x0]
  000925b8: str r2,[sp,#0xc]
  000925ba: ldr r6,[r0,#0x38]
  000925bc: cbz r6,0x000925da
  000925be: mov r8,sp
  000925c0: movs r2,#0x0
  000925c2: mov r0,r8
  000925c4: blx 0x0006f028
  000925c8: mov r0,r6
  000925ca: mov r1,r8
  000925cc: mov r2,r5
  000925ce: mov r3,r9
  000925d0: blx 0x00070414
  000925d4: mov r0,sp
  000925d6: blx 0x0006ee30
  000925da: ldr r0,[sp,#0xc]
  000925dc: ldr r1,[r4,#0x0]
  000925de: subs r0,r1,r0
  000925e0: itt eq
  000925e2: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000925e6: pop.eq {r4,r5,r6,r7,pc}
  000925e8: blx 0x0006e824
```
