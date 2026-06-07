# Layout::drawHeader
elf 0xd4200 body 68
Sig: undefined __stdcall drawHeader(String param_1)

## decompile
```c

/* Layout::drawHeader(AbyssEngine::String) */

void Layout::drawHeader(undefined4 param_1,String *param_2)

{
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_000e4254 + 0xe4216);
  local_1c = *piVar1;
  AbyssEngine::String::String(aSStack_28,param_2,false);
  drawHeader(param_1,aSStack_28,1);
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
  000e4200: push {r4,r5,r6,r7,lr}
  000e4202: add r7,sp,#0xc
  000e4204: push.w r11
  000e4208: sub sp,#0x10
  000e420a: mov r4,r0
  000e420c: ldr r0,[0x000e4254]
  000e420e: mov r5,sp
  000e4210: movs r2,#0x0
  000e4212: add r0,pc
  000e4214: ldr r6,[r0,#0x0]
  000e4216: ldr r0,[r6,#0x0]
  000e4218: str r0,[sp,#0xc]
  000e421a: mov r0,r5
  000e421c: blx 0x0006f028
  000e4220: mov r0,r4
  000e4222: mov r1,r5
  000e4224: movs r2,#0x1
  000e4226: blx 0x00074e6c
  000e422a: mov r0,sp
  000e422c: blx 0x0006ee30
  000e4230: ldr r0,[sp,#0xc]
  000e4232: ldr r1,[r6,#0x0]
  000e4234: subs r0,r1,r0
  000e4236: ittt eq
  000e4238: add.eq sp,#0x10
  000e423a: pop.eq.w r11
  000e423e: pop.eq {r4,r5,r6,r7,pc}
  000e4240: blx 0x0006e824
```
