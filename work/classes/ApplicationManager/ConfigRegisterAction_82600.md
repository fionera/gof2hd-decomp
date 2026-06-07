# ApplicationManager::ConfigRegisterAction
elf 0x82600 body 34
Sig: undefined __stdcall ConfigRegisterAction(longlong param_1, longlong param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::ConfigRegisterAction(long long, long long) */

void AbyssEngine::ApplicationManager::ConfigRegisterAction(longlong param_1,longlong param_2)

{
  Array *pAVar1;
  undefined4 in_stack_00000000;
  undefined4 in_stack_00000004;
  
  pAVar1 = (Array *)((int)param_1 + 0x88);
  ArrayAdd<long_long>(param_2,pAVar1);
  (*(code *)(DAT_001ab604 + 0x1ab608))(in_stack_00000000,in_stack_00000004,pAVar1);
  return;
}

```

## target disasm
```
  00092600: push {r4,r6,r7,lr}
  00092602: add r7,sp,#0x8
  00092604: add.w r4,r0,#0x88
  00092608: mov r0,r2
  0009260a: mov r1,r3
  0009260c: mov r2,r4
  0009260e: blx 0x00070420
  00092612: ldrd r0,r1,[r7,#0x8]
  00092616: mov r2,r4
  00092618: pop.w {r4,r6,r7,lr}
  0009261c: b.w 0x001ab5f8
  001ab5f8: bx pc
```
