# Mission::setTargetStation
elf 0x15bbb4 body 82
Sig: undefined __thiscall setTargetStation(Mission * this, int param_1)

## decompile
```c

/* Mission::setTargetStation(int) */

void __thiscall Mission::setTargetStation(Mission *this,int param_1)

{
  int *piVar1;
  int *piVar2;
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_0016bc14 + 0x16bbc8);
  piVar1 = *(int **)(DAT_0016bc18 + 0x16bbca);
  local_1c = *piVar2;
  *(int *)(this + 0x3c) = param_1;
  Galaxy::getStation(*piVar1);
  Station::getName();
  AbyssEngine::String::operator=((String *)(this + 0x40),aSStack_28);
  AbyssEngine::String::~String(aSStack_28);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016bbb4: push {r4,r5,r6,r7,lr}
  0016bbb6: add r7,sp,#0xc
  0016bbb8: push.w r11
  0016bbbc: sub sp,#0x10
  0016bbbe: mov r4,r0
  0016bbc0: ldr r0,[0x0016bc14]
  0016bbc2: ldr r2,[0x0016bc18]
  0016bbc4: add r0,pc
  0016bbc6: add r2,pc
  0016bbc8: ldr r6,[r0,#0x0]
  0016bbca: ldr r2,[r2,#0x0]
  0016bbcc: ldr r0,[r6,#0x0]
  0016bbce: str r0,[sp,#0xc]
  0016bbd0: str r1,[r4,#0x3c]
  0016bbd2: ldr r0,[r2,#0x0]
  0016bbd4: blx 0x00071c44
  0016bbd8: mov r5,sp
  0016bbda: mov r1,r0
  0016bbdc: mov r0,r5
  0016bbde: blx 0x000736a8
  0016bbe2: add.w r0,r4,#0x40
  0016bbe6: mov r1,r5
  0016bbe8: blx 0x0006f2b0
  0016bbec: mov r0,sp
  0016bbee: blx 0x0006ee30
  0016bbf2: ldr r0,[sp,#0xc]
  0016bbf4: ldr r1,[r6,#0x0]
  0016bbf6: subs r0,r1,r0
  0016bbf8: ittt eq
  0016bbfa: add.eq sp,#0x10
  0016bbfc: pop.eq.w r11
  0016bc00: pop.eq {r4,r5,r6,r7,pc}
  0016bc02: blx 0x0006e824
```
