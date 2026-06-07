# Layout::drawWindow
elf 0xd387c body 68
Sig: undefined __thiscall drawWindow(Layout * this, String param_1)

## decompile
```c

/* Layout::drawWindow(AbyssEngine::String) */

void __thiscall Layout::drawWindow(Layout *this,String *param_2)

{
  int *piVar1;
  String aSStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_000e38d0 + 0xe3892);
  local_1c = *piVar1;
  AbyssEngine::String::String(aSStack_28,param_2,false);
  drawWindow(this,aSStack_28,0);
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
  000e387c: push {r4,r5,r6,r7,lr}
  000e387e: add r7,sp,#0xc
  000e3880: push.w r11
  000e3884: sub sp,#0x10
  000e3886: mov r4,r0
  000e3888: ldr r0,[0x000e38d0]
  000e388a: mov r5,sp
  000e388c: movs r2,#0x0
  000e388e: add r0,pc
  000e3890: ldr r6,[r0,#0x0]
  000e3892: ldr r0,[r6,#0x0]
  000e3894: str r0,[sp,#0xc]
  000e3896: mov r0,r5
  000e3898: blx 0x0006f028
  000e389c: mov r0,r4
  000e389e: mov r1,r5
  000e38a0: movs r2,#0x0
  000e38a2: blx 0x00074e24
  000e38a6: mov r0,sp
  000e38a8: blx 0x0006ee30
  000e38ac: ldr r0,[sp,#0xc]
  000e38ae: ldr r1,[r6,#0x0]
  000e38b0: subs r0,r1,r0
  000e38b2: ittt eq
  000e38b4: add.eq sp,#0x10
  000e38b6: pop.eq.w r11
  000e38ba: pop.eq {r4,r5,r6,r7,pc}
  000e38bc: blx 0x0006e824
```
