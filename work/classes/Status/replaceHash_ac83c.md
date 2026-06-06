# Status::replaceHash
elf 0xac83c body 106
Sig: undefined __stdcall replaceHash(String param_1, String param_2)

## decompile
```c

/* Status::replaceHash(AbyssEngine::String, AbyssEngine::String) */

void Status::replaceHash(undefined4 param_1,undefined4 param_2,String *param_3,String *param_4)

{
  code *pcVar1;
  int *piVar2;
  undefined8 uVar3;
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000bc8c8 + 0xbc852);
  local_1c = *piVar2;
  AbyssEngine::String::String(aSStack_28,param_3,false);
  AbyssEngine::String::String(aSStack_34,param_4,false);
  uVar3 = AbyssEngine::String::String(aSStack_40,(char *)(DAT_000bc8cc + 0xbc86e),false);
  replaceHash(param_1,(int)((ulonglong)uVar3 >> 0x20),aSStack_28,aSStack_34,(int)uVar3);
  pcVar1 = *(code **)(DAT_000bc8d0 + 0xbc886);
  (*pcVar1)(aSStack_40);
  (*pcVar1)(aSStack_34);
  (*pcVar1)(aSStack_28);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000bc83c: push {r4,r5,r6,r7,lr}
  000bc83e: add r7,sp,#0xc
  000bc840: push.w r11
  000bc844: sub sp,#0x30
  000bc846: mov r4,r0
  000bc848: ldr r0,[0x000bc8c8]
  000bc84a: mov r1,r2
  000bc84c: movs r2,#0x0
  000bc84e: add r0,pc
  000bc850: mov r5,r3
  000bc852: ldr r6,[r0,#0x0]
  000bc854: ldr r0,[r6,#0x0]
  000bc856: str r0,[sp,#0x2c]
  000bc858: add r0,sp,#0x20
  000bc85a: blx 0x0006f028
  000bc85e: add r0,sp,#0x14
  000bc860: mov r1,r5
  000bc862: movs r2,#0x0
  000bc864: blx 0x0006f028
  000bc868: ldr r1,[0x000bc8cc]
  000bc86a: add r1,pc
  000bc86c: add r0,sp,#0x8
  000bc86e: movs r2,#0x0
  000bc870: blx 0x0006ee18
  000bc874: add r2,sp,#0x20
  000bc876: add r3,sp,#0x14
  000bc878: str r0,[sp,#0x0]
  000bc87a: mov r0,r4
  000bc87c: blx 0x000739b4
  000bc880: ldr r0,[0x000bc8d0]
  000bc882: add r0,pc
  000bc884: ldr r4,[r0,#0x0]
  000bc886: add r0,sp,#0x8
  000bc888: blx r4
  000bc88a: add r0,sp,#0x14
  000bc88c: blx r4
  000bc88e: add r0,sp,#0x20
  000bc890: blx r4
  000bc892: ldr r0,[sp,#0x2c]
  000bc894: ldr r1,[r6,#0x0]
  000bc896: subs r0,r1,r0
  000bc898: ittt eq
  000bc89a: add.eq sp,#0x30
  000bc89c: pop.eq.w r11
  000bc8a0: pop.eq {r4,r5,r6,r7,pc}
  000bc8a2: blx 0x0006e824
```
