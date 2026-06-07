# Layout::drawBox
elf 0xd48f0 body 88
Sig: undefined __stdcall drawBox(int param_1, int param_2, int param_3, int param_4, int param_5, String param_6)

## decompile
```c

/* Layout::drawBox(int, int, int, int, int, AbyssEngine::String) */

void Layout::drawBox(Layout *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                    undefined4 param_5,undefined4 param_6,String *param_7)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_000e4958 + 0xe4904);
  iVar2 = *piVar3;
  uVar1 = AbyssEngine::String::String((String *)&stack0xffffffd0,param_7,false);
  drawBox(param_1,param_2,param_3,param_4,param_5,param_6,uVar1,1);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  if (*piVar3 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e48f0: push {r4,r5,r6,r7,lr}
  000e48f2: add r7,sp,#0xc
  000e48f4: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  000e48f8: mov r4,r0
  000e48fa: ldr r0,[0x000e4958]
  000e48fc: mov r6,r1
  000e48fe: ldr r1,[r7,#0x10]
  000e4900: add r0,pc
  000e4902: mov r9,r2
  000e4904: mov r8,r3
  000e4906: ldr r5,[r0,#0x0]
  000e4908: add r0,sp,#0x10
  000e490a: ldr r2,[r5,#0x0]
  000e490c: str r2,[sp,#0x1c]
  000e490e: movs r2,#0x0
  000e4910: blx 0x0006f028
  000e4914: ldrd r1,r2,[r7,#0x8]
  000e4918: movs r3,#0x1
  000e491a: strd r1,r2,[sp,#0x0]
  000e491e: strd r0,r3,[sp,#0x8]
  000e4922: mov r0,r4
  000e4924: mov r1,r6
  000e4926: mov r2,r9
  000e4928: mov r3,r8
  000e492a: blx 0x00074e84
  000e492e: add r0,sp,#0x10
  000e4930: blx 0x0006ee30
  000e4934: ldr r0,[sp,#0x1c]
  000e4936: ldr r1,[r5,#0x0]
  000e4938: subs r0,r1,r0
  000e493a: ittt eq
  000e493c: add.eq sp,#0x20
  000e493e: pop.eq.w {r8,r9,r11}
  000e4942: pop.eq {r4,r5,r6,r7,pc}
  000e4944: blx 0x0006e824
```
