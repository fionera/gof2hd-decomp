# Agent::setMissionString
elf 0x177684 body 68
Sig: undefined __thiscall setMissionString(Agent * this, String param_1)

## decompile
```c

/* Agent::setMissionString(AbyssEngine::String) */

void __thiscall Agent::setMissionString(Agent *this,String *param_2)

{
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_001876d8 + 0x18769a);
  local_1c = *piVar1;
  AbyssEngine::String::String(aSStack_28,param_2,false);
  AbyssEngine::String::operator=((String *)(this + 0x6c),aSStack_28);
  AbyssEngine::String::~String(aSStack_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00187684: push {r4,r5,r6,r7,lr}
  00187686: add r7,sp,#0xc
  00187688: push.w r11
  0018768c: sub sp,#0x10
  0018768e: mov r4,r0
  00187690: ldr r0,[0x001876d8]
  00187692: mov r5,sp
  00187694: movs r2,#0x0
  00187696: add r0,pc
  00187698: ldr r6,[r0,#0x0]
  0018769a: ldr r0,[r6,#0x0]
  0018769c: str r0,[sp,#0xc]
  0018769e: mov r0,r5
  001876a0: blx 0x0006f028
  001876a4: add.w r0,r4,#0x6c
  001876a8: mov r1,r5
  001876aa: blx 0x0006f2b0
  001876ae: mov r0,sp
  001876b0: blx 0x0006ee30
  001876b4: ldr r0,[sp,#0xc]
  001876b6: ldr r1,[r6,#0x0]
  001876b8: subs r0,r1,r0
  001876ba: ittt eq
  001876bc: add.eq sp,#0x10
  001876be: pop.eq.w r11
  001876c2: pop.eq {r4,r5,r6,r7,pc}
  001876c4: blx 0x0006e824
```
